//=============================================================================
//
// �C�m�V�V���� [boar.cpp]
// Author : ���ꐶ
/*
�G�l�~�[�i�C�m�V�V�j
�O��Ɖ�]�ňړ�

enemy(boar)
moves forward and back
rotates to face player
*/
//=============================================================================
#include "enemy.h"
#include "game.h"
#include "camera.h"
#include "player.h"
#include "boar.h"
#include "checkhit.h"
#include "mathWk.h"
#include "collision.h"
#include "effect.h"
#include "rock.h"
#include "crate.h"
#include "status icon.h"

#include "fence.h"


//=============================================================================
// �}�N����`
#define TEXTURE			("data/TEXTURE/boar000.png")
#define MODEL			("data/MODEL/boar000.x")
#define	MOVE_VAL		(2.0f)			// �ړ���
#define CHASE_VAL		(4.0f)			// �ǂ�������Ƃ��̈ړ���
#define	ROT_VAL			(0.1f)			// ��]��
#define CHECK_ROT		(0.03f)			// �`�F�b�N���̉�]��
#define SCL_VALUE		(0.3f)			// �傫��
#define AGGRO_RANGE		(500)			// �A�O������
#define CHECK_RANGE		(200)			// �`�F�b�N����
#define LEASH_RANGE		(1500)			// �ǐՂ��鋗��
#define VISION_RAD		(0.7)			// ������͈�
#define VISION_ROAM		(0.95)			// �O���͈̔�


#define OBJECT_CHECK_RANGE	(200)		// ��Q���̃`�F�b�N�͈̔�


#define IDLE_TIME		(1*60)			// �A�C�h���̎���


#define HIGHT_VALUE		(5.0f)			// ���̈ʒu

#define WATER_HEIGHT	(-100)			// ���ʂ̍���

enum
{
	IS_FALSE = 0,
	IN_CHECK,
	IN_VISION
};

//=============================================================================
// �v���g�^�C�v�錾
void ResetBoar(int no);
void MoveBoar(int no);

int BoarVisionCheck(int no);
void SetBoarPos(int no);
void SetRoamingPos(int no);

//=============================================================================
// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		D3DTextureBoar = NULL;	// �e�N�X�`���ւ̃|�C���^

BOAR					boarWk[BOAR_MAX];			// �t�B�[���h�ւ̃|�C���^

//=============================================================================
// ����������
HRESULT InitBoar(void)
{
	BOAR *boar = &boarWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̐ݒ�
	D3DXCreateTextureFromFile(pDevice, TEXTURE, &D3DTextureBoar);

	FILE *fp;
	// �G�l�~�[�f�[�^�������Ă�t�@�C�����J��
	fp = fopen(ENEMY_DATA_FILE, "r");
	if (fp == NULL) {
		perror("Error opening file");
		return(-1);
	}


	int i;
	for (i = 0; i < BOAR_MAX; i++, boar++)
	{
		boar->Stat = IDLING;
		boar->IdleTime = IDLE_TIME;

		// �g�p���邩�A�|�b�v�ʒu�A�s���͈͂��t�@�C������Q�b�g
		fscanf(fp, "%d,%f,%f,%f,%f,%f", &boar->ene.use, &boar->StaringPos.x, &boar->StaringPos.y, &boar->StaringPos.z, &boar->RoamDistance.x, &boar->RoamDistance.y);

		boar->ene.Pos = boar->StaringPos;

		// ��]�ƃX�P�[���̏����ݒ�
		boar->ene.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		boar->ene.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);
		boar->ene.Grav = 0;

		// ���f���֌W�̏�����
		//D3DTextureBoar = NULL;
		boar->ene.Mesh = NULL;
		boar->ene.BuffMat = NULL;
		boar->ene.NumMat = 0;

		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(MODEL,			// ���f���f�[�^
			D3DXMESH_SYSTEMMEM,				// �g�p���郁�����̃I�v�V����
			pDevice,						// �f�o�C�X 
			NULL,							// ���g�p
			&boar->ene.BuffMat,				// �}�e���A���f�[�^�ւ̃|�C���^�[
			NULL,							// ���g�p
			&boar->ene.NumMat,				// D3DXMATERIAL�\���̂̐�
			&boar->ene.Mesh)))				// ���b�V���f�[�^�ւ̃|�C���^�[
		{
			return E_FAIL;
		}
	}

	fclose(fp);
	return S_OK;

}

