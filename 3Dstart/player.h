//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : 川井一生
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//=============================================================================
// マクロ定義

#define POS_X_PLAYER	(-1900.0f)	// (-1900.0f)	スタート
#define POS_Y_PLAYER	(700.0f)	// (700.0f)　	の
#define POS_Z_PLAYER	(-1800.0f)	// (-1800.0f)	位置

#define PLAYER_HP_MAX (5)			// プレイヤーの体力
#define PLAYER_STAM_MAX (300)		// プレイヤーのスタミナ

#define PLAYER_MAX	(1)				// プレイヤ－の使用している数



//=============================================================================
// 構造体宣言

typedef struct
{
	bool					use;						// 使用しているか
	bool					alive;						// 生きているか

	// マップ上の移動など
	D3DXVECTOR3				Pos;					// 位置					position
	D3DXVECTOR3				Rot;					// 向き(回転)			rotation
	D3DXVECTOR3				Scl;					// 大きさ(スケール)		scale

	D3DXVECTOR3				Vel;					// 移動量
	float					Grav;					// 重力

	// テクスチャー・モデル
	LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffPlayer = NULL;	// 頂点バッファへのポインタ
	LPD3DXMESH				Mesh;			// メッシュ情報へのポインタ
	LPD3DXBUFFER			D3DXBuffMatPlayer;		// マテリアル情報へのポインタ
	DWORD					NumMatPlayer;			// マテリアル情報の数
	D3DXMATRIX				mtxWorld;				// ワールドマトリックス

	// 影
	int						nIdxShadow;				// 影ID
	float					fSizeShadow;			// 影のサイズ
	D3DXCOLOR				colShadow;				// 影の色


	//その他
	int						team;					// 敵か味方を判断する
	int						BullCD;					// 弾発射時までの待ち
	int						HP;						// 体力
	float					HPShadow;				// 体力
	float					Stam;					// スタミナ
	int						StamTimer;				// スタミナが無くなってから使えるようになるまでの時間

	bool					Jump;					// ジャンプ
	int						HitTime;				// 当たった後の無敵時間
}PLAYER;



//=============================================================================
// プロトタイプ宣言
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void PlayerHit(int no);
void DrawPlayer(void);
PLAYER *GetPlayer(int no);
#endif
