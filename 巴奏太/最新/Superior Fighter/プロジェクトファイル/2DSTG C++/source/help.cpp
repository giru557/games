//=============================================================================
//
// ヘルプ表示クラス [help.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "help.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "textureloader.h"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//*****************************************************************************
// ヘルプ表示クラス ( 派生元: オブジェクトクラス (scene) )
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CHelp::CHelp()
{
	m_bHelpVisible = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CHelp::~CHelp()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CHelp::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// 2Dオブジェクトを生成
	for (int nCntScene = 0; nCntScene < HELP_SCENES; nCntScene++){
		m_pScene2D[nCntScene] = CScene2D::Create(pos, rot, size);
		m_pScene2D[nCntScene]->SetConvertState(false);
		m_pScene2D[nCntScene]->SetPriority(HELP_DRAW_PRIORITY);
	}

	m_pScene2D[0]->SetPos(HELP_OPEN_POSIITON);
	m_pScene2D[0]->SetDrawFlag(true);
	m_pScene2D[0]->SetSize(HELP_OPEN_SIZE);
	m_pScene2D[0]->BindTexture(CManager::GetTextureLoad()->m_TextureMp["HELPOPEN"]);

	m_pScene2D[1]->SetPos(HELP_POSIITON);
	m_pScene2D[1]->SetDrawFlag(false);
	m_pScene2D[1]->SetSize(HELP_SIZE);
	m_pScene2D[1]->BindTexture(CManager::GetTextureLoad()->m_TextureMp["HELP"]);

	SwitchVisible();
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CHelp::Uninit(void)
{
	// 開放処理 (sceneのポインタ配列から削除）
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CHelp::Update(void)
{
	// 頂点更新
	for (int nCntScene = 0; nCntScene < HELP_SCENES; nCntScene++) {
		m_pScene2D[nCntScene]->UpdateVertex();
	}

	CInputKeyboard *pKey = CManager::GetInputKeyboard();

	// キー入力で表示・非表示切り替え
	if (pKey->GetTrigger(DIK_H)) {
		SwitchVisible();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CHelp::Draw(void)
{
	
}

//=============================================================================
// 生成関数
//=============================================================================
CHelp *CHelp::Create(void)
{
	// 生成、初期化
	CHelp *pWave;
	pWave = new CHelp;
	if (pWave != NULL)
	{
		pWave->Init(VECTOR3_ZERO, VECTOR3_ZERO, (D3DXVECTOR2)VECTOR3_ZERO);
	}

	return pWave;
}

//=============================================================================
// 表示状態を入れ替える
//=============================================================================
void CHelp::SwitchVisible()
{
	m_bHelpVisible = !m_bHelpVisible;

	if (m_bHelpVisible) {
		m_pScene2D[0]->SetDrawFlag(false);
		m_pScene2D[1]->SetDrawFlag(true);
	}
	else {
		m_pScene2D[0]->SetDrawFlag(true);
		m_pScene2D[1]->SetDrawFlag(false);
	}
}