//-----------------------------------------------------------------------------
//
// �G�t�F�N�g �w�b�_�[ [effect.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _EFFECT_H_
#define _EFFECT_H_
#include "main.h"

//-----------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------
HRESULT InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR effCol, int nSize, float fAlphaShift);

#endif
