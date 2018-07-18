//=============================================================================
//
// �΂̏��� [rock.cpp]
// Author : ���ꐶ
//
//=============================================================================
#include "rock.h"

#include "camera.h"
#include "player.h"

#include "checkhit.h"
/*
���V���̐΂̏���

mesh rock
*/

//=============================================================================
// �}�N����`
#define TEX_FILE_ROCK000	("data/TEXTURE/rock000.png")				// ��1�̃e�N�X�`���[
#define TEX_FILE_ROCK001	("data/TEXTURE/rock_spire000.png")			// ��2�̃e�N�X�`���[

#define MODEL000			("data/MODEL/rock000.x")					// ��1�̃��f��
#define MODEL001			("data/MODEL/rock_spire000.x")				// ��2�̃��f��

#define MODELHIT000			("data/MODEL/rock_hitbox000.x")				// ��1�̓����蔻��p�̃��f��
#define MODELHIT001			("data/MODEL/rock_spire_hitbox000.x")		// ��2�̓����蔻��p�̃��f��

#define	VALUE_MOVE		(0.5f)											// �ړ���
#define	ROT_VALUE		(D3DX_PI * 0.02f)								// ��]��
#define SCL_VALUE		(1.0f)											// �傫��
#define RANGE_VALUE		(50)											// ������̋���


enum
{
	ROCK000 = 0,
	ROCK001,
	TYPE_MAX,
};


//=============================================================================
// �v���g�^�C�v�錾
void MoveRock(void);

HRESULT SetRockMesh(int no, int typeIdx);

void SetRockPos(int no);
//=============================================================================
// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		D3DTextureRock[TYPE_MAX] = { NULL , NULL };		// �e�N�X�`���ւ̃|�C���^

ROCK					rockWk[ROCK_MAX];			// �t�B�[���h�ւ̃|�C���^


//=============================================================================
// ����������
HRESULT InitRock(void)
{
	ROCK *rock = &rockWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̐ݒ�
	D3DXCreateTextureFromFile(pDevice, TEX_FILE_ROCK000, &D3DTextureRock[ROCK000]);
	D3DXCreateTextureFromFile(pDevice, TEX_FILE_ROCK001, &D3DTextureRock[ROCK001]);

	int i;
	for (i = 0; i < ROCK_MAX; i++, rock++)
	{
		rock->mesh.use = true;

		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		SetRockPos(i);

		// ���f���֌W�̏�����
		rock->mesh.VtxBuff = NULL;
		rock->mesh.Mesh = NULL;
		rock->mesh.BuffMat = NULL;
		rock->mesh.NumMat = 0;

		// X�t�@�C���̓ǂݍ���
		SetRockMesh(i,rock->mesh.TexId);


		if (rock->isHitbox)
		{
			SetVtxData(&rock->mesh);
		}
	}
	return S_OK;

}

//=============================================================================
// �I������
void UninitRock(void)
{
	ROCK *rock = &rockWk[0];
	for (int i = 0; i < TYPE_MAX; i++)
	{// �e�N�X�`���̊J��
		SAFE_RELEASE(D3DTextureRock[i]);
	}
	for (int i = 0; i < ROCK_MAX; i++, rock++)
	{// ���_�o�b�t�@�̊J��
		SAFE_RELEASE(rock->mesh.VtxBuff);

	// ���V���̉��
		SAFE_RELEASE(rock->mesh.Mesh);

		if (rock->mesh.IdxBuff != NULL)
		{// �C���f�b�N�X�o�b�t�@�ƃ��@�[�e�b�N�X�|�X�͈ꏏ�ɊJ��
			free(rock->mesh.IdxBuff);
			free(rock->mesh.VtxPos);
			rock->mesh.IdxBuff = NULL;
			rock->mesh.VtxPos = NULL;
		}

	}
}

//=============================================================================
// �X�V����
void UpdateRock(void)
{
	ROCK *rock = &rockWk[0];
	PLAYER *player = GetPlayer(0);
	D3DXVECTOR3 Vel = { 0.0f,0.0f,0.0f };
	int i;
	for (i = 0; i < ROCK_MAX; i++, rock++)
	{


	}
}

