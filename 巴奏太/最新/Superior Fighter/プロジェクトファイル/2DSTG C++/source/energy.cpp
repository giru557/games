//=============================================================================
//
// エネルギーの処理 [energy.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "energy.h"
#include "manager.h"
#include "game.h"


//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************


//*****************************************************************************
// エネルギークラス ( 継承元: オブジェクトクラス [scene] )
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CEnergy::CEnergy()
{
	m_nEnergy = ENERGY_POINTS_INIT;
	m_fEnergyRatio = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CEnergy::~CEnergy()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEnergy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	SetPos(pos);
	SetRot(rot);
	SetSize(size);

	// シーン生成
	for (int nCntScene = 0; nCntScene < ENERGY_LAYER; nCntScene++) {
		m_apScene[nCntScene] = CScene2D::Create(GetPos(), GetRot(), GetSize());
		m_apScene[nCntScene]->SetConvertState(false);
		m_apScene[nCntScene]->SetPriority(ENERGY_DRAW_PRIORITY);
	}

	// 色設定
	m_apScene[ENERGY_IDX_BACK]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f));
	m_apScene[ENERGY_IDX_FRONT]->SetColor(D3DXCOLOR(0.6f, 1.0f, 1.0f, 1.0f));
	m_apScene[ENERGY_IDX_MIDDLE]->SetColor(D3DXCOLOR(0.0f, 0.7f, 0.7f, 1.0f));
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEnergy::Uninit(void)
{
	// 開放処理 (sceneのポインタ配列から削除）
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEnergy::Update(void)
{
	// 自然回復
	Regenerate(ENERGY_POINTS_REGENERATE);

	// エネルギーのバーの処理
	BarProc();
}

//=============================================================================
// 描画処理
//=============================================================================
void CEnergy::Draw(void)
{

}

//=============================================================================
// 生成関数
//=============================================================================
CEnergy *CEnergy::Create(void)
{
	// 生成、初期化
	CEnergy *pEnergy;
	pEnergy = new CEnergy;
	if (pEnergy != NULL)
	{
		pEnergy->Init(ENERGY_POSITION, VECTOR3_ZERO, ENERGY_SIZE);
	}

	return pEnergy;
}

//=============================================================================
// バーの処理まとめ
//=============================================================================
void CEnergy::BarProc(void)
{
	// 割合
	m_fEnergyRatio = (float)m_nEnergy / (float)ENERGY_POINTS_MAX;

	// 頂点取得
	D3DXVECTOR3 *apVertexFront[4];
	memcpy(&apVertexFront[0], m_apScene[ENERGY_IDX_FRONT]->GetVertices(), sizeof(apVertexFront));

	// 位置、サイズ変更
	D3DXVECTOR2 size = ENERGY_SIZE;
	*apVertexFront[2] = D3DXVECTOR3(apVertexFront[0]->x + (size.x * m_fEnergyRatio), apVertexFront[2]->y, 0.0f);
	*apVertexFront[3] = D3DXVECTOR3(apVertexFront[1]->x + (size.x * m_fEnergyRatio), apVertexFront[3]->y, 0.0f);
	m_apScene[ENERGY_IDX_FRONT]->SetVertices(*apVertexFront[0], *apVertexFront[1], *apVertexFront[2], *apVertexFront[3]);

	// テクスチャ座標
	m_apScene[ENERGY_IDX_FRONT]->SetTextureUV(
		D3DXVECTOR2(0.0f, 1.0f),
		D3DXVECTOR2(0.0f, 0.0f),
		D3DXVECTOR2(1.0f * m_fEnergyRatio, 1.0f),
		D3DXVECTOR2(1.0f * m_fEnergyRatio, 0.0f));



	// 頂点取得
	D3DXVECTOR3 *apVertexMid[4];
	memcpy(&apVertexMid[0], m_apScene[ENERGY_IDX_MIDDLE]->GetVertices(), sizeof(apVertexFront));

	if (apVertexMid[2]->x > apVertexFront[2]->x) {
		D3DXVec3Lerp(apVertexMid[2], apVertexMid[2], apVertexFront[2], 0.05f);
		D3DXVec3Lerp(apVertexMid[3], apVertexMid[3], apVertexFront[3], 0.05f);
	}
	else {
		apVertexMid[2] = apVertexFront[2];
		apVertexMid[3] = apVertexFront[3];
	}

	m_apScene[ENERGY_IDX_MIDDLE]->SetVertices(*apVertexMid[0], *apVertexMid[1], *apVertexMid[2], *apVertexMid[3]);

}

//=============================================================================
// エネルギー使用 成功した場合trueを返す
//=============================================================================
bool CEnergy::UseEnergy(int nValue)
{
	if (m_nEnergy - nValue < 0) {
		return false;
	}
	else {
		m_nEnergy = m_nEnergy - nValue;
		return true;
	}
}

//=============================================================================
// エネルギー自然回復
//=============================================================================
void CEnergy::Regenerate(int nRegenSpeed)
{
	static int nTickRegen;
	nTickRegen++;
	if (nTickRegen > nRegenSpeed) {
		if (m_nEnergy < ENERGY_POINTS_MAX)
			m_nEnergy++;

		nTickRegen = 0;
	}
}

//=============================================================================
// エネルギーを回復させる
//=============================================================================
void CEnergy::AddEnergy(int nValue)
{
	if (m_nEnergy + nValue >= ENERGY_POINTS_MAX) {
		m_nEnergy = ENERGY_POINTS_MAX;
	}
	else {
		m_nEnergy += nValue;
	}
}