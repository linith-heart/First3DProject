//=============================================================================
//
// �X�J�C�{�b�N�X���� [skybox.h]
// Author : ���ꐶ
//
//=============================================================================
#ifndef _SKYB_H_
#define _SKYB_H_

#include "main.h"
#include "enemy.h"

//=============================================================================
// �}�N����`

#define SKYB_MAX		(1)


//=============================================================================
// �\���̐錾

typedef struct
{
	bool					use;				// �g���Ă邩

	LPDIRECT3DVERTEXBUFFER9 VtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^

	D3DXVECTOR3				Pos;				// �ʒu
	D3DXVECTOR3				Rot;				// ����(��])
	D3DXVECTOR3				Scl;				// �傫��(�X�P�[��)

	LPD3DXMESH				Mesh;				// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER			BuffMat;			// �}�e���A�����ւ̃|�C���^
	DWORD					NumMat;				// �}�e���A�����̐�

	int						TextID;				// �e�N�X�`���[�ԍ�

	D3DXMATRIX				mtxWorld;				// ���[���h�}�g���b�N�X

}SKYB;



//=============================================================================
// �v���g�^�C�v�錾
HRESULT InitSkyb(void);
void UninitSkyb(void);
void UpdateSkyb(void);
void DrawSkyb(void);
SKYB *GetSkyb(int no);
#endif
