//=============================================================================
//
// タイトル [title.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "title.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "textureloader.h"

//*****************************************************************************
// タイトルクラス ( 継承元: オブジェクトクラス [scene] )
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CTitle::CTitle()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CTitle::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// オブジェクトのインスタンス生成
	m_pSceneTitle = CScene2D::Create((D3DXVECTOR3)SCREEN_CENTER, VECTOR3_ZERO, D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));
	m_pSceneStart = CScene2D::Create(TITLE_TEXTURE_START_POS, VECTOR3_ZERO, TITLE_TEXTURE_START_SIZE);
	
	// テクスチャ貼り付け
	m_pSceneTitle->BindTexture(CManager::GetTextureLoad()->m_TextureMp["TITLE"]);
	m_pSceneStart->BindTexture(CManager::GetTextureLoad()->m_TextureMp["START"]);

	// フェードイン
	CManager::GetFade()->FadeIN();
	CManager::GetSound()->Play(CSound::SOUND_LABEL_BGM_TITLE);	// BGM

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CTitle::Uninit(void)
{
	CManager::GetSound()->Stop();

	// オブジェクトを破棄
	if (m_pSceneStart != NULL) {
		m_pSceneStart->Uninit();
		m_pSceneStart = NULL;
	}
	if (m_pSceneTitle != NULL) {
		m_pSceneTitle->Uninit();
		m_pSceneTitle = NULL;
	}

	// Release this object
	this->Release();
}

//=============================================================================
// 更新
//=============================================================================
void CTitle::Update(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	m_nBlinkCounter++;	// カウントアップ

	// エンターキー押下時
	if (pKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		// フェードアウトして次のモード
		CManager::GetFade()->FadeOUT(CManager::MODE_GAME);
	}

	// スタートボタンを点滅させる
	this->BlinkStartObj();
}

//=============================================================================
// 描画
//=============================================================================
void CTitle::Draw(void)
{

}

//=============================================================================
// スタートボタンのオブジェを点滅させる関数
//=============================================================================
void CTitle::BlinkStartObj(void)
{
	// フレームカウントが一定数を超えたら
	if (m_nBlinkCounter > TITLE_START_BLINK_FREQUENCY)
	{
		m_bAppear = !m_bAppear;	// フラグ反転
		m_nBlinkCounter = 0;	// カウンター初期化
	}

	// 真で表示、偽で非表示
	if (m_bAppear)
		m_pSceneStart->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	else
		m_pSceneStart->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
}