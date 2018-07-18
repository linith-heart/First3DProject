//=============================================================================
//
// ��̏��� [fence.h]
// Author : ���ꐶ
//
//=============================================================================
#ifndef _FENCE_H_
#define _FENCE_H_

#include "main.h"
#include "environment hub.h"

//=============================================================================
// �}�N����`

#define COMP4FENCE		(2)
#define FENCE_MAX		(6 * COMP4FENCE)

#define FENCE_DATA_START	("##Fence")	// ��̃f�[�^�̎n�܂������

//=============================================================================
// �\���̐錾

typedef struct
{
	MESH		mesh;		// ���b�V���I�u�W�F�N�g�̊�{�\����
	bool		isHitbox;
}FENCE;



//=============================================================================
// �v���g�^�C�v�錾
HRESULT InitFence(void);
void UninitFence(void);
void UpdateFence(void);
void DrawFence(void);
int hitCheckFence(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);

HRESULT MakeVertexFence(LPDIRECT3DDEVICE9 pDevice);
FENCE *GetFence(int no);
#endif
