//=============================================================================
//
// UIの処理 [UI.cpp]
// Author : 川井 一生
//
//=============================================================================
#include "main.h"
#include "UI.h"
#include "player.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************


enum
{
	PLAYER_BAR,		// プレイヤ－の体力と魔力を表示するプレ－ト
	PLAYER_HP_SHAD,	// プレイヤ－の体力のシャドウ
	PLAYER_HP,			// プレイヤ－の体力

	PLAYER_STAM,		// プレイヤ－のスタミナ

	UI_MAX,				// UIに使う量
};


// プレイヤ－の体力とスタミナ
#define TEXTURE_BAR	("data/TEXTURE/health_bar_bg00.png")
#define TEXTURE_BAR_SIZE_X	(610 /2)
#define TEXTURE_BAR_SIZE_Y	(65 /2)

// 体力
#define TEXTURE_HP	("data/TEXTURE/health_bar.png")
#define TEXTURE_HP_SIZE_X_MAX	(600 /2)
#define TEXTURE_HP_SIZE_Y		(35 /2)
// 体力のその時の量に応じて大きさを変える
#define TEXTURE_HP_SIZE_X ((TEXTURE_HP_SIZE_X_MAX/PLAYER_HP_MAX)*player->HP)
// 体力シャドウ
#define TEXTURE_HP_SHAD ("data/TEXTURE/health_bar_shadow.png")
// 大きさMAXは体力と一緒
#define TEXTURE_HP_SHAD_SIZE_X ((TEXTURE_HP_SIZE_X_MAX/PLAYER_HP_MAX)*player->HPShadow)


// スタミナ
#define TEXTURE_STAM	("data/TEXTURE/mana_bar.png")
#define TEXTURE_STAM_SIZE_X_MAX	(300)
#define TEXTURE_STAM_SIZE_Y		(18 /2)
// スタミナのその時の量に応じて大きさを変える
#define TEXTURE_STAM_SIZE_X ((TEXTURE_STAM_SIZE_X_MAX/PLAYER_STAM_MAX)*player->Stam)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexUi(int no);
void SetVertexUi(int no);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTexBar		= NULL;		// テクスチャへのポリゴン
LPDIRECT3DTEXTURE9		g_pD3DTexHp			= NULL;		// 同上
LPDIRECT3DTEXTURE9		g_pD3DTexHpShad		= NULL;		// 同上
LPDIRECT3DTEXTURE9		g_pD3DTexStam		= NULL;		// 同上


UI						uiWk[UI_MAX];					// UI構造体


