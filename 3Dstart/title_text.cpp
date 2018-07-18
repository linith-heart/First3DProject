//=============================================================================
//
// �^�C�g����ʂ̃e�L�X�g�̏��� [title_text.cpp]
// Author : ���ꐶ
//
/*
�^�C�g���Ń��S�̏���
�X�^�[�g�ŃQ�[�����Ŏg��Ȃ����̂������[�X

title _text.cpp is for logo
releace thing not used in game when start is selected
*/
//=============================================================================
#include "title_text.h"
#include "input.h"
#include "fade.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define	TEXT_TITLE				"data/TEXTURE/title_logo.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXT_EXIT				"data/TEXTURE/exit.png"				// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXT_START				"data/TEXTURE/start.png"			// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXT_POINTER			"data/TEXTURE/pointer.png"			// �ǂݍ��ރe�N�X�`���t�@�C����

#define	TITLE_WIDTH				(960)		// �^�C�g�����S�̕�
#define	TITLE_HEIGHT			(270)		// �^�C�g�����S�̍���
#define	TITLE_POS_X				(SCREEN_WIDTH / 2 - TITLE_WIDTH / 2)		// �^�C�g�����S�̈ʒu(X���W)
#define	TITLE_POS_Y				(0.0f)		// �^�C�g�����S�̈ʒu(Y���W)

#define	EXIT_WIDTH				(640)		// �I���{�^���̕�
#define	EXIT_HEIGHT				(120)		// �I���{�^���̍���
#define	EXIT_POS_X				(SCREEN_WIDTH / 2 - EXIT_WIDTH / 2)		// �I���{�^���̈ʒu(X���W)
#define	EXIT_POS_Y				(SCREEN_HEIGHT * 0.75f)		// �I���{�^���̈ʒu(Y���W)

#define	START_WIDTH				(480)		// �X�^�[�g�{�^���̕�
#define	START_HEIGHT			(120)		// �X�^�[�g�{�^���̍���
#define	START_POS_X				(SCREEN_WIDTH / 2 - START_WIDTH /2)		// �X�^�[�g�{�^���̈ʒu(X���W)
#define	START_POS_Y				(SCREEN_HEIGHT * 0.5f)		// �X�^�[�g�{�^���̈ʒu(Y���W)

#define POINTER_WIDTH			(120)		// �J�[�\���̕�
#define POINTER_HEIGHT			(120)		// �J�[�\���̍���

#define	COUNT_APPERA_START		(60)		// �X�^�[�g�{�^���o���܂ł̎���
#define ALPHA_RATE				(0.05)		// �����x�̕ϊ����x



enum
{
	TITLE_SELECT_START = 0,
	TITLE_SELECT_EXIT,
	TITLE_SELECT_MAX,
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexTitle(LPDIRECT3DDEVICE9 pDevice);
void DrawTitlePoly(LPDIRECT3DVERTEXBUFFER9 vtxbuff, LPDIRECT3DTEXTURE9 texture);
void SetColorTitleLogo(LPDIRECT3DVERTEXBUFFER9 vtxbuff, float Alpha);
void SetTitleVertexPos(void);


//*****************************************************************************
// �O���[�o���錾
//*****************************************************************************

LPDIRECT3DTEXTURE9		g_pD3DTextureTitle = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffTitle = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

LPDIRECT3DTEXTURE9		g_pD3DTextureExit = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffExit = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

LPDIRECT3DTEXTURE9		g_pD3DTextureStart = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffStart = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

LPDIRECT3DTEXTURE9		g_pD3DTexturePointer = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffPointer = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

int						g_Selection = 0;
int						g_TimeTillDisplay = 0;			// �\���܂ł̎���
float					g_Alpha = 0.0f;					// �e�N�X�`���[�̃A���t�@�x
bool					g_isFadeIn = true;				// �����x���グ�邩�����邩���f����t���O

float					g_PointerPosX = 0.0f;			// �J�[�\����X���W�̈ʒu
float					g_PointerPosY = 0.0f;			// �J�[�\����Y���W�̈ʒu

//=============================================================================
// �^�C�g����ʂ̃e�L�X�g�̏���������
HRESULT InitTitleText(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_TimeTillDisplay = COUNT_APPERA_START;
	g_Alpha = 0.0f;
	g_isFadeIn = true;


	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXT_TITLE, &g_pD3DTextureTitle);

