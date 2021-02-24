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
#define HEALTH_FRONT_POS_X (0)
#define HEALTH_FRONT_POS_Y (0)
#define HEALTH_BACK_POS_X (0)
#define HEALTH_BACK_POS_Y (0)

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
//プロトタイプ宣言
//-----------------------------------------------------------------------------
HRESULT InitUI(void);
void UninitUI(void);
void UpdateUI(void);
void DrawUI(void);
HPBAR *GetHPBAR(void);
GAMEOVER *GetGameOver(void);

#endif

