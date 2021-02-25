//-----------------------------------------------------------------------------
//
// フェードヘッダー [fade.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _FADE_H_
#define _FADE_H_
#include "main.h"

//フェードの状態
typedef enum
{
	FADE_NONE = 0,
	FADE_IN,
	FADE_OUT,
	FADE_MAX
} FADE;

//プロトタイプ宣言
HRESULT InitFade(MODE modeNext);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);
void SetFade(FADE fade, MODE modeNext);

#endif