//=============================================================================
//
// SCORE�̏��� [SCORE.cpp]
// Author : ��� �ꐶ
//
//=============================================================================
#include "main.h"
#include "score.h"
#include "gem.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEX_NUMBER		"data/TEXTURE/number000.png"
#define	TEX_SLASH		"data/TEXTURE/slash000.png"
#define TEX_NUM_PATTERN_X	(5)
#define TEX_NUM_PATTERN_Y	(2)
// NUMBER �Ɓ@SLASH�@�̕\���̑傫���͈ꏏ
#define	TEX_WIDTH		(50)						// ����̕�
#define	TEX_HEIGHT		(70)						// ����̍���

#define TEXT_COLOR		(D3DCOLOR_RGBA(255, 153, 48,255))

// �X���b�V���𒆐S�Ɍ��݂ƃZ�b�g�����ւƁA�E�ւƍ��W�Z�b�g
#define BASE_POS_X		((SCREEN_WIDTH / 2) + 100)
#define BASE_POS_Y		(SCREEN_HEIGHT * 0.07f)

#define	CUR_POS_X		(BASE_POS_X - TEX_WIDTH)
#define	CUR_POS_Y		(BASE_POS_Y)

#define	SLASH_POS_X		(BASE_POS_X)
#define	SLASH_POS_Y		(BASE_POS_Y)

#define SET_POS_X		(BASE_POS_X + TEX_WIDTH)
#define SET_POS_Y		(BASE_POS_Y)

