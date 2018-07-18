//=============================================================================
//
// UIのテキストプロンプト処理 [help_text.h]
// Author : 川井一生
//
//=============================================================================
#ifndef _HELP_TEXT_H_
#define _HELP_TEXT_H_

#include "main.h"

//=============================================================================
// 構造体宣言

typedef struct
{
	bool					use;						// 使用しているか

	LPDIRECT3DVERTEXBUFFER9 D3DVtxBuff = NULL;			// 頂点バッファインターフェースへのポインタ	pointer to vertex buffer
	int						AppearCnt;					// 表示/消えるまでの時間		time till display/dissapear
	int						DisplayCnt;					// 表示する時間					time displayed
	float					AlphaCnt;					// テキストのアルファ値			alpha value

	D3DXVECTOR2				Pos;						// 表示の位置
	D3DXVECTOR2				Size;						// 表示の大きさ

	int						TextId;						// 表示知るテクスチャーのID番号
}HELP_TEXT;

//=============================================================================
// マクロ定義

#define HELP_TEXT_MAX	(3)


//=============================================================================
// プロトタイプ宣言
HRESULT InitHelpText(void);
void SetHelpText(int no);
void UninitHelpText(void);
void ReleaseHelpText(int no);
void UpdateHelpText(void);
void DrawHelpText(void);
void SetAlphaHelpText(int no);
HELP_TEXT GetHelpText(int no);
#endif
