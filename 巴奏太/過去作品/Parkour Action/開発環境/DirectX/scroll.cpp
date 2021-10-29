//-----------------------------------------------------------------------------
//
// ゲームの処理 [Scroll.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "scroll.h"
#include "player.h"
#include "enemy.h"
#include "block.h"
#include "goalflag.h"

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
D3DXVECTOR3 g_scrollAll;	//すべてのスクロールをまとめる
D3DXVECTOR3 g_scrollMove;	//スクロールの移動値

//-----------------------------------------------------------------------------
// スクロールの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitScroll(void)
{

	return S_OK;
}

//-----------------------------------------------------------------------------
// スクロールの終了処理
//-----------------------------------------------------------------------------
void UninitScroll(void)
{
}

//-----------------------------------------------------------------------------
// スクロールの更新処理
//-----------------------------------------------------------------------------
void UpdateScroll(void)
{
	PLAYER *pPlayer;
	ENEMY *pEnemy;
	BLOCK *pBlock;
	GOAL *pGoal;
	pGoal = GetGoal();
	pBlock = GetBlock();
	pEnemy = GetEnemy();
	pPlayer = GetPlayer();

	//スクロール移動値代入
	g_scrollAll.x = -pPlayer->move.x + g_scrollMove.x;
	g_scrollMove.x = 0;

	//スクロール
	if (pPlayer->scrollState == PLAYERSCROLL_CENTER)
	{
		for (int nCnt = 0; nCnt < ENEMY_POLYGON; nCnt++, pEnemy++)
		{
			if (pEnemy->bUse == true) pEnemy->pos.x += g_scrollAll.x;
		}
		for (int nCnt = 0; nCnt < BLOCK_MAX; nCnt++, pBlock++)
		{
			if (pBlock->bUse == true) pBlock->pos.x += g_scrollAll.x;
		}
		pGoal->pos.x += g_scrollAll.x;
	}

	//プレイヤー場外
	//if (pPlayer->pos.y > SCREEN_HEIGHT + pPlayer->fHeight)
	//{
	//	for (int nCnt = 0; nCnt < ENEMY_POLYGON; nCnt++, pEnemy++)
	//	{
	//		if (pEnemy->bUse == true) pEnemy->pos.x += 200.0f;
	//	}
	//	for (int nCnt = 0; nCnt < BLOCK_MAX; nCnt++, pBlock++)
	//	{
	//		if (pBlock->bUse == true) pBlock->pos.x += 200.0f;
	//	}
	//	if (pGoal->bUse == true) pGoal->pos.x += 200.0f;
	//}

	//pEnemy = GetEnemy();
	//pGoal = GetGoal();

	//if (pPlayer->pos.y > SCREEN_HEIGHT + pPlayer->fHeight)
	//{
	//	pPlayer->pos.y = 0;
	//	for (int nCnt = 0; nCnt < BLOCK_MAX; nCnt++, pBlock++)
	//	{
	//		if (pBlock->bUse == true)
	//		{
	//			pBlock->pos.x += 200;
	//		}
	//	}
	//	for (int nCnt = 0; nCnt < ENEMY_POLYGON; nCnt++, pEnemy++)
	//	{
	//		if (pEnemy->bUse == true)
	//		{
	//			pEnemy->pos.x += 200; 
	//		}
	//	}
	//	if (pGoal->bUse == true)
	//	{
	//		pGoal->pos.x += 200;
	//	}
	//}
}

//-----------------------------------------------------------------------------
// スクロールの描画処理
//-----------------------------------------------------------------------------
void DrawScroll(void)
{
}

D3DXVECTOR3 *GetScroll(void)
{
	return &g_scrollMove;
}