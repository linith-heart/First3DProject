//=============================================================================
//
// 策の処理 [fence.cpp]
// Author : 川井一生
//
//=============================================================================
#include "fence.h"

#include "player.h"

#include "checkhit.h"
/*
メシュの策の処理

mesh fence
*/

//=============================================================================
// マクロ定義
#define TEX_FILE_FENCE000	("data/TEXTURE/fence000.png")					// 石のテクスチャー
#define MODEL000			("data/MODEL/fence000.x")						// 石のモデル
#define MODELHIT000			("data/MODEL/fence_hitbox000.x")				// 石のモデル
#define	VALUE_MOVE			(0.5f)											// 移動量
#define	ROT_VALUE			(D3DX_PI * 0.02f)								// 回転量
#define SCL_VALUE			(1.0f)											// 大きさ
#define RANGE_VALUE			(50)											// 箱からの距離


enum
{
	TEX_FENCE000 = 0,
	TEX_MAX,
};


//=============================================================================
// プロトタイプ宣言
void MoveFence(void);
void SetFencePos(int no);

//=============================================================================
// グローバル変数
LPDIRECT3DTEXTURE9		D3DTextureFence[TEX_MAX] = { NULL };		// テクスチャへのポインタ

FENCE					fenceWk[FENCE_MAX];			// フィールドへのポインタ


//=============================================================================
// 初期化処理
HRESULT InitFence(void)
{
	FENCE *fence = &fenceWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの設定
	D3DXCreateTextureFromFile(pDevice, TEX_FILE_FENCE000, &D3DTextureFence[TEX_FENCE000]);

	int i;
	for (i = 0; i < FENCE_MAX; i++, fence++)
	{
		fence->mesh.use = true;

		// 位置・回転・スケールの初期設定
		SetFencePos(i);

		// モデル関係の初期化
		fence->mesh.VtxBuff = NULL;
		//MakeVertexFence(pDevice);
		fence->mesh.Mesh = NULL;
		fence->mesh.BuffMat = NULL;
		fence->mesh.NumMat = 0;

		// Xファイルの読み込み

		if (i % COMP4FENCE == 0)
		{
			if (FAILED(D3DXLoadMeshFromX(MODEL000,	// モデルデータ
				D3DXMESH_SYSTEMMEM,					// 使用するメモリのオプション
				pDevice,							// デバイス 
				NULL,								// 未使用
				&fence->mesh.BuffMat,				// マテリアルデータへのポインター
				NULL,								// 未使用
				&fence->mesh.NumMat,				// D3DXMATERIAL構造体の数
				&fence->mesh.Mesh)))				// メッシュデータへのポインター
			{
				return E_FAIL;
			}
			fence->isHitbox = false;
		}
		else if (i% COMP4FENCE == 1)
		{
			if (FAILED(D3DXLoadMeshFromX(MODELHIT000,// モデルデータ
				D3DXMESH_SYSTEMMEM,					// 使用するメモリのオプション
				pDevice,							// デバイス 
				NULL,								// 未使用
				&fence->mesh.BuffMat,				// マテリアルデータへのポインター
				NULL,								// 未使用
				&fence->mesh.NumMat,				// D3DXMATERIAL構造体の数
				&fence->mesh.Mesh)))				// メッシュデータへのポインター
			{
				return E_FAIL;
			}
			fence->isHitbox = true;

			int			VtxCnt = fence->mesh.Mesh->GetNumVertices();	// メシュの頂点数
			int			Polycnt = fence->mesh.Mesh->GetNumFaces();		// メシュのポリゴン数
			int			BytePerVtx = fence->mesh.Mesh->GetNumBytesPerVertex();	// １頂点当たりのバイト数
			WORD		*idx;
			char		*pVtx;

			// ポリゴン数 * 3 だけインデックスが必要	
			// インデックスひとつのサイズはWORD型 = 2バイトなので,
			// インデックスを格納する配列を(ポリゴン数 * 3頂点 * 2バイト)分だけ動的確保
			// 終了時に free(fence->mesh.IdxBuff); でメモリ開放を忘れずに
			fence->mesh.IdxBuff = (WORD*)malloc(sizeof(WORD) * Polycnt * 3);

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			fence->mesh.Mesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&idx);
			// インデックスバファの情報を取得
			for (int i = 0; i < (Polycnt * 3); i++)
			{
				fence->mesh.IdxBuff[i] = idx[i];
			}

			// 頂点データをアンロックする
			fence->mesh.Mesh->UnlockIndexBuffer();


			// 頂点座標ひとつのサイズはD3DXVECTOR3型 = 12バイトなので,
			// 頂点座標を格納する配列を(頂点数 * 12バイト)分だけ動的確保
			// 終了時に free(fence->mesh.VtxPos); でメモリ開放を忘れずに
			fence->mesh.VtxPos = (D3DXVECTOR3*)malloc(sizeof(D3DXVECTOR3) * VtxCnt);

			// バーテックスデータをロック
			fence->mesh.Mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtx);

			// バーテックスデータを取得
			for (int i = 0; i < VtxCnt; i++)
			{
				fence->mesh.VtxPos[i] = *(D3DXVECTOR3*)&pVtx[i * BytePerVtx];
			}

			// 頂点データをアンロックする
			fence->mesh.Mesh->UnlockVertexBuffer();
		}
	}
	return S_OK;

}

