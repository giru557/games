//-----------------------------------------------------------------------------
//
// タイトルヘッダー [title.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _TITLE_H_
#define _TITLE_H_
#include "main.h"

//ステート
typedef enum
{
	TITLESTATE_NORMAL = 0,
	TITLESTATE_BLINK,
	TITLESTATE_SHRINK,
	TITLESTATE_MAX
} TITLESTATE;

//TITLE構造体
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	float fBlink;	//点滅
	float fShrink;	//大きくなったり小さくなったり
} TITLE;

//-----------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------
HRESULT InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);

#endif