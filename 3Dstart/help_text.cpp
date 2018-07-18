//=============================================================================
//
// UIのテキストプロンプト処理[help_text.cpp]
// Author : 川井一生
//
/*
ヘルプテキストは操作方法や目的を表す
テキストプロんぷを表示する

help_text shown the controles and what to do
acts as a help prompt
*/
//=============================================================================
#include "help_text.h"
#include "input.h"
#include "fade.h"
#include "player.h"
#include "gem.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEX_PG000		"data/TEXTURE/controls000.png"			// 読み込むテクスチャファイル名
#define	PG000_WIDTH		(600)									// 操作の幅
#define	PG000_HEIGHT	(400)									// 操作の高さ
#define	PG000_POS_X		(SCREEN_WIDTH / 2)						// 操作の位置(X座標)
#define	PG000_POS_Y		(SCREEN_HEIGHT * 0.5f)					// 操作の位置(Y座標)

#define	TEX_PG001		"data/TEXTURE/find_the_coin.png"		// 読み込むテクスチャファイル名
#define	PG001_WIDTH		(512)									// 操作の幅
#define	PG001_HEIGHT	(256)									// 操作の高さ
#define	PG001_POS_X		(SCREEN_WIDTH / 2)						// 操作の位置(X座標)
#define	PG001_POS_Y		(SCREEN_HEIGHT * 0.25f)					// 操作の位置(Y座標)

#define	TEX_PG002		"data/TEXTURE/door_is_open.png"			// 読み込むテクスチャファイル名
#define	PG002_WIDTH		(640)									// 操作の幅
#define	PG002_HEIGHT	(120)									// 操作の高さ
#define	PG002_POS_X		(SCREEN_WIDTH / 2)						// 操作の位置(X座標)
#define	PG002_POS_Y		(SCREEN_HEIGHT * 0.25f)					// 操作の位置(Y座標)

// not used 
#define	TEX_PG003		"data/TEXTURE/.png"						// 読み込むテクスチャファイル名
#define	PG003_WIDTH		(640)									// 操作の幅
#define	PG003_HEIGHT	(120)									// 操作の高さ
#define	PG003_POS_X		(SCREEN_WIDTH / 2)						// 操作の位置(X座標)
#define	PG003_POS_Y		(SCREEN_HEIGHT * 0.75f)					// 操作の位置(Y座標)

#define	TEX_PG004		"data/TEXTURE/.png"						// 読み込むテクスチャファイル名
#define	PG004_WIDTH		(640)									// 操作の幅
#define	PG004_HEIGHT	(120)									// 操作の高さ
#define	PG004_POS_X		(SCREEN_WIDTH / 2)						// 操作の位置(X座標)
#define	PG004_POS_Y		(SCREEN_HEIGHT * 0.75f)					// 操作の位置(Y座標)

#define	TEX_PG005		"data/TEXTURE/.png"						// 読み込むテクスチャファイル名
#define	PG005_WIDTH		(640)									// 操作の幅
#define	PG005_HEIGHT	(120)									// 操作の高さ
#define	PG005_POS_X		(SCREEN_WIDTH / 2)						// 操作の位置(X座標)
#define	PG005_POS_Y		(SCREEN_HEIGHT * 0.75f)					// 操作の位置(Y座標)




#define TIME_TILL_HOW_TO	(1 * 60)	// ゲーム開始から操作説明が始まるまでの時間
#define START_LEASH			(300)		// 300 sq スタート地から操作方法を表示する距離

#define	APPEAR_TIMER		(60)		// 出現までの時間
#define DISPLAY_TIMER		(1.5 * 60)		// 表示されてる時間

#define ALPHA_START			(255.0f)
#define ALPHA_END			(0.0f)

#define	ALPHA_RATE			(0.05f)		// アルファ値の変更料

enum
{
	IdxPG000 = 0,
	IdxPG001,
	IdxPG002,
	IdxPG003,
	IdxPG004,
	IdxPG005,

