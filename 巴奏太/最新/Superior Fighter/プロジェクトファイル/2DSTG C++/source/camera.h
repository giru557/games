//=============================================================================
//
// 疑似カメラ処理 [camera.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//*****************************************************************************
// ヘッダーのインクルード
//*****************************************************************************
#include "main.h"

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
	void Draw(void);

	void Shake(float fMagnitude, int nFrame);
	void Shake(bool stop);

	static D3DXVECTOR3 WorldToScreenPosition(D3DXVECTOR3 worldPos);	// 座標を画面の座標に変換
	static void SetCameraCenter(D3DXVECTOR3 centerPos);				// カメラの位置を設定
	static D3DXVECTOR3 GetCameraCenter(void) { return m_pos; }		// カメラ位置を取得
private:
	// 揺れのパラメーター
	typedef struct {
		bool bActive;		// カメラの揺れフラグ
		int nFrames;		// 揺れ時間
		int nCounter;		// フレームカウント用
		float fMagnitude;	// 揺れの強さ
	} SHAKE_PARAM;

	SHAKE_PARAM m_shake;			// 揺れのパラメーター

	static D3DXVECTOR3 m_pos;		// カメラ位置
	static D3DXVECTOR3 m_posDest;	// カメラの目標位置

	void CheckViewArea(void);	// カメラ表示範囲の判定
	void ShakeProc(void);		// 画面揺れ処理
};

#endif