//-----------------------------------------------------------------------------
//
// �V���b�v�̏��� [shop.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "shop.h"
#include "input.h"
#include "barrier.h"
#include "money.h"
#include "sound.h"

//-----------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------
#define SHOP_POLYGON (7)
#define SHOP_TEXTURE (7)
#define SHOP_FRAME_WIDTH (400)
#define SHOP_FRAME_HEIGHT (30)
#define SHOP_ICON_WIDTH (35)
#define SHOP_ICON_HEIGHT (25)

//-----------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffShop = NULL;  //���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureShop[SHOP_TEXTURE] = { NULL };		//�|���S���̃e�N�X�`���ւ̃|�C���^
D3DXVECTOR3 g_aPosShop[SHOP_POLYGON];			//�V���b�v�̃|���S����
int g_nProductValue;							//�V���b�v�̑I��p
PLAYERWEAPON g_WeaponSelected;					//�I���ς݂̕���

//-----------------------------------------------------------------------------
//�V���b�v�̏���������
//-----------------------------------------------------------------------------
HRESULT InitShop(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	g_nProductValue = 5;
	g_WeaponSelected = PLAYERWEAPON_NORMAL;

	g_aPosShop[0] = D3DXVECTOR3(640, 670, 0.0f);
	g_aPosShop[1] = D3DXVECTOR3(400, 670, 0.0f);
	g_aPosShop[2] = D3DXVECTOR3(550, 670, 0.0f);
	g_aPosShop[3] = D3DXVECTOR3(700, 670, 0.0f);
	g_aPosShop[4] = D3DXVECTOR3(850, 670, 0.0f);

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\shopui.png", &g_pTextureShop[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\icon_missile.png", &g_pTextureShop[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\icon_minigun.png", &g_pTextureShop[2]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\icon_beam.png", &g_pTextureShop[3]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\icon_barrier.png", &g_pTextureShop[4]);
	//D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\select.png", &g_pTextureShop[5]);
	D3DXCreateTextureFromFileEx(pDevice, "data\\TEXTURE\\select.png", 45, 45, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0xFF000000, NULL, NULL, &g_pTextureShop[5]);
	D3DXCreateTextureFromFileEx(pDevice, "data\\TEXTURE\\equip.png", 12, 12, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0xFF000000, NULL, NULL, &g_pTextureShop[6]);


	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * SHOP_POLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffShop, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffShop->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntShop = 0; nCntShop < SHOP_POLYGON; nCntShop++)
	{
		switch (nCntShop)
		{
		case 0:
			//���_���ݒ�
			SetVtx(nCntShop, g_aPosShop[nCntShop], SHOP_FRAME_WIDTH, SHOP_FRAME_HEIGHT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			break;

		case 1:
			//���_���ݒ�
			SetVtx(nCntShop, g_aPosShop[nCntShop], SHOP_ICON_WIDTH, SHOP_ICON_HEIGHT, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
			break;

		case 2:
			//���_���ݒ�
			SetVtx(nCntShop, g_aPosShop[nCntShop], SHOP_ICON_WIDTH, SHOP_ICON_HEIGHT, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
			break;

		case 3:
			//���_���ݒ�
			SetVtx(nCntShop, g_aPosShop[nCntShop], SHOP_ICON_WIDTH, SHOP_ICON_HEIGHT, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			break;

		case 4:
			//���_���ݒ�
			SetVtx(nCntShop, g_aPosShop[nCntShop], SHOP_ICON_WIDTH, SHOP_ICON_HEIGHT, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
			break;

		default:
			break;
		}

		//rhw�̐ݒ�
		pVertex[0].rhw = 1.0f;	//1.0f�Œ�
		pVertex[1].rhw = 1.0f;
		pVertex[2].rhw = 1.0f;
		pVertex[3].rhw = 1.0f;

		//���_���̐ݒ�
		pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVertex += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffShop->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//�V���b�v�̏I������
//-----------------------------------------------------------------------------
void UninitShop(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffShop != NULL)
	{
		g_pVtxBuffShop->Release();
		g_pVtxBuffShop = NULL;
	}

	//�e�N�X�`���̊J��
	for (int nCntShop = 0; nCntShop < SHOP_TEXTURE; nCntShop++)
	{
		if (g_pTextureShop[nCntShop] != NULL)
		{
			g_pTextureShop[nCntShop]->Release();
			g_pTextureShop[nCntShop] = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
//�V���b�v�̍X�V����
//-----------------------------------------------------------------------------
void UpdateShop(void)
{
	VERTEX_2D *pVertex;

	//if (GetKeyboardTrigger(DIK_RIGHTARROW) == true)
	//{
	//	PlaySound(SOUND_LABEL_SE_SELECT);
	//	g_nProductValue = (g_nProductValue + 1) % 4; //(% �Z�Z) �܂ł����Ƃ��܂�0���������ă��Z�b�g
	//}
	//if (GetKeyboardTrigger(DIK_LEFTARROW) == true)
	//{
	//	PlaySound(SOUND_LABEL_SE_SELECT);
	//	g_nProductValue = (g_nProductValue + 3) % 4; //(% �Z�Z) �܂ł����Ƃ��܂�0���������ă��Z�b�g
	//}

	if (g_pVtxBuffShop != NULL)
	{
		//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
		g_pVtxBuffShop->Lock(0, 0, (void**)&pVertex, 0);

		for (int nCntShop = 0; nCntShop < SHOP_POLYGON; nCntShop++)
		{
			switch (nCntShop)
			{
			case 1:
				if (g_nProductValue == 0)
				{
					SetVtx(5, D3DXVECTOR3(g_aPosShop[nCntShop].x, g_aPosShop[nCntShop].y + 5, 0.0f), 45, 45, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				}
				if (g_WeaponSelected == PLAYERWEAPON_MISSILE)
				{
					SetVtx(6, D3DXVECTOR3(g_aPosShop[nCntShop].x, g_aPosShop[nCntShop].y - 40, 0.0f), 12, 12, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
				break;
				
			case 2:
				if (g_nProductValue == 1)
				{
					SetVtx(5, D3DXVECTOR3(g_aPosShop[nCntShop].x, g_aPosShop[nCntShop].y + 5, 0.0f), 45, 45, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
				if (g_WeaponSelected == PLAYERWEAPON_MINIGUN)
				{
					SetVtx(6, D3DXVECTOR3(g_aPosShop[nCntShop].x, g_aPosShop[nCntShop].y - 40, 0.0f), 12, 12, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
				break;

			//case 3:
			//	if (g_nProductValue == 2)
			//	{
			//		SetVtx(5, D3DXVECTOR3(g_aPosShop[nCntShop].x, g_aPosShop[nCntShop].y + 5, 0.0f), 45, 45, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			//	}
			//	else
			//	{
			//		SetVtx(nCntShop, g_aPosShop[nCntShop], SHOP_ICON_WIDTH, SHOP_ICON_HEIGHT, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			//	}
			//	break;

			case 3:
				if (g_nProductValue == 2)
				{
					SetVtx(5, D3DXVECTOR3(g_aPosShop[nCntShop + 1].x, g_aPosShop[nCntShop + 1].y + 5, 0.0f), 45, 45, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
				if (g_WeaponSelected == PLAYERWEAPON_BARRIER)
				{
					SetVtx(6, D3DXVECTOR3(g_aPosShop[nCntShop + 1].x, g_aPosShop[nCntShop + 1].y - 40, 0.0f), 12, 12, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
				break;

			}

			pVertex += 4;
		}

		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffShop->Unlock();
	}

	//�w���{�^��������
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		if (UseMoney(10) == true)
		{
			PlaySound(SOUND_LABEL_SE_SELECT);
			g_nProductValue = (g_nProductValue + 1) % 3; //(% �Z�Z) �܂ł����Ƃ��܂�0���������ă��Z�b�g
			switch (g_nProductValue)
			{
			case 0:
				//�U�����~�T�C���ɕύX
				if (g_WeaponSelected != PLAYERWEAPON_MISSILE)
				{
					g_WeaponSelected = PLAYERWEAPON_MISSILE;
					if (CheckBarrier() == true)
					{
						RemoveBarrier();
					}

					//�w�����Đ�
					PlaySound(SOUND_LABEL_SE_SELECT2);
				}
				break;

			case 1:
				//�K�g�����O
				if (g_WeaponSelected != PLAYERWEAPON_MINIGUN)
				{
					g_WeaponSelected = PLAYERWEAPON_MINIGUN;
					if (CheckBarrier() == true)
					{
						RemoveBarrier();
					}

					//�w�����Đ�
					PlaySound(SOUND_LABEL_SE_SELECT2);
				}
				break;

			case 2:
				//�U�����o���A�ɕύX
				if (g_WeaponSelected != PLAYERWEAPON_BARRIER)
				{
					g_WeaponSelected = PLAYERWEAPON_BARRIER;
					SetBarrier();

					//�w�����Đ�
					PlaySound(SOUND_LABEL_SE_SELECT2);
				}
				break;

			default:
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//�V���b�v�̕`�揈��
//-----------------------------------------------------------------------------
void DrawShop(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffShop, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntShop = 0; nCntShop < SHOP_POLYGON; nCntShop++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureShop[nCntShop]);
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntShop * 4, 2);
	}

}

//-----------------------------------------------------------------------------
//�J���[�ݒ�
//-----------------------------------------------------------------------------
void SetVtx(int nIdx, D3DXVECTOR3 pos, int nWidth, int nHeight, D3DXCOLOR col)
{
	VERTEX_2D *pVertex;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffShop->Lock(0, 0, (void**)&pVertex, 0);

	pVertex += 4 * nIdx;
	
	//���_���W D3DXVECTOR3(X,Y, 0.0f);
	pVertex[0].pos = D3DXVECTOR3(pos.x - nWidth, pos.y + nHeight, 0.0f);
	pVertex[1].pos = D3DXVECTOR3(pos.x - nWidth, pos.y - nHeight, 0.0f);
	pVertex[2].pos = D3DXVECTOR3(pos.x + nWidth, pos.y + nHeight, 0.0f);
	pVertex[3].pos = D3DXVECTOR3(pos.x + nWidth, pos.y - nHeight, 0.0f);

	//���_�J���[�̐ݒ�
	pVertex[0].col = col;
	pVertex[1].col = col;
	pVertex[2].col = col;
	pVertex[3].col = col;


	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffShop->Unlock();
}

PLAYERWEAPON GetWeapon(void)
{
	return g_WeaponSelected;
}