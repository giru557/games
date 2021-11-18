//=============================================================================
//
// PLAYER [PLAYER.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "player.h"
#include "manager.h"
#include "input.h"
#include "camera.h"

//*****************************************************************************
// PLAYERクラス
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CPlayer::CPlayer()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size)
{
	CSceneX::Init(pos, rot, size);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	CSceneX::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPlayer::Update(void)
{
	D3DXVECTOR3 pos = GetPos();

	// 位置の更新と減衰の設定
	m_move.x += (0 - m_move.x) * 0.3f;
	m_move.y += (0 - m_move.y) * 0.3f;
	m_move.z += (0 - m_move.z) * 0.3f;
	pos += m_move;
	SetPos(pos);

	if (CManager::GetCamera()->GetStickState()) {
		Movement(1.0f);
	}

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_E)) {
		CManager::GetCamera()->StickCamera(this);
	}
	else if (CManager::GetInputKeyboard()->GetTrigger(DIK_Q)) {
		CManager::GetCamera()->StickCamera(NULL);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	CSceneX::Draw();
}

//=============================================================================
// 生成関数
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, char* modelFilePath)
{
	CPlayer *pPlayer;
	pPlayer = new CPlayer;
	if (pPlayer != NULL)
	{
		pPlayer->SetModelFilePath(modelFilePath);
		pPlayer->Init(pos, VECTOR3_ZERO, VECTOR3_ZERO);
	}

	return pPlayer;
}

//=============================================================================
// 移動処理
//=============================================================================
void CPlayer::Movement(float speed)
{
	const float fSpeed = speed;
	D3DXVECTOR3 rot = CManager::GetCamera()->GetRot();
	if (CManager::GetInputKeyboard()->GetPress(DIK_W)) {
		if (CManager::GetInputKeyboard()->GetPress(DIK_D)) {
			m_move += D3DXVECTOR3(sinf(D3DXToRadian(-135) + rot.y), 0, cosf(D3DXToRadian(-135) + rot.y)) * fSpeed;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_A)) {
			m_move += D3DXVECTOR3(sinf(D3DXToRadian(135) + rot.y), 0, cosf(D3DXToRadian(135) + rot.y)) * fSpeed;
		}
		else {
			m_move += D3DXVECTOR3(sinf(D3DX_PI + rot.y), 0, cosf(D3DX_PI + rot.y)) * fSpeed;
		}
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_S)) {
		if (CManager::GetInputKeyboard()->GetPress(DIK_D)) {
			m_move += D3DXVECTOR3(sinf(D3DXToRadian(-45.0f) + rot.y), 0, cosf(D3DXToRadian(-45.0f) + rot.y)) * fSpeed;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_A)) {
			m_move += D3DXVECTOR3(sinf(D3DXToRadian(45.0f) + rot.y), 0, cosf(D3DXToRadian(45.0f) + rot.y)) * fSpeed;
		}
		else {
			m_move += D3DXVECTOR3(sinf(0 + rot.y), 0, cosf(0 + rot.y)) * fSpeed;
		}

	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_D)) {
		m_move += D3DXVECTOR3(sinf(D3DXToRadian(-90) + rot.y), 0, cosf(D3DXToRadian(-90) + rot.y)) * fSpeed;
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_A)) {
		m_move += D3DXVECTOR3(sinf(D3DXToRadian(90) + rot.y), 0, cosf(D3DXToRadian(90) + rot.y)) * fSpeed;
	}

	if (CManager::GetInputKeyboard()->GetPress(DIK_SPACE)) {
		m_move += D3DXVECTOR3(0, fSpeed, 0);
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL)) {
		m_move += D3DXVECTOR3(0, -fSpeed, 0);
	}
}