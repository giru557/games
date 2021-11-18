//=============================================================================
//
// カメラ3D [camera3d.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _CAMERA3D_H_
#define _CAMERA3D_H_

//*****************************************************************************
// ヘッダーのインクルード
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
// カメラクラス
//*****************************************************************************
class CCamera3D
{
public:
	CCamera3D();
	~CCamera3D();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(LPDIRECT3DDEVICE9 pDevice);

	D3DXVECTOR3 GetRot(void) { return m_rot; }

private:
	D3DXVECTOR3 m_posV;	// 視点
	D3DXVECTOR3 m_posR;	// 注視点
	D3DXVECTOR3 m_vecU;	// 上方向ベクトル
	D3DXVECTOR3 m_rot;	// 向き
	D3DXMATRIX m_mtxProjection;		// プロジェクションマトリックス
	D3DXMATRIX m_mtxView;			// ビューマトリックス
};

#endif

