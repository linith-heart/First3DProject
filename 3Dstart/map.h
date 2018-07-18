//=============================================================================
//
// �}�b�v���� [map.h]
// Author : ���ꐶ
//
//=============================================================================
#ifndef _MAP_H_
#define _MAP_H_

#include "main.h"
#include "environment hub.h"

//=============================================================================
// �}�N����`

#define COMP4MAP	(4)			// ��̃}�b�v�ɕK�v�ȃ��b�V���̐�		count of components required to create the map

#define MAP_MAX		(1 * COMP4MAP)		// �\���p�Ɠ����蔻��p


//=============================================================================
// �\���̐錾
typedef struct
{
	MESH					mesh;
	bool					isHitBox;			// �����蔻��p�ł��邩
}MAP;

//=============================================================================
// �v���g�^�C�v�錾
HRESULT InitMap(void);
void UninitMap(void);
void UpdateMap(void);
void DrawMap(void);
MAP *GetMap(int no);

#endif
