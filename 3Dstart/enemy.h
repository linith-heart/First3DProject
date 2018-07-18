//=============================================================================
//
// エネミー処理 [enemy.h]
// Author : 川井一生
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

//=============================================================================
// マクロ定義

typedef enum
{
	CAR_TYPE = 0,
	SIGHT_TYPE,
	ENE_TYPE_MAX
}ENE_TYPE;


#define ENE_MAX	(1)			// フィールドの上の最大数

//=============================================================================
// 構造体宣言

typedef struct
{
	bool					use;					// 使用しているか
	// マップ上の移動など
	D3DXVECTOR3				Pos;					// 位置
	D3DXVECTOR3				Rot;					// 向き(回転)
	D3DXVECTOR3				Scl;					// 大きさ(スケール)
	float					Grav;

	// テクスチャー・モデル
	LPDIRECT3DVERTEXBUFFER9 VtxBuff = NULL;			// 頂点バッファへのポインタ
	LPD3DXMESH				Mesh;					// メッシュ情報へのポインタ
	LPD3DXBUFFER			BuffMat;				// マテリアル情報へのポインタ
	DWORD					NumMat;					// マテリアル情報の数
	D3DXMATRIX				mtxWorld;				// ワールドマトリックス

	// 影
	int						nIdxShadow;				// 影ID
	float					fSizeShadow;			// 影のサイズ
	D3DXCOLOR				colShadow;				// 影の色


	//その他
	int						team;					// 敵か味方を判断する


}ENE;



//=============================================================================
// プロトタイプ宣言
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

ENE *GetEnemy(void);
#endif
