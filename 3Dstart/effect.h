//=============================================================================
//
// エフェクト処理 [effect.h]
// Author : 
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"

enum
{
	Idx_SPARK = 0,
	Idx_DUST00,
	Idx_DUST01,
	Idx_DUST02,
	//Idx_FOUND,
	//Idx_CHECK,
	TEX_MAX
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);

int SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nTimer, int TexId);

#endif
