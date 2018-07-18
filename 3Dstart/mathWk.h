//=============================================================================
//
// 計算処理 [mathWk.h]
// Author : 川井一生
//
//=============================================================================
#ifndef _MATHWK_H_
#define _MATHWK_H_

#include "main.h"


//=============================================================================
// プロトタイプ宣言
D3DXVECTOR4 QuaternionMulty(D3DXVECTOR4 q1, D3DXVECTOR4 q2);
float dotProduct(D3DXVECTOR3 *v1, D3DXVECTOR3 *v2);
D3DXVECTOR3 *Vec3Cross(D3DXVECTOR3 *Out, D3DXVECTOR3 *vl, D3DXVECTOR3 *vr);
float Vec2Cross(D3DXVECTOR3 vl, D3DXVECTOR3 vr);
D3DXVECTOR3 *Vec3Normalize(D3DXVECTOR3 *v);
D3DXVECTOR3 Ang2Vec(D3DXVECTOR3 Rot, float VALUE);

bool isInFront(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 rot);
bool inView(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 rot, float rad);
#endif
