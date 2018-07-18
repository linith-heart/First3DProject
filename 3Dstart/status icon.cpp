//=============================================================================
//
// �o���b�g���� [staticoboard.cpp]
// Author : ���ꐶ
//
//=============================================================================
#include "status icon.h"
#include "input.h"
#include "camera.h"
#include "player.h"
#include "shadow.h"
#include "field.h"
#include "mathWk.h"

//=============================================================================
// �}�N����`
#define	TEX_FOUND		"data/TEXTURE/exclamation_mark.png"
#define	TEX_CHECK		"data/TEXTURE/question_mark.png"



#define	ICO_SIZE_X			(50.0f)											// �o���b�g�̃T�C�Y(X����)		size of the x axis
#define	ICO_SIZE_Y			(50.0f)											// �o���b�g�̃T�C�Y(Z����)		size of the y axis

#define	ALPHA				(170)

//=============================================================================
// �v���g�^�C�v�錾
D3DXMATRIX GetInvRotateMatStatIco(void);
HRESULT MakeVertexStatIco(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		D3DTexStatIco[STAT_ICO_MAX] = { NULL ,NULL };		// �e�N�X�`���ւ̃|�C���^		pointer for texture
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffStatIco = NULL;	// ���_�o�b�t�@�ւ̃|�C���^		pointer for vertex buffer

STAT_ICO					staticoWk[STAT_ICO_MAX];			// �t�B�[���h�ւ̃|�C���^


//=============================================================================
// �o���b�g����������
HRESULT InitStatIco(void)
{
	STAT_ICO *statico = &staticoWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int i;

	D3DXCreateTextureFromFile(pDevice, TEX_FOUND, &D3DTexStatIco[Idx_FOUND]);
	D3DXCreateTextureFromFile(pDevice, TEX_CHECK, &D3DTexStatIco[Idx_CHECK]);

	for (i = 0; i < STAT_ICO_MAX; i++, statico++)
	{
		statico->use = false;

		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		statico->Pos = VEC3CLEAR;
		statico->Scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

		statico->TexID = 0;

		MakeVertexStatIco(pDevice);

	}
	return S_OK;
}

//=============================================================================
// �o���b�g�I������
void UninitStatIco(void)
{
	STAT_ICO *statico = &staticoWk[0];
	for (int i = 0; i < STAT_ICO_MAX; i++)
		SAFE_RELEASE(D3DTexStatIco[i]);

	SAFE_RELEASE(D3DVtxBuffStatIco);
}

//=============================================================================
// �o���b�g�Z�b�g����
void SetStatIco(D3DXVECTOR3 Pos , int Stat)
{
	STAT_ICO *statico = &staticoWk[0];

	int i;
	for (i = 0; i < STAT_ICO_MAX; i++, statico++)
	{
		if (statico->use == false)
		{
			statico->use = true;
			statico->Pos = Pos;
			statico->fCount = 20;
			statico->TexID = Stat;
			return;
		}
	}


}

//=============================================================================
// �o���b�g�X�V����
void UpdateStatIco(void)
{
	STAT_ICO *statico = &staticoWk[0];
	int i;
	for (i = 0; i < STAT_ICO_MAX; i++, statico++)
		if (statico->use == true)
		{

			statico->fCount--;
			if (statico->fCount <= 0)
			{
				statico->use = false;
			}
		}
}
//=============================================================================
// �o���b�g�`�揈��
void DrawStatIco(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	STAT_ICO *statico = &staticoWk[0];
	CAMERA *camera = GetCamera(0);
	D3DXMATRIX mtxView, mtxScl, mtxRot, mtxTranslate;	// �X�P�[���A��]�A���s�ړ�
	int i;

	for (i = 0; i < STAT_ICO_MAX;i++, statico++)
		if (statico->use == true)
		{

			// ���e�X�g�ݒ�

			// ���e�X�g��L����
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, ALPHA);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			// ���C���e�B���O�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			// �r���[�}�g���b�N�X���擾
			mtxView = camera->mtxView;

			// ���[���h�}�g���N�X������
			D3DXMatrixIdentity(&statico->mtxWorld);

			//�|���S���𐳖ʂɌ�����(�t�s��)

			// �����s��̐����s��ɂƂ�t�s��
			statico->mtxWorld._11 = mtxView._11;
			statico->mtxWorld._12 = mtxView._21;
			statico->mtxWorld._13 = mtxView._31;

			statico->mtxWorld._21 = mtxView._12;
			statico->mtxWorld._22 = mtxView._22;
			statico->mtxWorld._23 = mtxView._32;

			statico->mtxWorld._31 = mtxView._13;
			statico->mtxWorld._32 = mtxView._23;
			statico->mtxWorld._33 = mtxView._33;


			//D3DXMatrixInverse(&statico->mtxWorld, NULL, &mtxView);
			//statico->mtxWorld._41 = 0.0f;
			//statico->mtxWorld._42 = 0.0f;
			//statico->mtxWorld._43 = 0.0f;

			// ���[���h�}�g���b�N�X�ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &statico->mtxWorld);

			// �X�P�[���𔽉f	(S)
			D3DXMatrixScaling(&mtxScl, statico->Scl.x, statico->Scl.y, statico->Scl.z);
			D3DXMatrixMultiply(&statico->mtxWorld, &statico->mtxWorld, &mtxScl); 	// &statico->mtxWorld = &statico->mtxWorld * &mtxScl
			//// ��]�𔽉f		(R)
			//D3DXMatrixRotationYawPitchRoll(&mtxRot, statico->Rot.y, statico->Rot.x, statico->Rot.z);
			//D3DXMatrixMultiply(&statico->mtxWorld, &statico->mtxWorld, &mtxRot);	// &statico->mtxWorld = &statico->mtxWorld * &mtxRot
			// ���s�ړ��𔽉f	(T)
			D3DXMatrixTranslation(&mtxTranslate, statico->Pos.x, statico->Pos.y, statico->Pos.z);
			D3DXMatrixMultiply(&statico->mtxWorld, &statico->mtxWorld, &mtxTranslate);

			//D3DXMatrixMultiply(&statico->mtxWorld, &statico->mtxWorld, &GetInvRotateMat());

			// ���[���h�}�g���b�N�X�ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &statico->mtxWorld);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, D3DVtxBuffStatIco, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �N�X�`���̐ݒ�
			pDevice->SetTexture(0, D3DTexStatIco[statico->TexID]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

			// ���C���e�B���O��L���ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			// ���e�X�g�𖳌���
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			//// �`�悵�������
			//statico->use = false;
		}
}

//=============================================================================
// ���_�̍쐬
HRESULT MakeVertexStatIco(LPDIRECT3DDEVICE9 pDevice)
{
	STAT_ICO *statico = &staticoWk[0];
	int i;
	for (i = 0; i < STAT_ICO_MAX; i++)
	{
		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
			D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
			FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
			D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
			&D3DVtxBuffStatIco,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			NULL)))						// NULL�ɐݒ�
		{
			return E_FAIL;
		}

		{//���_�o�b�t�@�̒��g�𖄂߂�
			VERTEX_3D *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			D3DVtxBuffStatIco->Lock(0, 0, (void**)&pVtx, 0);

			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-ICO_SIZE_X, ICO_SIZE_Y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(ICO_SIZE_X, ICO_SIZE_Y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-ICO_SIZE_X, -ICO_SIZE_Y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(ICO_SIZE_X, -ICO_SIZE_Y, 0.0f);

			// �@���x�N�g���̐ݒ�
			pVtx[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			// ���_�f�[�^���A�����b�N����
			D3DVtxBuffStatIco->Unlock();
		}
	}
	return S_OK;
}


//=============================================================================
// �o���b�g�̃Q�b�g�֐�
STAT_ICO *GetStatIco(int no)
{
	return (&staticoWk[no]);
}