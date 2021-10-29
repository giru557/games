//-----------------------------------------------------------------------------
//
// ゴール ヘッダー [goalflag.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _GOAL_H_
#define _GOAL_H_
#include "main.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// GOAL構造体
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 scroll;		// スクロール
	float fWidth;			// 幅
	float fHeight;			// 高さ
	bool bUse;				// 使用中フラグ
	bool bGoal;				// ゴールしたかどうか
	STAGE stage;
} GOAL;

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
HRESULT InitGoal(void);
void UninitGoal(void);
void UpdateGoal(void);
void DrawGoal(void);
void CollisionGoal(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight);
void SetFlag(D3DXVECTOR3 pos, float fWidth, float fHeight);
GOAL *GetGoal(void);

#endif

