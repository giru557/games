//-----------------------------------------------------------------------------
//
// ゲームの処理 [game.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "main.h"
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "enemy.h"
#include "sound.h"
#include "game.h"
#include "effect.h"
#include "barrier.h"
#include "pause.h"
#include "shop.h"
#include "money.h"
#include "missile.h"
#include "BG.h"
#include "minigun.h"
#include "health.h"
#include "coin.h"
#include "complete.h"

//-----------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------
bool g_bPause = false;

//-----------------------------------------------------------------------------
//ゲームの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitGame(void)
{
	//背景の初期化処理
	InitBG();

	//ポリゴンの初期化処理
	InitPlayer();

	//敵の初期化処理
	InitEnemy();

	//バリアの初期化
	InitBarrier();

	//爆発の初期化
	InitExplosion();

	//エフェクトの初期化
	InitEffect();

	//弾の初期化処理
	InitBullet();
	InitMinigun();
	InitMissile();

	//スコアの初期化処理
	InitScore();

	//ショップの初期化処理
	InitShop();

	//お金の初期化処理
	InitMoney();

	//体力の初期化処理
	InitHealth();

	//コインの初期化処理
	InitCoin();

	//ステージコンプの初期化処理
	InitComplete();

	//ポーズの初期化処理
	InitPause();

	//サウンド再生
	PlaySound(SOUND_LABEL_BGM003);

	//ポーズ解除
	SetPause(false);

	return S_OK;
}

//-----------------------------------------------------------------------------
//ゲームの終了処理
//-----------------------------------------------------------------------------
void UninitGame(void)
{
	//ポーズの終了処理
	UninitPause();

	//ステージコンプの終了処理
	UninitComplete();
	
	//コインの終了処理
	UninitCoin();

	//体力の終了処理
	UninitHealth();
	
	//お金の終了処理
	UninitMoney();

	//ショップの初期化処理
	UninitShop();

	//スコアの終了処理
	UninitScore();

	//弾の終了処理
	UninitMissile();
	UninitMinigun();
	UninitBullet();

	//エフェクトの終了処理
	UninitEffect();

	//爆発の終了処理
	UninitExplosion();

	//ポリゴンの終了処理
	UninitPlayer();

	//バリアの終了処理
	UninitBarrier();

	//敵の終了処理
	UninitEnemy();

	//背景の終了処理
	UninitBG();

	//サウンドの終了
	StopSound(SOUND_LABEL_BGM003);
}

//-----------------------------------------------------------------------------
//ゲームの更新処理
//-----------------------------------------------------------------------------
void UpdateGame(void)
{
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
		//g_bPause = g_bPause ? false : true;
	}

	if (g_bPause == true)
	{//ポーズ中
		//ポーズの更新処理
		UpdatePause();
	}
	else
	{//ポーズ中でない
		//背景の更新処理
		UpdateBG();

		//ポリゴンの更新処理
		UpdatePlayer();

		//敵の更新処理
		UpdateEnemy();

		//バリアの更新処理
		UpdateBarrier();

		//爆発の更新処理
		UpdateExplosion();

		//エフェクトの更新処理
		UpdateEffect();

		//弾の更新処理
		UpdateBullet();
		UpdateMinigun();
		UpdateMissile();

		//スコアの更新処理
		UpdateScore();

		//ショップの更新処理
		UpdateShop();

		//お金の更新処理
		UpdateMoney();

		//体力の更新処理
		UpdateHealth();

		//コインの更新処理
		UpdateCoin();

		//ステージコンプの更新処理
		UpdateComplete();

	}
}

//-----------------------------------------------------------------------------
//ゲームの描画処理
//-----------------------------------------------------------------------------
void DrawGame(void)
{
	//BGの描画処理
	DrawBG();

	//ポリゴンの描画処理
	DrawPlayer();

	//敵の描画処理
	DrawEnemy();

	//バリアの描画処理
	DrawBarrier();

	//爆発の描画処理
	DrawExplosion();

	//エフェクトの描画処理
	DrawEffect();

	//弾の描画処理
	DrawBullet();
	DrawMinigun();
	DrawMissile();

	//スコアの描画処理
	DrawScore();

	//ショップの描画処理
	DrawShop();

	//お金の描画処理
	DrawMoney();

	//体力の描画処理
	DrawHealth();

	//コインの描画処理
	DrawCoin();

	//ステージコンプの描画処理
	DrawComplete();

	//ポーズメニューの描画
	if (g_bPause == true)
	{
		DrawPause();
	}
}

//
//ポーズセット
//
void SetPause(bool bPause)
{
	g_bPause = bPause;
}