//=============================================================================
//
// ジェムの処理 [gem.cpp]
// Author : 川井一生
//
//=============================================================================
#include "gem.h"

#include "player.h"

#include "checkhit.h"

#include "effect.h"
/*
メシュのジェムの処理
鍵みたいな役
ゲーム内で＿＿集めれば次へと進める


mesh gem
acts like a key 
door to next level will open if all gems are collected 
*/

//=============================================================================
// マクロ定義
#define TEXTURE_GEM		("data/TEXTURE/gem000.png")	// ジェムのテクスチャー
#define MODEL			("data/MODEL/gem000.x")		// ジェムのモデル
#define	VALUE_MOVE		(0.5f)						// 移動量
#define	ROT_VALUE		(D3DX_PI * 0.02f)			// 回転量
#define SCL_VALUE		(0.5f)						// 大きさ
#define RANGE_VALUE		(50)						// 箱からの距離

#define SPARKLE_DISTANCE	(50)					// パーティクルの距離

//=============================================================================
// プロトタイプ宣言
void MoveGem(int no);

//=============================================================================
// グローバル変数
LPDIRECT3DTEXTURE9		D3DTextureGem = NULL;		// テクスチャへのポインタ

GEM						gemWk[GEM_MAX];			// フィールドへのポインタ

int						SparkCnt = 0;
//=============================================================================
// 初期化処理
HRESULT InitGem(void)
{
	GEM *gem = &gemWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int i;
	for (i = 0; i < GEM_MAX; i++, gem++)
	{
		gem->mesh.use = true;

		// 位置・回転・スケールの初期設定
		switch (i)
		{
		case 0:
			gem->mesh.Pos = D3DXVECTOR3(220.0f, 460.0f, -2300.0f);
			break;
		case 1:
			gem->mesh.Pos = D3DXVECTOR3(70.0f, 320.0f, 2500.0f);
			break;
		case 2:
			gem->mesh.Pos = D3DXVECTOR3(1960.0f, 0.0f, 360.0f);
			break;
		case 3:
			gem->mesh.Pos = D3DXVECTOR3(1650.0f, 0.0f, 2140.0f);
			break;
		}
		gem->mesh.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		gem->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);

		// モデル関係の初期化
		D3DXCreateTextureFromFile(pDevice, TEXTURE_GEM, &D3DTextureGem);
		gem->mesh.VtxBuff = NULL;
		//MakeVertexGem(pDevice);
		gem->mesh.Mesh = NULL;
		gem->mesh.BuffMat = NULL;
		gem->mesh.NumMat = 0;

		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(MODEL,			// モデルデータ
			D3DXMESH_SYSTEMMEM,					// 使用するメモリのオプション
			pDevice,							// デバイス 
			NULL,								// 未使用
			&gem->mesh.BuffMat,				// マテリアルデータへのポインター
			NULL,								// 未使用
			&gem->mesh.NumMat,				// D3DXMATERIAL構造体の数
			&gem->mesh.Mesh)))				// メッシュデータへのポインター
		{
			return E_FAIL;
		}
	}
}

//=============================================================================
// ジェムのリセット処理
void ResetGem(void)
{
	GEM *gem = &gemWk[0];
	int i;
	for (i = 0; i < GEM_MAX; i++, gem++)
	{
		gem->mesh.use = true;

		// 位置・回転・スケールの初期設定
		switch (i)
		{
		case 0:
			gem->mesh.Pos = D3DXVECTOR3(270.0f, 660.0f, -2020.0f);
			break;
		case 1:
			gem->mesh.Pos = D3DXVECTOR3(695.0f, 290.0f, 1675.0f);
			break;
		case 2:
			gem->mesh.Pos = D3DXVECTOR3(1960.0f, 0.0f, 360.0f);
			break;
		case 3:
			gem->mesh.Pos = D3DXVECTOR3(1650.0f, 0.0f, 2140.0f);
			break;
		case 4:
			gem->mesh.Pos = D3DXVECTOR3(-1495.0f, 725.0f, -2240.0f);
			break;
		case 5:
			gem->mesh.Pos = D3DXVECTOR3(140.0f, 50.0f, -150.0f);
			break;
		case 6:
			gem->mesh.Pos = D3DXVECTOR3(-500.0f, 90.0f, 1100.0f);
			break;
		}
		gem->mesh.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		gem->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);
	}
}

