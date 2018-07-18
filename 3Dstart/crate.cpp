//=============================================================================
//
// 箱の処理 [crate.cpp]
// Author : 川井一生
//
//=============================================================================
#include "crate.h"
#include "game.h"

#include "camera.h"
#include "player.h"

#include "checkhit.h"
/*
メシュの箱の処理

mesh box
*/

//=============================================================================
// マクロ定義
#define TEXTURE_WOOD_CRATE			("data/TEXTURE/crate000.png")		// 木の箱
#define TEXTURE_METAL_CRATE			("data/TEXTURE/crate001.png")		// 鉄の箱

#define MODEL			("data/MODEL/box.x")							// 箱のモデル


//#define TEXTURE_TEST				("data/TEXTURE/cave_entrance000.png")	// 鉄の箱
#define MODEL_TEST		("data/MAP/cliff_pt2.x")							// 箱のモデル


#define	VALUE_MOVE		(0.5f)											// 移動量
#define	ROT_VALUE		(D3DX_PI * 0.02f)								// 回転量
#define SCL_VALUE		(0.5f)											// 大きさ
#define RANGE_VALUE		(50)											// 箱からの距離

#define GRND_CHECK_HIGHT	(100.0f)		// 地面との当たり判定の線分の長さ	hight of line segment for ground check;
#define OBJ_CHECK_HIGHT		(90.0f)			// オブジェクトとの当たり判定の線分の長さの違い	difference between hight of line segment for object and ground check;


enum TEXTURE_NAME
{
	WOOD_TEX = 0,
	METAL_TEX,
	TEX_MAX,
};



//=============================================================================
// 頂点構造体


//=============================================================================
// プロトタイプ宣言

void SetCrateData(int i,int num, int data);

void MoveCrate(void);


void DrawCrateHitBox(void);


void SetCratePos(int no);

//=============================================================================
// グローバル変数
LPDIRECT3DTEXTURE9		D3DTextureCrate[TEX_MAX] = { NULL, NULL };		// テクスチャへのポインタ

CRATE					crateWk[CRATE_MAX];			// フィールドへのポインタ

D3DXVECTOR3				OldPos[CRATE_MAX];			// 古い位置

#ifdef _DEBUG

#endif

//=============================================================================
// 初期化処理
HRESULT InitCrate(void)
{
	CRATE *crate = &crateWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの設定
	D3DXCreateTextureFromFile(pDevice, TEXTURE_WOOD_CRATE, &D3DTextureCrate[WOOD_TEX]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_METAL_CRATE, &D3DTextureCrate[METAL_TEX]);

	FILE *fp;
	// オブジェクトデータが入ってるファイルを開く
	fp = fopen(OBJECT_DATA_FILE, "r");
	if (fp == NULL)
	{
		perror("Error opening file");
		return(-1);
	}

	// 箱のデータを検索
	char tbuf1[MAX_PATH];
	while (1)
	{
		fscanf(fp, "%s", &tbuf1[0]);
		if (strcmp(&tbuf1[0], CRATE_DATA_START) == 0) // 見つけたらブレーク
			break;
	}

	int i;
	for (i = 0; i < CRATE_MAX; i++, crate++)
	{
		// ストリングにデータを保存
		fscanf(fp, "%s", &tbuf1[0]);

		// ストリングからイントデータへの変換
		int j = 0;
		int adres = 0;
		int tdata = 0;
		int cnt = 0;
		bool flag = true;
		while (flag)
		{
			char check = tbuf1[j];
			if (check == ',')
			{
				tdata = atoi(&tbuf1[adres]);
				SetCrateData(i, cnt, tdata);
				cnt++;
				j++;
				adres = j;
			}
			else if (check == '\0')
			{
				flag = false;
				tdata = atoi(&tbuf1[adres]);
				SetCrateData(i, cnt, tdata);
			}
			else
				j++;
		}


		//crate->mesh.use = true;
		//// 位置・回転・スケールの初期設定
		//SetCratePos(i);	// 位置をセット
		//crate->mesh.TexId = METAL_TEX;



		crate->mesh.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		crate->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);

		OldPos[i] = { 0.0f,0.0f,0.0f };
		crate->mesh.Grav = 0.0f;

		crate->isSolid = true;

		// モデル関係の初期化
		crate->mesh.VtxBuff = NULL;
		crate->mesh.Mesh = NULL;
		crate->mesh.BuffMat = NULL;
		crate->mesh.NumMat = 0;

		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(MODEL,			// モデルデータ
			D3DXMESH_SYSTEMMEM,					// 使用するメモリのオプション
			pDevice,							// デバイス 
			NULL,								// 未使用
			&crate->mesh.BuffMat,				// マテリアルデータへのポインター
			NULL,								// 未使用
			&crate->mesh.NumMat,				// D3DXMATERIAL構造体の数
			&crate->mesh.Mesh)))				// メッシュデータへのポインター
		{
			return E_FAIL;
		}

		SetVtxData(&crate->mesh);

	}
	return S_OK;

}

