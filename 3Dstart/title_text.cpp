//=============================================================================
//
// タイトル画面のテキストの処理 [title_text.cpp]
// Author : 川井一生
//
/*
タイトルでロゴの処理
スタートでゲーム内で使わないものをリリース

title _text.cpp is for logo
releace thing not used in game when start is selected
*/
//=============================================================================
#include "title_text.h"
#include "input.h"
#include "fade.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define	TEXT_TITLE				"data/TEXTURE/title_logo.png"		// 読み込むテクスチャファイル名
#define	TEXT_EXIT				"data/TEXTURE/exit.png"				// 読み込むテクスチャファイル名
#define	TEXT_START				"data/TEXTURE/start.png"			// 読み込むテクスチャファイル名
#define TEXT_POINTER			"data/TEXTURE/pointer.png"			// 読み込むテクスチャファイル名

#define	TITLE_WIDTH				(960)		// タイトルロゴの幅
#define	TITLE_HEIGHT			(270)		// タイトルロゴの高さ
#define	TITLE_POS_X				(SCREEN_WIDTH / 2 - TITLE_WIDTH / 2)		// タイトルロゴの位置(X座標)
#define	TITLE_POS_Y				(0.0f)		// タイトルロゴの位置(Y座標)

#define	EXIT_WIDTH				(640)		// 終了ボタンの幅
#define	EXIT_HEIGHT				(120)		// 終了ボタンの高さ
#define	EXIT_POS_X				(SCREEN_WIDTH / 2 - EXIT_WIDTH / 2)		// 終了ボタンの位置(X座標)
#define	EXIT_POS_Y				(SCREEN_HEIGHT * 0.75f)		// 終了ボタンの位置(Y座標)

#define	START_WIDTH				(480)		// スタートボタンの幅
#define	START_HEIGHT			(120)		// スタートボタンの高さ
#define	START_POS_X				(SCREEN_WIDTH / 2 - START_WIDTH /2)		// スタートボタンの位置(X座標)
#define	START_POS_Y				(SCREEN_HEIGHT * 0.5f)		// スタートボタンの位置(Y座標)

#define POINTER_WIDTH			(120)		// カーソルの幅
#define POINTER_HEIGHT			(120)		// カーソルの高さ

#define	COUNT_APPERA_START		(60)		// スタートボタン出現までの時間
#define ALPHA_RATE				(0.05)		// 透明度の変換速度



enum
{
	TITLE_SELECT_START = 0,
	TITLE_SELECT_EXIT,
	TITLE_SELECT_MAX,
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexTitle(LPDIRECT3DDEVICE9 pDevice);
void DrawTitlePoly(LPDIRECT3DVERTEXBUFFER9 vtxbuff, LPDIRECT3DTEXTURE9 texture);
void SetColorTitleLogo(LPDIRECT3DVERTEXBUFFER9 vtxbuff, float Alpha);
void SetTitleVertexPos(void);


//*****************************************************************************
// グローバル宣言
//*****************************************************************************

LPDIRECT3DTEXTURE9		g_pD3DTextureTitle = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffTitle = NULL;		// 頂点バッファインターフェースへのポインタ

LPDIRECT3DTEXTURE9		g_pD3DTextureExit = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffExit = NULL;		// 頂点バッファインターフェースへのポインタ

LPDIRECT3DTEXTURE9		g_pD3DTextureStart = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffStart = NULL;		// 頂点バッファインターフェースへのポインタ

LPDIRECT3DTEXTURE9		g_pD3DTexturePointer = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffPointer = NULL;	// 頂点バッファインターフェースへのポインタ

int						g_Selection = 0;
int						g_TimeTillDisplay = 0;			// 表示までの時間
float					g_Alpha = 0.0f;					// テクスチャーのアルファ度
bool					g_isFadeIn = true;				// 透明度を上げるか下げるか判断するフラグ

float					g_PointerPosX = 0.0f;			// カーソルのX座標の位置
float					g_PointerPosY = 0.0f;			// カーソルのY座標の位置

//=============================================================================
// タイトル画面のテキストの初期化処理
HRESULT InitTitleText(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_TimeTillDisplay = COUNT_APPERA_START;
	g_Alpha = 0.0f;
	g_isFadeIn = true;


	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXT_TITLE, &g_pD3DTextureTitle);

