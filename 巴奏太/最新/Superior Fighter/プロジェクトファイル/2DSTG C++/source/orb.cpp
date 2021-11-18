//=============================================================================
//
// オーブの処理 [orb.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "orb.h"
#include "manager.h"
#include "game.h"
#include "textureloader.h"
#include "player.h"
#include "particle.h"
#include "energy.h"
#include "sound.h"

//*****************************************************************************
// オーブクラス ( 継承元: 2Dオブジェクトクラス [scene2d] )
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
COrb::COrb()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
COrb::~COrb()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT COrb::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	SetPos(pos);
	SetRot(rot);
	SetSize(size);

	// 初期化
	if (FAILED(CScene2D::Init(pos, rot, size))) {
		return E_FAIL;
	}

	SetObjType(OBJTYPE_ORB);
	BindTexture(CManager::GetTextureLoad()->m_TextureMp["EFFECT"]);
	SetColor(ORB_COLOR);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void COrb::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void COrb::Update(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();
	float fDistToPlayer = CManager::GetDistanceAB(playerPos, pos);

	// プレイヤーが検知距離に居たらフラグ立てる
	if (fDistToPlayer < ORB_FIND_TRIGGERDIST)
		m_bTrigger = true;

	// フラグ立ってたら追尾して吸収させる
	if (m_bTrigger) {
		D3DXVec3Lerp(&pos, &pos, &playerPos, 0.03f);
		if (fDistToPlayer < ORB_GET_TRIGGERDIST) {
			Apply();
			Uninit();
			return;
		}
	}
	SetPos(pos);

	// オーブからパーティクル出す
	CParticle::Create(
		pos,
		VECTOR3_ZERO,
		ORB_COLOR,
		D3DXVECTOR2(10, 10),
		CEffect::EFFECT_TEX_DEFAULT,
		1,
		1.0f,
		1,
		0.001f,
		0.001f,
		0,
		0);
}

//=============================================================================
// 描画処理
//=============================================================================
void COrb::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// レンダーステートの設定
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);		//ソース（描画元）の合成方法の設定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			//デスティネーション（描画先）の合成方法の設定

	// 描画
	CScene2D::Draw();

	// レンダーステートをもとに戻す
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		//ソース（描画元）の合成方法の設定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	//デスティネーション（描画先）の合成方法の設定
}

//=============================================================================
// 生成関数
//=============================================================================
COrb *COrb::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// 生成、初期化
	COrb *pOrb;
	pOrb = new COrb;
	if (pOrb != NULL)
	{
		pOrb->Init(pos, rot, size);
	}

	return pOrb;
}

COrb *COrb::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size, int nCreateChance)
{
	int nRandCreate = rand() % 100 + 1;
	if (nCreateChance >= nRandCreate) {
		// 生成、初期化
		COrb *pOrb;
		pOrb = new COrb;
		if (pOrb != NULL)
		{
			pOrb->Init(pos, rot, size);
		}

		return pOrb;
	}

	return NULL;
}

//=============================================================================
// オーブの効果を適用する
//=============================================================================
void COrb::Apply(void)
{
	CGame::GetEnergy()->AddEnergy(ORB_ENERGY_GAIN);	// エネルギーを回復
	CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_ORB);	// 音声

	CParticle::Create(	// パーティクル
		CGame::GetPlayer()->GetPos(),
		VECTOR3_ZERO,
		ORB_COLOR,
		D3DXVECTOR2(20, 20),
		CEffect::EFFECT_TEX_DEFAULT,
		1,
		10.0f,
		50,
		0.01f,
		0.01f,
		0,
		0);
}