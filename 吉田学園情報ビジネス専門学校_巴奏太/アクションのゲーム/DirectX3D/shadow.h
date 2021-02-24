//-----------------------------------------------------------------------------
//
// �e�̏��� �w�b�_�[ [shadow.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _SHADOW_H_
#define _SHADOW_H_
#include "main.h"

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
#define SHADOW_MAX (256)														// �e�̍ő吔

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
HRESULT InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(D3DXVECTOR3 pos, float fWidth, float fDepth);
void SetPositionShadow(int nIdx, D3DXVECTOR3 pos);
int *GetShadowIdx(void);

#endif

