//=============================================================================
//
// �Ώ��� [rock.h]
// Author : ���ꐶ
//
//=============================================================================
#ifndef _ROCK_H_
#define _ROCK_H_

#include "main.h"
#include "environment hub.h"

//=============================================================================
// �}�N����`

#define COMP4ROCK		(2)		// ��̐΂𐻍삷�̂ɕK�v�Ȑ�
#define ROCK_MAX		(15 * COMP4ROCK)

#define ROCK_DATA_START	("##Rock")	// �΂̃f�[�^�̎n�܂������

//=============================================================================
// �\���̐錾

typedef struct
{
	MESH		mesh;		// ���b�V���I�u�W�F�N�g�̊�{�\����
	bool		isHitbox;	// �����蔻��p�̃��b�V���ł��邩
	int			TypeIdx;
}ROCK;



//=============================================================================
// �v���g�^�C�v�錾
HRESULT InitRock(void);
void UninitRock(void);
void UpdateRock(void);
void DrawRock(void);
int hitCheckRock(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);

HRESULT MakeVertexRock(LPDIRECT3DDEVICE9 pDevice);
ROCK *GetRock(int no);
#endif
