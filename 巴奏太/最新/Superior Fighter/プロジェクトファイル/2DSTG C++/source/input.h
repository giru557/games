//=============================================================================
//
// 入力処理 [input.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define INPUT_KEYSTATE (256)	// 入力情報

#define INPUT_L_CLICK (0)	// 左クリック
#define INPUT_R_CLICK (1)	// 右クリック
#define INPUT_M_CLICK (2)	// ホイールクリック
#define INPUT_MAX_CLICK (3)	// 最大ボタン数

//*****************************************************************************
// 入力クラス ( 基本 )
//*****************************************************************************
class CInput
{
public:
	CInput();
	virtual ~CInput();
	
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

protected:
	static LPDIRECTINPUT8 m_pInput;		// インプットのオブジェクト
	HWND m_hWnd;						// ウィンドウのハンドル保存用
};

//*****************************************************************************
// キーボード入力クラス ( 継承元: 入力クラス )
//*****************************************************************************
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRepeat(int nKey);
	bool GetRelease(int nKey);

private:
	LPDIRECTINPUTDEVICE8 m_pDevice;	// キーボード用デバイス

	// キー状態
	BYTE m_aKeyState[INPUT_KEYSTATE];
	BYTE m_aKeyStateTrigger[INPUT_KEYSTATE];
	BYTE m_aKeyStateRepeat[INPUT_KEYSTATE];
	BYTE m_aKeyStateRelease[INPUT_KEYSTATE];
	BYTE m_aCountRepeat[INPUT_KEYSTATE];
};

//*****************************************************************************
// マウス入力クラス ( 継承元: 入力クラス )
//*****************************************************************************
class CInputMouse : public CInput
{
public:
	typedef enum
	{
		MOUSESTATE_LCLICK = 0,
		MOUSESTATE_RCLICK,
		MOUSESTATE_WHEELCLICK,
		MOUSESTATE_MAX
	} MOUSESTATE;

	CInputMouse();
	~CInputMouse();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetPress(int nButton);
	bool GetTrigger(int nButton);
	bool GetRepeat(int nButton);
	bool GetRelease(int nButton);
	D3DXVECTOR2 GetPos(void);
	D3DXVECTOR2 GetVelocity(void);

private:
	LPDIRECTINPUTDEVICE8 m_pDevice;	// マウス用デバイス

	DIMOUSESTATE2 m_diMouseState;	// 現在の状態
	DIMOUSESTATE2 m_diPrevState;	// 前回の状態
	int m_nRepeatCounter[INPUT_MAX_CLICK];			// リピート判定のカウンタ
};
#endif