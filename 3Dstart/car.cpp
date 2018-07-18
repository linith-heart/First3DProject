//=============================================================================
//
// 車処理 [car.cpp]
// Author : 川井一生
//
//
//使用されていない
//
/*
エネミー（車）
前後と回転で移動

enemy(car)
moves forward and back
rotates to face player
*/
//=============================================================================
#include "enemy.h"
#include "game.h"
#include "camera.h"
#include "player.h"
#include "car.h"
#include "checkhit.h"


//=============================================================================
// マクロ定義
#define TEXTURE			("data/TEXTURE/pig_map.jpg")
#define MODEL			("data/MODEL/car000.x")
#define	VALUE_MOVE		(2.0f)							// 移動量
#define	ROT_VAL			(0.02f)							// 回転量
#define	ROT_VAL_MAX		(0.3f)							// 回転量
#define SCL_VALUE		(1.0f)
#define AGGRO_RANGE		(500)							// 
#define CHECK_RANGE		(200)							// 

#define HIGHT_VALUE		(5.0f)							// 足の位置

//=============================================================================
// プロトタイプ宣言
void MoveCar(void);

//=============================================================================
// グローバル変数
LPDIRECT3DTEXTURE9		D3DTextureCar = NULL;	// テクスチャへのポインタ

CAR						carWk[CAR_MAX];			// フィールドへのポインタ

//=============================================================================
// 初期化処理
HRESULT InitCar(void)
{
	CAR *car = &carWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int i;
	for (i = 0; i < CAR_MAX; i++, car++)
	{
		car->ene.use = true;


		// 位置・回転・スケールの初期設定
		car->ene.Pos = D3DXVECTOR3(-1500.0f, 0.0f, -1300.0f);
		car->ene.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		car->ene.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);

		// モデル関係の初期化
		//D3DXCreateTextureFromFile(pDevice, TEXTURE, &D3DTextureCar);
		D3DTextureCar = NULL;
		car->ene.Mesh = NULL;
		car->ene.BuffMat = NULL;
		car->ene.NumMat= 0;

		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(MODEL,			// モデルデータ
			D3DXMESH_SYSTEMMEM,				// 使用するメモリのオプション
			pDevice,						// デバイス 
			NULL,							// 未使用
			&car->ene.BuffMat,				// マテリアルデータへのポインター
			NULL,							// 未使用
			&car->ene.NumMat,				// D3DXMATERIAL構造体の数
			&car->ene.Mesh)))				// メッシュデータへのポインター
		{
			return E_FAIL;
		}
	}
	return S_OK;

}

//=============================================================================
// 終了処理
void UninitCar(void)
{
	CAR *car = &carWk[0];

	if (D3DTextureCar != NULL)
	{// テクスチャの開放
		D3DTextureCar->Release();
		D3DTextureCar = NULL;
	}
	for (int i = 0; i < CAR_MAX; i++, car++)
	{
		if (car->ene.VtxBuff != NULL)
		{// 頂点バッファの開放
			car->ene.VtxBuff->Release();
			car->ene.VtxBuff = NULL;
		}
		if (car->ene.Mesh != NULL)
		{// メシュの解放
			car->ene.Mesh->Release();
			car->ene.Mesh = NULL;
		}
	}
}

//=============================================================================
// 更新処理
void UpdateCar(void)
{
	CAR *car = &carWk[0];
	PLAYER *player = GetPlayer(0);
	D3DXVECTOR3 Vel = { 0.0f,0.0f,0.0f };
	int i;
	for (i = 0; i < CAR_MAX; i++, car++)
		if (car->ene.use == true)
		{
			D3DXVECTOR3 temp3 = player->Pos - car->ene.Pos;
			float distance = D3DXVec3Length(&temp3);
			if (distance < AGGRO_RANGE)
			{
				D3DXVECTOR3 ObjPos = VEC3CLEAR;
				float tAng = atan2(temp3.z, temp3.x);
				if (VisionCheck(car->ene.Pos, player->Pos, &ObjPos))
				{
					if (car->Turn < ROT_VAL_MAX)
					{
						car->Turn += ROT_VAL;
					}
				}
				else if (car->Turn >= 0.0f)
				{
					car->Turn -= ROT_VAL;
				}

				tAng += (D3DX_PI * -car->Turn);
				car->ene.Rot.y = -tAng +(D3DX_PI * -0.5);

				Vel.x = cosf(tAng) * VALUE_MOVE;
				Vel.z = sinf(tAng) * VALUE_MOVE;

				car->ene.Pos += Vel;

				// 地面との当たり判定
				D3DXVECTOR3 distance = { 0.0f,0.0f,0.0f };	// 地面までの距離
				D3DXVECTOR3 head = car->ene.Pos;
				D3DXVECTOR3 feet = car->ene.Pos;
				head.y += HIGHT_VALUE;
				feet.y -= HIGHT_VALUE;

				if (CheckHitGroundPrep(head, feet, &distance))
				{
					car->ene.Pos.y += distance.y;
					car->ene.Grav = 0.0f;
				}
				else
				{
					car->ene.Grav -= GRAVITY;
					car->ene.Pos.y += car->ene.Grav;
				}

			}


		}
}

//=============================================================================
// 描画処理
void DrawCar(void)
{
	CAR *car = &carWk[0];
	CAMERA *camera = GetCamera(0);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// スケール、回転、平行移動
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	int i, j;

	for (i = 0; i < CAR_MAX; i++, car++)
		if (car->ene.use == true)
		{
			// ワールドマトリクス初期化
			D3DXMatrixIdentity(&car->ene.mtxWorld);

			// スケールを反映	(S)
			D3DXMatrixScaling(&mtxScl, car->ene.Scl.x, car->ene.Scl.y, car->ene.Scl.z);
			D3DXMatrixMultiply(&car->ene.mtxWorld, &car->ene.mtxWorld, &mtxScl); 	// &ene->mtxWorld = &ene->mtxWorld * &mtxScl
			// 回転を反映		(R)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, car->ene.Rot.y, car->ene.Rot.x, car->ene.Rot.z);
			D3DXMatrixMultiply(&car->ene.mtxWorld, &car->ene.mtxWorld, &mtxRot);	// &ene->mtxWorld = &ene->mtxWorld * &mtxRot
			// 平行移動を反映	(T)
			D3DXMatrixTranslation(&mtxTranslate, car->ene.Pos.x, car->ene.Pos.y, car->ene.Pos.z);
			D3DXMatrixMultiply(&car->ene.mtxWorld, &car->ene.mtxWorld, &mtxTranslate);

			// ワールドマトリックス設定
			pDevice->SetTransform(D3DTS_WORLD, &car->ene.mtxWorld);

			// 現在のマテリアルを保存
			pDevice->GetMaterial(&matDef);

			// マテリアル情報へのポインタを
			pD3DXMat = (D3DXMATERIAL*)car->ene.BuffMat->GetBufferPointer();


			for (j = 0; j < (int)car->ene.NumMat; j++)
			{
				pDevice->SetMaterial(&pD3DXMat[j].MatD3D);	// マテリアルの設定

				pDevice->SetTexture(0, D3DTextureCar);	// テクスチャの設定

				car->ene.Mesh->DrawSubset(j);
			}

			// マテリアルに戻す
			pDevice->SetMaterial(&matDef);
		}
}

//=============================================================================
// 車の移動
void MoveCar(void)
{
	CAR *car = &carWk[0];
	PLAYER *player = GetPlayer(0);

}

//=============================================================================
// 車のゲット関数
CAR *GetCar(int no)
{
	return (&carWk[no]);
}