//=============================================================================
// �C�m�V�V�̃��Z�b�g����
void ResetBoar(int no)
{
	BOAR *boar = &boarWk[0];

	boar->ene.use = true;
	boar->Stat = IDLING;
	boar->IdleTime = IDLE_TIME;

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	boar->ene.Pos = boar->StaringPos;
	boar->ene.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	boar->ene.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);
	boar->ene.Grav = 0;

}

//=============================================================================
// �I������
void UninitBoar(void)
{
	BOAR *boar = &boarWk[0];

	if (D3DTextureBoar != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureBoar->Release();
		D3DTextureBoar = NULL;
	}
	for (int i = 0; i < BOAR_MAX; i++, boar++)
	{
		if (boar->ene.VtxBuff != NULL)
		{// ���_�o�b�t�@�̊J��
			boar->ene.VtxBuff->Release();
			boar->ene.VtxBuff = NULL;
		}
		if (boar->ene.Mesh != NULL)
		{// ���V���̉��
			boar->ene.Mesh->Release();
			boar->ene.Mesh = NULL;
		}

	}
}

//=============================================================================
// �X�V����
void UpdateBoar(void)
{
	BOAR *boar = &boarWk[0];
	PLAYER *player = GetPlayer(0);
	D3DXVECTOR3 Vel = VEC3CLEAR;
	D3DXVECTOR3 temp = VEC3CLEAR;
	D3DXVECTOR3 temp2 = VEC3CLEAR;
	D3DXVECTOR3 ObjPos = VEC3CLEAR;
	float tLen = 0.0f;
	int i;
	for (i = 0; i < BOAR_MAX; i++, boar++)
	{
		if (boar->ene.use == true)
		{
			// �}�b�v���痎�������Ƀ��Z�b�g�Ŗ߂�
			if (boar->ene.Pos.y < WATER_HEIGHT)
			{
				ResetBoar(i);
			}

			switch (boar->Stat) // ���݂̏󋵂�����
			{//============================================ IDLING
			case IDLING:
				// �A�C�h���ɐ؂�ւ����Ƃ��Ɏ��Ԃ��[���Ȃ�Z�b�g
				if (boar->IdleTime <= 0)
					boar->IdleTime = IDLE_TIME;

				switch (BoarVisionCheck(i))
				{	// �`�F�b�N�͈�
				case IN_CHECK:
					temp = boar->ene.Pos;
					temp.y += 100;
					SetStatIco(temp, Idx_CHECK);

					boar->Stat = CHECKING;
					boar->IdleTime = 0;
					break;
					// ����͈�
				case IN_VISION:
					temp = boar->ene.Pos;
					temp.y += 100;
					SetStatIco(temp, Idx_FOUND);

					boar->Stat = CHASING;
					boar->IdleTime = 0;
					break;
				}

				boar->IdleTime--;
				// �A�C�h�����Ɏ��Ԃ�������؂�ւ�
				if (boar->IdleTime <= 0)
				{
					boar->Stat = ROAMING;
					SetRoamingPos(i);
				}
				break;
				//========================================= ROAMING
			case ROAMING:

				switch (BoarVisionCheck(i))
				{	// �`�F�b�N�͈�
				case IN_CHECK:
					temp = boar->ene.Pos;
					temp.y += 100;
					SetStatIco(temp, Idx_CHECK);

					boar->Stat = CHECKING;
					boar->IdleTime = 0;
					break;
					// ����͈�
				case IN_VISION:
					temp = boar->ene.Pos;
					temp.y += 100;
					SetStatIco(temp, Idx_FOUND);

					boar->Stat = CHASING;
					boar->IdleTime = 0;
					break;
				}

				temp = boar->ene.Pos - boar->RoamPos;
				tLen = D3DXVec3Length(&temp);
				if (tLen < 470)
				{
					boar->Stat = IDLING;
					boar->IdleTime = IDLE_TIME;
				}

				// �C�m�V�V�̈ړ�
				MoveBoar(i);


				// �ڂ̑O�ɏ�Q�������邩�`�F�b�N
				temp = boar->ene.Pos + Ang2Vec(boar->ene.Rot, OBJECT_CHECK_RANGE);
				if (VisionCheck(boar->ene.Pos, temp, &ObjPos))
				{
					temp = ObjPos - boar->ene.Pos;
					temp2 = Ang2Vec(boar->ene.Rot, 1);
					if (Vec2Cross(temp2, temp) <= 0)
						boar->ene.Rot.y -= ROT_VAL;
					else
						boar->ene.Rot.y += ROT_VAL;
					break;
				}

				// �ړI�n���ڂ̑O�ɂ��邩
				if (!inView(boar->ene.Pos, boar->RoamPos, boar->ene.Rot, VISION_ROAM))
				{	// ������ΉE�����։�]
					temp = boar->RoamPos - boar->ene.Pos;
					temp2 = Ang2Vec(boar->ene.Rot, 1);
					if (Vec2Cross(temp2, temp) <= 0)
						boar->ene.Rot.y += ROT_VAL / 2;
					else
						boar->ene.Rot.y -= ROT_VAL / 2;
				}
				break;
				//========================================= CHASING
			case CHASING:
				temp = player->Pos - boar->ene.Pos;
				tLen = D3DXVec3Length(&temp);
				if (tLen > AGGRO_RANGE)
				{
					boar->Stat = IDLING;
					break;
				}
				temp = boar->StaringPos - boar->ene.Pos;
				tLen = D3DXVec3Length(&temp);
				if (tLen > LEASH_RANGE)
				{
					boar->Stat = IDLING;
					break;
				}
				MoveBoar(i);

				// �ڂ̑O�ɏ�Q�������邩�`�F�b�N
				temp = boar->ene.Pos + Ang2Vec(boar->ene.Rot, OBJECT_CHECK_RANGE);
				if (VisionCheck(boar->ene.Pos, temp, &ObjPos))
				{
					temp = ObjPos - boar->ene.Pos;
					temp2 = Ang2Vec(boar->ene.Rot, 1);
					if (Vec2Cross(temp2, temp) <= 0)
						boar->ene.Rot.y -= ROT_VAL;
					else
						boar->ene.Rot.y += ROT_VAL;
					break;
				}
				// �ړI�n���ڂ̑O�ɂ��邩
				if (!inView(boar->ene.Pos, player->Pos, boar->ene.Rot, VISION_ROAM))
				{	// ������ΉE�����։�]
					temp = player->Pos - boar->ene.Pos;
					temp2 = Ang2Vec(boar->ene.Rot, 1);
					if (Vec2Cross(temp2, temp) <= 0)
						boar->ene.Rot.y += ROT_VAL;
					else
						boar->ene.Rot.y -= ROT_VAL;
				}
				break;
				//========================================= CHECKING
			case CHECKING:
				temp = player->Pos - boar->ene.Pos;
				tLen = D3DXVec3Length(&temp);
				if (tLen < AGGRO_RANGE)
				{
					// �ړI�n���ڂ̑O�ɂ��邩
					if (!inView(boar->ene.Pos, player->Pos, boar->ene.Rot, VISION_ROAM))
					{	// ������ΉE�����։�]
						temp = player->Pos - boar->ene.Pos;
						temp2 = Ang2Vec(boar->ene.Rot, 1);
						if (Vec2Cross(temp2, temp) <= 0)
							boar->ene.Rot.y += CHECK_ROT;
						else
							boar->ene.Rot.y -= CHECK_ROT;
					}
					else
					{
						temp = boar->ene.Pos;
						temp.y += 100;
						SetStatIco(temp, Idx_FOUND);

						boar->Stat = CHASING;
					}
				}
				else
				{
					boar->Stat = IDLING;
					boar->IdleTime = IDLE_TIME;
				}
				break;
			}
		}
	}
}

