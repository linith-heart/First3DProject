//=============================================================================
//
// 計算処理 [mathWk.cpp]
// Author : 川井一生
/*
mathWk.cpp では計算の処理をしている

便利関数はほぼ全部あるのですが勉強のため自分で計算式などをしている


mathWk.cpp is for math workthough directx functions do exist 

this is purely for studies
*/
//=============================================================================
#include "mathWk.h"



//=============================================================================
// クオータニオンの掛け算
D3DXVECTOR4 QuaternionMulty(D3DXVECTOR4 q1, D3DXVECTOR4 q2)
{
	D3DXVECTOR4 qOut = { 0.0f,0.0f,0.0f,0.0f };

	qOut.w = q1.w *q2.w - q1.x *q2.x - q1.y *q2.y - q1.z *q2.z;
	qOut.x = q1.w *q2.x + q2.w *q1.x + q1.y *q2.z - q1.z *q2.y;
	qOut.y = q1.w *q2.y + q2.w *q1.y + q1.z *q2.x - q1.x *q2.z;
	qOut.z = q1.w *q2.z + q2.w *q1.z + q1.x *q2.y - q1.y *q2.x;

	return (qOut);
}



//=============================================================================
// 内積
float dotProduct(D3DXVECTOR3 *v1, D3DXVECTOR3 *v2)
{
	float fOut = 0;

	fOut = (v1->x*v2->x) + (v1->y*v2->y) + (v1->z*v2->z);

	return(fOut);
}

//=============================================================================
// 三次元の外積
D3DXVECTOR3 *Vec3Cross(D3DXVECTOR3 *Out, D3DXVECTOR3 *vl, D3DXVECTOR3 *vr)
{
	Out->x = (vl->y*vr->z) - (vr->y*vl->z);
	Out->y = (vl->z*vr->x) - (vr->z*vl->x);
	Out->z = (vl->x*vr->y) - (vr->x*vl->y);

	return(Out);
}

//=============================================================================
// 右にいるか		check if object is on right side
// vl	: 対象
// vr	: チェックするもの
// <= 0 : 右
// >= 0 : 左
//=============================================================================
float Vec2Cross(D3DXVECTOR3 vl, D3DXVECTOR3 vr)
{

	float tOut;

	tOut = (vl.x*vr.z) - (vl.z*vr.x);

	return (tOut);
}


//=============================================================================
// 正規化
D3DXVECTOR3 *Vec3Normalize(D3DXVECTOR3 *v)
{
	float tLen = sqrt(v->x*v->x + v->y*v->y + v->z*v->z);					// 正規化
	v->x /= tLen;
	v->y /= tLen;
	v->z /= tLen;

	return(v);
}

//=============================================================================
// Rot値が示す方向へベクトルを作る
D3DXVECTOR3 Ang2Vec(D3DXVECTOR3 Rot, float Radius)
{
	D3DXVECTOR3 Out = { 0.0f,0.0f,0.0f };
	Out.x = (cosf(-Rot.y + (D3DX_PI * -0.5f))*cosf(Rot.x)) * Radius;
	Out.z = (sinf(Rot.y + (D3DX_PI * -0.5f))*cosf(Rot.x)) * Radius;
	Out.y = sinf(Rot.x) * Radius;

	return (Out);
}

//=============================================================================
// 内積を使って前にいるかいないか
// pos0 : 基準位置		base position
// pos1 : 対象位置		target position
// rot  : 基準となるものの方向・角度	the angle of witch the base is pointed
//=============================================================================
bool isInFront(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 rot)
{
	D3DXVECTOR3 temp3 = pos0 - pos1;
	float distance = D3DXVec3Length(&temp3);
	D3DXVECTOR3 tpos0 = pos0 - pos1;
	temp3 = Ang2Vec(rot, 0.1f);
	return(dotProduct(&temp3, &tpos0) > 0);
}

//=============================================================================
// 外積を使って視野内にいるか
// pos0 : 基準位置
// pos1 : 対象位置
// rot	: 基準となるものの角度
// rad	: 基準となるものの視野範囲
//=============================================================================
bool inView(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 rot, float rad)
{
	float tAng = acosf(rad);

	D3DXVECTOR3 left = rot;
	left.y += tAng;
	left = Ang2Vec(left, 1);
	D3DXVECTOR3 right = rot;
	right.y -= tAng;
	right = Ang2Vec(right, 1);

	D3DXVECTOR3 temp3 = pos1 - pos0;

	if (Vec2Cross(right, temp3) <= 0
		&& Vec2Cross(left, temp3) >= 0)
	{
		return (true);
	}
	return (false);
}
