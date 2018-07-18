//=============================================================================
//
// �����蔻�菈�� [checkhit.cpp]
// Author : ���ꐶ
//
//=============================================================================
#include "checkhit.h"
#include "player.h"
#include "boar.h"
#include "car.h"
#include "crate.h"
#include "rock.h"
#include "tree.h"
#include "bullet.h"
#include "mathWk.h"
#include "map.h"
#include "gem.h"
#include "fence.h"
//=============================================================================
// �}�N����`
#define GROUND_CHECK_RANGE		(50.0f)

//=============================================================================
// �v���g�^�C�v�錾


//=============================================================================
// �O���[�o���ϐ�:


//=============================================================================
// �����蔻�菈��
void UpdateCheckhit(void)
{
	PLAYER	*player	= GetPlayer(0);
	GEM		*gem	= GetGem(0);
	BOAR	*boar	= GetBoar(0);

	int i, j;

	// �v���C���|�ƃW�F��		player and gem hit check	
	for (i = 0, player = GetPlayer(0); i < PLAYER_MAX; i++, player++)
		if (player->use)
			for (j = 0, gem = GetGem(0); j < GEM_MAX; j++, gem++)
				if (gem->mesh.use)
					if ((CheckHitBC(player->Pos, gem->mesh.Pos, 30, 30)) == true)
						gem->mesh.use = false;


	// �v���C���[�ƃC�m�V�V		player and boar hit check
	for (i = 0, player = GetPlayer(0); i < PLAYER_MAX; i++, player++)
		if (player->use && player->HitTime <= 0)
			for (j = 0, boar = GetBoar(0); j < BOAR_MAX;j++,boar++)
				if (boar->ene.use)
					if (CheckHitBC(player->Pos, boar->ene.Pos, 20, 20))
					{
						PlayerHit(i);
						break;
					}


}

//=============================================================================
// BC�̓����蔻�菈��
// pos1  : �I�u�W�F�P�̈ʒu			position of first object
// pos2  : �I�u�W�F�Q�̈ʒu			position of second object
// size1 : �I�u�W�F�P�̓�����͈�	radius of hit box of first object
// size2 : �I�u�W�F�Q�̓�����͈�	radius of hit box of second object
//=============================================================================
bool CheckHitBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float size1, float size2)
{

	D3DXVECTOR3 temp = pos1 - pos2;
	float LengthSq = D3DXVec3LengthSq(&temp);

	if (LengthSq <= ((size1 + size2)*(size1 + size2)))
	{
		return true;
	}
	return false;
}


//=============================================================================
// ���@�[�e�b�N�X�f�[�^�̐���
// environment hub.h �ŗp�ӂ���MESH�\���̂�n��
// MESH�̒���VtxPos������
//=============================================================================
void SetVtxData(MESH *mesh)
{
	int			VtxCnt = mesh->Mesh->GetNumVertices();	// ���V���̒��_��
	int			Polycnt = mesh->Mesh->GetNumFaces();		// ���V���̃|���S����
	int			BytePerVtx = mesh->Mesh->GetNumBytesPerVertex();	// �P���_������̃o�C�g��
	WORD		*idx;
	char		*pVtx;

	// �|���S���� * 3 �����C���f�b�N�X���K�v	
	// �C���f�b�N�X�ЂƂ̃T�C�Y��WORD�^ = 2�o�C�g�Ȃ̂�,
	// �C���f�b�N�X���i�[����z���(�|���S���� * 3���_ * 2�o�C�g)���������I�m��
	// �I������ free(mesh->IdxBuff); �Ń������J����Y�ꂸ��
	mesh->IdxBuff = (WORD*)malloc(sizeof(WORD) * Polycnt * 3);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	mesh->Mesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&idx);
	// �C���f�b�N�X�o�t�@�̏����擾
	for (int i = 0; i < (Polycnt * 3); i++)
	{
		mesh->IdxBuff[i] = idx[i];
	}

	// ���_�f�[�^���A�����b�N����
	mesh->Mesh->UnlockIndexBuffer();


	// ���_���W�ЂƂ̃T�C�Y��D3DXVECTOR3�^ = 12�o�C�g�Ȃ̂�,
	// ���_���W���i�[����z���(���_�� * 12�o�C�g)���������I�m��
	// �I������ free(mesh->VtxPos); �Ń������J����Y�ꂸ��
	mesh->VtxPos = (D3DXVECTOR3*)malloc(sizeof(D3DXVECTOR3) * VtxCnt);

	// �o�[�e�b�N�X�f�[�^�����b�N
	mesh->Mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtx);

	// �o�[�e�b�N�X�f�[�^���擾
	for (int i = 0; i < VtxCnt; i++)
	{
		mesh->VtxPos[i] = *(D3DXVECTOR3*)&pVtx[i * BytePerVtx];
	}

	// ���_�f�[�^���A�����b�N����
	mesh->Mesh->UnlockVertexBuffer();
}