	D3DXCreateTextureFromFile(pDevice, TEXT_EXIT, &g_pD3DTextureExit);

	D3DXCreateTextureFromFile(pDevice, TEXT_START, &g_pD3DTextureStart);

	D3DXCreateTextureFromFile(pDevice, TEXT_POINTER, &g_pD3DTexturePointer);

	// 頂点情報の作成
	MakeVertexTitle(pDevice);

	return S_OK;

}

//=============================================================================
// タイトル画面のテキストの終了処理
void UninitTitleText(void)
{
	if (g_pD3DTextureTitle != NULL)
	{// テクスチャの開放
		g_pD3DTextureTitle->Release();
		g_pD3DTextureTitle = NULL;
	}

	if (g_pD3DVtxBuffTitle != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffTitle->Release();
		g_pD3DVtxBuffTitle = NULL;
	}

	if (g_pD3DTextureExit != NULL)
	{// テクスチャの開放
		g_pD3DTextureExit->Release();
		g_pD3DTextureExit = NULL;
	}

	if (g_pD3DVtxBuffExit != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffExit->Release();
		g_pD3DVtxBuffExit = NULL;
	}

	if (g_pD3DTextureStart != NULL)
	{// テクスチャの開放
		g_pD3DTextureStart->Release();
		g_pD3DTextureStart = NULL;
	}

	if (g_pD3DVtxBuffStart != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffStart->Release();
		g_pD3DVtxBuffStart = NULL;
	}

}

//=============================================================================
// タイトル画面のテキストの最新処理
void UpdateTitleText(void)
{
	// 透明度の変換処理
	if (g_isFadeIn)
	{
		g_Alpha += ALPHA_RATE;
		if (g_Alpha >= 1.0f)
			g_isFadeIn = false;
	}
	else if (!g_isFadeIn)
	{
		g_Alpha -= ALPHA_RATE;
		if (g_Alpha <= 0.0f)
			g_isFadeIn = true;
	}

	switch (g_Selection)
	{
	case TITLE_SELECT_START:
		SetColorTitleLogo(g_pD3DVtxBuffStart, g_Alpha);
		SetColorTitleLogo(g_pD3DVtxBuffExit, 1.0f);
		g_PointerPosX = START_POS_X - (START_WIDTH / 2) - POINTER_WIDTH;
		g_PointerPosY = START_POS_Y - (START_HEIGHT / 2) - POINTER_HEIGHT;
		break;

	case TITLE_SELECT_EXIT:
		SetColorTitleLogo(g_pD3DVtxBuffStart, 1.0f);
		SetColorTitleLogo(g_pD3DVtxBuffExit, g_Alpha);
		g_PointerPosX = EXIT_POS_X - (START_WIDTH / 2) - POINTER_WIDTH;
		g_PointerPosY = EXIT_POS_Y - (START_HEIGHT / 2) - POINTER_HEIGHT;
		break;
	}

	// ポインターの位置をセット
	void SetTitleVertexPos();


	// ポインターの処理
	if (GetKeyboardTrigger(DIK_W))
	{
		g_Selection = (g_Selection + 1) % TITLE_SELECT_MAX;
		g_Alpha = 1.0f;
		g_isFadeIn = false;
	}
	
	if (GetKeyboardTrigger(DIK_S))
	{
		g_Selection --;
		if (g_Selection < 0)
			g_Selection = TITLE_SELECT_MAX - 1;
		g_Alpha = 1.0f;
		g_isFadeIn = false;
	}


	if (GetKeyboardTrigger(DIK_RETURN)
		|| IsButtonTriggered(0, BUTTON_A)
		|| IsButtonTriggered(0, BUTTON_START))
	{
		// ゲームへ
		if (g_Selection == TITLE_SELECT_START)
			SetFade(FADE_OUT);
		else if (g_Selection == TITLE_SELECT_EXIT)
			SetMode(MODE_UNINIT,0);


	}
}

