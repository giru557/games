//=============================================================================
//
// 入力処理 [input.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "input.h"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECTINPUT8 CInput::m_pInput = NULL;

//*****************************************************************************
// 入力クラス ( 基本 )
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CInput::CInput()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CInput::~CInput()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	// ハンドル保存
	m_hWnd = hWnd;

	//DirectInputオブジェクトの生成
	if (m_pInput == NULL)
	{
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CInput::Uninit(void)
{
	// DirectInputオブジェクトの開放
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CInput::Update(void)
{

}


//*****************************************************************************
// キーボード入力クラス ( 継承元: 入力クラス )
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CInputKeyboard::CInputKeyboard()
{
	// キー情報のクリア
	memset(&m_aKeyState[0], 0, sizeof(m_aKeyState));
	memset(&m_aKeyStateTrigger[0], 0, sizeof(m_aKeyStateTrigger));
	memset(&m_aKeyStateRepeat[0], 0, sizeof(m_aKeyStateRepeat));
	memset(&m_aKeyStateRelease[0], 0, sizeof(m_aKeyStateRelease));
	memset(&m_aCountRepeat[0], 0, sizeof(m_aCountRepeat));
}

//=============================================================================
// デストラクタ
//=============================================================================
CInputKeyboard::~CInputKeyboard()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	//入力デバイス（キーボード）の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットの設定（キーボード）
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//協調モードの設定（キーボード）
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//アクセス権の取得
	m_pDevice->Acquire();
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CInputKeyboard::Uninit(void)
{
	// キーボードデバイスオブジェクトの開放
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	CInput::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[INPUT_KEYSTATE];

	//デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nKey = 0; nKey < INPUT_KEYSTATE; nKey++)
		{
			m_aKeyStateTrigger[nKey] = aKeyState[nKey] & (m_aKeyState[nKey] ^ aKeyState[nKey]);
			m_aKeyStateRelease[nKey] = (~aKeyState[nKey]) & m_aKeyState[nKey];
			m_aKeyState[nKey] = aKeyState[nKey];
		}
	}
	else
	{
		//アクセス権の取得
		m_pDevice->Acquire();
	}
}

//=============================================================================
// キーのプレス情報を取得
//=============================================================================
bool CInputKeyboard::GetPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}

//=============================================================================
// キーのトリガー情報を取得
//=============================================================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
// キーのリピート情報を取得
//=============================================================================
bool CInputKeyboard::GetRepeat(int nKey)
{
	if (m_aKeyStateTrigger[nKey] & 0x80)
	{
		return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
	}
	if ((m_aKeyState[nKey] & 0x80))
	{
		m_aCountRepeat[nKey]++;
		if (m_aCountRepeat[nKey] > 20)
		{
			while (m_aKeyState[nKey] & 0x80)
			{
				return (m_aKeyState[nKey] & 0x80) ? true : false;
			}
		}
	}
	if (m_aKeyStateRelease[nKey] & 0x80)
	{
		m_aCountRepeat[nKey] = 0;
	}
	return NULL;
}

//=============================================================================
// キーのリリース情報を取得
//=============================================================================
bool CInputKeyboard::GetRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}


//*****************************************************************************
// マウス入力クラス ( 継承元: 入力クラス )
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CInputMouse::CInputMouse()
{
	m_pDevice = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CInputMouse::~CInputMouse()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	//入力デバイス（キーボード）の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットの設定（キーボード）
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	//協調モードの設定（キーボード）
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//アクセス権の取得
	m_pDevice->Acquire();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CInputMouse::Uninit(void)
{
	// キーボードデバイスオブジェクトの開放
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	CInput::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CInputMouse::Update(void)
{
	// 更新前に現在の情報を保存しておく
	m_diPrevState = m_diMouseState;

	// 値更新
	if (FAILED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &m_diMouseState)))
	{
		//アクセス権の取得
		m_pDevice->Acquire();
	}
}

//=============================================================================
// マウスのプレス情報を取得
//=============================================================================
bool CInputMouse::GetPress(int nButton)
{
	return (m_diMouseState.rgbButtons[nButton] & 0x80) ? true : false;
}
//=============================================================================
// マウスのトリガー情報を取得
//=============================================================================
bool CInputMouse::GetTrigger(int nButton)
{
	if (!(m_diPrevState.rgbButtons[nButton] & 0x80) &&
		m_diMouseState.rgbButtons[nButton] & 0x80)
	{
		return true;
	}

	return false;
}

//=============================================================================
// マウスのリピート情報を取得
//=============================================================================
bool CInputMouse::GetRepeat(int nButton)
{
	if (GetTrigger(nButton))
	{
		return true;
	}
	if ((m_diMouseState.rgbButtons[nButton] & 0x80))
	{
		m_nRepeatCounter[nButton]++;
		if (m_nRepeatCounter[nButton] > 20)
		{
			while (m_diMouseState.rgbButtons[nButton] & 0x80)
			{
				return (m_diMouseState.rgbButtons[nButton] & 0x80) ? true : false;
			}
		}
	}
	if (GetRelease(nButton))
	{
		m_nRepeatCounter[nButton] = 0;
	}
	return NULL;
}
//=============================================================================
// マウスのリリース情報を取得
//=============================================================================
bool CInputMouse::GetRelease(int nButton)
{
	if (m_diPrevState.rgbButtons[nButton] & 0x80 &&
		!(m_diMouseState.rgbButtons[nButton] & 0x80))
	{
		return true;
	}
	
	return false;
}

//=============================================================================
// マウスのカーソル位置を取得
//=============================================================================
D3DXVECTOR2 CInputMouse::GetPos(void)
{
	POINT point;

	// マウス座標を取得
	GetCursorPos(&point);

	// スクリーン座標からクライアント座標（ウィンドウ座標）に変換
	ScreenToClient(m_hWnd, &point);

	return D3DXVECTOR2((float)point.x, (float)point.y);
}

//=============================================================================
// マウスの移動を取得
//=============================================================================
D3DXVECTOR2 CInputMouse::GetVelocity(void)
{
	return D3DXVECTOR2((float)m_diMouseState.lX, (float)m_diMouseState.lY);
}