//=============================================================================
// �|���S���Ɛ����Ƃ̓����蔻��
// p0  :�|���S���̒��_1		first vector of the polygon
// p1  :�|���S���̒��_2		second vector of the polygon
// p2  :�|���S���̒��_3		third vector of the polygon
// pos0:�n�_�i�ړ��O�j		starting position
// pos1:�I�_�i�ړ���j		ending position
// world: ���V���̃��[���h���W	world matrax of the mesh
// *distance : pos0 ����|���S���Ɛ����̌�_�܂ł̋���
//=============================================================================
int CheckHitLine(D3DXVECTOR3 p0, D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXMATRIX *world, D3DXVECTOR3 *distance)
{
	D3DXVECTOR3 norPlane;								// �|���S���̖@��

	D3DXVECTOR3 P0;
	D3DXVECTOR3 P1;
	D3DXVECTOR3 P2;

	// ���[�J�����W���烏�[���h���W�ɓ]��
	D3DXVec3TransformCoord(&P0, &p0, world);
	D3DXVec3TransformCoord(&P1, &p1, world);
	D3DXVec3TransformCoord(&P2, &p2, world);

	D3DXVECTOR3 v1 = P1 - P0;
	D3DXVECTOR3 v2 = P2 - P0;
	Vec3Cross(&norPlane, &v1, &v2);					// �O�ςŖ@�������߂�
	D3DXVec3Normalize(&norPlane, &norPlane);			// ���K��

	D3DXVECTOR3 vecFromP0ToPos0 = pos0 - P0;	// �|���S�����_0��������̎n�_�܂ł̃x�N�g��
	D3DXVECTOR3 vecFromP0ToPos1 = pos1 - P0;	// �|���S�����_0��������̏I�_�܂ł̃x�N�g��

	// ���ʂƐ����̓��ςƂ��ďՓ˂��Ă���\���𒲂ׂ�
	// ���߂��@���ƃx�N�g���Q�i�����̗��[�ƃ|���S����̔C�ӂ̓_�j��
	// ���ςƂ��ďՓ˂��Ă���\���𒲂ׂ�
	bool isCollisionPlaneToLineSegment;
	float dotPos0ToNor;
	float dotPos1ToNor;

	// �@���Ǝn�_�A�I�_�Ƃ̊e����
	dotPos0ToNor = dotProduct(&vecFromP0ToPos0, &norPlane);
	//dotPos1ToNor = -dotPos0ToNor;
	dotPos1ToNor = dotProduct(&vecFromP0ToPos1, &norPlane);

	isCollisionPlaneToLineSegment = (dotPos0ToNor * dotPos1ToNor <= 0);

	// ���ʂƐ������Փ˂��Ă��邩
	// �Փ˂��Ă���ꍇ�A�n�_�A�I�_���ʂ̕\���ɑ��݂��邱�ƂɂȂ�A
	// �Ȃ��p�����ꂼ��s�p�A�݊p�ɂȂ�̂œ��ς����ꂼ��قȂ镄��
	// �ɂȂ�B�قȂ镄�����|�����킹��ƕK�����ɂȂ邱�Ƃ𗘗p

	// ���ʓ������Ă��Ȃ�������I��
	if (!isCollisionPlaneToLineSegment)
	{
		return(0);	// �������Ă���\���͖���(�������Ă��Ȃ�)
	}

	// �ȉ��A�ڍׂȃ`�F�b�N //

	// �|���S���Ɛ����̌�_�����߂�

	// ����������߂�
	float distance0, distance1;
	distance0 = fabsf(dotProduct(&norPlane, &vecFromP0ToPos0));	// pos0���畽�ʂ܂ł̋���
																//distance1 = 1-distance0;	// pos1���畽�ʂ܂ł̋���
	distance1 = fabsf(dotProduct(&norPlane, &vecFromP0ToPos1));	// pos1���畽�ʂ܂ł̋���
	float internalRatio = distance0 / (distance0 + distance1);	// ������

																// p0�����_�ւ̃x�N�g��
	D3DXVECTOR3 vecFromP0ToPosIntersection; // p0�����_�ւ̃x�N�g��
	vecFromP0ToPosIntersection = ((1.0f - internalRatio) * vecFromP0ToPos0) + (internalRatio * vecFromP0ToPos1);

	// ��_�����߂�
	D3DXVECTOR3 posIntersectionPlaneToLine;
	posIntersectionPlaneToLine = P0 + vecFromP0ToPosIntersection;


	// �e�ӂƌ�_�̊֌W�𒲂ׂ� //
	const int MAX_VERTEX_POLYGON = 3;

	// �|���S���̊e�ӂ̃x�N�g��
	D3DXVECTOR3 vec[MAX_VERTEX_POLYGON];

	vec[0] = P1 - P0;
	vec[1] = P2 - P1;
	vec[2] = P0 - P2;

	// �e���_�ƌ�_�Ƃ̃x�N�g��
	D3DXVECTOR3 vecFromIntersection[MAX_VERTEX_POLYGON];

	vecFromIntersection[0] = posIntersectionPlaneToLine - P0;
	vecFromIntersection[1] = posIntersectionPlaneToLine - P1;
	vecFromIntersection[2] = posIntersectionPlaneToLine - P2;

	// �R�̃x�N�g���ɑ΂��ŏI�`�F�b�N
	for (int i = 0; i < MAX_VERTEX_POLYGON; i++)
	{// �e�ӂƌ�_�ւ̃x�N�g���̊O�ςŖ@�������߂Đ��K������
		D3DXVECTOR3 nor;
		Vec3Cross(&nor, &vec[i], &vecFromIntersection[i]);
		Vec3Normalize(&nor);
		if (dotProduct(&nor, &norPlane) <= 0)
		{// ���ςŌ����������Ă��邩���ׂ�
			return (0);	// �Ԃ����ĂȂ�
		}
	}


	*distance = posIntersectionPlaneToLine - pos1;

	// �Ԃ����Ă���
	return(1);
}