	TEX_MAX,
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexHelpText(LPDIRECT3DDEVICE9 pDevice, int no);

void SetHelpTextPos(int no);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTexHelp[TEX_MAX] = { NULL,NULL };// テクスチャへのポインタ

HELP_TEXT				helptextWk[HELP_TEXT_MAX];			// UIのテキストのワーク番号

int						game_timer = TIME_TILL_HOW_TO;

bool					allCollected = false;
//=============================================================================
// 初期化処理
HRESULT InitHelpText(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HELP_TEXT *heltex = &helptextWk[0];

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEX_PG000, &D3DTexHelp[IdxPG000]);
	D3DXCreateTextureFromFile(pDevice, TEX_PG001, &D3DTexHelp[IdxPG001]);
	D3DXCreateTextureFromFile(pDevice, TEX_PG002, &D3DTexHelp[IdxPG002]);
	D3DXCreateTextureFromFile(pDevice, TEX_PG003, &D3DTexHelp[IdxPG003]);
	D3DXCreateTextureFromFile(pDevice, TEX_PG004, &D3DTexHelp[IdxPG004]);
	D3DXCreateTextureFromFile(pDevice, TEX_PG005, &D3DTexHelp[IdxPG005]);

	game_timer = TIME_TILL_HOW_TO;

	for (int i = 0; i < HELP_TEXT_MAX; i++, heltex++)
	{
		// 表示しないをセット
		heltex->use = false;

		// 初期化
		heltex->D3DVtxBuff	= NULL;
		heltex->AppearCnt	= 0;
		heltex->DisplayCnt	= 0;
		heltex->AlphaCnt	= 255.0f;
		heltex->TextId		= i;
		// 大きさと位置をセット
		SetHelpTextPos(i);

		// 頂点情報の作成
		MakeVertexHelpText(pDevice, i);


	}

	return S_OK;
}

//=============================================================================
// UIのテキストのセット処理
void SetHelpText(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HELP_TEXT *heltex = &helptextWk[no];

	// 表示するに変更
	heltex->use = true;

	heltex->D3DVtxBuff = NULL;
	heltex->AppearCnt = APPEAR_TIMER;
	heltex->DisplayCnt = DISPLAY_TIMER;
	heltex->AlphaCnt = 0.0f;

	// 頂点情報の作成
	MakeVertexHelpText(pDevice, no);

}

//=============================================================================
// 終了処理
void UninitHelpText(void)
{
	for (int i = 0; i < HELP_TEXT_MAX; i++)
	{
		ReleaseHelpText(i);
	}
	for (int i = 0; i < TEX_MAX; i++)
		SAFE_RELEASE(D3DTexHelp[i]);
}

//=============================================================================
// 一つずつの終了処理
void ReleaseHelpText(int no)
{
	HELP_TEXT *heltex = &helptextWk[no];

	if (heltex->D3DVtxBuff != NULL)
	{// 頂点バッファの開放
		heltex->D3DVtxBuff->Release();
		heltex->D3DVtxBuff = NULL;
	}

}

