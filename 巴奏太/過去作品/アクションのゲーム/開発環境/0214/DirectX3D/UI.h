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

#define ENEMYUI_OBJ_AMOUNT (2)														// オブジェクト数
#define ENEMYUI_SHOWDIGIT (2)														// 表示する数字の数
#define ENEMYUI_TEXTURE_SPLIT_NUM (10)												// 数字テクスチャの分割数
#define ENEMYUI_SPACE_BETWEEN_CHAR (100)											// 数字間のスぺース
#define ENEMYUI_CHAR_WIDTH ((251 / 10) * ENEMYUI_CHAR_SIZE_MULTIPLIER)				// 数字の幅
#define ENEMYUI_CHAR_HEIGHT (38 * ENEMYUI_CHAR_SIZE_MULTIPLIER)						// 数字の高さ
#define ENEMYUI_CHAR_SIZE_MULTIPLIER (2.5f)											// 数字のサイズ倍率
#define ENEMYUI_CHAR_POS_X (SCREEN_WIDTH / 2 + 50)										// 数字の位置X
#define ENEMYUI_CHAR_POS_Y (5)														// 数字の位置Y
#define ENEMYUI_PICTURE_WIDTH (550 * ENEMYUI_PICTURE_SIZE_MULTIPLIER)				// 画像の幅
#define ENEMYUI_PICTURE_HEIGHT (150 * ENEMYUI_PICTURE_SIZE_MULTIPLIER)				// 画像の高さ
#define ENEMYUI_PICTURE_SIZE_MULTIPLIER (0.7)										// 画像のサイズ倍率
#define ENEMYUI_PICTURE_POS_X (SCREEN_WIDTH / 2 - 250)								// 画像の位置X
#define ENEMYUI_PICTURE_POS_Y (0)													// 画像の位置Y

#define MINUSENEMY_WIDTH (450 * MINUSENEMY_SIZE)
#define MINUSENEMY_HEIGHT (120 * MINUSENEMY_SIZE)
#define MINUSENEMY_SIZE (0.75f)
#define MINUSENEMY_POS_X (SCREEN_WIDTH / 2 + 250)
#define MINUSENEMY_POS_Y (SCREEN_HEIGHT / 2 - 50)
#define MINUSENEMY_AMOUNT_MAX (16)

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
// マイナス表示の構造体
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXMATRIX mtxWorld;										// ワールドマトリックス
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;							// 頂点バッファ
	LPDIRECT3DTEXTURE9 pTexture;								// テクスチャ
	D3DXVECTOR3 pos[MINUSENEMY_AMOUNT_MAX];						// 位置
	bool bUse[MINUSENEMY_AMOUNT_MAX];							// 使用状況
	float fMultiplier[MINUSENEMY_AMOUNT_MAX];					// 透明度の割合
} MinusEnemy;

//-----------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------
HRESULT InitUI(void);
void UninitUI(void);
void UpdateUI(void);
void DrawUI(void);
HPBAR *GetHPBAR(void);
GAMEOVER *GetGameOver(void);
EnemyUI *GetEnemyRemain(void);
void ShowEnemyDestroyed(void);

#endif

