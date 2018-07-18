//=============================================================================
//
// UI�̃e�L�X�g�v�����v�g����[help_text.cpp]
// Author : ���ꐶ
//
/*
�w���v�e�L�X�g�͑�����@��ړI��\��
�e�L�X�g�v����Ղ�\������

help_text shown the controles and what to do
acts as a help prompt
*/
//=============================================================================
#include "help_text.h"
#include "input.h"
#include "fade.h"
#include "player.h"
#include "gem.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEX_PG000		"data/TEXTURE/controls000.png"			// �ǂݍ��ރe�N�X�`���t�@�C����
#define	PG000_WIDTH		(600)									// ����̕�
#define	PG000_HEIGHT	(400)									// ����̍���
#define	PG000_POS_X		(SCREEN_WIDTH / 2)						// ����̈ʒu(X���W)
#define	PG000_POS_Y		(SCREEN_HEIGHT * 0.5f)					// ����̈ʒu(Y���W)

#define	TEX_PG001		"data/TEXTURE/find_the_coin.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	PG001_WIDTH		(512)									// ����̕�
#define	PG001_HEIGHT	(256)									// ����̍���
#define	PG001_POS_X		(SCREEN_WIDTH / 2)						// ����̈ʒu(X���W)
#define	PG001_POS_Y		(SCREEN_HEIGHT * 0.25f)					// ����̈ʒu(Y���W)

#define	TEX_PG002		"data/TEXTURE/door_is_open.png"			// �ǂݍ��ރe�N�X�`���t�@�C����
#define	PG002_WIDTH		(640)									// ����̕�
#define	PG002_HEIGHT	(120)									// ����̍���
#define	PG002_POS_X		(SCREEN_WIDTH / 2)						// ����̈ʒu(X���W)
#define	PG002_POS_Y		(SCREEN_HEIGHT * 0.25f)					// ����̈ʒu(Y���W)

// not used 
#define	TEX_PG003		"data/TEXTURE/.png"						// �ǂݍ��ރe�N�X�`���t�@�C����
#define	PG003_WIDTH		(640)									// ����̕�
#define	PG003_HEIGHT	(120)									// ����̍���
#define	PG003_POS_X		(SCREEN_WIDTH / 2)						// ����̈ʒu(X���W)
#define	PG003_POS_Y		(SCREEN_HEIGHT * 0.75f)					// ����̈ʒu(Y���W)

#define	TEX_PG004		"data/TEXTURE/.png"						// �ǂݍ��ރe�N�X�`���t�@�C����
#define	PG004_WIDTH		(640)									// ����̕�
#define	PG004_HEIGHT	(120)									// ����̍���
#define	PG004_POS_X		(SCREEN_WIDTH / 2)						// ����̈ʒu(X���W)
#define	PG004_POS_Y		(SCREEN_HEIGHT * 0.75f)					// ����̈ʒu(Y���W)

#define	TEX_PG005		"data/TEXTURE/.png"						// �ǂݍ��ރe�N�X�`���t�@�C����
#define	PG005_WIDTH		(640)									// ����̕�
#define	PG005_HEIGHT	(120)									// ����̍���
#define	PG005_POS_X		(SCREEN_WIDTH / 2)						// ����̈ʒu(X���W)
#define	PG005_POS_Y		(SCREEN_HEIGHT * 0.75f)					// ����̈ʒu(Y���W)




#define TIME_TILL_HOW_TO	(1 * 60)	// �Q�[���J�n���瑀��������n�܂�܂ł̎���
#define START_LEASH			(300)		// 300 sq �X�^�[�g�n���瑀����@��\�����鋗��

#define	APPEAR_TIMER		(60)		// �o���܂ł̎���
#define DISPLAY_TIMER		(1.5 * 60)		// �\������Ă鎞��

#define ALPHA_START			(255.0f)
#define ALPHA_END			(0.0f)

#define	ALPHA_RATE			(0.05f)		// �A���t�@�l�̕ύX��

enum
{
	IdxPG000 = 0,
	IdxPG001,
	IdxPG002,
	IdxPG003,
	IdxPG004,
	IdxPG005,

