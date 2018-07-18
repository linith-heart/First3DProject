//=============================================================================
//
// �X�e�[�^�X�A�C�R������ [status icon.h]
// Author : ���ꐶ
//
//=============================================================================
#ifndef _STATUS_ICON_H_
#define _STATUS_ICON_H_

#include "main.h"

//=============================================================================
// �}�N����`

#define BULL_MAX			(50)				// �o���b�g�̎g�p���Ă��鐔		max bullet count


enum
{
	Idx_FOUND,
	Idx_CHECK,
	STAT_ICO_MAX,
};

//=============================================================================
// �\���̐錾

typedef struct
{
	bool					use;					// �g�p���Ă��邩		if it is being used

	D3DXVECTOR3				Pos;					// �ʒu					posoton
	D3DXVECTOR3				Scl;					// �傫��(�X�P�[��)		scale
	D3DXVECTOR3				Vel;					// �ړ���				velocity

	D3DXMATRIX				mtxWorld;				// ���[���h�}�g���b�N�X	world matrax
	int						TexID;					// �e�N�X�`���[�ԍ�		texture id

	int						fCount;					// �t���[���ɂ���Ď��Ԃ�ݒ�	count for active frames

}STAT_ICO;



//=============================================================================
// �v���g�^�C�v�錾
HRESULT InitStatIco(void);
void UninitStatIco(void);
void UpdateStatIco(void);
void DrawStatIco(void);
void SetStatIco(D3DXVECTOR3 Pos, int Stat);

STAT_ICO *GetStatIco(int no);
#endif
