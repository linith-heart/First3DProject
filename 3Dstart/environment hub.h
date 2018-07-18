//=============================================================================
//
// ������ [environment.h]
// Author : ���ꐶ
//
//=============================================================================
#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#include "main.h"





//=============================================================================
// �}�N����`

#define OBJECT_DATA_FILE	("data/FILE/object_data.csv")
#define END_OF_DATA			("##end")	// �f�[�^�̏I��������


enum // �t�@�C������f�[�^�ϊ�������̖̂��O
{	//	FtD = file to data
	FtD_USE = 0,

	FtD_TYPE,	// ���

	FtD_POS_X,	// �ʒu
	FtD_POS_Y,
	FtD_POS_Z,

	FtD_ROT_X,	// ��]
	FtD_ROT_Y,
	FtD_ROT_Z,

	FtD_SCL_X,	// �傫��
	FtD_SCL_Y,
	FtD_SCL_Z,
};

//=============================================================================
// �\���̐錾

// ���b�V���̍\����
typedef struct
{
	bool					use;					// �g�p���Ă��邩

	// �}�b�v��̈ʒu�Ȃ�
	D3DXVECTOR3				Pos;					// �ʒu
	D3DXVECTOR3				Rot;					// ����(��])
	D3DXVECTOR3				Scl;					// �傫��(�X�P�[��)

	// �e�N�X�`���[�E���f��
	LPDIRECT3DVERTEXBUFFER9	VtxBuff = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3				*VtxPos;				// ���_�̏��
	WORD					*IdxBuff;				// �C���f�b�N�X�̐�

	LPD3DXMESH				Mesh;					// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER			BuffMat;				// �}�e���A�����ւ̃|�C���^
	DWORD					NumMat;					// �}�e���A�����̐�

	D3DXMATRIX				mtxWorld;				// ���[���h�}�g���b�N�X
	int						TexId;					// �ǂ̃e�N�X�`���[���g����

	//// �e
	//int						nIdxShadow;				// �eID
	//float					fSizeShadow;			// �e�̃T�C�Y
	//D3DXCOLOR				colShadow;				// �e�̐F

	// ���̑�
	float					Grav;					// 
}MESH;


// �r���{�[�h�̍\����
typedef struct
{
	bool					use;					// �g�p���Ă��邩

	// �}�b�v��̈ʒu�Ȃ�
	D3DXVECTOR3				Pos;					// �ʒu
	D3DXVECTOR3				Rot;					// ����(��])
	D3DXVECTOR3				Scl;					// �傫��(�X�P�[��)
	D3DXMATRIX				mtxWorld;				// ���[���h�}�g���b�N�X

	// �e�N�X�`���[
	int						TexId;					// �e�N�X�`���[�ԍ�		texture id

	//	�e
	int						nIdxShadow;				// �eID
	float					fSizeShadow;			// �e�̃T�C�Y
	D3DXCOLOR				colShadow;				// �e�̐F
}BILL;


//=============================================================================
// �v���g�^�C�v�錾
HRESULT InitEnvironment(void);
void UninitEnvironment(void);
void UpdateEnvironment(void);
void DrawEnvironment(void);
BILL *GetBill(void);
MESH *GetMesh(void);

#endif