//=============================================================================
// pos0 �� pos1 �̊ԂɃI�u�W�F�N�g�����邩�m�F���鏈��
// pos0  : �n�_
// pos1  : �I�_
// *ObjPos : �Ԃɂ������I�u�W�F�N�g��Pos���Q�b�g
//=============================================================================
bool VisionCheck(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 *ObjPos)
{
	CRATE *crate = GetCrate(0);
	ROCK *rock = GetRock(0);
	TREE *tree = GetTree(0);
	FENCE *fence = GetFence(0);

	D3DXVECTOR3	v0	= pos1 - pos0;				// pos0����pos1�܂ł̃x�N�g��
	float		L0	= D3DXVec3LengthSq(&v0);	// pos0����pos1�܂ł̋���
	D3DXVECTOR3 v1;								// pos0����`�F�b�N����I�u�W�F�N�g�܂ł̃x�N�g��
	float		L1;								// pos0����`�F�b�N����I�u�W�F�N�g�܂ł̋���
	D3DXVECTOR3 temp = { 0.0f,0.0f,0.0f };
	int i;
	// ���Ƃ̃r�W�����`�F�b�N
	for (i = 0, crate = GetCrate(0); i < CRATE_MAX; i++, crate++)
		if (crate->mesh.use == true)
		{
			v1 = crate->mesh.Pos - pos0;
			L1 = D3DXVec3LengthSq(&v1);
			if (L1 < L0)
			{
				if (CheckHitLinePrep(pos0, pos1, &temp, &crate->mesh))
				{
					*ObjPos = crate->mesh.Pos;
					return (true);
				}
			}
		}

	// �΂Ƃ̃r�W�����`�F�b�N
	for (i = 0, rock = GetRock(0); i < ROCK_MAX; i++, rock++)
		if (rock->mesh.use == true
			&& rock->isHitbox == true)
		{
			v1 = rock->mesh.Pos - pos0;
			L1 = D3DXVec3LengthSq(&v1);
			if (L1 < L0)
			{
				if (CheckHitLinePrep(pos0, pos1, &temp, &rock->mesh))
				{
					*ObjPos = rock->mesh.Pos;
					return (true);
				}
			}
		}

	// �؂Ƃ̃r�W�����`�F�b�N
	for (i = 0, tree = GetTree(0); i < TREE_MAX; i++, tree++)
		if (tree->mesh.use == true)
		{
			v1 = tree->mesh.Pos - pos0;
			L1 = D3DXVec3LengthSq(&v1);
			if (L1 < L0)
			{
				if (CheckHitLinePrep(pos0, pos1, &temp, &tree->mesh))
				{
					*ObjPos = tree->mesh.Pos;
					return (true);
				}
			}
		}

	// ��Ƃ̃r�W�����`�F�b�N
	for (i = 0, fence = GetFence(0); i < FENCE_MAX; i++, fence++)
		if (fence->mesh.use == true
			&& fence->isHitbox == true)
		{
			v1 = fence->mesh.Pos - pos0;
			L1 = D3DXVec3LengthSq(&v1);
			if (L1 < L0)
			{
				if (CheckHitLinePrep(pos0, pos1, &temp, &fence->mesh))
				{
					*ObjPos = fence->mesh.Pos;
					return (true);
				}
			}
		}
	// �Ԃɉ����Ȃ����
	return (false);
}







