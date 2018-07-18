//=============================================================================
//
// �v���C���[���� [player.h]
// Author : ���ꐶ
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//=============================================================================
// �}�N����`

#define POS_X_PLAYER	(-1900.0f)	// (-1900.0f)	�X�^�[�g
#define POS_Y_PLAYER	(700.0f)	// (700.0f)�@	��
#define POS_Z_PLAYER	(-1800.0f)	// (-1800.0f)	�ʒu

#define PLAYER_HP_MAX (5)			// �v���C���[�̗̑�
#define PLAYER_STAM_MAX (300)		// �v���C���[�̃X�^�~�i

#define PLAYER_MAX	(1)				// �v���C���|�̎g�p���Ă��鐔



//=============================================================================
// �\���̐錾

typedef struct
{
	bool					use;						// �g�p���Ă��邩
	bool					alive;						// �����Ă��邩

	// �}�b�v��̈ړ��Ȃ�
	D3DXVECTOR3				Pos;					// �ʒu					position
	D3DXVECTOR3				Rot;					// ����(��])			rotation
	D3DXVECTOR3				Scl;					// �傫��(�X�P�[��)		scale

	D3DXVECTOR3				Vel;					// �ړ���
	float					Grav;					// �d��

	// �e�N�X�`���[�E���f��
	LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffPlayer = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
	LPD3DXMESH				Mesh;			// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER			D3DXBuffMatPlayer;		// �}�e���A�����ւ̃|�C���^
	DWORD					NumMatPlayer;			// �}�e���A�����̐�
	D3DXMATRIX				mtxWorld;				// ���[���h�}�g���b�N�X

	// �e
	int						nIdxShadow;				// �eID
	float					fSizeShadow;			// �e�̃T�C�Y
	D3DXCOLOR				colShadow;				// �e�̐F


	//���̑�
	int						team;					// �G�������𔻒f����
	int						BullCD;					// �e���ˎ��܂ł̑҂�
	int						HP;						// �̗�
	float					HPShadow;				// �̗�
	float					Stam;					// �X�^�~�i
	int						StamTimer;				// �X�^�~�i�������Ȃ��Ă���g����悤�ɂȂ�܂ł̎���

	bool					Jump;					// �W�����v
	int						HitTime;				// ����������̖��G����
}PLAYER;



//=============================================================================
// �v���g�^�C�v�錾
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void PlayerHit(int no);
void DrawPlayer(void);
PLAYER *GetPlayer(int no);
#endif
