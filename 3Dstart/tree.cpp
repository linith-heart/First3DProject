//=============================================================================
//
// 木の処理 [tree.cpp]
// Author : 川井一生
//
//=============================================================================
#include "tree.h"

#include "camera.h"
#include "player.h"

#include "checkhit.h"
/*
メシュの木の処理

mesh tree
*/

//=============================================================================
// マクロ定義
#define TEXTURE_TREE	("data/TEXTURE/tree000.png")					// 木のテクスチャー
#define MODEL			("data/MODEL/tree000.x")						// 木のモデル
#define	VALUE_MOVE		(0.5f)											// 移動量
#define	ROT_VALUE		(D3DX_PI * 0.02f)								// 回転量
#define SCL_VALUE		(1.0f)											// 大きさ
#define RANGE_VALUE		(50)											// 箱からの距離

#define	ALPHA			(170)


//=============================================================================
// プロトタイプ宣言
void MoveTree(void);

//=============================================================================
// グローバル変数
LPDIRECT3DTEXTURE9		D3DTextureTree = NULL;		// テクスチャへのポインタ

TREE					treeWk[TREE_MAX];			// フィールドへのポインタ


//=============================================================================
// 初期化処理
HRESULT InitTree(void)
{
	TREE *tree = &treeWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの設定
	D3DXCreateTextureFromFile(pDevice, TEXTURE_TREE, &D3DTextureTree);

	int i;
	for (i = 0; i < TREE_MAX; i++, tree++)
	{
		tree->mesh.use = true;

		// 位置・回転・スケールの初期設定
		tree->mesh.Pos = D3DXVECTOR3(-100.0f * i, 0.0f, -100.0f);
		tree->mesh.Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		tree->mesh.Scl = D3DXVECTOR3(SCL_VALUE, SCL_VALUE, SCL_VALUE);

		// モデル関係の初期化
		tree->mesh.VtxBuff = NULL;
		//MakeVertexTree(pDevice);
		tree->mesh.Mesh = NULL;
		tree->mesh.BuffMat = NULL;
		tree->mesh.NumMat = 0;

		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(MODEL,			// モデルデータ
			D3DXMESH_SYSTEMMEM,					// 使用するメモリのオプション
			pDevice,							// デバイス 
			NULL,								// 未使用
			&tree->mesh.BuffMat,				// マテリアルデータへのポインター
			NULL,								// 未使用
			&tree->mesh.NumMat,				// D3DXMATERIAL構造体の数
			&tree->mesh.Mesh)))				// メッシュデータへのポインター
		{
			return E_FAIL;
		}


		int			VtxCnt = tree->mesh.Mesh->GetNumVertices();	// メシュの頂点数
		int			Polycnt = tree->mesh.Mesh->GetNumFaces();		// メシュのポリゴン数
		int			BytePerVtx = tree->mesh.Mesh->GetNumBytesPerVertex();	// １頂点当たりのバイト数
		WORD		*idx;
		char		*pVtx;

		// ポリゴン数 * 3 だけインデックスが必要	
		// インデックスひとつのサイズはWORD型 = 2バイトなので,
		// インデックスを格納する配列を(ポリゴン数 * 3頂点 * 2バイト)分だけ動的確保
		// 終了時に free(tree->mesh.IdxBuff); でメモリ開放を忘れずに
		tree->mesh.IdxBuff = (WORD*)malloc(sizeof(WORD) * Polycnt * 3);

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		tree->mesh.Mesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&idx);
		// インデックスバファの情報を取得
		for (int i = 0; i < (Polycnt * 3); i++)
		{
			tree->mesh.IdxBuff[i] = idx[i];
		}

		// 頂点データをアンロックする
		tree->mesh.Mesh->UnlockIndexBuffer();


		// 頂点座標ひとつのサイズはD3DXVECTOR3型 = 12バイトなので,
		// 頂点座標を格納する配列を(頂点数 * 12バイト)分だけ動的確保
		// 終了時に free(tree->mesh.VtxPos); でメモリ開放を忘れずに
		tree->mesh.VtxPos = (D3DXVECTOR3*)malloc(sizeof(D3DXVECTOR3) * VtxCnt);

		// バーテックスデータをロック
		tree->mesh.Mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtx);

		// バーテックスデータを取得
		for (int i = 0; i < VtxCnt; i++)
		{
			tree->mesh.VtxPos[i] = *(D3DXVECTOR3*)&pVtx[i * BytePerVtx];
		}

		// 頂点データをアンロックする
		tree->mesh.Mesh->UnlockVertexBuffer();

	}
	return S_OK;

}

