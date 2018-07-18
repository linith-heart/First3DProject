//=============================================================================
//
// �T�C�g���� [sight.cpp]
// Author : ���ꐶ
//
//
//�g�p����Ă��Ȃ�
//
/*
�G�l�~�[�i�T�C�g�j
�ڐ��ɂɓ���Ə������n�߂�

enemy (sight)
movement based on line of sight
*/
//=============================================================================
#include "enemy.h"
#include "camera.h"
#include "player.h"
#include "sight.h"
#include "bullet.h"
#include "checkhit.h"
#include "mathWk.h"


//=============================================================================
// �}�N����`
#define TEXTURE			("data/TEXTURE/pig_map.jpg")
#define MODEL			("data/MODEL/car001.x")
#define	VALUE_MOVE		(0.5f)							// �ړ���
#define	ROT_VALUE		(D3DX_PI * 0.02f)				// ��]��

#define AGGRO_RANGE		(500)							// �������鋗��
#define VISION_RAD		(0.7)							// ������͈�

//=============================================================================
// �v���g�^�C�v�錾
void MoveSight(void);

//=============================================================================
// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		D3DTextureSight = NULL;	// �e�N�X�`���ւ̃|�C���^

SIGHT						sightWk[SIGHT_MAX];			// �t�B�[���h�ւ̃|�C���^

//=============================================================================
// ����������
HRESULT InitSight(void)
{
	SIGHT *sight = &sightWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int i;
	for (i = 0; i < SIGHT_MAX; i++, sight++)
	{
		sight->ene.use = true;


		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		sight->ene.Pos = D3DXVECTOR3(100.0f, 0.0f, 0.0f);
//		sight->ene.Pos = D3DXVECTOR3(100.0f, 0.0f, 50.0f);
		sight->ene.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		sight->ene.Scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

		// ���f���֌W�̏�����
		//D3DXCreateTextureFromFile(pDevice, TEXTURE, &D3DTextureSight);
		D3DTextureSight = NULL;
		sight->ene.Mesh = NULL;
		sight->ene.BuffMat = NULL;
		sight->ene.NumMat= 0;

		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(MODEL,			// ���f���f�[�^
			D3DXMESH_SYSTEMMEM,				// �g�p���郁�����̃I�v�V����
			pDevice,						// �f�o�C�X 
			NULL,							// ���g�p
			&sight->ene.BuffMat,				// �}�e���A���f�[�^�ւ̃|�C���^�[
			NULL,							// ���g�p
			&sight->ene.NumMat,				// D3DXMATERIAL�\���̂̐�
			&sight->ene.Mesh)))				// ���b�V���f�[�^�ւ̃|�C���^�[
		{
			return E_FAIL;
		}
	}
	return S_OK;

}

//=============================================================================
// �I������
void UninitSight(void)
{
	SIGHT *sight = &sightWk[0];

	if (D3DTextureSight != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureSight->Release();
		D3DTextureSight = NULL;
	}

	for (int i = 0; i < SIGHT_MAX; i++, sight++)
	{
		if (sight->ene.VtxBuff != NULL)
		{// ���_�o�b�t�@�̊J��
			sight->ene.VtxBuff->Release();
			sight->ene.VtxBuff = NULL;
		}
		if (sight->ene.Mesh != NULL)
		{// ���V���̉��
			sight->ene.Mesh->Release();
			sight->ene.Mesh = NULL;
		}

	}
}

//=============================================================================
// �X�V����
void UpdateSight(void)
{
	SIGHT *sight = &sightWk[0];
	PLAYER *player = GetPlayer(0);
	D3DXVECTOR3 Vel = { 0.0f,0.0f,0.0f };
	int i;
	for (i = 0; i < SIGHT_MAX; i++, sight++)
	{
		D3DXVECTOR3 temp3 = player->Pos - sight->ene.Pos;
		float distance = D3DXVec3Length(&temp3);

		if (distance < AGGRO_RANGE)
		{

			if (inView(sight->ene.Pos,player->Pos,sight->ene.Rot, VISION_RAD))
			{
				D3DXVECTOR3 ObjPos = VEC3CLEAR;
				if (!VisionCheck(sight->ene.Pos, player->Pos, &ObjPos))
				{
					SetBull(sight->ene.Pos, sight->ene.Rot);
				}
			}

			sight->ene.Rot.y += ROT_VALUE;
		}
		else
		{
			sight->ene.Rot.y += ROT_VALUE;
		}

	
	}
}

//=============================================================================
// �`�揈��
void DrawSight(void)
{
	SIGHT *sight = &sightWk[0];
	CAMERA *camera = GetCamera(0);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// �X�P�[���A��]�A���s�ړ�
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	int i, j;

	for (i = 0; i < SIGHT_MAX; i++, sight++)
		if (sight->ene.use == true)
		{
			// ���[���h�}�g���N�X������
			D3DXMatrixIdentity(&sight->ene.mtxWorld);

			// �X�P�[���𔽉f	(S)
			D3DXMatrixScaling(&mtxScl, sight->ene.Scl.x, sight->ene.Scl.y, sight->ene.Scl.z);
			D3DXMatrixMultiply(&sight->ene.mtxWorld, &sight->ene.mtxWorld, &mtxScl); 	// &ene->mtxWorld = &ene->mtxWorld * &mtxScl
																			// ��]�𔽉f		(R)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, sight->ene.Rot.y, sight->ene.Rot.x, sight->ene.Rot.z);
			D3DXMatrixMultiply(&sight->ene.mtxWorld, &sight->ene.mtxWorld, &mtxRot);	// &ene->mtxWorld = &ene->mtxWorld * &mtxRot
																			// ���s�ړ��𔽉f	(T)
			D3DXMatrixTranslation(&mtxTranslate, sight->ene.Pos.x, sight->ene.Pos.y, sight->ene.Pos.z);
			D3DXMatrixMultiply(&sight->ene.mtxWorld, &sight->ene.mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &sight->ene.mtxWorld);

			// ���݂̃}�e���A����ۑ�
			pDevice->GetMaterial(&matDef);

			// �}�e���A�����ւ̃|�C���^��
			pD3DXMat = (D3DXMATERIAL*)sight->ene.BuffMat->GetBufferPointer();


			for (j = 0; j < (int)sight->ene.NumMat; j++)
			{
				pDevice->SetMaterial(&pD3DXMat[j].MatD3D);	// �}�e���A���̐ݒ�

				pDevice->SetTexture(0, D3DTextureSight);	// �e�N�X�`���̐ݒ�

				sight->ene.Mesh->DrawSubset(j);
			}

			// �}�e���A���ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
}

//=============================================================================
// �Ԃ̈ړ�
void MoveSight(void)
{
	SIGHT *sight = &sightWk[0];
	PLAYER *player = GetPlayer(0);

}

//=============================================================================
// �Ԃ̃Q�b�g�֐�
SIGHT *GetSight(int no)
{
	return (&sightWk[no]);
}