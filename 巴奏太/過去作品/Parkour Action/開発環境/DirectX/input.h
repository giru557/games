//-----------------------------------------------------------------------------
//
// 入力処理ヘッダー [input.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _INPUT_H_
#define _INPUT_H_
#include "main.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define KEY_STATE (256)	// キーボードの入力情報

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRelease(int nKey);
bool GetKeyboardRepeat(int nKey);

#endif