//=============================================================================
// タイトル画面のテキストの描画処理
void DrawTitleText(void)
{
	// タイトルの描画
	DrawTitlePoly(g_pD3DVtxBuffTitle, g_pD3DTextureTitle);
	// エグジットボタンの描画
	DrawTitlePoly(g_pD3DVtxBuffExit, g_pD3DTextureExit);
	// スタートボタンの描画
	DrawTitlePoly(g_pD3DVtxBuffStart, g_pD3DTextureStart);
	// カーソルの描画
	//DrawTitlePoly(g_pD3DVtxBuffPointer, g_pD3DTexturePointer);

}

void DrawTitlePoly(LPDIRECT3DVERTEXBUFFER9 vtxbuff, LPDIRECT3DTEXTURE9 texture)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, vtxbuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, texture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

}


//=============================================================================
// 頂点の作成
HRESULT MakeVertexTitle(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_2D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffTitle,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定  TITLE_POS_Y
		pVtx[0].vtx = D3DXVECTOR3(TITLE_POS_X, TITLE_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(TITLE_POS_X + TITLE_WIDTH, TITLE_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(TITLE_POS_X, TITLE_POS_Y + TITLE_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(TITLE_POS_X + TITLE_WIDTH, TITLE_POS_Y + TITLE_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

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
		g_pD3DVtxBuffTitle->Unlock();
	}

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_2D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffExit,	// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffExit->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(EXIT_POS_X, EXIT_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(EXIT_POS_X + EXIT_WIDTH, EXIT_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(EXIT_POS_X, EXIT_POS_Y + EXIT_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(EXIT_POS_X + EXIT_WIDTH, EXIT_POS_Y + EXIT_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

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
		g_pD3DVtxBuffExit->Unlock();
	}


	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_2D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffStart,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffStart->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(START_POS_X, START_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(START_POS_X + START_WIDTH, START_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(START_POS_X, START_POS_Y + START_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(START_POS_X + START_WIDTH, START_POS_Y + START_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

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
		g_pD3DVtxBuffStart->Unlock();
	}


	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_2D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffPointer,	// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffPointer->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(g_PointerPosX, g_PointerPosY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(g_PointerPosX + POINTER_WIDTH, g_PointerPosY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(g_PointerPosX, g_PointerPosY + POINTER_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(g_PointerPosX + POINTER_WIDTH, g_PointerPosY + POINTER_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

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
		g_pD3DVtxBuffPointer->Unlock();
	}

	return S_OK;

}

//=============================================================================
// 頂点の作成
void SetColorTitleLogo(LPDIRECT3DVERTEXBUFFER9 vtxbuff, float Alpha)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		vtxbuff->Lock(0, 0, (void**)&pVtx, 0);

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, Alpha);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, Alpha);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, Alpha);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, Alpha);

		// 頂点データをアンロックする
		vtxbuff->Unlock();
	}

}

void SetTitleVertexPos(void)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffPointer->Lock(0, 0, (void**)&pVtx, 0);

		// 反射光の設定
		pVtx[0].vtx = D3DXVECTOR3(g_PointerPosX, g_PointerPosY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(g_PointerPosX + POINTER_WIDTH, g_PointerPosY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(g_PointerPosX, g_PointerPosY + POINTER_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(g_PointerPosX + POINTER_WIDTH, g_PointerPosY + POINTER_HEIGHT, 0.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffPointer->Unlock();
	}

}