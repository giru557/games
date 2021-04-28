//-----------------------------------------------------------------------------
//
// プレイヤー ヘッダー [player.h]
// Author: Souta Tomoe
//-----------------------------------------------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "main.h"


//-----------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------
#define PLAYER_POS_X (640) //プレイヤーの位置 X
#define PLAYER_POS_Y (400) //プレイヤーの位置 Y
#define PLAYER_WIDTH (28)  //プレイヤーの幅
#define PLAYER_HEIGHT (48) //プレイヤーの高さ
#define PLAYER_SPEED ((float)1.5)   //プレイヤーの移動の速さ

#define TEXTURE_SPLIT_VERTICAL (2)
#define TEXTURE_SPLIT_HORIZONTAL (5)

//ステート管理
typedef enum
{
	PLAYERSTATE_APPEAR = 0,
	PLAYERSTATE_NORMAL,  	//通常状態
	PLAYERSTATE_DAMAGE,		//ダメージ状態
	PLAYERSTATE_WAIT,
	PLAYERSTATE_DEATH,
	PLAYERSTATE_MAX
} PLAYERSTATE;

//武器選択
typedef enum
{
	PLAYERWEAPON_NORMAL = 0,	//通常武器
	PLAYERWEAPON_MINIGUN,		//ガトリングガン
	PLAYERWEAPON_MISSILE,		//ミサイル
	PLAYERWEAPON_BARRIER,		//バリア
	PLAYERWEAPON_MAX
} PLAYERWEAPON;

//構造体宣言
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	PLAYERSTATE state;	//状態
	int nCounterState;	//状態カウンター
	int nLife;			//体力
	bool bUse;			//使用中か
} PLAYER;

//-----------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
bool HitPlayer(int nDamage);
PLAYER *GetPlayer(void);
void SetWeapon(int nWeaponValue);

#endif