//=============================================================================
//
// 車処理 [house.cpp]
// Author : 川井一生
//
//=============================================================================
#include "camera.h"
#include "fade.h"
#include "player.h"
#include "pig.h"
#include "house.h"

/*
エネミー（車）
前後と回転で移動

meshmy(house)
moves forward and back 
rotates to face player
*/

//=============================================================================
// マクロ定義
#define TEXTURE			("data/TEXTURE/cave_entrance000.png")
#define MODEL			("data/MODEL/cave_entrance000.x")
#define	VALUE_MOVE		(0.5f)											// 移動量
#define	ROT_VALUE		(D3DX_PI * 0.02f)								// 回転量
#define SCL_VALUE		(1.0f)

#define RANGE			(200.0f)

#define POS_X			(3700.0f)
#define POS_Y			(0.0f)
#define POS_Z			(3580.0f)


//=============================================================================
// プロトタイプ宣言
void MoveHouse(void);

//=============================================================================
// グローバル変数
LPDIRECT3DTEXTURE9		D3DTextureHouse = NULL;	// テクスチャへのポインタ

HOUSE						houseWk[HOUSE_MAX];			// フィールドへのポインタ

//=============================================================================
// 初期化処理
HRESULT InitHouse(void)
{
	HOUSE *house = &houseWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの設定
	D3DXCreateTextureFromFile(pDevice, TEXTURE, &D3DTextureHouse);

	int i;
	for (i = 0; i < HOUSE_MAX; i++, house++)
	{
		house->mesh.use = true;


		// 位置・回転・スケールの初期設定
		house->mesh.Pos = D3DXVECTOR3(POS_X, POS_Y, POS_Z);
		house->mesh.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		house->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);


		// モデル関係の初期化
		house->mesh.TexId = 0;
		//D3DTextureHouse = NULL;
		house->mesh.Mesh = NULL;
		house->mesh.BuffMat = NULL;
		house->mesh.NumMat= 0;

		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(MODEL,			// モデルデータ
			D3DXMESH_SYSTEMMEM,				// 使用するメモリのオプション
			pDevice,						// デバイス 
			NULL,							// 未使用
			&house->mesh.BuffMat,				// マテリアルデータへのポインター
			NULL,							// 未使用
			&house->mesh.NumMat,				// D3DXMATERIAL構造体の数
			&house->mesh.Mesh)))				// メッシュデータへのポインター
		{
			return E_FAIL;
		}
	}
	return S_OK;

}

//=============================================================================
// 終了処理
void UninitHouse(void)
{
	HOUSE *house = &houseWk[0];

	if (D3DTextureHouse != NULL)
	{// テクスチャの開放
		D3DTextureHouse->Release();
		D3DTextureHouse = NULL;
	}
	for (int i = 0; i < HOUSE_MAX; i++, house++)
	{
		if (house->mesh.VtxBuff != NULL)
		{// 頂点バッファの開放
			house->mesh.VtxBuff->Release();
			house->mesh.VtxBuff = NULL;
		}
		if (house->mesh.Mesh != NULL)
		{// メシュの解放
			house->mesh.Mesh->Release();
			house->mesh.Mesh = NULL;
		}

	}
}

//=============================================================================
// 更新処理
void UpdateHouse(void)
{
	HOUSE *house = &houseWk[0];
	PLAYER *player = GetPlayer(0);
	PIG *pig = GetPig(0);
	D3DXVECTOR3 Vel = { 0.0f,0.0f,0.0f };
	int i;
	for (i = 0; i < HOUSE_MAX; i++, house++)
	{
		// もし豚を連れている状態で家にちかずくと
		D3DXVECTOR3 temp = player->Pos - house->mesh.Pos;
		float tLen = D3DXVec3Length(&temp);
		if (tLen < RANGE)
		{

			D3DXVECTOR3 temp = house->mesh.Pos - pig->npc.Pos;


			SetFade(FADE_OUT);

		}



	}
}

//=============================================================================
// 描画処理
void DrawHouse(void)
{
	HOUSE *house = &houseWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// スケール、回転、平行移動
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	int i, j;

	for (i = 0; i < HOUSE_MAX; i++, house++)
		if (house->mesh.use == true)
		{
			// ワールドマトリクス初期化
			D3DXMatrixIdentity(&house->mesh.mtxWorld);

			// スケールを反映	(S)
			D3DXMatrixScaling(&mtxScl, house->mesh.Scl.x, house->mesh.Scl.y, house->mesh.Scl.z);
			D3DXMatrixMultiply(&house->mesh.mtxWorld, &house->mesh.mtxWorld, &mtxScl); 	// &mesh->mtxWorld = &mesh->mtxWorld * &mtxScl
			// 回転を反映		(R)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, house->mesh.Rot.y, house->mesh.Rot.x, house->mesh.Rot.z);
			D3DXMatrixMultiply(&house->mesh.mtxWorld, &house->mesh.mtxWorld, &mtxRot);	// &mesh->mtxWorld = &mesh->mtxWorld * &mtxRot
			// 平行移動を反映	(T)
			D3DXMatrixTranslation(&mtxTranslate, house->mesh.Pos.x, house->mesh.Pos.y, house->mesh.Pos.z);
			D3DXMatrixMultiply(&house->mesh.mtxWorld, &house->mesh.mtxWorld, &mtxTranslate);

			// ワールドマトリックス設定
			pDevice->SetTransform(D3DTS_WORLD, &house->mesh.mtxWorld);

			// 現在のマテリアルを保存
			pDevice->GetMaterial(&matDef);

			// マテリアル情報へのポインタを
			pD3DXMat = (D3DXMATERIAL*)house->mesh.BuffMat->GetBufferPointer();


			for (j = 0; j < (int)house->mesh.NumMat; j++)
			{
				pDevice->SetMaterial(&pD3DXMat[j].MatD3D);	// マテリアルの設定

				pDevice->SetTexture(0, D3DTextureHouse);	// テクスチャの設定

				house->mesh.Mesh->DrawSubset(j);
			}

			// マテリアルに戻す
			pDevice->SetMaterial(&matDef);
		}
}

//=============================================================================
// 車の移動
void MoveHouse(void)
{
	HOUSE *house = &houseWk[0];
	PLAYER *player = GetPlayer(0);

}

//=============================================================================
// 車のゲット関数
HOUSE *GetHouse(int no)
{
	return (&houseWk[no]);
}