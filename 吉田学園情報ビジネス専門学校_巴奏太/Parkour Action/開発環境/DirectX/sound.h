//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_SE_SELECT,		// 選択
	SOUND_LABEL_SE_SELECT2,		// 選択 (決定)
	SOUND_LABEL_SE_SELECT3,		// 選択 (ポーズ)
	SOUND_LABLE_SE_SELECT4,		// 選択 (ポーズ解除)
	SOUND_LABEL_SE_DIE_PLAYER,	// 死亡サウンド (プレイヤー)
	SOUND_LABEL_SE_DIE_ENEMY,	// 死亡サウンド (敵)
	SOUND_LABEL_SE_JUMP,		// ジャンプ
	SOUND_LABEL_SE_ONWALL,		// 壁張り付き
	SOUND_LABEL_SE_SLIDE,		// スライディング
	SOUND_LABEL_SE_LAND,		// 着地
	SOUND_LABEL_BGM_OP,			// BGM オープニング
	SOUND_LABEL_BGM_RESULT,		// BGM リザルト
	SOUND_LABEL_BGM_RANKING,	// BGM ランキング
	SOUND_LABEL_BGM_STAGE1,		// BGM ステージ1
	SOUND_LABEL_BGM_STAGE2,		// BGM ステージ2

	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
