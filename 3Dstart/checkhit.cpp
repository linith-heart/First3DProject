//=============================================================================
//
// 当たり判定処理 [checkhit.cpp]
// Author : 川井一生
//
//=============================================================================
#include "checkhit.h"
#include "player.h"
#include "boar.h"
#include "car.h"
#include "crate.h"
#include "rock.h"
#include "tree.h"
#include "bullet.h"
#include "mathWk.h"
#include "map.h"
#include "gem.h"
#include "fence.h"
//=============================================================================
// マクロ定義
#define GROUND_CHECK_RANGE		(50.0f)

//=============================================================================
// プロトタイプ宣言


//=============================================================================
// グローバル変数:


//=============================================================================
// 当たり判定処理
void UpdateCheckhit(void)
{
	PLAYER	*player	= GetPlayer(0);
	GEM		*gem	= GetGem(0);
	BOAR	*boar	= GetBoar(0);

	int i, j;

	// プレイヤ－とジェム		player and gem hit check	
	for (i = 0, player = GetPlayer(0); i < PLAYER_MAX; i++, player++)
		if (player->use)
			for (j = 0, gem = GetGem(0); j < GEM_MAX; j++, gem++)
				if (gem->mesh.use)
					if ((CheckHitBC(player->Pos, gem->mesh.Pos, 30, 30)) == true)
						gem->mesh.use = false;


	// プレイヤーとイノシシ		player and boar hit check
	for (i = 0, player = GetPlayer(0); i < PLAYER_MAX; i++, player++)
		if (player->use && player->HitTime <= 0)
			for (j = 0, boar = GetBoar(0); j < BOAR_MAX;j++,boar++)
				if (boar->ene.use)
					if (CheckHitBC(player->Pos, boar->ene.Pos, 20, 20))
					{
						PlayerHit(i);
						break;
					}


}

//=============================================================================
// BCの当たり判定処理
// pos1  : オブジェ１の位置			position of first object
// pos2  : オブジェ２の位置			position of second object
// size1 : オブジェ１の当たる範囲	radius of hit box of first object
// size2 : オブジェ２の当たる範囲	radius of hit box of second object
//=============================================================================
bool CheckHitBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float size1, float size2)
{

	D3DXVECTOR3 temp = pos1 - pos2;
	float LengthSq = D3DXVec3LengthSq(&temp);

	if (LengthSq <= ((size1 + size2)*(size1 + size2)))
	{
		return true;
	}
	return false;
}


//=============================================================================
// ヴァーテックスデータの制作
// environment hub.h で用意したMESH構造体を渡し
// MESHの中のVtxPosを準備
//=============================================================================
void SetVtxData(MESH *mesh)
{
	int			VtxCnt = mesh->Mesh->GetNumVertices();	// メシュの頂点数
	int			Polycnt = mesh->Mesh->GetNumFaces();		// メシュのポリゴン数
	int			BytePerVtx = mesh->Mesh->GetNumBytesPerVertex();	// １頂点当たりのバイト数
	WORD		*idx;
	char		*pVtx;

	// ポリゴン数 * 3 だけインデックスが必要	
	// インデックスひとつのサイズはWORD型 = 2バイトなので,
	// インデックスを格納する配列を(ポリゴン数 * 3頂点 * 2バイト)分だけ動的確保
	// 終了時に free(mesh->IdxBuff); でメモリ開放を忘れずに
	mesh->IdxBuff = (WORD*)malloc(sizeof(WORD) * Polycnt * 3);

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	mesh->Mesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&idx);
	// インデックスバファの情報を取得
	for (int i = 0; i < (Polycnt * 3); i++)
	{
		mesh->IdxBuff[i] = idx[i];
	}

	// 頂点データをアンロックする
	mesh->Mesh->UnlockIndexBuffer();


	// 頂点座標ひとつのサイズはD3DXVECTOR3型 = 12バイトなので,
	// 頂点座標を格納する配列を(頂点数 * 12バイト)分だけ動的確保
	// 終了時に free(mesh->VtxPos); でメモリ開放を忘れずに
	mesh->VtxPos = (D3DXVECTOR3*)malloc(sizeof(D3DXVECTOR3) * VtxCnt);

	// バーテックスデータをロック
	mesh->Mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtx);

	// バーテックスデータを取得
	for (int i = 0; i < VtxCnt; i++)
	{
		mesh->VtxPos[i] = *(D3DXVECTOR3*)&pVtx[i * BytePerVtx];
	}

	// 頂点データをアンロックする
	mesh->Mesh->UnlockVertexBuffer();
}


