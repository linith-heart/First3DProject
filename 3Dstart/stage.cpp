//=============================================================================
//
// �؂̏��� [stage.cpp]
// Author : ���ꐶ
//
//=============================================================================
#include "stage.h"

#include "camera.h"
#include "player.h"

#include "checkhit.h"
/*
���V���̖؂̏���

mesh stage
*/

//=============================================================================
// �}�N����`
#define TEXTURE_STAGE	("data/TEXTURE/stage000.png")					// ��̃e�N�X�`���[
#define MODEL			("data/MODEL/stage000.x")						// ��̃��f��
#define MODELHIT		("data/MODEL/stage_hitbox000.x")				// ��̓����蔻��p�̃��f��
#define	VALUE_MOVE		(0.5f)											// �ړ���
#define	ROT_VALUE		(D3DX_PI * 0.02f)								// ��]��
#define SCL_VALUE		(1.0f)											// �傫��
#define RANGE_VALUE		(50)											// ������̋���

#define	ALPHA			(170)


//=============================================================================
// �v���g�^�C�v�錾
void MoveStage(void);

void SetStagePos(int no);
//=============================================================================
// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		D3DTextureStage = NULL;		// �e�N�X�`���ւ̃|�C���^

STAGE					stageWk[STAGE_MAX];			// �t�B�[���h�ւ̃|�C���^


//=============================================================================
// ����������
HRESULT InitStage(void)
{
	STAGE *stage = &stageWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̐ݒ�
	D3DXCreateTextureFromFile(pDevice, TEXTURE_STAGE, &D3DTextureStage);

	int i;
	for (i = 0; i < STAGE_MAX; i++, stage++)
	{
		stage->mesh.use = true;

		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		SetStagePos(i);
		stage->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);

		// ���f���֌W�̏�����
		stage->mesh.VtxBuff = NULL;
		//MakeVertexStage(pDevice);
		stage->mesh.Mesh = NULL;
		stage->mesh.BuffMat = NULL;
		stage->mesh.NumMat = 0;

		if (i % COMP4STAGE == 0)
		{
			stage->isHitbox = false;
			// X�t�@�C���̓ǂݍ���
			if (FAILED(D3DXLoadMeshFromX(MODEL,			// ���f���f�[�^
				D3DXMESH_SYSTEMMEM,					// �g�p���郁�����̃I�v�V����
				pDevice,							// �f�o�C�X 
				NULL,								// ���g�p
				&stage->mesh.BuffMat,				// �}�e���A���f�[�^�ւ̃|�C���^�[
				NULL,								// ���g�p
				&stage->mesh.NumMat,				// D3DXMATERIAL�\���̂̐�
				&stage->mesh.Mesh)))				// ���b�V���f�[�^�ւ̃|�C���^�[
			{
				return E_FAIL;
			}
		}
		else if (i % COMP4STAGE == 1)
		{
			stage->isHitbox = true;
			// X�t�@�C���̓ǂݍ���
			if (FAILED(D3DXLoadMeshFromX(MODELHIT,			// ���f���f�[�^
				D3DXMESH_SYSTEMMEM,					// �g�p���郁�����̃I�v�V����
				pDevice,							// �f�o�C�X 
				NULL,								// ���g�p
				&stage->mesh.BuffMat,				// �}�e���A���f�[�^�ւ̃|�C���^�[
				NULL,								// ���g�p
				&stage->mesh.NumMat,				// D3DXMATERIAL�\���̂̐�
				&stage->mesh.Mesh)))				// ���b�V���f�[�^�ւ̃|�C���^�[
			{
				return E_FAIL;
			}

			SetVtxData(&stage->mesh);

		}

	}
	return S_OK;

}

//=============================================================================
// �I������
void UninitStage(void)
{
	STAGE *stage = &stageWk[0];
	if (D3DTextureStage != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureStage->Release();
		D3DTextureStage = NULL;
	}

	for (int i = 0; i < STAGE_MAX; i++, stage++)
	{
		if (stage->mesh.VtxBuff != NULL)
		{// ���_�o�b�t�@�̊J��
			stage->mesh.VtxBuff->Release();
			stage->mesh.VtxBuff = NULL;
		}

		if (stage->mesh.Mesh != NULL)
		{// ���V���̉��
			stage->mesh.Mesh->Release();
			stage->mesh.Mesh = NULL;
		}

		if (stage->mesh.IdxBuff != NULL)
		{// �C���f�b�N�X�o�b�t�@�ƃ��@�[�e�b�N�X�|�X�͈ꏏ�ɊJ��
			free(stage->mesh.IdxBuff);
			free(stage->mesh.VtxPos);
			stage->mesh.IdxBuff = NULL;
			stage->mesh.VtxPos = NULL;
		}
	}
}

//=============================================================================
// �X�V����
void UpdateStage(void)
{
	STAGE *stage = &stageWk[0];
	PLAYER *player = GetPlayer(0);
	D3DXVECTOR3 Vel = { 0.0f,0.0f,0.0f };
	int i;
	for (i = 0; i < STAGE_MAX; i++, stage++)
	{


	}
}

