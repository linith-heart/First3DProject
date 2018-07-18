//=============================================================================
//
// �W�F������ [gem.h]
// Author : ���ꐶ
//
//=============================================================================
#ifndef _GEM_H_
#define _GEM_H_

#include "main.h"
#include "environment hub.h"

//=============================================================================
// �}�N����`

#define GEM_MAX		(7)


//=============================================================================
// �\���̐錾

typedef struct
{
	MESH		mesh;		// ���b�V���I�u�W�F�N�g�̊�{�\����
}GEM;



//=============================================================================
// �v���g�^�C�v�錾
HRESULT InitGem(void);
void ResetGem(void);
void UninitGem(void);
void UpdateGem(void);
void DrawGem(void);
int hitCheckGem(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);

HRESULT MakeVertexGem(LPDIRECT3DDEVICE9 pDevice);
GEM *GetGem(int no);
#endif
