//=============================================================================
//
// ゲートの処理 [gate.cpp]
// Author : 川井一生
//
//=============================================================================
#include "gate.h"

#include "player.h"
#include "gem.h"

#include "checkhit.h"

#include "mathWk.h"
/*
ゲートの門の処理
マップ上のジェムをすべて集めると開く

mesh gate
opens once all gems are collected
*/

//=============================================================================
// マクロ定義
#define MODEL_GATE		("data/MODEL/gate000.x")							// 門のモデル
#define TEXTURE_GATE	("data/TEXTURE/gate000.png")						// 門のテクスチャー
#define MODEL_GATE_HIT	("data/MODEL/gate_hitbox000.x")						// 門のモデル

#define MODEL_DOOR		("data/MODEL/gate_door000.x")						// ドアのモデル
#define TEXTURE_DOOR	("data/TEXTURE/gate_door000.png")					// ドアのテクスチャー
#define MODEL_DOOR_HIT	("data/MODEL/gate_door_hit_box.x")				// ドアのモデル

#define DOOR_DISTANCE	(120 * SCL_VALUE)									// 門からのドアの位置までの距離
#define DOOR_OPEN_ANIMATION_TIME	(30)									// ドアが開くアニメーションの時間

#define	VALUE_MOVE		(0.5f)												// 移動量
#define	ROT_VALUE		(D3DX_PI * 0.02f)									// 回転量
#define SCL_VALUE		(1.0f)												// 大きさ
#define STARTING_ROT	(D3DX_PI * -0.15f)
#define RANGE_VALUE		(50)												// 箱からの距離


enum
{
	TEX_GATE = 0,
	TEX_DOOR,
	TEX_MAX,
};

//=============================================================================
// プロトタイプ宣言
void MoveGate(int no);

//=============================================================================
// グローバル変数
LPDIRECT3DTEXTURE9		D3DTextureGate[TEX_MAX] = { NULL , NULL };		// テクスチャへのポインタ

GATE						gateWk[GATE_MAX];			// フィールドへのポインタ

bool						isOpn = false;
int							OpnAnimCnt = DOOR_OPEN_ANIMATION_TIME;

//=============================================================================
// 初期化処理
HRESULT InitGate(void)
{
	GATE *gate = &gateWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの設定
	D3DXCreateTextureFromFile(pDevice, TEXTURE_GATE, &D3DTextureGate[TEX_GATE]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_DOOR, &D3DTextureGate[TEX_DOOR]);

	int i;
	for (i = 0; i < GATE_MAX; i ++, gate ++)
	{
		gate->mesh.use = true;

		gate->mesh.Pos = D3DXVECTOR3(3000.0f, 0.0f, 2450.0f);

		// 位置・回転・スケールの初期設定
		gate->mesh.Rot = D3DXVECTOR3(0.0f, STARTING_ROT, 0.0f);
		gate->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);

		// モデル関係の初期化
		gate->mesh.VtxBuff = NULL;
		//MakeVertexGate(pDevice);
		gate->mesh.Mesh = NULL;
		gate->mesh.BuffMat = NULL;
		gate->mesh.NumMat = 0;

		D3DXVECTOR3 temp;
		switch (i % COMP4GATE)
		{
		case 0: // 門
			if (FAILED(D3DXLoadMeshFromX(MODEL_GATE, D3DXMESH_SYSTEMMEM, pDevice, NULL, &gate->mesh.BuffMat, NULL, &gate->mesh.NumMat, &gate->mesh.Mesh)))
				return E_FAIL;
			gate->mesh.TexId = TEX_GATE;

			gate->isHitbox = false;
			break;
		case 1: // 門の当たり判定用
			if (FAILED(D3DXLoadMeshFromX(MODEL_GATE_HIT, D3DXMESH_SYSTEMMEM, pDevice, NULL, &gate->mesh.BuffMat, NULL, &gate->mesh.NumMat, &gate->mesh.Mesh)))
				return E_FAIL;
			gate->isHitbox = true;

			SetVtxData(&gate->mesh);
			break;
		case 2: // 片方のドア
			if (FAILED(D3DXLoadMeshFromX(MODEL_DOOR, D3DXMESH_SYSTEMMEM, pDevice, NULL, &gate->mesh.BuffMat, NULL, &gate->mesh.NumMat, &gate->mesh.Mesh)))
				return E_FAIL;
			gate->mesh.TexId = TEX_DOOR;

			gate->isHitbox = false;
			temp = Ang2Vec(gate->mesh.Rot, DOOR_DISTANCE);
			gate->mesh.Pos -= temp;
			break;
		case 3: // ドアの当たり判定用
			if (FAILED(D3DXLoadMeshFromX(MODEL_DOOR_HIT, D3DXMESH_SYSTEMMEM, pDevice, NULL, &gate->mesh.BuffMat, NULL, &gate->mesh.NumMat, &gate->mesh.Mesh)))
				return E_FAIL;
			gate->isHitbox = true;
			temp = Ang2Vec(gate->mesh.Rot, DOOR_DISTANCE);
			gate->mesh.Pos -= temp;
			SetVtxData(&gate->mesh);
			break;
		case 4: // もう片方のドア
			if (FAILED(D3DXLoadMeshFromX(MODEL_DOOR, D3DXMESH_SYSTEMMEM, pDevice, NULL, &gate->mesh.BuffMat, NULL, &gate->mesh.NumMat, &gate->mesh.Mesh)))
				return E_FAIL;
			gate->mesh.TexId = TEX_DOOR;

			gate->isHitbox = false;

			gate->mesh.Rot.y += (D3DX_PI * 1);
			temp = Ang2Vec(gate->mesh.Rot, DOOR_DISTANCE);
			gate->mesh.Pos -= temp;

			SetVtxData(&gate->mesh);
			break;
		case 5: // もう片方のドアの当たり判定用
			if (FAILED(D3DXLoadMeshFromX(MODEL_DOOR_HIT, D3DXMESH_SYSTEMMEM, pDevice, NULL, &gate->mesh.BuffMat, NULL, &gate->mesh.NumMat, &gate->mesh.Mesh)))
				return E_FAIL;
			gate->isHitbox = true;

			gate->mesh.Rot.y += (D3DX_PI * 1);
			temp = Ang2Vec(gate->mesh.Rot, DOOR_DISTANCE);
			gate->mesh.Pos -= temp;

			SetVtxData(&gate->mesh);
			break;
		}
	}
}

