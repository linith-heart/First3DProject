//=============================================================================
//
// ���̏��� [crate.cpp]
// Author : ���ꐶ
//
//=============================================================================
#include "crate.h"
#include "game.h"

#include "camera.h"
#include "player.h"

#include "checkhit.h"
/*
���V���̔��̏���

mesh box
*/

//=============================================================================
// �}�N����`
#define TEXTURE_WOOD_CRATE			("data/TEXTURE/crate000.png")		// �؂̔�
#define TEXTURE_METAL_CRATE			("data/TEXTURE/crate001.png")		// �S�̔�

#define MODEL			("data/MODEL/box.x")							// ���̃��f��


//#define TEXTURE_TEST				("data/TEXTURE/cave_entrance000.png")	// �S�̔�
#define MODEL_TEST		("data/MAP/cliff_pt2.x")							// ���̃��f��


#define	VALUE_MOVE		(0.5f)											// �ړ���
#define	ROT_VALUE		(D3DX_PI * 0.02f)								// ��]��
#define SCL_VALUE		(0.5f)											// �傫��
#define RANGE_VALUE		(50)											// ������̋���

#define GRND_CHECK_HIGHT	(100.0f)		// �n�ʂƂ̓����蔻��̐����̒���	hight of line segment for ground check;
#define OBJ_CHECK_HIGHT		(90.0f)			// �I�u�W�F�N�g�Ƃ̓����蔻��̐����̒����̈Ⴂ	difference between hight of line segment for object and ground check;


enum TEXTURE_NAME
{
	WOOD_TEX = 0,
	METAL_TEX,
	TEX_MAX,
};



//=============================================================================
// ���_�\����


//=============================================================================
// �v���g�^�C�v�錾

void SetCrateData(int i,int num, int data);

void MoveCrate(void);


void DrawCrateHitBox(void);


void SetCratePos(int no);

//=============================================================================
// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		D3DTextureCrate[TEX_MAX] = { NULL, NULL };		// �e�N�X�`���ւ̃|�C���^

CRATE					crateWk[CRATE_MAX];			// �t�B�[���h�ւ̃|�C���^

D3DXVECTOR3				OldPos[CRATE_MAX];			// �Â��ʒu

#ifdef _DEBUG

#endif

//=============================================================================
// ����������
HRESULT InitCrate(void)
{
	CRATE *crate = &crateWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̐ݒ�
	D3DXCreateTextureFromFile(pDevice, TEXTURE_WOOD_CRATE, &D3DTextureCrate[WOOD_TEX]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_METAL_CRATE, &D3DTextureCrate[METAL_TEX]);

	FILE *fp;
	// �I�u�W�F�N�g�f�[�^�������Ă�t�@�C�����J��
	fp = fopen(OBJECT_DATA_FILE, "r");
	if (fp == NULL)
	{
		perror("Error opening file");
		return(-1);
	}

	// ���̃f�[�^������
	char tbuf1[MAX_PATH];
	while (1)
	{
		fscanf(fp, "%s", &tbuf1[0]);
		if (strcmp(&tbuf1[0], CRATE_DATA_START) == 0) // ��������u���[�N
			break;
	}

	int i;
	for (i = 0; i < CRATE_MAX; i++, crate++)
	{
		// �X�g�����O�Ƀf�[�^��ۑ�
		fscanf(fp, "%s", &tbuf1[0]);

		// �X�g�����O����C���g�f�[�^�ւ̕ϊ�
		int j = 0;
		int adres = 0;
		int tdata = 0;
		int cnt = 0;
		bool flag = true;
		while (flag)
		{
			char check = tbuf1[j];
			if (check == ',')
			{
				tdata = atoi(&tbuf1[adres]);
				SetCrateData(i, cnt, tdata);
				cnt++;
				j++;
				adres = j;
			}
			else if (check == '\0')
			{
				flag = false;
				tdata = atoi(&tbuf1[adres]);
				SetCrateData(i, cnt, tdata);
			}
			else
				j++;
		}


		//crate->mesh.use = true;
		//// �ʒu�E��]�E�X�P�[���̏����ݒ�
		//SetCratePos(i);	// �ʒu���Z�b�g
		//crate->mesh.TexId = METAL_TEX;



		crate->mesh.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		crate->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);

		OldPos[i] = { 0.0f,0.0f,0.0f };
		crate->mesh.Grav = 0.0f;

		crate->isSolid = true;

		// ���f���֌W�̏�����
		crate->mesh.VtxBuff = NULL;
		crate->mesh.Mesh = NULL;
		crate->mesh.BuffMat = NULL;
		crate->mesh.NumMat = 0;

		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(MODEL,			// ���f���f�[�^
			D3DXMESH_SYSTEMMEM,					// �g�p���郁�����̃I�v�V����
			pDevice,							// �f�o�C�X 
			NULL,								// ���g�p
			&crate->mesh.BuffMat,				// �}�e���A���f�[�^�ւ̃|�C���^�[
			NULL,								// ���g�p
			&crate->mesh.NumMat,				// D3DXMATERIAL�\���̂̐�
			&crate->mesh.Mesh)))				// ���b�V���f�[�^�ւ̃|�C���^�[
		{
			return E_FAIL;
		}

		SetVtxData(&crate->mesh);

	}
	return S_OK;

}

