//-----------------------------------------------------------------------------
//
// ランキングスコア ヘッダー[rankedscore.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _RANKEDSCORE_H_
#define _RANKEDSCORE_H_
#include "main.h"

//-----------------------------------------------------------------------------
// RSCORE構造体
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	//位置
} RSCORE;

typedef struct
{
	int score;
	int hour;
	int minute;
	int second;
} SCORE;

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
HRESULT InitRScore(void);
void UninitRScore(void);
void UpdateRScore(void);
void DrawRScore(void);

#endif

