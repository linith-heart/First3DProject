//=============================================================================
//
// 箱処理 [crate.h]
// Author : 川井一生
//
//=============================================================================
#ifndef _CRATE_H_
#define _CRATE_H_

#include "main.h"
#include "environment hub.h"

//=============================================================================
// マクロ定義

#define CRATE_MAX		(17)

#define CRATE_DATA_START	("##Crate,,,,")	// 箱のデータの始まりを示す

//=============================================================================
// 構造体宣言

typedef struct
{
	MESH		mesh;		// メッシュオブジェクトの基本構造体
	bool		isSolid;	// 動かせるか
}CRATE;



//=============================================================================
// プロトタイプ宣言
HRESULT InitCrate(void);
void UninitCrate(void);
void UpdateCrate(void);
void DrawCrate(void);

CRATE *GetCrate(int no);
#endif