//=============================================================================
// ポリゴンと線分との当たり判定
// p0  :ポリゴンの頂点1		first vector of the polygon
// p1  :ポリゴンの頂点2		second vector of the polygon
// p2  :ポリゴンの頂点3		third vector of the polygon
// pos0:始点（移動前）		starting position
// pos1:終点（移動後）		ending position
// world: メシュのワールド座標	world matrax of the mesh
// *distance : pos0 からポリゴンと線分の交点までの距離
//=============================================================================
int CheckHitLine(D3DXVECTOR3 p0, D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXMATRIX *world, D3DXVECTOR3 *distance)
{
	D3DXVECTOR3 norPlane;								// ポリゴンの法線

	D3DXVECTOR3 P0;
	D3DXVECTOR3 P1;
	D3DXVECTOR3 P2;

	// ローカル座標からワールド座標に転換
	D3DXVec3TransformCoord(&P0, &p0, world);
	D3DXVec3TransformCoord(&P1, &p1, world);
	D3DXVec3TransformCoord(&P2, &p2, world);

	D3DXVECTOR3 v1 = P1 - P0;
	D3DXVECTOR3 v2 = P2 - P0;
	Vec3Cross(&norPlane, &v1, &v2);					// 外積で法線を求める
	D3DXVec3Normalize(&norPlane, &norPlane);			// 正規化

	D3DXVECTOR3 vecFromP0ToPos0 = pos0 - P0;	// ポリゴン頂点0から線分の始点までのベクトル
	D3DXVECTOR3 vecFromP0ToPos1 = pos1 - P0;	// ポリゴン頂点0から線分の終点までのベクトル

	// 平面と線分の内積とって衝突している可能性を調べる
	// 求めた法線とベクトル２つ（線分の両端とポリゴン上の任意の点）の
	// 内積とって衝突している可能性を調べる
	bool isCollisionPlaneToLineSegment;
	float dotPos0ToNor;
	float dotPos1ToNor;

	// 法線と始点、終点との各内積
	dotPos0ToNor = dotProduct(&vecFromP0ToPos0, &norPlane);
	//dotPos1ToNor = -dotPos0ToNor;
	dotPos1ToNor = dotProduct(&vecFromP0ToPos1, &norPlane);

	isCollisionPlaneToLineSegment = (dotPos0ToNor * dotPos1ToNor <= 0);

	// 平面と線分が衝突しているか
	// 衝突している場合、始点、終点が面の表裏に存在することになり、
	// なす角がそれぞれ鋭角、鈍角になるので内積がそれぞれ異なる符号
	// になる。異なる符号を掛け合わせると必ず負になることを利用

	// 結果当たっていなかったら終了
	if (!isCollisionPlaneToLineSegment)
	{
		return(0);	// 当たっている可能性は無い(当たっていない)
	}

	// 以下、詳細なチェック //

	// ポリゴンと線分の交点を求める

	// 内分比を求める
	float distance0, distance1;
	distance0 = fabsf(dotProduct(&norPlane, &vecFromP0ToPos0));	// pos0から平面までの距離
																//distance1 = 1-distance0;	// pos1から平面までの距離
	distance1 = fabsf(dotProduct(&norPlane, &vecFromP0ToPos1));	// pos1から平面までの距離
	float internalRatio = distance0 / (distance0 + distance1);	// 内分比

																// p0から交点へのベクトル
	D3DXVECTOR3 vecFromP0ToPosIntersection; // p0から交点へのベクトル
	vecFromP0ToPosIntersection = ((1.0f - internalRatio) * vecFromP0ToPos0) + (internalRatio * vecFromP0ToPos1);

	// 交点を求める
	D3DXVECTOR3 posIntersectionPlaneToLine;
	posIntersectionPlaneToLine = P0 + vecFromP0ToPosIntersection;


	// 各辺と交点の関係を調べる //
	const int MAX_VERTEX_POLYGON = 3;

	// ポリゴンの各辺のベクトル
	D3DXVECTOR3 vec[MAX_VERTEX_POLYGON];

	vec[0] = P1 - P0;
	vec[1] = P2 - P1;
	vec[2] = P0 - P2;

	// 各頂点と交点とのベクトル
	D3DXVECTOR3 vecFromIntersection[MAX_VERTEX_POLYGON];

	vecFromIntersection[0] = posIntersectionPlaneToLine - P0;
	vecFromIntersection[1] = posIntersectionPlaneToLine - P1;
	vecFromIntersection[2] = posIntersectionPlaneToLine - P2;

	// ３つのベクトルに対し最終チェック
	for (int i = 0; i < MAX_VERTEX_POLYGON; i++)
	{// 各辺と交点へのベクトルの外積で法線を求めて正規化する
		D3DXVECTOR3 nor;
		Vec3Cross(&nor, &vec[i], &vecFromIntersection[i]);
		Vec3Normalize(&nor);
		if (dotProduct(&nor, &norPlane) <= 0)
		{// 内積で向きが合っているか調べる
			return (0);	// ぶつかってない
		}
	}


	*distance = posIntersectionPlaneToLine - pos1;

	// ぶつかっている
	return(1);
}