//=============================================================================
// 終了処理
void UninitGem(void)
{
	GEM *gem = &gemWk[0];
	if (D3DTextureGem != NULL)
	{// テクスチャの開放
		D3DTextureGem->Release();
		D3DTextureGem = NULL;
	}
	for (int i = 0; i < GEM_MAX; i++, gem++)
	{
		if (gem->mesh.VtxBuff != NULL)
		{// 頂点バッファの開放
			gem->mesh.VtxBuff->Release();
			gem->mesh.VtxBuff = NULL;
		}
	}
}



//=============================================================================
// 更新処理
void UpdateGem(void)
{
	GEM *gem = &gemWk[0];
	PLAYER *player = GetPlayer(0);
	D3DXVECTOR3 Vel = VEC3CLEAR;
	D3DXVECTOR3 Sparkle = VEC3CLEAR;

	int i, j;
	for (i = 0; i < GEM_MAX; i++, gem++)
		if (gem->mesh.use)
		{
			gem->mesh.Rot.y += ROT_VALUE;
			for (j = 0, player = GetPlayer(0); j < PLAYER_MAX; j++, player++)
				if ((CheckHitBC(player->Pos, gem->mesh.Pos, 30, 30)) == true)
					gem->mesh.use = false;


			if (SparkCnt % 10 == 0)
			{
				Sparkle.x = gem->mesh.Pos.x + (rand() % SPARKLE_DISTANCE) - SPARKLE_DISTANCE / 2;
				Sparkle.y = gem->mesh.Pos.y + 10 + (rand() % SPARKLE_DISTANCE) - SPARKLE_DISTANCE / 2;
				Sparkle.z = gem->mesh.Pos.z + (rand() % SPARKLE_DISTANCE) - SPARKLE_DISTANCE / 2;

				SetEffect(Sparkle, D3DXVECTOR3(0.0f, 0.1f, 0.0f),
					D3DXCOLOR(0.65f, 0.05f, 0.85f, 0.50f), 16.0f, 16.0f, 60, Idx_SPARK);
			}
		}
	SparkCnt++;
}


//=============================================================================
// 描画処理
void DrawGem(void)
{
	GEM *gem = &gemWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// スケール、回転、平行移動
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	int i, j;

	for (i = 0; i < GEM_MAX; i++, gem++)
		if (gem->mesh.use == true)
		{
			// ワールドマトリクス初期化
			D3DXMatrixIdentity(&gem->mesh.mtxWorld);

			// スケールを反映	(S)
			D3DXMatrixScaling(&mtxScl, gem->mesh.Scl.x, gem->mesh.Scl.y, gem->mesh.Scl.z);
			D3DXMatrixMultiply(&gem->mesh.mtxWorld, &gem->mesh.mtxWorld, &mtxScl); 	// &mesh->mtxWorld = &mesh->mtxWorld * &mtxScl
																						// 回転を反映		(R)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, gem->mesh.Rot.y, gem->mesh.Rot.x, gem->mesh.Rot.z);
			D3DXMatrixMultiply(&gem->mesh.mtxWorld, &gem->mesh.mtxWorld, &mtxRot);	// &mesh->mtxWorld = &mesh->mtxWorld * &mtxRot
																						// 平行移動を反映	(T)
			D3DXMatrixTranslation(&mtxTranslate, gem->mesh.Pos.x, gem->mesh.Pos.y, gem->mesh.Pos.z);
			D3DXMatrixMultiply(&gem->mesh.mtxWorld, &gem->mesh.mtxWorld, &mtxTranslate);

			// ワールドマトリックス設定
			pDevice->SetTransform(D3DTS_WORLD, &gem->mesh.mtxWorld);

			// 現在のマテリアルを保存
			pDevice->GetMaterial(&matDef);

			// マテリアル情報へのポインタを
			pD3DXMat = (D3DXMATERIAL*)gem->mesh.BuffMat->GetBufferPointer();


			for (j = 0; j < (int)gem->mesh.NumMat; j++)
			{
				pDevice->SetMaterial(&pD3DXMat[j].MatD3D);	// マテリアルの設定

				pDevice->SetTexture(0, D3DTextureGem);	// テクスチャの設定
				gem->mesh.Mesh->DrawSubset(j);
			}

			// マテリアルに戻す
			pDevice->SetMaterial(&matDef);
		}
}

//=============================================================================
// ジェムの移動処理
void MoveGem(int no)
{
	GEM *gem = &gemWk[no];

	gem->mesh.Rot.y += ROT_VALUE;

}


//=============================================================================
// ジェムのゲット関数
GEM *GetGem(int no)
{
	return (&gemWk[no]);
}
