//=============================================================================
//
// �v�Z���� [mathWk.h]
// Author : ���ꐶ
//
//=============================================================================
#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include "main.h"


//=============================================================================
// �v���g�^�C�v�錾
#ifdef _DEBUG
void UpdateCollision(void);
#endif
void PlayerCollision(D3DXVECTOR3 oldPos);
void GroundCollision(D3DXVECTOR3 *Pos, float *Grav);


#endif