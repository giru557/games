//-----------------------------------------------------------------------------
//
// ���� �w�b�_�[ [money.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _MONEY_H_
#define _MONEY_H_
#include "main.h"

//-----------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------
HRESULT InitMoney(void);
void UninitMoney(void);
void UpdateMoney(void);
void DrawMoney(void);
void AddMoney(int nValue);
bool UseMoney(int nValue);

#endif
