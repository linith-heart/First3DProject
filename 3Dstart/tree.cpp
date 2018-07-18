//=============================================================================
//
// �؂̏��� [tree.cpp]
// Author : ���ꐶ
//
//=============================================================================
#include "tree.h"

#include "camera.h"
#include "player.h"

#include "checkhit.h"
/*
���V���̖؂̏���

mesh tree
*/

//=============================================================================
// �}�N����`
#define TEXTURE_TREE	("data/TEXTURE/tree000.png")					// �؂̃e�N�X�`���[
#define MODEL			("data/MODEL/tree000.x")						// �؂̃��f��
#define	VALUE_MOVE		(0.5f)											// �ړ���
#define	ROT_VALUE		(D3DX_PI * 0.02f)								// ��]��
#define SCL_VALUE		(1.0f)											// �傫��
#define RANGE_VALUE		(50)											// ������̋���

#define	ALPHA			(170)


//=============================================================================
// �v���g�^�C�v�錾
void MoveTree(void);

//=============================================================================
// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		D3DTextureTree = NULL;		// �e�N�X�`���ւ̃|�C���^

TREE					treeWk[TREE_MAX];			// �t�B�[���h�ւ̃|�C���^


//=============================================================================
// ����������
HRESULT InitTree(void)
{
	TREE *tree = &treeWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̐ݒ�
	D3DXCreateTextureFromFile(pDevice, TEXTURE_TREE, &D3DTextureTree);

	int i;
	for (i = 0; i < TREE_MAX; i++, tree++)
	{
		tree->mesh.use = true;

		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		tree->mesh.Pos = D3DXVECTOR3(-100.0f * i, 0.0f, -100.0f);
		tree->mesh.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		tree->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);

		// ���f���֌W�̏�����
		tree->mesh.VtxBuff = NULL;
		//MakeVertexTree(pDevice);
		tree->mesh.Mesh = NULL;
		tree->mesh.BuffMat = NULL;
		tree->mesh.NumMat = 0;

		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(MODEL,			// ���f���f�[�^
			D3DXMESH_SYSTEMMEM,					// �g�p���郁�����̃I�v�V����
			pDevice,							// �f�o�C�X 
			NULL,								// ���g�p
			&tree->mesh.BuffMat,				// �}�e���A���f�[�^�ւ̃|�C���^�[
			NULL,								// ���g�p
			&tree->mesh.NumMat,				// D3DXMATERIAL�\���̂̐�
			&tree->mesh.Mesh)))				// ���b�V���f�[�^�ւ̃|�C���^�[
		{
			return E_FAIL;
		}


		int			VtxCnt = tree->mesh.Mesh->GetNumVertices();	// ���V���̒��_��
		int			Polycnt = tree->mesh.Mesh->GetNumFaces();		// ���V���̃|���S����
		int			BytePerVtx = tree->mesh.Mesh->GetNumBytesPerVertex();	// �P���_������̃o�C�g��
		WORD		*idx;
		char		*pVtx;

		// �|���S���� * 3 �����C���f�b�N�X���K�v	
		// �C���f�b�N�X�ЂƂ̃T�C�Y��WORD�^ = 2�o�C�g�Ȃ̂�,
		// �C���f�b�N�X���i�[����z���(�|���S���� * 3���_ * 2�o�C�g)���������I�m��
		// �I������ free(tree->mesh.IdxBuff); �Ń������J����Y�ꂸ��
		tree->mesh.IdxBuff = (WORD*)malloc(sizeof(WORD) * Polycnt * 3);

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		tree->mesh.Mesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&idx);
		// �C���f�b�N�X�o�t�@�̏����擾
		for (int i = 0; i < (Polycnt * 3); i++)
		{
			tree->mesh.IdxBuff[i] = idx[i];
		}

		// ���_�f�[�^���A�����b�N����
		tree->mesh.Mesh->UnlockIndexBuffer();


		// ���_���W�ЂƂ̃T�C�Y��D3DXVECTOR3�^ = 12�o�C�g�Ȃ̂�,
		// ���_���W���i�[����z���(���_�� * 12�o�C�g)���������I�m��
		// �I������ free(tree->mesh.VtxPos); �Ń������J����Y�ꂸ��
		tree->mesh.VtxPos = (D3DXVECTOR3*)malloc(sizeof(D3DXVECTOR3) * VtxCnt);

		// �o�[�e�b�N�X�f�[�^�����b�N
		tree->mesh.Mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtx);

		// �o�[�e�b�N�X�f�[�^���擾
		for (int i = 0; i < VtxCnt; i++)
		{
			tree->mesh.VtxPos[i] = *(D3DXVECTOR3*)&pVtx[i * BytePerVtx];
		}

		// ���_�f�[�^���A�����b�N����
		tree->mesh.Mesh->UnlockVertexBuffer();

	}
	return S_OK;

}