//=============================================================================
// �`�揈��
void DrawRock(void)
{
	ROCK *rock = &rockWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// �X�P�[���A��]�A���s�ړ�
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	int i, j;

	for (i = 0; i < ROCK_MAX; i++, rock++)
		if (rock->mesh.use == true)
		{
			// ���[���h�}�g���N�X������
			D3DXMatrixIdentity(&rock->mesh.mtxWorld);

			// �X�P�[���𔽉f	(S)
			D3DXMatrixScaling(&mtxScl, rock->mesh.Scl.x, rock->mesh.Scl.y, rock->mesh.Scl.z);
			D3DXMatrixMultiply(&rock->mesh.mtxWorld, &rock->mesh.mtxWorld, &mtxScl); 	// &mesh->mtxWorld = &mesh->mtxWorld * &mtxScl
																						// ��]�𔽉f		(R)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, rock->mesh.Rot.y, rock->mesh.Rot.x, rock->mesh.Rot.z);
			D3DXMatrixMultiply(&rock->mesh.mtxWorld, &rock->mesh.mtxWorld, &mtxRot);	// &mesh->mtxWorld = &mesh->mtxWorld * &mtxRot
																						// ���s�ړ��𔽉f	(T)
			D3DXMatrixTranslation(&mtxTranslate, rock->mesh.Pos.x, rock->mesh.Pos.y, rock->mesh.Pos.z);
			D3DXMatrixMultiply(&rock->mesh.mtxWorld, &rock->mesh.mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &rock->mesh.mtxWorld);

			// ���݂̃}�e���A����ۑ�
			pDevice->GetMaterial(&matDef);

			// �}�e���A�����ւ̃|�C���^��
			pD3DXMat = (D3DXMATERIAL*)rock->mesh.BuffMat->GetBufferPointer();

			if (!rock->isHitbox == true)
				for (j = 0; j < (int)rock->mesh.NumMat; j++)
				{
					pDevice->SetMaterial(&pD3DXMat[j].MatD3D);	// �}�e���A���̐ݒ�

					pDevice->SetTexture(0, D3DTextureRock[rock->mesh.TexId]);	// �e�N�X�`���̐ݒ�
					rock->mesh.Mesh->DrawSubset(j);
				}

			// �}�e���A���ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
}

//=============================================================================
// �΂̈ړ�
void MoveRock(void)
{
	ROCK *rock = &rockWk[0];
	PLAYER *player = GetPlayer(0);

}

//=============================================================================
// �΂̃Q�b�g�֐�
ROCK *GetRock(int no)
{
	return (&rockWk[no]);
}

//=============================================================================
// �΂̈ʒu���Z�b�g
void SetRockPos(int no)
{
	ROCK *rock = &rockWk[no];

	switch (no)
	{
	case 0: case 1:
		rock->mesh.Pos = D3DXVECTOR3(3140.0f , 0.0f, 2175.0f);
		rock->mesh.Rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f);
		rock->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);

		rock->mesh.TexId = ROCK001;
		break;
	case 2: case 3:
		rock->mesh.Pos = D3DXVECTOR3(3090.0f , 0.0f, 2260.0f);
		rock->mesh.Rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.1f, 0.0f);
		rock->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);

		rock->mesh.TexId = ROCK001;
		break;
	case 4: case 5:
		rock->mesh.Pos = D3DXVECTOR3(2900.0f , 0.0f, 2760.0f);
		rock->mesh.Rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.2f, 0.0f);
		rock->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);

		rock->mesh.TexId = ROCK001;
		break;
	case 6: case 7:
		rock->mesh.Pos = D3DXVECTOR3(2890.0f , 0.0f, 2655.0f);
		rock->mesh.Rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.3f, 0.0f);
		rock->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);

		rock->mesh.TexId = ROCK001;
		break;
	case 8: case 9:
		rock->mesh.Pos = D3DXVECTOR3(2122.0f , 0.0f, 1055.0f);
		rock->mesh.Rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.4f, 0.0f);
		rock->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);

		rock->mesh.TexId = ROCK000;
		break;
	case 10: case 11:
		rock->mesh.Pos = D3DXVECTOR3(1550.0f, 0.0f, 945.0f);
		rock->mesh.Rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
		rock->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);

		rock->mesh.TexId = ROCK000;
		break;
	case 12: case 13:
		rock->mesh.Pos = D3DXVECTOR3(1630.0f, 0.0f, 1519.0f);
		rock->mesh.Rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.6f, 0.0f);
		rock->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);

		rock->mesh.TexId = ROCK000;
		break;
	case 14: case 15:
		rock->mesh.Pos = D3DXVECTOR3(1200.0f, 0.0f, 550.0f);
		rock->mesh.Rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.7f, 0.0f);
		rock->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);

		rock->mesh.TexId = ROCK000;
		break;
	case 16: case 17:
		rock->mesh.Pos = D3DXVECTOR3(570.0f, 0.0f, 460.0f);
		rock->mesh.Rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.8f, 0.0f);
		rock->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);

		rock->mesh.TexId = ROCK000;
		break;
	case 18: case 19:
		rock->mesh.Pos = D3DXVECTOR3(325.0f, 0.0f, 1000.0f);
		rock->mesh.Rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.9f, 0.0f);
		rock->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);

		rock->mesh.TexId = ROCK000;
		break;
	case 20: case 21:
		rock->mesh.Pos = D3DXVECTOR3(1060.0f, 0.0f, 1375.0f);
		rock->mesh.Rot = D3DXVECTOR3(0.0f, D3DX_PI * 1.0f, 0.0f);
		rock->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);

		rock->mesh.TexId = ROCK000;
		break;
	case 22: case 23:
		rock->mesh.Pos = D3DXVECTOR3(1900.0f, 0.0f, 1550.0f);
		rock->mesh.Rot = D3DXVECTOR3(0.0f, D3DX_PI * 1.1f, 0.0f);
		rock->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);

		rock->mesh.TexId = ROCK000;
		break;
	case 24: case 25:
		rock->mesh.Pos = D3DXVECTOR3(1313.0f, 0.0f, 2375.0f);
		rock->mesh.Rot = D3DXVECTOR3(0.0f, D3DX_PI * 1.2f, 0.0f);
		rock->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);

		rock->mesh.TexId = ROCK000;
		break;
	case 26: case 27:
		rock->mesh.Pos = D3DXVECTOR3(395.0f, 0.0f, 1975.0f);
		rock->mesh.Rot = D3DXVECTOR3(0.0f, D3DX_PI * 1.3f, 0.0f);
		rock->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);

		rock->mesh.TexId = ROCK000;
		break;
	case 28: case 29:
		rock->mesh.Pos = D3DXVECTOR3(950.0f, 0.0f, 1680.0f);
		rock->mesh.Rot = D3DXVECTOR3(0.0f, D3DX_PI * 1.4f, 0.0f);
		rock->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);

		rock->mesh.TexId = ROCK000;
		break;
	}
}



