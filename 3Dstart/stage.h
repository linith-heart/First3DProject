//=============================================================================
//
// ��̏��� [stage.h]
// Author : ���ꐶ
//
//=============================================================================
#ifndef _STAGE_H_
#define _STAGE_H_

#include "main.h"
#include "environment hub.h"

//=============================================================================
// �}�N����`

#define COMP4STAGE		(2)				// ������̂ɕK�v�Ȑ�
#define STAGE_MAX		(3 * COMP4STAGE)

#define STAGE_DATA_START	("##Stage") // ��̃f�[�^�̎n�܂������

//=============================================================================
// �\���̐錾

typedef struct
{
	MESH		mesh;		// ���b�V���I�u�W�F�N�g�̊�{�\����
	bool		isHitbox;
}STAGE;



//=============================================================================
// �v���g�^�C�v�錾
HRESULT InitStage(void);
void UninitStage(void);
void UpdateStage(void);
void DrawStage(void);
int hitCheckStage(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);

HRESULT MakeVertexStage(LPDIRECT3DDEVICE9 pDevice);
STAGE *GetStage(int no);
#endif
