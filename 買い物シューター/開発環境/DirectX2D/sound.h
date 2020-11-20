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
	SOUND_LABEL_BGM000 = 0,		// BGM0
	SOUND_LABEL_BGM001,			// BGM1
	SOUND_LABEL_BGM002,			// BGM2
	SOUND_LABEL_BGM003,			// BGM3 通常時
	SOUND_LABEL_BGM004,			// BGM4 ボス
	SOUND_LABEL_BGM_TITLE,		// タイトルBGM
	SOUND_LABEL_BGM_RESULT,		// リザルトBGM
	SOUND_LABEL_SE_SHOT,		// 弾発射音
	SOUND_LABEL_SE_HIT,			// ヒット音
	SOUND_LABEL_SE_HIT2,		// ヒット音
	SOUND_LABEL_SE_HIT3,		// ヒット音 (プレイヤー)
	SOUND_LABEL_SE_EXPLOSION,	// 爆発音
	SOUND_LABEL_SE_LASER,		// レーザー
	SOUND_LABEL_SE_LASER2,		// レーザー
	SOUND_LABEL_SE_LASER3,		// レーザー
	SOUND_LABEL_SE_SELECT,		// 選択
	SOUND_LABEL_SE_SELECT2,		// 選択 (決定)
	SOUND_LABEL_SE_SELECT3,		// 選択 (ポーズ)
	SOUND_LABLE_SE_SELECT4,		// 選択 (ポーズ解除)
	SOUND_LABEL_SE_MISSILE,		// ミサイル発射
	SOUND_LABEL_SE_DEAD,		// 自機死亡
	SOUND_LABEL_SE_STAGECOMP,	// ステージコンプリート
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
