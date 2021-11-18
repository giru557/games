//-----------------------------------------------------------------------------
//
// ミサイル ヘッダー [missile.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _MISSILE_H_
#define _MISSILE_H_
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
	MISSILESTATE_OWNER_PLAYER = 0,	//プレイヤーが弾を撃った状態
	MISSILESTATE_OWNER_ENEMY,		//敵が弾を撃った状態
	MISSILESTATE_MAX
} MISSILESTATE;

//-----------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------
#define MAX_MISSILE (256)	//弾の最大数

//-----------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------
HRESULT InitMissile(void);
void UninitMissile(void);
void UpdateMissile(void);
void DrawMissile(void);
void SetMissile(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, MISSILESTATE state);

#endif

