//=============================================================================
//
// 木の処理 [stage.cpp]
// Author : 川井一生
//
//=============================================================================
#include "stage.h"

#include "camera.h"
#include "player.h"

#include "checkhit.h"
/*
メシュの木の処理

mesh stage
*/

//=============================================================================
// マクロ定義
#define TEXTURE_STAGE	("data/TEXTURE/stage000.png")					// 台のテクスチャー
#define MODEL			("data/MODEL/stage000.x")						// 台のモデル
#define MODELHIT		("data/MODEL/stage_hitbox000.x")				// 台の当たり判定用のモデル
#define	VALUE_MOVE		(0.5f)											// 移動量
#define	ROT_VALUE		(D3DX_PI * 0.02f)								// 回転量
#define SCL_VALUE		(1.0f)											// 大きさ
#define RANGE_VALUE		(50)											// 箱からの距離

#define	ALPHA			(170)


//=============================================================================
// プロトタイプ宣言
void MoveStage(void);

void SetStagePos(int no);
//=============================================================================
// グローバル変数
LPDIRECT3DTEXTURE9		D3DTextureStage = NULL;		// テクスチャへのポインタ

STAGE					stageWk[STAGE_MAX];			// フィールドへのポインタ


//=============================================================================
// 初期化処理
HRESULT InitStage(void)
{
	STAGE *stage = &stageWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの設定
	D3DXCreateTextureFromFile(pDevice, TEXTURE_STAGE, &D3DTextureStage);

	int i;
	for (i = 0; i < STAGE_MAX; i++, stage++)
	{
		stage->mesh.use = true;

		// 位置・回転・スケールの初期設定
		SetStagePos(i);
		stage->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);

		// モデル関係の初期化
		stage->mesh.VtxBuff = NULL;
		//MakeVertexStage(pDevice);
		stage->mesh.Mesh = NULL;
		stage->mesh.BuffMat = NULL;
		stage->mesh.NumMat = 0;

		if (i % COMP4STAGE == 0)
		{
			stage->isHitbox = false;
			// Xファイルの読み込み
			if (FAILED(D3DXLoadMeshFromX(MODEL,			// モデルデータ
				D3DXMESH_SYSTEMMEM,					// 使用するメモリのオプション
				pDevice,							// デバイス 
				NULL,								// 未使用
				&stage->mesh.BuffMat,				// マテリアルデータへのポインター
				NULL,								// 未使用
				&stage->mesh.NumMat,				// D3DXMATERIAL構造体の数
				&stage->mesh.Mesh)))				// メッシュデータへのポインター
			{
				return E_FAIL;
			}
		}
		else if (i % COMP4STAGE == 1)
		{
			stage->isHitbox = true;
			// Xファイルの読み込み
			if (FAILED(D3DXLoadMeshFromX(MODELHIT,			// モデルデータ
				D3DXMESH_SYSTEMMEM,					// 使用するメモリのオプション
				pDevice,							// デバイス 
				NULL,								// 未使用
				&stage->mesh.BuffMat,				// マテリアルデータへのポインター
				NULL,								// 未使用
				&stage->mesh.NumMat,				// D3DXMATERIAL構造体の数
				&stage->mesh.Mesh)))				// メッシュデータへのポインター
			{
				return E_FAIL;
			}

			SetVtxData(&stage->mesh);

		}

	}
	return S_OK;

}

//=============================================================================
// 終了処理
void UninitStage(void)
{
	STAGE *stage = &stageWk[0];
	if (D3DTextureStage != NULL)
	{// テクスチャの開放
		D3DTextureStage->Release();
		D3DTextureStage = NULL;
	}

	for (int i = 0; i < STAGE_MAX; i++, stage++)
	{
		if (stage->mesh.VtxBuff != NULL)
		{// 頂点バッファの開放
			stage->mesh.VtxBuff->Release();
			stage->mesh.VtxBuff = NULL;
		}

		if (stage->mesh.Mesh != NULL)
		{// メシュの解放
			stage->mesh.Mesh->Release();
			stage->mesh.Mesh = NULL;
		}

		if (stage->mesh.IdxBuff != NULL)
		{// インデックスバッファとヴァーテックスポスは一緒に開放
			free(stage->mesh.IdxBuff);
			free(stage->mesh.VtxPos);
			stage->mesh.IdxBuff = NULL;
			stage->mesh.VtxPos = NULL;
		}
	}
}

//=============================================================================
// 更新処理
void UpdateStage(void)
{
	STAGE *stage = &stageWk[0];
	PLAYER *player = GetPlayer(0);
	D3DXVECTOR3 Vel = { 0.0f,0.0f,0.0f };
	int i;
	for (i = 0; i < STAGE_MAX; i++, stage++)
	{


	}
}

