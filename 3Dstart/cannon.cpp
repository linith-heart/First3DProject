//=============================================================================
//
// 大砲処理 [cannon.cpp]
// Author : 川井一生
//
//
//使用されていない
//
/*
エネミー（大砲）
回転と発射

enemy(cannon)
rotate and fire cannon
*/
//=============================================================================
#include "enemy.h"
#include "camera.h"
#include "player.h"
#include "cannon.h"
#include "bullet.h"

#include "checkhit.h"

//=============================================================================
// マクロ定義
#define TEXTURE			("data/TEXTURE/pig_map.jpg")
#define MODEL			("data/MODEL/car000.x")
#define	VALUE_MOVE		(0.5f)											// 移動量
#define	ROT_VALUE		(D3DX_PI * 0.02f)								// 回転量
#define SCL_VALUE		(1.0f)
#define AGGRO_RANGE		(500)											// アグロ距離

//=============================================================================
// プロトタイプ宣言
void MoveCan(void);

//=============================================================================
// グローバル変数
LPDIRECT3DTEXTURE9		D3DTextureCan = NULL;	// テクスチャへのポインタ

CAN						canWk[CAN_MAX];			// フィールドへのポインタ

//=============================================================================
// 初期化処理
HRESULT InitCannon(void)
{
	CAN *can = &canWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int i;
	for (i = 0; i < CAN_MAX; i++, can++)
	{
		can->ene.use = true;


		// 位置・回転・スケールの初期設定
		can->ene.Pos = D3DXVECTOR3(0.0f, 0.0f, 50.0f);
		can->ene.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		can->ene.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);

		// モデル関係の初期化
		//D3DXCreateTextureFromFile(pDevice, TEXTURE, &D3DTextureCan);
		D3DTextureCan = NULL;
		can->ene.Mesh = NULL;
		can->ene.BuffMat = NULL;
		can->ene.NumMat= 0;

		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(MODEL,			// モデルデータ
			D3DXMESH_SYSTEMMEM,				// 使用するメモリのオプション
			pDevice,						// デバイス 
			NULL,							// 未使用
			&can->ene.BuffMat,				// マテリアルデータへのポインター
			NULL,							// 未使用
			&can->ene.NumMat,				// D3DXMATERIAL構造体の数
			&can->ene.Mesh)))				// メッシュデータへのポインター
		{
			return E_FAIL;
		}
	}
	return S_OK;

}

//=============================================================================
// 終了処理
void UninitCannon(void)
{
	CAN *can = &canWk[0];

	if (D3DTextureCan != NULL)
	{// テクスチャの開放
		D3DTextureCan->Release();
		D3DTextureCan = NULL;
	}
	for (int i = 0; i < CAN_MAX; i++, can++)
	{
		if (can->ene.VtxBuff != NULL)
		{// 頂点バッファの開放
			can->ene.VtxBuff->Release();
			can->ene.VtxBuff = NULL;
		}
		if (can->ene.Mesh != NULL)
		{// メシュの解放
			can->ene.Mesh->Release();
			can->ene.Mesh = NULL;
		}
	}
}

//=============================================================================
// 更新処理
void UpdateCannon(void)
{
	CAN *can = &canWk[0];
	PLAYER *player = GetPlayer(0);
	D3DXVECTOR3 Vel = { 0.0f,0.0f,0.0f };
	int i;
	for (i = 0; i < CAN_MAX; i++, can++)
	{
		D3DXVECTOR3 temp3 = player->Pos - can->ene.Pos;
		float distance = D3DXVec3Length(&temp3);
		if (distance < AGGRO_RANGE)
		{
			float tAng = atan2(temp3.z, temp3.x);

			if (can->ShotCD <= 0)
			{
				D3DXVECTOR3 ObjPos = VEC3CLEAR;
				if (!VisionCheck(can->ene.Pos, player->Pos, &ObjPos))
				{
					SetBull(can->ene.Pos, can->ene.Rot);
					can->ShotCD = 50;
				}
			}
			can->ene.Rot.y = -tAng +(D3DX_PI * -0.5);

		}
		can->ShotCD--;

	}

}

//=============================================================================
// 描画処理
void DrawCannon(void)
{
	CAN *can = &canWk[0];
	CAMERA *camera = GetCamera(0);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// スケール、回転、平行移動
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	int i, j;

	for (i = 0; i < CAN_MAX; i++, can++)
		if (can->ene.use == true)
		{
			// ワールドマトリクス初期化
			D3DXMatrixIdentity(&can->ene.mtxWorld);

			// スケールを反映	(S)
			D3DXMatrixScaling(&mtxScl, can->ene.Scl.x, can->ene.Scl.y, can->ene.Scl.z);
			D3DXMatrixMultiply(&can->ene.mtxWorld, &can->ene.mtxWorld, &mtxScl); 	// &ene->mtxWorld = &ene->mtxWorld * &mtxScl
			// 回転を反映		(R)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, can->ene.Rot.y, can->ene.Rot.x, can->ene.Rot.z);
			D3DXMatrixMultiply(&can->ene.mtxWorld, &can->ene.mtxWorld, &mtxRot);	// &ene->mtxWorld = &ene->mtxWorld * &mtxRot
			// 平行移動を反映	(T)
			D3DXMatrixTranslation(&mtxTranslate, can->ene.Pos.x, can->ene.Pos.y, can->ene.Pos.z);
			D3DXMatrixMultiply(&can->ene.mtxWorld, &can->ene.mtxWorld, &mtxTranslate);

			// ワールドマトリックス設定
			pDevice->SetTransform(D3DTS_WORLD, &can->ene.mtxWorld);

			// 現在のマテリアルを保存
			pDevice->GetMaterial(&matDef);

			// マテリアル情報へのポインタを
			pD3DXMat = (D3DXMATERIAL*)can->ene.BuffMat->GetBufferPointer();


			for (j = 0; j < (int)can->ene.NumMat; j++)
			{
				pDevice->SetMaterial(&pD3DXMat[j].MatD3D);	// マテリアルの設定

				pDevice->SetTexture(0, D3DTextureCan);	// テクスチャの設定

				can->ene.Mesh->DrawSubset(j);
			}

			// マテリアルに戻す
			pDevice->SetMaterial(&matDef);
		}
}

//=============================================================================
// 車の移動
void MoveCannon(void)
{
	CAN *can = &canWk[0];
	PLAYER *player = GetPlayer(0);

}

//=============================================================================
// 車のゲット関数
CAN *GetCannon(int no)
{
	return (&canWk[no]);
}