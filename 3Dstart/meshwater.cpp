//=============================================================================
//
// メッシュ地面の処理 [meshwater.cpp]
// Author : 
//
//=============================================================================
#include "meshwater.h"

//=============================================================================
// マクロ定義
#define	TEXTURE_FILENAME	"data/TEXTURE/sem_clear_water002.png"		// 読み込むテクスチャファイル名

#define WAVE_RATO		(0.05f)		// 波の変換度
#define WAVE_HIGH		(20.0f)		// 波の最大の高さ

#define SURACE_HIGH		(-100.0f)		// 水面の高さ


//=============================================================================
// グローバル変数
LPDIRECT3DTEXTURE9 g_pD3DTextureField;			// テクスチャ読み込み場所
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffField;		// 頂点バッファインターフェースへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pD3DIdxBuffField;		// インデックスバッファインターフェースへのポインタ

D3DXMATRIX g_mtxWorldField;						// ワールドマトリックス
D3DXVECTOR3 g_posField;							// ポリゴン表示位置の中心座標
D3DXVECTOR3 g_rotField;							// ポリゴンの回転角

int g_nNumBlockXField, g_nNumBlockZField;		// ブロック数
int g_nNumVertexField;							// 総頂点数	
int g_nNumVertexIndexField;						// 総インデックス数
int g_nNumPolygonField;							// 総ポリゴン数
float g_fBlockSizeXField, g_fBlockSizeZField;	// ブロックサイズ

float WaveBase = 0.0f;
bool WaveUp = true;

