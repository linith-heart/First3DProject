//=============================================================================
//
// 策の処理 [fence.h]
// Author : 川井一生
//
//=============================================================================
#ifndef _FENCE_H_
#define _FENCE_H_

#include "main.h"
#include "environment hub.h"

//=============================================================================
// マクロ定義

#define COMP4FENCE		(2)
#define FENCE_MAX		(6 * COMP4FENCE)

#define FENCE_DATA_START	("##Fence")	// 策のデータの始まりを示す

//=============================================================================
// 構造体宣言

typedef struct
{
	MESH		mesh;		// メッシュオブジェクトの基本構造体
	bool		isHitbox;
}FENCE;



//=============================================================================
// プロトタイプ宣言
HRESULT InitFence(void);
void UninitFence(void);
void UpdateFence(void);
void DrawFence(void);
int hitCheckFence(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);

HRESULT MakeVertexFence(LPDIRECT3DDEVICE9 pDevice);
FENCE *GetFence(int no);
#endif
