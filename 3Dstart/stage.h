//=============================================================================
//
// 台の処理 [stage.h]
// Author : 川井一生
//
//=============================================================================
#ifndef _STAGE_H_
#define _STAGE_H_

#include "main.h"
#include "environment hub.h"

//=============================================================================
// マクロ定義

#define COMP4STAGE		(2)				// 台を作るのに必要な数
#define STAGE_MAX		(3 * COMP4STAGE)

#define STAGE_DATA_START	("##Stage") // 台のデータの始まりを示す

//=============================================================================
// 構造体宣言

typedef struct
{
	MESH		mesh;		// メッシュオブジェクトの基本構造体
	bool		isHitbox;
}STAGE;



//=============================================================================
// プロトタイプ宣言
HRESULT InitStage(void);
void UninitStage(void);
void UpdateStage(void);
void DrawStage(void);
int hitCheckStage(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);

HRESULT MakeVertexStage(LPDIRECT3DDEVICE9 pDevice);
STAGE *GetStage(int no);
#endif
