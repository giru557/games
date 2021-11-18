//=============================================================================
//
// マネージャ [manager.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "manager.h"
#include "camera.h"
#include "light.h"
#include "input.h"
#include "gamescene.h"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CRenderer *CManager::m_pRenderer = NULL;
CCamera *CManager::m_pCamera = NULL;
CLight *CManager::m_pLight = NULL;
CInputKeyboard *CManager::m_pInputKeyboard = NULL;
CInputMouse *CManager::m_pInputMouse = NULL;
CGame *CManager::m_pGame = NULL;
CTextureLoad *CManager::m_pTextureLoader = NULL;

//*****************************************************************************
// マネージャクラス
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CManager::CManager()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// レンダラの生成、初期化
	m_pRenderer = new CRenderer;
	if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
	{
		return E_FAIL;
	}

	// カメラの生成、初期化
	m_pCamera = new CCamera;
	if (m_pCamera != NULL)
	{
		m_pCamera->Init();
	}

	// ライトの生成、初期化
	m_pLight = new CLight;
	if (m_pLight != NULL)
	{
		m_pLight->Init();
	}

	// キーボードクラスの生成、初期化
	m_pInputKeyboard = new CInputKeyboard;
	if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// マウスクラスの生成、初期化
	m_pInputMouse = new CInputMouse;
	if (FAILED(m_pInputMouse->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// ゲームシーン生成、初期化
	m_pGame = new CGame;
	if (m_pGame != NULL)
	{
		m_pGame->Init(VECTOR3_ZERO, VECTOR3_ZERO, VECTOR3_ZERO);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CManager::Uninit(void)
{
	// オブジェクトをすべて破棄
	CScene::ReleaseAll();

	// マウスの破棄
	if (m_pInputMouse != NULL)
	{
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}

	// キーボードの破棄
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	// ライトの破棄
	if (m_pLight != NULL)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}

	// カメラの破棄
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	// レンダラの破棄
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	// ゲームシーン破棄
	if (m_pGame != NULL)
	{
		m_pGame = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CManager::Update(void)
{
	// カメラの更新呼び出し
	if (m_pCamera != NULL)
	{
		m_pCamera->Update();
	}

	// キーボードの更新呼び出し
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}

	// マウスの更新呼び出し
	if (m_pInputMouse != NULL)
	{
		m_pInputMouse->Update();
	}

	// ライトの更新呼び出し
	if (m_pLight != NULL)
	{
		m_pLight->Update();
	}

	// レンダラの更新呼び出し
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CManager::Draw(void)
{
	// レンダラの描画呼び出し
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}
}