//=============================================================================
//
// メイン処理 [main.h]
// Author : 川井一生
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//=============================================================================
// インクルードファイル
#include <windows.h>
#include "d3dx9.h"
#include <stdio.h>
#include <time.h>


#define DIRECTINPUT_VERSION (0x0800)	// 警告対処用
#include "dinput.h"

//=============================================================================
// ライブラリのリンク
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")

//=============================================================================
// マクロ定義

#define SCREEN_WIDTH	(1280)	// ウインドウの幅
#define SCREEN_HEIGHT	(720)	// ウインドウの高さ

#define	NUM_VERTEX		(4)		// 頂点数
#define	NUM_POLYGON		(2)		// ポリゴン数

// ２Ｄポリゴン頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// ３Ｄポリゴン頂点フォーマット( 頂点座標[３D] / 法線ベクトル / 反射光 / テクスチャ座標 )
#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)


// 上記２Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 vtx;	// 頂点座標
	float		rhw;	// テクスチャのパースペクティブコレクト用
	D3DCOLOR diffuse;	// 反射光
	D3DXVECTOR2 tex;	// テクスチャ座標
} VERTEX_2D;

//3Dポリゴン頂点フォーマットに合わせた構造体
typedef struct
{
	D3DXVECTOR3	vtx;		// 頂点座標
	D3DXVECTOR3	normal;		// 光線ベクトル
	D3DCOLOR	diffuse;	// 反射光
	D3DXVECTOR2	tex;		// テクスチャー座標
}VERTEX_3D;

//モードの切り替え
typedef enum
{
	MODE_TITLE = 0,			// タイトル画面
	MODE_GAME,				// ゲーム画面
	MODE_RESULT,			// リザルト画面
	MODE_MAX,
	MODE_NULL,
	MODE_UNINIT
} MODE;

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) {if (p!=NULL){(p)->Release();(p)=NULL;}}
#endif // !SAFE_RELEASE

#define VEC3CLEAR (D3DXVECTOR3(0.0f, 0.0f, 0.0f))

//=============================================================================
// プロトタイプ宣言
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode, int oldMode);
MODE GetMode(void);
void Uninit(void);
#endif