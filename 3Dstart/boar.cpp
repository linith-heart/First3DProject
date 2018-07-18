//=============================================================================
//
// イノシシ処理 [boar.cpp]
// Author : 川井一生
/*
エネミー（イノシシ）
前後と回転で移動

enemy(boar)
moves forward and back
rotates to face player
*/
//=============================================================================
#include "enemy.h"
#include "game.h"
#include "camera.h"
#include "player.h"
#include "boar.h"
#include "checkhit.h"
#include "mathWk.h"
#include "collision.h"
#include "effect.h"
#include "rock.h"
#include "crate.h"
#include "status icon.h"

#include "fence.h"


//=============================================================================
// マクロ定義
#define TEXTURE			("data/TEXTURE/boar000.png")
#define MODEL			("data/MODEL/boar000.x")
#define	MOVE_VAL		(2.0f)			// 移動量
#define CHASE_VAL		(4.0f)			// 追いかけるときの移動量
#define	ROT_VAL			(0.1f)			// 回転量
#define CHECK_ROT		(0.03f)			// チェック時の回転量
#define SCL_VALUE		(0.3f)			// 大きさ
#define AGGRO_RANGE		(500)			// アグロ距離
#define CHECK_RANGE		(200)			// チェック距離
#define LEASH_RANGE		(1500)			// 追跡する距離
#define VISION_RAD		(0.7)			// 見える範囲
#define VISION_ROAM		(0.95)			// 前方の範囲


#define OBJECT_CHECK_RANGE	(200)		// 障害物のチェックの範囲


#define IDLE_TIME		(1*60)			// アイドルの時間


#define HIGHT_VALUE		(5.0f)			// 足の位置

#define WATER_HEIGHT	(-100)			// 水面の高さ

enum
{
	IS_FALSE = 0,
	IN_CHECK,
	IN_VISION
};

//=============================================================================
// プロトタイプ宣言
void ResetBoar(int no);
void MoveBoar(int no);

int BoarVisionCheck(int no);
void SetBoarPos(int no);
void SetRoamingPos(int no);

//=============================================================================
// グローバル変数
LPDIRECT3DTEXTURE9		D3DTextureBoar = NULL;	// テクスチャへのポインタ

BOAR					boarWk[BOAR_MAX];			// フィールドへのポインタ

//=============================================================================
// 初期化処理
HRESULT InitBoar(void)
{
	BOAR *boar = &boarWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの設定
	D3DXCreateTextureFromFile(pDevice, TEXTURE, &D3DTextureBoar);

	FILE *fp;
	// エネミーデータが入ってるファイルを開く
	fp = fopen(ENEMY_DATA_FILE, "r");
	if (fp == NULL) {
		perror("Error opening file");
		return(-1);
	}


	int i;
	for (i = 0; i < BOAR_MAX; i++, boar++)
	{
		boar->Stat = IDLING;
		boar->IdleTime = IDLE_TIME;

		// 使用するか、ポップ位置、行動範囲をファイルからゲット
		fscanf(fp, "%d,%f,%f,%f,%f,%f", &boar->ene.use, &boar->StaringPos.x, &boar->StaringPos.y, &boar->StaringPos.z, &boar->RoamDistance.x, &boar->RoamDistance.y);

		boar->ene.Pos = boar->StaringPos;

		// 回転とスケールの初期設定
		boar->ene.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		boar->ene.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);
		boar->ene.Grav = 0;

		// モデル関係の初期化
		//D3DTextureBoar = NULL;
		boar->ene.Mesh = NULL;
		boar->ene.BuffMat = NULL;
		boar->ene.NumMat = 0;

		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(MODEL,			// モデルデータ
			D3DXMESH_SYSTEMMEM,				// 使用するメモリのオプション
			pDevice,						// デバイス 
			NULL,							// 未使用
			&boar->ene.BuffMat,				// マテリアルデータへのポインター
			NULL,							// 未使用
			&boar->ene.NumMat,				// D3DXMATERIAL構造体の数
			&boar->ene.Mesh)))				// メッシュデータへのポインター
		{
			return E_FAIL;
		}
	}

	fclose(fp);
	return S_OK;

}

