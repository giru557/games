//-----------------------------------------------------------------------------
//
// カメラの処理 ヘッダー [camera.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//-----------------------------------------------------------------------------
// カメラ構造体
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 posV;															// 視点
	D3DXVECTOR3 posVDest;														// 目的地視点
	D3DXVECTOR3 posR;															// 注視点
	D3DXVECTOR3 posRDest;														// 目的地注視点
	D3DXVECTOR3 vecU;															// 上方向ベクトル
	D3DXVECTOR3 rot;															// カメラの向き
	D3DXMATRIX mtxProjection;													// プロジェクションマトリックス
	D3DXMATRIX mtxView;															// ビューマトリックス
}Camera;

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);
Camera *GetCamera(void);

#endif