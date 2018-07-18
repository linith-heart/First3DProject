//=============================================================================
//
// 計算処理 [mathWk.h]
// Author : 川井一生
//
//=============================================================================
#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include "main.h"


//=============================================================================
// プロトタイプ宣言
#ifdef _DEBUG
void UpdateCollision(void);
#endif
void PlayerCollision(D3DXVECTOR3 oldPos);
void GroundCollision(D3DXVECTOR3 *Pos, float *Grav);


#endif