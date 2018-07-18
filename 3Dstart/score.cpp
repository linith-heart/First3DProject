//=============================================================================
//
// SCOREの処理 [SCORE.cpp]
// Author : 川井 一生
//
//=============================================================================
#include "main.h"
#include "score.h"
#include "gem.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
// 読み込むテクスチャファイル名
#define	TEX_NUMBER		"data/TEXTURE/number000.png"
#define	TEX_SLASH		"data/TEXTURE/slash000.png"
#define TEX_NUM_PATTERN_X	(5)
#define TEX_NUM_PATTERN_Y	(2)
// NUMBER と　SLASH　の表示の大きさは一緒
#define	TEX_WIDTH		(50)						// 操作の幅
#define	TEX_HEIGHT		(70)						// 操作の高さ

#define TEXT_COLOR		(D3DCOLOR_RGBA(255, 153, 48,255))

// スラッシュを中心に現在とセットを左へと、右へと座標セット
#define BASE_POS_X		((SCREEN_WIDTH / 2) + 100)
#define BASE_POS_Y		(SCREEN_HEIGHT * 0.07f)

#define	CUR_POS_X		(BASE_POS_X - TEX_WIDTH)
#define	CUR_POS_Y		(BASE_POS_Y)

#define	SLASH_POS_X		(BASE_POS_X)
#define	SLASH_POS_Y		(BASE_POS_Y)

#define SET_POS_X		(BASE_POS_X + TEX_WIDTH)
#define SET_POS_Y		(BASE_POS_Y)

