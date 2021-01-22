//-----------------------------------------------------------------------------
//
// DirectX2Dアクションゲームのヘッダーファイル [main.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _MAIN_H_
#define _MAIN_H_
#include <windows.h>
#include "d3dx9.h"						//描画処理に必要
#define DIRECTINPUT_VERSION (0x0800)
#include "dinput.h"						//入力処理に必要
#include "xaudio2.h"					//サウンド再生に必要


//-----------------------------------------------------------------------------
// ライブラリのリンク
//-----------------------------------------------------------------------------
#pragma comment(lib, "d3d9.lib")	//描画処理に必要
#pragma comment(lib, "d3dx9.lib")	//拡張ライブラリ
#pragma comment(lib, "dxguid.lib")	//コンポーネント使用に必要
#pragma comment(lib, "dinput8.lib") //入力処理に必要
#pragma comment(lib, "winmm.lib")	//時刻の表示に必要

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)  //頂点フォーマット
#define VERTEX_AMOUNT (4)											  //頂点の数
#define SCREEN_WIDTH  (1600)				//スクリーンの幅
#define SCREEN_HEIGHT (900)					//スクリーンの高さ
#define CLASS_NAME "WindowClass"			//ウィンドウクラスの名前
#define WINDOW_NAME "WindowsApp"			//ウィンドウの名前
#define WINDOW_POS_X (1)					//ウィンドウ左上のX座標
#define WINDOW_POS_Y (1)					//ウィンドウ左上のY座標
#define MAP_TILE_ROW (18)
#define MAP_TILE_COLUMN (157)
#define MAP_MAXCHAR (1024)

//-----------------------------------------------------------------------------
// 構造体宣言
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	//頂点座標
	float rhw;			//1.0で固定
	D3DCOLOR col;		//頂点カラー
	D3DXVECTOR2 tex;	//テクスチャ座標
}VERTEX_2D;

//-----------------------------------------------------------------------------
// ステージ
//-----------------------------------------------------------------------------
typedef enum
{
	STAGE_1 = 0,	// ステージ1
	STAGE_2,		// ステージ2
	STAGE_MAX
} STAGE;

//-----------------------------------------------------------------------------
// モードの種類
//-----------------------------------------------------------------------------
typedef enum
{
	MODE_TITLE = 0,	//タイトル画面
	MODE_GAME,		//ゲーム画面
	MODE_TUTORIAL,  //チュートリアル画面
	MODE_RESULT,	//リザルト画面
	MODE_RANKING,	//ランキング画面
	MODE_MAX
} MODE;

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
MODE GetMode(void);
void LoadMapData(STAGE stage);

#endif