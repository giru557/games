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
#include "textureloader.h"

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

	// オブジェクト生成
	m_pScene2D = CScene2D::Create((D3DXVECTOR3)SCREEN_CENTER, VECTOR3_ZERO, D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));

	// ゲームの終了状況を見てテクスチャを決める
	if (CGame::GetGameStatus() == CGame::GAMESTATUS_CLEAR)
		m_pScene2D->BindTexture(CManager::GetTextureLoad()->m_TextureMp["GAMECLEAR"]);
	else if (CGame::GetGameStatus() == CGame::GAMESTATUS_OVER)
		m_pScene2D->BindTexture(CManager::GetTextureLoad()->m_TextureMp["GAMEOVER"]);

	// カメラの中心を戻す
	CCamera *pCamera = CManager::GetCamera();
	pCamera->SetCameraCenter((D3DXVECTOR3)SCREEN_CENTER);

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CResult::Uninit(void)
{
	// 背景のポリゴンオブジェクトを破棄
	if (m_pScene2D != NULL) {
		m_pScene2D->Uninit();
		m_pScene2D = NULL;
	}

	// 開放
	Release();
}

//=============================================================================
// 更新
//=============================================================================
void CResult::Update(void)
{
	// エンターキー
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true)
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
