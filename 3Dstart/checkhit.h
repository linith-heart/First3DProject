//=============================================================================
//
// �����蔻��w�b�_�[ [checkhit.h]
// Author : ���ꐶ
//
//=============================================================================
#ifndef _CHECKHIT_H_
#define _CHECKHIT_H_

#include "main.h"
#include "environment hub.h"

//=============================================================================
// �v���g�^�C�v�錾


void UpdateCheckhit(void);

// 
bool CheckHitBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float size1, float size2);




// �����̓����蔻��
void SetVtxData(MESH *mesh);

int CheckHitLine(D3DXVECTOR3 p0, D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXMATRIX *world, D3DXVECTOR3 *distance);

bool VisionCheck(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 *ObjPos);

bool CheckHitLinePrep(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 *distance, MESH *mesh);

bool CheckHitGroundPrep(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 *distance);

bool CheckHitWallPrep(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 *distance);

#endif