//=============================================================================
// 終了処理
void UninitFence(void)
{
	FENCE *fence = &fenceWk[0];
	for (int i = 0; i < TEX_MAX; i++)
	{
		if (D3DTextureFence[i] != NULL)
		{// テクスチャの開放
			D3DTextureFence[i]->Release();
			D3DTextureFence[i] = NULL;
		}
	}
	for (int i = 0; i < FENCE_MAX; i++,fence++)
	{
		if (fence->mesh.VtxBuff != NULL)
		{// 頂点バッファの開放
			fence->mesh.VtxBuff->Release();
			fence->mesh.VtxBuff = NULL;
		}

		if (fence->mesh.Mesh != NULL)
		{// メシュの解放
			fence->mesh.Mesh->Release();
			fence->mesh.Mesh = NULL;
		}

		if (fence->mesh.IdxBuff != NULL)
		{// インデックスバッファとヴァーテックスポスは一緒に開放
			free(fence->mesh.IdxBuff);
			free(fence->mesh.VtxPos);
			fence->mesh.IdxBuff = NULL;
			fence->mesh.VtxPos = NULL;
		}

	}
}

//=============================================================================
// 更新処理
void UpdateFence(void)
{
	FENCE *fence = &fenceWk[0];
	PLAYER *player = GetPlayer(0);
	D3DXVECTOR3 Vel = { 0.0f,0.0f,0.0f };
	int i;
	for (i = 0; i < FENCE_MAX; i++, fence++)
	{

	}
}

//=============================================================================
// 描画処理
void DrawFence(void)
{
	FENCE *fence = &fenceWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// スケール、回転、平行移動
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	int i, j;

	for (i = 0; i < FENCE_MAX; i++, fence++)
		if (fence->mesh.use == true)
		{
			// ワールドマトリクス初期化
			D3DXMatrixIdentity(&fence->mesh.mtxWorld);

			// スケールを反映	(S)
			D3DXMatrixScaling(&mtxScl, fence->mesh.Scl.x, fence->mesh.Scl.y, fence->mesh.Scl.z);
			D3DXMatrixMultiply(&fence->mesh.mtxWorld, &fence->mesh.mtxWorld, &mtxScl); 	// &mesh->mtxWorld = &mesh->mtxWorld * &mtxScl
																						// 回転を反映		(R)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, fence->mesh.Rot.y, fence->mesh.Rot.x, fence->mesh.Rot.z);
			D3DXMatrixMultiply(&fence->mesh.mtxWorld, &fence->mesh.mtxWorld, &mtxRot);	// &mesh->mtxWorld = &mesh->mtxWorld * &mtxRot
																						// 平行移動を反映	(T)
			D3DXMatrixTranslation(&mtxTranslate, fence->mesh.Pos.x, fence->mesh.Pos.y, fence->mesh.Pos.z);
			D3DXMatrixMultiply(&fence->mesh.mtxWorld, &fence->mesh.mtxWorld, &mtxTranslate);

			// ワールドマトリックス設定
			pDevice->SetTransform(D3DTS_WORLD, &fence->mesh.mtxWorld);

			// 現在のマテリアルを保存
			pDevice->GetMaterial(&matDef);

			// マテリアル情報へのポインタを
			pD3DXMat = (D3DXMATERIAL*)fence->mesh.BuffMat->GetBufferPointer();

			if (i % 2 == 0)
				for (j = 0; j < (int)fence->mesh.NumMat; j++)
				{
					pDevice->SetMaterial(&pD3DXMat[j].MatD3D);	// マテリアルの設定

					pDevice->SetTexture(0, D3DTextureFence[fence->mesh.TexId]);	// テクスチャの設定
					fence->mesh.Mesh->DrawSubset(j);
				}

			// マテリアルに戻す
			pDevice->SetMaterial(&matDef);
		}
}

