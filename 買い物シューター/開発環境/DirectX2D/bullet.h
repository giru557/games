//-----------------------------------------------------------------------------
//
// 弾ヘッダー [bullet.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _BULLET_H_
#define _BULLET_H_
#include "main.h"
#include "player.h"
#include "enemy.h"
#include "explosion.h"
#include "score.h"
#include "sound.h"
#include "effect.h"
#include "barrier.h"

//弾のステート管理
typedef enum
{
	BULLETSTATE_OWNER_PLAYER = 0,	//プレイヤーが弾を撃った状態
	BULLETSTATE_OWNER_ENEMY,		//敵が弾を撃った状態
	BULLETSTATE_MAX
} BULLETSTATE;

//弾の種類
typedef enum
{
	BULLETTYPE_NORMAL = 0,
	BULLETTYPE_RAPIDFIRE,
	BULLETTYPE_RAPIDFIRE1,
	BULLETTYPE_RAPIDFIRE2,
	BULLETTYPE_MAX
} BULLETTYPE;

//-----------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------
#define MAX_BULLET (256)	//弾の最大数

//-----------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, BULLETSTATE state, BULLETTYPE type);

#endif
