//=============================================================================
//
// UI�̏��� [UI.cpp]
// Author : ��� �ꐶ
//
//=============================================================================
#include "main.h"
#include "UI.h"
#include "player.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************


enum
{
	PLAYER_BAR,		// �v���C���|�̗̑͂Ɩ��͂�\������v���|�g
	PLAYER_HP_SHAD,	// �v���C���|�̗̑͂̃V���h�E
	PLAYER_HP,			// �v���C���|�̗̑�

	PLAYER_STAM,		// �v���C���|�̃X�^�~�i

	UI_MAX,				// UI�Ɏg����
};


// �v���C���|�̗̑͂ƃX�^�~�i
#define TEXTURE_BAR	("data/TEXTURE/health_bar_bg00.png")
#define TEXTURE_BAR_SIZE_X	(610 /2)
#define TEXTURE_BAR_SIZE_Y	(65 /2)

// �̗�
#define TEXTURE_HP	("data/TEXTURE/health_bar.png")
#define TEXTURE_HP_SIZE_X_MAX	(600 /2)
#define TEXTURE_HP_SIZE_Y		(35 /2)
// �̗͂̂��̎��̗ʂɉ����đ傫����ς���
#define TEXTURE_HP_SIZE_X ((TEXTURE_HP_SIZE_X_MAX/PLAYER_HP_MAX)*player->HP)
// �̗̓V���h�E
#define TEXTURE_HP_SHAD ("data/TEXTURE/health_bar_shadow.png")
// �傫��MAX�̗͑͂ƈꏏ
#define TEXTURE_HP_SHAD_SIZE_X ((TEXTURE_HP_SIZE_X_MAX/PLAYER_HP_MAX)*player->HPShadow)


// �X�^�~�i
#define TEXTURE_STAM	("data/TEXTURE/mana_bar.png")
#define TEXTURE_STAM_SIZE_X_MAX	(300)
#define TEXTURE_STAM_SIZE_Y		(18 /2)
// �X�^�~�i�̂��̎��̗ʂɉ����đ傫����ς���
#define TEXTURE_STAM_SIZE_X ((TEXTURE_STAM_SIZE_X_MAX/PLAYER_STAM_MAX)*player->Stam)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexUi(int no);
void SetVertexUi(int no);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTexBar		= NULL;		// �e�N�X�`���ւ̃|���S��
LPDIRECT3DTEXTURE9		g_pD3DTexHp			= NULL;		// ����
LPDIRECT3DTEXTURE9		g_pD3DTexHpShad		= NULL;		// ����
LPDIRECT3DTEXTURE9		g_pD3DTexStam		= NULL;		// ����


UI						uiWk[UI_MAX];					// UI�\����


//=============================================================================
// ����������
HRESULT InitUi(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	UI *ui = &uiWk[0];
	PLAYER *player = GetPlayer(0);

	// �e�N�X�`���̓ǂݍ���
	// �v���C���|�̗̑͂Ɩ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BAR,		&g_pD3DTexBar);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_HP,		&g_pD3DTexHp);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_HP_SHAD,	&g_pD3DTexHpShad);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_STAM,	&g_pD3DTexStam);

	// UI�̏���������
	for (int i = 0; i < UI_MAX; i++, ui++)
	{
		switch (i)
		{
		case PLAYER_BAR:// �v���C���|�̗̑͂Ɩ��͂̃v���|�g
			ui->use = true;									// �g�p
			ui->type = PLAYER_BAR;						// ��ނ̎w��
			ui->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���W�f�[�^��������
			ui->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ��]�f�[�^��������
			ui->size_x = TEXTURE_BAR_SIZE_X;		// �e�N�X�`���[�̑傫��
			ui->size_y = TEXTURE_BAR_SIZE_Y;		// ����

			ui->Texture = g_pD3DTexBar;			// �e�N�X�`�����
			MakeVertexUi(i);								// ���_���̍쐬
			break;
		case PLAYER_HP_SHAD:// �v���C���|�̗̑�
			ui->use = true;									// �g�p
			ui->type = PLAYER_HP_SHAD;					// ��ނ̎w��
			ui->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���W�f�[�^��������
			ui->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ��]�f�[�^��������
			ui->size_x = TEXTURE_HP_SIZE_X_MAX;		// �e�N�X�`���[�̑傫��
			ui->size_y = TEXTURE_HP_SIZE_Y;			// ����

			ui->Texture = g_pD3DTexHp;			// �e�N�X�`�����
			MakeVertexUi(i);								// ���_���̍쐬
			break;
		case PLAYER_HP:// �v���C���|�̗̑�
			ui->use = true;									// �g�p
			ui->type = PLAYER_HP;							// ��ނ̎w��
			ui->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���W�f�[�^��������
			ui->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ��]�f�[�^��������
			ui->size_x = TEXTURE_HP_SIZE_X_MAX;		// �e�N�X�`���[�̑傫��
			ui->size_y = TEXTURE_HP_SIZE_Y;			// ����

			ui->Texture = g_pD3DTexHp;			// �e�N�X�`�����
			MakeVertexUi(i);								// ���_���̍쐬
			break;
		case PLAYER_STAM:// �v���C���|�̖���
			ui->use = true;									// �g�p
			ui->type = PLAYER_STAM;							// ��ނ̎w��
			ui->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���W�f�[�^��������
			ui->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ��]�f�[�^��������
			ui->size_x = TEXTURE_STAM_SIZE_X_MAX;		// �e�N�X�`���[�̑傫��
			ui->size_y = TEXTURE_STAM_SIZE_Y;			// ����

			ui->Texture = g_pD3DTexStam;			// �e�N�X�`�����
			MakeVertexUi(i);								// ���_���̍쐬
			break;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitUi(void)
{
	UI *ui = &uiWk[0];

	for (int i = 0; i < UI_MAX; i++, ui++)
	{
		if (ui->Texture != NULL)
		{	// �e�N�X�`���̊J��
			ui->Texture->Release();
			ui->Texture = NULL;
		}
	}

	if (g_pD3DTexBar != NULL)
	{
		g_pD3DTexBar->Release();
		g_pD3DTexBar = NULL;
	}
	if (g_pD3DTexHp != NULL)
	{
		g_pD3DTexHp->Release();
		g_pD3DTexHp = NULL;
	}
	if (g_pD3DTexHpShad != NULL)
	{
		g_pD3DTexHpShad->Release();
		g_pD3DTexHpShad = NULL;
	}
	if (g_pD3DTexStam != NULL)
	{
		g_pD3DTexStam->Release();
		g_pD3DTexStam = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateUi(void)
{
	UI *ui = &uiWk[0];
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < UI_MAX; i++, ui++)
	{
		if (ui->use == true)			// �g�p���Ă����ԂȂ�X�V����
		{
			switch (ui->type)
			{
			case PLAYER_BAR:		// �v���C���|�̗̑͂Ɩ��͂�\������v���|�g
				ui->pos.x = 10 + TEXTURE_BAR_SIZE_X;
				ui->pos.y = 10 + TEXTURE_BAR_SIZE_Y;
				break;
			case PLAYER_HP_SHAD:	// �v���C���|�̗̑�
				ui->pos.x = 16 + TEXTURE_HP_SIZE_X;
				ui->pos.y = 14 + TEXTURE_HP_SIZE_Y;
				ui->size_x = TEXTURE_HP_SHAD_SIZE_X;
				break;
			case PLAYER_HP:			// �v���C���|�̗̑�
				ui->pos.x = 16 + TEXTURE_HP_SIZE_X;
				ui->pos.y = 14 + TEXTURE_HP_SIZE_Y;
				ui->size_x = TEXTURE_HP_SIZE_X;
				break;
			case PLAYER_STAM:			// �v���C���|�̖���
				ui->pos.x = 16 + TEXTURE_STAM_SIZE_X;
				ui->pos.y = 53 + TEXTURE_STAM_SIZE_Y;
				ui->size_x = TEXTURE_STAM_SIZE_X;
				break;
			}

			SetVertexUi(i);	// �ړ���̍��W�Œ��_��ݒ�
		}
	}


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawUi(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	UI *ui = &uiWk[0];

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < UI_MAX; i++, ui++)
	{
		if (ui->use == true)			// �g�p���Ă����ԂȂ�`�悷��
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, ui->Texture);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_UI, ui->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexUi(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	UI *ui = &uiWk[no];

	// ���_���W�̐ݒ�
	ui->vertexWk[0].vtx = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	ui->vertexWk[1].vtx = D3DXVECTOR3(100.0f + ui->size_x, 100.0f, 0.0f);
	ui->vertexWk[2].vtx = D3DXVECTOR3(100.0f, 100.0f + ui->size_y, 0.0f);
	ui->vertexWk[3].vtx = D3DXVECTOR3(100.0f + ui->size_x, 100.0f + ui->size_y, 0.0f);

	// rhw�̐ݒ�
	ui->vertexWk[0].rhw =
		ui->vertexWk[1].rhw =
		ui->vertexWk[2].rhw =
		ui->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	ui->vertexWk[0].diffuse = 
		ui->vertexWk[1].diffuse = 
		ui->vertexWk[2].diffuse = 
		ui->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	ui->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	ui->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	ui->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	ui->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}


//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexUi(int no)
{
	UI *ui = &uiWk[no];

	// ���_���W�̐ݒ�
	ui->vertexWk[0].vtx.x = ui->pos.x - ui->size_x;
	ui->vertexWk[0].vtx.y = ui->pos.y - ui->size_y;
	ui->vertexWk[0].vtx.z = 0.0f;

	ui->vertexWk[1].vtx.x = ui->pos.x + ui->size_x;
	ui->vertexWk[1].vtx.y = ui->pos.y - ui->size_y;
	ui->vertexWk[1].vtx.z = 0.0f;

	ui->vertexWk[2].vtx.x = ui->pos.x - ui->size_x;
	ui->vertexWk[2].vtx.y = ui->pos.y + ui->size_y;
	ui->vertexWk[2].vtx.z = 0.0f;

	ui->vertexWk[3].vtx.x = ui->pos.x + ui->size_x;
	ui->vertexWk[3].vtx.y = ui->pos.y + ui->size_y;
	ui->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// �u���b�g�擾�֐�
//=============================================================================
UI *GetUi(int no)
{
	return(&uiWk[no]);
}