//=============================================================================
// イノシシのリセット処理
void ResetBoar(int no)
{
	BOAR *boar = &boarWk[0];

	boar->ene.use = true;
	boar->Stat = IDLING;
	boar->IdleTime = IDLE_TIME;

	// 位置・回転・スケールの初期設定
	boar->ene.Pos = boar->StaringPos;
	boar->ene.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	boar->ene.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);
	boar->ene.Grav = 0;

}

//=============================================================================
// 終了処理
void UninitBoar(void)
{
	BOAR *boar = &boarWk[0];

	if (D3DTextureBoar != NULL)
	{// テクスチャの開放
		D3DTextureBoar->Release();
		D3DTextureBoar = NULL;
	}
	for (int i = 0; i < BOAR_MAX; i++, boar++)
	{
		if (boar->ene.VtxBuff != NULL)
		{// 頂点バッファの開放
			boar->ene.VtxBuff->Release();
			boar->ene.VtxBuff = NULL;
		}
		if (boar->ene.Mesh != NULL)
		{// メシュの解放
			boar->ene.Mesh->Release();
			boar->ene.Mesh = NULL;
		}

	}
}

//=============================================================================
// 更新処理
void UpdateBoar(void)
{
	BOAR *boar = &boarWk[0];
	PLAYER *player = GetPlayer(0);
	D3DXVECTOR3 Vel = VEC3CLEAR;
	D3DXVECTOR3 temp = VEC3CLEAR;
	D3DXVECTOR3 temp2 = VEC3CLEAR;
	D3DXVECTOR3 ObjPos = VEC3CLEAR;
	float tLen = 0.0f;
	int i;
	for (i = 0; i < BOAR_MAX; i++, boar++)
	{
		if (boar->ene.use == true)
		{
			// マップから落ちた時にリセットで戻す
			if (boar->ene.Pos.y < WATER_HEIGHT)
			{
				ResetBoar(i);
			}

			switch (boar->Stat) // 現在の状況を検査
			{//============================================ IDLING
			case IDLING:
				// アイドルに切り替えたときに時間がゼロならセット
				if (boar->IdleTime <= 0)
					boar->IdleTime = IDLE_TIME;

				switch (BoarVisionCheck(i))
				{	// チェック範囲
				case IN_CHECK:
					temp = boar->ene.Pos;
					temp.y += 100;
					SetStatIco(temp, Idx_CHECK);

					boar->Stat = CHECKING;
					boar->IdleTime = 0;
					break;
					// 視野範囲
				case IN_VISION:
					temp = boar->ene.Pos;
					temp.y += 100;
					SetStatIco(temp, Idx_FOUND);

					boar->Stat = CHASING;
					boar->IdleTime = 0;
					break;
				}

				boar->IdleTime--;
				// アイドル中に時間が来たら切り替え
				if (boar->IdleTime <= 0)
				{
					boar->Stat = ROAMING;
					SetRoamingPos(i);
				}
				break;
				//========================================= ROAMING
			case ROAMING:

				switch (BoarVisionCheck(i))
				{	// チェック範囲
				case IN_CHECK:
					temp = boar->ene.Pos;
					temp.y += 100;
					SetStatIco(temp, Idx_CHECK);

					boar->Stat = CHECKING;
					boar->IdleTime = 0;
					break;
					// 視野範囲
				case IN_VISION:
					temp = boar->ene.Pos;
					temp.y += 100;
					SetStatIco(temp, Idx_FOUND);

					boar->Stat = CHASING;
					boar->IdleTime = 0;
					break;
				}

				temp = boar->ene.Pos - boar->RoamPos;
				tLen = D3DXVec3Length(&temp);
				if (tLen < 470)
				{
					boar->Stat = IDLING;
					boar->IdleTime = IDLE_TIME;
				}

				// イノシシの移動
				MoveBoar(i);


				// 目の前に障害物があるかチェック
				temp = boar->ene.Pos + Ang2Vec(boar->ene.Rot, OBJECT_CHECK_RANGE);
				if (VisionCheck(boar->ene.Pos, temp, &ObjPos))
				{
					temp = ObjPos - boar->ene.Pos;
					temp2 = Ang2Vec(boar->ene.Rot, 1);
					if (Vec2Cross(temp2, temp) <= 0)
						boar->ene.Rot.y -= ROT_VAL;
					else
						boar->ene.Rot.y += ROT_VAL;
					break;
				}

				// 目的地が目の前にあるか
				if (!inView(boar->ene.Pos, boar->RoamPos, boar->ene.Rot, VISION_ROAM))
				{	// 無ければ右か左へ回転
					temp = boar->RoamPos - boar->ene.Pos;
					temp2 = Ang2Vec(boar->ene.Rot, 1);
					if (Vec2Cross(temp2, temp) <= 0)
						boar->ene.Rot.y += ROT_VAL / 2;
					else
						boar->ene.Rot.y -= ROT_VAL / 2;
				}
				break;
				//========================================= CHASING
			case CHASING:
				temp = player->Pos - boar->ene.Pos;
				tLen = D3DXVec3Length(&temp);
				if (tLen > AGGRO_RANGE)
				{
					boar->Stat = IDLING;
					break;
				}
				temp = boar->StaringPos - boar->ene.Pos;
				tLen = D3DXVec3Length(&temp);
				if (tLen > LEASH_RANGE)
				{
					boar->Stat = IDLING;
					break;
				}
				MoveBoar(i);

				// 目の前に障害物があるかチェック
				temp = boar->ene.Pos + Ang2Vec(boar->ene.Rot, OBJECT_CHECK_RANGE);
				if (VisionCheck(boar->ene.Pos, temp, &ObjPos))
				{
					temp = ObjPos - boar->ene.Pos;
					temp2 = Ang2Vec(boar->ene.Rot, 1);
					if (Vec2Cross(temp2, temp) <= 0)
						boar->ene.Rot.y -= ROT_VAL;
					else
						boar->ene.Rot.y += ROT_VAL;
					break;
				}
				// 目的地が目の前にあるか
				if (!inView(boar->ene.Pos, player->Pos, boar->ene.Rot, VISION_ROAM))
				{	// 無ければ右か左へ回転
					temp = player->Pos - boar->ene.Pos;
					temp2 = Ang2Vec(boar->ene.Rot, 1);
					if (Vec2Cross(temp2, temp) <= 0)
						boar->ene.Rot.y += ROT_VAL;
					else
						boar->ene.Rot.y -= ROT_VAL;
				}
				break;
				//========================================= CHECKING
			case CHECKING:
				temp = player->Pos - boar->ene.Pos;
				tLen = D3DXVec3Length(&temp);
				if (tLen < AGGRO_RANGE)
				{
					// 目的地が目の前にあるか
					if (!inView(boar->ene.Pos, player->Pos, boar->ene.Rot, VISION_ROAM))
					{	// 無ければ右か左へ回転
						temp = player->Pos - boar->ene.Pos;
						temp2 = Ang2Vec(boar->ene.Rot, 1);
						if (Vec2Cross(temp2, temp) <= 0)
							boar->ene.Rot.y += CHECK_ROT;
						else
							boar->ene.Rot.y -= CHECK_ROT;
					}
					else
					{
						temp = boar->ene.Pos;
						temp.y += 100;
						SetStatIco(temp, Idx_FOUND);

						boar->Stat = CHASING;
					}
				}
				else
				{
					boar->Stat = IDLING;
					boar->IdleTime = IDLE_TIME;
				}
				break;
			}
		}
	}
}

