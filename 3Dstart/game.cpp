//=============================================================================
//
// �Q�[�����[�h�̏��� [game.cpp]
// Author : ���ꐶ
/*
�Q�[�����[�h�̏�Ԃ̎���������

�Q�[�����̂̏����̈ꊇ�Ǘ�

when g_mode is set to GAME_MODE exicute

main hub for when the game is active
*/
//=============================================================================
#include "main.h"
#include "game.h"
//===============================================
// �`����	character
#include "player.h"
#include "enemy.h"
#include "NPC.h"

//===============================================
// ��		environment
#include "environment hub.h"
#include "map.h"
#include "meshwater.h"
#include "skybox.h"
#include "light.h"
#include "shadow.h"
#include "gate.h"

//===============================================
// �A�C�e��
#include "gem.h"

//===============================================
// UI
#include "UI hub.h"

//===============================================
// ���̑�	etc
#include "camera.h"
#include "checkhit.h"
#include "status icon.h"
#include "effect.h"
#include "collision.h"




//=============================================================================
// �}�N����`

// ���b�V���t�B�[���h�̏����ݒ�
#define WATER_POS		(D3DXVECTOR3(0.0f, -100.0f, 0.0f))
#define WATER_ROT		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
#define WATER_B_CNT_X	(250)
#define WATER_B_CNT_Z	(250)
#define WATER_B_SCL_X	(75.0f)
#define WATER_B_SCL_Z	(75.0f)

//=============================================================================
// �v���g�^�C�v�錾

//=============================================================================
// �O���[�o���ϐ�

//=============================================================================
// �Q�[�����[�h�̏���������
HRESULT InitGame(int oldMode)
{
	switch (oldMode)
	{
	case MODE_NULL:
		// ���߂Ă̏������i�S���������j

		//=============================
		// �}�b�v�̏�����
		InitMap();
		InitMeshWater(WATER_POS, WATER_ROT, WATER_B_CNT_X, WATER_B_CNT_Z, WATER_B_SCL_X, WATER_B_SCL_Z);
		InitSkyb();
		//=============================
		// �I�u�W�F�N�g�̏�����
		InitEnvironment();
		//=============================
		// �G�l�~�[�̏�����
		InitEnemy();
		//=============================
		// �v���C���[�̏�����
		InitPlayer();
		//=============================
		// NPC�̏���������
		InitNpc();
		//=============================
		//	UI�̍ŐV����
		InitUiHub();
		//=============================
		// ���̑��̏�����
		InitLight();
		InitShadow();
		InitCamera(MODE_GAME);
		InitStatIco();
		InitEffect();
		break;

	case MODE_TITLE:
		// �^�C�g���ɂȂ�����������������

		//=============================
		// �I�u�W�F�N�g�̏�����
		ResetGate();
		//=============================
		// �G�l�~�[�̏�����
		InitEnemy();
		//=============================
		// �v���C���[�̏�����
		InitPlayer();
		//=============================
		// NPC�̏���������
		InitNpc();
		//=============================
		//	UI�̍ŐV����
		InitUiHub();

		InitStatIco();

		//=============================
		// �A�C�e���̈ʒu��������
		ResetGem();

		break;
	case MODE_RESULT:
		break;

	}
	return S_OK;
}

//=============================================================================
// �Q�[�����[�h�̏���������
void UninitGame(int mode)
{
	switch (mode)
	{
	case MODE_TITLE:
		// �^�C�g���Ɏg��Ȃ����̂������[�X

		//=============================
		// �v���C���[�̏I������
		UninitPlayer();
		//=============================
		// �G�l�~�[�̏I������
		UninitEnemy();
		//=============================
		// NPC�̏I������
		UninitNpc();
		//=============================
		// �X�e�[�^�X�A�C�R���̏I������
		UninitStatIco();
		//=============================
		// UI�̏I������
		UninitUiHub();
		break;

	case MODE_RESULT:
		//=============================
		// �}�b�v�̏I������
		UninitMap();
		UninitMeshWater();
		UninitSkyb();
		//=============================
		// �I�u�W�F�N�g�̏I������
		UninitEnvironment();
		//=============================
		// �V���h�E�̏I������
		UninitShadow();
		//=============================
		// �v���C���|�̏I������
		UninitPlayer();
		//=============================
		// �G�l�~�[�̏I������
		UninitEnemy();
		//=============================
		// NPC�̏I������
		UninitNpc();
		//=============================
		// �X�e�[�^�X�A�C�R���̏I������
		UninitStatIco();
		//=============================
		// UI�̏I������
		UninitUiHub();

		break;






	}
}

//=============================================================================
// �Q�[�����[�h�̍ŐV����
void UpdateGame(void)
{
	// ���̍ŐV����
	UpdateEnvironment();

	// �}�b�v�̍ŐV����
	UpdateMap();

	// �n�ʂ̍X�V����
	UpdateMeshWater();

	// �X�J�C�{�b�N�X�I������
	UpdateSkyb();

	// �V���h�E�̍ŐV����
	UpdateShadow();

	// �J�����̍ŐV����
	UpdateCamera();

	// ���f���̍ŐV����
	UpdatePlayer();
#ifdef _DEBUG
	// �R���W�����̍ŐV����
	UpdateCollision();
#endif
	// �G�l�~�[�̍ŐV����
	UpdateEnemy();

	// NPC�̍ŐV����
	UpdateNpc();

	// �u���b�g�̍ŐV����
	UpdateStatIco();

	// �����蔻��
	UpdateCheckhit();

	// UI�̍ŐV����
	UpdateUiHub();

	UpdateEffect();

}

//=============================================================================
// �Q�[�����[�h�̏���������
void DrawGame(void)
{

	// �}�b�v�̕`�揈��
	DrawMap();

	// �n�ʂ̕`�揈��
	DrawMeshWater();

	// �X�J�C�{�b�N�X�I������
	DrawSkyb();

	// �V���h�E�̕`�揈��
	DrawShadow();

	// �G�t�F�N�g�̕`�揈��
	DrawEffect();

	// ���̕`�揈��
	DrawEnvironment();

	// ���f���̕`��
	DrawPlayer();

	// �G�l�~�[�̕`��
	DrawEnemy();

	// NPC�̕`�揈��
	DrawNpc();

	// �X�e�[�^�X�A�C�R���̕`��
	DrawStatIco();

	// UI�̕`��
	DrawUiHub();

}
