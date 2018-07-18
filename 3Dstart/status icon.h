//=============================================================================
//
// ステータスアイコン処理 [status icon.h]
// Author : 川井一生
//
//=============================================================================
#ifndef _STATUS_ICON_H_
#define _STATUS_ICON_H_

#include "main.h"

//=============================================================================
// マクロ定義

#define BULL_MAX			(50)				// バレットの使用している数		max bullet count


enum
{
	Idx_FOUND,
	Idx_CHECK,
	STAT_ICO_MAX,
};

//=============================================================================
// 構造体宣言

typedef struct
{
	bool					use;					// 使用しているか		if it is being used

	D3DXVECTOR3				Pos;					// 位置					posoton
	D3DXVECTOR3				Scl;					// 大きさ(スケール)		scale
	D3DXVECTOR3				Vel;					// 移動量				velocity

	D3DXMATRIX				mtxWorld;				// ワールドマトリックス	world matrax
	int						TexID;					// テクスチャー番号		texture id

	int						fCount;					// フレームによって時間を設定	count for active frames

}STAT_ICO;



//=============================================================================
// プロトタイプ宣言
HRESULT InitStatIco(void);
void UninitStatIco(void);
void UpdateStatIco(void);
void DrawStatIco(void);
void SetStatIco(D3DXVECTOR3 Pos, int Stat);

STAT_ICO *GetStatIco(int no);
#endif
