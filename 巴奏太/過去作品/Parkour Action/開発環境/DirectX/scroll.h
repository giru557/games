//-----------------------------------------------------------------------------
//
// �X�N���[�� �w�b�_�[ [game.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _SCROLL_H_
#define _SCROLL_H_

#include "main.h"

//�v���g�^�C�v�錾
HRESULT InitScroll(void);
void UninitScroll(void);
void UpdateScroll(void);
void DrawScroll(void);
D3DXVECTOR3 *GetScroll(void);

#endif

