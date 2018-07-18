//=============================================================================
//
// 門の処理 [gate.h]
// Author : 川井一生
//
//=============================================================================
#ifndef _GATE_H_
#define _GATE_H_

#include "main.h"
#include "environment hub.h"

//=============================================================================
// マクロ定義
#define COMP4GATE		(6)			// 一つのゲートを作るのに必要な数	components required for creation the gate 
#define GATE_MAX		(1 * COMP4GATE)

#define GATE_DATA_START	("##Gate")	// ゲートのデータの始まりを示す

//=============================================================================
// 構造体宣言

typedef struct
{
	MESH		mesh;		// メッシュオブジェクトの基本構造体
	bool		isHitbox;	// 当たり判定用か描画用か
}GATE;



//=============================================================================
// プロトタイプ宣言
HRESULT InitGate(void);
void ResetGate(void);
void UninitGate(void);
void UpdateGate(void);
void DrawGate(void);
int hitCheckGate(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);

HRESULT MakeVertexGate(LPDIRECT3DDEVICE9 pDevice);
GATE *GetGate(int no);
#endif
