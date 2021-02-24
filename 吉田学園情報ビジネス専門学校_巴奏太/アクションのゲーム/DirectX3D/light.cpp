//-----------------------------------------------------------------------------
//
// ライトの処理 [light.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "light.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define LIGHT_MAX (3)															// ライトの数

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
D3DLIGHT9 g_light[LIGHT_MAX];		// ライトの情報

//-----------------------------------------------------------------------------
// ライトの初期化処理
//-----------------------------------------------------------------------------
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice;		// デバイスへのポインタ
	D3DXVECTOR3 vecDir;				// 設定用方向ベクトル

	//デバイス取得
	pDevice = GetDevice();

	//ライトをクリアする
	ZeroMemory(&g_light, sizeof(D3DLIGHT9));

	for (int nCntLight = 0; nCntLight < LIGHT_MAX; nCntLight++)
	{
		//ライトの設定
		switch (nCntLight)
		{
		case 0:	// ライト1
			//ライトの種類を選択
			g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			//ライトの拡散光を設定
			g_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//ライトの方向を設定
			vecDir = D3DXVECTOR3(0.2f, -0.8f, -0.4f);	// 左奥から手前に向かう方向
			D3DXVec3Normalize(&vecDir, &vecDir);		// 正規化する
			g_light[nCntLight].Direction = vecDir;
			break;

		case 1:	// ライト2
			//ライトの種類を選択
			g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			//ライトの拡散光を設定
			g_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);

			//ライトの方向を設定
			vecDir = D3DXVECTOR3(0.0, -1.0f, 0.0f);		// 上から下に向かう方向 
			D3DXVec3Normalize(&vecDir, &vecDir);		// 正規化する
			g_light[nCntLight].Direction = vecDir;
			break;

		case 2: // ライト3
			//ライトの種類を選択
			g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			//ライトの拡散光を設定
			g_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

			//ライトの方向を設定
			vecDir = D3DXVECTOR3(-0.5f, 0.0f, 0.4f);
			D3DXVec3Normalize(&vecDir, &vecDir);		// 正規化する
			g_light[nCntLight].Direction = vecDir;
			break;

		default:
			break;
		}

		//ライトを設定する
		pDevice->SetLight(nCntLight, &g_light[nCntLight]);

		//ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//-----------------------------------------------------------------------------
// ライトの終了処理
//-----------------------------------------------------------------------------
void UninitLight(void)
{

}

//-----------------------------------------------------------------------------
// ライトの更新処理
//-----------------------------------------------------------------------------
void UpdateLight(void)
{

}