//=============================================================================
// 描画処理
void DrawBoar(void)
{
	BOAR *boar = &boarWk[0];
	CAMERA *camera = GetCamera(0);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// スケール、回転、平行移動
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	int i, j;

	for (i = 0; i < BOAR_MAX; i++, boar++)
		if (boar->ene.use == true)
		{
			// ワールドマトリクス初期化
			D3DXMatrixIdentity(&boar->ene.mtxWorld);

			// スケールを反映	(S)
			D3DXMatrixScaling(&mtxScl, boar->ene.Scl.x, boar->ene.Scl.y, boar->ene.Scl.z);
			D3DXMatrixMultiply(&boar->ene.mtxWorld, &boar->ene.mtxWorld, &mtxScl); 	// &ene->mtxWorld = &ene->mtxWorld * &mtxScl
			// 回転を反映		(R)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, boar->ene.Rot.y, boar->ene.Rot.x, boar->ene.Rot.z);
			D3DXMatrixMultiply(&boar->ene.mtxWorld, &boar->ene.mtxWorld, &mtxRot);	// &ene->mtxWorld = &ene->mtxWorld * &mtxRot
			// 平行移動を反映	(T)
			D3DXMatrixTranslation(&mtxTranslate, boar->ene.Pos.x, boar->ene.Pos.y, boar->ene.Pos.z);
			D3DXMatrixMultiply(&boar->ene.mtxWorld, &boar->ene.mtxWorld, &mtxTranslate);

			// ワールドマトリックス設定
			pDevice->SetTransform(D3DTS_WORLD, &boar->ene.mtxWorld);

			// 現在のマテリアルを保存
			pDevice->GetMaterial(&matDef);

			// マテリアル情報へのポインタを
			pD3DXMat = (D3DXMATERIAL*)boar->ene.BuffMat->GetBufferPointer();


			for (j = 0; j < (int)boar->ene.NumMat; j++)
			{
				pDevice->SetMaterial(&pD3DXMat[j].MatD3D);	// マテリアルの設定

				pDevice->SetTexture(0, D3DTextureBoar);	// テクスチャの設定

				boar->ene.Mesh->DrawSubset(j);
			}

			// マテリアルに戻す
			pDevice->SetMaterial(&matDef);
		}
}

