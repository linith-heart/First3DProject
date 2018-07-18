//=============================================================================
//
// 方向矢印の処理 [waypoint.cpp]
// Author : 川井一生
/*
waypoint.cpp プレイヤーから目的地の方向を教えてくれる矢印

displays the direction from player to objective
*/
//=============================================================================

//===============================================
// チャラ	character
#include "player.h"
//===============================================
// 環境		environment
#include "camera.h"

//===============================================
// UI
#include "waypoint.h"
//===============================================
// その他	etc


//=============================================================================
// マクロ定義
#define TEXTURE			("data/TEXTURE/waypoint000.png")
#define MODEL			("data/MODEL/waypoint001.x")
#define SCL_VALUE		(1.0f)

//=============================================================================
// プロトタイプ宣言

//=============================================================================
// グローバル変数
LPDIRECT3DTEXTURE9		D3DTextureWayp = NULL;	// テクスチャへのポインタ

WAYP		waypWk;

//=============================================================================
// 方向矢印の初期化処理
HRESULT InitWaypoint(void)
{
	WAYP *wayp = &waypWk;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	wayp->use = false;

	// 位置・回転・スケールの初期設定
	wayp->Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	wayp->Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	wayp->Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);

	// モデル関係の初期化
	//D3DXCreateTextureFromFile(pDevice, TEXTURE, &D3DTextureWayp);
	D3DTextureWayp = NULL;
	wayp->Mesh = NULL;
	wayp->BuffMat = NULL;
	wayp->NumMat = 0;

	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(MODEL,			// モデルデータ
		D3DXMESH_SYSTEMMEM,				// 使用するメモリのオプション
		pDevice,						// デバイス 
		NULL,							// 未使用
		&wayp->BuffMat,				// マテリアルデータへのポインター
		NULL,							// 未使用
		&wayp->NumMat,				// D3DXMATERIAL構造体の数
		&wayp->Mesh)))				// メッシュデータへのポインター
	{
		return E_FAIL;
	}
	return S_OK;
}

//=============================================================================
// 方向矢印の終了処理
void UninitWyapoint(void)
{
	WAYP *wayp = &waypWk;

	if (D3DTextureWayp != NULL)
	{// テクスチャの開放
		D3DTextureWayp->Release();
		D3DTextureWayp = NULL;
	}

	if (wayp->VtxBuff != NULL)
	{// 頂点バッファの開放
		wayp->VtxBuff->Release();
		wayp->VtxBuff = NULL;
	}

}

//=============================================================================
// 方向矢印の最新処理
void UpdateWaypoint(void)
{

}

//=============================================================================
// 方向矢印の描画処理
void DrawWaypoint(void)
{
	WAYP *wayp = &waypWk;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// スケール、回転、平行移動
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	int i, j;

	if (wayp->use == true)
	{
		// ワールドマトリクス初期化
		D3DXMatrixIdentity(&wayp->mtxWorld);

		// スケールを反映	(S)
		D3DXMatrixScaling(&mtxScl, wayp->Scl.x, wayp->Scl.y, wayp->Scl.z);
		D3DXMatrixMultiply(&wayp->mtxWorld, &wayp->mtxWorld, &mtxScl); 	// &mesh->mtxWorld = &mesh->mtxWorld * &mtxScl
																					// 回転を反映		(R)
		D3DXMatrixRotationYawPitchRoll(&mtxRot, wayp->Rot.y, wayp->Rot.x, wayp->Rot.z);
		D3DXMatrixMultiply(&wayp->mtxWorld, &wayp->mtxWorld, &mtxRot);	// &mesh->mtxWorld = &mesh->mtxWorld * &mtxRot
																					// 平行移動を反映	(T)
		D3DXMatrixTranslation(&mtxTranslate, wayp->Pos.x, wayp->Pos.y, wayp->Pos.z);
		D3DXMatrixMultiply(&wayp->mtxWorld, &wayp->mtxWorld, &mtxTranslate);

		// ワールドマトリックス設定
		pDevice->SetTransform(D3DTS_WORLD, &wayp->mtxWorld);

		// 現在のマテリアルを保存
		pDevice->GetMaterial(&matDef);

		// マテリアル情報へのポインタを
		pD3DXMat = (D3DXMATERIAL*)wayp->BuffMat->GetBufferPointer();


		for (j = 0; j < (int)wayp->NumMat; j++)
		{
			pDevice->SetMaterial(&pD3DXMat[j].MatD3D);	// マテリアルの設定

			pDevice->SetTexture(0, D3DTextureWayp);	// テクスチャの設定

			wayp->Mesh->DrawSubset(j);
		}

		// マテリアルに戻す
		pDevice->SetMaterial(&matDef);
	}
}

//=============================================================================
// 方向矢印のゲット関数
WAYP *GetWaypoint(void)
{
	return(&waypWk);
}