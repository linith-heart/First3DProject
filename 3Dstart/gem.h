//=============================================================================
//
// ジェム処理 [gem.h]
// Author : 川井一生
//
//=============================================================================
#ifndef _GEM_H_
#define _GEM_H_

#include "main.h"
#include "environment hub.h"

//=============================================================================
// マクロ定義

#define GEM_MAX		(7)


//=============================================================================
// 構造体宣言

typedef struct
{
	MESH		mesh;		// メッシュオブジェクトの基本構造体
}GEM;



//=============================================================================
// プロトタイプ宣言
HRESULT InitGem(void);
void ResetGem(void);
void UninitGem(void);
void UpdateGem(void);
void DrawGem(void);
int hitCheckGem(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);

HRESULT MakeVertexGem(LPDIRECT3DDEVICE9 pDevice);
GEM *GetGem(int no);
#endif
