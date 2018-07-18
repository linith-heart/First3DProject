//=============================================================================
//
// ���U���g��ʏ��� [result.cpp]
// Author : ���ꐶ
//
//=============================================================================
#include "result.h"
#include "input.h"
#include "fade.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_RESULT		"data/TEXTURE/bg000.jpg"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_EXIT		"data/TEXTURE/exit.png"			// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_START		"data/TEXTURE/start.png"		// �ǂݍ��ރe�N�X�`���t�@�C����

#define	RESULT_EXIT_WIDTH		(640)		// ���U���g���S�̕�
#define	RESULT_EXIT_HEIGHT		(120)		// ���U���g���S�̍���
#define	RESULT_EXIT_POS_X		(SCREEN_WIDTH / 2 - RESULT_EXIT_WIDTH / 2)		// ���U���g���S�̈ʒu(X���W)
#define	RESULT_EXIT_POS_Y		(SCREEN_HEIGHT * 0.75f)		// ���U���g���S�̈ʒu(Y���W)

#define	START_WIDTH				(480)		// �X�^�[�g�{�^���̕�
#define	START_HEIGHT			(120)		// �X�^�[�g�{�^���̍���
#define	START_POS_X				(SCREEN_WIDTH / 2 - START_WIDTH /2)		// �X�^�[�g�{�^���̈ʒu(X���W)
#define	START_POS_Y				(SCREEN_HEIGHT * 0.5f)		// �X�^�[�g�{�^���̈ʒu(Y���W)

#define	COUNT_APPERA_START		(60)		// �X�^�[�g�{�^���o���܂ł̎���
#define	INTERVAL_DISP_START		(60)		// �X�^�[�g�{�^���_�ł̎���

#define	COUNT_WAIT_DEMO			(60 * 5)	// �f���܂ł̑҂�����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexResult(LPDIRECT3DDEVICE9 pDevice);
void SetColorResultLogo(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureResult = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffResult = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pD3DTextureBg = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffBg = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pD3DTextureReturn = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffReturn = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
int						g_nCountAppearReturn = 0;		//
float					g_fAlphaLogoResult = 0.0f;			//
int						g_nCountDispResult = 0;				//
bool					g_bDispResult = false;			//
int						g_nConutDemoResult = 0;				//

//=============================================================================
// ����������
//=============================================================================
HRESULT InitResult(int oldMode)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_nCountAppearReturn = 0;
	g_fAlphaLogoResult = 0.0f;
	g_nCountDispResult = 0;
	g_bDispResult = false;
	g_nConutDemoResult = 0;

	// ���_���̍쐬
	MakeVertexResult(pDevice);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
		TEXTURE_RESULT,				// �t�@�C���̖��O
		&g_pD3DTextureBg);		// �ǂݍ��ރ������[

	D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
		TEXTURE_EXIT,				// �t�@�C���̖��O
		&g_pD3DTextureResult);		// �ǂݍ��ރ������[


	D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
		TEXTURE_START,				// �t�@�C���̖��O
		&g_pD3DTextureReturn);		// �ǂݍ��ރ������[

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitResult(int oldMode)
{
	if (g_pD3DTextureBg != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureBg->Release();
		g_pD3DTextureBg = NULL;
	}

	if (g_pD3DVtxBuffBg != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffBg->Release();
		g_pD3DVtxBuffBg = NULL;
	}

	if (g_pD3DTextureResult != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureResult->Release();
		g_pD3DTextureResult = NULL;
	}

	if (g_pD3DVtxBuffResult != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffResult->Release();
		g_pD3DVtxBuffResult = NULL;
	}

	if (g_pD3DTextureReturn != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureReturn->Release();
		g_pD3DTextureReturn = NULL;
	}

	if (g_pD3DVtxBuffReturn != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffReturn->Release();
		g_pD3DVtxBuffReturn = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateResult(void)
{
#if 0
	if (g_nCountAppearStart >= COUNT_APPERA_START)
	{
		g_nConutDemo++;
		if (g_nConutDemo > COUNT_WAIT_DEMO)
		{
			SetFade(FADE_OUT, MODE_RESULT);
		}
	}
#endif

	if (g_fAlphaLogoResult < 1.0f)
	{
		g_fAlphaLogoResult += 0.005f;
		if (g_fAlphaLogoResult >= 1.0f)
		{
			g_fAlphaLogoResult = 1.0f;
		}
		SetColorResultLogo();
	}
	else
	{
		g_nCountAppearReturn++;
		if (g_nCountAppearReturn > COUNT_APPERA_START)
		{
			g_nCountDispResult = (g_nCountDispResult + 1) % 80;
			if (g_nCountDispResult > INTERVAL_DISP_START)
			{
				g_bDispResult = false;
			}
			else
			{
				g_bDispResult = true;
			}
		}
	}

	if (GetKeyboardTrigger(DIK_RETURN))
	{
		if (g_nCountAppearReturn == 0)
		{// ���U���g�o��X�L�b�v
			g_fAlphaLogoResult = 1.0f;
			SetColorResultLogo();

			g_nCountAppearReturn = COUNT_APPERA_START;
		}
		else
		{// �Q�[����
			SetFade(FADE_OUT);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pD3DVtxBuffBg, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureBg);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);


	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pD3DVtxBuffResult, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureResult);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	if (g_bDispResult == true)
	{
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_pD3DVtxBuffResult, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureResult);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexResult(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffBg,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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
		g_pD3DVtxBuffBg->Unlock();
	}

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffResult,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(RESULT_EXIT_POS_X, RESULT_EXIT_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(RESULT_EXIT_POS_X + RESULT_EXIT_WIDTH, RESULT_EXIT_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(RESULT_EXIT_POS_X, RESULT_EXIT_POS_Y + RESULT_EXIT_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(RESULT_EXIT_POS_X + RESULT_EXIT_WIDTH, RESULT_EXIT_POS_Y + RESULT_EXIT_HEIGHT, 0.0f);

		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogoResult);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogoResult);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogoResult);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogoResult);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffResult->Unlock();
	}


	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffReturn,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffReturn->Lock(0, 0, (void**)&pVtx, 0);

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
		g_pD3DVtxBuffReturn->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
void SetColorResultLogo(void)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffReturn->Lock(0, 0, (void**)&pVtx, 0);

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogoResult);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogoResult);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogoResult);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogoResult);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffReturn->Unlock();
	}

}