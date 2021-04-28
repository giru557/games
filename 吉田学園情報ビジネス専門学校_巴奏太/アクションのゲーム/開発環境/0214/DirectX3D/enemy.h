//-----------------------------------------------------------------------------
//
// 敵の処理 ヘッダー [enemy.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "player.h"
#include <time.h>

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define ENEMY_AMOUNT_MAX (32)		// 用意している敵の最大数
#define ENEMY_KNOCKBACK (10)		// ノックバック量
#define ENEMY_INVINCIBLE_TIME (500)	// 攻撃された時の無敵時間
#define ENEMY_HEIGHT (20)			// 敵の高さ
#define ENEMY_WIDTH (20)			// 敵の幅
#define ENEMY_FALLSPEED (0.4f)		// 落下速度
#define ENEMY_DETECT_RADIUS (200)	// 敵感知距離
#define ENEMY_MOVESPEED (0.2f)		// 移動速度
#define ENEMY_ATTACK_RADIUS (35)	// 攻撃範囲
#define ENEMY_ATTACK_COOLTIME (1000) // 攻撃CT
#define ENEMY_HP_MAX (3)			// 敵の体力

//-----------------------------------------------------------------------------
// 敵の構造体
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;				// 位置
	D3DXVECTOR3 posOld;				// 前回の位置
	D3DXVECTOR3 move;				// 移動
	D3DXVECTOR3 rot;				// 向き
	D3DXVECTOR3 rotDest;			// 目標の向き
	D3DXMATRIX mtxWorld;			// ワールドマトリックス
	LPD3DXMESH pMesh;				// メッシュへのポインタ
	LPD3DXBUFFER pBuffMat;			// マテリアルへのポインタ
	DWORD nNumMat;					// マテリアルの数
	LPDIRECT3DTEXTURE9 pTexture[10];// テクスチャへのポインタ

	int nLifeMax;					// 最大体力
	int nLife;						// 体力
	int nIdx;						// 自分のID
	bool bUse;						// 使用中か
	bool bHit;						// 攻撃判定中か
	bool bInvincible;				// 無敵状態
	bool bAttack;					// 攻撃した
	DWORD dwTimeInv;				// 無敵時間計算用
	DWORD dwTimeAtk;				// 攻撃時間計算用
} Enemy;

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
Enemy *GetEnemy(void);
void SetEnemy(D3DXVECTOR3 pos);

#endif

