//=============================================================================
//
// 方向矢印の処理 [waypoint.h]
// Author : 川井一生
//
//=============================================================================
#ifndef _WAYPOINT_H_
#define _WAYPOINT_H_

#include "main.h"

//=============================================================================
// マクロ定義


//=============================================================================
// 構造体宣言
typedef struct
{
	bool					use;					// 使用しているか

	// マップ上の位置など
	D3DXVECTOR3				Pos;					// 位置
	D3DXVECTOR3				Rot;					// 向き(回転)
	D3DXVECTOR3				Scl;					// 大きさ(スケール)

	// テクスチャー・モデル
	LPDIRECT3DVERTEXBUFFER9 VtxBuff = NULL;			// 頂点バッファへのポインタ
	LPD3DXMESH				Mesh;					// メッシュ情報へのポインタ
	LPD3DXBUFFER			BuffMat;				// マテリアル情報へのポインタ
	DWORD					NumMat;					// マテリアル情報の数
	D3DXMATRIX				mtxWorld;				// ワールドマトリックス

}WAYP;

//=============================================================================
// プロトタイプ宣言
HRESULT InitWaypoint(void);
void UninitWyapoint(void);
void UpdateWaypoint(void);
void DrawWaypoint(void);
WAYP *GetWaypoint(int no);
#endif