//=============================================================================
// �`�揈��
void DrawBoar(void)
{
	BOAR *boar = &boarWk[0];
	CAMERA *camera = GetCamera(0);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// �X�P�[���A��]�A���s�ړ�
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	int i, j;

	for (i = 0; i < BOAR_MAX; i++, boar++)
		if (boar->ene.use == true)
		{
			// ���[���h�}�g���N�X������
			D3DXMatrixIdentity(&boar->ene.mtxWorld);

			// �X�P�[���𔽉f	(S)
			D3DXMatrixScaling(&mtxScl, boar->ene.Scl.x, boar->ene.Scl.y, boar->ene.Scl.z);
			D3DXMatrixMultiply(&boar->ene.mtxWorld, &boar->ene.mtxWorld, &mtxScl); 	// &ene->mtxWorld = &ene->mtxWorld * &mtxScl
			// ��]�𔽉f		(R)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, boar->ene.Rot.y, boar->ene.Rot.x, boar->ene.Rot.z);
			D3DXMatrixMultiply(&boar->ene.mtxWorld, &boar->ene.mtxWorld, &mtxRot);	// &ene->mtxWorld = &ene->mtxWorld * &mtxRot
			// ���s�ړ��𔽉f	(T)
			D3DXMatrixTranslation(&mtxTranslate, boar->ene.Pos.x, boar->ene.Pos.y, boar->ene.Pos.z);
			D3DXMatrixMultiply(&boar->ene.mtxWorld, &boar->ene.mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &boar->ene.mtxWorld);

			// ���݂̃}�e���A����ۑ�
			pDevice->GetMaterial(&matDef);

			// �}�e���A�����ւ̃|�C���^��
			pD3DXMat = (D3DXMATERIAL*)boar->ene.BuffMat->GetBufferPointer();


			for (j = 0; j < (int)boar->ene.NumMat; j++)
			{
				pDevice->SetMaterial(&pD3DXMat[j].MatD3D);	// �}�e���A���̐ݒ�

				pDevice->SetTexture(0, D3DTextureBoar);	// �e�N�X�`���̐ݒ�

				boar->ene.Mesh->DrawSubset(j);
			}

			// �}�e���A���ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
}

//=============================================================================
// �C�m�V�V�̎���`�F�b�N
int BoarVisionCheck(int no)
{
	BOAR *boar = &boarWk[no];
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < PLAYER_MAX; i++, player++)
		if (player->use)
		{
			// �͈͂��`�F�b�N
			D3DXVECTOR3 temp3 = player->Pos - boar->ene.Pos;
			float distance = D3DXVec3Length(&temp3);
			if (distance < AGGRO_RANGE)

				// ����͈͂��`�F�b�N
				if (inView(boar->ene.Pos, player->Pos, boar->ene.Rot, VISION_RAD))
				{

					// ���C�`�F�b�N
					D3DXVECTOR3 ObjPos = VEC3CLEAR;
					if (!VisionCheck(boar->ene.Pos, player->Pos, &ObjPos))
						return IN_VISION;
				}
				else if (distance < CHECK_RANGE)
					return IN_CHECK;
		}
	return IS_FALSE;
}

