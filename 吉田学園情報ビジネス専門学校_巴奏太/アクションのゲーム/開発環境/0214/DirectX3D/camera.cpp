//-----------------------------------------------------------------------------
//
// カメラの処理 [camera.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "camera.h"
#include "input.h"
#include "model.h"
#include "player.h"
#include "field.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define CAMERA_DISTANCE (175.0f)													// 視点と注視点までの距離
#define CAMERA_SPEED (0.05f)														// カメラの旋回速度

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
Camera g_camera;		// カメラの情報

//-----------------------------------------------------------------------------
// カメラの初期化処理
//-----------------------------------------------------------------------------
void InitCamera(void)
{
	g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// カメラの向き
	g_camera.posV = D3DXVECTOR3(0.0f, 125.0f, 0.0f);				// 視点
	g_camera.posR = D3DXVECTOR3(g_camera.posV.x + sinf(g_camera.rot.x) * CAMERA_DISTANCE, 30.0f, g_camera.posV.z + cosf(g_camera.rot.z) * CAMERA_DISTANCE);					// 注視点
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);					// 上向きベクトル
}

//-----------------------------------------------------------------------------
// カメラの終了処理
//-----------------------------------------------------------------------------
void UninitCamera(void)
{

}

//-----------------------------------------------------------------------------
// カメラの更新処理
//-----------------------------------------------------------------------------
void UpdateCamera(void)
{
	Model *pModel = GetModel();
	Player *pPlayer = GetPlayer();
	D3DXVECTOR3 Diff;

	//カメラの回転にマウスの移動量を足す
	g_camera.rot.y += GetMouseVelocity().x / 550;
	g_camera.rot.z -= GetMouseVelocity().y / 550;

	//視点　左旋回
	if (GetKeyboardPress(DIK_Z) == true)
	{
		//視点の位置移動
		g_camera.rot.y += CAMERA_SPEED;

		//視点の固定、g_camera.rotの向きに
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * CAMERA_DISTANCE;
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * CAMERA_DISTANCE;
	}
	//視点　右旋回
	else if (GetKeyboardPress(DIK_C) == true)
	{
		g_camera.rot.y -= CAMERA_SPEED;

		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * CAMERA_DISTANCE;
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * CAMERA_DISTANCE;
	}

	//----------------------縦回転バージョン(3次元極座標(球座標))---------------------------------
	//注視点をモデルに
	g_camera.posRDest.x = pPlayer->pos.x + sinf(pPlayer->rot.y + D3DX_PI) * 10;
	g_camera.posRDest.z = pPlayer->pos.z + cosf(pPlayer->rot.y + D3DX_PI) * 10;
	g_camera.posR.y = pPlayer->pos.y + 30.0f;

	//視点をモデルからカメラの向きに長さ分だけ
	g_camera.posV.x = pPlayer->pos.x - sinf(g_camera.rot.y) * cosf(g_camera.rot.z) * CAMERA_DISTANCE;
	g_camera.posV.z = pPlayer->pos.z - cosf(g_camera.rot.y) * cosf(g_camera.rot.z) * CAMERA_DISTANCE;
	g_camera.posV.y = pPlayer->pos.y - sinf(g_camera.rot.z) * CAMERA_DISTANCE;
	//---------------------------------------------------------------------

	//--------------------------縦回転なし----------------------------------
	////注視点をモデルに
	//g_camera.posRDest.x = pPlayer->pos.x + sinf(pPlayer->rot.y + D3DX_PI) * 10;
	//g_camera.posRDest.z = pPlayer->pos.z + cosf(pPlayer->rot.y + D3DX_PI) * 10;
	//g_camera.posR.y = pPlayer->pos.y + 30.0f;

	////視点をモデルからカメラの向きに長さ分だけ
	//g_camera.posV.x = pPlayer->pos.x - sinf(g_camera.rot.y) * CAMERA_DISTANCE;
	//g_camera.posV.z = pPlayer->pos.z - cosf(g_camera.rot.y) * CAMERA_DISTANCE;
	//g_camera.posV.y = pPlayer->pos.y + 125.0f;
	//---------------------------------------------------------------------

	//差分計算
	Diff.x = g_camera.posRDest.x - g_camera.posR.x;
	Diff.z = g_camera.posRDest.z - g_camera.posR.z;

	g_camera.posR.x += Diff.x * 0.15f;
	g_camera.posR.z += Diff.z * 0.15f;

	////注視点　右旋回
	//if (GetKeyboardPress(DIK_E) == true)
	//{
	//	g_camera.rot.y += CAMERA_SPEED;
	//	g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * CAMERA_DISTANCE;
	//	g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * CAMERA_DISTANCE;
	//}
	////注視点　左旋回
	//else if (GetKeyboardPress(DIK_Q) == true)
	//{
	//	//向きの移動
	//	g_camera.rot.y -= CAMERA_SPEED;

	//	//注視点の位置固定、g_camera.rotの向きに
	//	g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * CAMERA_DISTANCE;
	//	g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * CAMERA_DISTANCE;
	//}
	////注視点　上昇
	//else if (GetKeyboardPress(DIK_T) == true)
	//{
	//	g_camera.posR.y += CAMERA_SPEED * 10;
	//}
	////注視点　下降
	//else if (GetKeyboardPress(DIK_B) == true)
	//{
	//	g_camera.posR.y -= CAMERA_SPEED * 10;
	//}
	//視点　上昇
	if (GetKeyboardPress(DIK_Y) == true)
	{
		g_camera.posV.y += CAMERA_SPEED * 10;
	}
	//視点　下降
	else if (GetKeyboardPress(DIK_N) == true)
	{
		g_camera.posV.y -= CAMERA_SPEED * 10;
	}

	//回転がπを超えた際の対処
	if (g_camera.rot.y > D3DX_PI) g_camera.rot.y -= D3DX_PI * 2.0f;
	else if (g_camera.rot.y < -D3DX_PI) g_camera.rot.y += D3DX_PI * 2.0f;

	//縦回転に制限を設ける
	if (g_camera.rot.z > D3DX_PI / 2) g_camera.rot.z = D3DX_PI / 2;
	else if (g_camera.rot.z < -D3DX_PI / 2) g_camera.rot.z = -D3DX_PI / 2;

}

//-----------------------------------------------------------------------------
// カメラの設定
//-----------------------------------------------------------------------------
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	//プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(
		&g_camera.mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		2000.0f);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(
		&g_camera.mtxView,
		&g_camera.posV,
		&g_camera.posR,
		&g_camera.vecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

//-----------------------------------------------------------------------------
// カメラの情報取得
//-----------------------------------------------------------------------------
Camera *GetCamera(void)
{
	return &g_camera;
}