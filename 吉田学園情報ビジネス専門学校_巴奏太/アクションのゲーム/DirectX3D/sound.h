//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_SE_DMGPLAYER,	// �v���C���[�_���[�W��
	SOUND_LABEL_SE_DMGENEMY,	// �G�_���[�W��
	SOUND_LABEL_BGM_TITLE,		// �^�C�g�����BGM
	SOUND_LABEL_BGM_GAME,		// �Q�[����BGM
	SOUND_LABEL_BGM_RESULT,		// ���U���g���BGM

	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