//=============================================================================
// pos0 と pos1 の間にオブジェクトがあるか確認する処理
// pos0  : 始点
// pos1  : 終点
// *ObjPos : 間にあったオブジェクトのPosをゲット
//=============================================================================
bool VisionCheck(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 *ObjPos)
{
	CRATE *crate = GetCrate(0);
	ROCK *rock = GetRock(0);
	TREE *tree = GetTree(0);
	FENCE *fence = GetFence(0);

	D3DXVECTOR3	v0	= pos1 - pos0;				// pos0からpos1までのベクトル
	float		L0	= D3DXVec3LengthSq(&v0);	// pos0からpos1までの距離
	D3DXVECTOR3 v1;								// pos0からチェックするオブジェクトまでのベクトル
	float		L1;								// pos0からチェックするオブジェクトまでの距離
	D3DXVECTOR3 temp = { 0.0f,0.0f,0.0f };
	int i;
	// 箱とのビジョンチェック
	for (i = 0, crate = GetCrate(0); i < CRATE_MAX; i++, crate++)
		if (crate->mesh.use == true)
		{
			v1 = crate->mesh.Pos - pos0;
			L1 = D3DXVec3LengthSq(&v1);
			if (L1 < L0)
			{
				if (CheckHitLinePrep(pos0, pos1, &temp, &crate->mesh))
				{
					*ObjPos = crate->mesh.Pos;
					return (true);
				}
			}
		}

	// 石とのビジョンチェック
	for (i = 0, rock = GetRock(0); i < ROCK_MAX; i++, rock++)
		if (rock->mesh.use == true
			&& rock->isHitbox == true)
		{
			v1 = rock->mesh.Pos - pos0;
			L1 = D3DXVec3LengthSq(&v1);
			if (L1 < L0)
			{
				if (CheckHitLinePrep(pos0, pos1, &temp, &rock->mesh))
				{
					*ObjPos = rock->mesh.Pos;
					return (true);
				}
			}
		}

	// 木とのビジョンチェック
	for (i = 0, tree = GetTree(0); i < TREE_MAX; i++, tree++)
		if (tree->mesh.use == true)
		{
			v1 = tree->mesh.Pos - pos0;
			L1 = D3DXVec3LengthSq(&v1);
			if (L1 < L0)
			{
				if (CheckHitLinePrep(pos0, pos1, &temp, &tree->mesh))
				{
					*ObjPos = tree->mesh.Pos;
					return (true);
				}
			}
		}

	// 策とのビジョンチェック
	for (i = 0, fence = GetFence(0); i < FENCE_MAX; i++, fence++)
		if (fence->mesh.use == true
			&& fence->isHitbox == true)
		{
			v1 = fence->mesh.Pos - pos0;
			L1 = D3DXVec3LengthSq(&v1);
			if (L1 < L0)
			{
				if (CheckHitLinePrep(pos0, pos1, &temp, &fence->mesh))
				{
					*ObjPos = fence->mesh.Pos;
					return (true);
				}
			}
		}
	// 間に何もなければ
	return (false);
}







