//=============================================================================
//
// サウンドの処理 [sound.h]
// Author: Sota Tomoe, Akira Tanaka
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//*****************************************************************************
// ヘッダーのインクルード
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// サウンドクラス (基本)
//*****************************************************************************
class CSound
{
public:
	CSound();
	~CSound();

	// サウンドファイル
	typedef enum
	{
		SOUND_LABEL_SE_LASER00 = 0,
		SOUND_LABEL_SE_EXPLOSION00,
		SOUND_LABEL_SE_DASH00,
		SOUND_LABEL_SE_DASH01,
		SOUND_LABEL_SE_DASH02,
		SOUND_LABEL_SE_HIT00,
		SOUND_LABEL_SE_HIT03,
		SOUND_LABEL_SE_IMPACT00,
		SOUND_LABEL_SE_FORCEFIELD,
		SOUND_LABEL_SE_BEAM,
		SOUND_LABEL_BGM_TITLE,
		SOUND_LABEL_BGM_INGAME,
		SOUND_LABEL_BGM_GAMEOVER,
		SOUND_LABEL_BGM_RESULT,
		SOUND_LABEL_BGM_RANKING,

		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT Play(SOUND_LABEL label);
	void Stop(SOUND_LABEL label);
	void Stop(void);

private:
	// パラメータ構造体
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	} SOUNDPARAM;

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunk(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice;			// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイス
	BYTE *m_apDataAudio[SOUND_LABEL_MAX] = {};					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// オーディオデータサイズ

	static SOUNDPARAM m_aParam[SOUND_LABEL_MAX];
};
#endif
