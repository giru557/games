//-----------------------------------------------------------------------------
//
// ゲームの処理 [game.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "game.h"
#include "fade.h"
#include "input.h"
#include "light.h"
#include "camera.h"
#include "model.h"
#include "player.h"
#include "polygon.h"
#include "meshfield.h"
#include "bullet.h"
#include "wall.h"
#include "billboard.h"
#include "shadow.h"
#include "motion.h"
#include "field.h"
#include "enemy.h"
#include "collision.h"
#include "healthbar.h"
#include "UI.h"

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// ゲームの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitGame(void)
{
	//ライトの初期化処理
	InitLight();

	//カメラの初期化処理
	InitCamera();

	//モデルの初期化処理
	InitModel();

	//モーションの初期化処理
	InitMotion();

	//プレイヤーの初期化処理
	InitPlayer();

	//敵の初期化処理
	InitEnemy();

	//ポリゴンの初期化処理
	//InitPolygon();

	//地面の初期化処理
	//InitMeshfield();

	//test
	InitField();

	//弾の初期化処理
	//InitBullet();

	//壁の初期化処理
	InitWall();

	//ビルボードの初期化処理
	//InitBillboard();

	//HPバーの初期化処理
	InitHP();

	//影の初期化処理
	InitShadow();

	//コリジョンの初期化
	InitCollision();

	//UIの初期化
	InitUI();

	//カーソルを非表示
	ShowCursor(FALSE);

	return S_OK;
}

//-----------------------------------------------------------------------------
// ゲームの終了処理
//-----------------------------------------------------------------------------
void UninitGame(void)
{
	//UIの終了処理
	UninitUI();

	//コリジョンの終了処理
	InitCollision();

	//影の終了処理
	UninitShadow();

	//HPバーの終了処理
	UninitHP();

	//ビルボードの終了処理
	//UninitBillboard();

	//壁の終了処理
	UninitWall();

	//弾の終了処理
	UninitBullet();

	//test
	//UninitField();

	//地面の終了処理
	//UninitMeshfield();

	//ポリゴンの終了処理
	//UninitPolygon();

	//敵の終了処理
	UninitEnemy();

	//プレイヤーの終了処理
	UninitPlayer();

	//モデルの終了処理
	UninitModel();

	//カメラの終了処理
	UninitCamera();

	//ライトの終了処理
	UninitLight();

	//カーソル表示
	ShowCursor(TRUE);
}

//-----------------------------------------------------------------------------
// ゲームの更新処理
//-----------------------------------------------------------------------------
void UpdateGame(void)
{
	//ライトの更新処理
	UpdateLight();

	//カメラの更新処理
	UpdateCamera();

	//モデルの更新処理
	UpdateModel();

	//モーションの更新処理
	UpdateMotion();

	//プレイヤーの更新処理
	UpdatePlayer();

	//敵の更新処理
	UpdateEnemy();

	//ポリゴンの更新処理
	//UpdatePolygon();

	//地面の更新処理
	//UpdateMeshfield();

	//test
	UpdateField();

	//弾の更新処理
	//UpdateBullet();

	//壁の更新処理
	UpdateWall();

	//ビルボードの更新処理
	//UpdateBillboard();

	//HPバーの更新処理
	UpdateHP();

	//影の更新処理
	UpdateShadow();

	//コリジョンの更新処理
	UpdateCollision();

	//UIの更新処理
	UpdateUI();

	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		SetFade(FADE_OUT, MODE_RESULT);
	}
}

//-----------------------------------------------------------------------------
// ゲームの描画処理
//-----------------------------------------------------------------------------
void DrawGame(void)
{
	//カメラの設定
	SetCamera();

	//モデルの描画処理
	DrawModel();

	//プレイヤーの描画処理
	DrawPlayer();

	//敵の描画処理
	DrawEnemy();

	//ポリゴンの描画処理
	//DrawPolygon();

	//地面の描画処理
	//DrawMeshfield();

	//test
	DrawField();

	//弾の描画処理
	//DrawBullet();

	//壁の描画処理
	DrawWall();

	//ビルボードの描画処理
	//DrawBillboard();

	//HPバーの描画処理
	DrawHP();

	//影の描画処理
	DrawShadow();

	//コリジョンの描画処理
	DrawCollision();

	//UIの描画処理
	DrawUI();
}

//-----------------------------------------------------------------------------
// ポーズセット
//-----------------------------------------------------------------------------
void SetPause(bool bPause)
{

}