//=============================================================================
//
// ゲームモードの処理 [game.cpp]
// Author : 川井一生
/*
ゲームモードの状態の時処理する

ゲーム自体の処理の一括管理

when g_mode is set to GAME_MODE exicute

main hub for when the game is active
*/
//=============================================================================
#include "main.h"
#include "game.h"
//===============================================
// チャラ	character
#include "player.h"
#include "enemy.h"
#include "NPC.h"

//===============================================
// 環境		environment
#include "environment hub.h"
#include "map.h"
#include "meshwater.h"
#include "skybox.h"
#include "light.h"
#include "shadow.h"
#include "gate.h"

//===============================================
// アイテム
#include "gem.h"

//===============================================
// UI
#include "UI hub.h"

//===============================================
// その他	etc
#include "camera.h"
#include "checkhit.h"
#include "status icon.h"
#include "effect.h"
#include "collision.h"




//=============================================================================
// マクロ定義

// メッシュフィールドの初期設定
#define WATER_POS		(D3DXVECTOR3(0.0f, -100.0f, 0.0f))
#define WATER_ROT		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
#define WATER_B_CNT_X	(250)
#define WATER_B_CNT_Z	(250)
#define WATER_B_SCL_X	(75.0f)
#define WATER_B_SCL_Z	(75.0f)

//=============================================================================
// プロトタイプ宣言

//=============================================================================
// グローバル変数

//=============================================================================
// ゲームモードの初期化処理
HRESULT InitGame(int oldMode)
{
	switch (oldMode)
	{
	case MODE_NULL:
		// 初めての初期化（全部初期化）

		//=============================
		// マップの初期化
		InitMap();
		InitMeshWater(WATER_POS, WATER_ROT, WATER_B_CNT_X, WATER_B_CNT_Z, WATER_B_SCL_X, WATER_B_SCL_Z);
		InitSkyb();
		//=============================
		// オブジェクトの初期化
		InitEnvironment();
		//=============================
		// エネミーの初期化
		InitEnemy();
		//=============================
		// プレイヤーの初期化
		InitPlayer();
		//=============================
		// NPCの初期化処理
		InitNpc();
		//=============================
		//	UIの最新処理
		InitUiHub();
		//=============================
		// その他の初期化
		InitLight();
		InitShadow();
		InitCamera(MODE_GAME);
		InitStatIco();
		InitEffect();
		break;

	case MODE_TITLE:
		// タイトルになかった物だけ初期化

		//=============================
		// オブジェクトの初期化
		ResetGate();
		//=============================
		// エネミーの初期化
		InitEnemy();
		//=============================
		// プレイヤーの初期化
		InitPlayer();
		//=============================
		// NPCの初期化処理
		InitNpc();
		//=============================
		//	UIの最新処理
		InitUiHub();

		InitStatIco();

		//=============================
		// アイテムの位置を初期化
		ResetGem();

		break;
	case MODE_RESULT:
		break;

	}
	return S_OK;
}

//=============================================================================
// ゲームモードの初期化処理
void UninitGame(int mode)
{
	switch (mode)
	{
	case MODE_TITLE:
		// タイトルに使わないものをリリース

		//=============================
		// プレイヤーの終了処理
		UninitPlayer();
		//=============================
		// エネミーの終了処理
		UninitEnemy();
		//=============================
		// NPCの終了処理
		UninitNpc();
		//=============================
		// ステータスアイコンの終了処理
		UninitStatIco();
		//=============================
		// UIの終了処理
		UninitUiHub();
		break;

	case MODE_RESULT:
		//=============================
		// マップの終了処理
		UninitMap();
		UninitMeshWater();
		UninitSkyb();
		//=============================
		// オブジェクトの終了処理
		UninitEnvironment();
		//=============================
		// シャドウの終了処理
		UninitShadow();
		//=============================
		// プレイヤ－の終了処理
		UninitPlayer();
		//=============================
		// エネミーの終了処理
		UninitEnemy();
		//=============================
		// NPCの終了処理
		UninitNpc();
		//=============================
		// ステータスアイコンの終了処理
		UninitStatIco();
		//=============================
		// UIの終了処理
		UninitUiHub();

		break;






	}
}

//=============================================================================
// ゲームモードの最新処理
void UpdateGame(void)
{
	// 環境の最新処理
	UpdateEnvironment();

	// マップの最新処理
	UpdateMap();

	// 地面の更新処理
	UpdateMeshWater();

	// スカイボックス終了処理
	UpdateSkyb();

	// シャドウの最新処理
	UpdateShadow();

	// カメラの最新処理
	UpdateCamera();

	// モデルの最新処理
	UpdatePlayer();
#ifdef _DEBUG
	// コリジョンの最新処理
	UpdateCollision();
#endif
	// エネミーの最新処理
	UpdateEnemy();

	// NPCの最新処理
	UpdateNpc();

	// ブレットの最新処理
	UpdateStatIco();

	// 当たり判定
	UpdateCheckhit();

	// UIの最新処理
	UpdateUiHub();

	UpdateEffect();

}

//=============================================================================
// ゲームモードの初期化処理
void DrawGame(void)
{

	// マップの描画処理
	DrawMap();

	// 地面の描画処理
	DrawMeshWater();

	// スカイボックス終了処理
	DrawSkyb();

	// シャドウの描画処理
	DrawShadow();

	// エフェクトの描画処理
	DrawEffect();

	// 環境の描画処理
	DrawEnvironment();

	// モデルの描画
	DrawPlayer();

	// エネミーの描画
	DrawEnemy();

	// NPCの描画処理
	DrawNpc();

	// ステータスアイコンの描画
	DrawStatIco();

	// UIの描画
	DrawUiHub();

}
