//=============================================================================
//
// 疑似カメラの処理 [camera.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "manager.h"
#include "game.h"
#include "player.h"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
D3DXVECTOR3 CCamera::m_pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);
D3DXVECTOR3 CCamera::m_posDest = CCamera::m_pos;

//*****************************************************************************
// カメラクラス
//*****************************************************************************


//=============================================================================
// コンストラクタ
//=============================================================================
CCamera::CCamera()
{
	m_shake.bActive = false;
	m_shake.nFrames = 0;
	m_shake.nCounter = 0;
	m_shake.fMagnitude = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
// 初期化
//=============================================================================
void CCamera::Init(void)
{

}

//=============================================================================
// 終了
//=============================================================================
void CCamera::Uninit(void)
{

}

//=============================================================================
// 更新
//=============================================================================
void CCamera::Update(void)
{
	// 表示範囲制限
	this->CheckViewArea();

	// 画面揺れ
	this->ShakeProc();
}

//=============================================================================
// 描画
//=============================================================================
void CCamera::Draw(void)
{

}

//=============================================================================
// ワールド座標からスクリーン座標に変換
//=============================================================================
D3DXVECTOR3 CCamera::WorldToScreenPosition(D3DXVECTOR3 WorldPos)
{
	// スクリーン座標の原点
	D3DXVECTOR3 screenOrigin = D3DXVECTOR3(m_pos.x - SCREEN_WIDTH / 2.0f, m_pos.y - SCREEN_HEIGHT / 2.0f, 0.0f);

	// ワールド座標からスクリーン座標に
	D3DXVECTOR3 screenPos = D3DXVECTOR3(WorldPos.x - screenOrigin.x, WorldPos.y - screenOrigin.y, 0.0f);

	return screenPos;
}

//=============================================================================
// カメラの中心位置を設定
//=============================================================================
void CCamera::SetCameraCenter(D3DXVECTOR3 centerPos)
{
	m_posDest = centerPos;
}

//=============================================================================
// カメラ表示範囲の判定
//=============================================================================
void CCamera::CheckViewArea(void)
{
	// 画面外判定 X軸
	if (m_posDest.x - SCREEN_WIDTH / 2 <= 0.0f)
	{
		m_posDest.x = SCREEN_WIDTH / 2;
	}
	else if (m_posDest.x + SCREEN_WIDTH / 2 >= GAME_FIELD_WIDTH)
	{
		m_posDest.x = GAME_FIELD_WIDTH - SCREEN_WIDTH / 2;
	}

	// 画面外判定 Y軸
	if (m_posDest.y - SCREEN_HEIGHT / 2 <= 0.0f)
	{
		m_posDest.y = SCREEN_HEIGHT / 2;
	}
	else if (m_posDest.y + SCREEN_HEIGHT / 2 >= GAME_FIELD_HEIGHT)
	{
		m_posDest.y = GAME_FIELD_HEIGHT - SCREEN_HEIGHT / 2;
	}
}

//=============================================================================
// 画面揺れ処理
//=============================================================================
void CCamera::ShakeProc(void)
{
	// 揺らすフラグが立ってる時
	if (m_shake.bActive)
	{
		m_shake.nCounter++;	// フレームをカウントする

		// ランダム方向にカメラをずらす
		float fRandAngle = D3DXToRadian(rand() % 360);
		D3DXVECTOR3 randVec = D3DXVECTOR3(sinf(fRandAngle) * m_shake.fMagnitude, cosf(fRandAngle) * m_shake.fMagnitude, 0.0f);
		m_pos = m_posDest + randVec;

		// 指定フレーム数を超えたらフラグ下ろす
		if (m_shake.nCounter >= m_shake.nFrames) m_shake.bActive = false;
	}
	else
	{
		// ずれたままにならないように戻す
		m_pos = m_posDest;
		this->Shake(false);
	}
}

//=============================================================================
// 画面揺れ呼び出し
//=============================================================================
void CCamera::Shake(float fMagnitude, int nFrame)
{
	if (!m_shake.bActive)
	{
		m_shake.bActive = true;
		m_shake.fMagnitude = fMagnitude;
		m_shake.nFrames = nFrame;
	}
}

void CCamera::Shake(bool stop)
{
	if (!stop)
	{
		m_shake.bActive = false;
		m_shake.fMagnitude = 0.0f;
		m_shake.nCounter = 0;
		m_shake.nFrames = 0;
	}
}