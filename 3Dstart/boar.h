//=============================================================================
//
// イノシシの処理 [boar.h]
// Author : 川井一生
//
//=============================================================================
#ifndef _BOAR_H_
#define _BOAR_H_

#include "main.h"
#include "enemy.h"

//=============================================================================
// マクロ定義

#define BOAR_MAX		(6)

// エネミーのデータのファイルパス
#define ENEMY_DATA_FILE	("data/FILE/enemy_data.csv")

enum
{
	CHASING,
	ROAMING,
	IDLING,
	CHECKING,
};

//=============================================================================
// 構造体宣言

typedef struct
{
	ENE			ene;			// エネミーの基本構造体
	D3DXVECTOR3 StaringPos;		// ゲーム開始時の位置
	D3DXVECTOR3 RoamPos;		// 歩く目的の位置
	D3DXVECTOR2 RoamDistance;	// 歩く範囲

	int			Stat;			// 今何をしているか
	int			IdleTime;		// アイドル時間
}BOAR;



//=============================================================================
// プロトタイプ宣言
HRESULT InitBoar(void);
void UninitBoar(void);
void UpdateBoar(void);
void DrawBoar(void);
BOAR *GetBoar(int no);
#endif