//=============================================================================
// �C�m�V�V�̈ړ�
void MoveBoar(int no)
{
	BOAR *boar = &boarWk[no];
	PLAYER *player = GetPlayer(0);
	D3DXVECTOR3 Vel = VEC3CLEAR;
	D3DXVECTOR3 oldPos = boar->ene.Pos;

	FENCE *fence = GetFence(0);

	switch (boar->Stat)
	{
	case ROAMING:
		Vel = Ang2Vec(boar->ene.Rot, MOVE_VAL);
		boar->ene.Pos += Vel;
		break;
	case CHECKING:
		break;
	case CHASING:
		Vel = Ang2Vec(boar->ene.Rot, CHASE_VAL);
		boar->ene.Pos += Vel;
		SetEffect(boar->ene.Pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(0.65f, 0.05f, 0.85f, 0.5f), 30.0f, 30.0f, 30, Idx_DUST02);
		break;
	}

	//===========================================================
	// ��̓����蔻��
	D3DXVECTOR3 tDistance;
	D3DXVECTOR3 tVel = boar->ene.Pos + Vel;
	D3DXVECTOR3 tPos = oldPos;
	D3DXVECTOR3 tsize = Ang2Vec(boar->ene.Rot, 2);

	// ���f���̌����ڂɃR���W���������킹�Ă݂�		fix collision to match player modle
	tVel.y += 10;
	tPos.y += 10;

	int i;

	D3DXVECTOR3 vec = VEC3CLEAR;
	float len = 0;
	for (i = 0, fence = GetFence(0); i < FENCE_MAX; i++, fence++)
		if (fence->mesh.use == true
			&& fence->isHitbox == true)
		{
			vec = boar->ene.Pos - fence->mesh.Pos;
			len = D3DXVec3Length(&vec);
			if (len < 400)
				if (CheckHitLinePrep(tPos, tVel, &tDistance, &fence->mesh))
				{
					// �������Ă���̂ŁA����������_�܂ł̋����𑫂�
					boar->ene.Pos += tDistance - tsize;
					break;
				}
		}

	// �n�ʂƂ̓����蔻��
	GroundCollision(&boar->ene.Pos, &boar->ene.Grav);

	// �ǂƂ̓����蔻��
	D3DXVECTOR3 distance = { 0.0f,0.0f,0.0f };	// �ǂ܂ł̋���
	D3DXVECTOR3 StartingPos = oldPos;
	D3DXVECTOR3 EndPos = boar->ene.Pos;
	D3DXVECTOR3 temp = Ang2Vec(boar->ene.Rot, 100);

	EndPos += temp;
	EndPos.y += 5;
	StartingPos.y += 5;
	if (CheckHitWallPrep(StartingPos, EndPos, &distance))
	{
		float tLen = D3DXVec3LengthSq(&distance);
		if (tLen < 500)
		{
			float ftemp = Vec2Cross(boar->ene.Pos, boar->RoamPos);

			if (ftemp < 0)
				boar->ene.Rot.y -= ROT_VAL;
			else
				boar->ene.Rot.y += ROT_VAL;
			if (tLen < 300)
			{
				if (ftemp < 0)
					boar->ene.Rot.y -= ROT_VAL;
				else
					boar->ene.Rot.y += ROT_VAL;
				if (tLen < 200)
				{
					if (ftemp < 0)
						boar->ene.Rot.y -= ROT_VAL;
					else
						boar->ene.Rot.y += ROT_VAL;
					if (tLen < 150)
					{
						if (ftemp < 0)
							boar->ene.Rot.y -= ROT_VAL;
						else
							boar->ene.Rot.y += ROT_VAL;
					}
				}
			}
		}
	}
}


//=============================================================================
// �C�m�V�V�̍s��������߂鏈��
void SetRoamingPos(int no)
{
	BOAR *boar = &boarWk[no];
	ROCK *rock = GetRock(0);
	CRATE *crate = GetCrate(0);
	int i;
	bool tRock = true;
	bool tCrate = true;

	int x = boar->RoamDistance.x;
	int z = boar->RoamDistance.y;
	while (tRock || tCrate)
	{
		tRock = true;
		tCrate = true;

		boar->RoamPos.x = boar->StaringPos.x - (rand() % x);
		boar->RoamPos.z = boar->StaringPos.z + (rand() % z);

			for (i = 0, rock = GetRock(0); i < ROCK_MAX; i++, rock++)
		{
			if (!CheckHitBC(boar->StaringPos, rock->mesh.Pos, 10, 175))
				tRock = false;
		}

		for (i = 0, crate = GetCrate(0); i < CRATE_MAX; i++, crate++)
		{
			if (!CheckHitBC(boar->StaringPos, crate->mesh.Pos, 10, 175))
				tCrate = false;
		}
	}
}

//=============================================================================
// �C�m�V�V�̃Q�b�g�֐�
BOAR *GetBoar(int no)
{
	return (&boarWk[no]);
}