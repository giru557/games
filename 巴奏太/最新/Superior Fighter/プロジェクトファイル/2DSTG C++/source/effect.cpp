//=============================================================================
//
// エフェクトクラス [effect.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "effect.h"
#include "manager.h"
#include "renderer.h"
#include "textureloader.h"
#include "camera.h"

//*****************************************************************************
// エフェクトクラス ( 継承元: 2Dポリゴン [scene2D] )
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CEffect::CEffect(int nPriority) : CScene2D(nPriority)
{
	m_fAlpha = 1.0f;
	m_fShrink = 1.0f;
	m_fShrinkSpeed = 0.0f;
	m_fTransparentSpeed = 0.0f;
	m_blendDst = D3DBLEND_ONE;
	m_blendSrc = D3DBLEND_ONE;
}

//=============================================================================
// デストラクタ
//=============================================================================
CEffect::~CEffect()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CEffect::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// 初期化処理
	if (FAILED(CScene2D::Init(pos, rot, size)))
	{
		return E_FAIL;
	}

	// 描画優先度を設定
	this->SetPriority(EFFECT_DRAW_PRIORITY);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEffect::Uninit(void)
{
	// 終了
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEffect::Update(void)
{
	// 位置の更新と減衰の設定
	D3DXVECTOR3 pos = GetPos();
	m_move.x += (0 - m_move.x) * 0.01f;
	m_move.y += (0 - m_move.y) * 0.01f;
	pos += m_move;
	SetPos(pos);

	// 縮小処理
	D3DXVECTOR2 size = GetSize();
	this->SetSize(D3DXVECTOR2(size.x * m_fShrink, size.y * m_fShrink));
	m_fShrink -= m_fShrinkSpeed;
	if (m_fShrink <= 0.8f)
	{
		this->Uninit();
		return;
	}
	
	// 透明化処理
	D3DXCOLOR col = GetColor();
	this->SetColor(D3DXCOLOR(col.r, col.g, col.b, col.a * m_fAlpha));
	m_fAlpha -= m_fTransparentSpeed;
	if (m_fAlpha <= 0.8f)
	{
		this->Uninit();
		return;
	}

	// 頂点更新
	UpdateVertex();

	// 画面外のやつを描画しない
	D3DXVECTOR3 screenPos = CCamera::WorldToScreenPosition(pos);
	if (screenPos.x > SCREEN_WIDTH + size.x ||
		screenPos.x < 0 - size.x ||
		screenPos.y > SCREEN_HEIGHT + size.y ||
		screenPos.y < 0 - size.y)
		this->SetDrawFlag(false);
	else {
		this->SetDrawFlag(true);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CEffect::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	
	// レンダーステートの設定
	pDevice->SetRenderState(D3DRS_SRCBLEND, m_blendSrc);		//ソース（描画元）の合成方法の設定
	pDevice->SetRenderState(D3DRS_DESTBLEND, m_blendDst);			//デスティネーション（描画先）の合成方法の設定

	// 描画
	CScene2D::Draw();

	// レンダーステートをもとに戻す
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		//ソース（描画元）の合成方法の設定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	//デスティネーション（描画先）の合成方法の設定
}

//=============================================================================
// テクスチャの設定
//=============================================================================
void CEffect::SetTexture(EFFECT_TEX tex)
{
	switch (tex)
	{
	case EFFECT_TEX_DEFAULT:
		this->BindTexture(CManager::GetTextureLoad()->m_TextureMp["EFFECT"]);
		break;

	case EFFECT_TEX_PARTS1:
		this->BindTexture(CManager::GetTextureLoad()->m_TextureMp["PLAYERPARTS"]);
		break;

	case EFFECT_TEX_PARTS2:
		this->BindTexture(CManager::GetTextureLoad()->m_TextureMp["PLAYERPARTS2"]);
		break;

	case EFFECT_TEX_PARTS3:
		this->BindTexture(CManager::GetTextureLoad()->m_TextureMp["PLAYERPARTS3"]);
		break;

	case EFFECT_TEX_SMOKE:
		this->BindTexture(CManager::GetTextureLoad()->m_TextureMp["SMOKE"]);
		break;

	default:
		break;
	}
}

//=============================================================================
// 生成関数
//=============================================================================
CEffect *CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size, D3DXCOLOR col, EFFECT_TEX tex, float fShrinkSpd, float fTransparentSpd)
{
	CEffect *pEffect;
	pEffect = new CEffect;
	if (pEffect != NULL)
	{
		pEffect->Init(pos, rot, size);
		pEffect->SetTexture(tex);
		pEffect->SetColor(col);

		// 値がどちらも無効だった場合消してしまう
		if (fTransparentSpd <= 0.0f && fShrinkSpd <= 0.0f)
		{
			pEffect->Uninit();
			pEffect = NULL;
		}
		else
		{
			// 引数から値を受け取って代入、マイナスの場合０に矯正
			pEffect->m_fTransparentSpeed = fTransparentSpd;
			pEffect->m_fShrinkSpeed = fShrinkSpd;
			if (pEffect->m_fTransparentSpeed <= 0.0f) pEffect->m_fTransparentSpeed = 0.0f;
			if (pEffect->m_fShrinkSpeed <= 0.0f) pEffect->m_fShrinkSpeed = 0.0f;
		}
	}

	return pEffect;
}