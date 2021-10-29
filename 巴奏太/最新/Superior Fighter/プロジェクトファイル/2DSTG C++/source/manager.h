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
class CSound;
class CTitle;
class CGame;
class CResult;
class CFade;
class CCamera;
class CTextureLoad;
class CDebug;
class CPause;

//*****************************************************************************
// マネージャクラス
//*****************************************************************************
class CManager
{
public:
	// モード一覧
	typedef enum
	{
		MODE_TITLE = 0,
		MODE_GAME,
		MODE_RESULT,
		MODE_MAX
	} MODE;

	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// インスタンス取得関数
	static CRenderer *GetRenderer(void) { return m_pRenderer; }
	static CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }
	static CInputMouse *GetInputMouse(void) { return m_pInputMouse; }
	static CSound *GetSound(void) { return m_pSound; }
	static CTitle *GetTitle(void) { return m_pTitle; }
	static CGame *GetGame(void) { return m_pGame; }
	static CResult *GetResult(void) { return m_pResult; }
	static CFade *GetFade(void) { return m_pFade; }
	static CCamera *GetCamera(void) { return m_pCamera; }
	static CTextureLoad *GetTextureLoad(void) { return m_pTextureLoad; }
	static CPause *GetPause(void) { return m_pPause; }

#ifdef _DEBUG
	static CDebug *GetDebugger(void) { return m_pDebug; }
#endif

	// モード管理関数
	static MODE GetMode(void) { return m_mode; }
	static void SetMode(MODE mode);

	// 便利関数
	static D3DXVECTOR3 GetVectorFromPointAtoB(D3DXVECTOR3 pointA, D3DXVECTOR3 pointB);	// 座標AからBに向かうベクトルを取得
	static float GetDistanceAB(D3DXVECTOR3 posA, D3DXVECTOR3 posB);

private:
	static CRenderer *m_pRenderer;
	static CInputKeyboard *m_pInputKeyboard;
	static CInputMouse *m_pInputMouse;
	static CSound *m_pSound;
	static CTitle *m_pTitle;
	static CGame *m_pGame;
	static CResult *m_pResult;
	static CFade *m_pFade;
	static CCamera *m_pCamera;
	static CTextureLoad *m_pTextureLoad;
	static CPause *m_pPause;

#ifdef _DEBUG
	static CDebug *m_pDebug;
#endif

	static MODE m_mode;
};

#endif

