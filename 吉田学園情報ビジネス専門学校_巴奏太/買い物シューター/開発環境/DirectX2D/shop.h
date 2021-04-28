//-----------------------------------------------------------------------------
//
// �V���b�v �w�b�_�[ [shop.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _SHOP_H_
#define _SHOP_H_
#include "main.h"
#include "player.h"

typedef enum
{
	SHOP_PRODUCT_1 = 0,	//�V���b�v�Ŕ�����A�C�e��1
	SHOP_PRODUCT_2,		//2
	SHOP_PRODUCT_3,		//3
	SHOP_PRODUCT_4,		//4
	SHOP_PRODUCT_MAX
} SHOP;

//-----------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------
HRESULT InitShop(void);
void UninitShop(void);
void UpdateShop(void);
void DrawShop(void);
void SetVtx(int nIdx, D3DXVECTOR3 pos, int nWidth, int nHeight, D3DXCOLOR col);
PLAYERWEAPON GetWeapon(void);

#endif