//=============================================================================
// 描画処理
void DrawStage(void)
{
	STAGE *stage = &stageWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// スケール、回転、平行移動
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	int i, j;



	for (i = 0; i < STAGE_MAX; i++, stage++)
		if (stage->mesh.use == true)
		{

			// ワールドマトリクス初期化
			D3DXMatrixIdentity(&stage->mesh.mtxWorld);

			// スケールを反映	(S)
			D3DXMatrixScaling(&mtxScl, stage->mesh.Scl.x, stage->mesh.Scl.y, stage->mesh.Scl.z);
			D3DXMatrixMultiply(&stage->mesh.mtxWorld, &stage->mesh.mtxWorld, &mtxScl); 	// &mesh->mtxWorld = &mesh->mtxWorld * &mtxScl
																						// 回転を反映		(R)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, stage->mesh.Rot.y, stage->mesh.Rot.x, stage->mesh.Rot.z);
			D3DXMatrixMultiply(&stage->mesh.mtxWorld, &stage->mesh.mtxWorld, &mtxRot);	// &mesh->mtxWorld = &mesh->mtxWorld * &mtxRot
																						// 平行移動を反映	(T)
			D3DXMatrixTranslation(&mtxTranslate, stage->mesh.Pos.x, stage->mesh.Pos.y, stage->mesh.Pos.z);
			D3DXMatrixMultiply(&stage->mesh.mtxWorld, &stage->mesh.mtxWorld, &mtxTranslate);

			// ワールドマトリックス設定
			pDevice->SetTransform(D3DTS_WORLD, &stage->mesh.mtxWorld);

			// 現在のマテリアルを保存
			pDevice->GetMaterial(&matDef);

			// マテリアル情報へのポインタを
			pD3DXMat = (D3DXMATERIAL*)stage->mesh.BuffMat->GetBufferPointer();

			if (!stage->isHitbox)
				for (j = 0; j < (int)stage->mesh.NumMat; j++)
				{
					pDevice->SetMaterial(&pD3DXMat[j].MatD3D);	// マテリアルの設定

					pDevice->SetTexture(0, D3DTextureStage);	// テクスチャの設定
					stage->mesh.Mesh->DrawSubset(j);
				}

			// マテリアルに戻す
			pDevice->SetMaterial(&matDef);
		}

}

//=============================================================================
// 木の移動
void MoveStage(void)
{
	STAGE *stage = &stageWk[0];
	PLAYER *player = GetPlayer(0);

}

void SetStagePos(int no)
{
	STAGE *stage = &stageWk[no];

	switch (no)
	{
	case 0:	case 1:

		stage->mesh.Pos = D3DXVECTOR3(280.0f, 400.0f, -2130.0f);
		stage->mesh.Rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
		break;
	case 2:	case 3:
		stage->mesh.Pos = D3DXVECTOR3(450.0f, 25.0f, 2200.0f);
		stage->mesh.Rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.2f, 0.0f);
		break;
	case 4:	case 5:
		stage->mesh.Pos = D3DXVECTOR3(700.0f, 25.0f, 1800.0f);
		stage->mesh.Rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
		break;
	case 6:	case 7:
		stage->mesh.Pos = D3DXVECTOR3(280.0f, 400.0f, -2130.0f);
		stage->mesh.Rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
		break;
	}

}

//=============================================================================
// 箱のゲット関数
STAGE *GetStage(int no)
{
	return (&stageWk[no]);
}






//int hitCheckStage(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
//{
//	STAGE *stage = &stageWk[0];
//	int				ans = 0;	// 一先ず当たっていないをセットしておく
//
//	D3DXVECTOR3		pos[3];		// ポリゴンの法線
//	D3DXVECTOR3		vel = { 0.0f,0.0f,0.0f };
//
//
//	int PolyCnt = stage->mesh.Mesh->GetNumFaces();
//
//	for (int i = 0; i<PolyCnt; i++)
//	{	// まずは、ポリゴンの頂点を求める
//		int idx0 = stage->mesh.IdxBuff[i * 3];
//		int idx1 = stage->mesh.IdxBuff[i * 3 + 1];
//		int idx2 = stage->mesh.IdxBuff[i * 3 + 2];
//
//		// 頂点座標の設定
//		pos[0] = stage->mesh.VtxPos[idx0];
//		pos[1] = stage->mesh.VtxPos[idx1];
//		pos[2] = stage->mesh.VtxPos[idx2];
//
//		// 左下側ポリゴンと線分の当たり判定
//		ans = CheckHitLine(pos[0], pos[2], pos[3], pos0, pos1, &stage->mesh.mtxWorld, &vel);
//		if (ans != 0)
//		{
//			break;
//		}
//
//
//	}
//
//	return(ans);
//
//}
