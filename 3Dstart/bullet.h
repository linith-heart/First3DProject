//=============================================================================
//
// バレット処理 [bullet.h]
// Author : 川井一生
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//=============================================================================
// マクロ定義

#define	BULL_SIZE_X			(5.0f)				// バレットのサイズ(X方向)		size of the x axis
#define	BULL_SIZE_Z			(5.0f)				// バレットのサイズ(Z方向)		size of the y axis

#define BULL_MAX			(50)				// バレットの使用している数		max bullet count

//=============================================================================
// 構造体宣言

typedef struct
{
	bool					use;					// 使用しているか		if it is being used

	D3DXVECTOR3				Pos;					// 位置					posoton
	D3DXVECTOR3				Scl;					// 大きさ(スケール)		scale
	D3DXVECTOR3				Vel;					// 移動量				velocity

	D3DXMATRIX				mtxWorld;				// ワールドマトリックス	world matrax
	int						TexID;					// テクスチャー番号		texture id

	int						fCount;					// フレームによって時間を設定	count for active frames

	int						nIdxShadow;				// 影ID					shaodow id
	float					fSizeShadow;			// 影のサイズ			shoadow size
	D3DXCOLOR				colShadow;				// 影の色				shoadow color

}BULL;



//=============================================================================
// プロトタイプ宣言
HRESULT InitBull(void);
void UninitBull(void);
void UpdateBull(void);
void DrawBull(void);
void SetBull(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot);

BULL *GetBull(int no);
#endif
