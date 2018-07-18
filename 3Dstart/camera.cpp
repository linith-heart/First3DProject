//=============================================================================
//
// カメラ処理 [field.cpp]
// Author : 川井一生
//
//=============================================================================
#include "main.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "mathWk.h"

//=============================================================================
// マクロ定義
#define	POS_X_CAM		(-400.0f)						// カメラの初期位置(X座標)
#define	POS_Y_CAM		(100.0f)						// カメラの初期位置(Y座標)
#define	POS_Z_CAM		(0.0f)							// カメラの初期位置(Z座標)


#define	TITLE_EYE_X		(4300.0f)						// カメラの初期位置(X座標)
#define	TITLE_EYE_Y		(880.0f)						// カメラの初期位置(Y座標)
#define	TITLE_EYE_Z		(740.0f)						// カメラの初期位置(Z座標)

#define	TITLE_AT_X		(3900.0f)						// カメラの初期位置(X座標)
#define	TITLE_AT_Y		(710.0f)						// カメラの初期位置(Y座標)
#define	TITLE_AT_Z		(775.0f)						// カメラの初期位置(Z座標)


#define TITLE_ROT		(0.5f * D3DX_PI)

#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							// ビュー平面の視野角
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比	
#define	VIEW_NEAR_Z		(10.0f)											// ビュー平面のNearZ値
#define	VIEW_FAR_Z		(8500.0f)										// ビュー平面のFarZ値

#define CAM_HIGHT		(80.0f)						// カメラの地面からの高さ

#define MOVE_VALUE		(1)							// 移動量
#define ROT_VALUE		(D3DX_PI * 0.01f)			// 回転量
#define FLIP_CAM		(D3DX_PI * 0.05f)			// フリップ時の回転量
#define FLIP_TIME		(20)						// 回転するフレーム数



enum camStat
{
	FOLLOW_CAM	= 0,
	FREE_CAM	= 1,
	TITLE_CAM,
	CAM_MAX,
};
#define GAME_CAM_MAX (2)

//=============================================================================
// プロトタイプ宣言
void MoveCamera(int no);
void MovetoPlayer(int no);
void SwitchCam(int no);
void ResetCam(int no);

//=============================================================================
// グローバル変数
CAMERA					cameraWk[CAMERA_MAX];		// カメラの構造体

D3DXMATRIX				mtxProjection;				// プロジェクションマトリックス

int						cam_mode = 0;				// カメラのモード

bool					isFlip = false;
int						FlipCnt = 0;

//=============================================================================
// カメラの初期化処理
void InitCamera(int mode)
{
	CAMERA *camera = &cameraWk[0];
	int i;
	for (i = 0; i < CAM_MAX; i++, camera++)
	{
		switch (i)
		{
		case FOLLOW_CAM:
			if (mode == MODE_GAME)
				cam_mode = FOLLOW_CAM;

			camera->Eye = D3DXVECTOR3(POS_X_CAM, POS_Y_CAM, POS_Z_CAM);
			camera->At = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			camera->Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			camera->MtP = true;
			break;

		case FREE_CAM:
			camera->Eye = D3DXVECTOR3(POS_X_CAM, POS_Y_CAM, POS_Z_CAM);
			camera->At = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			camera->Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			camera->MtP = false;
			break;
		case TITLE_CAM:
			if (mode == MODE_TITLE)
				cam_mode = TITLE_CAM;

			camera->Eye = D3DXVECTOR3(TITLE_EYE_X, TITLE_EYE_Y, TITLE_EYE_Z);
			camera->At = D3DXVECTOR3(TITLE_AT_X, TITLE_AT_Y, TITLE_AT_Z);
			camera->Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			camera->MtP = false;
			break;
		}
	}
}
//=============================================================================
// カメラの最新処理

void UpdateCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	CAMERA *camera = &cameraWk[cam_mode];
	PLAYER *player = GetPlayer(0);
	int i;
	int j;
	int check;

	SwitchCam(cam_mode);

	MoveCamera(cam_mode);

	if (player->alive == true)
	{
		switch (cam_mode)
		{
		case FOLLOW_CAM:
			MovetoPlayer(cam_mode);
			break;
		case FREE_CAM:
			break;
		}
	}
	else
	{
		switch (cam_mode)
		{
		case FOLLOW_CAM:
			camera->At = player->Pos;			// 注視点をプレイヤ－の座標にする
			break;
		case FREE_CAM:
			break;
		case TITLE_CAM:
			break;
		}
	}

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&camera->mtxView);
	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&camera->mtxView, &camera->Eye, &camera->At, &camera->Up);
	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &camera->mtxView);

	// プロジェクションマトリックス初期化
	D3DXMatrixIdentity(&mtxProjection);
	// プロジェクションマトリックス作成
	D3DXMatrixPerspectiveFovLH(&mtxProjection,
		VIEW_ANGLE,
		VIEW_ASPECT,
		VIEW_NEAR_Z,	//ビュー平面のNearZ値
		VIEW_FAR_Z);	//ビュー平面のFarZ値
	// プロジェクションマトリックス設定
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);


}

