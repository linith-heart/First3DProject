#ifdef _DEBUG
//=============================================================================
//
// デバッグデータ表示処理 [DBD.cpp]
// Author : 川井一生
/*
デバッグのデータを管理

hub for debug data
*/
//=============================================================================
#include "main.h"



//=========================================================タイトル



//=========================================================ゲーム
//=================プレイヤー
#include "player.h"

//=================エネミー
#include "boar.h"

//=================NPC
#include "pig.h"

//=================エフェクト
#include "effect.h"
#include "shadow.h"
#include "bullet.h"

//=================フィールド
#include "meshwater.h"
#include "skybox.h"
#include "light.h"
#include "camera.h"

//=================その他
#include "checkhit.h"
#include "input.h"
#include "DBD.h"

//=========================================================リザルト



//=============================================================================
// マクロ定義

//=============================================================================
// プロトタイプ宣言
int Line(int no);

void CheckifGamePadPressed(void);
void CheckBoarStat(void);
//=============================================================================
// グローバル変数

LPD3DXFONT		g_pD3DXFont = NULL;		// フォントへのポインタ

int				But_A = 0;
int				But_B = 0;
int				But_X = 0;
int				But_Y = 0;
int				But_L1 = 0;
int				But_R1 = 0;
int				But_L2 = 0;
int				But_R2 = 0;
int				But_LS = 0;
int				But_RS = 0;
int				But_SELECT = 0;
int				But_START = 0;

int				Joy_RV = 0;
int				Joy_RH = 0;
int				Joy_LV = 0;
int				Joy_LH = 0;

int				Idle = 0;
int				Roam = 0;
int				Chase = 0;
int				Check = 0;

//=============================================================================
void InitDBD(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 情報表示用フォントを設定
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pD3DXFont);

}

