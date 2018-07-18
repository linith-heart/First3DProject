//=============================================================================
//
// �}�b�v���� [map.cpp]
// Author : ���ꐶ
//
//=============================================================================
#include "camera.h"
#include "map.h"
#include "input.h"

#include "checkhit.h"

//=============================================================================
// �}�N����`
#define	TEXTURE_MAP000		"data/MAP/cliff_path.png"				// �ǂݍ��ރe�N�X�`���t�@�C����	name of the teture file
#define	TEXTURE_MAP001		"data/MAP/cliff_bg000.png"					// �ǂݍ��ރe�N�X�`���t�@�C����	name of the teture file

enum
{
	MAP_TEX = 0,
	BG_MAP_TEX,
	TEX_MAX,
};


#define MAP000				"data/MAP/cliff_path001.x"						// �\���p�̃��V���̃t�@�C����	name of mesh file for draw
#define MAPBG000			"data/MAP/cliff_bg.x"					// �\���p�̃��V���̃t�@�C����	name of mesh file for draw
#define HITPATH000			"data/MAP/cliff_path_hitbox001.x"				// �����蔻��p���b�V���̃t�@�C����	name of mesh file for hitbox
#define HITWALL000			"data/MAP/cliff_wall_hitbox001.x"				// �����蔻��p���b�V���̃t�@�C����	name of mesh file for hitbox


#define CURRENT_MAP			(MAP000)
#define CURRENT_BG_MAP		(MAPBG000)
#define CURRENT_HIT_PATH	(HITPATH000)
#define CURRENT_HIT_WALL	(HITWALL000)

#define CURRENT_TEXTURE		(TEXTURE_MAP000)
#define CURRENT_BG_TEXTURE	(TEXTURE_MAP001)

#define MAP_SIZE		(25.0f)											// �傫��


//=============================================================================
// �v���g�^�C�v�錾

//=============================================================================
// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		D3DTextureMap[TEX_MAX] = { NULL, NULL };	// �e�N�X�`���ւ̃|�C���^

MAP						mapWk[MAP_MAX];			// �t�B�[���h�ւ̃|�C���^

bool					DrawFlag = true;

//=============================================================================
// ����������
HRESULT InitMap(void)
{
	MAP *map = &mapWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// �e�N�X�`���̐ݒ�
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MAP000, &D3DTextureMap[MAP_TEX]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MAP001, &D3DTextureMap[BG_MAP_TEX]);
	int i;
	for (i = 0; i < MAP_MAX; i++, map++)
	{
		//===========================================================
		// �`��p�̏�����
		map->mesh.use = true;

		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		map->mesh.Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		map->mesh.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		map->mesh.Scl = D3DXVECTOR3(MAP_SIZE, MAP_SIZE, MAP_SIZE);

		// ���f���֌W�̏�����
		map->mesh.Mesh = NULL;
		map->mesh.BuffMat = NULL;
		map->mesh.NumMat = 0;

		// �`��p��X�t�@�C���̓ǂݍ���
		switch (i % COMP4MAP)
		{
		case 0:
			if (FAILED(D3DXLoadMeshFromX(CURRENT_MAP, D3DXMESH_SYSTEMMEM, pDevice, NULL, &map->mesh.BuffMat, NULL, &map->mesh.NumMat, &map->mesh.Mesh)))
			{
				return E_FAIL;
			}
			map->isHitBox = false;
			break;
		case 1:
			if (FAILED(D3DXLoadMeshFromX(CURRENT_BG_MAP, D3DXMESH_SYSTEMMEM, pDevice, NULL, &map->mesh.BuffMat, NULL, &map->mesh.NumMat, &map->mesh.Mesh)))
			{
				return E_FAIL;
			}
			map->isHitBox = false;
			break;
		case 2:
			if (FAILED(D3DXLoadMeshFromX(CURRENT_HIT_PATH, D3DXMESH_SYSTEMMEM, pDevice, NULL, &map->mesh.BuffMat, NULL, &map->mesh.NumMat, &map->mesh.Mesh)))
			{
				return E_FAIL;
			}
			map->isHitBox = true;
			SetVtxData(&map->mesh);
			break;
		case 3:
			if (FAILED(D3DXLoadMeshFromX(CURRENT_HIT_WALL, D3DXMESH_SYSTEMMEM, pDevice, NULL, &map->mesh.BuffMat, NULL, &map->mesh.NumMat, &map->mesh.Mesh)))
			{
				return E_FAIL;
			}
			map->isHitBox = true;
			SetVtxData(&map->mesh);
			break;
		}






	}
	return S_OK;

}

