//=============================================================================
//
// NPC処理 [NPC.h]
// Author : 川井一生
//
//=============================================================================
#ifndef _NPC_H_
#define _NPC_H_

#include "main.h"

//=============================================================================
// マクロ定義

typedef enum
{
	PIG_TYPE,
	NPC_TYPE_MAX
}NPC_TYPE;


#define NPC_MAX	(1)			// フィールドの上の最大数

//=============================================================================
// 構造体宣言

typedef struct
{
	bool					use;					// 使用しているか
	// マップ上の移動など
	D3DXVECTOR3				Pos;					// 位置
	D3DXVECTOR3				Rot;					// 向き(回転)
	D3DXVECTOR3				Scl;					// 大きさ(スケール)
	D3DXVECTOR3				Vel;					// 移動量

	int						Frames;					// 目的地までのフレーム
	int						FrameCnt;				// 現在のフレームカウント


	// テクスチャー・モデル
	LPDIRECT3DVERTEXBUFFER9 VtxBuff = NULL;			// 頂点バッファへのポインタ
	LPD3DXMESH				Mesh;					// メッシュ情報へのポインタ
	LPD3DXBUFFER			BuffMat;				// マテリアル情報へのポインタ
	DWORD					NumMat;					// マテリアル情報の数
	D3DXMATRIX				mtxWorld;				// ワールドマトリックス

	// 影

	//その他
	int						team;					// 敵か味方を判断する
}NPC;



//=============================================================================
// プロトタイプ宣言
HRESULT InitNpc(void);
void UninitNpc(void);
void UpdateNpc(void);
void DrawNpc(void);

NPC *GetNpc(int no);
#endif