//=============================================================================
// 初期化処理
HRESULT InitUi(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	UI *ui = &uiWk[0];
	PLAYER *player = GetPlayer(0);

	// テクスチャの読み込み
	// プレイヤ－の体力と魔力
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BAR,		&g_pD3DTexBar);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_HP,		&g_pD3DTexHp);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_HP_SHAD,	&g_pD3DTexHpShad);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_STAM,	&g_pD3DTexStam);

	// UIの初期化処理
	for (int i = 0; i < UI_MAX; i++, ui++)
	{
		switch (i)
		{
		case PLAYER_BAR:// プレイヤ－の体力と魔力のプレ－ト
			ui->use = true;									// 使用
			ui->type = PLAYER_BAR;						// 種類の指定
			ui->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 座標データを初期化
			ui->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 回転データを初期化
			ui->size_x = TEXTURE_BAR_SIZE_X;		// テクスチャーの大きさ
			ui->size_y = TEXTURE_BAR_SIZE_Y;		// 同上

			ui->Texture = g_pD3DTexBar;			// テクスチャ情報
			MakeVertexUi(i);								// 頂点情報の作成
			break;
		case PLAYER_HP_SHAD:// プレイヤ－の体力
			ui->use = true;									// 使用
			ui->type = PLAYER_HP_SHAD;					// 種類の指定
			ui->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 座標データを初期化
			ui->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 回転データを初期化
			ui->size_x = TEXTURE_HP_SIZE_X_MAX;		// テクスチャーの大きさ
			ui->size_y = TEXTURE_HP_SIZE_Y;			// 同上

			ui->Texture = g_pD3DTexHp;			// テクスチャ情報
			MakeVertexUi(i);								// 頂点情報の作成
			break;
		case PLAYER_HP:// プレイヤ－の体力
			ui->use = true;									// 使用
			ui->type = PLAYER_HP;							// 種類の指定
			ui->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 座標データを初期化
			ui->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 回転データを初期化
			ui->size_x = TEXTURE_HP_SIZE_X_MAX;		// テクスチャーの大きさ
			ui->size_y = TEXTURE_HP_SIZE_Y;			// 同上

			ui->Texture = g_pD3DTexHp;			// テクスチャ情報
			MakeVertexUi(i);								// 頂点情報の作成
			break;
		case PLAYER_STAM:// プレイヤ－の魔力
			ui->use = true;									// 使用
			ui->type = PLAYER_STAM;							// 種類の指定
			ui->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 座標データを初期化
			ui->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 回転データを初期化
			ui->size_x = TEXTURE_STAM_SIZE_X_MAX;		// テクスチャーの大きさ
			ui->size_y = TEXTURE_STAM_SIZE_Y;			// 同上

			ui->Texture = g_pD3DTexStam;			// テクスチャ情報
			MakeVertexUi(i);								// 頂点情報の作成
			break;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitUi(void)
{
	UI *ui = &uiWk[0];

	for (int i = 0; i < UI_MAX; i++, ui++)
	{
		if (ui->Texture != NULL)
		{	// テクスチャの開放
			ui->Texture->Release();
			ui->Texture = NULL;
		}
	}

	if (g_pD3DTexBar != NULL)
	{
		g_pD3DTexBar->Release();
		g_pD3DTexBar = NULL;
	}
	if (g_pD3DTexHp != NULL)
	{
		g_pD3DTexHp->Release();
		g_pD3DTexHp = NULL;
	}
	if (g_pD3DTexHpShad != NULL)
	{
		g_pD3DTexHpShad->Release();
		g_pD3DTexHpShad = NULL;
	}
	if (g_pD3DTexStam != NULL)
	{
		g_pD3DTexStam->Release();
		g_pD3DTexStam = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateUi(void)
{
	UI *ui = &uiWk[0];
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < UI_MAX; i++, ui++)
	{
		if (ui->use == true)			// 使用している状態なら更新する
		{
			switch (ui->type)
			{
			case PLAYER_BAR:		// プレイヤ－の体力と魔力を表示するプレ－ト
				ui->pos.x = 10 + TEXTURE_BAR_SIZE_X;
				ui->pos.y = 10 + TEXTURE_BAR_SIZE_Y;
				break;
			case PLAYER_HP_SHAD:	// プレイヤ－の体力
				ui->pos.x = 16 + TEXTURE_HP_SIZE_X;
				ui->pos.y = 14 + TEXTURE_HP_SIZE_Y;
				ui->size_x = TEXTURE_HP_SHAD_SIZE_X;
				break;
			case PLAYER_HP:			// プレイヤ－の体力
				ui->pos.x = 16 + TEXTURE_HP_SIZE_X;
				ui->pos.y = 14 + TEXTURE_HP_SIZE_Y;
				ui->size_x = TEXTURE_HP_SIZE_X;
				break;
			case PLAYER_STAM:			// プレイヤ－の魔力
				ui->pos.x = 16 + TEXTURE_STAM_SIZE_X;
				ui->pos.y = 53 + TEXTURE_STAM_SIZE_Y;
				ui->size_x = TEXTURE_STAM_SIZE_X;
				break;
			}

			SetVertexUi(i);	// 移動後の座標で頂点を設定
		}
	}


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawUi(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	UI *ui = &uiWk[0];

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < UI_MAX; i++, ui++)
	{
		if (ui->use == true)			// 使用している状態なら描画する
		{
			// テクスチャの設定
			pDevice->SetTexture(0, ui->Texture);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_UI, ui->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexUi(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	UI *ui = &uiWk[no];

	// 頂点座標の設定
	ui->vertexWk[0].vtx = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	ui->vertexWk[1].vtx = D3DXVECTOR3(100.0f + ui->size_x, 100.0f, 0.0f);
	ui->vertexWk[2].vtx = D3DXVECTOR3(100.0f, 100.0f + ui->size_y, 0.0f);
	ui->vertexWk[3].vtx = D3DXVECTOR3(100.0f + ui->size_x, 100.0f + ui->size_y, 0.0f);

	// rhwの設定
	ui->vertexWk[0].rhw =
		ui->vertexWk[1].rhw =
		ui->vertexWk[2].rhw =
		ui->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	ui->vertexWk[0].diffuse = 
		ui->vertexWk[1].diffuse = 
		ui->vertexWk[2].diffuse = 
		ui->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	ui->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	ui->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	ui->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	ui->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}


//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexUi(int no)
{
	UI *ui = &uiWk[no];

	// 頂点座標の設定
	ui->vertexWk[0].vtx.x = ui->pos.x - ui->size_x;
	ui->vertexWk[0].vtx.y = ui->pos.y - ui->size_y;
	ui->vertexWk[0].vtx.z = 0.0f;

	ui->vertexWk[1].vtx.x = ui->pos.x + ui->size_x;
	ui->vertexWk[1].vtx.y = ui->pos.y - ui->size_y;
	ui->vertexWk[1].vtx.z = 0.0f;

	ui->vertexWk[2].vtx.x = ui->pos.x - ui->size_x;
	ui->vertexWk[2].vtx.y = ui->pos.y + ui->size_y;
	ui->vertexWk[2].vtx.z = 0.0f;

	ui->vertexWk[3].vtx.x = ui->pos.x + ui->size_x;
	ui->vertexWk[3].vtx.y = ui->pos.y + ui->size_y;
	ui->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// ブレット取得関数
//=============================================================================
UI *GetUi(int no)
{
	return(&uiWk[no]);
}