//=============================================================================
// カメラの移動処理
void MoveCamera(int no)
{
	CAMERA *camera = &cameraWk[no];
	PLAYER *player = GetPlayer(0);
	D3DXVECTOR3 Vel = { 0.0f,0.0f,0.0f }; //移動量
	switch (no)
	{
	case FREE_CAM:

		//================================================================
		//=======================================カメラの平行移動
		// 前進			move forward
		if (GetKeyboardPress(DIK_UP))
		{
			D3DXVECTOR3 temp = camera->Eye - camera->At;
			float tempY = temp.y;										// 高さを保存
			temp.y = 0.0f;												// 平面の計算にするために高さを消す
			float angle = atan2(temp.z, temp.x);
			Vel.x = cos(angle)*MOVE_VALUE;
			Vel.z = sin(angle)*MOVE_VALUE;

			camera->At.x -= Vel.x;
			camera->At.z -= Vel.z;
			camera->Eye.x -= Vel.x;
			camera->Eye.z -= Vel.z;

		}
		//===========================================
		// 後進			move backword
		if (GetKeyboardPress(DIK_DOWN))
		{
			D3DXVECTOR3 temp = camera->Eye - camera->At;
			float tempY = temp.y;										// 高さを保存
			temp.y = 0.0f;												// 平面の計算にするために高さを消す
			float angle = atan2(temp.z, temp.x);
			Vel.x = cos(angle)*MOVE_VALUE;
			Vel.z = sin(angle)*MOVE_VALUE;

			camera->At.x += Vel.x;
			camera->At.z += Vel.z;
			camera->Eye.x += Vel.x;
			camera->Eye.z += Vel.z;

		}
		//===========================================
		//	左に進む		move left
		if (GetKeyboardPress(DIK_LEFT))
		{
			D3DXVECTOR3 temp = camera->Eye - camera->At;
			float tempY = temp.y;										// 高さを保存
			temp.y = 0.0f;												// 平面の計算にするために高さを消す
			float angle = atan2(temp.z, temp.x);
			Vel.x = cos(angle + (D3DX_PI * 0.5f))*MOVE_VALUE;
			Vel.z = sin(angle + (D3DX_PI * 0.5f))*MOVE_VALUE;

			camera->At.x -= Vel.x;
			camera->At.z -= Vel.z;
			camera->Eye.x -= Vel.x;
			camera->Eye.z -= Vel.z;

		}
		//===========================================
		// 右に進む			move right
		if (GetKeyboardPress(DIK_RIGHT))
		{
			D3DXVECTOR3 temp = camera->Eye - camera->At;
			float tempY = temp.y;										// 高さを保存
			temp.y = 0.0f;												// 平面の計算にするために高さを消す
			float angle = atan2(temp.z, temp.x);
			Vel.x = cos(angle + (D3DX_PI * 0.5f))*MOVE_VALUE;
			Vel.z = sin(angle + (D3DX_PI * 0.5f))*MOVE_VALUE;

			camera->At.x += Vel.x;
			camera->At.z += Vel.z;
			camera->Eye.x += Vel.x;
			camera->Eye.z += Vel.z;

		}
		break;

		//====================================================================
	case FOLLOW_CAM:
		//==============================カメラの回転
		// 左回転				rotate left
		if (GetKeyboardPress(DIK_J)
			|| IsButtonPressed(0, BUTTON_R_RIGHT)
			|| IsButtonPressed(0, BUTTON_R2))

		{
			D3DXVECTOR3 temp3 = camera->Eye - camera->At;					// 現在のカメラのベクトル
			D3DXVECTOR3 v = temp3;											// 回転軸
			D3DXVECTOR4 p = D3DXVECTOR4(temp3.x, temp3.y, temp3.z, 0.0f);	// 回転したいバーテックス
			v.x = 0.0f;														// 現在のカメラベクトルに対して90度回転
			v.z = 0.0f;
			v.y = 1.0f;
			D3DXVECTOR4 q = D3DXVECTOR4(-v.x*sin(ROT_VALUE / 2), -v.y*sin(ROT_VALUE / 2), -v.z*sin(ROT_VALUE / 2), cos(ROT_VALUE / 2));	// 回転計算用のクオータニオン
			D3DXVECTOR4 r = D3DXVECTOR4(v.x*sin(ROT_VALUE / 2), v.y*sin(ROT_VALUE / 2), v.z*sin(ROT_VALUE / 2), cos(ROT_VALUE / 2));		// 回転計算用のクオータニオン
			D3DXVECTOR4 temp4 = QuaternionMulty(QuaternionMulty(r, p), q);	// クオータニオンの掛け算で移動量計算

			temp3 = camera->Eye;

			camera->Eye.x = temp4.x + camera->At.x;
			camera->Eye.y = temp4.y + camera->At.y;
			camera->Eye.z = temp4.z + camera->At.z;

			if (((camera->Eye.x - camera->At.x)*(camera->Eye.x - camera->At.x))
				+ ((camera->Eye.z - camera->At.z)*(camera->Eye.z - camera->At.z)) < 2500)
			{
				camera->Eye = temp3;
			}
		}
		//===========================================
		// 右回転				rotate right
		if (GetKeyboardPress(DIK_L)
			|| IsButtonPressed(0, BUTTON_R_LEFT)
			|| IsButtonPressed(0, BUTTON_L2))
		{
			D3DXVECTOR3 temp3 = camera->Eye - camera->At;					// 現在のカメラのベクトル
			D3DXVECTOR3 v = temp3;											// 回転軸
			D3DXVECTOR4 p = D3DXVECTOR4(temp3.x, temp3.y, temp3.z, 0.0f);	// 回転したいバーテックス
			v.x = 0.0f;														// 現在のカメラベクトルに対して90度回転
			v.z = 0.0f;
			v.y = 1.0f;
			D3DXVECTOR4 q = D3DXVECTOR4(v.x*sin(ROT_VALUE / 2), v.y*sin(ROT_VALUE / 2), v.z*sin(ROT_VALUE / 2), cos(ROT_VALUE / 2));		// 回転計算用のクオータニオン
			D3DXVECTOR4 r = D3DXVECTOR4(-v.x*sin(ROT_VALUE / 2), -v.y*sin(ROT_VALUE / 2), -v.z*sin(ROT_VALUE / 2), cos(ROT_VALUE / 2));	// 回転計算用のクオータニオン
			D3DXVECTOR4 temp4 = QuaternionMulty(QuaternionMulty(r, p), q);	// クオータニオンの掛け算で移動量計算

			temp3 = camera->Eye;

			camera->Eye.x = temp4.x + camera->At.x;
			camera->Eye.y = temp4.y + camera->At.y;
			camera->Eye.z = temp4.z + camera->At.z;

			if (((camera->Eye.x - camera->At.x)*(camera->Eye.x - camera->At.x))
				+ ((camera->Eye.z - camera->At.z)*(camera->Eye.z - camera->At.z)) < 2500)
			{
				camera->Eye = temp3;
			}

		}
		//===========================================
		// 上回転				rotate up
		if (GetKeyboardPress(DIK_I)
			|| IsButtonPressed(0, BUTTON_R_DOWN))
		{
			D3DXVECTOR3 temp3 = camera->Eye - camera->At;					// 現在のカメラのベクトル
			D3DXVECTOR3 v = temp3;											// 回転軸
			D3DXVECTOR4 p = D3DXVECTOR4(temp3.x, temp3.y, temp3.z, 0.0f);	// 回転したいバーテックス
			v.y = 0.0f;														// 現在のカメラベクトルに対して90度回転
			v.x = -temp3.z;
			v.z = temp3.x;
			Vec3Normalize(&v);												// 正規化
			D3DXVECTOR4 q = D3DXVECTOR4(-v.x*sin(ROT_VALUE / 2), -v.y*sin(ROT_VALUE / 2), -v.z*sin(ROT_VALUE / 2), cos(ROT_VALUE / 2));	// 回転計算用のクオータニオン
			D3DXVECTOR4 r = D3DXVECTOR4(v.x*sin(ROT_VALUE / 2), v.y*sin(ROT_VALUE / 2), v.z*sin(ROT_VALUE / 2), cos(ROT_VALUE / 2));		// 回転計算用のクオータニオン
			D3DXVECTOR4 temp4 = QuaternionMulty(QuaternionMulty(r, p), q);	// クオータニオンの掛け算で移動量計算

			temp3 = camera->Eye;

			camera->Eye.x = temp4.x + camera->At.x;
			camera->Eye.y = temp4.y + camera->At.y;
			camera->Eye.z = temp4.z + camera->At.z;

			if (((camera->Eye.x - camera->At.x)*(camera->Eye.x - camera->At.x))
				+ ((camera->Eye.z - camera->At.z)*(camera->Eye.z - camera->At.z)) < 2500)
			{
				camera->Eye = temp3;
			}

		}
		//===========================================
		// 下回転				rotate down
		if (GetKeyboardPress(DIK_K)
			|| IsButtonPressed(0, BUTTON_R_UP))
		{
			D3DXVECTOR3 temp3 = camera->Eye - camera->At;					// 現在のカメラのベクトル
			D3DXVECTOR3 v = temp3;											// 回転軸
			D3DXVECTOR4 p = D3DXVECTOR4(temp3.x, temp3.y, temp3.z, 0.0f);	// 回転したいバーテックス
			v.y = 0.0f;														// 現在のカメラベクトルに対して90度回転
			v.x = -temp3.z;
			v.z = temp3.x;
			Vec3Normalize(&v);												// 正規化
			D3DXVECTOR4 q = D3DXVECTOR4(v.x*sin(ROT_VALUE / 2), v.y*sin(ROT_VALUE / 2), v.z*sin(ROT_VALUE / 2), cos(ROT_VALUE / 2));		// 回転計算用のクオータニオン
			D3DXVECTOR4 r = D3DXVECTOR4(-v.x*sin(ROT_VALUE / 2), -v.y*sin(ROT_VALUE / 2), -v.z*sin(ROT_VALUE / 2), cos(ROT_VALUE / 2));	// 回転計算用のクオータニオン
			D3DXVECTOR4 temp4 = QuaternionMulty(QuaternionMulty(r, p), q);	// クオータニオンの掛け算で移動量計算

			temp3 = camera->Eye;

			camera->Eye.x = temp4.x + camera->At.x;
			camera->Eye.y = temp4.y + camera->At.y;
			camera->Eye.z = temp4.z + camera->At.z;

			if (camera->Eye.y < 10)
			{
				camera->Eye = temp3;
			}
		}

		//===========================================
		// カメラ反転	flip camera
		if (GetKeyboardTrigger(DIK_RSHIFT) && !isFlip
			|| IsButtonTriggered(0, BUTTON_RS) && !isFlip)
		{
			isFlip = true;
			FlipCnt = FLIP_TIME;
		}
		if (isFlip)
		{
			D3DXVECTOR3 temp3 = camera->Eye - camera->At;					// 現在のカメラのベクトル
			D3DXVECTOR3 v = temp3;											// 回転軸
			D3DXVECTOR4 p = D3DXVECTOR4(temp3.x, temp3.y, temp3.z, 0.0f);	// 回転したいバーテックス
			v.x = 0.0f;														// 現在のカメラベクトルに対して90度回転
			v.z = 0.0f;
			v.y = 1.0f;
			D3DXVECTOR4 q = D3DXVECTOR4(v.x*sin(FLIP_CAM / 2), v.y*sin(FLIP_CAM / 2), v.z*sin(FLIP_CAM / 2), cos(FLIP_CAM / 2));		// 回転計算用のクオータニオン
			D3DXVECTOR4 r = D3DXVECTOR4(-v.x*sin(FLIP_CAM / 2), -v.y*sin(FLIP_CAM / 2), -v.z*sin(FLIP_CAM / 2), cos(FLIP_CAM / 2));	// 回転計算用のクオータニオン
			D3DXVECTOR4 temp4 = QuaternionMulty(QuaternionMulty(r, p), q);	// クオータニオンの掛け算で移動量計算

			temp3 = camera->Eye;

			camera->Eye.x = temp4.x + camera->At.x;
			camera->Eye.y = temp4.y + camera->At.y;
			camera->Eye.z = temp4.z + camera->At.z;

			if (((camera->Eye.x - camera->At.x)*(camera->Eye.x - camera->At.x))
				+ ((camera->Eye.z - camera->At.z)*(camera->Eye.z - camera->At.z)) < 2500)
			{
				camera->Eye = temp3;
			}
			FlipCnt--;
			if (FlipCnt <= 0)
				isFlip = false;
		}
		//====================================================================
		//===========================================ズームイン・ズームアウト
		// ズームイン
		if (GetKeyboardPress(DIK_U))
		{
			D3DXVECTOR3 temp = camera->Eye - camera->At;
			float	CamLenSq = D3DXVec3LengthSq(&temp);
			if (CamLenSq >= 23000)
			{
				D3DXVECTOR3 temp = camera->Eye - camera->At;
				temp /= 180;
				camera->Eye -= temp;
			}
		}
		//===========================================
		// ズームアウト
		if (GetKeyboardPress(DIK_O))
		{
			D3DXVECTOR3 temp = camera->Eye - camera->At;
			float	CamLenSq = D3DXVec3LengthSq(&temp);
			if (CamLenSq <= 230000)
			{
				D3DXVECTOR3 temp = camera->Eye - camera->At;
				temp /= 180;
				camera->Eye += temp;
			}
		}
		break;
		case TITLE_CAM:
			break;
	}
}

