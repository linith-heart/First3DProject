//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : ���ꐶ
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "game.h"
#include "fade.h"
#include "collision.h"
#include "bullet.h"
#include "camera.h"
#include "shadow.h"
#include "mathWk.h"

#include "map.h"
#include "crate.h"
#include "rock.h"
#include "tree.h"
#include "gate.h"
#include "checkhit.h"
#include "effect.h"

//=============================================================================
// �}�N����`
#define TEXTURE			("data/TEXTURE/pig_map.jpg")
#define MODEL			("data/MODEL/pig000.x")

#define	MOVE_VAL		(3.0f)				// �ړ���
#define JUMP_VAL		(6)					// �W�����v�̗�
#define	ROT_VAL			(D3DX_PI * 0.02f)	// ��]��

#define STAM_FULL_RESET_TIMER	(120)		// ���S����X�^�~�i�̃��Z�b�g����
#define STAM_RESET_TIMER	(-30)			// �X�^�~�i�̃��Z�b�g����
#define STAM_DRAIN_RATE		(1.75)			// �X�^�~�i�̏����
#define STAM_CHARGE_RATE	(2)				// �X�^�~�i�̉񕜗�

#define IMMUNE_TIME			(60)			// ���G����

#define GAME_OVER_TIMER	(4 * 60)

//=============================================================================
// �v���g�^�C�v�錾
void ActionInput(void);
void MoveInput(void);


//=============================================================================
// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		D3DTexturePlayer = NULL;	// �e�N�X�`���ւ̃|�C���^
PLAYER					playerWk[PLAYER_MAX];		// �v���C���[�ւ̃|�C���^

int						GOTimer = GAME_OVER_TIMER;				// �v���C���[������ł����ʂ��ς��܂ł̃^�C�}�[

int						fuckme = 0;
//=============================================================================
// ����������
HRESULT InitPlayer(void)
{
	PLAYER *player = &playerWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̐ݒ�
	D3DXCreateTextureFromFile(pDevice, TEXTURE, &D3DTexturePlayer);

	int i;
	for (i = 0; i < PLAYER_MAX; i++, player++)
	{
		player->use = true;
		player->alive = true;

		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		player->Pos = D3DXVECTOR3(POS_X_PLAYER, POS_Y_PLAYER, POS_Z_PLAYER);
		player->Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		player->Scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

		player->HP = PLAYER_HP_MAX;
		player->Stam = PLAYER_STAM_MAX;
		player->Jump = false;
		player->Grav = 0.0f;
		player->HitTime = 0;

		player->Vel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);



		// ���f���֌W�̏�����
		player->Mesh = NULL;
		player->D3DXBuffMatPlayer = NULL;
		player->NumMatPlayer = 0;

		// X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX(MODEL,			// ���f���f�[�^
			D3DXMESH_SYSTEMMEM,				// �g�p���郁�����̃I�v�V����
			pDevice,						// �f�o�C�X 
			NULL,							// ���g�p
			&player->D3DXBuffMatPlayer,		// �}�e���A���f�[�^�ւ̃|�C���^�[
			NULL,							// ���g�p
			&player->NumMatPlayer,			// D3DXMATERIAL�\���̂̐�
			&player->Mesh)))		// ���b�V���f�[�^�ւ̃|�C���^�[
		{
			return E_FAIL;
		}


		// �e�̐���
		player->nIdxShadow = CreateShadow(player->Pos, 25.0f, 25.0f);

		player->fSizeShadow = 25.0f;
		player->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
	}

	GOTimer = GAME_OVER_TIMER;

	return S_OK;
}

//=============================================================================
// �I������
void UninitPlayer(void)
{
	PLAYER *player = &playerWk[0];

	// mesh matirial player

	if(D3DTexturePlayer != NULL)
	{// �e�N�X�`���̊J��
		D3DTexturePlayer->Release();
		D3DTexturePlayer = NULL;
	}
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (player->D3DVtxBuffPlayer != NULL)
		{// ���_�o�b�t�@�̊J��
			player->D3DVtxBuffPlayer->Release();
			player->D3DVtxBuffPlayer = NULL;
		}
		if (player->Mesh != NULL)
		{// ���V���̉��
			player->Mesh->Release();
			player->Mesh = NULL;
		}

	}
}