void DrawDBD(int no)
{
	RECT	rect	= { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	PLAYER	*player = GetPlayer(0);
	BOAR	*boar	= GetBoar(0);
	PIG		*pig	= GetPig(0);
	CAMERA	*camera = GetCamera(0);

	char	str[256];

	D3DXVECTOR3 temp = camera->Eye - camera->At;
	float	CamLenSq = D3DXVec3LengthSq(&temp);

	// カメラの角度
	temp.y = 0.0f;
	float tAng = atan2(temp.z, temp.x);


	int isUp = 0;
	if (pig->isUpAnim == true)
		isUp = 1;
	else
		isUp = 0;


	CheckifGamePadPressed();

	CheckBoarStat();

	temp = boar->ene.Pos - boar->RoamPos;
	float Dist2Point = D3DXVec3Length(&temp);

	temp = boar->StaringPos - boar->ene.Pos;
	float LeashLen = D3DXVec3Length(&temp);

	float tempX = rand() % 1200;
	float tempZ = rand() % 1000;


	// FPS
	sprintf(str, "FPS:%d\n", no);
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	// カメラの位置　camera Eye	
	rect = { 0,Line(1),SCREEN_WIDTH,SCREEN_HEIGHT };		// bg x and y
	sprintf(str, "cam x:%f z:%f y:%f\n", camera->Eye.x, camera->Eye.z, camera->Eye.y);
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	// カメラのEyeからAtの長さ　distance between camEye and camAt
	rect = { 0,Line(2),SCREEN_WIDTH,SCREEN_HEIGHT };		// bg x and y
	sprintf(str, "cam len sq%f\n", CamLenSq);
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	// プレイヤーの移動量　player velocity
	rect = { 0,Line(3),SCREEN_WIDTH,SCREEN_HEIGHT };		// bg x and y
	sprintf(str, "player:Pos x=%f,z=%f,y=%f\n", player->Pos.x, player->Pos.z, player->Pos.y);
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	// プレイヤーの角度　player angle
	rect = { 0,Line(4),SCREEN_WIDTH,SCREEN_HEIGHT };		// bg x and y
	sprintf(str, "player:Rot.x%f,Rot.y%f,Rot.z %f \n", player->Rot.x / D3DX_PI, player->Rot.y / D3DX_PI, player->Rot.z / D3DX_PI);
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	// gamepad test
	rect = { 0,Line(5),SCREEN_WIDTH,SCREEN_HEIGHT };		// bg x and y
	sprintf(str, "G_Pad A=%d, B=%d, X=%d, Y=%d, L1=%d, R1=%d, L2=%d, R2=%d, SL=%d, ST=%d, LS=%d, RS=%d, RV=%d, RH=%d, LV=%d, VH=%d, \n", But_A, But_B, But_X, But_Y, But_L1, But_R1, But_L2, But_R2, But_SELECT, But_START, But_LS, But_RS, Joy_RV,Joy_RH,Joy_LV,Joy_LH);
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	// boar stats
	rect = { 0,Line(6),SCREEN_WIDTH,SCREEN_HEIGHT };		// bg x and y
	sprintf(str, "boar:IDLE=%d, ROAM=%d, CHASE=%d, CHECK=%d \n", Idle, Roam, Chase, Check);
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	// boar pos
	rect = { 0,Line(7),SCREEN_WIDTH,SCREEN_HEIGHT };		// bg x and y
	sprintf(str, "BoarPos:x=%f, z=%f \n", boar->ene.Pos.x, boar->ene.Pos.z);
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	// boar roam pos
	rect = { 0,Line(8),SCREEN_WIDTH,SCREEN_HEIGHT };		// bg x and y
	sprintf(str, "RoamPos:x=%f, z=%f \n", boar->RoamPos.x, boar->RoamPos.z);
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	// boar roam pos
	rect = { 0,Line(9),SCREEN_WIDTH,SCREEN_HEIGHT };		// bg x and y
	sprintf(str, "Boar Dist2Point=%f, LeashLen=%f \n", Dist2Point, LeashLen);
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	// boar roam pos
	rect = { 0,Line(10),SCREEN_WIDTH,SCREEN_HEIGHT };		// bg x and y
	sprintf(str, "tempX=%f \n", tempX);
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	// boar roam pos
	rect = { 0,Line(11),SCREEN_WIDTH,SCREEN_HEIGHT };		// bg x and y
	sprintf(str, "tempZ=%f \n", tempZ);
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}

int Line(int no)
{
	return (no * 15);
}


void CheckifGamePadPressed(void)
{
	// reset buttons
	But_A = 0;
	But_B = 0;
	But_X = 0;
	But_Y = 0;
	But_L1 = 0;
	But_R1 = 0;
	But_L2 = 0;
	But_R2 = 0;
	But_LS = 0;
	But_RS = 0;
	But_SELECT = 0;
	But_START = 0;
	Joy_RV = 0;
	Joy_RH = 0;
	Joy_LV = 0;
	Joy_LH = 0;

	if (IsButtonPressed(0, BUTTON_A))
		But_A++;

	if (IsButtonPressed(0, BUTTON_B))
		But_B++;

	if (IsButtonPressed(0, BUTTON_X))
		But_X++;

	if (IsButtonPressed(0, BUTTON_Y))
		But_Y++;

	if (IsButtonPressed(0, BUTTON_L1))
		But_L1++;

	if (IsButtonPressed(0, BUTTON_R1))
		But_R1++;

	if (IsButtonPressed(0, BUTTON_L2))
		But_L2++;

	if (IsButtonPressed(0, BUTTON_R2))
		But_R2++;

	if (IsButtonPressed(0, BUTTON_SELECT))
		But_SELECT++;

	if (IsButtonPressed(0, BUTTON_START))
		But_START++;

	if (IsButtonPressed(0, BUTTON_LS))
		But_LS++;

	if (IsButtonPressed(0, BUTTON_RS))
		But_RS++;

	if (IsButtonPressed(0, BUTTON_R_UP))
		Joy_RV++;

	if (IsButtonPressed(0, BUTTON_R_DOWN))
		Joy_RV--;

	if (IsButtonPressed(0, BUTTON_R_RIGHT))
		Joy_RH++;

	if (IsButtonPressed(0, BUTTON_R_LEFT))
		Joy_RH--;

	if (IsButtonPressed(0, BUTTON_UP))
		Joy_LV++;

	if (IsButtonPressed(0, BUTTON_DOWN))
		Joy_LV--;

	if (IsButtonPressed(0, BUTTON_RIGHT))
		Joy_LH++;

	if (IsButtonPressed(0, BUTTON_LEFT))
		Joy_LH--;
}


void UninitDBD(void)
{
	if (g_pD3DXFont != NULL)
	{// 情報表示用フォントの開放
		g_pD3DXFont->Release();
		g_pD3DXFont = NULL;
	}
}

void CheckBoarStat(void)
{
	BOAR *boar = GetBoar(0);
	// reset stat
	Idle = 0;
	Roam = 0;
	Chase = 0;
	Check = 0;
	switch (boar->Stat)
	{
	case IDLING:
		Idle++;
		break;
	case ROAMING:
		Roam++;
		break;
	case CHASING:
		Chase++;
		break;
	case CHECKING:
		Check++;	
		break;
	}

}
#endif
