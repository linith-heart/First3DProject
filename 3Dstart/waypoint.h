//=============================================================================
//
// �������̏��� [waypoint.h]
// Author : ���ꐶ
//
//=============================================================================
#ifndef _WAYPOINT_H_
#define _WAYPOINT_H_

#include "main.h"

//=============================================================================
// �}�N����`


//=============================================================================
// �\���̐錾
typedef struct
{
	bool					use;					// �g�p���Ă��邩

	// �}�b�v��̈ʒu�Ȃ�
	D3DXVECTOR3				Pos;					// �ʒu
	D3DXVECTOR3				Rot;					// ����(��])
	D3DXVECTOR3				Scl;					// �傫��(�X�P�[��)

	// �e�N�X�`���[�E���f��
	LPDIRECT3DVERTEXBUFFER9 VtxBuff = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
	LPD3DXMESH				Mesh;					// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER			BuffMat;				// �}�e���A�����ւ̃|�C���^
	DWORD					NumMat;					// �}�e���A�����̐�
	D3DXMATRIX				mtxWorld;				// ���[���h�}�g���b�N�X

}WAYP;

//=============================================================================
// �v���g�^�C�v�錾
HRESULT InitWaypoint(void);
void UninitWyapoint(void);
void UpdateWaypoint(void);
void DrawWaypoint(void);
WAYP *GetWaypoint(int no);
#endif