//=============================================================================
//
// メッシュ水の処理 [meshwater.h]
// Author : 川井一生
//
//=============================================================================
#ifndef _MESHWATER_H_
#define _MESHWATER_H_

#include "main.h"

//=============================================================================
// プロトタイプ宣言
HRESULT InitMeshWater(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
int nNumBlockX, int nNumBlockZ, float nBlockSizeX, float nBlockSizeZ);
void UninitMeshWater(void);
void UpdateMeshWater(void);
void DrawMeshWater(void);

#endif
