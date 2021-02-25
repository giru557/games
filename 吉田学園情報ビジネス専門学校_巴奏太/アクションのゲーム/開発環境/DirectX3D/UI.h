//-----------------------------------------------------------------------------
//
// UI ヘッダー [UI.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _UI_H_
#define _UI_H_
#include "main.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define HEALTH_OBJ_AMOUNT (2)			// オブジェ数
#define HEALTH_FRONT_POS_X (0)			// HPバーの位置 X (前)
#define HEALTH_FRONT_POS_Y (0)			// HPバーの位置 Y (前)
#define HEALTH_BACK_POS_X (0)			// HPバーの位置 X (後)
#define HEALTH_BACK_POS_Y (0)			// HPバーの位置 Y (後)
#define HEALTH_BAR_WIDTH (66 / 2 * HEALTH_SIZE_MULTIPLIER)
#define HEALTH_BAR_HEIGHT (19 / 2 * HEALTH_SIZE_MULTIPLIER)
#define HEALTH_WIDTH (51 * HEALTH_SIZE_MULTIPLIER)
#define HEALTH_HEIGHT (19 / 2 * HEALTH_SIZE_MULTIPLIER)
#define HEALTH_SIZE_MULTIPLIER (5)

#define ENEMYUI_OBJ_AMOUNT (2)					// オブジェクト数
#define ENEMYUI_SHOWDIGIT (2)					// 表示する数字の数
#define ENEMYUI_TEXTURE_SPLIT_NUM (10)			// 数字テクスチャの分割数
#define ENEMYUI_SPACE_BETWEEN_CHAR (100)			// 数字間のスぺース
#define ENEMYUI_CHAR_WIDTH (100)				// 数字の幅
#define ENEMYUI_CHAR_HEIGHT (150)				// 数字の高さ
#define ENEMYUI_CHAR_POS_X (SCREENWIDTH / 2)	// 数字の位置X
#define ENEMYUI_CHAR_POS_Y (0)					// 数字の位置Y
#define ENEMYUI_PICTURE_WIDTH (200)				// 画像の幅
#define ENEMYUI_PICTURE_HEIGHT (100)			// 画像の高さ
#define ENEMYUI_PICTURE_POS_X (0)				// 画像の位置X
#define ENEMYUI_PICTURE_POS_Y (0)				// 画像の位置Y

//-----------------------------------------------------------------------------
// 体力バーの構造体
//-----------------------------------------------------------------------------
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;
	LPDIRECT3DTEXTURE9 pTexture;
	D3DXVECTOR3 pos;
} HPBAR;

//-----------------------------------------------------------------------------
// ゲームオーバーの構造体
//-----------------------------------------------------------------------------
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff[2];
	LPDIRECT3DTEXTURE9 pTexture[3];
	D3DXVECTOR3 pos;
	bool bGameOver;
	bool bDraw;
} GAMEOVER;

//-----------------------------------------------------------------------------
// 敵の残量の構造体
//-----------------------------------------------------------------------------
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff[2];
	LPDIRECT3DTEXTURE9 pTexture[2];
	D3DXVECTOR3 pos[2];
	int nAmount;
}EnemyUI;

//-----------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------
HRESULT InitUI(void);
void UninitUI(void);
void UpdateUI(void);
void DrawUI(void);
HPBAR *GetHPBAR(void);
GAMEOVER *GetGameOver(void);

#endif

