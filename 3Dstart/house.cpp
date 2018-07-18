//=============================================================================
//
// �ԏ��� [house.cpp]
// Author : ���ꐶ
//
//=============================================================================
#include "camera.h"
#include "fade.h"
#include "player.h"
#include "pig.h"
#include "house.h"

/*
�G�l�~�[�i�ԁj
�O��Ɖ�]�ňړ�

meshmy(house)
moves forward and back 
rotates to face player
*/

//=============================================================================
// �}�N����`
#define TEXTURE			("data/TEXTURE/cave_entrance000.png")
#define MODEL			("data/MODEL/cave_entrance000.x")
#define	VALUE_MOVE		(0.5f)											// �ړ���
#define	ROT_VALUE		(D3DX_PI * 0.02f)								// ��]��
#define SCL_VALUE		(1.0f)

#define RANGE			(200.0f)

#define POS_X			(3700.0f)
#define POS_Y			(0.0f)
#define POS_Z			(3580.0f)


//=============================================================================
// �v���g�^�C�v�錾
void MoveHouse(void);

//=============================================================================
// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		D3DTextureHouse = NULL;	// �e�N�X�`���ւ̃|�C���^

HOUSE						houseWk[HOUSE_MAX];			// �t�B�[���h�ւ̃|�C���^

//=============================================================================
// ����������
HRESULT InitHouse(void)
{
	HOUSE *house = &houseWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̐ݒ�
	D3DXCreateTextureFromFile(pDevice, TEXTURE, &D3DTextureHouse);

	int i;
	for (i = 0; i < HOUSE_MAX; i++, house++)
	{
		house->mesh.use = true;


		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		house->mesh.Pos = D3DXVECTOR3(POS_X, POS_Y, POS_Z);
		house->mesh.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		house->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);


		// ���f���֌W�̏�����
		house->mesh.TexId = 0;
		//D3DTextureHouse = NULL;
		house->mesh.Mesh = NULL;
		house->mesh.BuffMat = NULL;
		house->mesh.NumMat= 0;

		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(MODEL,			// ���f���f�[�^
			D3DXMESH_SYSTEMMEM,				// �g�p���郁�����̃I�v�V����
			pDevice,						// �f�o�C�X 
			NULL,							// ���g�p
			&house->mesh.BuffMat,				// �}�e���A���f�[�^�ւ̃|�C���^�[
			NULL,							// ���g�p
			&house->mesh.NumMat,				// D3DXMATERIAL�\���̂̐�
			&house->mesh.Mesh)))				// ���b�V���f�[�^�ւ̃|�C���^�[
		{
			return E_FAIL;
		}
	}
	return S_OK;

}

//=============================================================================
// �I������
void UninitHouse(void)
{
	HOUSE *house = &houseWk[0];

	if (D3DTextureHouse != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureHouse->Release();
		D3DTextureHouse = NULL;
	}
	for (int i = 0; i < HOUSE_MAX; i++, house++)
	{
		if (house->mesh.VtxBuff != NULL)
		{// ���_�o�b�t�@�̊J��
			house->mesh.VtxBuff->Release();
			house->mesh.VtxBuff = NULL;
		}
		if (house->mesh.Mesh != NULL)
		{// ���V���̉��
			house->mesh.Mesh->Release();
			house->mesh.Mesh = NULL;
		}

	}
}

//=============================================================================
// �X�V����
void UpdateHouse(void)
{
	HOUSE *house = &houseWk[0];
	PLAYER *player = GetPlayer(0);
	PIG *pig = GetPig(0);
	D3DXVECTOR3 Vel = { 0.0f,0.0f,0.0f };
	int i;
	for (i = 0; i < HOUSE_MAX; i++, house++)
	{
		// �����؂�A��Ă����ԂŉƂɂ���������
		D3DXVECTOR3 temp = player->Pos - house->mesh.Pos;
		float tLen = D3DXVec3Length(&temp);
		if (tLen < RANGE)
		{

			D3DXVECTOR3 temp = house->mesh.Pos - pig->npc.Pos;


			SetFade(FADE_OUT);

		}



	}
}

//=============================================================================
// �`�揈��
void DrawHouse(void)
{
	HOUSE *house = &houseWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// �X�P�[���A��]�A���s�ړ�
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	int i, j;

	for (i = 0; i < HOUSE_MAX; i++, house++)
		if (house->mesh.use == true)
		{
			// ���[���h�}�g���N�X������
			D3DXMatrixIdentity(&house->mesh.mtxWorld);

			// �X�P�[���𔽉f	(S)
			D3DXMatrixScaling(&mtxScl, house->mesh.Scl.x, house->mesh.Scl.y, house->mesh.Scl.z);
			D3DXMatrixMultiply(&house->mesh.mtxWorld, &house->mesh.mtxWorld, &mtxScl); 	// &mesh->mtxWorld = &mesh->mtxWorld * &mtxScl
			// ��]�𔽉f		(R)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, house->mesh.Rot.y, house->mesh.Rot.x, house->mesh.Rot.z);
			D3DXMatrixMultiply(&house->mesh.mtxWorld, &house->mesh.mtxWorld, &mtxRot);	// &mesh->mtxWorld = &mesh->mtxWorld * &mtxRot
			// ���s�ړ��𔽉f	(T)
			D3DXMatrixTranslation(&mtxTranslate, house->mesh.Pos.x, house->mesh.Pos.y, house->mesh.Pos.z);
			D3DXMatrixMultiply(&house->mesh.mtxWorld, &house->mesh.mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &house->mesh.mtxWorld);

			// ���݂̃}�e���A����ۑ�
			pDevice->GetMaterial(&matDef);

			// �}�e���A�����ւ̃|�C���^��
			pD3DXMat = (D3DXMATERIAL*)house->mesh.BuffMat->GetBufferPointer();


			for (j = 0; j < (int)house->mesh.NumMat; j++)
			{
				pDevice->SetMaterial(&pD3DXMat[j].MatD3D);	// �}�e���A���̐ݒ�

				pDevice->SetTexture(0, D3DTextureHouse);	// �e�N�X�`���̐ݒ�

				house->mesh.Mesh->DrawSubset(j);
			}

			// �}�e���A���ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
}

//=============================================================================
// �Ԃ̈ړ�
void MoveHouse(void)
{
	HOUSE *house = &houseWk[0];
	PLAYER *player = GetPlayer(0);

}

//=============================================================================
// �Ԃ̃Q�b�g�֐�
HOUSE *GetHouse(int no)
{
	return (&houseWk[no]);
}