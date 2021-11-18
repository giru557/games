//=============================================================================
//
// カメラ [camera.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "manager.h"

//*****************************************************************************
// カメラクラス
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CCamera::CCamera()
{

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
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);

	m_fCamDist = CAMERA_DISTANCE;
	m_fCamSpd = CAMERA_SPEED;
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
	// キーボード、マウス取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CInputMouse *pMouse = CManager::GetInputMouse();

	// カメラ移動
	//if (pKeyboard->GetPress(DIK_W))
	//{
	//	if (pKeyboard->GetPress(DIK_D))
	//		MoveR(45.0f);
	//	else if (pKeyboard->GetPress(DIK_A))
	//		MoveR(-45.0f);
	//	else
	//		MoveR(0.0f);
	//}
	//else if (pKeyboard->GetPress(DIK_S))
	//{
	//	if (pKeyboard->GetPress(DIK_D))
	//		MoveR(135.0f);
	//	else if (pKeyboard->GetPress(DIK_A))
	//		MoveR(-135.0f);
	//	else
	//		MoveR(-180.0f);
	//}
	//else if (pKeyboard->GetPress(DIK_A))
	//	MoveR(-90.0f);
	//else if (pKeyboard->GetPress(DIK_D))
	//	MoveR(90.0f);
	//if (pKeyboard->GetPress(DIK_SPACE))
	//	m_posR.y += m_fCamSpd;
	//else if (pKeyboard->GetPress(DIK_LCONTROL))
	//	m_posR.y -= m_fCamSpd;



	// 視点移動
	D3DXVECTOR2 mouseVel = pMouse->GetVelocity() / 300;
	m_rot.y += mouseVel.x;
	m_rot.z += mouseVel.y;

	// 角度制限
	if (m_rot.z > 1.2f) m_rot.z = 1.2f;
	if (m_rot.z < -1.2f) m_rot.z = -1.2f;

	// 視点位置を三角関数で設定
	m_posV.x = m_posR.x + sinf(m_rot.y) * cosf(m_rot.z) * m_fCamDist;
	m_posV.z = m_posR.z + cosf(m_rot.y) * cosf(m_rot.z) * m_fCamDist;
	m_posV.y = m_posR.y + (m_rot.z) * m_fCamDist;
}

//=============================================================================
// カメラを設定
//=============================================================================
void CCamera::SetCamera(LPDIRECT3DDEVICE9 pDevice)
{
	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスを生成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection, D3DXToRadian(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 10.0f, 5000.0f);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスの生成
	D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//=============================================================================
// カメラ移動
//=============================================================================
void CCamera::MoveR(float fAngDeg)
{
	m_posR.x += sinf(m_rot.y + D3DX_PI + D3DXToRadian(fAngDeg)) * m_fCamSpd;
	m_posR.z += cosf(m_rot.y + D3DX_PI + D3DXToRadian(fAngDeg)) * m_fCamSpd;
}