//=============================================================================
// 初期化処理
HRESULT InitMeshWater(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
							int nNumBlockX, int nNumBlockZ, float fBlockSizeX, float fBlockSizeZ)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ポリゴン表示位置の中心座標を設定
	g_posField = pos;

	g_rotField = rot;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
								TEXTURE_FILENAME,		// ファイルの名前
								&g_pD3DTextureField);	// 読み込むメモリー

	// ブロック数の設定
	g_nNumBlockXField = nNumBlockX;
	g_nNumBlockZField = nNumBlockZ;

	// 頂点数の設定
	g_nNumVertexField = (nNumBlockX + 1) * (nNumBlockZ + 1);

	// インデックス数の設定
	g_nNumVertexIndexField = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;

	// ポリゴン数の設定
	g_nNumPolygonField = nNumBlockX * nNumBlockZ * 2 + (nNumBlockZ - 1) * 4;

	// ブロックサイズの設定
	g_fBlockSizeXField = fBlockSizeX;
	g_fBlockSizeZField = fBlockSizeZ;

	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVertexField,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
												FVF_VERTEX_3D,						// 使用する頂点フォーマット
												D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffField,				// 頂点バッファインターフェースへのポインタ
												NULL)))								// NULLに設定
	{
        return E_FAIL;
	}

	// オブジェクトのインデックスバッファを生成
    if(FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * g_nNumVertexIndexField,		// 頂点データ用に確保するバッファサイズ(バイト単位)
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
		g_pD3DVtxBuffField->Lock( 0, 0, (void**)&pVtx, 0 );

		for(int nCntVtxZ = 0; nCntVtxZ < (g_nNumBlockZField + 1); nCntVtxZ++)
		{
			for(int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXField + 1); nCntVtxX++)
			{
				// 頂点座標の設定
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.x = -(g_nNumBlockXField / 2.0f) * g_fBlockSizeXField + nCntVtxX * g_fBlockSizeXField;
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y = 0.0f;
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.z = (g_nNumBlockZField / 2.0f) * g_fBlockSizeZField - nCntVtxZ * g_fBlockSizeZField;

				// 法線の設定
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].normal = D3DXVECTOR3(0.0f, 1.0, 0.0f);

				// 反射光の設定
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標の設定
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].tex.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].tex.y = texSizeZ * nCntVtxZ;
			}
		}

		// 頂点データをアンロックする
		g_pD3DVtxBuffField->Unlock();
	}

	{//インデックスバッファの中身を埋める
		WORD *pIdx;

		// インデックスデータの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DIdxBuffField->Lock( 0, 0, (void**)&pIdx, 0 );

		int nCntIdx = 0;
		for(int nCntVtxZ = 0; nCntVtxZ < g_nNumBlockZField; nCntVtxZ++)
		{
			if(nCntVtxZ > 0)
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (g_nNumBlockXField + 1);
				nCntIdx++;
			}

			for(int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXField + 1); nCntVtxX++)
			{
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (g_nNumBlockXField + 1) + nCntVtxX;
				nCntIdx++;
				pIdx[nCntIdx] = nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX;
				nCntIdx++;
			}

			if(nCntVtxZ < (g_nNumBlockZField - 1))
			{// 縮退ポリゴンのためのダブりの設定
				pIdx[nCntIdx] = nCntVtxZ * (g_nNumBlockXField + 1) + g_nNumBlockXField;
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
void UninitMeshWater(void)
{
	if(g_pD3DVtxBuffField)
	{// 頂点バッファの開放
		g_pD3DVtxBuffField->Release();
		g_pD3DVtxBuffField = NULL;
	}

	if(g_pD3DIdxBuffField)
	{// インデックスバッファの開放
		g_pD3DIdxBuffField->Release();
		g_pD3DIdxBuffField = NULL;
	}

	if(g_pD3DTextureField)
	{// テクスチャの開放
		g_pD3DTextureField->Release();
		g_pD3DTextureField = NULL;
	}
}

//=============================================================================
// 更新処理
void UpdateMeshWater(void)
{
	//===============================================================
	// なみの処理
	VERTEX_3D *pVtx;
	float	wave	= WaveBase;	// 頂点の座標をグローバルの座標をベースに設定
	bool	TempUp	= WaveUp;	// 上がるか下がるかグローバルからローカルへ
	// 頂点データをロック
	g_pD3DVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);
	for (int z = 0; z < (g_nNumBlockZField + 1); z++)
		for (int x = 0; x < (g_nNumBlockXField + 1); x++)
		{
			//if (TempUp == true)
			//{	// 上げる
			//	if (wave >= 1.0f + WAVE_RATO)
			//		TempUp = false;
			//	else
			//		wave += WAVE_RATO;
			//}
			//else
			//{	// 下げる
			//	if (wave <= 0.0f - WAVE_RATO)
			//		TempUp = true;
			//	else
			//		wave -= WAVE_RATO;
			//}
			// 頂点座標の設定
			if (x % 2 == 0)
			{
				if (z % 2 == 0)
					pVtx[z * (g_nNumBlockXField + 1) + x].vtx.y = SURACE_HIGH + (cosf(wave)*WAVE_HIGH);
				else
					pVtx[z * (g_nNumBlockXField + 1) + x].vtx.y = SURACE_HIGH + (-cosf(wave)*WAVE_HIGH);
			}
			else
			{
				if (z % 2 == 0)
					pVtx[z * (g_nNumBlockXField + 1) + x].vtx.y = SURACE_HIGH + (sinf(wave)*WAVE_HIGH);
				else
					pVtx[z * (g_nNumBlockXField + 1) + x].vtx.y = SURACE_HIGH + (-sinf(wave)*WAVE_HIGH);
			}
		}
	// 頂点データをアンロック
	g_pD3DVtxBuffField->Unlock();

	// グローバルの位置処理
	if (WaveUp == true)
	{	// 上げる
		if (WaveBase >= 1000.0f)
			WaveUp = false;
		else
			WaveBase += WAVE_RATO;
	}
	else
	{	// 下げる
		if (WaveBase <= 0.0f)
			WaveUp = true;
		else
			WaveBase -= WAVE_RATO;
	}

}

//=============================================================================
// 描画処理
void DrawMeshWater(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTranslate;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldField);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotField.y, g_rotField.x, g_rotField.z);
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, g_posField.x, g_posField.y, g_posField.z);
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldField);

	// 頂点バッファをレンダリングパイプラインに設定
	pDevice->SetStreamSource(0, g_pD3DVtxBuffField, 0, sizeof(VERTEX_3D));

	// インデックスバッファをレンダリングパイプラインに設定
	pDevice->SetIndices(g_pD3DIdxBuffField);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureField);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_nNumVertexField, 0, g_nNumPolygonField);
}

