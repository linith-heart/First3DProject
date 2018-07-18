//=============================================================================
//
// 石処理 [rock.h]
// Author : 川井一生
//
//=============================================================================
#ifndef _ROCK_H_
#define _ROCK_H_

#include "main.h"
#include "environment hub.h"

//=============================================================================
// マクロ定義

#define COMP4ROCK		(2)		// 一つの石を製作すのに必要な数
#define ROCK_MAX		(15 * COMP4ROCK)

#define ROCK_DATA_START	("##Rock")	// 石のデータの始まりを示す

//=============================================================================
// 構造体宣言

typedef struct
{
	MESH		mesh;		// メッシュオブジェクトの基本構造体
	bool		isHitbox;	// 当たり判定用のメッシュであるか
	int			TypeIdx;
}ROCK;



//=============================================================================
// プロトタイプ宣言
HRESULT InitRock(void);
void UninitRock(void);
void UpdateRock(void);
void DrawRock(void);
int hitCheckRock(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);

HRESULT MakeVertexRock(LPDIRECT3DDEVICE9 pDevice);
ROCK *GetRock(int no);
#endif
