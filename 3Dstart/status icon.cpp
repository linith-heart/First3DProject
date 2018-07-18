//=============================================================================
//
// バレット処理 [staticoboard.cpp]
// Author : 川井一生
//
//=============================================================================
#include "status icon.h"
#include "input.h"
#include "camera.h"
#include "player.h"
#include "shadow.h"
#include "field.h"
#include "mathWk.h"

//=============================================================================
// マクロ定義
#define	TEX_FOUND		"data/TEXTURE/exclamation_mark.png"
#define	TEX_CHECK		"data/TEXTURE/question_mark.png"



#define	ICO_SIZE_X			(50.0f)											// バレットのサイズ(X方向)		size of the x axis
#define	ICO_SIZE_Y			(50.0f)											// バレットのサイズ(Z方向)		size of the y axis

#define	ALPHA				(170)

//=============================================================================
// プロトタイプ宣言
D3DXMATRIX GetInvRotateMatStatIco(void);
HRESULT MakeVertexStatIco(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// グローバル変数
LPDIRECT3DTEXTURE9		D3DTexStatIco[STAT_ICO_MAX] = { NULL ,NULL };		// テクスチャへのポインタ		pointer for texture
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffStatIco = NULL;	// 頂点バッファへのポインタ		pointer for vertex buffer

STAT_ICO					staticoWk[STAT_ICO_MAX];			// フィールドへのポインタ


//=============================================================================
// バレット初期化処理
HRESULT InitStatIco(void)
{
	STAT_ICO *statico = &staticoWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int i;

	D3DXCreateTextureFromFile(pDevice, TEX_FOUND, &D3DTexStatIco[Idx_FOUND]);
	D3DXCreateTextureFromFile(pDevice, TEX_CHECK, &D3DTexStatIco[Idx_CHECK]);

	for (i = 0; i < STAT_ICO_MAX; i++, statico++)
	{
		statico->use = false;

		// 位置・回転・スケールの初期設定
		statico->Pos = VEC3CLEAR;
		statico->Scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

		statico->TexID = 0;

		MakeVertexStatIco(pDevice);

	}
	return S_OK;
}

//=============================================================================
// バレット終了処理
void UninitStatIco(void)
{
	STAT_ICO *statico = &staticoWk[0];
	for (int i = 0; i < STAT_ICO_MAX; i++)
		SAFE_RELEASE(D3DTexStatIco[i]);

	SAFE_RELEASE(D3DVtxBuffStatIco);
}

//=============================================================================
// バレットセット処理
void SetStatIco(D3DXVECTOR3 Pos , int Stat)
{
	STAT_ICO *statico = &staticoWk[0];

	int i;
	for (i = 0; i < STAT_ICO_MAX; i++, statico++)
	{
		if (statico->use == false)
		{
			statico->use = true;
			statico->Pos = Pos;
			statico->fCount = 20;
			statico->TexID = Stat;
			return;
		}
	}


}

//=============================================================================
// バレット更新処理
void UpdateStatIco(void)
{
	STAT_ICO *statico = &staticoWk[0];
	int i;
	for (i = 0; i < STAT_ICO_MAX; i++, statico++)
		if (statico->use == true)
		{

			statico->fCount--;
			if (statico->fCount <= 0)
			{
				statico->use = false;
			}
		}
}
//=============================================================================
// バレット描画処理
void DrawStatIco(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	STAT_ICO *statico = &staticoWk[0];
	CAMERA *camera = GetCamera(0);
	D3DXMATRIX mtxView, mtxScl, mtxRot, mtxTranslate;	// スケール、回転、平行移動
	int i;

	for (i = 0; i < STAT_ICO_MAX;i++, statico++)
		if (statico->use == true)
		{

			// αテスト設定

			// αテストを有効に
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, ALPHA);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			// ラインティングを無効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			// ビューマトリックスを取得
			mtxView = camera->mtxView;

			// ワールドマトリクス初期化
			D3DXMatrixIdentity(&statico->mtxWorld);

			//ポリゴンを正面に向ける(逆行列)

			// 直交行列の正方行列にとる逆行列
			statico->mtxWorld._11 = mtxView._11;
			statico->mtxWorld._12 = mtxView._21;
			statico->mtxWorld._13 = mtxView._31;

			statico->mtxWorld._21 = mtxView._12;
			statico->mtxWorld._22 = mtxView._22;
			statico->mtxWorld._23 = mtxView._32;

			statico->mtxWorld._31 = mtxView._13;
			statico->mtxWorld._32 = mtxView._23;
			statico->mtxWorld._33 = mtxView._33;


			//D3DXMatrixInverse(&statico->mtxWorld, NULL, &mtxView);
			//statico->mtxWorld._41 = 0.0f;
			//statico->mtxWorld._42 = 0.0f;
			//statico->mtxWorld._43 = 0.0f;

			// ワールドマトリックス設定
			pDevice->SetTransform(D3DTS_WORLD, &statico->mtxWorld);

			// スケールを反映	(S)
			D3DXMatrixScaling(&mtxScl, statico->Scl.x, statico->Scl.y, statico->Scl.z);
			D3DXMatrixMultiply(&statico->mtxWorld, &statico->mtxWorld, &mtxScl); 	// &statico->mtxWorld = &statico->mtxWorld * &mtxScl
			//// 回転を反映		(R)
			//D3DXMatrixRotationYawPitchRoll(&mtxRot, statico->Rot.y, statico->Rot.x, statico->Rot.z);
			//D3DXMatrixMultiply(&statico->mtxWorld, &statico->mtxWorld, &mtxRot);	// &statico->mtxWorld = &statico->mtxWorld * &mtxRot
			// 平行移動を反映	(T)
			D3DXMatrixTranslation(&mtxTranslate, statico->Pos.x, statico->Pos.y, statico->Pos.z);
			D3DXMatrixMultiply(&statico->mtxWorld, &statico->mtxWorld, &mtxTranslate);

			//D3DXMatrixMultiply(&statico->mtxWorld, &statico->mtxWorld, &GetInvRotateMat());

			// ワールドマトリックス設定
			pDevice->SetTransform(D3DTS_WORLD, &statico->mtxWorld);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, D3DVtxBuffStatIco, 0, sizeof(VERTEX_3D));

			// 頂点フォーマット設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// クスチャの設定
			pDevice->SetTexture(0, D3DTexStatIco[statico->TexID]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

			// ラインティングを有効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			// αテストを無効に
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			//// 描画したら消す
			//statico->use = false;
		}
}

