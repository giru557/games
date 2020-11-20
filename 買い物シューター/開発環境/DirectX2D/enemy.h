//-----------------------------------------------------------------------------
//
// 敵ヘッダー [enemy.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "main.h"

//-----------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------
#define MAX_ENEMY (256) //敵の最大数
#define MAX_TYPE_ENEMY (3) //敵の種類数
#define ENEMY_BIG_WIDTH (32)
#define ENEMY_BIG_HEIGHT (32)
#define ENEMY_MEDIUM_WIDTH (32)
#define ENEMY_MEDIUM_HEIGHT (16)
#define ENEMY_SMALL_WIDTH (16)
#define ENEMY_SMALL_HEIGHT (16)
#define ENEMY_SIZE (30)
#define ENEMY_SPEED (0.5f)

//-----------------------------------------------------------------------------
//ステート管理
//-----------------------------------------------------------------------------
typedef enum
{
	ENEMYSTATE_NORMAL = 0,	//通常状態
	ENEMYSTATE_DAMAGE,		//ダメージ状態
	ENEMYSTATE_MAX
} ENEMYSTATE;

//-----------------------------------------------------------------------------
//敵種類
//-----------------------------------------------------------------------------
typedef enum
{
	ENEMYTYPE_BIG = 0,	//大きい敵
	ENEMYTYPE_MEDIUM,	//中くらいの敵
	ENEMYTYPE_SMALL,	//小さい敵
	ENEMYTYPE_MAX
} ENEMYTYPE;

//敵のグループ
typedef enum
{
	ENEMYGROUP_A = 0,	//敵のグループA
	ENEMYGROUP_B,		//グループB
	ENEMYGROUP_C,		//グループC
	ENEMYGROUP_D,		//グループD
	ENEMYGROUP_E,		//グループE
	ENEMYGROUP_F,		//グループF
	ENEMYGROUP_MAX
} ENEMYGROUP;

//-----------------------------------------------------------------------------
//構造体宣言
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動
	ENEMYTYPE nType;	//種類
	bool bUse;			//使用しているか
	ENEMYSTATE state;	//状態
	int nCounterState;	//状態カウンター
	int nLife;			//体力
	int nAtkInterval;	//攻撃インターバル
	ENEMYGROUP group;	//グループ
} ENEMY;

//-----------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, ENEMYTYPE nType, ENEMYGROUP group);
ENEMY *GetEnemy(void);
bool HitEnemy(int nIdx, int nDamage);

#endif