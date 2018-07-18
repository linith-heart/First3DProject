//=============================================================================
//
// �W�F���̏��� [gem.cpp]
// Author : ���ꐶ
//
//=============================================================================
#include "gem.h"

#include "player.h"

#include "checkhit.h"

#include "effect.h"
/*
���V���̃W�F���̏���
���݂����Ȗ�
�Q�[�����ŁQ�Q�W�߂�Ύ��ւƐi�߂�


mesh gem
acts like a key 
door to next level will open if all gems are collected 
*/

//=============================================================================
// �}�N����`
#define TEXTURE_GEM		("data/TEXTURE/gem000.png")	// �W�F���̃e�N�X�`���[
#define MODEL			("data/MODEL/gem000.x")		// �W�F���̃��f��
#define	VALUE_MOVE		(0.5f)						// �ړ���
#define	ROT_VALUE		(D3DX_PI * 0.02f)			// ��]��
#define SCL_VALUE		(0.5f)						// �傫��
#define RANGE_VALUE		(50)						// ������̋���

#define SPARKLE_DISTANCE	(50)					// �p�[�e�B�N���̋���

//=============================================================================
// �v���g�^�C�v�錾
void MoveGem(int no);

//=============================================================================
// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		D3DTextureGem = NULL;		// �e�N�X�`���ւ̃|�C���^

GEM						gemWk[GEM_MAX];			// �t�B�[���h�ւ̃|�C���^

int						SparkCnt = 0;
//=============================================================================
// ����������
HRESULT InitGem(void)
{
	GEM *gem = &gemWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int i;
	for (i = 0; i < GEM_MAX; i++, gem++)
	{
		gem->mesh.use = true;

		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		switch (i)
		{
		case 0:
			gem->mesh.Pos = D3DXVECTOR3(220.0f, 460.0f, -2300.0f);
			break;
		case 1:
			gem->mesh.Pos = D3DXVECTOR3(70.0f, 320.0f, 2500.0f);
			break;
		case 2:
			gem->mesh.Pos = D3DXVECTOR3(1960.0f, 0.0f, 360.0f);
			break;
		case 3:
			gem->mesh.Pos = D3DXVECTOR3(1650.0f, 0.0f, 2140.0f);
			break;
		}
		gem->mesh.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		gem->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);

		// ���f���֌W�̏�����
		D3DXCreateTextureFromFile(pDevice, TEXTURE_GEM, &D3DTextureGem);
		gem->mesh.VtxBuff = NULL;
		//MakeVertexGem(pDevice);
		gem->mesh.Mesh = NULL;
		gem->mesh.BuffMat = NULL;
		gem->mesh.NumMat = 0;

		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(MODEL,			// ���f���f�[�^
			D3DXMESH_SYSTEMMEM,					// �g�p���郁�����̃I�v�V����
			pDevice,							// �f�o�C�X 
			NULL,								// ���g�p
			&gem->mesh.BuffMat,				// �}�e���A���f�[�^�ւ̃|�C���^�[
			NULL,								// ���g�p
			&gem->mesh.NumMat,				// D3DXMATERIAL�\���̂̐�
			&gem->mesh.Mesh)))				// ���b�V���f�[�^�ւ̃|�C���^�[
		{
			return E_FAIL;
		}
	}
}

//=============================================================================
// �W�F���̃��Z�b�g����
void ResetGem(void)
{
	GEM *gem = &gemWk[0];
	int i;
	for (i = 0; i < GEM_MAX; i++, gem++)
	{
		gem->mesh.use = true;

		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		switch (i)
		{
		case 0:
			gem->mesh.Pos = D3DXVECTOR3(270.0f, 660.0f, -2020.0f);
			break;
		case 1:
			gem->mesh.Pos = D3DXVECTOR3(695.0f, 290.0f, 1675.0f);
			break;
		case 2:
			gem->mesh.Pos = D3DXVECTOR3(1960.0f, 0.0f, 360.0f);
			break;
		case 3:
			gem->mesh.Pos = D3DXVECTOR3(1650.0f, 0.0f, 2140.0f);
			break;
		case 4:
			gem->mesh.Pos = D3DXVECTOR3(-1495.0f, 725.0f, -2240.0f);
			break;
		case 5:
			gem->mesh.Pos = D3DXVECTOR3(140.0f, 50.0f, -150.0f);
			break;
		case 6:
			gem->mesh.Pos = D3DXVECTOR3(-500.0f, 90.0f, 1100.0f);
			break;
		}
		gem->mesh.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		gem->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);
	}
}

