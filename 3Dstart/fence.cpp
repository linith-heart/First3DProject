//=============================================================================
//
// ��̏��� [fence.cpp]
// Author : ���ꐶ
//
//=============================================================================
#include "fence.h"

#include "player.h"

#include "checkhit.h"
/*
���V���̍�̏���

mesh fence
*/

//=============================================================================
// �}�N����`
#define TEX_FILE_FENCE000	("data/TEXTURE/fence000.png")					// �΂̃e�N�X�`���[
#define MODEL000			("data/MODEL/fence000.x")						// �΂̃��f��
#define MODELHIT000			("data/MODEL/fence_hitbox000.x")				// �΂̃��f��
#define	VALUE_MOVE			(0.5f)											// �ړ���
#define	ROT_VALUE			(D3DX_PI * 0.02f)								// ��]��
#define SCL_VALUE			(1.0f)											// �傫��
#define RANGE_VALUE			(50)											// ������̋���


enum
{
	TEX_FENCE000 = 0,
	TEX_MAX,
};


//=============================================================================
// �v���g�^�C�v�錾
void MoveFence(void);
void SetFencePos(int no);

//=============================================================================
// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		D3DTextureFence[TEX_MAX] = { NULL };		// �e�N�X�`���ւ̃|�C���^

FENCE					fenceWk[FENCE_MAX];			// �t�B�[���h�ւ̃|�C���^


//=============================================================================
// ����������
HRESULT InitFence(void)
{
	FENCE *fence = &fenceWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̐ݒ�
	D3DXCreateTextureFromFile(pDevice, TEX_FILE_FENCE000, &D3DTextureFence[TEX_FENCE000]);

	int i;
	for (i = 0; i < FENCE_MAX; i++, fence++)
	{
		fence->mesh.use = true;

		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		SetFencePos(i);

		// ���f���֌W�̏�����
		fence->mesh.VtxBuff = NULL;
		//MakeVertexFence(pDevice);
		fence->mesh.Mesh = NULL;
		fence->mesh.BuffMat = NULL;
		fence->mesh.NumMat = 0;

		// X�t�@�C���̓ǂݍ���

		if (i % COMP4FENCE == 0)
		{
			if (FAILED(D3DXLoadMeshFromX(MODEL000,	// ���f���f�[�^
				D3DXMESH_SYSTEMMEM,					// �g�p���郁�����̃I�v�V����
				pDevice,							// �f�o�C�X 
				NULL,								// ���g�p
				&fence->mesh.BuffMat,				// �}�e���A���f�[�^�ւ̃|�C���^�[
				NULL,								// ���g�p
				&fence->mesh.NumMat,				// D3DXMATERIAL�\���̂̐�
				&fence->mesh.Mesh)))				// ���b�V���f�[�^�ւ̃|�C���^�[
			{
				return E_FAIL;
			}
			fence->isHitbox = false;
		}
		else if (i% COMP4FENCE == 1)
		{
			if (FAILED(D3DXLoadMeshFromX(MODELHIT000,// ���f���f�[�^
				D3DXMESH_SYSTEMMEM,					// �g�p���郁�����̃I�v�V����
				pDevice,							// �f�o�C�X 
				NULL,								// ���g�p
				&fence->mesh.BuffMat,				// �}�e���A���f�[�^�ւ̃|�C���^�[
				NULL,								// ���g�p
				&fence->mesh.NumMat,				// D3DXMATERIAL�\���̂̐�
				&fence->mesh.Mesh)))				// ���b�V���f�[�^�ւ̃|�C���^�[
			{
				return E_FAIL;
			}
			fence->isHitbox = true;

			int			VtxCnt = fence->mesh.Mesh->GetNumVertices();	// ���V���̒��_��
			int			Polycnt = fence->mesh.Mesh->GetNumFaces();		// ���V���̃|���S����
			int			BytePerVtx = fence->mesh.Mesh->GetNumBytesPerVertex();	// �P���_������̃o�C�g��
			WORD		*idx;
			char		*pVtx;

			// �|���S���� * 3 �����C���f�b�N�X���K�v	
			// �C���f�b�N�X�ЂƂ̃T�C�Y��WORD�^ = 2�o�C�g�Ȃ̂�,
			// �C���f�b�N�X���i�[����z���(�|���S���� * 3���_ * 2�o�C�g)���������I�m��
			// �I������ free(fence->mesh.IdxBuff); �Ń������J����Y�ꂸ��
			fence->mesh.IdxBuff = (WORD*)malloc(sizeof(WORD) * Polycnt * 3);

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			fence->mesh.Mesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&idx);
			// �C���f�b�N�X�o�t�@�̏����擾
			for (int i = 0; i < (Polycnt * 3); i++)
			{
				fence->mesh.IdxBuff[i] = idx[i];
			}

			// ���_�f�[�^���A�����b�N����
			fence->mesh.Mesh->UnlockIndexBuffer();


			// ���_���W�ЂƂ̃T�C�Y��D3DXVECTOR3�^ = 12�o�C�g�Ȃ̂�,
			// ���_���W���i�[����z���(���_�� * 12�o�C�g)���������I�m��
			// �I������ free(fence->mesh.VtxPos); �Ń������J����Y�ꂸ��
			fence->mesh.VtxPos = (D3DXVECTOR3*)malloc(sizeof(D3DXVECTOR3) * VtxCnt);

			// �o�[�e�b�N�X�f�[�^�����b�N
			fence->mesh.Mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtx);

			// �o�[�e�b�N�X�f�[�^���擾
			for (int i = 0; i < VtxCnt; i++)
			{
				fence->mesh.VtxPos[i] = *(D3DXVECTOR3*)&pVtx[i * BytePerVtx];
			}

			// ���_�f�[�^���A�����b�N����
			fence->mesh.Mesh->UnlockVertexBuffer();
		}
	}
	return S_OK;

}

