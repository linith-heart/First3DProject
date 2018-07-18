//=============================================================================
//
// �ԏ��� [car.cpp]
// Author : ���ꐶ
//
//
//�g�p����Ă��Ȃ�
//
/*
�G�l�~�[�i�ԁj
�O��Ɖ�]�ňړ�

enemy(car)
moves forward and back
rotates to face player
*/
//=============================================================================
#include "enemy.h"
#include "game.h"
#include "camera.h"
#include "player.h"
#include "car.h"
#include "checkhit.h"


//=============================================================================
// �}�N����`
#define TEXTURE			("data/TEXTURE/pig_map.jpg")
#define MODEL			("data/MODEL/car000.x")
#define	VALUE_MOVE		(2.0f)							// �ړ���
#define	ROT_VAL			(0.02f)							// ��]��
#define	ROT_VAL_MAX		(0.3f)							// ��]��
#define SCL_VALUE		(1.0f)
#define AGGRO_RANGE		(500)							// 
#define CHECK_RANGE		(200)							// 

#define HIGHT_VALUE		(5.0f)							// ���̈ʒu

//=============================================================================
// �v���g�^�C�v�錾
void MoveCar(void);

//=============================================================================
// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		D3DTextureCar = NULL;	// �e�N�X�`���ւ̃|�C���^

CAR						carWk[CAR_MAX];			// �t�B�[���h�ւ̃|�C���^

//=============================================================================
// ����������
HRESULT InitCar(void)
{
	CAR *car = &carWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int i;
	for (i = 0; i < CAR_MAX; i++, car++)
	{
		car->ene.use = true;


		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		car->ene.Pos = D3DXVECTOR3(-1500.0f, 0.0f, -1300.0f);
		car->ene.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		car->ene.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);

		// ���f���֌W�̏�����
		//D3DXCreateTextureFromFile(pDevice, TEXTURE, &D3DTextureCar);
		D3DTextureCar = NULL;
		car->ene.Mesh = NULL;
		car->ene.BuffMat = NULL;
		car->ene.NumMat= 0;

		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(MODEL,			// ���f���f�[�^
			D3DXMESH_SYSTEMMEM,				// �g�p���郁�����̃I�v�V����
			pDevice,						// �f�o�C�X 
			NULL,							// ���g�p
			&car->ene.BuffMat,				// �}�e���A���f�[�^�ւ̃|�C���^�[
			NULL,							// ���g�p
			&car->ene.NumMat,				// D3DXMATERIAL�\���̂̐�
			&car->ene.Mesh)))				// ���b�V���f�[�^�ւ̃|�C���^�[
		{
			return E_FAIL;
		}
	}
	return S_OK;

}

//=============================================================================
// �I������
void UninitCar(void)
{
	CAR *car = &carWk[0];

	if (D3DTextureCar != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureCar->Release();
		D3DTextureCar = NULL;
	}
	for (int i = 0; i < CAR_MAX; i++, car++)
	{
		if (car->ene.VtxBuff != NULL)
		{// ���_�o�b�t�@�̊J��
			car->ene.VtxBuff->Release();
			car->ene.VtxBuff = NULL;
		}
		if (car->ene.Mesh != NULL)
		{// ���V���̉��
			car->ene.Mesh->Release();
			car->ene.Mesh = NULL;
		}
	}
}

