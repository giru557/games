//-----------------------------------------------------------------------------
//
// �Q�[�� �w�b�_�[ [game.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

//�v���g�^�C�v�錾
HRESULT InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetPause(bool bPause);

#endif