//=============================================================================
// �I������
void UninitGem(void)
{
	GEM *gem = &gemWk[0];
	if (D3DTextureGem != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureGem->Release();
		D3DTextureGem = NULL;
	}
	for (int i = 0; i < GEM_MAX; i++, gem++)
	{
		if (gem->mesh.VtxBuff != NULL)
		{// ���_�o�b�t�@�̊J��
			gem->mesh.VtxBuff->Release();
			gem->mesh.VtxBuff = NULL;
		}
	}
}



//=============================================================================
// �X�V����
void UpdateGem(void)
{
	GEM *gem = &gemWk[0];
	PLAYER *player = GetPlayer(0);
	D3DXVECTOR3 Vel = VEC3CLEAR;
	D3DXVECTOR3 Sparkle = VEC3CLEAR;

	int i, j;
	for (i = 0; i < GEM_MAX; i++, gem++)
		if (gem->mesh.use)
		{
			gem->mesh.Rot.y += ROT_VALUE;
			for (j = 0, player = GetPlayer(0); j < PLAYER_MAX; j++, player++)
				if ((CheckHitBC(player->Pos, gem->mesh.Pos, 30, 30)) == true)
					gem->mesh.use = false;


			if (SparkCnt % 10 == 0)
			{
				Sparkle.x = gem->mesh.Pos.x + (rand() % SPARKLE_DISTANCE) - SPARKLE_DISTANCE / 2;
				Sparkle.y = gem->mesh.Pos.y + 10 + (rand() % SPARKLE_DISTANCE) - SPARKLE_DISTANCE / 2;
				Sparkle.z = gem->mesh.Pos.z + (rand() % SPARKLE_DISTANCE) - SPARKLE_DISTANCE / 2;

				SetEffect(Sparkle, D3DXVECTOR3(0.0f, 0.1f, 0.0f),
					D3DXCOLOR(0.65f, 0.05f, 0.85f, 0.50f), 16.0f, 16.0f, 60, Idx_SPARK);
			}
		}
	SparkCnt++;
}


//=============================================================================
// �`�揈��
void DrawGem(void)
{
	GEM *gem = &gemWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// �X�P�[���A��]�A���s�ړ�
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	int i, j;

	for (i = 0; i < GEM_MAX; i++, gem++)
		if (gem->mesh.use == true)
		{
			// ���[���h�}�g���N�X������
			D3DXMatrixIdentity(&gem->mesh.mtxWorld);

			// �X�P�[���𔽉f	(S)
			D3DXMatrixScaling(&mtxScl, gem->mesh.Scl.x, gem->mesh.Scl.y, gem->mesh.Scl.z);
			D3DXMatrixMultiply(&gem->mesh.mtxWorld, &gem->mesh.mtxWorld, &mtxScl); 	// &mesh->mtxWorld = &mesh->mtxWorld * &mtxScl
																						// ��]�𔽉f		(R)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, gem->mesh.Rot.y, gem->mesh.Rot.x, gem->mesh.Rot.z);
			D3DXMatrixMultiply(&gem->mesh.mtxWorld, &gem->mesh.mtxWorld, &mtxRot);	// &mesh->mtxWorld = &mesh->mtxWorld * &mtxRot
																						// ���s�ړ��𔽉f	(T)
			D3DXMatrixTranslation(&mtxTranslate, gem->mesh.Pos.x, gem->mesh.Pos.y, gem->mesh.Pos.z);
			D3DXMatrixMultiply(&gem->mesh.mtxWorld, &gem->mesh.mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &gem->mesh.mtxWorld);

			// ���݂̃}�e���A����ۑ�
			pDevice->GetMaterial(&matDef);

			// �}�e���A�����ւ̃|�C���^��
			pD3DXMat = (D3DXMATERIAL*)gem->mesh.BuffMat->GetBufferPointer();


			for (j = 0; j < (int)gem->mesh.NumMat; j++)
			{
				pDevice->SetMaterial(&pD3DXMat[j].MatD3D);	// �}�e���A���̐ݒ�

				pDevice->SetTexture(0, D3DTextureGem);	// �e�N�X�`���̐ݒ�
				gem->mesh.Mesh->DrawSubset(j);
			}

			// �}�e���A���ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
}

//=============================================================================
// �W�F���̈ړ�����
void MoveGem(int no)
{
	GEM *gem = &gemWk[no];

	gem->mesh.Rot.y += ROT_VALUE;

}


//=============================================================================
// �W�F���̃Q�b�g�֐�
GEM *GetGem(int no)
{
	return (&gemWk[no]);
}
