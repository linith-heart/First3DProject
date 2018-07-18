//=============================================================================
//
// 木処理 [tree.h]
// Author : 川井一生
//
//=============================================================================
#ifndef _TREE_H_
#define _TREE_H_

#include "main.h"
#include "environment hub.h"

//=============================================================================
// マクロ定義

#define TREE_MAX		(1)

#define TREE_DATA_START	("##Tree")	// 木のデータの始まりを示す

//=============================================================================
// 構造体宣言

typedef struct
{
	MESH		mesh;		// メッシュオブジェクトの基本構造体
}TREE;



//=============================================================================
// プロトタイプ宣言
HRESULT InitTree(void);
void UninitTree(void);
void UpdateTree(void);
void DrawTree(void);
int hitCheckTree(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);

HRESULT MakeVertexTree(LPDIRECT3DDEVICE9 pDevice);
TREE *GetTree(int no);
#endif