	TEX_MAX,
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexHelpText(LPDIRECT3DDEVICE9 pDevice, int no);

void SetHelpTextPos(int no);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTexHelp[TEX_MAX] = { NULL,NULL };// �e�N�X�`���ւ̃|�C���^

HELP_TEXT				helptextWk[HELP_TEXT_MAX];			// UI�̃e�L�X�g�̃��[�N�ԍ�

int						game_timer = TIME_TILL_HOW_TO;

bool					allCollected = false;
//=============================================================================
// ����������
HRESULT InitHelpText(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HELP_TEXT *heltex = &helptextWk[0];

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEX_PG000, &D3DTexHelp[IdxPG000]);
	D3DXCreateTextureFromFile(pDevice, TEX_PG001, &D3DTexHelp[IdxPG001]);
	D3DXCreateTextureFromFile(pDevice, TEX_PG002, &D3DTexHelp[IdxPG002]);
	D3DXCreateTextureFromFile(pDevice, TEX_PG003, &D3DTexHelp[IdxPG003]);
	D3DXCreateTextureFromFile(pDevice, TEX_PG004, &D3DTexHelp[IdxPG004]);
	D3DXCreateTextureFromFile(pDevice, TEX_PG005, &D3DTexHelp[IdxPG005]);

	game_timer = TIME_TILL_HOW_TO;

	for (int i = 0; i < HELP_TEXT_MAX; i++, heltex++)
	{
		// �\�����Ȃ����Z�b�g
		heltex->use = false;

		// ������
		heltex->D3DVtxBuff	= NULL;
		heltex->AppearCnt	= 0;
		heltex->DisplayCnt	= 0;
		heltex->AlphaCnt	= 255.0f;
		heltex->TextId		= i;
		// �傫���ƈʒu���Z�b�g
		SetHelpTextPos(i);

		// ���_���̍쐬
		MakeVertexHelpText(pDevice, i);


	}

	return S_OK;
}

//=============================================================================
// UI�̃e�L�X�g�̃Z�b�g����
void SetHelpText(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HELP_TEXT *heltex = &helptextWk[no];

	// �\������ɕύX
	heltex->use = true;

	heltex->D3DVtxBuff = NULL;
	heltex->AppearCnt = APPEAR_TIMER;
	heltex->DisplayCnt = DISPLAY_TIMER;
	heltex->AlphaCnt = 0.0f;

	// ���_���̍쐬
	MakeVertexHelpText(pDevice, no);

}

//=============================================================================
// �I������
void UninitHelpText(void)
{
	for (int i = 0; i < HELP_TEXT_MAX; i++)
	{
		ReleaseHelpText(i);
	}
	for (int i = 0; i < TEX_MAX; i++)
		SAFE_RELEASE(D3DTexHelp[i]);
}

//=============================================================================
// ����̏I������
void ReleaseHelpText(int no)
{
	HELP_TEXT *heltex = &helptextWk[no];

	if (heltex->D3DVtxBuff != NULL)
	{// ���_�o�b�t�@�̊J��
		heltex->D3DVtxBuff->Release();
		heltex->D3DVtxBuff = NULL;
	}

}

