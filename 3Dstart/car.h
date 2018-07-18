//=============================================================================
//
// �ԏ��� [car.h]
// Author : ���ꐶ
//
//=============================================================================
#ifndef _CAR_H_
#define _CAR_H_

#include "main.h"
#include "enemy.h"

//=============================================================================
// �}�N����`

#define CAR_MAX		(1)


//=============================================================================
// �\���̐錾

typedef struct
{
	ENE			ene;		// �G�l�~�[�̊�{�\����
	float		Turn;		// �G�l�~�[�̉�]��
}CAR;



//=============================================================================
// �v���g�^�C�v�錾
HRESULT InitCar(void);
void UninitCar(void);
void UpdateCar(void);
void DrawCar(void);
CAR *GetCar(int no);
#endif