//=============================================================================
// 策の移動
void MoveFence(void)
{
	FENCE *fence = &fenceWk[0];
	PLAYER *player = GetPlayer(0);

}

//=============================================================================
// 策のゲット関数
FENCE *GetFence(int no)
{
	return (&fenceWk[no]);
}






int hitCheckFence(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
{
	FENCE *fence = &fenceWk[0];
	int				ans = 0;	// 一先ず当たっていないをセットしておく

	D3DXVECTOR3		pos[3];		// ポリゴンの法線
	D3DXVECTOR3		vel = { 0.0f,0.0f,0.0f };


	int PolyCnt = fence->mesh.Mesh->GetNumFaces();

	for (int i = 0; i<PolyCnt; i++)
	{	// まずは、ポリゴンの頂点を求める
		int idx0 = fence->mesh.IdxBuff[i * 3];
		int idx1 = fence->mesh.IdxBuff[i * 3 + 1];
		int idx2 = fence->mesh.IdxBuff[i * 3 + 2];

		// 頂点座標の設定
		pos[0] = fence->mesh.VtxPos[idx0];
		pos[1] = fence->mesh.VtxPos[idx1];
		pos[2] = fence->mesh.VtxPos[idx2];

		// 左下側ポリゴンと線分の当たり判定
		ans = CheckHitLine(pos[0], pos[2], pos[3], pos0, pos1, &fence->mesh.mtxWorld, &vel);
		if (ans != 0)
		{
			break;
		}


	}

	return(ans);

}

void SetFencePos(int no)
{
	FENCE *fence = &fenceWk[no];

	switch (no)
	{
	case 0: case 1:
		fence->mesh.Pos = D3DXVECTOR3(-580.0f , 454.0f, -1900.0f);
		fence->mesh.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		fence->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);
		break;
	case 2: case 3:
		fence->mesh.Pos = D3DXVECTOR3(-580.0f, 454.0f, -2350.0f);
		fence->mesh.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		fence->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);
		break;
	case 4: case 5:
		fence->mesh.Pos = D3DXVECTOR3(-340.0f , 430.0f, -1800.0f);
		fence->mesh.Rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5, 0.0f);
		fence->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);
		break;
	case 6: case 7:
		fence->mesh.Pos = D3DXVECTOR3(700.0f , 10.0f, -950.0f);
		fence->mesh.Rot = D3DXVECTOR3(0.0f, D3DX_PI * -0.25, 0.0f);
		fence->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);
		break;
	case 8: case 9:
		fence->mesh.Pos = D3DXVECTOR3(550.0f, 10.0f, -800.0f);
		fence->mesh.Rot = D3DXVECTOR3(0.0f, D3DX_PI * -0.25, 0.0f);
		fence->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);
		break;
	case 10: case 11:
		fence->mesh.Pos = D3DXVECTOR3(-600.0f , 0.0f, 50.0f);
		fence->mesh.Rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5, 0.0f);
		fence->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);
		break;
	case 12: case 13:
		fence->mesh.Pos = D3DXVECTOR3(-345.0f , 0.0f, 155.0f);
		fence->mesh.Rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.25, 0.0f);
		fence->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);
		break;
	case 14: case 15:
		fence->mesh.Pos = D3DXVECTOR3(-200.0f , 0.0f, -100.0f);
		fence->mesh.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		fence->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);
		break;
	case 16: case 17:
		fence->mesh.Pos = D3DXVECTOR3(-200.0f , 0.0f, -100.0f);
		fence->mesh.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		fence->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);
		break;
	case 18: case 19:
		fence->mesh.Pos = D3DXVECTOR3(-200.0f , 0.0f, -100.0f);
		fence->mesh.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		fence->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);
		break;
	case 20: case 21:
		fence->mesh.Pos = D3DXVECTOR3(-200.0f , 0.0f, -100.0f);
		fence->mesh.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		fence->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);
		break;
	}
}