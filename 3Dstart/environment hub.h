//=============================================================================
//
// 環境処理 [environment.h]
// Author : 川井一生
//
//=============================================================================
#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#include "main.h"





//=============================================================================
// マクロ定義

#define OBJECT_DATA_FILE	("data/FILE/object_data.csv")
#define END_OF_DATA			("##end")	// データの終わりを示す


enum // ファイルからデータ変換するものの名前
{	//	FtD = file to data
	FtD_USE = 0,

	FtD_TYPE,	// 種類

	FtD_POS_X,	// 位置
	FtD_POS_Y,
	FtD_POS_Z,

	FtD_ROT_X,	// 回転
	FtD_ROT_Y,
	FtD_ROT_Z,

	FtD_SCL_X,	// 大きさ
	FtD_SCL_Y,
	FtD_SCL_Z,
};

//=============================================================================
// 構造体宣言

// メッシュの構造体
typedef struct
{
	bool					use;					// 使用しているか

	// マップ上の位置など
	D3DXVECTOR3				Pos;					// 位置
	D3DXVECTOR3				Rot;					// 向き(回転)
	D3DXVECTOR3				Scl;					// 大きさ(スケール)

	// テクスチャー・モデル
	LPDIRECT3DVERTEXBUFFER9	VtxBuff = NULL;			// 頂点バッファへのポインタ
	D3DXVECTOR3				*VtxPos;				// 頂点の情報
	WORD					*IdxBuff;				// インデックスの数

	LPD3DXMESH				Mesh;					// メッシュ情報へのポインタ
	LPD3DXBUFFER			BuffMat;				// マテリアル情報へのポインタ
	DWORD					NumMat;					// マテリアル情報の数

	D3DXMATRIX				mtxWorld;				// ワールドマトリックス
	int						TexId;					// どのテクスチャーを使うか

	//// 影
	//int						nIdxShadow;				// 影ID
	//float					fSizeShadow;			// 影のサイズ
	//D3DXCOLOR				colShadow;				// 影の色

	// その他
	float					Grav;					// 
}MESH;


// ビルボードの構造体
typedef struct
{
	bool					use;					// 使用しているか

	// マップ上の位置など
	D3DXVECTOR3				Pos;					// 位置
	D3DXVECTOR3				Rot;					// 向き(回転)
	D3DXVECTOR3				Scl;					// 大きさ(スケール)
	D3DXMATRIX				mtxWorld;				// ワールドマトリックス

	// テクスチャー
	int						TexId;					// テクスチャー番号		texture id

	//	影
	int						nIdxShadow;				// 影ID
	float					fSizeShadow;			// 影のサイズ
	D3DXCOLOR				colShadow;				// 影の色
}BILL;


//=============================================================================
// プロトタイプ宣言
HRESULT InitEnvironment(void);
void UninitEnvironment(void);
void UpdateEnvironment(void);
void DrawEnvironment(void);
BILL *GetBill(void);
MESH *GetMesh(void);

#endif
