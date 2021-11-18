#define _CRT_SECURE_NO_WARNINGS
//=============================================================================
//
// メイン処理 [main.h]
// Author : Sota Tomoe
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include <windows.h>
#include <vector>
#include <unordered_map>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800)
#include "dinput.h"
#include "xaudio2.h"

using namespace std;

//*****************************************************************************
// ライブラリファイルのリンク
//*****************************************************************************
#pragma comment(lib, "d3d9.lib")	// 描画処理に必要
#pragma comment(lib, "d3dx9.lib")	// 拡張ライブラリ
#pragma comment(lib, "dxguid.lib")	// コンポーネント使用に必要
#pragma comment(lib, "dinput8.lib")	// 入力処理に必要
#pragma comment(lib, "winmm.lib")	// 時刻の表示に必要

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2)	// 頂点フォーマット3D
#define SCREEN_WIDTH (1280)			// スクリーンの幅	
#define SCREEN_HEIGHT (720)			// スクリーンの高さ
#define SCREEN_CENTER (D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2))	// スクリーンの中心
#define WINDOW_POS_X (1)			// ウィンドウ左上X座標
#define WINDOW_POS_Y (1)			// ウィンドウ左上Y座標
#define VECTOR3_ZERO (D3DXVECTOR3(0,0,0))

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
int GetFPS(void);

#endif