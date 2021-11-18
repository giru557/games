//=============================================================================
//
// カメラ [camera.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//*****************************************************************************
// ヘッダーのインクルード
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CAMERA_DISTANCE (500)
#define CAMERA_SPEED (3.0f)

//*****************************************************************************
// カメラクラス
//*****************************************************************************
class CCamera
{
public:
	CCamera();
	~CCamera();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(LPDIRECT3DDEVICE9 pDevice);

	D3DXVECTOR3 GetRot(void) { return m_rot; }

private:
	void MoveR(float fAngDeg);

	D3DXVECTOR3 m_posV;	// 視点
	D3DXVECTOR3 m_posR;	// 注視点
	D3DXVECTOR3 m_vecU;	// 上方向ベクトル
	D3DXVECTOR3 m_rot;	// 向き
	D3DXMATRIX m_mtxProjection;		// プロジェクションマトリックス
	D3DXMATRIX m_mtxView;			// ビューマトリックス
	float m_fCamDist;	// 注視点から視点までの距離
	float m_fCamSpd;	// カメラ移動速度

};

#endif