//=============================================================================
// �����̓����蔻��̏�������
// pos0 : �n�_
// pos1 : �I�_
// *distance : pos0����|���S���Ɛ����̌�_�܂ł̃x�N�g��
// *mesh : �����蔻��̑ΏۂɂȂ郁�V���^�̃I�u�W�F�N�g
//=============================================================================
bool CheckHitLinePrep(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 *distance, MESH *mesh)
{
	bool	ans = false;		// ����

	D3DXVECTOR3		pos[3];		// �|���S���̖@��


	int PolyCnt = mesh->Mesh->GetNumFaces();

	for (int i = 0; i<PolyCnt; i++)
	{	// �|���S���̒��_�����߂�

		int idx0 = mesh->IdxBuff[i * 3];
		int idx1 = mesh->IdxBuff[i * 3 + 1];
		int idx2 = mesh->IdxBuff[i * 3 + 2];

		// ���_���W�̐ݒ�
		pos[0] = mesh->VtxPos[idx0];
		pos[1] = mesh->VtxPos[idx1];
		pos[2] = mesh->VtxPos[idx2];

		// �|���S���Ɛ����̓����蔻��
		ans = CheckHitLine(pos[0], pos[1], pos[2], pos0, pos1, &mesh->mtxWorld, distance);
		if (ans == true)
		{
			break;
		}


	}

	return(ans);

}


//=============================================================================
// �����̓����蔻��̏�������
// pos0 : �n�_
// pos1 : �I�_
// *distance : pos0����|���S���Ɛ����̌�_�܂ł̃x�N�g��
//=============================================================================
bool CheckHitGroundPrep(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 *distance)
{
	MAP *map = GetMap(2);

	bool	ans = false;		// ����

	D3DXVECTOR3		pos[3];		// �|���S���̖@��

	int PolyCnt = map->mesh.Mesh->GetNumFaces();

	for (int i = 0; i<PolyCnt; i++)
	{	// �|���S���̒��_�����߂�

		int idx0 = map->mesh.IdxBuff[i * 3];
		int idx1 = map->mesh.IdxBuff[i * 3 + 1];
		int idx2 = map->mesh.IdxBuff[i * 3 + 2];

		// ���_���W�̐ݒ�
		pos[0] = map->mesh.VtxPos[idx0];
		pos[1] = map->mesh.VtxPos[idx1];
		pos[2] = map->mesh.VtxPos[idx2];

		// �|���S���Ɛ����̓����蔻��
		ans = CheckHitLine(pos[0], pos[1], pos[2], pos0, pos1, &map->mesh.mtxWorld, distance);
		if (ans != 0)
		{
			break;
		}
	}
	return(ans);
}


//=============================================================================
// �����̓����蔻��̏�������
// pos0 : �n�_
// pos1 : �I�_
// *distance : pos0����|���S���Ɛ����̌�_�܂ł̃x�N�g��
//=============================================================================
bool CheckHitWallPrep(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 *distance)
{
	MAP *map = GetMap(3);

	bool	ans = false;	// ����

	D3DXVECTOR3		pos[3];	// �|���S���̖@��

	int PolyCnt = map->mesh.Mesh->GetNumFaces();

	for (int i = 0; i<PolyCnt; i++)
	{	// �|���S���̒��_�����߂�

		int idx0 = map->mesh.IdxBuff[i * 3];
		int idx1 = map->mesh.IdxBuff[i * 3 + 1];
		int idx2 = map->mesh.IdxBuff[i * 3 + 2];

		// ���_���W�̐ݒ�
		pos[0] = map->mesh.VtxPos[idx0];
		pos[1] = map->mesh.VtxPos[idx1];
		pos[2] = map->mesh.VtxPos[idx2];

		// �|���S���Ɛ����̓����蔻��
		ans = CheckHitLine(pos[0], pos[1], pos[2], pos0, pos1, &map->mesh.mtxWorld, distance);
		if (ans != 0)
		{
			break;
		}
	}
	return(ans);
}