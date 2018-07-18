//=============================================================================
//
// ��C���� [cannon.cpp]
// Author : ���ꐶ
//
//
//�g�p����Ă��Ȃ�
//
/*
�G�l�~�[�i��C�j
��]�Ɣ���

enemy(cannon)
rotate and fire cannon
*/
//=============================================================================
#include "enemy.h"
#include "camera.h"
#include "player.h"
#include "cannon.h"
#include "bullet.h"

#include "checkhit.h"

//=============================================================================
// �}�N����`
#define TEXTURE			("data/TEXTURE/pig_map.jpg")
#define MODEL			("data/MODEL/car000.x")
#define	VALUE_MOVE		(0.5f)											// �ړ���
#define	ROT_VALUE		(D3DX_PI * 0.02f)								// ��]��
#define SCL_VALUE		(1.0f)
#define AGGRO_RANGE		(500)											// �A�O������

//=============================================================================
// �v���g�^�C�v�錾
void MoveCan(void);

//=============================================================================
// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		D3DTextureCan = NULL;	// �e�N�X�`���ւ̃|�C���^

CAN						canWk[CAN_MAX];			// �t�B�[���h�ւ̃|�C���^

//=============================================================================
// ����������
HRESULT InitCannon(void)
{
	CAN *can = &canWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int i;
	for (i = 0; i < CAN_MAX; i++, can++)
	{
		can->ene.use = true;


		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		can->ene.Pos = D3DXVECTOR3(0.0f, 0.0f, 50.0f);
		can->ene.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		can->ene.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);

		// ���f���֌W�̏�����
		//D3DXCreateTextureFromFile(pDevice, TEXTURE, &D3DTextureCan);
		D3DTextureCan = NULL;
		can->ene.Mesh = NULL;
		can->ene.BuffMat = NULL;
		can->ene.NumMat= 0;

		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(MODEL,			// ���f���f�[�^
			D3DXMESH_SYSTEMMEM,				// �g�p���郁�����̃I�v�V����
			pDevice,						// �f�o�C�X 
			NULL,							// ���g�p
			&can->ene.BuffMat,				// �}�e���A���f�[�^�ւ̃|�C���^�[
			NULL,							// ���g�p
			&can->ene.NumMat,				// D3DXMATERIAL�\���̂̐�
			&can->ene.Mesh)))				// ���b�V���f�[�^�ւ̃|�C���^�[
		{
			return E_FAIL;
		}
	}
	return S_OK;

}

//=============================================================================
// �I������
void UninitCannon(void)
{
	CAN *can = &canWk[0];

	if (D3DTextureCan != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureCan->Release();
		D3DTextureCan = NULL;
	}
	for (int i = 0; i < CAN_MAX; i++, can++)
	{
		if (can->ene.VtxBuff != NULL)
		{// ���_�o�b�t�@�̊J��
			can->ene.VtxBuff->Release();
			can->ene.VtxBuff = NULL;
		}
		if (can->ene.Mesh != NULL)
		{// ���V���̉��
			can->ene.Mesh->Release();
			can->ene.Mesh = NULL;
		}
	}
}

//=============================================================================
// �X�V����
void UpdateCannon(void)
{
	CAN *can = &canWk[0];
	PLAYER *player = GetPlayer(0);
	D3DXVECTOR3 Vel = { 0.0f,0.0f,0.0f };
	int i;
	for (i = 0; i < CAN_MAX; i++, can++)
	{
		D3DXVECTOR3 temp3 = player->Pos - can->ene.Pos;
		float distance = D3DXVec3Length(&temp3);
		if (distance < AGGRO_RANGE)
		{
			float tAng = atan2(temp3.z, temp3.x);

			if (can->ShotCD <= 0)
			{
				D3DXVECTOR3 ObjPos = VEC3CLEAR;
				if (!VisionCheck(can->ene.Pos, player->Pos, &ObjPos))
				{
					SetBull(can->ene.Pos, can->ene.Rot);
					can->ShotCD = 50;
				}
			}
			can->ene.Rot.y = -tAng +(D3DX_PI * -0.5);

		}
		can->ShotCD--;

	}

}

//=============================================================================
// �`�揈��
void DrawCannon(void)
{
	CAN *can = &canWk[0];
	CAMERA *camera = GetCamera(0);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// �X�P�[���A��]�A���s�ړ�
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	int i, j;

	for (i = 0; i < CAN_MAX; i++, can++)
		if (can->ene.use == true)
		{
			// ���[���h�}�g���N�X������
			D3DXMatrixIdentity(&can->ene.mtxWorld);

			// �X�P�[���𔽉f	(S)
			D3DXMatrixScaling(&mtxScl, can->ene.Scl.x, can->ene.Scl.y, can->ene.Scl.z);
			D3DXMatrixMultiply(&can->ene.mtxWorld, &can->ene.mtxWorld, &mtxScl); 	// &ene->mtxWorld = &ene->mtxWorld * &mtxScl
			// ��]�𔽉f		(R)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, can->ene.Rot.y, can->ene.Rot.x, can->ene.Rot.z);
			D3DXMatrixMultiply(&can->ene.mtxWorld, &can->ene.mtxWorld, &mtxRot);	// &ene->mtxWorld = &ene->mtxWorld * &mtxRot
			// ���s�ړ��𔽉f	(T)
			D3DXMatrixTranslation(&mtxTranslate, can->ene.Pos.x, can->ene.Pos.y, can->ene.Pos.z);
			D3DXMatrixMultiply(&can->ene.mtxWorld, &can->ene.mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &can->ene.mtxWorld);

			// ���݂̃}�e���A����ۑ�
			pDevice->GetMaterial(&matDef);

			// �}�e���A�����ւ̃|�C���^��
			pD3DXMat = (D3DXMATERIAL*)can->ene.BuffMat->GetBufferPointer();


			for (j = 0; j < (int)can->ene.NumMat; j++)
			{
				pDevice->SetMaterial(&pD3DXMat[j].MatD3D);	// �}�e���A���̐ݒ�

				pDevice->SetTexture(0, D3DTextureCan);	// �e�N�X�`���̐ݒ�

				can->ene.Mesh->DrawSubset(j);
			}

			// �}�e���A���ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
}

//=============================================================================
// �Ԃ̈ړ�
void MoveCannon(void)
{
	CAN *can = &canWk[0];
	PLAYER *player = GetPlayer(0);

}

//=============================================================================
// �Ԃ̃Q�b�g�֐�
CAN *GetCannon(int no)
{
	return (&canWk[no]);
}