	D3DXCreateTextureFromFile(pDevice, TEXT_EXIT, &g_pD3DTextureExit);

	D3DXCreateTextureFromFile(pDevice, TEXT_START, &g_pD3DTextureStart);

	D3DXCreateTextureFromFile(pDevice, TEXT_POINTER, &g_pD3DTexturePointer);

	// ���_���̍쐬
	MakeVertexTitle(pDevice);

	return S_OK;

}

//=============================================================================
// �^�C�g����ʂ̃e�L�X�g�̏I������
void UninitTitleText(void)
{
	if (g_pD3DTextureTitle != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureTitle->Release();
		g_pD3DTextureTitle = NULL;
	}

	if (g_pD3DVtxBuffTitle != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffTitle->Release();
		g_pD3DVtxBuffTitle = NULL;
	}

	if (g_pD3DTextureExit != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureExit->Release();
		g_pD3DTextureExit = NULL;
	}

	if (g_pD3DVtxBuffExit != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffExit->Release();
		g_pD3DVtxBuffExit = NULL;
	}

	if (g_pD3DTextureStart != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureStart->Release();
		g_pD3DTextureStart = NULL;
	}

	if (g_pD3DVtxBuffStart != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffStart->Release();
		g_pD3DVtxBuffStart = NULL;
	}

}

//=============================================================================
// �^�C�g����ʂ̃e�L�X�g�̍ŐV����
void UpdateTitleText(void)
{
	// �����x�̕ϊ�����
	if (g_isFadeIn)
	{
		g_Alpha += ALPHA_RATE;
		if (g_Alpha >= 1.0f)
			g_isFadeIn = false;
	}
	else if (!g_isFadeIn)
	{
		g_Alpha -= ALPHA_RATE;
		if (g_Alpha <= 0.0f)
			g_isFadeIn = true;
	}

	switch (g_Selection)
	{
	case TITLE_SELECT_START:
		SetColorTitleLogo(g_pD3DVtxBuffStart, g_Alpha);
		SetColorTitleLogo(g_pD3DVtxBuffExit, 1.0f);
		g_PointerPosX = START_POS_X - (START_WIDTH / 2) - POINTER_WIDTH;
		g_PointerPosY = START_POS_Y - (START_HEIGHT / 2) - POINTER_HEIGHT;
		break;

	case TITLE_SELECT_EXIT:
		SetColorTitleLogo(g_pD3DVtxBuffStart, 1.0f);
		SetColorTitleLogo(g_pD3DVtxBuffExit, g_Alpha);
		g_PointerPosX = EXIT_POS_X - (START_WIDTH / 2) - POINTER_WIDTH;
		g_PointerPosY = EXIT_POS_Y - (START_HEIGHT / 2) - POINTER_HEIGHT;
		break;
	}

	// �|�C���^�[�̈ʒu���Z�b�g
	void SetTitleVertexPos();


	// �|�C���^�[�̏���
	if (GetKeyboardTrigger(DIK_W))
	{
		g_Selection = (g_Selection + 1) % TITLE_SELECT_MAX;
		g_Alpha = 1.0f;
		g_isFadeIn = false;
	}
	
	if (GetKeyboardTrigger(DIK_S))
	{
		g_Selection --;
		if (g_Selection < 0)
			g_Selection = TITLE_SELECT_MAX - 1;
		g_Alpha = 1.0f;
		g_isFadeIn = false;
	}


	if (GetKeyboardTrigger(DIK_RETURN)
		|| IsButtonTriggered(0, BUTTON_A)
		|| IsButtonTriggered(0, BUTTON_START))
	{
		// �Q�[����
		if (g_Selection == TITLE_SELECT_START)
			SetFade(FADE_OUT);
		else if (g_Selection == TITLE_SELECT_EXIT)
			SetMode(MODE_UNINIT,0);


	}
}