enum
{
	Idx_NUM = 0,
	Idx_SLA,
	TEX_MAX,				// スコアにいる数
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexScore(int no);
void SetVertexScore(int no);
void SetTextureScore(int no, int cntPattern);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTexScore[TEX_MAX] = { NULL, NULL };		// テクスチャへのポリゴン


SCORE					scoreWk[SCORE_MAX];	// SCORE構造体


//=============================================================================
// 初期化処理
HRESULT InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SCORE *score = &scoreWk[0];

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEX_NUMBER, &D3DTexScore[Idx_NUM]);
	D3DXCreateTextureFromFile(pDevice, TEX_SLASH, &D3DTexScore[Idx_SLA]);

	// SCOREの初期化処理
	for (int i = 0; i < SCORE_MAX; i++, score++)
	{
		switch (i)
		{
		case SCORE_CUR:// 現在のジェムの数
			score->use = true;								// 使用
			score->TexId = Idx_NUM;							// テクスチャの指定
			score->Pos = { CUR_POS_X , CUR_POS_Y};			// 座標データを初期化
			score->Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 回転データを初期化
			score->Size = {TEX_WIDTH, TEX_HEIGHT};			// テクスチャーの大きさ
			score->Val = 0;

			MakeVertexScore(i);								// 頂点情報の作成
			break;
		case SCORE_SLASH:// スラッシュ
			score->use = true;								// 使用
			score->TexId = Idx_SLA;							// テクスチャの指定
			score->Pos = { SLASH_POS_X , SLASH_POS_Y };		// 座標データを初期化
			score->Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 回転データを初期化
			score->Size = { TEX_WIDTH, TEX_HEIGHT };		// テクスチャーの大きさ
			score->Val = 0;

			MakeVertexScore(i);								// 頂点情報の作成
			break;
		case SCORE_SET:// マップ上のジェムの数
			score->use = true;								// 使用
			score->TexId = Idx_NUM;							// テクスチャの指定
			score->Pos = { SET_POS_X , SET_POS_Y };			// 座標データを初期化
			score->Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 回転データを初期化
			score->Size = { TEX_WIDTH, TEX_HEIGHT };		// テクスチャーの大きさ
			score->Val = GEM_MAX;

			MakeVertexScore(i);								// 頂点情報の作成
			break;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
void UninitScore(void)
{
	for (int i = 0; i < TEX_MAX; i++)
	{
		SAFE_RELEASE (D3DTexScore[i]);
	}
}

//=============================================================================
// 更新処理
void UpdateScore(void)
{
	SCORE *score = &scoreWk[0];
	GEM *gem = GetGem(0);

	int j, tCnt = 0;

	for (int i = 0; i < SCORE_MAX; i++, score++)
	{
		if (score->use == true)			// 使用している状態なら更新する
		{
			switch (i)
			{
			case SCORE_CUR:
				for (j = 0; j < GEM_MAX; j++, gem++)
					if (!gem->mesh.use)
						tCnt++;

				score->Val = tCnt;

				SetTextureScore(i, score->Val);
				break;
			case SCORE_SLASH:

				break;
			case SCORE_SET:
				SetTextureScore(i, score->Val);
				break;
			}

		}
	}


}

//=============================================================================
// 描画処理
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SCORE *score = &scoreWk[0];

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < SCORE_MAX; i++, score++)
	{
		if (score->use == true)			// 使用している状態なら描画する
		{
			// テクスチャの設定
			pDevice->SetTexture(0, D3DTexScore[score->TexId]);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_SCORE, score->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
HRESULT MakeVertexScore(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SCORE *score = &scoreWk[no];

	// 頂点座標の設定
	score->vertexWk[0].vtx = D3DXVECTOR3(score->Pos.x - (score->Size.x / 2), score->Pos.y - (score->Size.y / 2), 0.0f);
	score->vertexWk[1].vtx = D3DXVECTOR3(score->Pos.x + (score->Size.x / 2), score->Pos.y - (score->Size.y / 2), 0.0f);
	score->vertexWk[2].vtx = D3DXVECTOR3(score->Pos.x - (score->Size.x / 2), score->Pos.y + (score->Size.y / 2), 0.0f);
	score->vertexWk[3].vtx = D3DXVECTOR3(score->Pos.x + (score->Size.x / 2), score->Pos.y + (score->Size.y / 2), 0.0f);

	// rhwの設定
	score->vertexWk[0].rhw =
		score->vertexWk[1].rhw =
		score->vertexWk[2].rhw =
		score->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	score->vertexWk[0].diffuse = TEXT_COLOR;
	score->vertexWk[1].diffuse = TEXT_COLOR;
	score->vertexWk[2].diffuse = TEXT_COLOR;
	score->vertexWk[3].diffuse = TEXT_COLOR;

	// テクスチャ座標の設定
	score->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	score->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	score->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	score->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
void SetTextureScore(int no, int cntPattern)
{
	SCORE *score = &scoreWk[no];

	// テクスチャ座標の設定
	int x = cntPattern % TEX_NUM_PATTERN_X;
	int y = cntPattern / TEX_NUM_PATTERN_X;
	float sizeX = 1.0f / TEX_NUM_PATTERN_X;
	float sizeY = 1.0f / TEX_NUM_PATTERN_Y;
	score->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	score->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	score->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	score->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 頂点座標の設定
void SetVertexScore(int no)
{
	SCORE *score = &scoreWk[no];

	// 頂点座標の設定
	score->vertexWk[0].vtx.x = score->Pos.x - score->Size.x;
	score->vertexWk[0].vtx.y = score->Pos.y - score->Size.y;
	score->vertexWk[0].vtx.z = 0.0f;

	score->vertexWk[1].vtx.x = score->Pos.x + score->Size.x;
	score->vertexWk[1].vtx.y = score->Pos.y - score->Size.y;
	score->vertexWk[1].vtx.z = 0.0f;

	score->vertexWk[2].vtx.x = score->Pos.x - score->Size.x;
	score->vertexWk[2].vtx.y = score->Pos.y + score->Size.y;
	score->vertexWk[2].vtx.z = 0.0f;

	score->vertexWk[3].vtx.x = score->Pos.x + score->Size.x;
	score->vertexWk[3].vtx.y = score->Pos.y + score->Size.y;
	score->vertexWk[3].vtx.z = 0.0f;
}
