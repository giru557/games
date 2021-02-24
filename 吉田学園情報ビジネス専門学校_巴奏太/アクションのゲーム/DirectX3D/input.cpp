//-----------------------------------------------------------------------------
//
// 入力の処理 [input.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "input.h"

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
LPDIRECTINPUT8 g_pInput = NULL;													//DirectInputオブジェクトのポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;										//入力デバイス（キーボード）へのポインタ
LPDIRECTINPUTDEVICE8 g_pDevMouse = NULL;										//マウスのデバイスへのポインタ
DIMOUSESTATE g_MouseState;														//マウスの状態
BYTE g_aKeyState[KEY_STATE];													//入力されたキーの判定
BYTE g_aKeyStateTrigger[KEY_STATE];												//入力されたキーの判定(トリガー)
BYTE g_aKeyStateRelease[KEY_STATE];												//入力されたキーの判定(リリース)
BYTE g_aKeyStateRepeat[KEY_STATE];												//入力されたキーの判定(リピート)
BYTE g_aCountRepeat[256];														//リピート判定で必要なカウンター


//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	//入力デバイス（キーボード）の生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//マウスの入力デバイス生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysMouse, &g_pDevMouse, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットの設定（キーボード）
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//データフォーマットの設定（マウス）
	if (FAILED(g_pDevMouse->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	//協調モードの設定（キーボード）
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//協調モードの設定（マウス）
	if (FAILED(g_pDevMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
	{
		return E_FAIL;
	}

	//アクセス権の取得
	g_pDevKeyboard->Acquire();
	g_pDevMouse->Acquire();

	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void UninitKeyboard(void)
{
	if (g_pDevKeyboard != NULL)
	{
		//アクセス権の破棄
		g_pDevKeyboard->Unacquire();

		//開放
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void UpdateKeyboard(void)
{
	BYTE aKeyState[KEY_STATE];

	//デバイスからデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nKey = 0; nKey < KEY_STATE; nKey++)
		{
			g_aKeyStateTrigger[nKey] = aKeyState[nKey] & (g_aKeyState[nKey] ^ aKeyState[nKey]);
			g_aKeyStateRelease[nKey] = (~aKeyState[nKey]) & g_aKeyState[nKey];
			g_aKeyState[nKey] = aKeyState[nKey];
		}
	}
	else
	{
		//アクセス権の取得
		g_pDevKeyboard->Acquire();
	}

	//データ取得
	if (SUCCEEDED(g_pDevMouse->GetDeviceState(sizeof(DIMOUSESTATE), &g_MouseState)))
	{
	}
	else
	{
		g_pDevMouse->Acquire();
	}
}

//-----------------------------------------------------------------------------
// キーボード情報の取得
//-----------------------------------------------------------------------------
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//-----------------------------------------------------------------------------
// キーボード情報の取得 (トリガー)
//-----------------------------------------------------------------------------
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//-----------------------------------------------------------------------------
// キーボード情報の取得 (リリース)
//-----------------------------------------------------------------------------
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//-----------------------------------------------------------------------------
// キーボード情報の取得 (リピート)
//-----------------------------------------------------------------------------
bool GetKeyboardRepeat(int nKey)
{
	if (g_aKeyStateTrigger[nKey] & 0x80)
	{
		return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
	}
	if ((g_aKeyState[nKey] & 0x80))
	{
		g_aCountRepeat[nKey]++;
		if (g_aCountRepeat[nKey] > 20)
		{
			while (g_aKeyState[nKey] & 0x80)
			{
				return (g_aKeyState[nKey] & 0x80) ? true : false;
			}
		}
	}
	if (g_aKeyStateRelease[nKey] & 0x80)
	{
		g_aCountRepeat[nKey] = 0;
	}
	return NULL;
}

//-----------------------------------------------------------------------------
// マウスの移動量を取得
//-----------------------------------------------------------------------------
D3DXVECTOR2 GetMouseVelocity(void)
{
	return D3DXVECTOR2((float)g_MouseState.lX, (float)g_MouseState.lY);
}