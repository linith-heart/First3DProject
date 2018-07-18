//=============================================================================
//
// �؏��� [tree.h]
// Author : ���ꐶ
//
//=============================================================================
#ifndef _TREE_H_
#define _TREE_H_

#include "main.h"
#include "environment hub.h"

//=============================================================================
// �}�N����`

#define TREE_MAX		(1)

#define TREE_DATA_START	("##Tree")	// �؂̃f�[�^�̎n�܂������

//=============================================================================
// �\���̐錾

typedef struct
{
	MESH		mesh;		// ���b�V���I�u�W�F�N�g�̊�{�\����
}TREE;



//=============================================================================
// �v���g�^�C�v�錾
HRESULT InitTree(void);
void UninitTree(void);
void UpdateTree(void);
void DrawTree(void);
int hitCheckTree(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);

HRESULT MakeVertexTree(LPDIRECT3DDEVICE9 pDevice);
TREE *GetTree(int no);
#endif
