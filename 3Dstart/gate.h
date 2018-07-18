//=============================================================================
//
// ��̏��� [gate.h]
// Author : ���ꐶ
//
//=============================================================================
#ifndef _GATE_H_
#define _GATE_H_

#include "main.h"
#include "environment hub.h"

//=============================================================================
// �}�N����`
#define COMP4GATE		(6)			// ��̃Q�[�g�����̂ɕK�v�Ȑ�	components required for creation the gate 
#define GATE_MAX		(1 * COMP4GATE)

#define GATE_DATA_START	("##Gate")	// �Q�[�g�̃f�[�^�̎n�܂������

//=============================================================================
// �\���̐錾

typedef struct
{
	MESH		mesh;		// ���b�V���I�u�W�F�N�g�̊�{�\����
	bool		isHitbox;	// �����蔻��p���`��p��
}GATE;



//=============================================================================
// �v���g�^�C�v�錾
HRESULT InitGate(void);
void ResetGate(void);
void UninitGate(void);
void UpdateGate(void);
void DrawGate(void);
int hitCheckGate(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);

HRESULT MakeVertexGate(LPDIRECT3DDEVICE9 pDevice);
GATE *GetGate(int no);
#endif
