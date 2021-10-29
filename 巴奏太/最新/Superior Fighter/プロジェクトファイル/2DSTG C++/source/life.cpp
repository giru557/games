//=============================================================================
//
// 体力の処理 [life.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "life.h"
#include "manager.h"
#include "game.h"
#include "camera.h"
#include "textureloader.h"
#include "sound.h"
#include "particle.h"
#include "player.h"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//*****************************************************************************
// ライフクラス ( 継承元: オブジェクトクラス [scene] )
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CLife::CLife()
{
	memset(&m_apScene2D[0], NULL, sizeof(m_apScene2D));
	m_pos = VECTOR3_ZERO;
	m_nLife = 0;
	m_fLifeRatio = 0.0f;
	m_fDmgTransparency = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CLife::~CLife()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLife::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// 体力ポリゴン用の設定
	m_apScene2D[LIFE_IDX_FRONT] = CScene2D::Create(pos + LIFE_POSITION_OFFSET, rot, LIFE_SIZE_FRONT);
	m_apScene2D[LIFE_IDX_BACK] = CScene2D::Create(pos, rot, LIFE_SIZE_BACK);
	m_apScene2D[LIFE_IDX_MIDDLE] = CScene2D::Create(LIFE_POSITION_MIDDLE, rot, LIFE_SIZE_MIDDLE);
	m_apScene2D[LIFE_IDX_MIDDLE2] = CScene2D::Create(LIFE_POSITION_MIDDLE, rot, LIFE_SIZE_MIDDLE);

	for (int nCnt = 0; nCnt < LIFE_LAYER; nCnt++)
	{
		if (m_apScene2D[nCnt] != NULL) {
			m_apScene2D[nCnt]->SetConvertState(false);
			m_apScene2D[nCnt]->SetPriority(4);
		}
	}

	m_apScene2D[LIFE_IDX_MIDDLE]->SetColor(LIFE_MIDDLE_COLOR);
	m_apScene2D[LIFE_IDX_MIDDLE2]->SetDrawFlag(false);
	m_apScene2D[LIFE_IDX_FRONT]->BindTexture(CManager::GetTextureLoad()->m_TextureMp["LIFE1"]);
	m_apScene2D[LIFE_IDX_BACK]->BindTexture(CManager::GetTextureLoad()->m_TextureMp["LIFE3"]);

	// 背景ポリゴン用の設定
	m_apSceneBG = CScene2D::Create((D3DXVECTOR3)SCREEN_CENTER, VECTOR3_ZERO, D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));
	m_apSceneBG->SetConvertState(false);
	m_apSceneBG->SetPriority(1);
	m_apSceneBG->SetColor(LIFE_DMG_COLOR_TRANSPARENT);

	// 初期化
	m_fLifeRatio = 1.0f;
	m_fDmgTransparency = LIFE_DMG_TRANSPARENCY;
	m_nLife = LIFE_HP_DEFAULT;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLife::Uninit(void)
{
	// 終了処理
	for (int nCnt = 0; nCnt < LIFE_LAYER; nCnt++)
	{
		if (m_apScene2D[nCnt] != NULL) {
			m_apScene2D[nCnt]->Uninit();
			m_apScene2D[nCnt] = NULL;
		}
	}

	// 開放処理 (sceneのポインタ配列から削除）
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CLife::Update(void)
{
	// バーの処理
	BarProc();

	// 煙を出す処理
	Smoke();

	// IF DMGED
	if (m_bDmg)
	{
		m_apSceneBG->SetColor(D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f * m_fDmgTransparency));
		m_fDmgTransparency -= LIFE_DMG_TRANS_SPEED;

		if (m_fDmgTransparency <= 0)
		{
			m_bDmg = false;
			m_apSceneBG->SetColor(LIFE_DMG_COLOR_TRANSPARENT);
			m_fDmgTransparency = LIFE_DMG_TRANSPARENCY;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CLife::Draw(void)
{

}

//=============================================================================
// ダメージ処理
//=============================================================================
void CLife::Damage(int nValue)
{
	// ダメージフラグを立てる
	m_bDmg = true;

	// 画面を揺らす
	CCamera *pCam = CManager::GetCamera();
	pCam->Shake(5.0f, 10);

	// 体力をへらす
	m_nLife -= nValue;

	// 体力０以下の場合ゲームオーバー
	if (m_nLife <= 0)
	{
		CManager::GetGame()->SetGameStatus(CGame::GAMESTATUS_OVER);
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_EXPLOSION00);
	}
}

//=============================================================================
// 回復処理
//=============================================================================
void CLife::Heal(int nValue)
{
	if (m_nLife + nValue >= LIFE_HP_MAX) {
		m_nLife = LIFE_HP_MAX;
	}
	else {
		m_nLife += nValue;
	}
}

//=============================================================================
// 生成関数
//=============================================================================
CLife *CLife::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// 生成、初期化
	CLife *pLife;
	pLife = new CLife;
	if (pLife != NULL)
	{
		pLife->Init(pos, rot, (D3DXVECTOR2)VECTOR3_ZERO);
	}

	return pLife;
}

//=============================================================================
// 煙を出す
//=============================================================================
void CLife::Smoke()
{
	static int nTickFrame;
	if (m_fLifeRatio <= 0.5f) {
		nTickFrame++;
		if (nTickFrame >= PLAYER_SMOKE_FREQ) {
			CParticle *pParticle = CParticle::Create(
				CGame::GetPlayer()->GetPos(),
				true,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				D3DXVECTOR2(40, 40),
				CEffect::EFFECT_TEX_SMOKE,
				1,
				7.0f,
				5,
				0.005f,
				0.01f,
				0.0f,
				0.0f);
			pParticle->SetPriority(4);
			pParticle->SetRenderState(D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA);

			nTickFrame = 0;
		}
	}
}

//=============================================================================
// バーの処理
//=============================================================================
void CLife::BarProc(void)
{
	// 体力の割合を計算
	m_fLifeRatio = (float)m_nLife / (float)LIFE_HP_MAX;
	if (m_fLifeRatio <= 0.0f) m_fLifeRatio = 0.0f;

	// 頂点取得
	D3DXVECTOR3 *apVertex[4];
	memcpy(&apVertex[0], m_apScene2D[LIFE_IDX_FRONT]->GetVertices(), sizeof(apVertex));

	// 頂点設定
	D3DXVECTOR2 size = LIFE_SIZE_FRONT;
	*apVertex[2] = D3DXVECTOR3(apVertex[0]->x + (size.x * m_fLifeRatio), apVertex[2]->y, 0.0f);
	*apVertex[3] = D3DXVECTOR3(apVertex[1]->x + (size.x * m_fLifeRatio), apVertex[3]->y, 0.0f);
	m_apScene2D[LIFE_IDX_FRONT]->SetVertices(*apVertex[0], *apVertex[1], *apVertex[2], *apVertex[3]);

	// テクスチャ座標
	m_apScene2D[LIFE_IDX_FRONT]->SetTextureUV(
		D3DXVECTOR2(0.0f, 1.0f),
		D3DXVECTOR2(0.0f, 0.0f),
		D3DXVECTOR2(1.0f * m_fLifeRatio, 1.0f),
		D3DXVECTOR2(1.0f * m_fLifeRatio, 0.0f));



	// 頂点取得
	D3DXVECTOR3 *apVertexMid2[4];
	memcpy(&apVertexMid2[0], m_apScene2D[LIFE_IDX_MIDDLE2]->GetVertices(), sizeof(apVertexMid2));

	// 頂点設定
	D3DXVECTOR2 sizeMid = LIFE_SIZE_MIDDLE;
	*apVertexMid2[2] = D3DXVECTOR3(apVertexMid2[0]->x + (sizeMid.x * m_fLifeRatio), apVertexMid2[2]->y, 0.0f);
	*apVertexMid2[3] = D3DXVECTOR3(apVertexMid2[1]->x + (sizeMid.x * m_fLifeRatio), apVertexMid2[3]->y, 0.0f);
	m_apScene2D[LIFE_IDX_MIDDLE2]->SetVertices(*apVertexMid2[0], *apVertexMid2[1], *apVertexMid2[2], *apVertexMid2[3]);


	// 頂点取得
	D3DXVECTOR3 *apVertexMid[4];
	memcpy(&apVertexMid[0], m_apScene2D[LIFE_IDX_MIDDLE]->GetVertices(), sizeof(apVertexMid));

	if (apVertexMid[2]->x > apVertexMid2[2]->x) {
		D3DXVec3Lerp(apVertexMid[2], apVertexMid[2], apVertexMid2[2], 0.05f);
		D3DXVec3Lerp(apVertexMid[3], apVertexMid[3], apVertexMid2[3], 0.05f);
	}
	else {
		apVertexMid[2] = apVertexMid2[2];
		apVertexMid[3] = apVertexMid2[3];
	}

	m_apScene2D[LIFE_IDX_MIDDLE]->SetVertices(*apVertexMid[0], *apVertexMid[1], *apVertexMid[2], *apVertexMid[3]);

}