//=============================================================================
// �X�V����
void UpdatePlayer(void)
{
	PLAYER *player = &playerWk[0];
	MAP *map = GetMap(2);
	if (player->use == true
		&& player->alive == true)
	{
		MoveInput();
		ActionInput();


		if (player->Pos.y < -100.0f)
		{
			player->alive = false;
		}

		if (player->BullCD > 0)
			player->BullCD--;
	}
	else if (player->alive == false)
	{
		if (GOTimer > 0)
			GOTimer--;
		else
			SetFade(FADE_OUT);

		player->Pos.y += player->Grav;
	}

	if (player->HitTime > 0)
		player->HitTime--;


	// �e�̈ʒu�ݒ�
	D3DXVECTOR3 temp = player->Pos;
	D3DXVECTOR3 distance;
	D3DXVECTOR3 head = player->Pos;
	D3DXVECTOR3 feet = player->Pos;
	head.y += 5;
	feet.y -= 300;
	CheckHitGroundPrep(feet, head, &distance);
	
	temp = distance + player->Pos;

	SetPositionShadow(player->nIdxShadow, D3DXVECTOR3(player->Pos.x, temp.y, player->Pos.z));

	SetVertexShadow(player->nIdxShadow, player->fSizeShadow, player->fSizeShadow);
	SetColorShadow(player->nIdxShadow, player->colShadow);

}

//=============================================================================
// �`�揈��
void DrawPlayer(void)
{
	PLAYER *player = &playerWk[0];
	CAMERA *camera = GetCamera(0);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// �X�P�[���A��]�A���s�ړ�
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	int i,j;

	for (i = 0; i < PLAYER_MAX; i++, player++)
	{
		// ���[���h�}�g���N�X������
		D3DXMatrixIdentity(&player->mtxWorld);

		// �X�P�[���𔽉f	(S)
		D3DXMatrixScaling(&mtxScl, player->Scl.x, player->Scl.y, player->Scl.z);
		D3DXMatrixMultiply(&player->mtxWorld, &player->mtxWorld, &mtxScl); 	// &player->mtxWorld = &player->mtxWorld * &mtxScl
		// ��]�𔽉f		(R)
		D3DXMatrixRotationYawPitchRoll(&mtxRot, player->Rot.y, player->Rot.x, player->Rot.z);
		D3DXMatrixMultiply(&player->mtxWorld, &player->mtxWorld, &mtxRot);	// &player->mtxWorld = &player->mtxWorld * &mtxRot
		// ���s�ړ��𔽉f	(T)
		D3DXMatrixTranslation(&mtxTranslate, player->Pos.x, player->Pos.y, player->Pos.z);
		D3DXMatrixMultiply(&player->mtxWorld, &player->mtxWorld, &mtxTranslate);

		// ���[���h�}�g���b�N�X�ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &player->mtxWorld);

		// ���݂̃}�e���A����ۑ�
		pDevice->GetMaterial(&matDef);

		// �}�e���A�����ւ̃|�C���^��
		pD3DXMat = (D3DXMATERIAL*)player->D3DXBuffMatPlayer->GetBufferPointer();


		for (j = 0; j < (int)player->NumMatPlayer; j++)
		{
			pDevice->SetMaterial(&pD3DXMat[j].MatD3D);	// �}�e���A���̐ݒ�

			pDevice->SetTexture(0, D3DTexturePlayer);	// �e�N�X�`���̐ݒ�

			player->Mesh->DrawSubset(j);
		}

		// �}�e���A���ɖ߂�
		pDevice->SetMaterial(&matDef);
	}
}

//=============================================================================
// �A�N�V�����n�̃L�[����
void ActionInput(void)
{
	/* �g�p����Ă���L�[ used keys
	�e����
	SPACE
	
	*/
	PLAYER *player = &playerWk[0];
	CAMERA *camera = GetCamera(0);

	//==============================================
	// �e�۔���
	//if (GetKeyboardPress(DIK_T))
	//{
	//	player->Rot.y += 0.05;
	//}

	if (GetKeyboardTrigger(DIK_SPACE)
		&& player->Jump == false
		|| IsButtonPressed(0, BUTTON_A)
		&& player->Jump == false)
	{
		player->Grav = JUMP_VAL;
		player->Jump = true;
	}


}

