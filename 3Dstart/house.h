//=============================================================================
//
// �ԏ��� [house.h]
// Author : ���ꐶ
//
//=============================================================================
#ifndef _HOUSE_H_
#define _HOUSE_H_

#include "main.h"
#include "environment hub.h"

//=============================================================================
// �}�N����`

#define HOUSE_MAX		(1)


//=============================================================================
// �\���̐錾

typedef struct
{
	MESH		mesh;		//���b�V���I�u�W�F�N�g�̊�{�\����
}HOUSE;



//=============================================================================
// �v���g�^�C�v�錾
HRESULT InitHouse(void);
void UninitHouse(void);
void UpdateHouse(void);
void DrawHouse(void);
HOUSE *GetHouse(int no);
#endif
