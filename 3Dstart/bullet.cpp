//=============================================================================
//
// バレット処理 [bullboard.cpp]
// Author : 川井一生
//
//=============================================================================
#include "bullet.h"
#include "input.h"
#include "camera.h"
#include "player.h"
#include "shadow.h"
#include "field.h"
#include "mathWk.h"

//=============================================================================
// マクロ定義
#define	TEXTURE_BULLET000	"data/TEXTURE/bullet000.png"						// 読み込むテクスチャファイル名	name of the teture file

// 分かりやすいようにIDに名前
enum TEXTURE_NAME
{
	BULLET000,
	TEXTURE_MAX,
};

#define	MOVE_VALUE			(5.0f)											// 移動量						moving distance
#define	ROT_VALUE			(D3DX_PI * 0.02f)								// 回転量						rotation amount

#define	BULL_SIZE_X			(5.0f)											// バレットのサイズ(X方向)		size of the x axis
#define	BULL_SIZE_Z			(5.0f)											// バレットのサイズ(Z方向)		size of the y axis

#define	ALPHA				(170)

//=============================================================================
// プロトタイプ宣言
D3DXMATRIX GetInvRotateMatBull(void);
HRESULT MakeVertexBull(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// グローバル変数
LPDIRECT3DTEXTURE9		D3DTextureBull[TEXTURE_MAX] = { NULL};		// テクスチャへのポインタ		pointer for texture
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffBull = NULL;	// 頂点バッファへのポインタ		pointer for vertex buffer

BULL					bullWk[BULL_MAX];			// フィールドへのポインタ


//=============================================================================
// バレット初期化処理
HRESULT InitBull(void)
{
	BULL *bull = &bullWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int i;

	D3DXCreateTextureFromFile(pDevice, TEXTURE_BULLET000, &D3DTextureBull[BULLET000]);

	for (i = 0; i < BULL_MAX; i++, bull++)
	{

		bull->use = false;

		// 位置・回転・スケールの初期設定
		bull->Pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
		//bull->Rot = D3DXVECTOR3(-D3DX_PI * 0.5f, 0.0f, 0.0f);
		bull->Scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

		MakeVertexBull(pDevice);
		switch (i)
		{
		case 0:
			bull->TexID = BULLET000;
			break;
		}

		// 影の生成
		bull->nIdxShadow = CreateShadow(bull->Pos, 0.1f, 25.0f);

		bull->fSizeShadow = 10.0f;
		bull->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
	}
	return S_OK;
}

//=============================================================================
// バレット終了処理
void UninitBull(void)
{
	BULL *bull = &bullWk[0];
	for (int i = 0; i < TEXTURE_MAX; i++, bull++)
	{
		if (D3DTextureBull[i] != NULL)
		{// テクスチャの開放
			D3DTextureBull[i]->Release();
			D3DTextureBull[i] = NULL;
		}
	}
	if (g_pD3DVtxBuffBull != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffBull->Release();
		g_pD3DVtxBuffBull = NULL;
	}

}

//=============================================================================
// バレットセット処理
void SetBull(D3DXVECTOR3 Pos , D3DXVECTOR3 Rot)
{
	BULL *bull = &bullWk[0];
	CAMERA *camera = GetCamera(0);
	PLAYER *player = GetPlayer(0);
	int i;
	for (i = 0; i < BULL_MAX; i++, bull++)
	{
		if (bull->use == false)
		{
			bull->use = true;
			bull->Pos = Pos;
			bull->fCount = 50;
			bull->Vel = Ang2Vec(Rot, MOVE_VALUE);

			bull->nIdxShadow = CreateShadow(bull->Pos, 0.1f, 25.0f);
			return;
		}
	}


}

//=============================================================================
// バレット更新処理
void UpdateBull(void)
{
	BULL *bull = &bullWk[0];
	int i;
	for (i = 0; i < BULL_MAX; i++, bull++)
		if (bull->use == true)
		{
			bull->Pos += bull->Vel;



			// 影の位置設定
			SetPositionShadow(bull->nIdxShadow, D3DXVECTOR3(bull->Pos.x, 0.1f, bull->Pos.z));

			SetVertexShadow(bull->nIdxShadow, bull->fSizeShadow, bull->fSizeShadow);
			SetColorShadow(bull->nIdxShadow, bull->colShadow);

			bull->fCount--;
			if (bull->fCount <= 0)
			{
				bull->use = false;
				ReleaseShadow(bull->nIdxShadow);
			}
		}
}
//=============================================================================
// バレット描画処理
void DrawBull(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BULL *bull = &bullWk[0];
	CAMERA *camera = GetCamera(0);
	D3DXMATRIX mtxView, mtxScl, mtxRot, mtxTranslate;	// スケール、回転、平行移動
	int i;

	for (i = 0; i < BULL_MAX;i++, bull++)
		if (bull->use == true)
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
			D3DXMatrixIdentity(&bull->mtxWorld);


			//ポリゴンを正面に向ける(逆行列)

			// 直交行列の正方行列にとる逆行列
			bull->mtxWorld._11 = mtxView._11;
			bull->mtxWorld._12 = mtxView._21;
			bull->mtxWorld._13 = mtxView._31;

			bull->mtxWorld._21 = mtxView._12;
			bull->mtxWorld._22 = mtxView._22;
			bull->mtxWorld._23 = mtxView._32;

			bull->mtxWorld._31 = mtxView._13;
			bull->mtxWorld._32 = mtxView._23;
			bull->mtxWorld._33 = mtxView._33;


			//D3DXMatrixInverse(&bull->mtxWorld, NULL, &mtxView);
			//bull->mtxWorld._41 = 0.0f;
			//bull->mtxWorld._42 = 0.0f;
			//bull->mtxWorld._43 = 0.0f;






			// ワールドマトリックス設定
			pDevice->SetTransform(D3DTS_WORLD, &bull->mtxWorld);

			// スケールを反映	(S)
			D3DXMatrixScaling(&mtxScl, bull->Scl.x, bull->Scl.y, bull->Scl.z);
			D3DXMatrixMultiply(&bull->mtxWorld, &bull->mtxWorld, &mtxScl); 	// &bull->mtxWorld = &bull->mtxWorld * &mtxScl
			//// 回転を反映		(R)
			//D3DXMatrixRotationYawPitchRoll(&mtxRot, bull->Rot.y, bull->Rot.x, bull->Rot.z);
			//D3DXMatrixMultiply(&bull->mtxWorld, &bull->mtxWorld, &mtxRot);	// &bull->mtxWorld = &bull->mtxWorld * &mtxRot
			// 平行移動を反映	(T)
			D3DXMatrixTranslation(&mtxTranslate, bull->Pos.x, bull->Pos.y, bull->Pos.z);
			D3DXMatrixMultiply(&bull->mtxWorld, &bull->mtxWorld, &mtxTranslate);

			//D3DXMatrixMultiply(&bull->mtxWorld, &bull->mtxWorld, &GetInvRotateMat());






			// ワールドマトリックス設定
			pDevice->SetTransform(D3DTS_WORLD, &bull->mtxWorld);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pD3DVtxBuffBull, 0, sizeof(VERTEX_3D));

			// 頂点フォーマット設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// クスチャの設定
			pDevice->SetTexture(0, D3DTextureBull[bull->TexID]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

			// ラインティングを有効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			// αテストを無効に
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		}
}





