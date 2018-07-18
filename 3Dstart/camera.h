//=============================================================================
//
// カメラ処理 [camera.h]
// Author : 
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"
//=============================================================================
// マクロ定義


#define CAMERA_MAX		(3)							// カメラの数

//=============================================================================
// 構造体宣言
typedef struct
{
	//bool					use;				// カメラを使用しているか	if camera is active or not
	D3DXVECTOR3				Eye;				// カメラの視点				position of camera
	D3DXVECTOR3				At;					// カメラの注視点			position of cameras view point
	D3DXVECTOR3				Up;					// カメラの上方向ベクトル	direction of up


	bool					MtP;				// プレイヤーを注視点にする
	D3DXMATRIX				mtxView;			// ビューマトリックス		view matrix
}CAMERA;





//=============================================================================
// プロトタイプ宣言
void InitCamera(int mode);
void UpdateCamera(void);

CAMERA *GetCamera(int no);
int GetCamMode(void);
#endif
