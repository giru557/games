//-----------------------------------------------------------------------------
//
// スコアの処理 ヘッダー [score.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _SCORE_H_
#define _SCORE_H_
#include "main.h"

//-----------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------
HRESULT InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void AddScore(int nValue);
int GetScore(void);

#endif