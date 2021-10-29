//-----------------------------------------------------------------------------
//
// 敵 ヘッダー [enemy.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "main.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define ENEMY_POLYGON (64)		// オブジェクト使用数
#define ENEMY_WIDTH ((500 / 2) * 0.1f)	// 敵の幅
#define ENEMY_HEIGHT (670 * 0.1f)		// 敵の高さ

#define ENEMY_TEX_SPLIT_V (1)
#define ENEMY_TEX_SPLIT_H (12)
#define ENEMY_TEX_ANIMINTERVAL (5)


//-----------------------------------------------------------------------------
// ENEMY構造体
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 posold;		// 前回位置
	D3DXVECTOR3 move;		// 移動値
	D3DXVECTOR3 scroll;		// スクロール
	float fWidth;			// 幅
	float fHeight;			// 高さ
	int nPatternH;			// アニメーションパターン
	int nPatternV;			// アニメーションパターン
	int nMaxPatternH;		// アニメーションパターン最大数
	int nMaxPatternV;		// アニメーションパターン最大数
	int nCounterAnim;		// アニメーションカウンタ
	bool bUse;				// 使用中フラグ
	bool bJump;				// 空中にいるかどうか
} ENEMY;

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight);
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move);
ENEMY *GetEnemy(void);

#endif