enum
{
	Idx_NUM = 0,
	Idx_SLA,
	TEX_MAX,				// �X�R�A�ɂ��鐔
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexScore(int no);
void SetVertexScore(int no);
void SetTextureScore(int no, int cntPattern);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTexScore[TEX_MAX] = { NULL, NULL };		// �e�N�X�`���ւ̃|���S��


SCORE					scoreWk[SCORE_MAX];	// SCORE�\����


//=============================================================================
// ����������
HRESULT InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SCORE *score = &scoreWk[0];

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEX_NUMBER, &D3DTexScore[Idx_NUM]);
	D3DXCreateTextureFromFile(pDevice, TEX_SLASH, &D3DTexScore[Idx_SLA]);

	// SCORE�̏���������
	for (int i = 0; i < SCORE_MAX; i++, score++)
	{
		switch (i)
		{
		case SCORE_CUR:// ���݂̃W�F���̐�
			score->use = true;								// �g�p
			score->TexId = Idx_NUM;							// �e�N�X�`���̎w��
			score->Pos = { CUR_POS_X , CUR_POS_Y};			// ���W�f�[�^��������
			score->Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ��]�f�[�^��������
			score->Size = {TEX_WIDTH, TEX_HEIGHT};			// �e�N�X�`���[�̑傫��
			score->Val = 0;

			MakeVertexScore(i);								// ���_���̍쐬
			break;
		case SCORE_SLASH:// �X���b�V��
			score->use = true;								// �g�p
			score->TexId = Idx_SLA;							// �e�N�X�`���̎w��
			score->Pos = { SLASH_POS_X , SLASH_POS_Y };		// ���W�f�[�^��������
			score->Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ��]�f�[�^��������
			score->Size = { TEX_WIDTH, TEX_HEIGHT };		// �e�N�X�`���[�̑傫��
			score->Val = 0;

			MakeVertexScore(i);								// ���_���̍쐬
			break;
		case SCORE_SET:// �}�b�v��̃W�F���̐�
			score->use = true;								// �g�p
			score->TexId = Idx_NUM;							// �e�N�X�`���̎w��
			score->Pos = { SET_POS_X , SET_POS_Y };			// ���W�f�[�^��������
			score->Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ��]�f�[�^��������
			score->Size = { TEX_WIDTH, TEX_HEIGHT };		// �e�N�X�`���[�̑傫��
			score->Val = GEM_MAX;

			MakeVertexScore(i);								// ���_���̍쐬
			break;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
void UninitScore(void)
{
	for (int i = 0; i < TEX_MAX; i++)
	{
		SAFE_RELEASE (D3DTexScore[i]);
	}
}

//=============================================================================
// �X�V����
void UpdateScore(void)
{
	SCORE *score = &scoreWk[0];
	GEM *gem = GetGem(0);

	int j, tCnt = 0;

	for (int i = 0; i < SCORE_MAX; i++, score++)
	{
		if (score->use == true)			// �g�p���Ă����ԂȂ�X�V����
		{
			switch (i)
			{
			case SCORE_CUR:
				for (j = 0; j < GEM_MAX; j++, gem++)
					if (!gem->mesh.use)
						tCnt++;

				score->Val = tCnt;

				SetTextureScore(i, score->Val);
				break;
			case SCORE_SLASH:

				break;
			case SCORE_SET:
				SetTextureScore(i, score->Val);
				break;
			}

		}
	}


}

//=============================================================================
// �`�揈��
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SCORE *score = &scoreWk[0];

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < SCORE_MAX; i++, score++)
	{
		if (score->use == true)			// �g�p���Ă����ԂȂ�`�悷��
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, D3DTexScore[score->TexId]);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_SCORE, score->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
HRESULT MakeVertexScore(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SCORE *score = &scoreWk[no];

	// ���_���W�̐ݒ�
	score->vertexWk[0].vtx = D3DXVECTOR3(score->Pos.x - (score->Size.x / 2), score->Pos.y - (score->Size.y / 2), 0.0f);
	score->vertexWk[1].vtx = D3DXVECTOR3(score->Pos.x + (score->Size.x / 2), score->Pos.y - (score->Size.y / 2), 0.0f);
	score->vertexWk[2].vtx = D3DXVECTOR3(score->Pos.x - (score->Size.x / 2), score->Pos.y + (score->Size.y / 2), 0.0f);
	score->vertexWk[3].vtx = D3DXVECTOR3(score->Pos.x + (score->Size.x / 2), score->Pos.y + (score->Size.y / 2), 0.0f);

	// rhw�̐ݒ�
	score->vertexWk[0].rhw =
		score->vertexWk[1].rhw =
		score->vertexWk[2].rhw =
		score->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	score->vertexWk[0].diffuse = TEXT_COLOR;
	score->vertexWk[1].diffuse = TEXT_COLOR;
	score->vertexWk[2].diffuse = TEXT_COLOR;
	score->vertexWk[3].diffuse = TEXT_COLOR;

	// �e�N�X�`�����W�̐ݒ�
	score->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	score->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	score->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	score->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
void SetTextureScore(int no, int cntPattern)
{
	SCORE *score = &scoreWk[no];

	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEX_NUM_PATTERN_X;
	int y = cntPattern / TEX_NUM_PATTERN_X;
	float sizeX = 1.0f / TEX_NUM_PATTERN_X;
	float sizeY = 1.0f / TEX_NUM_PATTERN_Y;
	score->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	score->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	score->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	score->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// ���_���W�̐ݒ�
void SetVertexScore(int no)
{
	SCORE *score = &scoreWk[no];

	// ���_���W�̐ݒ�
	score->vertexWk[0].vtx.x = score->Pos.x - score->Size.x;
	score->vertexWk[0].vtx.y = score->Pos.y - score->Size.y;
	score->vertexWk[0].vtx.z = 0.0f;

	score->vertexWk[1].vtx.x = score->Pos.x + score->Size.x;
	score->vertexWk[1].vtx.y = score->Pos.y - score->Size.y;
	score->vertexWk[1].vtx.z = 0.0f;

	score->vertexWk[2].vtx.x = score->Pos.x - score->Size.x;
	score->vertexWk[2].vtx.y = score->Pos.y + score->Size.y;
	score->vertexWk[2].vtx.z = 0.0f;

	score->vertexWk[3].vtx.x = score->Pos.x + score->Size.x;
	score->vertexWk[3].vtx.y = score->Pos.y + score->Size.y;
	score->vertexWk[3].vtx.z = 0.0f;
}
