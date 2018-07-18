//=============================================================================
//
// UIの処理 [ui.h]
// Author : 
//
//=============================================================================
#ifndef _UI_H_
#define _UI_H_

#include "main.h"


// マクロ定義
#define	NUM_UI		(2)					// ポリゴン数


//*****************************************************************************
// 構造体宣言
//*****************************************************************************

typedef struct	// プレイヤー構造体
{
	bool			use;						// true:使用  false:未使用44
	D3DXVECTOR3		pos;						// ポリゴンの移動量
	D3DXVECTOR3		rot;						// ポリゴンの回転量
	int				type;
	int				size_x;
	int				size_y;

	LPDIRECT3DTEXTURE9	Texture;				// テクスチャ情報
	VERTEX_2D		vertexWk[NUM_VERTEX];		// 頂点情報格納ワーク

} UI;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitUi(void);
void UninitUi(void);
void UpdateUi(void);
void DrawUi(void);
void SetUi(D3DXVECTOR3 pos);
UI *GetUi(int no);


#endif
