//=============================================================================
//
// �ԏ��� [pig.cpp]
// Author : ���ꐶ
//
//=============================================================================
#include "fade.h"
#include "npc.h"
#include "camera.h"
#include "player.h"
#include "house.h"
#include "pig.h"
#include "boar.h"

/*
�G�l�~�[�i�ԁj
�O��Ɖ�]�ňړ�

npcmy(pig)
moves forward and back 
rotates to face player
*/

//=============================================================================
// �}�N����`
#define TEXTURE			("data/TEXTURE/pig_map.jpg")
#define MODEL			("data/MODEL/pig000.x")
#define	VEL_MAX			(3.0f)							// �ړ���
#define	ROT_VALUE		(D3DX_PI * 0.02f)				// ��]��
#define SCL_VALUE		(0.7f)
#define RANG_RAT		(75)							// �v���C���[�܂ł̋����̗�		range ratio from pig to player
#define GATHER_RANGE	(100)

#define FRAME_VALUE		(50)


//=============================================================================
// �v���g�^�C�v�錾
void MovePig(void);

//=============================================================================
// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		D3DTexturePig = NULL;	// �e�N�X�`���ւ̃|�C���^		pointer to texture

PIG						pigWk[PIG_MAX];			// �\���̂ւ̃|�C���^			pointer to stuct

int						GOT;					// �Q�[���I�[�o�[�̃^�C�}�[		GameOverTimer



//=============================================================================
// ����������
HRESULT InitPig(void)
{
	PIG *pig = &pigWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int i;
	for (i = 0; i < PIG_MAX; i++, pig++)
	{
		pig->npc.use = true;

		pig->GoHome = false;
		pig->LetOut = false;
		pig->isUpAnim = false;
		pig->fUpAnim = 0.0f;
		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		pig->npc.Pos = D3DXVECTOR3(-1650.0f, 0.0f, -1450.0f);
		pig->npc.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pig->npc.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);

		// ���f���֌W�̏�����
		D3DXCreateTextureFromFile(pDevice, TEXTURE, &D3DTexturePig);
		//D3DTexturePig = NULL;
		pig->npc.Mesh = NULL;
		pig->npc.BuffMat = NULL;
		pig->npc.NumMat= 0;

		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(MODEL,		// ���f���f�[�^
			D3DXMESH_SYSTEMMEM,					// �g�p���郁�����̃I�v�V����
			pDevice,							// �f�o�C�X 
			NULL,								// ���g�p
			&pig->npc.BuffMat,					// �}�e���A���f�[�^�ւ̃|�C���^�[
			NULL,								// ���g�p
			&pig->npc.NumMat,					// D3DXMATERIAL�\���̂̐�
			&pig->npc.Mesh)))					// ���b�V���f�[�^�ւ̃|�C���^�[
		{
			return E_FAIL;
		}
	}
	return S_OK;

}

//=============================================================================
// �I������
void UninitPig(void)
{
	PIG *pig = &pigWk[0];

	if (D3DTexturePig != NULL)
	{// �e�N�X�`���̊J��
		D3DTexturePig->Release();
		D3DTexturePig = NULL;
	}
	for (int i = 0; i < PIG_MAX; i++)
	{
		if (pig->npc.VtxBuff != NULL)
		{// ���_�o�b�t�@�̊J��
			pig->npc.VtxBuff->Release();
			pig->npc.VtxBuff = NULL;
		}
		if (pig->npc.Mesh != NULL)
		{// ���V���̉��
			pig->npc.Mesh->Release();
			pig->npc.Mesh = NULL;
		}
	}
}

//=============================================================================
// �X�V����
void UpdatePig(void)
{
	PIG *pig = &pigWk[0];
	PLAYER *player = GetPlayer(0);
	HOUSE *house = GetHouse(0);
	BOAR *boar = GetBoar(0);
	int i;
	for (i = 0; i < PIG_MAX; i++, pig++)
	{

		pig->npc.Pos = boar->RoamPos;



		//=================================================
		// �����Ă��鎞�̃A�j���[�V����
		if (pig->npc.Vel.x != 0 || pig->npc.Vel.z!= 0)
		{
			if (pig->isUpAnim == true)
			{
				if (pig->fUpAnim < 1.0f)
				{
					pig->fUpAnim += 0.1f;
					pig->npc.Pos.y += pig->fUpAnim;
				}
				else
				{
					pig->fUpAnim += 0.1f;
					pig->isUpAnim = false;
				}
			}
			if (pig->isUpAnim == false)
			{
				if (pig->fUpAnim > 0.1f)
				{
					pig->fUpAnim -= 0.1f;
					pig->npc.Pos.y -= pig->fUpAnim;
				}
				else
				{
					pig->isUpAnim = true;
				}
			}
		}
		else 
		{
			while (pig->fUpAnim > 0.1f)
			{
				pig->fUpAnim -= 0.1f;
				pig->npc.Pos.y -= pig->fUpAnim;
				pig->isUpAnim = true;
			}
		}
	}
}

//=============================================================================
// �`�揈��
void DrawPig(void)
{
	PIG *pig = &pigWk[0];
	CAMERA *camera = GetCamera(0);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// �X�P�[���A��]�A���s�ړ�
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	int i, j;

	for (i = 0; i < PIG_MAX; i++, pig++)
		if (pig->npc.use == true)
		{
			// ���[���h�}�g���N�X������
			D3DXMatrixIdentity(&pig->npc.mtxWorld);

			// �X�P�[���𔽉f	(S)
			D3DXMatrixScaling(&mtxScl, pig->npc.Scl.x, pig->npc.Scl.y, pig->npc.Scl.z);
			D3DXMatrixMultiply(&pig->npc.mtxWorld, &pig->npc.mtxWorld, &mtxScl); 	// &npc->mtxWorld = &npc->mtxWorld * &mtxScl
			// ��]�𔽉f		(R)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pig->npc.Rot.y, pig->npc.Rot.x, pig->npc.Rot.z);
			D3DXMatrixMultiply(&pig->npc.mtxWorld, &pig->npc.mtxWorld, &mtxRot);	// &npc->mtxWorld = &npc->mtxWorld * &mtxRot
			// ���s�ړ��𔽉f	(T)
			D3DXMatrixTranslation(&mtxTranslate, pig->npc.Pos.x, pig->npc.Pos.y, pig->npc.Pos.z);
			D3DXMatrixMultiply(&pig->npc.mtxWorld, &pig->npc.mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &pig->npc.mtxWorld);

			// ���݂̃}�e���A����ۑ�
			pDevice->GetMaterial(&matDef);

			// �}�e���A�����ւ̃|�C���^��
			pD3DXMat = (D3DXMATERIAL*)pig->npc.BuffMat->GetBufferPointer();


			for (j = 0; j < (int)pig->npc.NumMat; j++)
			{
				pDevice->SetMaterial(&pD3DXMat[j].MatD3D);	// �}�e���A���̐ݒ�

				pDevice->SetTexture(0, D3DTexturePig);	// �e�N�X�`���̐ݒ�

				pig->npc.Mesh->DrawSubset(j);
			}

			// �}�e���A���ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
}

//=============================================================================
// �Ԃ̈ړ�
void MovePig(void)
{
	PIG *pig = &pigWk[0];
	PLAYER *player = GetPlayer(0);

}

//=============================================================================
// �Ԃ̃Q�b�g�֐�
PIG *GetPig(int no)
{
	return (&pigWk[no]);
}