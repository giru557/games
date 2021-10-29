//-----------------------------------------------------------------------------
//
// バリア ヘッダー [barrier.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _BARRIER_H_
#define _BARRIER_H_
#include "main.h"

//-----------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------
HRESULT InitBarrier(void);
void UninitBarrier(void);
void UpdateBarrier(void);
void DrawBarrier(void);
void SetBarrier(void);
void RemoveBarrier(void);
void BreakBarrier(void);
bool CheckBarrier(void);
bool CheckBarrierActive(void);

#endif