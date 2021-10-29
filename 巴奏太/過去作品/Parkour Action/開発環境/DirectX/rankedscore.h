//-----------------------------------------------------------------------------
//
// �����L���O�X�R�A �w�b�_�[[rankedscore.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _RANKEDSCORE_H_
#define _RANKEDSCORE_H_
#include "main.h"

//-----------------------------------------------------------------------------
// RSCORE�\����
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
} RSCORE;

typedef struct
{
	int score;
	int hour;
	int minute;
	int second;
} SCORE;

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
HRESULT InitRScore(void);
void UninitRScore(void);
void UpdateRScore(void);
void DrawRScore(void);

#endif

