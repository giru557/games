//-----------------------------------------------------------------------------
//
// クリアタイム ヘッダー [timescore.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _TIMESCORE_H_
#define _TIMESCORE_H_
#include "main.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// TIMESCORE構造体
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;				// 位置
} TIMESCORE;

//-----------------------------------------------------------------------------
// 時間管理構造体
//-----------------------------------------------------------------------------
typedef struct
{
	DWORD dwStartTime;			//開始時間	
	DWORD dwFinishTime;			//終了時間 (ゴールしたとき)
	DWORD dwCurrentTime;			//現在時間
	DWORD dwMinute;					//分換算
	DWORD dwSecond;					//秒換算
	bool bStartFlag;			//スタート時のフラグ管理
} TIME;
//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
HRESULT InitTimeScore(void);
void UninitTimeScore(void);
void UpdateTimeScore(void);
void DrawTimeScore(void);
TIME *GetTimeScore(void);

#endif
