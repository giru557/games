//=============================================================================
//
// ミニマップ [minimap.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "minimap.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "orb.h"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//*****************************************************************************
// ミニマップクラス ( 派生元: オブジェクトクラス (scene) )
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CMiniMap::CMiniMap()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CMiniMap::~CMiniMap()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMiniMap::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	SetPos(pos);

	// 一回全部オブジェクト作る
	for (int nCntScene = 0; nCntScene < MINIMAP_DRAW_MAX; nCntScene++) {
		m_apScene[nCntScene] = CScene2D::Create(VECTOR3_ZERO, VECTOR3_ZERO, D3DXVECTOR2(10, 10));
		m_apScene[nCntScene]->SetConvertState(false);
		m_apScene[nCntScene]->SetPriority(4);
		m_apScene[nCntScene]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		m_apScene[nCntScene]->SetDrawFlag(false);
	}

	m_apScene[IDX_MAP]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.7f, 0.5f));
	m_apScene[IDX_MAP]->SetPos(D3DXVECTOR3(MINIMAP_SIZE.x / 2, MINIMAP_SIZE.y / 2, 0) + m_pos);
	m_apScene[IDX_MAP]->SetSize(size);
	m_apScene[IDX_MAP]->SetDrawFlag(true);
	m_apScene[IDX_MAP]->UpdateVertex();

	m_apScene[IDX_PLAYER]->SetDrawFlag(true);
	m_apScene[IDX_PLAYER]->SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMiniMap::Uninit(void)
{
	// 開放処理 (sceneのポインタ配列から削除）
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CMiniMap::Update(void)
{
	//
	for (int nCntScene = 0; nCntScene < MINIMAP_DRAW_MAX; nCntScene++) {
		if (nCntScene == IDX_PLAYER) {
			D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();
			m_apScene[IDX_PLAYER]->SetPos((playerPos / MINIMAP_SIZE_DEVIDE) + m_pos);
			m_apScene[IDX_PLAYER]->UpdateVertex();
		}
		else if (nCntScene != IDX_MAP) {
			m_apScene[nCntScene]->SetDrawFlag(false);
		}
	}

	vector<CScene*> *sceneVc = CScene::GetSceneList(ENEMY_DRAW_PRIORITY);
	for (auto itr = sceneVc->begin(); itr != sceneVc->end(); itr++) {
		if ((*itr)->GetObjType() == CScene::OBJTYPE_ENEMY) {
			AddToMap((*itr)->GetPos() / MINIMAP_SIZE_DEVIDE + m_pos, D3DXCOLOR(1.0f, 0, 0, 1));
		}
		else if ((*itr)->GetObjType() == CScene::OBJTYPE_ORB) {
			AddToMap((*itr)->GetPos() / MINIMAP_SIZE_DEVIDE + m_pos, D3DXCOLOR(ORB_COLOR));
		}
		else if ((*itr)->GetObjType() == CScene::OBJTYPE_BOSS) {
			AddToMap((*itr)->GetPos() / MINIMAP_SIZE_DEVIDE + m_pos, D3DXCOLOR(0.8f, 0, 0, 1));
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CMiniMap::Draw(void)
{

}

//=============================================================================
// 生成関数
//=============================================================================
CMiniMap *CMiniMap::Create(void)
{
	// 生成、初期化
	CMiniMap *pWave;
	pWave = new CMiniMap;
	if (pWave != NULL)
	{
		pWave->Init(MINIMAP_POSITION, VECTOR3_ZERO, MINIMAP_SIZE);
	}

	return pWave;
}

//=============================================================================
// マップに表示させる
//=============================================================================
void CMiniMap::AddToMap(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	for (int nCntScene = 0; nCntScene < MINIMAP_DRAW_MAX; nCntScene++) {
		if (m_apScene[nCntScene]->GetDrawFlag() == false) {
			m_apScene[nCntScene]->SetPos(pos);
			m_apScene[nCntScene]->SetColor(col);
			m_apScene[nCntScene]->SetDrawFlag(true);
			m_apScene[nCntScene]->UpdateVertex();
			break;
		}
	}
}