//=============================================================================
// 更新処理
void UpdateHelpText(void)
{
	HELP_TEXT *heltex = &helptextWk[0];
	PLAYER *player = GetPlayer(0);
	GEM *gem = GetGem(0);

	// スタートから(TIME_TILL_HOW_TO)秒後に操作方法を表示
	if (game_timer > 0)
		game_timer--;
	else if (game_timer == 0)
	{
		SetHelpText(0);
		game_timer -= 10;
	}

	// マップ上のジェムがまだあるかチェック
	int tCnt = 0;
	for (int i = 0; i < GEM_MAX; i++, gem++)
		if (gem->mesh.use)
			tCnt++;
	if (tCnt == 0)
		if (!allCollected)
		{
			SetHelpText(IdxPG002);
			allCollected = true;
		}

	for (int i = 0; i < HELP_TEXT_MAX; i++,heltex++)
		if (heltex->use == true)
		{
			if (heltex->AppearCnt > 0)
			{ // アルファ度を上げる
				heltex->AlphaCnt += ALPHA_RATE;
				heltex->AppearCnt--;
				SetAlphaHelpText(i);
			}
			else if (heltex->DisplayCnt > 0)
			{ // (DisplayCnt)秒表示
				if (heltex->TextId != 0) // 操作方法はボタンを押すまで表示
					heltex->DisplayCnt--;
				else
				{
					D3DXVECTOR3 temp = player->Pos - (D3DXVECTOR3(POS_X_PLAYER, POS_Y_PLAYER, POS_Z_PLAYER));
					float tLen = D3DXVec3Length(&temp);
					if (tLen > START_LEASH)
						heltex->DisplayCnt = 0;

				}


				if (GetKeyboardPress(DIK_RETURN)
					|| IsButtonPressed(0, BUTTON_B))
				{
					heltex->DisplayCnt = 0;
				}
			}
			else if (heltex->AppearCnt > -APPEAR_TIMER)
			{ // アルファ度を下げる
				heltex->AlphaCnt -= ALPHA_RATE;
				heltex->AppearCnt--;
				SetAlphaHelpText(i);
			}
			else
			{ // 完全に消えたらリリース
				heltex->use = false;
				ReleaseHelpText(i);
				// 操作表示の後に説明表示
				if (i == 0
					&& (i + 1) < HELP_TEXT_MAX)
				{
					SetHelpText(i + 1);
				}
			}
		}
	if (GetKeyboardTrigger(DIK_P))
	{
		SetHelpText(0);
	}
}

//=============================================================================
// 描画処理
void DrawHelpText(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HELP_TEXT *heltex = &helptextWk[0];

	for (int i = 0; i < HELP_TEXT_MAX; i++, heltex++)
		if (heltex->use == true)
		{
			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, heltex->D3DVtxBuff, 0, sizeof(VERTEX_2D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			// テクスチャの設定
			pDevice->SetTexture(0, D3DTexHelp[heltex->TextId]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
		}
}

//=============================================================================
// 頂点の作成
HRESULT MakeVertexHelpText(LPDIRECT3DDEVICE9 pDevice, int no)
{
	HELP_TEXT *heltex = &helptextWk[no];
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_2D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&heltex->D3DVtxBuff,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		heltex->D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(heltex->Pos.x - (heltex->Size.x / 2), heltex->Pos.y - (heltex->Size.y / 2), 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(heltex->Pos.x + (heltex->Size.x / 2), heltex->Pos.y - (heltex->Size.y / 2), 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(heltex->Pos.x - (heltex->Size.x / 2), heltex->Pos.y + (heltex->Size.y / 2), 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(heltex->Pos.x + (heltex->Size.x / 2), heltex->Pos.y + (heltex->Size.y / 2), 0.0f);

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
		heltex->D3DVtxBuff->Unlock();
	}

	return S_OK;
}

//=============================================================================
// アルファ値変更の処理
void SetAlphaHelpText(int no)
{
	HELP_TEXT *heltex = &helptextWk[no];
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		heltex->D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, heltex->AlphaCnt);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, heltex->AlphaCnt);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, heltex->AlphaCnt);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, heltex->AlphaCnt);

		// 頂点データをアンロックする
		heltex->D3DVtxBuff->Unlock();
	}

}


void SetHelpTextPos(int no)
{
	HELP_TEXT *heltex = &helptextWk[no];

	switch (no)
	{
	case IdxPG000:
		heltex->Pos = { PG000_POS_X,PG000_POS_Y };
		heltex->Size = { PG000_WIDTH,PG000_HEIGHT };
		break;
	case IdxPG001:
		heltex->Pos = { PG001_POS_X,PG001_POS_Y };
		heltex->Size = { PG001_WIDTH,PG001_HEIGHT };
		break;
	case IdxPG002:
		heltex->Pos = { PG002_POS_X,PG002_POS_Y };
		heltex->Size = { PG002_WIDTH,PG002_HEIGHT };
		break;
	}
}

//=============================================================================
// UIのテキストのゲット関数
HELP_TEXT GetHelpText(int no)
{
	return(helptextWk[no]);
}