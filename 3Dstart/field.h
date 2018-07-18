//=============================================================================
//
// �n�ʏ��� [field.h]
// Author : ���ꐶ
//
//=============================================================================
#ifndef _FIELD_H_
#define _FIELD_H_

#include "main.h"

#define FIELD_SIZE_MAX	(100)						// �t�B�[���h�̑傫��
#define FIELD_MAX		(1)						// �t�B�[���h�̎g�p���Ă��鐔

typedef struct
{
	D3DXVECTOR3				Pos;					// �ʒu
	D3DXVECTOR3				Rot;					// ����(��])

	int						BlkCntX, BlkCntZ;		// �u���b�N��
	int						Vtx;					// �����_��
	int						VtxIdx;					// ���C���f�b�N�X��
	int						PolyCnt;				// ���|���S����
	float					BlkSizeX, BlkSizeZ;		// �u���b�N�T�C�Y


	D3DXMATRIX				mtxWorld;				// ���[���h�}�g���b�N�X

}FIELD;



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nNumBlockX, int nNumBlockZ, float fBlockSizeX, float fBlockSizeZ);
void UninitField(void);
void UpdateField(void);
void DrawField(void);
void *GetField(int no);
#endif