//=============================================================================
// �I������
void UninitTree(void)
{
	TREE *tree = &treeWk[0];
	if (D3DTextureTree != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureTree->Release();
		D3DTextureTree = NULL;
	}

	for (int i = 0; i < TREE_MAX; i++, tree++)
	{
		if (tree->mesh.VtxBuff != NULL)
		{// ���_�o�b�t�@�̊J��
			tree->mesh.VtxBuff->Release();
			tree->mesh.VtxBuff = NULL;
		}

		if (tree->mesh.Mesh != NULL)
		{// ���V���̉��
			tree->mesh.Mesh->Release();
			tree->mesh.Mesh = NULL;
		}

		if (tree->mesh.IdxBuff != NULL)
		{// �C���f�b�N�X�o�b�t�@�ƃ��@�[�e�b�N�X�|�X�͈ꏏ�ɊJ��
			free(tree->mesh.IdxBuff);
			free(tree->mesh.VtxPos);
			tree->mesh.IdxBuff = NULL;
			tree->mesh.VtxPos = NULL;
		}
	}
}

//=============================================================================
// �X�V����
void UpdateTree(void)
{
	TREE *tree = &treeWk[0];
	PLAYER *player = GetPlayer(0);
	D3DXVECTOR3 Vel = { 0.0f,0.0f,0.0f };
	int i;
	for (i = 0; i < TREE_MAX; i++, tree++)
	{


	}
}

//=============================================================================
// �`�揈��
void DrawTree(void)
{
	TREE *tree = &treeWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// �X�P�[���A��]�A���s�ړ�
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	int i, j;

	// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, ALPHA);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	for (i = 0; i < TREE_MAX; i++, tree++)
		if (tree->mesh.use == true)
		{

			// ���[���h�}�g���N�X������
			D3DXMatrixIdentity(&tree->mesh.mtxWorld);

			// �X�P�[���𔽉f	(S)
			D3DXMatrixScaling(&mtxScl, tree->mesh.Scl.x, tree->mesh.Scl.y, tree->mesh.Scl.z);
			D3DXMatrixMultiply(&tree->mesh.mtxWorld, &tree->mesh.mtxWorld, &mtxScl); 	// &mesh->mtxWorld = &mesh->mtxWorld * &mtxScl
																						// ��]�𔽉f		(R)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, tree->mesh.Rot.y, tree->mesh.Rot.x, tree->mesh.Rot.z);
			D3DXMatrixMultiply(&tree->mesh.mtxWorld, &tree->mesh.mtxWorld, &mtxRot);	// &mesh->mtxWorld = &mesh->mtxWorld * &mtxRot
																						// ���s�ړ��𔽉f	(T)
			D3DXMatrixTranslation(&mtxTranslate, tree->mesh.Pos.x, tree->mesh.Pos.y, tree->mesh.Pos.z);
			D3DXMatrixMultiply(&tree->mesh.mtxWorld, &tree->mesh.mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &tree->mesh.mtxWorld);

			// ���݂̃}�e���A����ۑ�
			pDevice->GetMaterial(&matDef);

			// �}�e���A�����ւ̃|�C���^��
			pD3DXMat = (D3DXMATERIAL*)tree->mesh.BuffMat->GetBufferPointer();


			for (j = 0; j < (int)tree->mesh.NumMat; j++)
			{
				pDevice->SetMaterial(&pD3DXMat[j].MatD3D);	// �}�e���A���̐ݒ�

				pDevice->SetTexture(0, D3DTextureTree);	// �e�N�X�`���̐ݒ�
				tree->mesh.Mesh->DrawSubset(j);
			}

			// �}�e���A���ɖ߂�
			pDevice->SetMaterial(&matDef);
		}

	// ���e�X�g�𖳌���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

//=============================================================================
// �؂̈ړ�
void MoveTree(void)
{
	TREE *tree = &treeWk[0];
	PLAYER *player = GetPlayer(0);

}

//=============================================================================
// ���̃Q�b�g�֐�
TREE *GetTree(int no)
{
	return (&treeWk[no]);
}






int hitCheckTree(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
{
	TREE *tree = &treeWk[0];
	int				ans = 0;	// ��悸�������Ă��Ȃ����Z�b�g���Ă���

	D3DXVECTOR3		pos[3];		// �|���S���̖@��
	D3DXVECTOR3		vel = { 0.0f,0.0f,0.0f };


	int PolyCnt = tree->mesh.Mesh->GetNumFaces();

	for (int i = 0; i<PolyCnt; i++)
	{	// �܂��́A�|���S���̒��_�����߂�
		int idx0 = tree->mesh.IdxBuff[i * 3];
		int idx1 = tree->mesh.IdxBuff[i * 3 + 1];
		int idx2 = tree->mesh.IdxBuff[i * 3 + 2];

		// ���_���W�̐ݒ�
		pos[0] = tree->mesh.VtxPos[idx0];
		pos[1] = tree->mesh.VtxPos[idx1];
		pos[2] = tree->mesh.VtxPos[idx2];

		// �������|���S���Ɛ����̓����蔻��
		ans = CheckHitLine(pos[0], pos[2], pos[3], pos0, pos1, &tree->mesh.mtxWorld, &vel);
		if (ans != 0)
		{
			break;
		}


	}

	return(ans);

}