//=============================================================================
// �X�V����
void UpdateHelpText(void)
{
	HELP_TEXT *heltex = &helptextWk[0];
	PLAYER *player = GetPlayer(0);
	GEM *gem = GetGem(0);

	// �X�^�[�g����(TIME_TILL_HOW_TO)�b��ɑ�����@��\��
	if (game_timer > 0)
		game_timer--;
	else if (game_timer == 0)
	{
		SetHelpText(0);
		game_timer -= 10;
	}

	// �}�b�v��̃W�F�����܂����邩�`�F�b�N
	int tCnt = 0;
	for (int i = 0; i < GEM_MAX; i++, gem++)
		if (gem->mesh.use)
			tCnt++;
	if (tCnt == 0)
		if (!allCollected)
		{
			SetHelpText(IdxPG002);
			allCollected = true;
		}

	for (int i = 0; i < HELP_TEXT_MAX; i++,heltex++)
		if (heltex->use == true)
		{
			if (heltex->AppearCnt > 0)
			{ // �A���t�@�x���グ��
				heltex->AlphaCnt += ALPHA_RATE;
				heltex->AppearCnt--;
				SetAlphaHelpText(i);
			}
			else if (heltex->DisplayCnt > 0)
			{ // (DisplayCnt)�b�\��
				if (heltex->TextId != 0) // ������@�̓{�^���������܂ŕ\��
					heltex->DisplayCnt--;
				else
				{
					D3DXVECTOR3 temp = player->Pos - (D3DXVECTOR3(POS_X_PLAYER, POS_Y_PLAYER, POS_Z_PLAYER));
					float tLen = D3DXVec3Length(&temp);
					if (tLen > START_LEASH)
						heltex->DisplayCnt = 0;

				}


				if (GetKeyboardPress(DIK_RETURN)
					|| IsButtonPressed(0, BUTTON_B))
				{
					heltex->DisplayCnt = 0;
				}
			}
			else if (heltex->AppearCnt > -APPEAR_TIMER)
			{ // �A���t�@�x��������
				heltex->AlphaCnt -= ALPHA_RATE;
				heltex->AppearCnt--;
				SetAlphaHelpText(i);
			}
			else
			{ // ���S�ɏ������烊���[�X
				heltex->use = false;
				ReleaseHelpText(i);
				// ����\���̌�ɐ����\��
				if (i == 0
					&& (i + 1) < HELP_TEXT_MAX)
				{
					SetHelpText(i + 1);
				}
			}
		}
	if (GetKeyboardTrigger(DIK_P))
	{
		SetHelpText(0);
	}
}

//=============================================================================
// �`�揈��
void DrawHelpText(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HELP_TEXT *heltex = &helptextWk[0];

	for (int i = 0; i < HELP_TEXT_MAX; i++, heltex++)
		if (heltex->use == true)
		{
			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, heltex->D3DVtxBuff, 0, sizeof(VERTEX_2D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, D3DTexHelp[heltex->TextId]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
		}
}

//=============================================================================
// ���_�̍쐬
HRESULT MakeVertexHelpText(LPDIRECT3DDEVICE9 pDevice, int no)
{
	HELP_TEXT *heltex = &helptextWk[no];
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&heltex->D3DVtxBuff,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		heltex->D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(heltex->Pos.x - (heltex->Size.x / 2), heltex->Pos.y - (heltex->Size.y / 2), 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(heltex->Pos.x + (heltex->Size.x / 2), heltex->Pos.y - (heltex->Size.y / 2), 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(heltex->Pos.x - (heltex->Size.x / 2), heltex->Pos.y + (heltex->Size.y / 2), 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(heltex->Pos.x + (heltex->Size.x / 2), heltex->Pos.y + (heltex->Size.y / 2), 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		heltex->D3DVtxBuff->Unlock();
	}

	return S_OK;
}

//=============================================================================
// �A���t�@�l�ύX�̏���
void SetAlphaHelpText(int no)
{
	HELP_TEXT *heltex = &helptextWk[no];
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		heltex->D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, heltex->AlphaCnt);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, heltex->AlphaCnt);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, heltex->AlphaCnt);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, heltex->AlphaCnt);

		// ���_�f�[�^���A�����b�N����
		heltex->D3DVtxBuff->Unlock();
	}

}


void SetHelpTextPos(int no)
{
	HELP_TEXT *heltex = &helptextWk[no];

	switch (no)
	{
	case IdxPG000:
		heltex->Pos = { PG000_POS_X,PG000_POS_Y };
		heltex->Size = { PG000_WIDTH,PG000_HEIGHT };
		break;
	case IdxPG001:
		heltex->Pos = { PG001_POS_X,PG001_POS_Y };
		heltex->Size = { PG001_WIDTH,PG001_HEIGHT };
		break;
	case IdxPG002:
		heltex->Pos = { PG002_POS_X,PG002_POS_Y };
		heltex->Size = { PG002_WIDTH,PG002_HEIGHT };
		break;
	}
}

//=============================================================================
// UI�̃e�L�X�g�̃Q�b�g�֐�
HELP_TEXT GetHelpText(int no)
{
	return(helptextWk[no]);
}