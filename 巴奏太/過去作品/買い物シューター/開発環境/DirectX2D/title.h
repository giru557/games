//-----------------------------------------------------------------------------
//
// �^�C�g���w�b�_�[ [title.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _TITLE_H_
#define _TITLE_H_
#include "main.h"

//�X�e�[�g
typedef enum
{
	TITLESTATE_NORMAL = 0,
	TITLESTATE_BLINK,
	TITLESTATE_SHRINK,
	TITLESTATE_MAX
} TITLESTATE;

//TITLE�\����
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	float fBlink;	//�_��
	float fShrink;	//�傫���Ȃ����菬�����Ȃ�����
} TITLE;

//-----------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------
HRESULT InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);

#endif