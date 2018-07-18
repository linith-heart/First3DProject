//=============================================================================
//
// �������̏��� [waypoint.cpp]
// Author : ���ꐶ
/*
waypoint.cpp �v���C���[����ړI�n�̕����������Ă������

displays the direction from player to objective
*/
//=============================================================================

//===============================================
// �`����	character
#include "player.h"
//===============================================
// ��		environment
#include "camera.h"

//===============================================
// UI
#include "waypoint.h"
//===============================================
// ���̑�	etc


//=============================================================================
// �}�N����`
#define TEXTURE			("data/TEXTURE/waypoint000.png")
#define MODEL			("data/MODEL/waypoint001.x")
#define SCL_VALUE		(1.0f)

//=============================================================================
// �v���g�^�C�v�錾

//=============================================================================
// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		D3DTextureWayp = NULL;	// �e�N�X�`���ւ̃|�C���^

WAYP		waypWk;

//=============================================================================
// �������̏���������
HRESULT InitWaypoint(void)
{
	WAYP *wayp = &waypWk;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	wayp->use = false;

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	wayp->Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	wayp->Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	wayp->Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);

	// ���f���֌W�̏�����
	//D3DXCreateTextureFromFile(pDevice, TEXTURE, &D3DTextureWayp);
	D3DTextureWayp = NULL;
	wayp->Mesh = NULL;
	wayp->BuffMat = NULL;
	wayp->NumMat = 0;

	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(MODEL,			// ���f���f�[�^
		D3DXMESH_SYSTEMMEM,				// �g�p���郁�����̃I�v�V����
		pDevice,						// �f�o�C�X 
		NULL,							// ���g�p
		&wayp->BuffMat,				// �}�e���A���f�[�^�ւ̃|�C���^�[
		NULL,							// ���g�p
		&wayp->NumMat,				// D3DXMATERIAL�\���̂̐�
		&wayp->Mesh)))				// ���b�V���f�[�^�ւ̃|�C���^�[
	{
		return E_FAIL;
	}
	return S_OK;
}

//=============================================================================
// �������̏I������
void UninitWyapoint(void)
{
	WAYP *wayp = &waypWk;

	if (D3DTextureWayp != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureWayp->Release();
		D3DTextureWayp = NULL;
	}

	if (wayp->VtxBuff != NULL)
	{// ���_�o�b�t�@�̊J��
		wayp->VtxBuff->Release();
		wayp->VtxBuff = NULL;
	}

}

//=============================================================================
// �������̍ŐV����
void UpdateWaypoint(void)
{

}

//=============================================================================
// �������̕`�揈��
void DrawWaypoint(void)
{
	WAYP *wayp = &waypWk;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// �X�P�[���A��]�A���s�ړ�
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	int i, j;

	if (wayp->use == true)
	{
		// ���[���h�}�g���N�X������
		D3DXMatrixIdentity(&wayp->mtxWorld);

		// �X�P�[���𔽉f	(S)
		D3DXMatrixScaling(&mtxScl, wayp->Scl.x, wayp->Scl.y, wayp->Scl.z);
		D3DXMatrixMultiply(&wayp->mtxWorld, &wayp->mtxWorld, &mtxScl); 	// &mesh->mtxWorld = &mesh->mtxWorld * &mtxScl
																					// ��]�𔽉f		(R)
		D3DXMatrixRotationYawPitchRoll(&mtxRot, wayp->Rot.y, wayp->Rot.x, wayp->Rot.z);
		D3DXMatrixMultiply(&wayp->mtxWorld, &wayp->mtxWorld, &mtxRot);	// &mesh->mtxWorld = &mesh->mtxWorld * &mtxRot
																					// ���s�ړ��𔽉f	(T)
		D3DXMatrixTranslation(&mtxTranslate, wayp->Pos.x, wayp->Pos.y, wayp->Pos.z);
		D3DXMatrixMultiply(&wayp->mtxWorld, &wayp->mtxWorld, &mtxTranslate);

		// ���[���h�}�g���b�N�X�ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &wayp->mtxWorld);

		// ���݂̃}�e���A����ۑ�
		pDevice->GetMaterial(&matDef);

		// �}�e���A�����ւ̃|�C���^��
		pD3DXMat = (D3DXMATERIAL*)wayp->BuffMat->GetBufferPointer();


		for (j = 0; j < (int)wayp->NumMat; j++)
		{
			pDevice->SetMaterial(&pD3DXMat[j].MatD3D);	// �}�e���A���̐ݒ�

			pDevice->SetTexture(0, D3DTextureWayp);	// �e�N�X�`���̐ݒ�

			wayp->Mesh->DrawSubset(j);
		}

		// �}�e���A���ɖ߂�
		pDevice->SetMaterial(&matDef);
	}
}

//=============================================================================
// �������̃Q�b�g�֐�
WAYP *GetWaypoint(void)
{
	return(&waypWk);
}