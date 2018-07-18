//=============================================================================
//
// UI�̃e�L�X�g�v�����v�g���� [help_text.h]
// Author : ���ꐶ
//
//=============================================================================
#ifndef _HELP_TEXT_H_
#define _HELP_TEXT_H_

#include "main.h"

//=============================================================================
// �\���̐錾

typedef struct
{
	bool					use;						// �g�p���Ă��邩

	LPDIRECT3DVERTEXBUFFER9 D3DVtxBuff = NULL;			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^	pointer to vertex buffer
	int						AppearCnt;					// �\��/������܂ł̎���		time till display/dissapear
	int						DisplayCnt;					// �\�����鎞��					time displayed
	float					AlphaCnt;					// �e�L�X�g�̃A���t�@�l			alpha value

	D3DXVECTOR2				Pos;						// �\���̈ʒu
	D3DXVECTOR2				Size;						// �\���̑傫��

	int						TextId;						// �\���m��e�N�X�`���[��ID�ԍ�
}HELP_TEXT;

//=============================================================================
// �}�N����`

#define HELP_TEXT_MAX	(3)


//=============================================================================
// �v���g�^�C�v�錾
HRESULT InitHelpText(void);
void SetHelpText(int no);
void UninitHelpText(void);
void ReleaseHelpText(int no);
void UpdateHelpText(void);
void DrawHelpText(void);
void SetAlphaHelpText(int no);
HELP_TEXT GetHelpText(int no);
#endif
