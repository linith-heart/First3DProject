//=============================================================================
//
// ��C���� [cannon.h]
// Author : ���ꐶ
//
//=============================================================================
#ifndef _CANNON_H_
#define _CANNON_H_

#include "main.h"
#include "enemy.h"

//=============================================================================
// �}�N����`

#define CAN_MAX		(1)


//=============================================================================
// �\���̐錾

typedef struct
{
	ENE			ene;		// �G�l�~�[�̊�{�\����
	int			ShotCD;		// �e���˂̃N�[���_�E��
}CAN;



//=============================================================================
// �v���g�^�C�v�錾
HRESULT InitCannon(void);
void UninitCannon(void);
void UpdateCannon(void);
void DrawCannon(void);
CAN *GetCannon(int no);
#endif