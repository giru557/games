//-----------------------------------------------------------------------------
//
// プレイヤー ヘッダー [player.h]
// Author: Souta Tomoe
//-----------------------------------------------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "main.h"
#include "block.h"


//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define PLAYER_POS_X (SCREEN_WIDTH / 2)					// プレイヤーの座標 X
#define PLAYER_POS_Y (200)					// プレイヤーの座標 Y
#define PLAYER_WIDTH ((495/ 2) * 0.15f)		// プレイヤーの幅
#define PLAYER_HEIGHT (670 * 0.15f)			// プレイヤーの高さ
#define PLAYER_WIDTH_SLIDING ((495/2) * 0.175f) // プレイヤーの幅(スライディング中)
#define PLAYER_HEIGHT_SLIDING (470 * 0.175f)	  // プレイヤーの高さ(スライディング中)
#define PLAYER_SPEED (2.0f)					// プレイヤーの移動速度
#define PLAYER_SPEED_DECAY (0.25f);			// プレイヤーの移動速度減衰	
#define PLAYER_GRAVITY (1.2f)				// プレイヤーの重力
#define PLAYER_TEX_SPLIT_V (1)				// プレイヤーのテクスチャの分割数 (縦)
#define PLAYER_TEX_SPLIT_H (12)				// プレイヤーのテクスチャの分割数 (横)
#define PLAYER_TEX_SPLIT_SLIDE_V (1)		// スライディング中の分割数
#define PLAYER_TEX_SPLIT_SLIDE_H (6)		// スライディング中の分割数
#define PLAYER_TEX_ANIMINTERVAL (5)		// テクスチャのアニメーションの更新頻度

//-----------------------------------------------------------------------------
// スクロールステート
//-----------------------------------------------------------------------------
typedef enum
{
	PLAYERSCROLL_NORMAL = 0,	// 通常時
	PLAYERSCROLL_CENTER,		// プレイヤーが中央にいる場合
	PLAYERSCROLL_MAX
} PLAYERSCROLL;

//-----------------------------------------------------------------------------
// プレイヤーの状態
//-----------------------------------------------------------------------------
typedef enum
{
	PLAYERSTATE_NORMAL = 0,		// 通常時
	PLAYERSTATE_ONWALL,			// 壁張り付き時
	PLAYERSTATE_SLIDE,			// スライディング時
	PLAYERSTATE_LAND,			// 着地時
	PLAYERSTATE_MAX
} PLAYERSTATE;

//-----------------------------------------------------------------------------
// プレイヤーのダメージ状態
//-----------------------------------------------------------------------------
typedef enum
{
	DAMAGESTATE_NORMAL = 0,		// 通常時
	DAMAGESTATE_HURT,			// ダメージ時
	DAMAGESTATE_INV,			// 無敵
	DAMAGESTATE_DIE,			// 死亡
	DAMAGESTATE_MAX
} DAMAGESTATE;

//-----------------------------------------------------------------------------
// 構造体宣言
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 posold; // 前回の位置
	D3DXVECTOR3 move;	// 移動
	float fWidth;		// 幅	
	float fHeight;		// 高さ
	float fSpeed;		// 移動速度
	int nMaxPatternV;	// アニメーション分割数(縦)
	int nMaxPatternH;	// アニメーション分割数(横)
	int nPatternV;		// アニメーションパターン (縦)
	int nPatternH;		// アニメーションパターン (横)
	int nCounterAnim;	// アニメーションカウンター
	int nLastKey;		// 最後に押したキー (1でA 0でD)
	int nWaitSlide;		// スライドクールダウン
	int nWait;			// 待機時間
	int nLife;			// 体力
	bool bUse;			// 使用中か
	bool bJump;			// ジャンプ中か
	bool bOnWall;		// 壁に張り付いている
	PLAYERSCROLL scrollState; // スクロールの状態
	PLAYERSTATE state;		  // プレイヤーの状態
	DAMAGESTATE damage;		  // ダメージ状態
} PLAYER;

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(void);

#endif