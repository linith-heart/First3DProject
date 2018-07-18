//=============================================================================
//
// �G�l�~�[���� [enemy.h]
// Author : ���ꐶ
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

//=============================================================================
// �}�N����`

typedef enum
{
	CAR_TYPE = 0,
	SIGHT_TYPE,
	ENE_TYPE_MAX
}ENE_TYPE;


#define ENE_MAX	(1)			// �t�B�[���h�̏�̍ő吔

//=============================================================================
// �\���̐錾

typedef struct
{
	bool					use;					// �g�p���Ă��邩
	// �}�b�v��̈ړ��Ȃ�
	D3DXVECTOR3				Pos;					// �ʒu
	D3DXVECTOR3				Rot;					// ����(��])
	D3DXVECTOR3				Scl;					// �傫��(�X�P�[��)
	float					Grav;

	// �e�N�X�`���[�E���f��
	LPDIRECT3DVERTEXBUFFER9 VtxBuff = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
	LPD3DXMESH				Mesh;					// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER			BuffMat;				// �}�e���A�����ւ̃|�C���^
	DWORD					NumMat;					// �}�e���A�����̐�
	D3DXMATRIX				mtxWorld;				// ���[���h�}�g���b�N�X

	// �e
	int						nIdxShadow;				// �eID
	float					fSizeShadow;			// �e�̃T�C�Y
	D3DXCOLOR				colShadow;				// �e�̐F


	//���̑�
	int						team;					// �G�������𔻒f����


}ENE;



//=============================================================================
// �v���g�^�C�v�錾
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

ENE *GetEnemy(void);
#endif
