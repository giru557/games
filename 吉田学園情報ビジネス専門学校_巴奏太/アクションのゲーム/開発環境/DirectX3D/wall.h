//-----------------------------------------------------------------------------
//
// 壁の処理 ヘッダー [wall.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _WALL_H_
#define _WALL_H_
#include "main.h"

//-----------------------------------------------------------------------------
// 壁の構造体
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXMATRIX mtxWorld;														// ワールドマトリックス
	D3DXVECTOR3 pos;															// 位置
	D3DXVECTOR3 rot;															// 向き
	LPDIRECT3DVERTEXBUFFER9  pVtx;												// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 pIdxBuff;											// インデックスバッファへのポインタ
	float fWidth;
	float fHeight;
	bool bUse;																	// 使用しているかどうか
} Wall;

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define WALL_MAX (32)															// 壁の数
#define WALL_WIDTH (100.0f)													// 壁の幅
#define WALL_HEIGHT (100.0f)													// 壁の高さ

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
HRESULT InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
Wall *GetWall(void);
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 nor, float fWidth, float fHeight);

#endif

