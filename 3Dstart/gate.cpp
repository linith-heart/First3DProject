//=============================================================================
//
// �Q�[�g�̏��� [gate.cpp]
// Author : ���ꐶ
//
//=============================================================================
#include "gate.h"

#include "player.h"
#include "gem.h"

#include "checkhit.h"

#include "mathWk.h"
/*
�Q�[�g�̖�̏���
�}�b�v��̃W�F�������ׂďW�߂�ƊJ��

mesh gate
opens once all gems are collected
*/

//=============================================================================
// �}�N����`
#define MODEL_GATE		("data/MODEL/gate000.x")							// ��̃��f��
#define TEXTURE_GATE	("data/TEXTURE/gate000.png")						// ��̃e�N�X�`���[
#define MODEL_GATE_HIT	("data/MODEL/gate_hitbox000.x")						// ��̃��f��

#define MODEL_DOOR		("data/MODEL/gate_door000.x")						// �h�A�̃��f��
#define TEXTURE_DOOR	("data/TEXTURE/gate_door000.png")					// �h�A�̃e�N�X�`���[
#define MODEL_DOOR_HIT	("data/MODEL/gate_door_hit_box.x")				// �h�A�̃��f��

#define DOOR_DISTANCE	(120 * SCL_VALUE)									// �傩��̃h�A�̈ʒu�܂ł̋���
#define DOOR_OPEN_ANIMATION_TIME	(30)									// �h�A���J���A�j���[�V�����̎���

#define	VALUE_MOVE		(0.5f)												// �ړ���
#define	ROT_VALUE		(D3DX_PI * 0.02f)									// ��]��
#define SCL_VALUE		(1.0f)												// �傫��
#define STARTING_ROT	(D3DX_PI * -0.15f)
#define RANGE_VALUE		(50)												// ������̋���


enum
{
	TEX_GATE = 0,
	TEX_DOOR,
	TEX_MAX,
};

//=============================================================================
// �v���g�^�C�v�錾
void MoveGate(int no);

//=============================================================================
// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		D3DTextureGate[TEX_MAX] = { NULL , NULL };		// �e�N�X�`���ւ̃|�C���^

GATE						gateWk[GATE_MAX];			// �t�B�[���h�ւ̃|�C���^

bool						isOpn = false;
int							OpnAnimCnt = DOOR_OPEN_ANIMATION_TIME;

