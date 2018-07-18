//=============================================================================
//
// ���b�V�����̏��� [meshwater.h]
// Author : ���ꐶ
//
//=============================================================================
#ifndef _MESHWATER_H_
#define _MESHWATER_H_

#include "main.h"

//=============================================================================
// �v���g�^�C�v�錾
HRESULT InitMeshWater(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
int nNumBlockX, int nNumBlockZ, float nBlockSizeX, float nBlockSizeZ);
void UninitMeshWater(void);
void UpdateMeshWater(void);
void DrawMeshWater(void);

#endif
