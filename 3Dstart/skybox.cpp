//=============================================================================
//
// �X�J�C�{�b�N�X���� [skybox.cpp]
// Author : ���ꐶ
//
//=============================================================================
#include "camera.h"
#include "skybox.h"

//=============================================================================
// �}�N����`
#define	TEXTURE_SKYB000	"data/TEXTURE/skydome000.jpg"						// �ǂݍ��ރe�N�X�`���t�@�C����	name of the teture file

#define SKYB_SIZE		(8050.0f)											// �傫��
#define	ROT_VALUE		(D3DX_PI * 0.00005f)								// ��]��

#define	TITLE_POS_X		(3900.0f)						// �J�����̏����ʒu(X���W)
#define	TITLE_POS_Y		(710.0f)						// �J�����̏����ʒu(Y���W)
#define	TITLE_POS_Z		(775.0f)						// �J�����̏����ʒu(Z���W)

//=============================================================================
// �v���g�^�C�v�錾
void MoveSkyb(void);

//=============================================================================
// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		D3DTextureSkyb = NULL;	// �e�N�X�`���ւ̃|�C���^

SKYB						skybWk[SKYB_MAX];			// �t�B�[���h�ւ̃|�C���^

//=============================================================================
// ����������
HRESULT InitSkyb(void)
{
	SKYB *skyb = &skybWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MODE mode = GetMode();

	// �e�N�X�`���̐ݒ�
	D3DXCreateTextureFromFile(pDevice, TEXTURE_SKYB000, &D3DTextureSkyb);

	int i;
	for (i = 0; i < SKYB_MAX; i++, skyb++)
	{
		skyb->use = true;

		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		if (mode == MODE_TITLE)
		{
			skyb->Pos = D3DXVECTOR3(TITLE_POS_X, TITLE_POS_Y, TITLE_POS_Z);
		}
		else if (mode == MODE_GAME)
		{
			skyb->Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
		skyb->Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		skyb->Scl = D3DXVECTOR3(SKYB_SIZE, SKYB_SIZE, SKYB_SIZE);

		// ���f���֌W�̏�����
		skyb->Mesh = NULL;
		skyb->BuffMat = NULL;
		skyb->NumMat = 0;

		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX("data/MODEL/skydome000.x",			// ���f���f�[�^
			D3DXMESH_SYSTEMMEM,				// �g�p���郁�����̃I�v�V����
			pDevice,						// �f�o�C�X 
			NULL,							// ���g�p
			&skyb->BuffMat,					// �}�e���A���f�[�^�ւ̃|�C���^�[
			NULL,							// ���g�p
			&skyb->NumMat,					// D3DXMATERIAL�\���̂̐�
			&skyb->Mesh)))					// ���b�V���f�[�^�ւ̃|�C���^�[
		{
			return E_FAIL;
		}
	}
	return S_OK;

}

//=============================================================================
// �I������
void UninitSkyb(void)
{
	SKYB *skyb = &skybWk[0];

	if (D3DTextureSkyb != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureSkyb->Release();
		D3DTextureSkyb = NULL;
	}
	for (int i = 0; i < SKYB_MAX; i++, skyb++)
	{
		if (skyb->VtxBuff != NULL)
		{// ���_�o�b�t�@�̊J��
			skyb->VtxBuff->Release();
			skyb->VtxBuff = NULL;
		}

		if (skyb->Mesh != NULL)
		{// ���V���̉��
			skyb->Mesh->Release();
			skyb->Mesh = NULL;
		}
	}
}

//=============================================================================
// �X�V����
void UpdateSkyb(void)
{
	CAMERA *camera = GetCamera(0);
	SKYB *skyb = &skybWk[0];
	skyb->Pos = camera->At;
	skyb->Rot.y += ROT_VALUE;

	if (skyb->Rot.y >= D3DX_PI * 2)
	{
		skyb->Rot.y = D3DX_PI * 0;
	}
}

//=============================================================================
// �`�揈��
void DrawSkyb(void)
{
	SKYB *skyb = &skybWk[0];
	CAMERA *camera = GetCamera(0);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// �X�P�[���A��]�A���s�ړ�
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	int i, j;

	for (i = 0; i < SKYB_MAX; i++, skyb++)
		if (skyb->use == true)
		{
			// ���C�e�B���O�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			// ���[���h�}�g���N�X������
			D3DXMatrixIdentity(&skyb->mtxWorld);

			// �X�P�[���𔽉f	(S)
			D3DXMatrixScaling(&mtxScl, skyb->Scl.x, skyb->Scl.y, skyb->Scl.z);
			D3DXMatrixMultiply(&skyb->mtxWorld, &skyb->mtxWorld, &mtxScl); 	// &ene->mtxWorld = &ene->mtxWorld * &mtxScl
																			// ��]�𔽉f		(R)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, skyb->Rot.y, skyb->Rot.x, skyb->Rot.z);
			D3DXMatrixMultiply(&skyb->mtxWorld, &skyb->mtxWorld, &mtxRot);	// &ene->mtxWorld = &ene->mtxWorld * &mtxRot
																			// ���s�ړ��𔽉f	(T)
			D3DXMatrixTranslation(&mtxTranslate, skyb->Pos.x, skyb->Pos.y, skyb->Pos.z);
			D3DXMatrixMultiply(&skyb->mtxWorld, &skyb->mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &skyb->mtxWorld);

			// ���݂̃}�e���A����ۑ�
			pDevice->GetMaterial(&matDef);

			// �}�e���A�����ւ̃|�C���^��
			pD3DXMat = (D3DXMATERIAL*)skyb->BuffMat->GetBufferPointer();


			for (j = 0; j < (int)skyb->NumMat; j++)
			{
				pDevice->SetMaterial(&pD3DXMat[j].MatD3D);	// �}�e���A���̐ݒ�

				pDevice->SetTexture(0, D3DTextureSkyb);	// �e�N�X�`���̐ݒ�

				skyb->Mesh->DrawSubset(j);
			}

			// ���C�e�B���O��L���ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			// �}�e���A���ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
}

//=============================================================================
// �X�J�C�{�b�N�X�̈ړ�����
void MoveSkyb(void)
{
	SKYB *skyb = &skybWk[0];
}
//=============================================================================
// �X�J�C�{�b�N�X�̃Q�b�g�֐�
SKYB *GetSkyb(int no)
{
	return (&skybWk[no]);
}