//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : 川井一生
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
// マクロ定義
#define TEXTURE			("data/TEXTURE/pig_map.jpg")
#define MODEL			("data/MODEL/pig000.x")

#define	MOVE_VAL		(3.0f)				// 移動量
#define JUMP_VAL		(6)					// ジャンプの量
#define	ROT_VAL			(D3DX_PI * 0.02f)	// 回転量

#define STAM_FULL_RESET_TIMER	(120)		// 完全消費時スタミナのリセット時間
#define STAM_RESET_TIMER	(-30)			// スタミナのリセット時間
#define STAM_DRAIN_RATE		(1.75)			// スタミナの消費量
#define STAM_CHARGE_RATE	(2)				// スタミナの回復量

#define IMMUNE_TIME			(60)			// 無敵時間

#define GAME_OVER_TIMER	(4 * 60)

//=============================================================================
// プロトタイプ宣言
void ActionInput(void);
void MoveInput(void);


//=============================================================================
// グローバル変数
LPDIRECT3DTEXTURE9		D3DTexturePlayer = NULL;	// テクスチャへのポインタ
PLAYER					playerWk[PLAYER_MAX];		// プレイヤーへのポインタ

int						GOTimer = GAME_OVER_TIMER;				// プレイヤーが死んでから画面が変わるまでのタイマー

int						fuckme = 0;
//=============================================================================
// 初期化処理
HRESULT InitPlayer(void)
{
	PLAYER *player = &playerWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの設定
	D3DXCreateTextureFromFile(pDevice, TEXTURE, &D3DTexturePlayer);

	int i;
	for (i = 0; i < PLAYER_MAX; i++, player++)
	{
		player->use = true;
		player->alive = true;

		// 位置・回転・スケールの初期設定
		player->Pos = D3DXVECTOR3(POS_X_PLAYER, POS_Y_PLAYER, POS_Z_PLAYER);
		player->Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		player->Scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

		player->HP = PLAYER_HP_MAX;
		player->Stam = PLAYER_STAM_MAX;
		player->Jump = false;
		player->Grav = 0.0f;
		player->HitTime = 0;

		player->Vel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);



		// モデル関係の初期化
		player->Mesh = NULL;
		player->D3DXBuffMatPlayer = NULL;
		player->NumMatPlayer = 0;

		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(MODEL,			// モデルデータ
			D3DXMESH_SYSTEMMEM,				// 使用するメモリのオプション
			pDevice,						// デバイス 
			NULL,							// 未使用
			&player->D3DXBuffMatPlayer,		// マテリアルデータへのポインター
			NULL,							// 未使用
			&player->NumMatPlayer,			// D3DXMATERIAL構造体の数
			&player->Mesh)))		// メッシュデータへのポインター
		{
			return E_FAIL;
		}


		// 影の生成
		player->nIdxShadow = CreateShadow(player->Pos, 25.0f, 25.0f);

		player->fSizeShadow = 25.0f;
		player->colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
	}

	GOTimer = GAME_OVER_TIMER;

	return S_OK;
}

//=============================================================================
// 終了処理
void UninitPlayer(void)
{
	PLAYER *player = &playerWk[0];

	// mesh matirial player

	if(D3DTexturePlayer != NULL)
	{// テクスチャの開放
		D3DTexturePlayer->Release();
		D3DTexturePlayer = NULL;
	}
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (player->D3DVtxBuffPlayer != NULL)
		{// 頂点バッファの開放
			player->D3DVtxBuffPlayer->Release();
			player->D3DVtxBuffPlayer = NULL;
		}
		if (player->Mesh != NULL)
		{// メシュの解放
			player->Mesh->Release();
			player->Mesh = NULL;
		}

	}
}