//=============================================================================
// �I������
void UninitMap(void)
{
	MAP *map = &mapWk[0];
	for (int i = 0; i < TEX_MAX;i++)
		if (D3DTextureMap[i] != NULL)
		{// �e�N�X�`���̊J��
			D3DTextureMap[i]->Release();
			D3DTextureMap[i] = NULL;
		}
	for (int i = 0; i < MAP_MAX; i++, map++)
	{
		if (map->mesh.VtxBuff != NULL)
		{// ���_�o�b�t�@�̊J��
			map->mesh.VtxBuff->Release();
			map->mesh.VtxBuff = NULL;
		}

		if (map->mesh.Mesh != NULL)
		{// ���V���̉��
			map->mesh.Mesh->Release();
			map->mesh.Mesh = NULL;
		}

		if (map->mesh.IdxBuff != NULL)
		{// �C���f�b�N�X�o�b�t�@�ƃ��@�[�e�b�N�X�|�X�͈ꏏ�ɊJ��
			free(map->mesh.IdxBuff);
			free(map->mesh.VtxPos);
		}
	}
}

//=============================================================================
// �X�V����
void UpdateMap(void)
{
	MAP *map = &mapWk[0];

	if (GetKeyboardTrigger(DIK_NUMPAD9))
	{
		if (DrawFlag)
			DrawFlag = false;
		else if (!DrawFlag)
			DrawFlag = true;
	}


}

//=============================================================================
// �`�揈��
void DrawMap(void)
{
	MAP *map = &mapWk[0];
	CAMERA *camera = GetCamera(0);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// �X�P�[���A��]�A���s�ړ�
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	int i, j;

	for (i = 0; i < MAP_MAX; i++, map++)
		if (map->mesh.use == true)
		{
			// ���C�e�B���O�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			// ���[���h�}�g���N�X������
			D3DXMatrixIdentity(&map->mesh.mtxWorld);

			// �X�P�[���𔽉f	(S)
			D3DXMatrixScaling(&mtxScl, map->mesh.Scl.x, map->mesh.Scl.y, map->mesh.Scl.z);
			D3DXMatrixMultiply(&map->mesh.mtxWorld, &map->mesh.mtxWorld, &mtxScl); 	// &ene->mtxWorld = &ene->mtxWorld * &mtxScl
			// ��]�𔽉f		(R)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, map->mesh.Rot.y, map->mesh.Rot.x, map->mesh.Rot.z);
			D3DXMatrixMultiply(&map->mesh.mtxWorld, &map->mesh.mtxWorld, &mtxRot);	// &ene->mtxWorld = &ene->mtxWorld * &mtxRot
			// ���s�ړ��𔽉f	(T)
			D3DXMatrixTranslation(&mtxTranslate, map->mesh.Pos.x, map->mesh.Pos.y, map->mesh.Pos.z);
			D3DXMatrixMultiply(&map->mesh.mtxWorld, &map->mesh.mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &map->mesh.mtxWorld);

			// ���݂̃}�e���A����ۑ�
			pDevice->GetMaterial(&matDef);

			// �}�e���A�����ւ̃|�C���^��
			pD3DXMat = (D3DXMATERIAL*)map->mesh.BuffMat->GetBufferPointer();

			if (DrawFlag)
			{
				if (i % COMP4MAP < 2)
				{
					for (j = 0; j < (int)map->mesh.NumMat; j++)
					{
						pDevice->SetMaterial(&pD3DXMat[j].MatD3D);	// �}�e���A���̐ݒ�
						pDevice->SetTexture(0, D3DTextureMap[i]);	// �e�N�X�`���̐ݒ�
						map->mesh.Mesh->DrawSubset(j);
					}
				}
			}
			else if (!DrawFlag)
			{
				if (i % COMP4MAP == 3)
				{
					for (j = 0; j < (int)map->mesh.NumMat; j++)
					{
						pDevice->SetMaterial(&pD3DXMat[j].MatD3D);	// �}�e���A���̐ݒ�
						pDevice->SetTexture(0, D3DTextureMap[i]);	// �e�N�X�`���̐ݒ�
						map->mesh.Mesh->DrawSubset(j);
					}
				}

			}
			// ���C�e�B���O��L���ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			// �}�e���A���ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
}

//=============================================================================
// �}�b�v�̃Q�b�g�֐�
MAP *GetMap(int no)
{
	return (&mapWk[no]);
}