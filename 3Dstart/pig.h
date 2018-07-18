//=============================================================================
//
// 車処理 [pig.h]
// Author : 川井一生
//
//=============================================================================
#ifndef _PIG_H_
#define _PIG_H_

#include "main.h"
#include "NPC.h"

//=============================================================================
// マクロ定義

#define PIG_MAX		(1)


//=============================================================================
// 構造体宣言

typedef struct
{
	NPC			npc;		// NPCの基本構造体		base struct for NPC types
	bool		GoHome;		// 家に向かってるか		if pig is going to go to house
	bool		LetOut;		// 解放するか			if pig is to be let out


	bool		isUpAnim;	// アニメーションしているか			if there is animation
	float		fUpAnim;	// アニマーション時の移動量			move value during animation
	float		AnimPos;	// アニメーション時の位置			position value during animation
	
}PIG;



//=============================================================================
// プロトタイプ宣言
HRESULT InitPig(void);
void UninitPig(void);
void UpdatePig(void);
void DrawPig(void);
PIG *GetPig(int no);
#endif
