//=============================================================================
//
// コリジョン処理 [collision.cpp]
// Author : 川井一生
/*
colision.cpp 物体との当たり判定をしている

メンテナンス性を考えプレイヤーと物体、エネミーと物体の当たり判定
をcollision.cppで一括管理


collision.cpp is the hub for hitscan using line segment with objects
things like player and objects and enemy and objects process will be here 
*/
//=============================================================================
#include "collision.h"
#include "game.h"
#include "input.h"
#include "checkhit.h"
#include "mathWk.h"

//===============================================
// プレイヤー
#include "player.h"

//===============================================
// エネミー
#include "boar.h"

//===============================================
//マップ
#include "map.h"

//===============================================
// オブジェクト
#include "crate.h"
#include "rock.h"
#include "tree.h"
#include "gate.h"
#include "fence.h"
#include "stage.h"

//=============================================================================
// マクロ定義

#define WALL_LEN		(500)					// ＿＿から対象の壁ポリゴンの距離が＿＿いないか
#define HIGHT_GROUND	(500.0f)				// 地面との当たり判定の線分の高さ
#define HIGHT_VALUE		(100.0f)				// オブジェクトとの当たり判定の線分の高さ
#define HIGHT_RETURN	(90.0f)					// 線分の戻す量

//=============================================================================
// グローバル宣言


//=============================================================================
// デバッグ用（飛ぶ処理）
#ifdef _DEBUG
bool					Fly = false;
void UpdateCollision(void)
{
	PLAYER *player = GetPlayer(0);

	if (GetKeyboardTrigger(DIK_0))
	{
		if (Fly)
			Fly = false;
		else if (!Fly)
			Fly = true;
	}

	if (Fly)
	{
		if (GetKeyboardPress(DIK_R))
			player->Pos.y += 2.0f;
		if (GetKeyboardPress(DIK_F))
			player->Pos.y -= 2.0f;
	}

}
#endif // _DEBUG