//=============================================================================
// ����������
HRESULT InitGate(void)
{
	GATE *gate = &gateWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̐ݒ�
	D3DXCreateTextureFromFile(pDevice, TEXTURE_GATE, &D3DTextureGate[TEX_GATE]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_DOOR, &D3DTextureGate[TEX_DOOR]);

	int i;
	for (i = 0; i < GATE_MAX; i ++, gate ++)
	{
		gate->mesh.use = true;

		gate->mesh.Pos = D3DXVECTOR3(3000.0f, 0.0f, 2450.0f);

		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		gate->mesh.Rot = D3DXVECTOR3(0.0f, STARTING_ROT, 0.0f);
		gate->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);

		// ���f���֌W�̏�����
		gate->mesh.VtxBuff = NULL;
		//MakeVertexGate(pDevice);
		gate->mesh.Mesh = NULL;
		gate->mesh.BuffMat = NULL;
		gate->mesh.NumMat = 0;

		D3DXVECTOR3 temp;
		switch (i % COMP4GATE)
		{
		case 0: // ��
			if (FAILED(D3DXLoadMeshFromX(MODEL_GATE, D3DXMESH_SYSTEMMEM, pDevice, NULL, &gate->mesh.BuffMat, NULL, &gate->mesh.NumMat, &gate->mesh.Mesh)))
				return E_FAIL;
			gate->mesh.TexId = TEX_GATE;

			gate->isHitbox = false;
			break;
		case 1: // ��̓����蔻��p
			if (FAILED(D3DXLoadMeshFromX(MODEL_GATE_HIT, D3DXMESH_SYSTEMMEM, pDevice, NULL, &gate->mesh.BuffMat, NULL, &gate->mesh.NumMat, &gate->mesh.Mesh)))
				return E_FAIL;
			gate->isHitbox = true;

			SetVtxData(&gate->mesh);
			break;
		case 2: // �Е��̃h�A
			if (FAILED(D3DXLoadMeshFromX(MODEL_DOOR, D3DXMESH_SYSTEMMEM, pDevice, NULL, &gate->mesh.BuffMat, NULL, &gate->mesh.NumMat, &gate->mesh.Mesh)))
				return E_FAIL;
			gate->mesh.TexId = TEX_DOOR;

			gate->isHitbox = false;
			temp = Ang2Vec(gate->mesh.Rot, DOOR_DISTANCE);
			gate->mesh.Pos -= temp;
			break;
		case 3: // �h�A�̓����蔻��p
			if (FAILED(D3DXLoadMeshFromX(MODEL_DOOR_HIT, D3DXMESH_SYSTEMMEM, pDevice, NULL, &gate->mesh.BuffMat, NULL, &gate->mesh.NumMat, &gate->mesh.Mesh)))
				return E_FAIL;
			gate->isHitbox = true;
			temp = Ang2Vec(gate->mesh.Rot, DOOR_DISTANCE);
			gate->mesh.Pos -= temp;
			SetVtxData(&gate->mesh);
			break;
		case 4: // �����Е��̃h�A
			if (FAILED(D3DXLoadMeshFromX(MODEL_DOOR, D3DXMESH_SYSTEMMEM, pDevice, NULL, &gate->mesh.BuffMat, NULL, &gate->mesh.NumMat, &gate->mesh.Mesh)))
				return E_FAIL;
			gate->mesh.TexId = TEX_DOOR;

			gate->isHitbox = false;

			gate->mesh.Rot.y += (D3DX_PI * 1);
			temp = Ang2Vec(gate->mesh.Rot, DOOR_DISTANCE);
			gate->mesh.Pos -= temp;

			SetVtxData(&gate->mesh);
			break;
		case 5: // �����Е��̃h�A�̓����蔻��p
			if (FAILED(D3DXLoadMeshFromX(MODEL_DOOR_HIT, D3DXMESH_SYSTEMMEM, pDevice, NULL, &gate->mesh.BuffMat, NULL, &gate->mesh.NumMat, &gate->mesh.Mesh)))
				return E_FAIL;
			gate->isHitbox = true;

			gate->mesh.Rot.y += (D3DX_PI * 1);
			temp = Ang2Vec(gate->mesh.Rot, DOOR_DISTANCE);
			gate->mesh.Pos -= temp;

			SetVtxData(&gate->mesh);
			break;
		}
	}
}

//=============================================================================
// ��̃��Z�b�g����
void ResetGate(void)
{
	GATE *gate = &gateWk[0];

	isOpn = false;
	OpnAnimCnt = DOOR_OPEN_ANIMATION_TIME;
	int i;
	for (i = 0; i < GATE_MAX; i++, gate++)
	{
		gate->mesh.use = true;

		gate->mesh.Pos = D3DXVECTOR3(3000.0f, 0.0f, 2450.0f);

		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		gate->mesh.Rot = D3DXVECTOR3(0.0f, STARTING_ROT, 0.0f);
		gate->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);


		D3DXVECTOR3 temp;
		switch (i % COMP4GATE)
		{
		case 0:
			break;
		case 1:
			break;
		case 2:
			temp = Ang2Vec(gate->mesh.Rot, DOOR_DISTANCE);
			gate->mesh.Pos -= temp;
			break;
		case 3:
			temp = Ang2Vec(gate->mesh.Rot, DOOR_DISTANCE);
			gate->mesh.Pos -= temp;
			break;
		case 4:
			gate->mesh.Rot.y += (D3DX_PI * 1);
			temp = Ang2Vec(gate->mesh.Rot, DOOR_DISTANCE);
			gate->mesh.Pos -= temp;
			break;
		case 5:
			gate->mesh.Rot.y += (D3DX_PI * 1);
			temp = Ang2Vec(gate->mesh.Rot, DOOR_DISTANCE);
			gate->mesh.Pos -= temp;
			break;
		}
	}

}

