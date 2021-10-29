//=============================================================================
//
// �T�E���h�̏��� [sound.h]
// Author: Sota Tomoe, Akira Tanaka
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//*****************************************************************************
// �w�b�_�[�̃C���N���[�h
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �T�E���h�N���X (��{)
//*****************************************************************************
class CSound
{
public:
	CSound();
	~CSound();

	// �T�E���h�t�@�C��
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
	// �p�����[�^�\����
	typedef struct
	{
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g
	} SOUNDPARAM;

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunk(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice;			// �}�X�^�[�{�C�X
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX] = {};	// �\�[�X�{�C�X
	BYTE *m_apDataAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^�T�C�Y

	static SOUNDPARAM m_aParam[SOUND_LABEL_MAX];
};
#endif