//=============================================================================
// 更新処理
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


	// 影の位置設定
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
// 描画処理
void DrawPlayer(void)
{
	PLAYER *player = &playerWk[0];
	CAMERA *camera = GetCamera(0);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// スケール、回転、平行移動
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	int i,j;

	for (i = 0; i < PLAYER_MAX; i++, player++)
	{
		// ワールドマトリクス初期化
		D3DXMatrixIdentity(&player->mtxWorld);

		// スケールを反映	(S)
		D3DXMatrixScaling(&mtxScl, player->Scl.x, player->Scl.y, player->Scl.z);
		D3DXMatrixMultiply(&player->mtxWorld, &player->mtxWorld, &mtxScl); 	// &player->mtxWorld = &player->mtxWorld * &mtxScl
		// 回転を反映		(R)
		D3DXMatrixRotationYawPitchRoll(&mtxRot, player->Rot.y, player->Rot.x, player->Rot.z);
		D3DXMatrixMultiply(&player->mtxWorld, &player->mtxWorld, &mtxRot);	// &player->mtxWorld = &player->mtxWorld * &mtxRot
		// 平行移動を反映	(T)
		D3DXMatrixTranslation(&mtxTranslate, player->Pos.x, player->Pos.y, player->Pos.z);
		D3DXMatrixMultiply(&player->mtxWorld, &player->mtxWorld, &mtxTranslate);

		// ワールドマトリックス設定
		pDevice->SetTransform(D3DTS_WORLD, &player->mtxWorld);

		// 現在のマテリアルを保存
		pDevice->GetMaterial(&matDef);

		// マテリアル情報へのポインタを
		pD3DXMat = (D3DXMATERIAL*)player->D3DXBuffMatPlayer->GetBufferPointer();


		for (j = 0; j < (int)player->NumMatPlayer; j++)
		{
			pDevice->SetMaterial(&pD3DXMat[j].MatD3D);	// マテリアルの設定

			pDevice->SetTexture(0, D3DTexturePlayer);	// テクスチャの設定

			player->Mesh->DrawSubset(j);
		}

		// マテリアルに戻す
		pDevice->SetMaterial(&matDef);
	}
}

//=============================================================================
// アクション系のキー入力
void ActionInput(void)
{
	/* 使用されているキー used keys
	弾発射
	SPACE
	
	*/
	PLAYER *player = &playerWk[0];
	CAMERA *camera = GetCamera(0);

	//==============================================
	// 弾丸発射
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
// 移動系の処理
// 使用キー
// 移動 : W,A,S,D
// 
// 
// 
// 
void MoveInput(void)
{
	/* 使用されているキー used keys
	移動 move
	W
	ASD
	*/
	PLAYER *player = &playerWk[0];
	CAMERA *camera = GetCamera(0);

	// 移動するときの障害物の当たり判定に使うもの
	MAP *map = GetMap(0);
	CRATE *crate = GetCrate(0);
	ROCK *rock = GetRock(0);
	TREE *tree = GetTree(0);
	GATE *gate = GetGate(0);

	int i;

	player->Vel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXVECTOR3 oldPos = player->Pos;



	//===========================================
	// 前後					forward and back
	if (GetKeyboardPress(DIK_W)
		|| IsButtonPressed(0, BUTTON_UP))
	{
		if (player->Vel.z < MOVE_VAL)
		{
			player->Vel.z += MOVE_VAL;
			if (GetKeyboardPress(DIK_S)
				|| IsButtonPressed(0, BUTTON_DOWN)) // 同時押しの時、無効にする
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
	{								// 入力が無い場合 0.0f に近くける
		if (player->Vel.z > 0)
			player->Vel.z -= MOVE_VAL;
		else if (player->Vel.z < 0)
			player->Vel.z += MOVE_VAL;
	}

	//===========================================
	// 左右					right and left
	if (GetKeyboardPress(DIK_D)
		|| IsButtonPressed(0, BUTTON_RIGHT))
	{
		if (player->Vel.x < MOVE_VAL)
		{
			player->Vel.x += MOVE_VAL;
			if (GetKeyboardPress(DIK_A)
				|| IsButtonPressed(0, BUTTON_LEFT)) // 同時押しの時、無効にする
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
	{								// 入力が無い場合 0.0f に近くける
		if (player->Vel.x > 0)
			player->Vel.x -= MOVE_VAL;
		else if (player->Vel.x < 0)
			player->Vel.x += MOVE_VAL;
	}

	//===========================================
	// 移動量の計算処理
	if (player->Vel.x != 0.0f
		|| player->Vel.z != 0.0f)
	{
		// カメラの角度
		D3DXVECTOR3 temp = camera->Eye - camera->At;
		temp.y = 0.0f;
		float tAng = atan2f(temp.z, temp.x);

		// 移動量を規準化
		float Angle = atan2f(player->Vel.z, player->Vel.x);
		player->Vel.x = cosf(Angle + tAng + (D3DX_PI * 0.5)) * MOVE_VAL;
		player->Vel.z = sinf(Angle + tAng + (D3DX_PI * 0.5)) * MOVE_VAL;

		player->Rot.y = -Angle - tAng + (D3DX_PI * 1);


	}


	// 移動
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
		
		// エフェクトをセット
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
// プレイヤーの当たり処理
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
// プレイヤーのゲット関数
PLAYER *GetPlayer(int no)
{
	return (&playerWk[no]);
}