//=============================================================================
//
// マップ処理 [map.h]
// Author : 川井一生
//
//=============================================================================
#ifndef _MAP_H_
#define _MAP_H_

#include "main.h"
#include "environment hub.h"

//=============================================================================
// マクロ定義

#define COMP4MAP	(4)			// 一つのマップに必要なメッシュの数		count of components required to create the map

#define MAP_MAX		(1 * COMP4MAP)		// 表示用と当たり判定用


//=============================================================================
// 構造体宣言
typedef struct
{
	MESH					mesh;
	bool					isHitBox;			// 当たり判定用であるか
}MAP;

//=============================================================================
// プロトタイプ宣言
HRESULT InitMap(void);
void UninitMap(void);
void UpdateMap(void);
void DrawMap(void);
MAP *GetMap(int no);

#endif