//=============================================================================
// �I������
void UninitFence(void)
{
	FENCE *fence = &fenceWk[0];
	for (int i = 0; i < TEX_MAX; i++)
	{
		if (D3DTextureFence[i] != NULL)
		{// �e�N�X�`���̊J��
			D3DTextureFence[i]->Release();
			D3DTextureFence[i] = NULL;
		}
	}
	for (int i = 0; i < FENCE_MAX; i++,fence++)
	{
		if (fence->mesh.VtxBuff != NULL)
		{// ���_�o�b�t�@�̊J��
			fence->mesh.VtxBuff->Release();
			fence->mesh.VtxBuff = NULL;
		}

		if (fence->mesh.Mesh != NULL)
		{// ���V���̉��
			fence->mesh.Mesh->Release();
			fence->mesh.Mesh = NULL;
		}

		if (fence->mesh.IdxBuff != NULL)
		{// �C���f�b�N�X�o�b�t�@�ƃ��@�[�e�b�N�X�|�X�͈ꏏ�ɊJ��
			free(fence->mesh.IdxBuff);
			free(fence->mesh.VtxPos);
			fence->mesh.IdxBuff = NULL;
			fence->mesh.VtxPos = NULL;
		}

	}
}

//=============================================================================
// �X�V����
void UpdateFence(void)
{
	FENCE *fence = &fenceWk[0];
	PLAYER *player = GetPlayer(0);
	D3DXVECTOR3 Vel = { 0.0f,0.0f,0.0f };
	int i;
	for (i = 0; i < FENCE_MAX; i++, fence++)
	{

	}
}

