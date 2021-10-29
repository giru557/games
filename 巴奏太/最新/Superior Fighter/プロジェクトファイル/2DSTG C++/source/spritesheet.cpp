//=============================================================================
//
// スプライトシートの処理 [spritesheet.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "spritesheet.h"
#include "manager.h"
#include "textureloader.h"

//*****************************************************************************
// スプライトシートクラス ( 継承元: 2Dポリゴン [scene2D] )
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CSpriteSheet::CSpriteSheet()
{
	m_nCounterAnim = 0;
	m_nPatternH = 0;
	m_nPatternV = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CSpriteSheet::~CSpriteSheet()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSpriteSheet::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// 初期化処理
	if (FAILED(CScene2D::Init(pos, rot, size)))
	{
		return E_FAIL;
	}

	// スプライトの情報設定
	this->SetSheetInfo(m_type);

	// テクスチャ座標を更新
	this->SetTextureUV(
		D3DXVECTOR2(m_nPatternH / m_fSheetColumn, (m_nPatternV + 1.0f) / m_fSheetRow),
		D3DXVECTOR2(m_nPatternH / m_fSheetColumn, m_nPatternH / m_fSheetRow),
		D3DXVECTOR2((m_nPatternH + 1.0f) / m_fSheetColumn, (m_nPatternV + 1.0f) / m_fSheetRow),
		D3DXVECTOR2((m_nPatternH + 1.0f) / m_fSheetColumn, m_nPatternH / m_fSheetRow)
	);

	// 描画優先度
	this->SetPriority(SPRITESHEET_DRAW_PRIORITY);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSpriteSheet::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CSpriteSheet::Update(void)
{
	CScene2D::Update();

	// カウントアップ
	m_nCounterAnim++;

	// 一定フレームごとに処理を実行
	if (m_nCounterAnim % m_nAnimationSpeed == 0)
	{
		// パターンを更新
		m_nPatternH++;

		// アニメーションが最後まで再生されたら
		if (m_nPatternH >= m_fSheetColumn)
		{
			// 縦軸も最後まで再生されたら消す
			if (m_nPatternV >= m_fSheetRow - 1)
			{
				Uninit();
				return;
			}
			// 縦軸を一個下にずらす
			else
			{
				m_nPatternH = 0;
				m_nPatternV++;
			}
		}

		// テクスチャ座標を更新
		this->SetTextureUV(
			D3DXVECTOR2(m_nPatternH / m_fSheetColumn, (m_nPatternV + 1.0f) / m_fSheetRow),
			D3DXVECTOR2(m_nPatternH / m_fSheetColumn, m_nPatternV / m_fSheetRow),
			D3DXVECTOR2((m_nPatternH + 1.0f) / m_fSheetColumn, (m_nPatternV + 1.0f) / m_fSheetRow),
			D3DXVECTOR2((m_nPatternH + 1.0f) / m_fSheetColumn, m_nPatternV / m_fSheetRow)
		);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CSpriteSheet::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// 生成関数
//=============================================================================
CSpriteSheet *CSpriteSheet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size, SPRITESHEET type)
{
	// 生成、初期化
	CSpriteSheet *pSpriteSheet;
	pSpriteSheet = new CSpriteSheet;
	if (pSpriteSheet != NULL)
	{
		pSpriteSheet->m_type = type;
		pSpriteSheet->Init(pos, rot, size);
	}

	return pSpriteSheet;
}

//=============================================================================
// シートの情報を設定
//=============================================================================
void CSpriteSheet::SetSheetInfo(SPRITESHEET type)
{
	CTextureLoad *pTex = CManager::GetTextureLoad();
	switch (type)
	{
	case SPRITESHEET_EXPLOSION:
		this->BindTexture(pTex->m_TextureMp["EXPLOSION"]);			// Texture
		this->m_fSheetColumn = SPRITESHEET_EXPLOSION_COLUMN;		// Sprite Column
		this->m_fSheetRow = SPRITESHEET_EXPLOSION_ROW;				// Sprite Row
		this->m_nAnimationSpeed = SPRITESHEET_EXPLOSION_ANIMSPEED;	// Animation Speed
		break;

	case SPRITESHEET_TEST:
		this->BindTexture(pTex->m_TextureMp["TEST"]);
		this->m_fSheetColumn = SPRITESHEET_TEST_COLUMN;
		this->m_fSheetRow = SPRITESHEET_TEST_ROW;
		this->m_nAnimationSpeed = SPRITESHEET_TEST_ANIMSPEED;
		break;

	case SPRITESHEET_EXPLOSION2:
		this->BindTexture(pTex->m_TextureMp["EXPLOSION2"]);
		this->m_fSheetColumn = SPRITESHEET_EXPLOSION2_COLUMN;
		this->m_fSheetRow = SPRITESHEET_EXPLOSION2_ROW;
		this->m_nAnimationSpeed = SPRITESHEET_EXPLOSION2_ANIMSPEED;
		break;

	default:
		break;
	}
}