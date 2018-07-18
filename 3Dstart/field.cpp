//=============================================================================
//
// 地面処理 [field.cpp]
// Author : 川井一生
//
//=============================================================================
#include "field.h"
#include "input.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_FIELD	"data/TEXTURE/field000.jpg"						// 読み込むテクスチャファイル名	name of the teture file

#define	VALUE_MOVE		(0.5f)											// 移動量						moving distance
#define	VALUE_ROTATE	(D3DX_PI * 0.02f)								// 回転量						rotation amount

#define	SIZE_X			(50.0f)										// 地面のサイズ(X方向)			size of the x axis
#define	SIZE_Z			(50.0f)										// 地面のサイズ(Z方向)			size of the y axis



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void GetKeyboardMove(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureField = NULL;	// テクスチャへのポインタ			pointer for texture
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffField = NULL;	// 頂点バッファへのポインタ		pointer for vertex buffer
LPDIRECT3DINDEXBUFFER9 g_pD3DIdxBuffField;		// インデックスバッファインターフェースへのポインタ

FIELD					fieldWk[FIELD_MAX];			// フィールドへのポインタ		pointer for field



//D3DXMATRIX g_mtxWorldField;						// ワールドマトリックス
//D3DXVECTOR3 g_posField;							// ポリゴン表示位置の中心座標
//D3DXVECTOR3 g_rotField;							// ポリゴンの回転角




//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int BlkCntX, int BlkCntZ, float BlkSizeX, float BlkSizeZ)
{
	FIELD *field = &fieldWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	field->Pos = pos;
	field->Rot = rot;

	D3DXCreateTextureFromFile(pDevice, TEXTURE_FIELD, &D3DTextureField);

	// ブロック数の設定
	field->BlkCntX = BlkCntX;
	field->BlkCntZ = BlkCntZ;

	// 頂点数の設定
	field->Vtx = (BlkCntX + 1) * (BlkCntZ + 1);

	// インデックス数の設定
	field->VtxIdx = (BlkCntX + 1) * 2 * BlkCntZ + (BlkCntZ - 1) * 2;

	// ポリゴン数の設定
	field->PolyCnt = BlkCntX * BlkCntZ * 2 + (BlkCntZ - 1) * 4;	

	// ブロックサイズの設定
	field->BlkSizeX = BlkSizeX;
	field->BlkSizeZ = BlkSizeZ;

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * field->Vtx,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_3D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffField,				// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}

	// オブジェクトのインデックスバッファを生成
	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * field->VtxIdx,		// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		D3DFMT_INDEX16,						// 使用するインデックスフォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DIdxBuffField,				// インデックスバッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

#if 0
	const float texSizeX = 1.0f / g_nNumBlockX;
	const float texSizeZ = 1.0f / g_nNumBlockZ;
#else
	const float texSizeX = 1.0f;
	const float texSizeZ = 1.0f;
#endif

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtxZ = 0; nCntVtxZ < (field->BlkCntZ + 1); nCntVtxZ++)
	{
		for (int nCntVtxX = 0; nCntVtxX < (field->BlkCntX + 1); nCntVtxX++)
		{
			// 頂点座標の設定
			pVtx[nCntVtxZ * (field->BlkCntX + 1) + nCntVtxX].vtx.x = -(field->BlkCntX / 2.0f) * field->BlkSizeX + nCntVtxX * field->BlkSizeX;
			pVtx[nCntVtxZ * (field->BlkCntX + 1) + nCntVtxX].vtx.y = 0.0f;
			pVtx[nCntVtxZ * (field->BlkCntX + 1) + nCntVtxX].vtx.z = (field->BlkCntZ / 2.0f) * field->BlkSizeZ - nCntVtxZ * field->BlkSizeZ;

			// 法線の設定
			pVtx[nCntVtxZ * (field->BlkCntX + 1) + nCntVtxX].normal = D3DXVECTOR3(0.0f, 1.0, 0.0f);

			// 反射光の設定
			pVtx[nCntVtxZ * (field->BlkCntX + 1) + nCntVtxX].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[nCntVtxZ * (field->BlkCntX + 1) + nCntVtxX].tex.x = texSizeX * nCntVtxX;
			pVtx[nCntVtxZ * (field->BlkCntX + 1) + nCntVtxX].tex.y = texSizeZ * nCntVtxZ;
		}
	}

	// 頂点データをアンロックする
	g_pD3DVtxBuffField->Unlock();
	}

	{//インデックスバッファの中身を埋める
	WORD *pIdx;

	// インデックスデータの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DIdxBuffField->Lock(0, 0, (void**)&pIdx, 0);

	int nCntIdx = 0;
	for (int nCntVtxZ = 0; nCntVtxZ < field->BlkCntZ; nCntVtxZ++)
	{
		if (nCntVtxZ > 0)
		{// 縮退ポリゴンのためのダブりの設定
			pIdx[nCntIdx] = (nCntVtxZ + 1) * (field->BlkCntX + 1);
			nCntIdx++;
		}

		for (int nCntVtxX = 0; nCntVtxX < (field->BlkCntX + 1); nCntVtxX++)
		{
			pIdx[nCntIdx] = (nCntVtxZ + 1) * (field->BlkCntX + 1) + nCntVtxX;
			nCntIdx++;
			pIdx[nCntIdx] = nCntVtxZ * (field->BlkCntX + 1) + nCntVtxX;
			nCntIdx++;
		}

		if (nCntVtxZ < (field->BlkCntZ - 1))
		{// 縮退ポリゴンのためのダブりの設定
			pIdx[nCntIdx] = nCntVtxZ * (field->BlkCntX + 1) + field->BlkCntX;
			nCntIdx++;
		}
	}

	// インデックスデータをアンロックする
	g_pD3DIdxBuffField->Unlock();
	}


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitField(void)
{
	if(D3DTextureField != NULL)
	{// テクスチャの開放
		D3DTextureField->Release();
		D3DTextureField = NULL;
	}

	if(g_pD3DVtxBuffField != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffField->Release();
		g_pD3DVtxBuffField = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateField(void)
{



}

//=============================================================================
// 描画処理
//=============================================================================
void DrawField(void)
{
	FIELD *field = &fieldWk[0];
	CAMERA *camera = GetCamera(0);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;	// 回転、平行移動
	int i;
	for (i = 0; i < FIELD_MAX; i++, field++)
	{
		// ワールドマトリクス初期化
		D3DXMatrixIdentity(&field->mtxWorld);


		// 回転を反映		(R)
		D3DXMatrixRotationYawPitchRoll(&mtxRot, field->Rot.y, field->Rot.x, field->Rot.z);
		D3DXMatrixMultiply(&field->mtxWorld, &field->mtxWorld, &mtxRot);	// &field->mtxWorld = &field->mtxWorld * &mtxRot

		// 平行移動を反映	(T)
		D3DXMatrixTranslation(&mtxTranslate, field->Pos.x, field->Pos.y, field->Pos.z);
		D3DXMatrixMultiply(&field->mtxWorld, &field->mtxWorld, &mtxTranslate);

		// ワールドマトリックス設定
		pDevice->SetTransform(D3DTS_WORLD, &field->mtxWorld);

		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pD3DVtxBuffField, 0, sizeof(VERTEX_3D));

		// 頂点フォーマット設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// クスチャの設定
		pDevice->SetTexture(0, D3DTextureField);

		// ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, field->Vtx, 0, NUM_POLYGON);
	}
}

void GetKeyboardMove(void)
{
	FIELD *field = &fieldWk[0];
}

void *GetField(int no)
{
	return (&fieldWk[no]);
}