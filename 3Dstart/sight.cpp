//=============================================================================
//
// サイト処理 [sight.cpp]
// Author : 川井一生
//
//
//使用されていない
//
/*
エネミー（サイト）
目線にに入ると処理を始める

enemy (sight)
movement based on line of sight
*/
//=============================================================================
#include "enemy.h"
#include "camera.h"
#include "player.h"
#include "sight.h"
#include "bullet.h"
#include "checkhit.h"
#include "mathWk.h"


//=============================================================================
// マクロ定義
#define TEXTURE			("data/TEXTURE/pig_map.jpg")
#define MODEL			("data/MODEL/car001.x")
#define	VALUE_MOVE		(0.5f)							// 移動量
#define	ROT_VALUE		(D3DX_PI * 0.02f)				// 回転量

#define AGGRO_RANGE		(500)							// 反応する距離
#define VISION_RAD		(0.7)							// 見える範囲

//=============================================================================
// プロトタイプ宣言
void MoveSight(void);

//=============================================================================
// グローバル変数
LPDIRECT3DTEXTURE9		D3DTextureSight = NULL;	// テクスチャへのポインタ

SIGHT						sightWk[SIGHT_MAX];			// フィールドへのポインタ

//=============================================================================
// 初期化処理
HRESULT InitSight(void)
{
	SIGHT *sight = &sightWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int i;
	for (i = 0; i < SIGHT_MAX; i++, sight++)
	{
		sight->ene.use = true;


		// 位置・回転・スケールの初期設定
		sight->ene.Pos = D3DXVECTOR3(100.0f, 0.0f, 0.0f);
//		sight->ene.Pos = D3DXVECTOR3(100.0f, 0.0f, 50.0f);
		sight->ene.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		sight->ene.Scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

		// モデル関係の初期化
		//D3DXCreateTextureFromFile(pDevice, TEXTURE, &D3DTextureSight);
		D3DTextureSight = NULL;
		sight->ene.Mesh = NULL;
		sight->ene.BuffMat = NULL;
		sight->ene.NumMat= 0;

		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(MODEL,			// モデルデータ
			D3DXMESH_SYSTEMMEM,				// 使用するメモリのオプション
			pDevice,						// デバイス 
			NULL,							// 未使用
			&sight->ene.BuffMat,				// マテリアルデータへのポインター
			NULL,							// 未使用
			&sight->ene.NumMat,				// D3DXMATERIAL構造体の数
			&sight->ene.Mesh)))				// メッシュデータへのポインター
		{
			return E_FAIL;
		}
	}
	return S_OK;

}

//=============================================================================
// 終了処理
void UninitSight(void)
{
	SIGHT *sight = &sightWk[0];

	if (D3DTextureSight != NULL)
	{// テクスチャの開放
		D3DTextureSight->Release();
		D3DTextureSight = NULL;
	}

	for (int i = 0; i < SIGHT_MAX; i++, sight++)
	{
		if (sight->ene.VtxBuff != NULL)
		{// 頂点バッファの開放
			sight->ene.VtxBuff->Release();
			sight->ene.VtxBuff = NULL;
		}
		if (sight->ene.Mesh != NULL)
		{// メシュの解放
			sight->ene.Mesh->Release();
			sight->ene.Mesh = NULL;
		}

	}
}

//=============================================================================
// 更新処理
void UpdateSight(void)
{
	SIGHT *sight = &sightWk[0];
	PLAYER *player = GetPlayer(0);
	D3DXVECTOR3 Vel = { 0.0f,0.0f,0.0f };
	int i;
	for (i = 0; i < SIGHT_MAX; i++, sight++)
	{
		D3DXVECTOR3 temp3 = player->Pos - sight->ene.Pos;
		float distance = D3DXVec3Length(&temp3);

		if (distance < AGGRO_RANGE)
		{

			if (inView(sight->ene.Pos,player->Pos,sight->ene.Rot, VISION_RAD))
			{
				D3DXVECTOR3 ObjPos = VEC3CLEAR;
				if (!VisionCheck(sight->ene.Pos, player->Pos, &ObjPos))
				{
					SetBull(sight->ene.Pos, sight->ene.Rot);
				}
			}

			sight->ene.Rot.y += ROT_VALUE;
		}
		else
		{
			sight->ene.Rot.y += ROT_VALUE;
		}

	
	}
}

//=============================================================================
// 描画処理
void DrawSight(void)
{
	SIGHT *sight = &sightWk[0];
	CAMERA *camera = GetCamera(0);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// スケール、回転、平行移動
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	int i, j;

	for (i = 0; i < SIGHT_MAX; i++, sight++)
		if (sight->ene.use == true)
		{
			// ワールドマトリクス初期化
			D3DXMatrixIdentity(&sight->ene.mtxWorld);

			// スケールを反映	(S)
			D3DXMatrixScaling(&mtxScl, sight->ene.Scl.x, sight->ene.Scl.y, sight->ene.Scl.z);
			D3DXMatrixMultiply(&sight->ene.mtxWorld, &sight->ene.mtxWorld, &mtxScl); 	// &ene->mtxWorld = &ene->mtxWorld * &mtxScl
																			// 回転を反映		(R)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, sight->ene.Rot.y, sight->ene.Rot.x, sight->ene.Rot.z);
			D3DXMatrixMultiply(&sight->ene.mtxWorld, &sight->ene.mtxWorld, &mtxRot);	// &ene->mtxWorld = &ene->mtxWorld * &mtxRot
																			// 平行移動を反映	(T)
			D3DXMatrixTranslation(&mtxTranslate, sight->ene.Pos.x, sight->ene.Pos.y, sight->ene.Pos.z);
			D3DXMatrixMultiply(&sight->ene.mtxWorld, &sight->ene.mtxWorld, &mtxTranslate);

			// ワールドマトリックス設定
			pDevice->SetTransform(D3DTS_WORLD, &sight->ene.mtxWorld);

			// 現在のマテリアルを保存
			pDevice->GetMaterial(&matDef);

			// マテリアル情報へのポインタを
			pD3DXMat = (D3DXMATERIAL*)sight->ene.BuffMat->GetBufferPointer();


			for (j = 0; j < (int)sight->ene.NumMat; j++)
			{
				pDevice->SetMaterial(&pD3DXMat[j].MatD3D);	// マテリアルの設定

				pDevice->SetTexture(0, D3DTextureSight);	// テクスチャの設定

				sight->ene.Mesh->DrawSubset(j);
			}

			// マテリアルに戻す
			pDevice->SetMaterial(&matDef);
		}
}

//=============================================================================
// 車の移動
void MoveSight(void)
{
	SIGHT *sight = &sightWk[0];
	PLAYER *player = GetPlayer(0);

}

//=============================================================================
// 車のゲット関数
SIGHT *GetSight(int no)
{
	return (&sightWk[no]);
}