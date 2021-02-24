//-----------------------------------------------------------------------------
//
// 影の処理 ヘッダー [shadow.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _SHADOW_H_
#define _SHADOW_H_
#include "main.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define SHADOW_MAX (256)														// 影の最大数

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
HRESULT InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(D3DXVECTOR3 pos, float fWidth, float fDepth);
void SetPositionShadow(int nIdx, D3DXVECTOR3 pos);
int *GetShadowIdx(void);

#endif

