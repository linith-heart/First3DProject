//=============================================================================
//
// �o���b�g���� [bullet.h]
// Author : ���ꐶ
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//=============================================================================
// �}�N����`

#define	BULL_SIZE_X			(5.0f)				// �o���b�g�̃T�C�Y(X����)		size of the x axis
#define	BULL_SIZE_Z			(5.0f)				// �o���b�g�̃T�C�Y(Z����)		size of the y axis

#define BULL_MAX			(50)				// �o���b�g�̎g�p���Ă��鐔		max bullet count

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

	int						nIdxShadow;				// �eID					shaodow id
	float					fSizeShadow;			// �e�̃T�C�Y			shoadow size
	D3DXCOLOR				colShadow;				// �e�̐F				shoadow color

}BULL;



//=============================================================================
// �v���g�^�C�v�錾
HRESULT InitBull(void);
void UninitBull(void);
void UpdateBull(void);
void DrawBull(void);
void SetBull(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot);

BULL *GetBull(int no);
#endif
