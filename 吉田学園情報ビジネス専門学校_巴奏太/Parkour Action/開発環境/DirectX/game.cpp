//-----------------------------------------------------------------------------
//
// ゲームの処理 [game.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "game.h"
#include "input.h"
#include "sound.h"
#include "goalflag.h"
#include "player.h"
#include "block.h"
#include "enemy.h"
#include "timescore.h"
#include "scroll.h"

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
bool g_bPause = false;
bool g_bStartFlag;
DWORD g_dwStartTime;
DWORD g_dwFinishTime;


//-----------------------------------------------------------------------------
// ゲームの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitGame(void)
{
	GOAL *pGoal;
	pGoal = GetGoal();

	//背景の初期化処理
	InitBG();

	//ブロックの初期化処理
	InitBlock();

	//ゴールの初期化処理
	InitGoal();
	
	//敵の初期化処理
	InitEnemy();

	//マップデータ読み込み
	LoadMapData(pGoal->stage);

	//クリアタイムの初期化処理
	InitTimeScore();

	//プレイヤーの初期化処理
	InitPlayer();

	//ポーズの初期化処理
	InitPause();

	switch (pGoal->stage)
	{
	case STAGE_1:
		PlaySound(SOUND_LABEL_BGM_STAGE1);
		break;
	case STAGE_2:
		PlaySound(SOUND_LABEL_BGM_STAGE2);
		break;
	default:
		break;
	}

	//ポーズ解除
	SetPause(false);

	return S_OK;
}

//-----------------------------------------------------------------------------
// ゲームの終了処理
//-----------------------------------------------------------------------------
void UninitGame(void)
{
	//クリアタイムの終了処理
	UninitTimeScore();

	//プレイヤーの終了処理
	UninitPlayer();

	//敵の終了処理
	UninitEnemy();

	//ゴールの終了処理
	UninitGoal();

	//ブロックの終了処理
	UninitBlock();

	// 背景の終了処理
	UninitBG();

	StopSound();
}

//-----------------------------------------------------------------------------
// ゲームの更新処理
//-----------------------------------------------------------------------------
void UpdateGame(void)
{
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	//ポーズ切り替え
	if (GetKeyboardTrigger(DIK_ESCAPE) == true)
	{
		if (g_bPause == false)
		{
			g_bPause = true;
			//ポーズ音再生
			PlaySound(SOUND_LABEL_SE_SELECT3);
		}
		else
		{
			g_bPause = false;
			//ポーズ解除音再生
			PlaySound(SOUND_LABLE_SE_SELECT4);
		}
		//g_bPause = g_bPause ? false : true; なんで使えん
	}

	if (g_bPause == true)
	{//ポーズ中
		//ポーズの更新処理
		UpdatePause();
	}
	else
	{//ポーズ中でない
		//スクロールの更新処理
		UpdateScroll();

		//背景の更新処理
		UpdateBG();

		//ブロックの更新処理
		UpdateBlock();

		//ゴールの更新処理
		UpdateGoal();

		//敵の更新処理
		UpdateEnemy();

		//プレイヤーの更新処理
		UpdatePlayer();

		//クリアタイムの更新処理
		UpdateTimeScore();

	}
}

//-----------------------------------------------------------------------------
// ゲームの描画処理
//-----------------------------------------------------------------------------
void DrawGame(void)
{
	//背景の描画処理
	DrawBG();

	//ブロックの描画処理
	DrawBlock();

	//ゴールの描画処理
	DrawGoal();

	//敵の描画処理
	DrawEnemy();

	//プレイヤーの描画処理
	DrawPlayer();

	//クリアタイムの描画処理
	DrawTimeScore();

	//ポーズメニューの描画
	if (g_bPause == true)
	{
		DrawPause();
	}
}

//-----------------------------------------------------------------------------
// ポーズセット
//-----------------------------------------------------------------------------
void SetPause(bool bPause)
{
	g_bPause = bPause;
}