//=============================================================================
// 終了処理
void UninitTree(void)
{
	TREE *tree = &treeWk[0];
	if (D3DTextureTree != NULL)
	{// テクスチャの開放
		D3DTextureTree->Release();
		D3DTextureTree = NULL;
	}

	for (int i = 0; i < TREE_MAX; i++, tree++)
	{
		if (tree->mesh.VtxBuff != NULL)
		{// 頂点バッファの開放
			tree->mesh.VtxBuff->Release();
			tree->mesh.VtxBuff = NULL;
		}

		if (tree->mesh.Mesh != NULL)
		{// メシュの解放
			tree->mesh.Mesh->Release();
			tree->mesh.Mesh = NULL;
		}

		if (tree->mesh.IdxBuff != NULL)
		{// インデックスバッファとヴァーテックスポスは一緒に開放
			free(tree->mesh.IdxBuff);
			free(tree->mesh.VtxPos);
			tree->mesh.IdxBuff = NULL;
			tree->mesh.VtxPos = NULL;
		}
	}
}

//=============================================================================
// 更新処理
void UpdateTree(void)
{
	TREE *tree = &treeWk[0];
	PLAYER *player = GetPlayer(0);
	D3DXVECTOR3 Vel = { 0.0f,0.0f,0.0f };
	int i;
	for (i = 0; i < TREE_MAX; i++, tree++)
	{


	}
}

//=============================================================================
// 描画処理
void DrawTree(void)
{
	TREE *tree = &treeWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// スケール、回転、平行移動
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	int i, j;

	// αテストを有効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, ALPHA);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	for (i = 0; i < TREE_MAX; i++, tree++)
		if (tree->mesh.use == true)
		{

			// ワールドマトリクス初期化
			D3DXMatrixIdentity(&tree->mesh.mtxWorld);

			// スケールを反映	(S)
			D3DXMatrixScaling(&mtxScl, tree->mesh.Scl.x, tree->mesh.Scl.y, tree->mesh.Scl.z);
			D3DXMatrixMultiply(&tree->mesh.mtxWorld, &tree->mesh.mtxWorld, &mtxScl); 	// &mesh->mtxWorld = &mesh->mtxWorld * &mtxScl
																						// 回転を反映		(R)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, tree->mesh.Rot.y, tree->mesh.Rot.x, tree->mesh.Rot.z);
			D3DXMatrixMultiply(&tree->mesh.mtxWorld, &tree->mesh.mtxWorld, &mtxRot);	// &mesh->mtxWorld = &mesh->mtxWorld * &mtxRot
																						// 平行移動を反映	(T)
			D3DXMatrixTranslation(&mtxTranslate, tree->mesh.Pos.x, tree->mesh.Pos.y, tree->mesh.Pos.z);
			D3DXMatrixMultiply(&tree->mesh.mtxWorld, &tree->mesh.mtxWorld, &mtxTranslate);

			// ワールドマトリックス設定
			pDevice->SetTransform(D3DTS_WORLD, &tree->mesh.mtxWorld);

			// 現在のマテリアルを保存
			pDevice->GetMaterial(&matDef);

			// マテリアル情報へのポインタを
			pD3DXMat = (D3DXMATERIAL*)tree->mesh.BuffMat->GetBufferPointer();


			for (j = 0; j < (int)tree->mesh.NumMat; j++)
			{
				pDevice->SetMaterial(&pD3DXMat[j].MatD3D);	// マテリアルの設定

				pDevice->SetTexture(0, D3DTextureTree);	// テクスチャの設定
				tree->mesh.Mesh->DrawSubset(j);
			}

			// マテリアルに戻す
			pDevice->SetMaterial(&matDef);
		}

	// αテストを無効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

//=============================================================================
// 木の移動
void MoveTree(void)
{
	TREE *tree = &treeWk[0];
	PLAYER *player = GetPlayer(0);

}

//=============================================================================
// 箱のゲット関数
TREE *GetTree(int no)
{
	return (&treeWk[no]);
}






int hitCheckTree(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
{
	TREE *tree = &treeWk[0];
	int				ans = 0;	// 一先ず当たっていないをセットしておく

	D3DXVECTOR3		pos[3];		// ポリゴンの法線
	D3DXVECTOR3		vel = { 0.0f,0.0f,0.0f };


	int PolyCnt = tree->mesh.Mesh->GetNumFaces();

	for (int i = 0; i<PolyCnt; i++)
	{	// まずは、ポリゴンの頂点を求める
		int idx0 = tree->mesh.IdxBuff[i * 3];
		int idx1 = tree->mesh.IdxBuff[i * 3 + 1];
		int idx2 = tree->mesh.IdxBuff[i * 3 + 2];

		// 頂点座標の設定
		pos[0] = tree->mesh.VtxPos[idx0];
		pos[1] = tree->mesh.VtxPos[idx1];
		pos[2] = tree->mesh.VtxPos[idx2];

		// 左下側ポリゴンと線分の当たり判定
		ans = CheckHitLine(pos[0], pos[2], pos[3], pos0, pos1, &tree->mesh.mtxWorld, &vel);
		if (ans != 0)
		{
			break;
		}


	}

	return(ans);

}