//=============================================================================
// 頂点の作成
HRESULT MakeVertexStatIco(LPDIRECT3DDEVICE9 pDevice)
{
	STAT_ICO *statico = &staticoWk[0];
	int i;
	for (i = 0; i < STAT_ICO_MAX; i++)
	{
		// オブジェクトの頂点バッファを生成
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
			FVF_VERTEX_3D,				// 使用する頂点フォーマット
			D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
			&D3DVtxBuffStatIco,			// 頂点バッファインターフェースへのポインタ
			NULL)))						// NULLに設定
		{
			return E_FAIL;
		}

		{//頂点バッファの中身を埋める
			VERTEX_3D *pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			D3DVtxBuffStatIco->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-ICO_SIZE_X, ICO_SIZE_Y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(ICO_SIZE_X, ICO_SIZE_Y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-ICO_SIZE_X, -ICO_SIZE_Y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(ICO_SIZE_X, -ICO_SIZE_Y, 0.0f);

			// 法線ベクトルの設定
			pVtx[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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

			// 頂点データをアンロックする
			D3DVtxBuffStatIco->Unlock();
		}
	}
	return S_OK;
}


//=============================================================================
// バレットのゲット関数
STAT_ICO *GetStatIco(int no)
{
	return (&staticoWk[no]);
}