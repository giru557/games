//=============================================================================
//
// タレットの残骸 [debris.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "debris.h"
#include "manager.h"
#include "textureloader.h"
#include "particle.h"

//*****************************************************************************
// 残骸クラス ( 継承元: 2Dポリゴン [scene2D] )
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CDebris::CDebris()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CDebris::~CDebris()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CDebris::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// 初期化処理
	if (FAILED(CScene2D::Init(pos, rot, size)))
	{
		E_FAIL;
	}

	// テクスチャの設定
	// 種類の設定
	BindTexture(CManager::GetTextureLoad()->m_TextureMp["BOSSTURRETBROKEN"]);
	SetObjType(OBJTYPE_DEBRIS);
	SetPriority(2);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CDebris::Uninit(void)
{
	CScene2D::Uninit();
}
//=============================================================================
// 更新処理
//=============================================================================
void CDebris::Update(void)
{
	CScene2D::Update();

	static int nFrameCounter;
	nFrameCounter++;

	if (nFrameCounter > 60) {
		CParticle *pParticle = CParticle::Create(
			GetPos(),
			true,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			D3DXVECTOR2(80, 80),
			CEffect::EFFECT_TEX_SMOKE,
			1,
			3.0f,
			5,
			0.0005f,
			0.001f,
			0.0f,
			0.0f);
		pParticle->SetPriority(3);
		pParticle->SetRenderState(D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA);

		nFrameCounter = 0;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CDebris::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// 生成関数
//=============================================================================
CDebris *CDebris::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// プレイヤーの生成、初期化
	CDebris *pDebris = NULL;
	pDebris = new CDebris;
	if (pDebris != NULL)
	{
		pDebris->Init(pos, rot, size);
	}

	return pDebris;
}