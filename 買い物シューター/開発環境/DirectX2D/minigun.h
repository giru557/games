//-----------------------------------------------------------------------------
//
// ミニガンヘッダー [minigun.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _MINIGUN_H_
#define _MINIGUN_H_
#include "main.h"
#include "player.h"
#include "enemy.h"
#include "explosion.h"
#include "score.h"
#include "sound.h"
#include "effect.h"

//弾のステート管理
typedef enum
{
	MINIGUNSTATE_OWNER_PLAYER = 0,	//プレイヤーが弾を撃った状態
	MINIGUNSTATE_OWNER_ENEMY,		//敵が弾を撃った状態
	MINIGUNSTATE_MAX
} MINIGUNSTATE;

//弾の種類
typedef enum
{
	MINIGUNTYPE_LEFT = 0,
	MINIGUNTYPE_RIGHT,
	MINIGUNTYPE_MAX
} MINIGUNTYPE;

//-----------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------
#define MAX_MINIGUN (256)	//弾の最大数

//-----------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------
HRESULT InitMinigun(void);
void UninitMinigun(void);
void UpdateMinigun(void);
void DrawMinigun(void);
void SetMinigun(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, MINIGUNSTATE state);

#endif