//=============================================================================
// �^�C�g����ʂ̃e�L�X�g�̕`�揈��
void DrawTitleText(void)
{
	// �^�C�g���̕`��
	DrawTitlePoly(g_pD3DVtxBuffTitle, g_pD3DTextureTitle);
	// �G�O�W�b�g�{�^���̕`��
	DrawTitlePoly(g_pD3DVtxBuffExit, g_pD3DTextureExit);
	// �X�^�[�g�{�^���̕`��
	DrawTitlePoly(g_pD3DVtxBuffStart, g_pD3DTextureStart);
	// �J�[�\���̕`��
	//DrawTitlePoly(g_pD3DVtxBuffPointer, g_pD3DTexturePointer);

}

void DrawTitlePoly(LPDIRECT3DVERTEXBUFFER9 vtxbuff, LPDIRECT3DTEXTURE9 texture)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, vtxbuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, texture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

}


//=============================================================================
// ���_�̍쐬
HRESULT MakeVertexTitle(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffTitle,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�  TITLE_POS_Y
		pVtx[0].vtx = D3DXVECTOR3(TITLE_POS_X, TITLE_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(TITLE_POS_X + TITLE_WIDTH, TITLE_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(TITLE_POS_X, TITLE_POS_Y + TITLE_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(TITLE_POS_X + TITLE_WIDTH, TITLE_POS_Y + TITLE_HEIGHT, 0.0f);

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
		g_pD3DVtxBuffTitle->Unlock();
	}

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffExit,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffExit->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(EXIT_POS_X, EXIT_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(EXIT_POS_X + EXIT_WIDTH, EXIT_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(EXIT_POS_X, EXIT_POS_Y + EXIT_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(EXIT_POS_X + EXIT_WIDTH, EXIT_POS_Y + EXIT_HEIGHT, 0.0f);

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
		g_pD3DVtxBuffExit->Unlock();
	}


	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffStart,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffStart->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(START_POS_X, START_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(START_POS_X + START_WIDTH, START_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(START_POS_X, START_POS_Y + START_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(START_POS_X + START_WIDTH, START_POS_Y + START_HEIGHT, 0.0f);

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
		g_pD3DVtxBuffStart->Unlock();
	}


	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffPointer,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffPointer->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(g_PointerPosX, g_PointerPosY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(g_PointerPosX + POINTER_WIDTH, g_PointerPosY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(g_PointerPosX, g_PointerPosY + POINTER_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(g_PointerPosX + POINTER_WIDTH, g_PointerPosY + POINTER_HEIGHT, 0.0f);

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
		g_pD3DVtxBuffPointer->Unlock();
	}

	return S_OK;

}

//=============================================================================
// ���_�̍쐬
void SetColorTitleLogo(LPDIRECT3DVERTEXBUFFER9 vtxbuff, float Alpha)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		vtxbuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, Alpha);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, Alpha);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, Alpha);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, Alpha);

		// ���_�f�[�^���A�����b�N����
		vtxbuff->Unlock();
	}

}

void SetTitleVertexPos(void)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffPointer->Lock(0, 0, (void**)&pVtx, 0);

		// ���ˌ��̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(g_PointerPosX, g_PointerPosY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(g_PointerPosX + POINTER_WIDTH, g_PointerPosY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(g_PointerPosX, g_PointerPosY + POINTER_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(g_PointerPosX + POINTER_WIDTH, g_PointerPosY + POINTER_HEIGHT, 0.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffPointer->Unlock();
	}

}