//=============================================================================
// �I������
void UninitGate(void)
{
	GATE *gate = &gateWk[0];
	for (int i = 0; i < TEX_MAX;i++)
		if (D3DTextureGate[i]!= NULL)
		{// �e�N�X�`���̊J��
			D3DTextureGate[i]->Release();
			D3DTextureGate[i] = NULL;
		}
	for (int i = 0; i < GATE_MAX; i++, gate++)
	{
		if (gate->mesh.VtxBuff != NULL)
		{// ���_�o�b�t�@�̊J��
			gate->mesh.VtxBuff->Release();
			gate->mesh.VtxBuff = NULL;
		}

		if (gate->mesh.Mesh != NULL)
		{// ���V���̉��
			gate->mesh.Mesh->Release();
			gate->mesh.Mesh = NULL;
		}

		if (gate->mesh.IdxBuff != NULL)
		{// �C���f�b�N�X�o�b�t�@�ƃ��@�[�e�b�N�X�|�X�͈ꏏ�ɊJ��
			free(gate->mesh.IdxBuff);
			free(gate->mesh.VtxPos);
			gate->mesh.IdxBuff = NULL;
			gate->mesh.VtxPos = NULL;
		}

	}
}



//=============================================================================
// �X�V����
void UpdateGate(void)
{
	GATE *gate = &gateWk[0];
	GEM *gem = GetGem(0);
	D3DXVECTOR3 Vel = { 0.0f,0.0f,0.0f };
	int i;
	int GemCnt = 0;
	for (i = 0; i < GEM_MAX; i++, gem++)
		if (gem->mesh.use == false)
		{
			GemCnt++;
		}
	if (GemCnt >= GEM_MAX)
	{
		isOpn = true;
	}
	for (i = 0; i < GATE_MAX; i++, gate++)
		if (gate->mesh.use
			&& isOpn
			&& OpnAnimCnt > 0)
		{
			switch (i % COMP4GATE)
			{
			case 0:
				break;
			case 1:
				break;
			case 2:
				gate->mesh.Rot.y += ROT_VALUE;
				break;
			case 3:
				gate->mesh.Rot.y += ROT_VALUE;
				break;
			case 4:
				gate->mesh.Rot.y -= ROT_VALUE;
				break;
			case 5:
				gate->mesh.Rot.y -= ROT_VALUE;
				break;
			}

		}
	if (isOpn)
		OpnAnimCnt--;
}


//=============================================================================
// �`�揈��
void DrawGate(void)
{
	GATE *gate = &gateWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// �X�P�[���A��]�A���s�ړ�
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	int i, j;

	for (i = 0; i < GATE_MAX; i++, gate++)
		if (gate->mesh.use == true)
		{
			// ���[���h�}�g���N�X������
			D3DXMatrixIdentity(&gate->mesh.mtxWorld);

			// �X�P�[���𔽉f	(S)
			D3DXMatrixScaling(&mtxScl, gate->mesh.Scl.x, gate->mesh.Scl.y, gate->mesh.Scl.z);
			D3DXMatrixMultiply(&gate->mesh.mtxWorld, &gate->mesh.mtxWorld, &mtxScl); 	// &mesh->mtxWorld = &mesh->mtxWorld * &mtxScl
																					// ��]�𔽉f		(R)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, gate->mesh.Rot.y, gate->mesh.Rot.x, gate->mesh.Rot.z);
			D3DXMatrixMultiply(&gate->mesh.mtxWorld, &gate->mesh.mtxWorld, &mtxRot);	// &mesh->mtxWorld = &mesh->mtxWorld * &mtxRot
																					// ���s�ړ��𔽉f	(T)
			D3DXMatrixTranslation(&mtxTranslate, gate->mesh.Pos.x, gate->mesh.Pos.y, gate->mesh.Pos.z);
			D3DXMatrixMultiply(&gate->mesh.mtxWorld, &gate->mesh.mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &gate->mesh.mtxWorld);

			// ���݂̃}�e���A����ۑ�
			pDevice->GetMaterial(&matDef);

			// �}�e���A�����ւ̃|�C���^��
			pD3DXMat = (D3DXMATERIAL*)gate->mesh.BuffMat->GetBufferPointer();

			if (gate->isHitbox == false)
				for (j = 0; j < (int)gate->mesh.NumMat; j++)
				{
					pDevice->SetMaterial(&pD3DXMat[j].MatD3D);	// �}�e���A���̐ݒ�

					pDevice->SetTexture(0, D3DTextureGate[gate->mesh.TexId]);	// �e�N�X�`���̐ݒ�
					gate->mesh.Mesh->DrawSubset(j);
				}

			// �}�e���A���ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
}

//=============================================================================
// �W�F���̈ړ�����
void MoveGate(int no)
{
	GATE *gate = &gateWk[no];

	gate->mesh.Rot.y += ROT_VALUE;

}


//=============================================================================
// �W�F���̃Q�b�g�֐�
GATE *GetGate(int no)
{
	return (&gateWk[no]);
}
