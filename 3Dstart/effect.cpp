//=============================================================================
//
// エフェクト処理 [effect.cpp]
// Author : 
//
//=============================================================================
#include "effect.h"
#include "input.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEX_SPARK		"data/TEXTURE/sparkle000.png"	// 読み込むテクスチャーファイル名
#define	TEX_DUST00		"data/TEXTURE/dust000.png"
#define	TEX_DUST01		"data/TEXTURE/dust001.png"
#define	TEX_DUST02		"data/TEXTURE/dust002.png"
//#define	TEX_FOUND		"data/TEXTURE/exclamation_mark.png"
//#define	TEX_CHECK		"data/TEXTURE/question_mark.png"
#define	EFFECT_SIZE_X			(50.0f)							// ビルボードの幅
#define	EFFECT_SIZE_Y			(50.0f)							// ビルボードの高さ
#define	VALUE_MOVE_BULLET		(2.0f)							// 移動速度

#define	MAX_EFFECT				(4096)							// エフェクト最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 回転
	D3DXVECTOR3 scale;		// スケール
	D3DXVECTOR3 move;		// 移動量
	D3DXCOLOR col;			// 色
	float fSizeX;			// 幅
	float fSizeY;			// 高さ
	int nTimer;				// タイマー
	int TexId;				// テクスチャー番号
	float nDecAlpha;		// 減衰値
	bool bUse;				// 使用しているかどうか
} EFFECT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice);
void SetVertexEffect(int nIdxEffect, float fSizeX, float fSizeY);
void SetColorEffect(int nIdxEffect, D3DXCOLOR col);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTexEff[TEX_MAX] = { NULL,NULL,NULL,NULL };		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffEff = NULL;		// 頂点バッファインターフェースへのポインタ

D3DXMATRIX				g_mtxWorldEffect;				// ワールドマトリックス

EFFECT					Eff[MAX_EFFECT];				// イフェクトワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexEffect(pDevice);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEX_SPARK,  &D3DTexEff[Idx_SPARK]);
	D3DXCreateTextureFromFile(pDevice, TEX_DUST00, &D3DTexEff[Idx_DUST00]);
	D3DXCreateTextureFromFile(pDevice, TEX_DUST01, &D3DTexEff[Idx_DUST01]);
	D3DXCreateTextureFromFile(pDevice, TEX_DUST02, &D3DTexEff[Idx_DUST02]);
	//D3DXCreateTextureFromFile(pDevice, TEX_FOUND, &D3DTexEff[Idx_FOUND]);
	//D3DXCreateTextureFromFile(pDevice, TEX_CHECK, &D3DTexEff[Idx_CHECK]);

	for (int i = 0; i < MAX_EFFECT; i++)
	{
		Eff[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Eff[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Eff[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		Eff[i].move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		Eff[i].fSizeX = EFFECT_SIZE_X;
		Eff[i].fSizeY = EFFECT_SIZE_Y;
		Eff[i].TexId = 0;
		Eff[i].nTimer = 0;
		Eff[i].bUse = false;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEffect(void)
{
	for (int i = 0; i < TEX_MAX; i++)
		SAFE_RELEASE(D3DTexEff[i]);

	SAFE_RELEASE(D3DVtxBuffEff);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEffect(void)
{
	for (int i = 0; i < MAX_EFFECT; i++)
	{
		if (Eff[i].bUse)
		{
			Eff[i].pos.x += Eff[i].move.x;
			Eff[i].pos.y += Eff[i].move.y;
			Eff[i].pos.z += Eff[i].move.z;

			Eff[i].col.a -= Eff[i].nDecAlpha;
			if (Eff[i].col.a <= 0.0f)
			{
				Eff[i].col.a = 0.0f;
			}
			SetColorEffect(i,
				D3DXCOLOR(Eff[i].col.r, Eff[i].col.b,
					Eff[i].col.b, Eff[i].col.a));

			Eff[i].nTimer--;
			if (Eff[i].nTimer <= 0)
			{
				Eff[i].bUse = false;
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;
	int cam_mode = GetCamMode();
	CAMERA *camera = GetCamera(cam_mode);

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);			// Z比較なし

	//	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
	//	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	//	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

	for (int i = 0; i < MAX_EFFECT; i++)
	{
		if (Eff[i].bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldEffect);

			// ビューマトリックスを取得
			mtxView = camera->mtxView;

			g_mtxWorldEffect._11 = mtxView._11;
			g_mtxWorldEffect._12 = mtxView._21;
			g_mtxWorldEffect._13 = mtxView._31;
			g_mtxWorldEffect._21 = mtxView._12;
			g_mtxWorldEffect._22 = mtxView._22;
			g_mtxWorldEffect._23 = mtxView._32;
			g_mtxWorldEffect._31 = mtxView._13;
			g_mtxWorldEffect._32 = mtxView._23;
			g_mtxWorldEffect._33 = mtxView._33;

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, Eff[i].scale.x, Eff[i].scale.y, Eff[i].scale.z);
			D3DXMatrixMultiply(&g_mtxWorldEffect, &g_mtxWorldEffect, &mtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, Eff[i].pos.x, Eff[i].pos.y, Eff[i].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldEffect, &g_mtxWorldEffect, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldEffect);

			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, D3DVtxBuffEff, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, D3DTexEff[Eff[i].TexId]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * 4), NUM_POLYGON);

			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		}
	}

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);				// Z比較あり

	//	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	//	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	//	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_EFFECT,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF_VERTEX_3D,								// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuffEff,						// 頂点バッファインターフェースへのポインタ
		NULL)))										// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffEff->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < MAX_EFFECT; i++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-EFFECT_SIZE_X / 2, -EFFECT_SIZE_Y / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(EFFECT_SIZE_X / 2, -EFFECT_SIZE_Y / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-EFFECT_SIZE_X / 2, EFFECT_SIZE_Y / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(EFFECT_SIZE_X / 2, EFFECT_SIZE_Y / 2, 0.0f);

			// 法線の設定
			pVtx[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// 頂点データをアンロックする
		D3DVtxBuffEff->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexEffect(int nIdxEffect, float fSizeX, float fSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffEff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxEffect * 4);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// 頂点データをアンロックする
		D3DVtxBuffEff->Unlock();
	}
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorEffect(int nIdxEffect, D3DXCOLOR col)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuffEff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxEffect * 4);

		// 頂点座標の設定
		pVtx[0].diffuse =
			pVtx[1].diffuse =
			pVtx[2].diffuse =
			pVtx[3].diffuse = col;

		// 頂点データをアンロックする
		D3DVtxBuffEff->Unlock();
	}
}
//=============================================================================
// エフェクトの設定
//=============================================================================
int SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nTimer, int TexId)
{
	int nIdxEffect = -1;

	for (int i = 0; i < MAX_EFFECT; i++)
	{
		if (!Eff[i].bUse)
		{
			Eff[i].pos = pos;
			Eff[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			Eff[i].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			Eff[i].move = move;
			Eff[i].col = col;
			Eff[i].fSizeX = fSizeX;
			Eff[i].fSizeY = fSizeY;
			Eff[i].TexId = TexId;
			Eff[i].nTimer = nTimer;
			Eff[i].nDecAlpha = col.a / nTimer;
			Eff[i].bUse = true;

			// 頂点座標の設定
			SetVertexEffect(i, fSizeX, fSizeY);

			// 頂点カラーの設定
			SetColorEffect(i,
				D3DXCOLOR(Eff[i].col.r, Eff[i].col.b,
					Eff[i].col.b, Eff[i].col.a));

			nIdxEffect = i;

			break;
		}
	}

	return nIdxEffect;
}