//=============================================================================
// 線分の当たり判定の準備処理
// pos0 : 始点
// pos1 : 終点
// *distance : pos0からポリゴンと線分の交点までのベクトル
// *mesh : 当たり判定の対象になるメシュ型のオブジェクト
//=============================================================================
bool CheckHitLinePrep(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 *distance, MESH *mesh)
{
	bool	ans = false;		// 結果

	D3DXVECTOR3		pos[3];		// ポリゴンの法線


	int PolyCnt = mesh->Mesh->GetNumFaces();

	for (int i = 0; i<PolyCnt; i++)
	{	// ポリゴンの頂点を求める

		int idx0 = mesh->IdxBuff[i * 3];
		int idx1 = mesh->IdxBuff[i * 3 + 1];
		int idx2 = mesh->IdxBuff[i * 3 + 2];

		// 頂点座標の設定
		pos[0] = mesh->VtxPos[idx0];
		pos[1] = mesh->VtxPos[idx1];
		pos[2] = mesh->VtxPos[idx2];

		// ポリゴンと線分の当たり判定
		ans = CheckHitLine(pos[0], pos[1], pos[2], pos0, pos1, &mesh->mtxWorld, distance);
		if (ans == true)
		{
			break;
		}


	}

	return(ans);

}


//=============================================================================
// 線分の当たり判定の準備処理
// pos0 : 始点
// pos1 : 終点
// *distance : pos0からポリゴンと線分の交点までのベクトル
//=============================================================================
bool CheckHitGroundPrep(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 *distance)
{
	MAP *map = GetMap(2);

	bool	ans = false;		// 結果

	D3DXVECTOR3		pos[3];		// ポリゴンの法線

	int PolyCnt = map->mesh.Mesh->GetNumFaces();

	for (int i = 0; i<PolyCnt; i++)
	{	// ポリゴンの頂点を求める

		int idx0 = map->mesh.IdxBuff[i * 3];
		int idx1 = map->mesh.IdxBuff[i * 3 + 1];
		int idx2 = map->mesh.IdxBuff[i * 3 + 2];

		// 頂点座標の設定
		pos[0] = map->mesh.VtxPos[idx0];
		pos[1] = map->mesh.VtxPos[idx1];
		pos[2] = map->mesh.VtxPos[idx2];

		// ポリゴンと線分の当たり判定
		ans = CheckHitLine(pos[0], pos[1], pos[2], pos0, pos1, &map->mesh.mtxWorld, distance);
		if (ans != 0)
		{
			break;
		}
	}
	return(ans);
}


//=============================================================================
// 線分の当たり判定の準備処理
// pos0 : 始点
// pos1 : 終点
// *distance : pos0からポリゴンと線分の交点までのベクトル
//=============================================================================
bool CheckHitWallPrep(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 *distance)
{
	MAP *map = GetMap(3);

	bool	ans = false;	// 結果

	D3DXVECTOR3		pos[3];	// ポリゴンの法線

	int PolyCnt = map->mesh.Mesh->GetNumFaces();

	for (int i = 0; i<PolyCnt; i++)
	{	// ポリゴンの頂点を求める

		int idx0 = map->mesh.IdxBuff[i * 3];
		int idx1 = map->mesh.IdxBuff[i * 3 + 1];
		int idx2 = map->mesh.IdxBuff[i * 3 + 2];

		// 頂点座標の設定
		pos[0] = map->mesh.VtxPos[idx0];
		pos[1] = map->mesh.VtxPos[idx1];
		pos[2] = map->mesh.VtxPos[idx2];

		// ポリゴンと線分の当たり判定
		ans = CheckHitLine(pos[0], pos[1], pos[2], pos0, pos1, &map->mesh.mtxWorld, distance);
		if (ans != 0)
		{
			break;
		}
	}
	return(ans);
}