//=============================================================================
// カメラの注視点をプレイヤ－にセット
void MovetoPlayer(int no)
{
	CAMERA *camera = &cameraWk[no];
	PLAYER *player = GetPlayer(0);

	D3DXVECTOR3 temp = camera->Eye - camera->At;	// 注視点から視点までのベクトルを保存

	camera->At = player->Pos;			// 注視点をプレイヤ－の座標にする
	camera->At.y += CAM_HIGHT;			// 注視点をプレイヤ－の座標にする
	camera->Eye = camera->At + temp;	// 注視点に保存したベクトルを足す
}

//=============================================================================
// カメラの切り替え
void SwitchCam(int no)
{
	CAMERA *camera = &cameraWk[no];
	MODE mode = GetMode();

	switch (mode)
	{
	case MODE_TITLE:
		int i;
		for (i = 0, camera = &cameraWk[0]; i < CAM_MAX; i++, camera++)
		{
			if (i == TITLE_CAM)
			{
				cam_mode = i;
			}
		}
		break;
	case MODE_GAME:
		if (cam_mode == TITLE_CAM)
		{
			cam_mode = FOLLOW_CAM;
			ResetCam(FOLLOW_CAM);
		}


		if (GetKeyboardTrigger(DIK_M)
			|| IsButtonPressed(0, BUTTON_START))
		{
			int i, temp;
			temp = 0;

			temp = (no + 1) % GAME_CAM_MAX;
			cam_mode = temp;
			switch (temp)
			{
			case FREE_CAM:
				D3DXVECTOR3 tEye = camera->Eye;
				D3DXVECTOR3 tAt = camera->At;

				for (i = 0, camera = &cameraWk[0]; i < CAM_MAX; i++, camera++)
					if (i == temp)
					{
						camera->Eye = tEye;
						camera->At = tAt;
					}
				break;
			}
		}
		break;
	}

}

//=============================================================================
// カメラをリセット
void ResetCam(int no)
{
	CAMERA *camera = &cameraWk[no];

	switch (no)
	{
	case FOLLOW_CAM:
		camera->Eye = D3DXVECTOR3(POS_X_CAM, POS_Y_CAM, POS_Z_CAM);
		camera->At = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		break;
	case FREE_CAM:
		break;
	case TITLE_CAM:
		break;
	}

}
//=============================================================================
// カメラのゲット
int GetCamMode(void)
{
	return cam_mode;
}
//=============================================================================
// カメラのゲット
CAMERA *GetCamera(int no)
{
	return (&cameraWk[no]);
}