//=============================================================================
// ストリングからイントデータへの変換
// i	: 現在対象の箱の番号
// num  : 入れたいデータの種類番号
// data : 入れたいデータ
//=============================================================================
void SetCrateData(int i,int num, int data)
{
	CRATE *crate = &crateWk[i];
	switch (num)
	{
	case FtD_USE:
		crate->mesh.use = data;
		return;
	case FtD_TYPE:
		crate->mesh.TexId = data;
		return;
	case FtD_POS_X:
		crate->mesh.Pos.x = data;
		return;
	case FtD_POS_Y:
		crate->mesh.Pos.y = data;
		return;
	case FtD_POS_Z:
		crate->mesh.Pos.z = data;
		return;
	}
}


//=============================================================================
// 終了処理
void UninitCrate(void)
{
	CRATE *crate = &crateWk[0];

	for (int i = 0; i < TEX_MAX; i++)
	{
		// テクスチャの解放
		SAFE_RELEASE(D3DTextureCrate[i]);
	}

	for (int i = 0; i < CRATE_MAX; i++, crate++)
	{
		// 頂点バッファの開放
		SAFE_RELEASE(crate->mesh.VtxBuff);
		// メシュの解放
		SAFE_RELEASE(crate->mesh.Mesh);

		if (crate->mesh.IdxBuff != NULL)
		{// インデックスバッファとヴァーテックスポスは一緒に開放
			free(crate->mesh.IdxBuff);
			free(crate->mesh.VtxPos);
			crate->mesh.IdxBuff = NULL;
			crate->mesh.VtxPos = NULL;
		}
	}
}

//=============================================================================
// 更新処理
void UpdateCrate(void)
{
	CRATE *crate = &crateWk[0];
	PLAYER *player = GetPlayer(0);
	int i;
	for (i = 0; i < CRATE_MAX; i++)
	{
		if ((crate + i)->mesh.use)
			if ((crate + i)->mesh.Pos != OldPos[i])
			{
				D3DXVECTOR3 distance = { 0.0f,0.0f,0.0f };
				D3DXVECTOR3 head = (crate + i)->mesh.Pos;	// 箱の上の麺
				D3DXVECTOR3 feet = (crate + i)->mesh.Pos;	// 箱の下の麺
				head.y += GRND_CHECK_HIGHT;
				feet.y += (crate + i)->mesh.Grav;
				bool tFlg = false;							// 箱の下に何かあるか

				if (CheckHitGroundPrep(head, feet, &distance))	// 地面との当たり判定
					tFlg = true;

				head.y -= OBJ_CHECK_HIGHT;

				if (tFlg == false)
					for (int j = 0; j < CRATE_MAX; j++)
						if (j != i)	// 他の箱との当たり判定
							if (CheckHitLinePrep(head, feet, &distance, &(crate + j)->mesh))
							{
								tFlg = true;
								break;
							}

				if (tFlg == true)
				{	// 下に何かあるから重力を足さない
					(crate + i)->mesh.Pos.y += distance.y - 5;
					(crate + i)->mesh.Grav = 0.0f;
				}
				else
				{	// 下に何もないので重力を足す
					(crate + i)->mesh.Grav -= GRAVITY;
				}
			}
		OldPos[i] = (crate + i)->mesh.Pos;
		(crate + i)->mesh.Pos.y += (crate + i)->mesh.Grav;
	}
}

