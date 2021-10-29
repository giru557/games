//=============================================================================
//
// リザルト画面 [result.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "result.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "camera.h"
#include "game.h"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CResult::m_apTexture[RESULT_TEXTURE_COUNT] = {};

//*****************************************************************************
// リザルトクラス ( 継承元: オブジェクトクラス [scene] )
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CResult::CResult()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CResult::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの生成
	if (FAILED(D3DXCreateTextureFromFile(pDevice, RESULT_TEXTURE_0_FILEPATH, &m_apTexture[0])) ||
		FAILED(D3DXCreateTextureFromFile(pDevice, RESULT_TEXTURE_1_FILEPATH, &m_apTexture[1])))
	{
		return E_FAIL;
	}

	// オブジェクト生成
	m_pScene2D = CScene2D::Create((D3DXVECTOR3)SCREEN_CENTER, VECTOR3_ZERO, D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));

	// ゲームの終了状況を見てテクスチャを決める
	CGame::GAMESTATUS status = CGame::GetGameStatus();
	if (status == CGame::GAMESTATUS_CLEAR)
	{
		m_pScene2D->BindTexture(m_apTexture[0]);
	}
	else if (status == CGame::GAMESTATUS_OVER)
	{
		m_pScene2D->BindTexture(m_apTexture[1]);
	}

	CCamera *pCamera = CManager::GetCamera();
	pCamera->SetCameraCenter((D3DXVECTOR3)SCREEN_CENTER);

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CResult::Uninit(void)
{
	for (int nCntTex = 0; nCntTex < RESULT_TEXTURE_COUNT; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
	m_pScene2D->Uninit();
	m_pScene2D = NULL;
	Release();
}

//=============================================================================
// 更新
//=============================================================================
void CResult::Update(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	if (pKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		// フェードアウトして次のモード
		CManager::GetFade()->FadeOUT(CManager::MODE_TITLE);
	}
}

//=============================================================================
// 描画
//=============================================================================
void CResult::Draw(void)
{

}
