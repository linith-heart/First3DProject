//=============================================================================
//
// 地面処理 [field.h]
// Author : 川井一生
//
//=============================================================================
#ifndef _FIELD_H_
#define _FIELD_H_

#include "main.h"

#define FIELD_SIZE_MAX	(100)						// フィールドの大きさ
#define FIELD_MAX		(1)						// フィールドの使用している数

typedef struct
{
	D3DXVECTOR3				Pos;					// 位置
	D3DXVECTOR3				Rot;					// 向き(回転)

	int						BlkCntX, BlkCntZ;		// ブロック数
	int						Vtx;					// 総頂点数
	int						VtxIdx;					// 総インデックス数
	int						PolyCnt;				// 総ポリゴン数
	float					BlkSizeX, BlkSizeZ;		// ブロックサイズ


	D3DXMATRIX				mtxWorld;				// ワールドマトリックス

}FIELD;



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nNumBlockX, int nNumBlockZ, float fBlockSizeX, float fBlockSizeZ);
void UninitField(void);
void UpdateField(void);
void DrawField(void);
void *GetField(int no);
#endif