//=============================================================================
// 描画処理
void DrawCrate(void)
{
	CRATE *crate = &crateWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// スケール、回転、平行移動
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	int i, j;

	for (i = 0; i < CRATE_MAX; i++, crate++)
		if (crate->mesh.use == true)
		{



			// ワールドマトリクス初期化
			D3DXMatrixIdentity(&crate->mesh.mtxWorld);

			// スケールを反映	(S)
			D3DXMatrixScaling(&mtxScl, crate->mesh.Scl.x, crate->mesh.Scl.y, crate->mesh.Scl.z);
			D3DXMatrixMultiply(&crate->mesh.mtxWorld, &crate->mesh.mtxWorld, &mtxScl); 	// &mesh->mtxWorld = &mesh->mtxWorld * &mtxScl
			// 回転を反映		(R)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, crate->mesh.Rot.y, crate->mesh.Rot.x, crate->mesh.Rot.z);
			D3DXMatrixMultiply(&crate->mesh.mtxWorld, &crate->mesh.mtxWorld, &mtxRot);	// &mesh->mtxWorld = &mesh->mtxWorld * &mtxRot
			// 平行移動を反映	(T)
			D3DXMatrixTranslation(&mtxTranslate, crate->mesh.Pos.x, crate->mesh.Pos.y, crate->mesh.Pos.z);
			D3DXMatrixMultiply(&crate->mesh.mtxWorld, &crate->mesh.mtxWorld, &mtxTranslate);

			// ワールドマトリックス設定
			pDevice->SetTransform(D3DTS_WORLD, &crate->mesh.mtxWorld);

			// 現在のマテリアルを保存
			pDevice->GetMaterial(&matDef);

			// マテリアル情報へのポインタを
			pD3DXMat = (D3DXMATERIAL*)crate->mesh.BuffMat->GetBufferPointer();


			for (j = 0; j < (int)crate->mesh.NumMat; j++)
			{
				pDevice->SetMaterial(&pD3DXMat[j].MatD3D);	// マテリアルの設定

				switch (crate->mesh.TexId)
				{
				case WOOD_TEX:
					pDevice->SetTexture(0, D3DTextureCrate[WOOD_TEX]);	// テクスチャの設定
					break;
				case METAL_TEX:
					pDevice->SetTexture(0, D3DTextureCrate[METAL_TEX]);	// テクスチャの設定
					break;
				}
				crate->mesh.Mesh->DrawSubset(j);
			}


			// マテリアルに戻す
			pDevice->SetMaterial(&matDef);
		}
}

//=============================================================================
// 箱の移動
void MoveCrate(void)
{
	CRATE *crate = &crateWk[0];
	PLAYER *player = GetPlayer(0);

}

//=============================================================================
// 箱のゲット関数
CRATE *GetCrate(int no)
{
	return (&crateWk[no]);
}

//=============================================================================
// 箱の位置をセットする処理
void SetCratePos(int no)
{
	CRATE *crate = &crateWk[no];

	switch (no)
	{
	case 0:
		crate->mesh.Pos = D3DXVECTOR3(-40.0f, 460.0f, -2220.0f);
		break;
	case 1:
		crate->mesh.Pos = D3DXVECTOR3(-500.0f, 10.0f, 1100.0f);
		break;
	case 2:
		crate->mesh.Pos = D3DXVECTOR3(-500.0f, 110.0f, 1100.0f);
		break;
	case 3:
		crate->mesh.Pos = D3DXVECTOR3(-444.0f, 10.0f, 1186.0f);
		break;
	case 4:
		crate->mesh.Pos = D3DXVECTOR3(-40.0f, 460.0f, -2270.0f);
		break;
	case 5:
		crate->mesh.Pos = D3DXVECTOR3(-40.0f, 500.0f, -2270.0f);
		break;
	case 6:
		crate->mesh.Pos = D3DXVECTOR3(10.0f, 460.0f, -2270.0f);
		break;
	case 7:
		crate->mesh.Pos = D3DXVECTOR3(10.0f, 500.0f, -2270.0f);
		break;
	case 8:
		crate->mesh.Pos = D3DXVECTOR3(10.0f, 550.0f, -2270.0f);
		break;
	case 9:
		crate->mesh.Pos = D3DXVECTOR3(10.0f, 460.0f, -2220.0f);
		break;
	case 10:
		crate->mesh.Pos = D3DXVECTOR3(10.0f, 500.0f, -2220.0f);
		break;
	case 11:
		crate->mesh.Pos = D3DXVECTOR3(10.0f, 550.0f, -2220.0f);
		break;
	case 12:
		crate->mesh.Pos = D3DXVECTOR3(10.0f, 600.0f, -2220.0f);
		break;

		// starting area
	case 13:
		crate->mesh.Pos = D3DXVECTOR3(-1640.0f, 640.0f, -1630.0f);
		break;
	case 14:
		crate->mesh.Pos = D3DXVECTOR3(-1490.0f, 640.0f, -2240.0f);
		break;
	case 15:
		crate->mesh.Pos = D3DXVECTOR3(-1490.0f, 690.0f, -2240.0f);
		break;
	case 16:
		crate->mesh.Pos = D3DXVECTOR3(-1590.0f, 640.0f, -2260.0f);
		break;

	}
}
