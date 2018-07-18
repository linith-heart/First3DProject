//=============================================================================
//
// 車処理 [pig.cpp]
// Author : 川井一生
//
//=============================================================================
#include "fade.h"
#include "npc.h"
#include "camera.h"
#include "player.h"
#include "house.h"
#include "pig.h"
#include "boar.h"

/*
エネミー（車）
前後と回転で移動

npcmy(pig)
moves forward and back 
rotates to face player
*/

//=============================================================================
// マクロ定義
#define TEXTURE			("data/TEXTURE/pig_map.jpg")
#define MODEL			("data/MODEL/pig000.x")
#define	VEL_MAX			(3.0f)							// 移動量
#define	ROT_VALUE		(D3DX_PI * 0.02f)				// 回転量
#define SCL_VALUE		(0.7f)
#define RANG_RAT		(75)							// プレイヤーまでの距離の率		range ratio from pig to player
#define GATHER_RANGE	(100)

#define FRAME_VALUE		(50)


//=============================================================================
// プロトタイプ宣言
void MovePig(void);

//=============================================================================
// グローバル変数
LPDIRECT3DTEXTURE9		D3DTexturePig = NULL;	// テクスチャへのポインタ		pointer to texture

PIG						pigWk[PIG_MAX];			// 構造体へのポインタ			pointer to stuct

int						GOT;					// ゲームオーバーのタイマー		GameOverTimer



//=============================================================================
// 初期化処理
HRESULT InitPig(void)
{
	PIG *pig = &pigWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int i;
	for (i = 0; i < PIG_MAX; i++, pig++)
	{
		pig->npc.use = true;

		pig->GoHome = false;
		pig->LetOut = false;
		pig->isUpAnim = false;
		pig->fUpAnim = 0.0f;
		// 位置・回転・スケールの初期設定
		pig->npc.Pos = D3DXVECTOR3(-1650.0f, 0.0f, -1450.0f);
		pig->npc.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pig->npc.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);

		// モデル関係の初期化
		D3DXCreateTextureFromFile(pDevice, TEXTURE, &D3DTexturePig);
		//D3DTexturePig = NULL;
		pig->npc.Mesh = NULL;
		pig->npc.BuffMat = NULL;
		pig->npc.NumMat= 0;

		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(MODEL,		// モデルデータ
			D3DXMESH_SYSTEMMEM,					// 使用するメモリのオプション
			pDevice,							// デバイス 
			NULL,								// 未使用
			&pig->npc.BuffMat,					// マテリアルデータへのポインター
			NULL,								// 未使用
			&pig->npc.NumMat,					// D3DXMATERIAL構造体の数
			&pig->npc.Mesh)))					// メッシュデータへのポインター
		{
			return E_FAIL;
		}
	}
	return S_OK;

}

//=============================================================================
// 終了処理
void UninitPig(void)
{
	PIG *pig = &pigWk[0];

	if (D3DTexturePig != NULL)
	{// テクスチャの開放
		D3DTexturePig->Release();
		D3DTexturePig = NULL;
	}
	for (int i = 0; i < PIG_MAX; i++)
	{
		if (pig->npc.VtxBuff != NULL)
		{// 頂点バッファの開放
			pig->npc.VtxBuff->Release();
			pig->npc.VtxBuff = NULL;
		}
		if (pig->npc.Mesh != NULL)
		{// メシュの解放
			pig->npc.Mesh->Release();
			pig->npc.Mesh = NULL;
		}
	}
}

//=============================================================================
// 更新処理
void UpdatePig(void)
{
	PIG *pig = &pigWk[0];
	PLAYER *player = GetPlayer(0);
	HOUSE *house = GetHouse(0);
	BOAR *boar = GetBoar(0);
	int i;
	for (i = 0; i < PIG_MAX; i++, pig++)
	{

		pig->npc.Pos = boar->RoamPos;



		//=================================================
		// 動いている時のアニメーション
		if (pig->npc.Vel.x != 0 || pig->npc.Vel.z!= 0)
		{
			if (pig->isUpAnim == true)
			{
				if (pig->fUpAnim < 1.0f)
				{
					pig->fUpAnim += 0.1f;
					pig->npc.Pos.y += pig->fUpAnim;
				}
				else
				{
					pig->fUpAnim += 0.1f;
					pig->isUpAnim = false;
				}
			}
			if (pig->isUpAnim == false)
			{
				if (pig->fUpAnim > 0.1f)
				{
					pig->fUpAnim -= 0.1f;
					pig->npc.Pos.y -= pig->fUpAnim;
				}
				else
				{
					pig->isUpAnim = true;
				}
			}
		}
		else 
		{
			while (pig->fUpAnim > 0.1f)
			{
				pig->fUpAnim -= 0.1f;
				pig->npc.Pos.y -= pig->fUpAnim;
				pig->isUpAnim = true;
			}
		}
	}
}

//=============================================================================
// 描画処理
void DrawPig(void)
{
	PIG *pig = &pigWk[0];
	CAMERA *camera = GetCamera(0);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// スケール、回転、平行移動
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	int i, j;

	for (i = 0; i < PIG_MAX; i++, pig++)
		if (pig->npc.use == true)
		{
			// ワールドマトリクス初期化
			D3DXMatrixIdentity(&pig->npc.mtxWorld);

			// スケールを反映	(S)
			D3DXMatrixScaling(&mtxScl, pig->npc.Scl.x, pig->npc.Scl.y, pig->npc.Scl.z);
			D3DXMatrixMultiply(&pig->npc.mtxWorld, &pig->npc.mtxWorld, &mtxScl); 	// &npc->mtxWorld = &npc->mtxWorld * &mtxScl
			// 回転を反映		(R)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pig->npc.Rot.y, pig->npc.Rot.x, pig->npc.Rot.z);
			D3DXMatrixMultiply(&pig->npc.mtxWorld, &pig->npc.mtxWorld, &mtxRot);	// &npc->mtxWorld = &npc->mtxWorld * &mtxRot
			// 平行移動を反映	(T)
			D3DXMatrixTranslation(&mtxTranslate, pig->npc.Pos.x, pig->npc.Pos.y, pig->npc.Pos.z);
			D3DXMatrixMultiply(&pig->npc.mtxWorld, &pig->npc.mtxWorld, &mtxTranslate);

			// ワールドマトリックス設定
			pDevice->SetTransform(D3DTS_WORLD, &pig->npc.mtxWorld);

			// 現在のマテリアルを保存
			pDevice->GetMaterial(&matDef);

			// マテリアル情報へのポインタを
			pD3DXMat = (D3DXMATERIAL*)pig->npc.BuffMat->GetBufferPointer();


			for (j = 0; j < (int)pig->npc.NumMat; j++)
			{
				pDevice->SetMaterial(&pD3DXMat[j].MatD3D);	// マテリアルの設定

				pDevice->SetTexture(0, D3DTexturePig);	// テクスチャの設定

				pig->npc.Mesh->DrawSubset(j);
			}

			// マテリアルに戻す
			pDevice->SetMaterial(&matDef);
		}
}

//=============================================================================
// 車の移動
void MovePig(void)
{
	PIG *pig = &pigWk[0];
	PLAYER *player = GetPlayer(0);

}

//=============================================================================
// 車のゲット関数
PIG *GetPig(int no)
{
	return (&pigWk[no]);
}