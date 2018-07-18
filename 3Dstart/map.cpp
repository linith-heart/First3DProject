//=============================================================================
//
// マップ処理 [map.cpp]
// Author : 川井一生
//
//=============================================================================
#include "camera.h"
#include "map.h"
#include "input.h"

#include "checkhit.h"

//=============================================================================
// マクロ定義
#define	TEXTURE_MAP000		"data/MAP/cliff_path.png"				// 読み込むテクスチャファイル名	name of the teture file
#define	TEXTURE_MAP001		"data/MAP/cliff_bg000.png"					// 読み込むテクスチャファイル名	name of the teture file

enum
{
	MAP_TEX = 0,
	BG_MAP_TEX,
	TEX_MAX,
};


#define MAP000				"data/MAP/cliff_path001.x"						// 表示用のメシュのファイル名	name of mesh file for draw
#define MAPBG000			"data/MAP/cliff_bg.x"					// 表示用のメシュのファイル名	name of mesh file for draw
#define HITPATH000			"data/MAP/cliff_path_hitbox001.x"				// 当たり判定用メッシュのファイル名	name of mesh file for hitbox
#define HITWALL000			"data/MAP/cliff_wall_hitbox001.x"				// 当たり判定用メッシュのファイル名	name of mesh file for hitbox


#define CURRENT_MAP			(MAP000)
#define CURRENT_BG_MAP		(MAPBG000)
#define CURRENT_HIT_PATH	(HITPATH000)
#define CURRENT_HIT_WALL	(HITWALL000)

#define CURRENT_TEXTURE		(TEXTURE_MAP000)
#define CURRENT_BG_TEXTURE	(TEXTURE_MAP001)

#define MAP_SIZE		(25.0f)											// 大きさ


//=============================================================================
// プロトタイプ宣言

//=============================================================================
// グローバル変数
LPDIRECT3DTEXTURE9		D3DTextureMap[TEX_MAX] = { NULL, NULL };	// テクスチャへのポインタ

MAP						mapWk[MAP_MAX];			// フィールドへのポインタ

bool					DrawFlag = true;

//=============================================================================
// 初期化処理
HRESULT InitMap(void)
{
	MAP *map = &mapWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// テクスチャの設定
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MAP000, &D3DTextureMap[MAP_TEX]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MAP001, &D3DTextureMap[BG_MAP_TEX]);
	int i;
	for (i = 0; i < MAP_MAX; i++, map++)
	{
		//===========================================================
		// 描画用の初期化
		map->mesh.use = true;

		// 位置・回転・スケールの初期設定
		map->mesh.Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		map->mesh.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		map->mesh.Scl = D3DXVECTOR3(MAP_SIZE, MAP_SIZE, MAP_SIZE);

		// モデル関係の初期化
		map->mesh.Mesh = NULL;
		map->mesh.BuffMat = NULL;
		map->mesh.NumMat = 0;

		// 描画用のXファイルの読み込み
		switch (i % COMP4MAP)
		{
		case 0:
			if (FAILED(D3DXLoadMeshFromX(CURRENT_MAP, D3DXMESH_SYSTEMMEM, pDevice, NULL, &map->mesh.BuffMat, NULL, &map->mesh.NumMat, &map->mesh.Mesh)))
			{
				return E_FAIL;
			}
			map->isHitBox = false;
			break;
		case 1:
			if (FAILED(D3DXLoadMeshFromX(CURRENT_BG_MAP, D3DXMESH_SYSTEMMEM, pDevice, NULL, &map->mesh.BuffMat, NULL, &map->mesh.NumMat, &map->mesh.Mesh)))
			{
				return E_FAIL;
			}
			map->isHitBox = false;
			break;
		case 2:
			if (FAILED(D3DXLoadMeshFromX(CURRENT_HIT_PATH, D3DXMESH_SYSTEMMEM, pDevice, NULL, &map->mesh.BuffMat, NULL, &map->mesh.NumMat, &map->mesh.Mesh)))
			{
				return E_FAIL;
			}
			map->isHitBox = true;
			SetVtxData(&map->mesh);
			break;
		case 3:
			if (FAILED(D3DXLoadMeshFromX(CURRENT_HIT_WALL, D3DXMESH_SYSTEMMEM, pDevice, NULL, &map->mesh.BuffMat, NULL, &map->mesh.NumMat, &map->mesh.Mesh)))
			{
				return E_FAIL;
			}
			map->isHitBox = true;
			SetVtxData(&map->mesh);
			break;
		}






	}
	return S_OK;

}

