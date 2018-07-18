//=============================================================================
//
// ������ [crate.h]
// Author : ���ꐶ
//
//=============================================================================
#ifndef _CRATE_H_
#define _CRATE_H_

#include "main.h"
#include "environment hub.h"

//=============================================================================
// �}�N����`

#define CRATE_MAX		(17)

#define CRATE_DATA_START	("##Crate,,,,")	// ���̃f�[�^�̎n�܂������

//=============================================================================
// �\���̐錾

typedef struct
{
	MESH		mesh;		// ���b�V���I�u�W�F�N�g�̊�{�\����
	bool		isSolid;	// �������邩
}CRATE;



//=============================================================================
// �v���g�^�C�v�錾
HRESULT InitCrate(void);
void UninitCrate(void);
void UpdateCrate(void);
void DrawCrate(void);

CRATE *GetCrate(int no);
#endif