//=============================================================================
// �X�g�����O����C���g�f�[�^�ւ̕ϊ�
// i	: ���ݑΏۂ̔��̔ԍ�
// num  : ���ꂽ���f�[�^�̎�ޔԍ�
// data : ���ꂽ���f�[�^
//=============================================================================
void SetCrateData(int i,int num, int data)
{
	CRATE *crate = &crateWk[i];
	switch (num)
	{
	case FtD_USE:
		crate->mesh.use = data;
		return;
	case FtD_TYPE:
		crate->mesh.TexId = data;
		return;
	case FtD_POS_X:
		crate->mesh.Pos.x = data;
		return;
	case FtD_POS_Y:
		crate->mesh.Pos.y = data;
		return;
	case FtD_POS_Z:
		crate->mesh.Pos.z = data;
		return;
	}
}


//=============================================================================
// �I������
void UninitCrate(void)
{
	CRATE *crate = &crateWk[0];

	for (int i = 0; i < TEX_MAX; i++)
	{
		// �e�N�X�`���̉��
		SAFE_RELEASE(D3DTextureCrate[i]);
	}

	for (int i = 0; i < CRATE_MAX; i++, crate++)
	{
		// ���_�o�b�t�@�̊J��
		SAFE_RELEASE(crate->mesh.VtxBuff);
		// ���V���̉��
		SAFE_RELEASE(crate->mesh.Mesh);

		if (crate->mesh.IdxBuff != NULL)
		{// �C���f�b�N�X�o�b�t�@�ƃ��@�[�e�b�N�X�|�X�͈ꏏ�ɊJ��
			free(crate->mesh.IdxBuff);
			free(crate->mesh.VtxPos);
			crate->mesh.IdxBuff = NULL;
			crate->mesh.VtxPos = NULL;
		}
	}
}

//=============================================================================
// �X�V����
void UpdateCrate(void)
{
	CRATE *crate = &crateWk[0];
	PLAYER *player = GetPlayer(0);
	int i;
	for (i = 0; i < CRATE_MAX; i++)
	{
		if ((crate + i)->mesh.use)
			if ((crate + i)->mesh.Pos != OldPos[i])
			{
				D3DXVECTOR3 distance = { 0.0f,0.0f,0.0f };
				D3DXVECTOR3 head = (crate + i)->mesh.Pos;	// ���̏�̖�
				D3DXVECTOR3 feet = (crate + i)->mesh.Pos;	// ���̉��̖�
				head.y += GRND_CHECK_HIGHT;
				feet.y += (crate + i)->mesh.Grav;
				bool tFlg = false;							// ���̉��ɉ������邩

				if (CheckHitGroundPrep(head, feet, &distance))	// �n�ʂƂ̓����蔻��
					tFlg = true;

				head.y -= OBJ_CHECK_HIGHT;

				if (tFlg == false)
					for (int j = 0; j < CRATE_MAX; j++)
						if (j != i)	// ���̔��Ƃ̓����蔻��
							if (CheckHitLinePrep(head, feet, &distance, &(crate + j)->mesh))
							{
								tFlg = true;
								break;
							}

				if (tFlg == true)
				{	// ���ɉ������邩��d�͂𑫂��Ȃ�
					(crate + i)->mesh.Pos.y += distance.y - 5;
					(crate + i)->mesh.Grav = 0.0f;
				}
				else
				{	// ���ɉ����Ȃ��̂ŏd�͂𑫂�
					(crate + i)->mesh.Grav -= GRAVITY;
				}
			}
		OldPos[i] = (crate + i)->mesh.Pos;
		(crate + i)->mesh.Pos.y += (crate + i)->mesh.Grav;
	}
}