//=============================================================================
// 終了処理
void UninitMap(void)
{
	MAP *map = &mapWk[0];
	for (int i = 0; i < TEX_MAX;i++)
		if (D3DTextureMap[i] != NULL)
		{// テクスチャの開放
			D3DTextureMap[i]->Release();
			D3DTextureMap[i] = NULL;
		}
	for (int i = 0; i < MAP_MAX; i++, map++)
	{
		if (map->mesh.VtxBuff != NULL)
		{// 頂点バッファの開放
			map->mesh.VtxBuff->Release();
			map->mesh.VtxBuff = NULL;
		}

		if (map->mesh.Mesh != NULL)
		{// メシュの解放
			map->mesh.Mesh->Release();
			map->mesh.Mesh = NULL;
		}

		if (map->mesh.IdxBuff != NULL)
		{// インデックスバッファとヴァーテックスポスは一緒に開放
			free(map->mesh.IdxBuff);
			free(map->mesh.VtxPos);
		}
	}
}

//=============================================================================
// 更新処理
void UpdateMap(void)
{
	MAP *map = &mapWk[0];

	if (GetKeyboardTrigger(DIK_NUMPAD9))
	{
		if (DrawFlag)
			DrawFlag = false;
		else if (!DrawFlag)
			DrawFlag = true;
	}


}

//=============================================================================
// 描画処理
void DrawMap(void)
{
	MAP *map = &mapWk[0];
	CAMERA *camera = GetCamera(0);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// スケール、回転、平行移動
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	int i, j;

	for (i = 0; i < MAP_MAX; i++, map++)
		if (map->mesh.use == true)
		{
			// ライティングを無効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			// ワールドマトリクス初期化
			D3DXMatrixIdentity(&map->mesh.mtxWorld);

			// スケールを反映	(S)
			D3DXMatrixScaling(&mtxScl, map->mesh.Scl.x, map->mesh.Scl.y, map->mesh.Scl.z);
			D3DXMatrixMultiply(&map->mesh.mtxWorld, &map->mesh.mtxWorld, &mtxScl); 	// &ene->mtxWorld = &ene->mtxWorld * &mtxScl
			// 回転を反映		(R)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, map->mesh.Rot.y, map->mesh.Rot.x, map->mesh.Rot.z);
			D3DXMatrixMultiply(&map->mesh.mtxWorld, &map->mesh.mtxWorld, &mtxRot);	// &ene->mtxWorld = &ene->mtxWorld * &mtxRot
			// 平行移動を反映	(T)
			D3DXMatrixTranslation(&mtxTranslate, map->mesh.Pos.x, map->mesh.Pos.y, map->mesh.Pos.z);
			D3DXMatrixMultiply(&map->mesh.mtxWorld, &map->mesh.mtxWorld, &mtxTranslate);

			// ワールドマトリックス設定
			pDevice->SetTransform(D3DTS_WORLD, &map->mesh.mtxWorld);

			// 現在のマテリアルを保存
			pDevice->GetMaterial(&matDef);

			// マテリアル情報へのポインタを
			pD3DXMat = (D3DXMATERIAL*)map->mesh.BuffMat->GetBufferPointer();

			if (DrawFlag)
			{
				if (i % COMP4MAP < 2)
				{
					for (j = 0; j < (int)map->mesh.NumMat; j++)
					{
						pDevice->SetMaterial(&pD3DXMat[j].MatD3D);	// マテリアルの設定
						pDevice->SetTexture(0, D3DTextureMap[i]);	// テクスチャの設定
						map->mesh.Mesh->DrawSubset(j);
					}
				}
			}
			else if (!DrawFlag)
			{
				if (i % COMP4MAP == 3)
				{
					for (j = 0; j < (int)map->mesh.NumMat; j++)
					{
						pDevice->SetMaterial(&pD3DXMat[j].MatD3D);	// マテリアルの設定
						pDevice->SetTexture(0, D3DTextureMap[i]);	// テクスチャの設定
						map->mesh.Mesh->DrawSubset(j);
					}
				}

			}
			// ライティングを有効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			// マテリアルに戻す
			pDevice->SetMaterial(&matDef);
		}
}

//=============================================================================
// マップのゲット関数
MAP *GetMap(int no)
{
	return (&mapWk[no]);
}