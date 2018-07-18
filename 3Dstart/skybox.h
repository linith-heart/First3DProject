//=============================================================================
//
// スカイボックス処理 [skybox.h]
// Author : 川井一生
//
//=============================================================================
#ifndef _SKYB_H_
#define _SKYB_H_

#include "main.h"
#include "enemy.h"

//=============================================================================
// マクロ定義

#define SKYB_MAX		(1)


//=============================================================================
// 構造体宣言

typedef struct
{
	bool					use;				// 使ってるか

	LPDIRECT3DVERTEXBUFFER9 VtxBuff = NULL;		// 頂点バッファへのポインタ

	D3DXVECTOR3				Pos;				// 位置
	D3DXVECTOR3				Rot;				// 向き(回転)
	D3DXVECTOR3				Scl;				// 大きさ(スケール)

	LPD3DXMESH				Mesh;				// メッシュ情報へのポインタ
	LPD3DXBUFFER			BuffMat;			// マテリアル情報へのポインタ
	DWORD					NumMat;				// マテリアル情報の数

	int						TextID;				// テクスチャー番号

	D3DXMATRIX				mtxWorld;				// ワールドマトリックス

}SKYB;



//=============================================================================
// プロトタイプ宣言
HRESULT InitSkyb(void);
void UninitSkyb(void);
void UpdateSkyb(void);
void DrawSkyb(void);
SKYB *GetSkyb(int no);
#endif