//=============================================================================
// �`�揈��
void DrawCrate(void)
{
	CRATE *crate = &crateWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// �X�P�[���A��]�A���s�ړ�
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	int i, j;

	for (i = 0; i < CRATE_MAX; i++, crate++)
		if (crate->mesh.use == true)
		{



			// ���[���h�}�g���N�X������
			D3DXMatrixIdentity(&crate->mesh.mtxWorld);

			// �X�P�[���𔽉f	(S)
			D3DXMatrixScaling(&mtxScl, crate->mesh.Scl.x, crate->mesh.Scl.y, crate->mesh.Scl.z);
			D3DXMatrixMultiply(&crate->mesh.mtxWorld, &crate->mesh.mtxWorld, &mtxScl); 	// &mesh->mtxWorld = &mesh->mtxWorld * &mtxScl
			// ��]�𔽉f		(R)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, crate->mesh.Rot.y, crate->mesh.Rot.x, crate->mesh.Rot.z);
			D3DXMatrixMultiply(&crate->mesh.mtxWorld, &crate->mesh.mtxWorld, &mtxRot);	// &mesh->mtxWorld = &mesh->mtxWorld * &mtxRot
			// ���s�ړ��𔽉f	(T)
			D3DXMatrixTranslation(&mtxTranslate, crate->mesh.Pos.x, crate->mesh.Pos.y, crate->mesh.Pos.z);
			D3DXMatrixMultiply(&crate->mesh.mtxWorld, &crate->mesh.mtxWorld, &mtxTranslate);

			// ���[���h�}�g���b�N�X�ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &crate->mesh.mtxWorld);

			// ���݂̃}�e���A����ۑ�
			pDevice->GetMaterial(&matDef);

			// �}�e���A�����ւ̃|�C���^��
			pD3DXMat = (D3DXMATERIAL*)crate->mesh.BuffMat->GetBufferPointer();


			for (j = 0; j < (int)crate->mesh.NumMat; j++)
			{
				pDevice->SetMaterial(&pD3DXMat[j].MatD3D);	// �}�e���A���̐ݒ�

				switch (crate->mesh.TexId)
				{
				case WOOD_TEX:
					pDevice->SetTexture(0, D3DTextureCrate[WOOD_TEX]);	// �e�N�X�`���̐ݒ�
					break;
				case METAL_TEX:
					pDevice->SetTexture(0, D3DTextureCrate[METAL_TEX]);	// �e�N�X�`���̐ݒ�
					break;
				}
				crate->mesh.Mesh->DrawSubset(j);
			}


			// �}�e���A���ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
}

//=============================================================================
// ���̈ړ�
void MoveCrate(void)
{
	CRATE *crate = &crateWk[0];
	PLAYER *player = GetPlayer(0);

}

//=============================================================================
// ���̃Q�b�g�֐�
CRATE *GetCrate(int no)
{
	return (&crateWk[no]);
}

//=============================================================================
// ���̈ʒu���Z�b�g���鏈��
void SetCratePos(int no)
{
	CRATE *crate = &crateWk[no];

	switch (no)
	{
	case 0:
		crate->mesh.Pos = D3DXVECTOR3(-40.0f, 460.0f, -2220.0f);
		break;
	case 1:
		crate->mesh.Pos = D3DXVECTOR3(-500.0f, 10.0f, 1100.0f);
		break;
	case 2:
		crate->mesh.Pos = D3DXVECTOR3(-500.0f, 110.0f, 1100.0f);
		break;
	case 3:
		crate->mesh.Pos = D3DXVECTOR3(-444.0f, 10.0f, 1186.0f);
		break;
	case 4:
		crate->mesh.Pos = D3DXVECTOR3(-40.0f, 460.0f, -2270.0f);
		break;
	case 5:
		crate->mesh.Pos = D3DXVECTOR3(-40.0f, 500.0f, -2270.0f);
		break;
	case 6:
		crate->mesh.Pos = D3DXVECTOR3(10.0f, 460.0f, -2270.0f);
		break;
	case 7:
		crate->mesh.Pos = D3DXVECTOR3(10.0f, 500.0f, -2270.0f);
		break;
	case 8:
		crate->mesh.Pos = D3DXVECTOR3(10.0f, 550.0f, -2270.0f);
		break;
	case 9:
		crate->mesh.Pos = D3DXVECTOR3(10.0f, 460.0f, -2220.0f);
		break;
	case 10:
		crate->mesh.Pos = D3DXVECTOR3(10.0f, 500.0f, -2220.0f);
		break;
	case 11:
		crate->mesh.Pos = D3DXVECTOR3(10.0f, 550.0f, -2220.0f);
		break;
	case 12:
		crate->mesh.Pos = D3DXVECTOR3(10.0f, 600.0f, -2220.0f);
		break;

		// starting area
	case 13:
		crate->mesh.Pos = D3DXVECTOR3(-1640.0f, 640.0f, -1630.0f);
		break;
	case 14:
		crate->mesh.Pos = D3DXVECTOR3(-1490.0f, 640.0f, -2240.0f);
		break;
	case 15:
		crate->mesh.Pos = D3DXVECTOR3(-1490.0f, 690.0f, -2240.0f);
		break;
	case 16:
		crate->mesh.Pos = D3DXVECTOR3(-1590.0f, 640.0f, -2260.0f);
		break;

	}
}
