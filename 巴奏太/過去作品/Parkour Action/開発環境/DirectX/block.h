//-----------------------------------------------------------------------------
//
// ブロックのヘッダー [block.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _BLOCK_H_
#define _BLOCK_H_
#include "main.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define BLOCK_MAX (1024)

//-----------------------------------------------------------------------------
// ブロックのテクスチャ種類
//-----------------------------------------------------------------------------
typedef enum
{
	BLOCKTEXTURE_NORMAL = 0,	// 通常ブロック
	BLOCKTEXTURE_TOP,			// 上側ブロック
	BLOCKTEXTURE_TOPRIGHT,		// 上側、右端ブロック
	BLOCKTEXTURE_TOPLEFT,		// 上側、左端ブロック
	BLOCKTEXTURE_RIGHT,			// 右端ブロック
	BLOCKTEXTURE_LEFT,			// 左端ブロック
	BLOCKTEXTURE_MAX
} BLOCKTEXTURE;

//-----------------------------------------------------------------------------
// ブロックの構造体
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 firstpos;	// 初期位置
	D3DXVECTOR3 move;		// 移動
	D3DXVECTOR3 scroll;		// スクロール
	BLOCKTEXTURE texture;	// ブロックのテクスチャ
	float fWidth;			// 幅
	float fHeight;			// 高さ
	float fMoveTime;
	float fMoveTimeFirst;
	float fMoveSpeed;
	int nTypeNum;			// ブロック番号
	bool bReturn;
	bool bUse;				// 使用しているか
} BLOCK;

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
HRESULT InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight, int nTypeNum, BLOCKTEXTURE texture, float fMoveTime, float fMoveSpeed);
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight);
BLOCK *GetBlock(void);

#endif