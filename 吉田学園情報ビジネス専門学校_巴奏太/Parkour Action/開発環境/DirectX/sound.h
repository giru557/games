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
	SOUND_LABEL_SE_SELECT,		// �I��
	SOUND_LABEL_SE_SELECT2,		// �I�� (����)
	SOUND_LABEL_SE_SELECT3,		// �I�� (�|�[�Y)
	SOUND_LABLE_SE_SELECT4,		// �I�� (�|�[�Y����)
	SOUND_LABEL_SE_DIE_PLAYER,	// ���S�T�E���h (�v���C���[)
	SOUND_LABEL_SE_DIE_ENEMY,	// ���S�T�E���h (�G)
	SOUND_LABEL_SE_JUMP,		// �W�����v
	SOUND_LABEL_SE_ONWALL,		// �ǒ���t��
	SOUND_LABEL_SE_SLIDE,		// �X���C�f�B���O
	SOUND_LABEL_SE_LAND,		// ���n
	SOUND_LABEL_BGM_OP,			// BGM �I�[�v�j���O
	SOUND_LABEL_BGM_RESULT,		// BGM ���U���g
	SOUND_LABEL_BGM_RANKING,	// BGM �����L���O
	SOUND_LABEL_BGM_STAGE1,		// BGM �X�e�[�W1
	SOUND_LABEL_BGM_STAGE2,		// BGM �X�e�[�W2

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