//=============================================================================
// �ړ��n�̏���
// �g�p�L�[
// �ړ� : W,A,S,D
// 
// 
// 
// 
void MoveInput(void)
{
	/* �g�p����Ă���L�[ used keys
	�ړ� move
	W
	ASD
	*/
	PLAYER *player = &playerWk[0];
	CAMERA *camera = GetCamera(0);

	// �ړ�����Ƃ��̏�Q���̓����蔻��Ɏg������
	MAP *map = GetMap(0);
	CRATE *crate = GetCrate(0);
	ROCK *rock = GetRock(0);
	TREE *tree = GetTree(0);
	GATE *gate = GetGate(0);

	int i;

	player->Vel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXVECTOR3 oldPos = player->Pos;



	//===========================================
	// �O��					forward and back
	if (GetKeyboardPress(DIK_W)
		|| IsButtonPressed(0, BUTTON_UP))
	{
		if (player->Vel.z < MOVE_VAL)
		{
			player->Vel.z += MOVE_VAL;
			if (GetKeyboardPress(DIK_S)
				|| IsButtonPressed(0, BUTTON_DOWN)) // ���������̎��A�����ɂ���
				player->Vel.z -= MOVE_VAL;
		}
	}
	else if (GetKeyboardPress(DIK_S)
		|| IsButtonPressed(0, BUTTON_DOWN))
	{
		if (player->Vel.z > -MOVE_VAL)
		{
			player->Vel.z -= MOVE_VAL;
		}
	}
	else
	{								// ���͂������ꍇ 0.0f �ɋ߂�����
		if (player->Vel.z > 0)
			player->Vel.z -= MOVE_VAL;
		else if (player->Vel.z < 0)
			player->Vel.z += MOVE_VAL;
	}

	//===========================================
	// ���E					right and left
	if (GetKeyboardPress(DIK_D)
		|| IsButtonPressed(0, BUTTON_RIGHT))
	{
		if (player->Vel.x < MOVE_VAL)
		{
			player->Vel.x += MOVE_VAL;
			if (GetKeyboardPress(DIK_A)
				|| IsButtonPressed(0, BUTTON_LEFT)) // ���������̎��A�����ɂ���
				player->Vel.x -= MOVE_VAL;
		}
	}
	else if (GetKeyboardPress(DIK_A)
		|| IsButtonPressed(0, BUTTON_LEFT))
	{
		if (player->Vel.x > -MOVE_VAL)
		{
			player->Vel.x -= MOVE_VAL;
		}
	}
	else
	{								// ���͂������ꍇ 0.0f �ɋ߂�����
		if (player->Vel.x > 0)
			player->Vel.x -= MOVE_VAL;
		else if (player->Vel.x < 0)
			player->Vel.x += MOVE_VAL;
	}

	//===========================================
	// �ړ��ʂ̌v�Z����
	if (player->Vel.x != 0.0f
		|| player->Vel.z != 0.0f)
	{
		// �J�����̊p�x
		D3DXVECTOR3 temp = camera->Eye - camera->At;
		temp.y = 0.0f;
		float tAng = atan2f(temp.z, temp.x);

		// �ړ��ʂ��K����
		float Angle = atan2f(player->Vel.z, player->Vel.x);
		player->Vel.x = cosf(Angle + tAng + (D3DX_PI * 0.5)) * MOVE_VAL;
		player->Vel.z = sinf(Angle + tAng + (D3DX_PI * 0.5)) * MOVE_VAL;

		player->Rot.y = -Angle - tAng + (D3DX_PI * 1);


	}


	// �ړ�
	player->Pos += player->Vel;

	if (player->StamTimer < 0)
		player->StamTimer++;
	else if (player->StamTimer > 0)
		player->StamTimer--;

	if (GetKeyboardPress(DIK_LSHIFT) && player->StamTimer <= 0
		|| IsButtonPressed(0, BUTTON_X) && player->StamTimer <= 0)
	{
		player->Pos += player->Vel;
		player->Stam -= STAM_DRAIN_RATE;
		
		// �G�t�F�N�g���Z�b�g
		SetEffect(player->Pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(0.65f, 0.05f, 0.85f, 0.75f), 16.0f, 16.0f, 30, Idx_DUST02);

		if (player->Stam <= 0)
			player->StamTimer = STAM_FULL_RESET_TIMER;
		else 
			player->StamTimer = STAM_RESET_TIMER;
	}
	else if (player->StamTimer == 0)
	{
		if (player->Stam < PLAYER_STAM_MAX)
			player->Stam += STAM_CHARGE_RATE;
		else if (player->Stam > PLAYER_STAM_MAX)
			player->Stam = PLAYER_STAM_MAX;
	}

	PlayerCollision(oldPos);


}

//=============================================================================
// �v���C���[�̓����菈��
void PlayerHit(int no)
{
	PLAYER *player = &playerWk[0];

	if (player->HP > 0)
		player->HP--;
	else
		player->alive = false;
	player->HitTime = IMMUNE_TIME;
}


//=============================================================================
// �v���C���[�̃Q�b�g�֐�
PLAYER *GetPlayer(int no)
{
	return (&playerWk[no]);
}