//=============================================================================
// 門のリセット処理
void ResetGate(void)
{
	GATE *gate = &gateWk[0];

	isOpn = false;
	OpnAnimCnt = DOOR_OPEN_ANIMATION_TIME;
	int i;
	for (i = 0; i < GATE_MAX; i++, gate++)
	{
		gate->mesh.use = true;

		gate->mesh.Pos = D3DXVECTOR3(3000.0f, 0.0f, 2450.0f);

		// 位置・回転・スケールの初期設定
		gate->mesh.Rot = D3DXVECTOR3(0.0f, STARTING_ROT, 0.0f);
		gate->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);


		D3DXVECTOR3 temp;
		switch (i % COMP4GATE)
		{
		case 0:
			break;
		case 1:
			break;
		case 2:
			temp = Ang2Vec(gate->mesh.Rot, DOOR_DISTANCE);
			gate->mesh.Pos -= temp;
			break;
		case 3:
			temp = Ang2Vec(gate->mesh.Rot, DOOR_DISTANCE);
			gate->mesh.Pos -= temp;
			break;
		case 4:
			gate->mesh.Rot.y += (D3DX_PI * 1);
			temp = Ang2Vec(gate->mesh.Rot, DOOR_DISTANCE);
			gate->mesh.Pos -= temp;
			break;
		case 5:
			gate->mesh.Rot.y += (D3DX_PI * 1);
			temp = Ang2Vec(gate->mesh.Rot, DOOR_DISTANCE);
			gate->mesh.Pos -= temp;
			break;
		}
	}

}

//=============================================================================
// 終了処理
void UninitGate(void)
{
	GATE *gate = &gateWk[0];
	for (int i = 0; i < TEX_MAX;i++)
		if (D3DTextureGate[i]!= NULL)
		{// テクスチャの開放
			D3DTextureGate[i]->Release();
			D3DTextureGate[i] = NULL;
		}
	for (int i = 0; i < GATE_MAX; i++, gate++)
	{
		if (gate->mesh.VtxBuff != NULL)
		{// 頂点バッファの開放
			gate->mesh.VtxBuff->Release();
			gate->mesh.VtxBuff = NULL;
		}

		if (gate->mesh.Mesh != NULL)
		{// メシュの解放
			gate->mesh.Mesh->Release();
			gate->mesh.Mesh = NULL;
		}

		if (gate->mesh.IdxBuff != NULL)
		{// インデックスバッファとヴァーテックスポスは一緒に開放
			free(gate->mesh.IdxBuff);
			free(gate->mesh.VtxPos);
			gate->mesh.IdxBuff = NULL;
			gate->mesh.VtxPos = NULL;
		}

	}
}