//=============================================================================
// �΂̕`��p�Ɠ����蔻��p���f���쐬
HRESULT SetRockMesh(int no, int typeIdx)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ROCK *rock = &rockWk[no];
	switch (typeIdx)
	{
	case ROCK000:
		if (no % COMP4ROCK == 0)
		{
			if (FAILED(D3DXLoadMeshFromX(MODEL000, D3DXMESH_SYSTEMMEM, pDevice, NULL, &rock->mesh.BuffMat, NULL, &rock->mesh.NumMat, &rock->mesh.Mesh)))
			{
				return E_FAIL;
			}

			rock->isHitbox = false;
		}
		else if (no % COMP4ROCK == 1)
		{
			if (FAILED(D3DXLoadMeshFromX(MODELHIT000, D3DXMESH_SYSTEMMEM, pDevice, NULL, &rock->mesh.BuffMat, NULL, &rock->mesh.NumMat, &rock->mesh.Mesh)))
			{
				return E_FAIL;
			}
			rock->isHitbox = true;
		}
		break;
	case ROCK001:
		if (no % COMP4ROCK == 0)
		{
			if (FAILED(D3DXLoadMeshFromX(MODEL001, D3DXMESH_SYSTEMMEM, pDevice, NULL, &rock->mesh.BuffMat, NULL, &rock->mesh.NumMat, &rock->mesh.Mesh)))
			{
				return E_FAIL;
			}
			rock->isHitbox = false;
		}
		else if (no % COMP4ROCK == 1)
		{
			if (FAILED(D3DXLoadMeshFromX(MODELHIT001, D3DXMESH_SYSTEMMEM, pDevice, NULL, &rock->mesh.BuffMat, NULL, &rock->mesh.NumMat, &rock->mesh.Mesh)))
			{
				return E_FAIL;
			}
			rock->isHitbox = true;
		}
		break;
	}


}