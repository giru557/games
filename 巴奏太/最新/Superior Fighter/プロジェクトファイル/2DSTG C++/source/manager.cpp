//=============================================================================
//
// マネージャ [manager.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "fade.h"
#include "camera.h"
#include "textureloader.h"
#include "debugger.h"
#include "pause.h"
#include "camera3d.h"
#include "light.h"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CRenderer *CManager::m_pRenderer = NULL;
CInputKeyboard *CManager::m_pInputKeyboard = NULL;
CInputMouse *CManager::m_pInputMouse = NULL;
CSound *CManager::m_pSound = NULL;
CTitle *CManager::m_pTitle = NULL;
CGame *CManager::m_pGame = NULL;
CResult *CManager::m_pResult = NULL;
CFade  *CManager::m_pFade = NULL;
CCamera *CManager::m_pCamera = NULL;
CPause *CManager::m_pPause = NULL;
CTextureLoad *CManager::m_pTextureLoad = NULL;
CCamera3D *CManager::m_pCamera3D = NULL;
CLight *CManager::m_pLight = NULL;
CManager::MODE CManager::m_mode = CManager::MODE_TITLE;
#ifdef _DEBUG
CDebug *CManager::m_pDebug = NULL;
#endif

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

	// ライトを生成
	m_pLight = new CLight;
	if (m_pLight != NULL) {
		m_pLight->Init();
	}

	// 3Dカメラを生成
	m_pCamera3D = new CCamera3D;
	if (m_pCamera3D != NULL) {
		m_pCamera3D->Init();
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

	// サウンドクラスの生成、初期化
	m_pSound = new CSound;
	if (FAILED(m_pSound->Init(hWnd)))
	{
		return E_FAIL;
	}

	// フェードクラスの生成、初期化
	m_pFade = new CFade;
	if (FAILED(m_pFade->Init()))
	{
		return E_FAIL;
	}

	// カメラクラスの生成、初期化
	m_pCamera = new CCamera;
	if (m_pCamera != NULL)
	{
		m_pCamera->Init();
	}

	// ポーズクラスの生成
	m_pPause = new CPause;
	if (m_pPause != NULL) {
		m_pPause->Init();
	}

	// テクスチャ読み込みクラス
	m_pTextureLoad = new CTextureLoad;
	if (FAILED(m_pTextureLoad->Init()))
	{
		return E_FAIL;
	}

#ifdef _DEBUG
	m_pDebug = new CDebug;
	if (FAILED(m_pDebug->Init())) {
		return E_FAIL;
	}
#endif

	// モード選択
	SetMode(MODE_TITLE);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CManager::Uninit(void)
{
	// オブジェクトをすべて破棄
	CScene::ReleaseAll();

#ifdef _DEBUG
	if (m_pDebug != NULL) {
		m_pDebug->Uninit();
		delete m_pDebug;
		m_pDebug = NULL;
	}
#endif

	// テクスチャを破棄
	if (m_pTextureLoad != NULL)
	{
		m_pTextureLoad->Uninit();
		delete m_pTextureLoad;
		m_pTextureLoad = NULL;
	}

	// ポーズの破棄
	if (m_pPause != NULL) {
		m_pPause->Uninit();
		delete m_pPause;
		m_pPause = NULL;
	}

	// カメラの破棄
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}
	
	// フェードクラスの破棄
	if (m_pFade != NULL)
	{
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = NULL;
	}

	// サウンドクラスの破棄
	if (m_pSound != NULL)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}

	// キーボードの破棄
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	// マウスの破棄
	if (m_pInputMouse != NULL)
	{
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}

	// カメラ破棄
	if (m_pCamera3D != NULL) {
		m_pCamera3D->Uninit();
		delete m_pCamera3D;
		m_pCamera3D = NULL;
	}

	// ライトを破棄
	if (m_pLight != NULL) {
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}

	// レンダラの破棄
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CManager::Update(void)
{
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

	// レンダラの更新呼び出し
	if (m_pRenderer != NULL)
	{
		// ポーズ状態じゃないときだけ更新
		if (!m_pPause->GetPauseState()) {
			m_pRenderer->Update();
		}
	}

	// フェードの更新
	if (m_pFade != NULL)
	{
		m_pFade->Update();
	}

	// カメラの更新
	if (m_pCamera != NULL)
	{
		m_pCamera->Update();
	}

	if (m_pCamera3D != NULL) {
		m_pCamera3D->Update();
	}

	// ライトの更新
	if (m_pLight != NULL) {
		m_pLight->Update();
	}

	// ポーズの更新
	if (m_pPause != NULL) {
		m_pPause->Update();
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

//=============================================================================
// モードの設定
//=============================================================================
void CManager::SetMode(MODE mode)
{
	// 現在モードの破棄
	switch (m_mode)
	{
	case MODE_TITLE:
		if (m_pTitle != NULL)
		{
			m_pTitle->Uninit();
			m_pTitle = NULL;
		}
		break;

	case MODE_GAME:
		if (m_pGame != NULL)
		{
			m_pGame->Uninit();
			m_pGame = NULL;
		}
		break;

	case MODE_RESULT:
		if (m_pResult != NULL)
		{
			m_pResult->Uninit();
			m_pResult = NULL;
		}
		break;

	default:
		break;
	}

	// 全オブジェクトの開放
	CScene::ReleaseAll();

	// 指定モードの生成
	m_mode = mode;
	switch (mode)
	{
	case MODE_TITLE:
		m_pTitle = new CTitle;
		if (m_pTitle != NULL)
		{
			m_pTitle->Init(VECTOR3_ZERO, VECTOR3_ZERO, (D3DXVECTOR2)VECTOR3_ZERO);
		}
		break;

	case MODE_GAME:
		m_pGame = new CGame;
		if (m_pGame != NULL)
		{
			m_pGame->Init(VECTOR3_ZERO, VECTOR3_ZERO, (D3DXVECTOR2)VECTOR3_ZERO);
		}
		break;

	case MODE_RESULT:
		m_pResult = new CResult;
		if (m_pResult != NULL)
		{
			m_pResult->Init(VECTOR3_ZERO, VECTOR3_ZERO, (D3DXVECTOR2)VECTOR3_ZERO);
		}
		break;
	default:
		break;
	}
}

//=============================================================================
// AからBに向かう単位ベクトルを取得
//=============================================================================
D3DXVECTOR3 CManager::GetVectorFromPointAtoB(D3DXVECTOR3 pointA, D3DXVECTOR3 pointB)
{
	D3DXVECTOR2 vec;

	vec = (D3DXVECTOR2)pointB - (D3DXVECTOR2)pointA;
	D3DXVec2Normalize(&vec, &vec);
	
	return (D3DXVECTOR3)vec;
}

//=============================================================================
// 位置AとBの間の距離を取得
//=============================================================================
float CManager::GetDistanceAB(D3DXVECTOR3 posA, D3DXVECTOR3 posB)
{
	D3DXVECTOR3 diff;

	diff.x = posA.x - posB.x;
	diff.y = posA.y - posB.y;

	return sqrtf(diff.x * diff.x + diff.y * diff.y);
}