//=============================================================================
// イノシシの視野チェック
int BoarVisionCheck(int no)
{
	BOAR *boar = &boarWk[no];
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < PLAYER_MAX; i++, player++)
		if (player->use)
		{
			// 範囲をチェック
			D3DXVECTOR3 temp3 = player->Pos - boar->ene.Pos;
			float distance = D3DXVec3Length(&temp3);
			if (distance < AGGRO_RANGE)

				// 視野範囲をチェック
				if (inView(boar->ene.Pos, player->Pos, boar->ene.Rot, VISION_RAD))
				{

					// レイチェック
					D3DXVECTOR3 ObjPos = VEC3CLEAR;
					if (!VisionCheck(boar->ene.Pos, player->Pos, &ObjPos))
						return IN_VISION;
				}
				else if (distance < CHECK_RANGE)
					return IN_CHECK;
		}
	return IS_FALSE;
}

//=============================================================================
// イノシシの移動
void MoveBoar(int no)
{
	BOAR *boar = &boarWk[no];
	PLAYER *player = GetPlayer(0);
	D3DXVECTOR3 Vel = VEC3CLEAR;
	D3DXVECTOR3 oldPos = boar->ene.Pos;

	FENCE *fence = GetFence(0);

	switch (boar->Stat)
	{
	case ROAMING:
		Vel = Ang2Vec(boar->ene.Rot, MOVE_VAL);
		boar->ene.Pos += Vel;
		break;
	case CHECKING:
		break;
	case CHASING:
		Vel = Ang2Vec(boar->ene.Rot, CHASE_VAL);
		boar->ene.Pos += Vel;
		SetEffect(boar->ene.Pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(0.65f, 0.05f, 0.85f, 0.5f), 30.0f, 30.0f, 30, Idx_DUST02);
		break;
	}

	//===========================================================
	// 策の当たり判定
	D3DXVECTOR3 tDistance;
	D3DXVECTOR3 tVel = boar->ene.Pos + Vel;
	D3DXVECTOR3 tPos = oldPos;
	D3DXVECTOR3 tsize = Ang2Vec(boar->ene.Rot, 2);

	// モデルの見た目にコリジョンを合わせてみる		fix collision to match player modle
	tVel.y += 10;
	tPos.y += 10;

	int i;

	D3DXVECTOR3 vec = VEC3CLEAR;
	float len = 0;
	for (i = 0, fence = GetFence(0); i < FENCE_MAX; i++, fence++)
		if (fence->mesh.use == true
			&& fence->isHitbox == true)
		{
			vec = boar->ene.Pos - fence->mesh.Pos;
			len = D3DXVec3Length(&vec);
			if (len < 400)
				if (CheckHitLinePrep(tPos, tVel, &tDistance, &fence->mesh))
				{
					// 当たっているので、当たった交点までの距離を足す
					boar->ene.Pos += tDistance - tsize;
					break;
				}
		}

	// 地面との当たり判定
	GroundCollision(&boar->ene.Pos, &boar->ene.Grav);

	// 壁との当たり判定
	D3DXVECTOR3 distance = { 0.0f,0.0f,0.0f };	// 壁までの距離
	D3DXVECTOR3 StartingPos = oldPos;
	D3DXVECTOR3 EndPos = boar->ene.Pos;
	D3DXVECTOR3 temp = Ang2Vec(boar->ene.Rot, 100);

	EndPos += temp;
	EndPos.y += 5;
	StartingPos.y += 5;
	if (CheckHitWallPrep(StartingPos, EndPos, &distance))
	{
		float tLen = D3DXVec3LengthSq(&distance);
		if (tLen < 500)
		{
			float ftemp = Vec2Cross(boar->ene.Pos, boar->RoamPos);

			if (ftemp < 0)
				boar->ene.Rot.y -= ROT_VAL;
			else
				boar->ene.Rot.y += ROT_VAL;
			if (tLen < 300)
			{
				if (ftemp < 0)
					boar->ene.Rot.y -= ROT_VAL;
				else
					boar->ene.Rot.y += ROT_VAL;
				if (tLen < 200)
				{
					if (ftemp < 0)
						boar->ene.Rot.y -= ROT_VAL;
					else
						boar->ene.Rot.y += ROT_VAL;
					if (tLen < 150)
					{
						if (ftemp < 0)
							boar->ene.Rot.y -= ROT_VAL;
						else
							boar->ene.Rot.y += ROT_VAL;
					}
				}
			}
		}
	}
}


//=============================================================================
// イノシシの行き先を決める処理
void SetRoamingPos(int no)
{
	BOAR *boar = &boarWk[no];
	ROCK *rock = GetRock(0);
	CRATE *crate = GetCrate(0);
	int i;
	bool tRock = true;
	bool tCrate = true;

	int x = boar->RoamDistance.x;
	int z = boar->RoamDistance.y;
	while (tRock || tCrate)
	{
		tRock = true;
		tCrate = true;

		boar->RoamPos.x = boar->StaringPos.x - (rand() % x);
		boar->RoamPos.z = boar->StaringPos.z + (rand() % z);

			for (i = 0, rock = GetRock(0); i < ROCK_MAX; i++, rock++)
		{
			if (!CheckHitBC(boar->StaringPos, rock->mesh.Pos, 10, 175))
				tRock = false;
		}

		for (i = 0, crate = GetCrate(0); i < CRATE_MAX; i++, crate++)
		{
			if (!CheckHitBC(boar->StaringPos, crate->mesh.Pos, 10, 175))
				tCrate = false;
		}
	}
}

//=============================================================================
// イノシシのゲット関数
BOAR *GetBoar(int no)
{
	return (&boarWk[no]);
}