//=============================================================================
// 更新処理
void UpdateGate(void)
{
	GATE *gate = &gateWk[0];
	GEM *gem = GetGem(0);
	D3DXVECTOR3 Vel = { 0.0f,0.0f,0.0f };
	int i;
	int GemCnt = 0;
	for (i = 0; i < GEM_MAX; i++, gem++)
		if (gem->mesh.use == false)
		{
			GemCnt++;
		}
	if (GemCnt >= GEM_MAX)
	{
		isOpn = true;
	}
	for (i = 0; i < GATE_MAX; i++, gate++)
		if (gate->mesh.use
			&& isOpn
			&& OpnAnimCnt > 0)
		{
			switch (i % COMP4GATE)
			{
			case 0:
				break;
			case 1:
				break;
			case 2:
				gate->mesh.Rot.y += ROT_VALUE;
				break;
			case 3:
				gate->mesh.Rot.y += ROT_VALUE;
				break;
			case 4:
				gate->mesh.Rot.y -= ROT_VALUE;
				break;
			case 5:
				gate->mesh.Rot.y -= ROT_VALUE;
				break;
			}

		}
	if (isOpn)
		OpnAnimCnt--;
}


//=============================================================================
// 描画処理
void DrawGate(void)
{
	GATE *gate = &gateWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// スケール、回転、平行移動
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	int i, j;

	for (i = 0; i < GATE_MAX; i++, gate++)
		if (gate->mesh.use == true)
		{
			// ワールドマトリクス初期化
			D3DXMatrixIdentity(&gate->mesh.mtxWorld);

			// スケールを反映	(S)
			D3DXMatrixScaling(&mtxScl, gate->mesh.Scl.x, gate->mesh.Scl.y, gate->mesh.Scl.z);
			D3DXMatrixMultiply(&gate->mesh.mtxWorld, &gate->mesh.mtxWorld, &mtxScl); 	// &mesh->mtxWorld = &mesh->mtxWorld * &mtxScl
																					// 回転を反映		(R)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, gate->mesh.Rot.y, gate->mesh.Rot.x, gate->mesh.Rot.z);
			D3DXMatrixMultiply(&gate->mesh.mtxWorld, &gate->mesh.mtxWorld, &mtxRot);	// &mesh->mtxWorld = &mesh->mtxWorld * &mtxRot
																					// 平行移動を反映	(T)
			D3DXMatrixTranslation(&mtxTranslate, gate->mesh.Pos.x, gate->mesh.Pos.y, gate->mesh.Pos.z);
			D3DXMatrixMultiply(&gate->mesh.mtxWorld, &gate->mesh.mtxWorld, &mtxTranslate);

			// ワールドマトリックス設定
			pDevice->SetTransform(D3DTS_WORLD, &gate->mesh.mtxWorld);

			// 現在のマテリアルを保存
			pDevice->GetMaterial(&matDef);

			// マテリアル情報へのポインタを
			pD3DXMat = (D3DXMATERIAL*)gate->mesh.BuffMat->GetBufferPointer();

			if (gate->isHitbox == false)
				for (j = 0; j < (int)gate->mesh.NumMat; j++)
				{
					pDevice->SetMaterial(&pD3DXMat[j].MatD3D);	// マテリアルの設定

					pDevice->SetTexture(0, D3DTextureGate[gate->mesh.TexId]);	// テクスチャの設定
					gate->mesh.Mesh->DrawSubset(j);
				}

			// マテリアルに戻す
			pDevice->SetMaterial(&matDef);
		}
}

//=============================================================================
// ジェムの移動処理
void MoveGate(int no)
{
	GATE *gate = &gateWk[no];

	gate->mesh.Rot.y += ROT_VALUE;

}


//=============================================================================
// ジェムのゲット関数
GATE *GetGate(int no)
{
	return (&gateWk[no]);
}
