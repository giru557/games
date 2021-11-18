//=============================================================================
//
// 背景クラス [bg.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "bg.h"
#include "manager.h"
#include "game.h"
#include "textureloader.h"

//*****************************************************************************
// 背景クラス ( 派生元: オブジェクトクラス (scene) )
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CBackground::CBackground()
{
	m_fFadeValue = 0.0f;
	m_pScene2D = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CBackground::~CBackground()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBackground::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	m_pScene2D = CScene2D::Create(pos, rot, size);
	m_pScene2D->BindTexture(NULL);					// テクスチャ設定
	m_pScene2D->SetPriority(BG_DRAW_PRIORITY);		// 描画優先度設定

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBackground::Uninit(void)
{
	// 開放処理 (sceneのポインタ配列から削除）
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBackground::Update(void)
{
	// フェード処理
	switch (m_fade)
	{
	case BG_FADE_IN:
		m_fFadeValue -= 0.02f;
		if (m_fFadeValue <= 0) {
			m_fFadeValue = 0.0f;
			m_fade = BG_FADE_NONE;
		}
		break;

	case BG_FADE_OUT:
		m_fFadeValue += 0.02f;
		if (m_fFadeValue >= 1.0f) {
			m_fade = BG_FADE_IN;
		}
		break;

	default:
		break;
	}

	// 透明度設定
	m_pScene2D->SetColor(D3DXCOLOR(0, 0, 0, m_fFadeValue));
}

//=============================================================================
// 描画処理
//=============================================================================
void CBackground::Draw(void)
{

}

//=============================================================================
// 生成関数
//=============================================================================
CBackground *CBackground::Create(void)
{
	// 生成、初期化
	CBackground *pBG;
	pBG = new CBackground;
	if (pBG != NULL)
	{
		pBG->Init(D3DXVECTOR3(GAME_FIELD_WIDTH / 2, GAME_FIELD_HEIGHT / 2, 0.0f), VECTOR3_ZERO, D3DXVECTOR2(GAME_FIELD_WIDTH, GAME_FIELD_HEIGHT));
	}

	return pBG;
}

//=============================================================================
// フェード
//=============================================================================
void CBackground::SetFade(CBackground::BG_FADE fade)
{
	m_fade = fade;
}