//=============================================================================
// プレイヤ－のコリジョン処理
void PlayerCollision(D3DXVECTOR3 oldPos)
{
	PLAYER	*player	= GetPlayer(0);
	CRATE	*crate	= GetCrate(0);
	ROCK	*rock	= GetRock(0);
	FENCE	*fence	= GetFence(0);
	TREE	*tree	= GetTree(0);
	GATE	*gate	= GetGate(0);
	STAGE	*stage	= GetStage(0);

	int i;
	// 移動前と現在地が一緒かチェック
	D3DXVECTOR3		vec = player->Vel - oldPos;
	float			len = D3DXVec3Length(&vec);
	if (len > 0.1f)
	{	// 阿多あり判定の準備		prep for check hit
		player->Vel = player->Pos + player->Vel;
		D3DXVECTOR3 tDistance;
		D3DXVECTOR3 tVel = player->Vel;
		D3DXVECTOR3 tPos = oldPos;
		D3DXVECTOR3 tsize = Ang2Vec(player->Rot, 10);

		// モデルの見た目にコリジョンを合わせてみる		fix collision to match player modle
		tVel.y += 10;
		tPos.y += 10;
		//===========================================================
		// 箱との当たり判定
		for (i = 0, crate = GetCrate(0); i < CRATE_MAX; i++, crate++)
			if (crate->isSolid == true
				&& crate->mesh.use == true)
			{
				vec = player->Pos - crate->mesh.Pos;
				len = D3DXVec3Length(&vec);
				if (len < 200)
					if (CheckHitLinePrep(tPos, tVel, &tDistance, &crate->mesh))
					{
						// 当たっているので、当たった交点までの距離を足す
						player->Pos += tDistance;
						break;
					}
			}
		//===========================================================
		// 石の当たり判定
		for (i = 0, rock = GetRock(0); i < ROCK_MAX; i++, rock++)
			if (rock->mesh.use == true
				&& rock->isHitbox == true)
			{
				vec = player->Pos - rock->mesh.Pos;
				len = D3DXVec3Length(&vec);
				if (len < 400)
					if (CheckHitLinePrep(tPos, tVel, &tDistance, &rock->mesh))
					{
						// 当たっているので、当たった交点までの距離を足す
						player->Pos += tDistance - tsize;
						break;
					}
			}
		//===========================================================
		// 策の当たり判定
		for (i = 0, fence = GetFence(0); i < FENCE_MAX; i++, fence++)
			if (fence->mesh.use == true
				&& fence->isHitbox == true)
			{
				vec = player->Pos - fence->mesh.Pos;
				len = D3DXVec3Length(&vec);
				if (len < 400)
					if (CheckHitLinePrep(tPos, tVel, &tDistance, &fence->mesh))
					{
						// 当たっているので、当たった交点までの距離を足す
						player->Pos += tDistance;
						break;
					}
			}

		//===========================================================
		// 木の当たり判定
		for (i = 0, tree = GetTree(0); i < TREE_MAX; i++, tree++)
			if (tree->mesh.use == true)
			{
				vec = player->Pos - tree->mesh.Pos;
				len = D3DXVec3Length(&vec);
				if (len < 200)
					if (CheckHitLinePrep(tPos, tVel, &tDistance, &tree->mesh))
					{
						// 当たっているので、当たった交点までの距離を足す
						player->Pos += tDistance;
						break;
					}
			}
		//===========================================================
		// 門の当たり判定
		for (i = 0, gate = GetGate(0); i < GATE_MAX; i++, gate++)
			if (gate->mesh.use == true
				&& gate->isHitbox == true)
			{
				vec = player->Pos - gate->mesh.Pos;
				len = D3DXVec3Length(&vec);
				if (len < 500)
					if (CheckHitLinePrep(tPos, tVel, &tDistance, &gate->mesh))
					{
						// 当たっているので、当たった交点までの距離を足す
						player->Pos += tDistance;
						break;
					}
			}



		// 壁との当たり判定
		D3DXVECTOR3 distance = { 0.0f,0.0f,0.0f };	// 壁までの距離
		D3DXVECTOR3 head = oldPos;
		D3DXVECTOR3 feet = player->Pos;
		//D3DXVECTOR3 temp = Ang2Vec(player->Rot, 100);

		feet += tsize;
		head.y += 5;
		feet.y += 5;
		if (CheckHitWallPrep(feet, head, &distance))
		{
			float tLen = D3DXVec3LengthSq(&distance);
			if (tLen < WALL_LEN)
			{
				player->Pos += distance - tsize;
			}
		}
	}
#ifdef _DEBUG
	if (!Fly)
#endif // DEBUG
	{
		// 地面との当たり判定
		D3DXVECTOR3 distance = { 0.0f,0.0f,0.0f };	// 地面までの距離
		D3DXVECTOR3 head = player->Pos;
		D3DXVECTOR3 feet = player->Pos;
		head.y += HIGHT_GROUND;
		feet.y += player->Grav;

		bool tFlg = false;

		if (CheckHitGroundPrep(head, feet, &distance))
			tFlg = true;

		head.y -= HIGHT_GROUND;
		head.y += HIGHT_VALUE;
		// 箱との当たり判定
		if (tFlg == false)
			for (i = 0, crate = GetCrate(0); i < CRATE_MAX; i++, crate++)
				if (crate->isSolid == true
					&& crate->mesh.use == true)
				{
					vec = player->Pos - crate->mesh.Pos;
					len = D3DXVec3Length(&vec);
					if (len < 200
						&& player->Pos.y > crate->mesh.Pos.y)
						if (CheckHitLinePrep(head, feet, &distance, &crate->mesh))
							tFlg = true;
				}
		// 石との当たり判定
		if (tFlg == false)
			for (i = 0, rock = GetRock(0); i < ROCK_MAX; i++, rock++)
				if (rock->mesh.use == true
					&& rock->isHitbox == true)
				{
					vec = player->Pos - rock->mesh.Pos;
					len = D3DXVec3Length(&vec);
					if (len < 400
						&& player->Pos.y > rock->mesh.Pos.y)
						if (CheckHitLinePrep(head, feet, &distance, &rock->mesh))
							tFlg = true;
				}
		// 台との当たり判定
		if (tFlg == false)
			for (i = 0, stage = GetStage(0); i < STAGE_MAX; i++, stage++)
				if (stage->mesh.use == true
					&& stage->isHitbox == true)
				{
					vec = player->Pos - stage->mesh.Pos;
					len = D3DXVec3Length(&vec);
					if (len < 700)
						if( player->Pos.y > stage->mesh.Pos.y)
						if (CheckHitLinePrep(head, feet, &distance, &stage->mesh))
							tFlg = true;
				}

		if (tFlg == true)
		{
			if (player->Grav <= 0)
			{
				player->Pos.y += distance.y + player->Grav - 2;
				player->Grav = 0.0f; // 重力をリセット
				player->Jump = false;
			}
			else
			{
				if (player->Grav == 0.0f)
					player->Grav = 1.0f;
				player->Grav -= GRAVITY;
				player->Pos.y += player->Grav;
			}
		}
		else
		{
			player->Grav -= GRAVITY;
			player->Pos.y += player->Grav;
		}
	}



}


//=============================================================================
// 地面のコリジョン処理
void GroundCollision(D3DXVECTOR3 *Pos, float *Grav)
{
	// 地面との当たり判定
	D3DXVECTOR3 distance = { 0.0f,0.0f,0.0f };	// 地面までの距離
	D3DXVECTOR3 head = *Pos;
	D3DXVECTOR3 feet = *Pos;
	head.y += HIGHT_VALUE;
	feet.y -= HIGHT_VALUE;

	if (CheckHitGroundPrep(head, feet, &distance))
	{
		Pos->y += distance.y - HIGHT_RETURN;
		*Grav = 0.0f;
	}
	else
	{
		*Grav -= GRAVITY;
		Pos->y += *Grav;
	}
}
