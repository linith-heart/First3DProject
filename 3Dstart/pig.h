//=============================================================================
//
// �ԏ��� [pig.h]
// Author : ���ꐶ
//
//=============================================================================
#ifndef _PIG_H_
#define _PIG_H_

#include "main.h"
#include "NPC.h"

//=============================================================================
// �}�N����`

#define PIG_MAX		(1)


//=============================================================================
// �\���̐錾

typedef struct
{
	NPC			npc;		// NPC�̊�{�\����		base struct for NPC types
	bool		GoHome;		// �ƂɌ������Ă邩		if pig is going to go to house
	bool		LetOut;		// ������邩			if pig is to be let out


	bool		isUpAnim;	// �A�j���[�V�������Ă��邩			if there is animation
	float		fUpAnim;	// �A�j�}�[�V�������̈ړ���			move value during animation
	float		AnimPos;	// �A�j���[�V�������̈ʒu			position value during animation
	
}PIG;



//=============================================================================
// �v���g�^�C�v�錾
HRESULT InitPig(void);
void UninitPig(void);
void UpdatePig(void);
void DrawPig(void);
PIG *GetPig(int no);
#endif