//=============================================================================
// �X�V����
void UpdateCar(void)
{
	CAR *car = &carWk[0];
	PLAYER *player = GetPlayer(0);
	D3DXVECTOR3 Vel = { 0.0f,0.0f,0.0f };
	int i;
	for (i = 0; i < CAR_MAX; i++, car++)
		if (car->ene.use == true)
		{
			D3DXVECTOR3 temp3 = player->Pos - car->ene.Pos;
			float distance = D3DXVec3Length(&temp3);
			if (distance < AGGRO_RANGE)
			{
				D3DXVECTOR3 ObjPos = VEC3CLEAR;
				float tAng = atan2(temp3.z, temp3.x);
				if (VisionCheck(car->ene.Pos, player->Pos, &ObjPos))
				{
					if (car->Turn < ROT_VAL_MAX)
					{
						car->Turn += ROT_VAL;
					}
				}
				else if (car->Turn >= 0.0f)
				{
					car->Turn -= ROT_VAL;
				}

				tAng += (D3DX_PI * -car->Turn);
				car->ene.Rot.y = -tAng +(D3DX_PI * -0.5);

				Vel.x = cosf(tAng) * VALUE_MOVE;
				Vel.z = sinf(tAng) * VALUE_MOVE;

				car->ene.Pos += Vel;

				// �n�ʂƂ̓����蔻��
				D3DXVECTOR3 distance = { 0.0f,0.0f,0.0f };	// �n�ʂ܂ł̋���
				D3DXVECTOR3 head = car->ene.Pos;
				D3DXVECTOR3 feet = car->ene.Pos;
				head.y += HIGHT_VALUE;
				feet.y -= HIGHT_VALUE;

				if (CheckHitGroundPrep(head, feet, &distance))
				{
					car->ene.Pos.y += distance.y;
					car->ene.Grav = 0.0f;
				}
				else
				{
					car->ene.Grav -= GRAVITY;
					car->ene.Pos.y += car->ene.Grav;
				}

			}


		}
}

//=============================================================================
// �`�揈��
void DrawCar(void)
{
	CAR *car = &carWk[0];
	CAMERA *camera = GetCamera(0);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// �X�P�[���A��]�A���s�ړ�
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	int i, j;

	for (i = 0; i < CAR_MAX; i++, car++)
		if (car->ene.use == true)
		{
			// ���[���h�}�g���N�X������
			D3DXMatrixIdentity(&car->ene.mtxWorld);

			// �X�P�[���𔽉f	(S)
			D3DXMatrixScaling(&mtxScl, car->ene.Scl.x, car->ene.Scl.y, car->ene.Scl.z);
			D3DXMatrixMultiply(&car->ene.mtxWorld, &car->ene.mtxWorld, &mtxScl); 	// &ene->mtxWorld = &ene->mtxWorld * &mtxScl
			// ��]�𔽉f		(R)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, car->ene.Rot.y, car->ene.Rot.x, car->ene.Rot.z);
			D3DXMatrixMultiply(&car->ene.mtxWorld, &car->ene.mtxWorld, &mtxRot);	// &ene->mtxWorld = &ene->mtxWorld * &mtxRot
			// ���s�ړ��𔽉f	(T)
			D3DXMatrixTranslation(&mtxTranslate, car->ene.Pos.x, car->ene.Pos.y, car->ene.Pos.z);
			D3DXMatrixMultiply(&car->ene.mtxWorld, &car->ene.mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &car->ene.mtxWorld);

			// ���݂̃}�e���A����ۑ�
			pDevice->GetMaterial(&matDef);

			// �}�e���A�����ւ̃|�C���^��
			pD3DXMat = (D3DXMATERIAL*)car->ene.BuffMat->GetBufferPointer();


			for (j = 0; j < (int)car->ene.NumMat; j++)
			{
				pDevice->SetMaterial(&pD3DXMat[j].MatD3D);	// �}�e���A���̐ݒ�

				pDevice->SetTexture(0, D3DTextureCar);	// �e�N�X�`���̐ݒ�

				car->ene.Mesh->DrawSubset(j);
			}

			// �}�e���A���ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
}

//=============================================================================
// �Ԃ̈ړ�
void MoveCar(void)
{
	CAR *car = &carWk[0];
	PLAYER *player = GetPlayer(0);

}

//=============================================================================
// �Ԃ̃Q�b�g�֐�
CAR *GetCar(int no)
{
	return (&carWk[no]);
}