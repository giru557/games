//=============================================================================
//
// マネージャ [manager.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

//*****************************************************************************
// ヘッダのインクルード
//*****************************************************************************
#include "main.h"
#include "renderer.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CInputKeyboard;
class CInputMouse;
class CGame;
class CCamera;
class CLight;
class CTextureLoad;

//*****************************************************************************
// マネージャクラス
//*****************************************************************************
class CManager
{
public:
	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// GETTER,SETTER
	static CRenderer *GetRenderer(void) { return m_pRenderer; }
	static CCamera *GetCamera(void) { return m_pCamera; }
	static CLight *GetLight(void) { return m_pLight; }
	static CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }
	static CInputMouse *GetInputMouse(void) { return m_pInputMouse; }
	static CGame *GetGame(void) { return m_pGame; }
	static CTextureLoad *GetTexture(void) { return m_pTextureLoader; }

private:
	static CRenderer *m_pRenderer;
	static CCamera *m_pCamera;
	static CLight *m_pLight;
	static CInputKeyboard *m_pInputKeyboard;
	static CInputMouse *m_pInputMouse;
	static CGame *m_pGame;
	static CTextureLoad *m_pTextureLoader;
};

#endif