//=============================================================================
// �`�揈��
void DrawFence(void)
{
	FENCE *fence = &fenceWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// �X�P�[���A��]�A���s�ړ�
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	int i, j;

	for (i = 0; i < FENCE_MAX; i++, fence++)
		if (fence->mesh.use == true)
		{
			// ���[���h�}�g���N�X������
			D3DXMatrixIdentity(&fence->mesh.mtxWorld);

			// �X�P�[���𔽉f	(S)
			D3DXMatrixScaling(&mtxScl, fence->mesh.Scl.x, fence->mesh.Scl.y, fence->mesh.Scl.z);
			D3DXMatrixMultiply(&fence->mesh.mtxWorld, &fence->mesh.mtxWorld, &mtxScl); 	// &mesh->mtxWorld = &mesh->mtxWorld * &mtxScl
																						// ��]�𔽉f		(R)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, fence->mesh.Rot.y, fence->mesh.Rot.x, fence->mesh.Rot.z);
			D3DXMatrixMultiply(&fence->mesh.mtxWorld, &fence->mesh.mtxWorld, &mtxRot);	// &mesh->mtxWorld = &mesh->mtxWorld * &mtxRot
																						// ���s�ړ��𔽉f	(T)
			D3DXMatrixTranslation(&mtxTranslate, fence->mesh.Pos.x, fence->mesh.Pos.y, fence->mesh.Pos.z);
			D3DXMatrixMultiply(&fence->mesh.mtxWorld, &fence->mesh.mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &fence->mesh.mtxWorld);

			// ���݂̃}�e���A����ۑ�
			pDevice->GetMaterial(&matDef);

			// �}�e���A�����ւ̃|�C���^��
			pD3DXMat = (D3DXMATERIAL*)fence->mesh.BuffMat->GetBufferPointer();

			if (i % 2 == 0)
				for (j = 0; j < (int)fence->mesh.NumMat; j++)
				{
					pDevice->SetMaterial(&pD3DXMat[j].MatD3D);	// �}�e���A���̐ݒ�

					pDevice->SetTexture(0, D3DTextureFence[fence->mesh.TexId]);	// �e�N�X�`���̐ݒ�
					fence->mesh.Mesh->DrawSubset(j);
				}

			// �}�e���A���ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
}

//=============================================================================
// ��̈ړ�
void MoveFence(void)
{
	FENCE *fence = &fenceWk[0];
	PLAYER *player = GetPlayer(0);

}

//=============================================================================
// ��̃Q�b�g�֐�
FENCE *GetFence(int no)
{
	return (&fenceWk[no]);
}






int hitCheckFence(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
{
	FENCE *fence = &fenceWk[0];
	int				ans = 0;	// ��悸�������Ă��Ȃ����Z�b�g���Ă���

	D3DXVECTOR3		pos[3];		// �|���S���̖@��
	D3DXVECTOR3		vel = { 0.0f,0.0f,0.0f };


	int PolyCnt = fence->mesh.Mesh->GetNumFaces();

	for (int i = 0; i<PolyCnt; i++)
	{	// �܂��́A�|���S���̒��_�����߂�
		int idx0 = fence->mesh.IdxBuff[i * 3];
		int idx1 = fence->mesh.IdxBuff[i * 3 + 1];
		int idx2 = fence->mesh.IdxBuff[i * 3 + 2];

		// ���_���W�̐ݒ�
		pos[0] = fence->mesh.VtxPos[idx0];
		pos[1] = fence->mesh.VtxPos[idx1];
		pos[2] = fence->mesh.VtxPos[idx2];

		// �������|���S���Ɛ����̓����蔻��
		ans = CheckHitLine(pos[0], pos[2], pos[3], pos0, pos1, &fence->mesh.mtxWorld, &vel);
		if (ans != 0)
		{
			break;
		}


	}

	return(ans);

}

void SetFencePos(int no)
{
	FENCE *fence = &fenceWk[no];

	switch (no)
	{
	case 0: case 1:
		fence->mesh.Pos = D3DXVECTOR3(-580.0f , 454.0f, -1900.0f);
		fence->mesh.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		fence->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);
		break;
	case 2: case 3:
		fence->mesh.Pos = D3DXVECTOR3(-580.0f, 454.0f, -2350.0f);
		fence->mesh.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		fence->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);
		break;
	case 4: case 5:
		fence->mesh.Pos = D3DXVECTOR3(-340.0f , 430.0f, -1800.0f);
		fence->mesh.Rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5, 0.0f);
		fence->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);
		break;
	case 6: case 7:
		fence->mesh.Pos = D3DXVECTOR3(700.0f , 10.0f, -950.0f);
		fence->mesh.Rot = D3DXVECTOR3(0.0f, D3DX_PI * -0.25, 0.0f);
		fence->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);
		break;
	case 8: case 9:
		fence->mesh.Pos = D3DXVECTOR3(550.0f, 10.0f, -800.0f);
		fence->mesh.Rot = D3DXVECTOR3(0.0f, D3DX_PI * -0.25, 0.0f);
		fence->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);
		break;
	case 10: case 11:
		fence->mesh.Pos = D3DXVECTOR3(-600.0f , 0.0f, 50.0f);
		fence->mesh.Rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5, 0.0f);
		fence->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);
		break;
	case 12: case 13:
		fence->mesh.Pos = D3DXVECTOR3(-345.0f , 0.0f, 155.0f);
		fence->mesh.Rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.25, 0.0f);
		fence->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);
		break;
	case 14: case 15:
		fence->mesh.Pos = D3DXVECTOR3(-200.0f , 0.0f, -100.0f);
		fence->mesh.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		fence->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);
		break;
	case 16: case 17:
		fence->mesh.Pos = D3DXVECTOR3(-200.0f , 0.0f, -100.0f);
		fence->mesh.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		fence->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);
		break;
	case 18: case 19:
		fence->mesh.Pos = D3DXVECTOR3(-200.0f , 0.0f, -100.0f);
		fence->mesh.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		fence->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);
		break;
	case 20: case 21:
		fence->mesh.Pos = D3DXVECTOR3(-200.0f , 0.0f, -100.0f);
		fence->mesh.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		fence->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);
		break;
	}
}