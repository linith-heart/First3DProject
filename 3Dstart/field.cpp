//=============================================================================
//
// �n�ʏ��� [field.cpp]
// Author : ���ꐶ
//
//=============================================================================
#include "field.h"
#include "input.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_FIELD	"data/TEXTURE/field000.jpg"						// �ǂݍ��ރe�N�X�`���t�@�C����	name of the teture file

#define	VALUE_MOVE		(0.5f)											// �ړ���						moving distance
#define	VALUE_ROTATE	(D3DX_PI * 0.02f)								// ��]��						rotation amount

#define	SIZE_X			(50.0f)										// �n�ʂ̃T�C�Y(X����)			size of the x axis
#define	SIZE_Z			(50.0f)										// �n�ʂ̃T�C�Y(Z����)			size of the y axis



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void GetKeyboardMove(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureField = NULL;	// �e�N�X�`���ւ̃|�C���^			pointer for texture
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffField = NULL;	// ���_�o�b�t�@�ւ̃|�C���^		pointer for vertex buffer
LPDIRECT3DINDEXBUFFER9 g_pD3DIdxBuffField;		// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

FIELD					fieldWk[FIELD_MAX];			// �t�B�[���h�ւ̃|�C���^		pointer for field



//D3DXMATRIX g_mtxWorldField;						// ���[���h�}�g���b�N�X
//D3DXVECTOR3 g_posField;							// �|���S���\���ʒu�̒��S���W
//D3DXVECTOR3 g_rotField;							// �|���S���̉�]�p




//=============================================================================
// ����������
//=============================================================================
HRESULT InitField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int BlkCntX, int BlkCntZ, float BlkSizeX, float BlkSizeZ)
{
	FIELD *field = &fieldWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	field->Pos = pos;
	field->Rot = rot;

	D3DXCreateTextureFromFile(pDevice, TEXTURE_FIELD, &D3DTextureField);

	// �u���b�N���̐ݒ�
	field->BlkCntX = BlkCntX;
	field->BlkCntZ = BlkCntZ;

	// ���_���̐ݒ�
	field->Vtx = (BlkCntX + 1) * (BlkCntZ + 1);

	// �C���f�b�N�X���̐ݒ�
	field->VtxIdx = (BlkCntX + 1) * 2 * BlkCntZ + (BlkCntZ - 1) * 2;

	// �|���S�����̐ݒ�
	field->PolyCnt = BlkCntX * BlkCntZ * 2 + (BlkCntZ - 1) * 4;	

	// �u���b�N�T�C�Y�̐ݒ�
	field->BlkSizeX = BlkSizeX;
	field->BlkSizeZ = BlkSizeZ;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * field->Vtx,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,						// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffField,				// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))								// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	// �I�u�W�F�N�g�̃C���f�b�N�X�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * field->VtxIdx,		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
		D3DFMT_INDEX16,						// �g�p����C���f�b�N�X�t�H�[�}�b�g
		D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DIdxBuffField,				// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))								// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

#if 0
	const float texSizeX = 1.0f / g_nNumBlockX;
	const float texSizeZ = 1.0f / g_nNumBlockZ;
#else
	const float texSizeX = 1.0f;
	const float texSizeZ = 1.0f;
#endif

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pD3DVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtxZ = 0; nCntVtxZ < (field->BlkCntZ + 1); nCntVtxZ++)
	{
		for (int nCntVtxX = 0; nCntVtxX < (field->BlkCntX + 1); nCntVtxX++)
		{
			// ���_���W�̐ݒ�
			pVtx[nCntVtxZ * (field->BlkCntX + 1) + nCntVtxX].vtx.x = -(field->BlkCntX / 2.0f) * field->BlkSizeX + nCntVtxX * field->BlkSizeX;
			pVtx[nCntVtxZ * (field->BlkCntX + 1) + nCntVtxX].vtx.y = 0.0f;
			pVtx[nCntVtxZ * (field->BlkCntX + 1) + nCntVtxX].vtx.z = (field->BlkCntZ / 2.0f) * field->BlkSizeZ - nCntVtxZ * field->BlkSizeZ;

			// �@���̐ݒ�
			pVtx[nCntVtxZ * (field->BlkCntX + 1) + nCntVtxX].normal = D3DXVECTOR3(0.0f, 1.0, 0.0f);

			// ���ˌ��̐ݒ�
			pVtx[nCntVtxZ * (field->BlkCntX + 1) + nCntVtxX].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[nCntVtxZ * (field->BlkCntX + 1) + nCntVtxX].tex.x = texSizeX * nCntVtxX;
			pVtx[nCntVtxZ * (field->BlkCntX + 1) + nCntVtxX].tex.y = texSizeZ * nCntVtxZ;
		}
	}

	// ���_�f�[�^���A�����b�N����
	g_pD3DVtxBuffField->Unlock();
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
	WORD *pIdx;

	// �C���f�b�N�X�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pD3DIdxBuffField->Lock(0, 0, (void**)&pIdx, 0);

	int nCntIdx = 0;
	for (int nCntVtxZ = 0; nCntVtxZ < field->BlkCntZ; nCntVtxZ++)
	{
		if (nCntVtxZ > 0)
		{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
			pIdx[nCntIdx] = (nCntVtxZ + 1) * (field->BlkCntX + 1);
			nCntIdx++;
		}

		for (int nCntVtxX = 0; nCntVtxX < (field->BlkCntX + 1); nCntVtxX++)
		{
			pIdx[nCntIdx] = (nCntVtxZ + 1) * (field->BlkCntX + 1) + nCntVtxX;
			nCntIdx++;
			pIdx[nCntIdx] = nCntVtxZ * (field->BlkCntX + 1) + nCntVtxX;
			nCntIdx++;
		}

		if (nCntVtxZ < (field->BlkCntZ - 1))
		{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
			pIdx[nCntIdx] = nCntVtxZ * (field->BlkCntX + 1) + field->BlkCntX;
			nCntIdx++;
		}
	}

	// �C���f�b�N�X�f�[�^���A�����b�N����
	g_pD3DIdxBuffField->Unlock();
	}


	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitField(void)
{
	if(D3DTextureField != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureField->Release();
		D3DTextureField = NULL;
	}

	if(g_pD3DVtxBuffField != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffField->Release();
		g_pD3DVtxBuffField = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateField(void)
{



}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawField(void)
{
	FIELD *field = &fieldWk[0];
	CAMERA *camera = GetCamera(0);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;	// ��]�A���s�ړ�
	int i;
	for (i = 0; i < FIELD_MAX; i++, field++)
	{
		// ���[���h�}�g���N�X������
		D3DXMatrixIdentity(&field->mtxWorld);


		// ��]�𔽉f		(R)
		D3DXMatrixRotationYawPitchRoll(&mtxRot, field->Rot.y, field->Rot.x, field->Rot.z);
		D3DXMatrixMultiply(&field->mtxWorld, &field->mtxWorld, &mtxRot);	// &field->mtxWorld = &field->mtxWorld * &mtxRot

		// ���s�ړ��𔽉f	(T)
		D3DXMatrixTranslation(&mtxTranslate, field->Pos.x, field->Pos.y, field->Pos.z);
		D3DXMatrixMultiply(&field->mtxWorld, &field->mtxWorld, &mtxTranslate);

		// ���[���h�}�g���b�N�X�ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &field->mtxWorld);

		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_pD3DVtxBuffField, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �N�X�`���̐ݒ�
		pDevice->SetTexture(0, D3DTextureField);

		// �|���S���̕`��
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, field->Vtx, 0, NUM_POLYGON);
	}
}

void GetKeyboardMove(void)
{
	FIELD *field = &fieldWk[0];
}

void *GetField(int no)
{
	return (&fieldWk[no]);
}