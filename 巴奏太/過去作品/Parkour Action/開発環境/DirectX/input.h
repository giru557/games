//-----------------------------------------------------------------------------
//
// ���͏����w�b�_�[ [input.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _INPUT_H_
#define _INPUT_H_
#include "main.h"

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
#define KEY_STATE (256)	// �L�[�{�[�h�̓��͏��

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRelease(int nKey);
bool GetKeyboardRepeat(int nKey);

#endif

