//=============================================================================
//
// �o���b�g���� [bullboard.cpp]
// Author : ���ꐶ
//
//=============================================================================
#include "bullet.h"
#include "input.h"
#include "camera.h"
#include "player.h"
#include "shadow.h"
#include "field.h"
#include "mathWk.h"

//=============================================================================
// �}�N����`
#define	TEXTURE_BULLET000	"data/TEXTURE/bullet000.png"						// �ǂݍ��ރe�N�X�`���t�@�C����	name of the teture file

// ������₷���悤��ID�ɖ��O
enum TEXTURE_NAME
{
	BULLET000,
	TEXTURE_MAX,
};

#define	MOVE_VALUE			(5.0f)											// �ړ���						moving distance
#define	ROT_VALUE			(D3DX_PI * 0.02f)								// ��]��						rotation amount

#define	BULL_SIZE_X			(5.0f)											// �o���b�g�̃T�C�Y(X����)		size of the x axis
#define	BULL_SIZE_Z			(5.0f)											// �o���b�g�̃T�C�Y(Z����)		size of the y axis

#define	ALPHA				(170)

//=============================================================================
// �v���g�^�C�v�錾
D3DXMATRIX GetInvRotateMatBull(void);
HRESULT MakeVertexBull(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		D3DTextureBull[TEXTURE_MAX] = { NULL};		// �e�N�X�`���ւ̃|�C���^		pointer for texture
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffBull = NULL;	// ���_�o�b�t�@�ւ̃|�C���^		pointer for vertex buffer

BULL					bullWk[BULL_MAX];			// �t�B�[���h�ւ̃|�C���^


//=============================================================================
// �o���b�g����������
HRESULT InitBull(void)
{
	BULL *bull = &bullWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int i;

	D3DXCreateTextureFromFile(pDevice, TEXTURE_BULLET000, &D3DTextureBull[BULLET000]);

	for (i = 0; i < BULL_MAX; i++, bull++)
	{

		bull->use = false;

		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		bull->Pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
		//bull->Rot = D3DXVECTOR3(-D3DX_PI * 0.5f, 0.0f, 0.0f);
		bull->Scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

		MakeVertexBull(pDevice);
		switch (i)
		{
		case 0:
			bull->TexID = BULLET000;
			break;
		}

		// �e�̐���
		bull->nIdxShadow = CreateShadow(bull->Pos, 0.1f, 25.0f);

		bull->fSizeShadow = 10.0f;
		bull->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
	}
	return S_OK;
}

//=============================================================================
// �o���b�g�I������
void UninitBull(void)
{
	BULL *bull = &bullWk[0];
	for (int i = 0; i < TEXTURE_MAX; i++, bull++)
	{
		if (D3DTextureBull[i] != NULL)
		{// �e�N�X�`���̊J��
			D3DTextureBull[i]->Release();
			D3DTextureBull[i] = NULL;
		}
	}
	if (g_pD3DVtxBuffBull != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffBull->Release();
		g_pD3DVtxBuffBull = NULL;
	}

}

//=============================================================================
// �o���b�g�Z�b�g����
void SetBull(D3DXVECTOR3 Pos , D3DXVECTOR3 Rot)
{
	BULL *bull = &bullWk[0];
	CAMERA *camera = GetCamera(0);
	PLAYER *player = GetPlayer(0);
	int i;
	for (i = 0; i < BULL_MAX; i++, bull++)
	{
		if (bull->use == false)
		{
			bull->use = true;
			bull->Pos = Pos;
			bull->fCount = 50;
			bull->Vel = Ang2Vec(Rot, MOVE_VALUE);

			bull->nIdxShadow = CreateShadow(bull->Pos, 0.1f, 25.0f);
			return;
		}
	}


}

//=============================================================================
// �o���b�g�X�V����
void UpdateBull(void)
{
	BULL *bull = &bullWk[0];
	int i;
	for (i = 0; i < BULL_MAX; i++, bull++)
		if (bull->use == true)
		{
			bull->Pos += bull->Vel;



			// �e�̈ʒu�ݒ�
			SetPositionShadow(bull->nIdxShadow, D3DXVECTOR3(bull->Pos.x, 0.1f, bull->Pos.z));

			SetVertexShadow(bull->nIdxShadow, bull->fSizeShadow, bull->fSizeShadow);
			SetColorShadow(bull->nIdxShadow, bull->colShadow);

			bull->fCount--;
			if (bull->fCount <= 0)
			{
				bull->use = false;
				ReleaseShadow(bull->nIdxShadow);
			}
		}
}
//=============================================================================
// �o���b�g�`�揈��
void DrawBull(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BULL *bull = &bullWk[0];
	CAMERA *camera = GetCamera(0);
	D3DXMATRIX mtxView, mtxScl, mtxRot, mtxTranslate;	// �X�P�[���A��]�A���s�ړ�
	int i;

	for (i = 0; i < BULL_MAX;i++, bull++)
		if (bull->use == true)
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
			D3DXMatrixIdentity(&bull->mtxWorld);


			//�|���S���𐳖ʂɌ�����(�t�s��)

			// �����s��̐����s��ɂƂ�t�s��
			bull->mtxWorld._11 = mtxView._11;
			bull->mtxWorld._12 = mtxView._21;
			bull->mtxWorld._13 = mtxView._31;

			bull->mtxWorld._21 = mtxView._12;
			bull->mtxWorld._22 = mtxView._22;
			bull->mtxWorld._23 = mtxView._32;

			bull->mtxWorld._31 = mtxView._13;
			bull->mtxWorld._32 = mtxView._23;
			bull->mtxWorld._33 = mtxView._33;


			//D3DXMatrixInverse(&bull->mtxWorld, NULL, &mtxView);
			//bull->mtxWorld._41 = 0.0f;
			//bull->mtxWorld._42 = 0.0f;
			//bull->mtxWorld._43 = 0.0f;






			// ���[���h�}�g���b�N�X�ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &bull->mtxWorld);

			// �X�P�[���𔽉f	(S)
			D3DXMatrixScaling(&mtxScl, bull->Scl.x, bull->Scl.y, bull->Scl.z);
			D3DXMatrixMultiply(&bull->mtxWorld, &bull->mtxWorld, &mtxScl); 	// &bull->mtxWorld = &bull->mtxWorld * &mtxScl
			//// ��]�𔽉f		(R)
			//D3DXMatrixRotationYawPitchRoll(&mtxRot, bull->Rot.y, bull->Rot.x, bull->Rot.z);
			//D3DXMatrixMultiply(&bull->mtxWorld, &bull->mtxWorld, &mtxRot);	// &bull->mtxWorld = &bull->mtxWorld * &mtxRot
			// ���s�ړ��𔽉f	(T)
			D3DXMatrixTranslation(&mtxTranslate, bull->Pos.x, bull->Pos.y, bull->Pos.z);
			D3DXMatrixMultiply(&bull->mtxWorld, &bull->mtxWorld, &mtxTranslate);

			//D3DXMatrixMultiply(&bull->mtxWorld, &bull->mtxWorld, &GetInvRotateMat());






			// ���[���h�}�g���b�N�X�ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &bull->mtxWorld);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pD3DVtxBuffBull, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �N�X�`���̐ݒ�
			pDevice->SetTexture(0, D3DTextureBull[bull->TexID]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

			// ���C���e�B���O��L���ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			// ���e�X�g�𖳌���
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		}
}





//=============================================================================
// ���_�̍쐬
HRESULT MakeVertexBull(LPDIRECT3DDEVICE9 pDevice)
{
	BULL *bull = &bullWk[0];
	int i;
	for (i = 0; i < BULL_MAX; i++)
	{
		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
			D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
			FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
			D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
			&g_pD3DVtxBuffBull,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			NULL)))						// NULL�ɐݒ�
		{
			return E_FAIL;
		}

		{//���_�o�b�t�@�̒��g�𖄂߂�
			VERTEX_3D *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			g_pD3DVtxBuffBull->Lock(0, 0, (void**)&pVtx, 0);

			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-BULL_SIZE_X, BULL_SIZE_Z, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(BULL_SIZE_X, BULL_SIZE_Z, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-BULL_SIZE_X, -BULL_SIZE_Z, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(BULL_SIZE_X, -BULL_SIZE_Z, 0.0f);

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
			g_pD3DVtxBuffBull->Unlock();
		}
	}
	return S_OK;
}

//=============================================================================
// �r���{�[�h�ɂ��鏈��
D3DXMATRIX GetInvRotateMatBull(void)
{
	CAMERA *camera = GetCamera(0);
	D3DXMATRIX Inv;
	D3DXVECTOR3 LookAt;
	LookAt = -camera->Eye + camera->At;
	D3DXMatrixIdentity(&Inv);
	D3DXMatrixLookAtLH(&Inv, &D3DXVECTOR3(0, 0, 0), &LookAt, &camera->Up);
	D3DXMatrixInverse(&Inv, NULL, &Inv);
	return Inv;
}

//=============================================================================
// �o���b�g�̃Q�b�g�֐�
BULL *GetBull(int no)
{
	return (&bullWk[no]);
}