//=============================================================================
// 頂点の作成
HRESULT MakeVertexBull(LPDIRECT3DDEVICE9 pDevice)
{
	BULL *bull = &bullWk[0];
	int i;
	for (i = 0; i < BULL_MAX; i++)
	{
		// オブジェクトの頂点バッファを生成
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
			FVF_VERTEX_3D,				// 使用する頂点フォーマット
			D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
			&g_pD3DVtxBuffBull,			// 頂点バッファインターフェースへのポインタ
			NULL)))						// NULLに設定
		{
			return E_FAIL;
		}

		{//頂点バッファの中身を埋める
			VERTEX_3D *pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			g_pD3DVtxBuffBull->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-BULL_SIZE_X, BULL_SIZE_Z, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(BULL_SIZE_X, BULL_SIZE_Z, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-BULL_SIZE_X, -BULL_SIZE_Z, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(BULL_SIZE_X, -BULL_SIZE_Z, 0.0f);

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
			g_pD3DVtxBuffBull->Unlock();
		}
	}
	return S_OK;
}

//=============================================================================
// ビルボードにする処理
D3DXMATRIX GetInvRotateMatBull(void)
{
	CAMERA *camera = GetCamera(0);
	D3DXMATRIX Inv;
	D3DXVECTOR3 LookAt;
	LookAt = -camera->Eye + camera->At;
	D3DXMatrixIdentity(&Inv);
	D3DXMatrixLookAtLH(&Inv, &D3DXVECTOR3(0, 0, 0), &LookAt, &camera->Up);
	D3DXMatrixInverse(&Inv, NULL, &Inv);
	return Inv;
}

//=============================================================================
// バレットのゲット関数
BULL *GetBull(int no)
{
	return (&bullWk[no]);
}