//=============================================================================
// �`�揈��
void DrawStage(void)
{
	STAGE *stage = &stageWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// �X�P�[���A��]�A���s�ړ�
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	int i, j;



	for (i = 0; i < STAGE_MAX; i++, stage++)
		if (stage->mesh.use == true)
		{

			// ���[���h�}�g���N�X������
			D3DXMatrixIdentity(&stage->mesh.mtxWorld);

			// �X�P�[���𔽉f	(S)
			D3DXMatrixScaling(&mtxScl, stage->mesh.Scl.x, stage->mesh.Scl.y, stage->mesh.Scl.z);
			D3DXMatrixMultiply(&stage->mesh.mtxWorld, &stage->mesh.mtxWorld, &mtxScl); 	// &mesh->mtxWorld = &mesh->mtxWorld * &mtxScl
																						// ��]�𔽉f		(R)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, stage->mesh.Rot.y, stage->mesh.Rot.x, stage->mesh.Rot.z);
			D3DXMatrixMultiply(&stage->mesh.mtxWorld, &stage->mesh.mtxWorld, &mtxRot);	// &mesh->mtxWorld = &mesh->mtxWorld * &mtxRot
																						// ���s�ړ��𔽉f	(T)
			D3DXMatrixTranslation(&mtxTranslate, stage->mesh.Pos.x, stage->mesh.Pos.y, stage->mesh.Pos.z);
			D3DXMatrixMultiply(&stage->mesh.mtxWorld, &stage->mesh.mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &stage->mesh.mtxWorld);

			// ���݂̃}�e���A����ۑ�
			pDevice->GetMaterial(&matDef);

			// �}�e���A�����ւ̃|�C���^��
			pD3DXMat = (D3DXMATERIAL*)stage->mesh.BuffMat->GetBufferPointer();

			if (!stage->isHitbox)
				for (j = 0; j < (int)stage->mesh.NumMat; j++)
				{
					pDevice->SetMaterial(&pD3DXMat[j].MatD3D);	// �}�e���A���̐ݒ�

					pDevice->SetTexture(0, D3DTextureStage);	// �e�N�X�`���̐ݒ�
					stage->mesh.Mesh->DrawSubset(j);
				}

			// �}�e���A���ɖ߂�
			pDevice->SetMaterial(&matDef);
		}

}

//=============================================================================
// �؂̈ړ�
void MoveStage(void)
{
	STAGE *stage = &stageWk[0];
	PLAYER *player = GetPlayer(0);

}

void SetStagePos(int no)
{
	STAGE *stage = &stageWk[no];

	switch (no)
	{
	case 0:	case 1:

		stage->mesh.Pos = D3DXVECTOR3(280.0f, 400.0f, -2130.0f);
		stage->mesh.Rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
		break;
	case 2:	case 3:
		stage->mesh.Pos = D3DXVECTOR3(450.0f, 25.0f, 2200.0f);
		stage->mesh.Rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.2f, 0.0f);
		break;
	case 4:	case 5:
		stage->mesh.Pos = D3DXVECTOR3(700.0f, 25.0f, 1800.0f);
		stage->mesh.Rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
		break;
	case 6:	case 7:
		stage->mesh.Pos = D3DXVECTOR3(280.0f, 400.0f, -2130.0f);
		stage->mesh.Rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
		break;
	}

}

//=============================================================================
// ���̃Q�b�g�֐�
STAGE *GetStage(int no)
{
	return (&stageWk[no]);
}






//int hitCheckStage(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
//{
//	STAGE *stage = &stageWk[0];
//	int				ans = 0;	// ��悸�������Ă��Ȃ����Z�b�g���Ă���
//
//	D3DXVECTOR3		pos[3];		// �|���S���̖@��
//	D3DXVECTOR3		vel = { 0.0f,0.0f,0.0f };
//
//
//	int PolyCnt = stage->mesh.Mesh->GetNumFaces();
//
//	for (int i = 0; i<PolyCnt; i++)
//	{	// �܂��́A�|���S���̒��_�����߂�
//		int idx0 = stage->mesh.IdxBuff[i * 3];
//		int idx1 = stage->mesh.IdxBuff[i * 3 + 1];
//		int idx2 = stage->mesh.IdxBuff[i * 3 + 2];
//
//		// ���_���W�̐ݒ�
//		pos[0] = stage->mesh.VtxPos[idx0];
//		pos[1] = stage->mesh.VtxPos[idx1];
//		pos[2] = stage->mesh.VtxPos[idx2];
//
//		// �������|���S���Ɛ����̓����蔻��
//		ans = CheckHitLine(pos[0], pos[2], pos[3], pos0, pos1, &stage->mesh.mtxWorld, &vel);
//		if (ans != 0)
//		{
//			break;
//		}
//
//
//	}
//
//	return(ans);
//
//}
