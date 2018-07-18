//=============================================================================
//
// NPC���� [NPC.h]
// Author : ���ꐶ
//
//=============================================================================
#ifndef _NPC_H_
#define _NPC_H_

#include "main.h"

//=============================================================================
// �}�N����`

typedef enum
{
	PIG_TYPE,
	NPC_TYPE_MAX
}NPC_TYPE;


#define NPC_MAX	(1)			// �t�B�[���h�̏�̍ő吔

//=============================================================================
// �\���̐錾

typedef struct
{
	bool					use;					// �g�p���Ă��邩
	// �}�b�v��̈ړ��Ȃ�
	D3DXVECTOR3				Pos;					// �ʒu
	D3DXVECTOR3				Rot;					// ����(��])
	D3DXVECTOR3				Scl;					// �傫��(�X�P�[��)
	D3DXVECTOR3				Vel;					// �ړ���

	int						Frames;					// �ړI�n�܂ł̃t���[��
	int						FrameCnt;				// ���݂̃t���[���J�E���g


	// �e�N�X�`���[�E���f��
	LPDIRECT3DVERTEXBUFFER9 VtxBuff = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
	LPD3DXMESH				Mesh;					// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER			BuffMat;				// �}�e���A�����ւ̃|�C���^
	DWORD					NumMat;					// �}�e���A�����̐�
	D3DXMATRIX				mtxWorld;				// ���[���h�}�g���b�N�X

	// �e

	//���̑�
	int						team;					// �G�������𔻒f����
}NPC;



//=============================================================================
// �v���g�^�C�v�錾
HRESULT InitNpc(void);
void UninitNpc(void);
void UpdateNpc(void);
void DrawNpc(void);

NPC *GetNpc(int no);
#endif
