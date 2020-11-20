//-----------------------------------------------------------------------------
//
// �^�C�g���̏��� [title.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "title.h"
#include "input.h"
#include "fade.h"

//�}�N��
#define BG_POLYGON (3) //�e�N�X�`���̖���
#define BG_TITLE_WIDTH (933 / 2)	//�^�C�g�����S�̉���
#define BG_TITLE_HEIGHT (112 / 2)	//�^�C�g�����S�̍���
#define BG_PENTER_WIDTH (857 / 2)		//PRESSENTER�̉���
#define BG_PENTER_HEIGHT (72 / 2)		//PRESSENTER�̍���

//�v���g�^�C�v�錾
void ReturnShrink(int nIdx);
void ReturnBlink(int nIdx);

//-----------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffTitle = NULL;				//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureTitle[BG_POLYGON] = { NULL };		//�|���S���̃e�N�X�`���ւ̃|�C���^
TITLE g_Title[BG_POLYGON];											//TITLE�̏��
TITLESTATE g_TitleState[BG_POLYGON];								//�^�C�g���̏��

//-----------------------------------------------------------------------------
//�^�C�g���̏���������
//-----------------------------------------------------------------------------
HRESULT InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	//�ʒu������
	g_Title[1].pos = D3DXVECTOR3(640, 128, 0.0f);
	g_Title[2].pos = D3DXVECTOR3(640, 580, 0.0f);

	//TITLE������
	for (int nCntT = 0; nCntT < BG_POLYGON; nCntT++)
	{
		g_Title[nCntT].fBlink = 1.0f;
		g_Title[nCntT].fShrink = 1.0f;

		g_TitleState[nCntT] = TITLESTATE_NORMAL;
	}

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\space_bg.png", &g_pTextureTitle[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\title_logo.png", &g_pTextureTitle[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\press_enter.png", &g_pTextureTitle[2]);


	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * BG_POLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTitle, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntT = 0; nCntT < BG_POLYGON; nCntT++)
	{
		switch (nCntT)
		{
		case 0:
			//���_���W D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);		 //TRIANGLESTRIP�Ŏl�p
			pVertex[1].pos = D3DXVECTOR3(0, 0, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
			break;

		case 1:
			//���_���W D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(g_Title[nCntT].pos.x - BG_TITLE_WIDTH, g_Title[nCntT].pos.y + BG_TITLE_HEIGHT, 0.0f);		 //TRIANGLESTRIP�Ŏl�p
			pVertex[1].pos = D3DXVECTOR3(g_Title[nCntT].pos.x - BG_TITLE_WIDTH, g_Title[nCntT].pos.y - BG_TITLE_HEIGHT, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_Title[nCntT].pos.x + BG_TITLE_WIDTH, g_Title[nCntT].pos.y + BG_TITLE_HEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_Title[nCntT].pos.x + BG_TITLE_WIDTH, g_Title[nCntT].pos.y - BG_TITLE_HEIGHT, 0.0f);

			g_TitleState[nCntT] = TITLESTATE_SHRINK;
			break;

		case 2:
			//���_���W D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(g_Title[nCntT].pos.x - BG_PENTER_WIDTH, g_Title[nCntT].pos.y + BG_PENTER_HEIGHT, 0.0f);		 //TRIANGLESTRIP�Ŏl�p
			pVertex[1].pos = D3DXVECTOR3(g_Title[nCntT].pos.x - BG_PENTER_WIDTH, g_Title[nCntT].pos.y - BG_PENTER_HEIGHT, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_Title[nCntT].pos.x + BG_PENTER_WIDTH, g_Title[nCntT].pos.y + BG_PENTER_HEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_Title[nCntT].pos.x + BG_PENTER_WIDTH, g_Title[nCntT].pos.y - BG_PENTER_HEIGHT, 0.0f);

			g_TitleState[nCntT] = TITLESTATE_BLINK;
			break;

		default:
			break;
		}


		//rhw�̐ݒ�
		pVertex[0].rhw = 1.0f;	//1.0f�Œ�
		pVertex[1].rhw = 1.0f;
		pVertex[2].rhw = 1.0f;
		pVertex[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVertex[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);  //D3DCOLOR_RGBA(��, ��, ��, �����x);
		pVertex[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVertex[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVertex[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//���_���̐ݒ�
		pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVertex += 4; //���_���炵
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitle->Unlock();

	SetFade(FADE_IN, MODE_TITLE);
	return S_OK;
}

//-----------------------------------------------------------------------------
//�^�C�g���̏I������
//-----------------------------------------------------------------------------
void UninitTitle(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}

	for (int nCntTex = 0; nCntTex < BG_POLYGON; nCntTex++)
	{
		//�e�N�X�`���̊J��
		if (g_pTextureTitle[nCntTex] != NULL)
		{
			g_pTextureTitle[nCntTex]->Release();
			g_pTextureTitle[nCntTex] = NULL;
		}
	}

}

//-----------------------------------------------------------------------------
//�^�C�g���̍X�V����
//-----------------------------------------------------------------------------
void UpdateTitle(void)
{
	VERTEX_2D *pVertex;

	if (g_pVtxBuffTitle != NULL)
	{
		//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
		g_pVtxBuffTitle->Lock(0, 0, (void**)&pVertex, 0);

		for (int nCntT = 0; nCntT < BG_POLYGON; nCntT++)
		{
			switch (g_TitleState[nCntT])
			{
			case TITLESTATE_NORMAL:
				break;

			case TITLESTATE_SHRINK:
				ReturnShrink(nCntT);
				break;

			case TITLESTATE_BLINK:
				ReturnBlink(nCntT);

			default:
				break;
			}

			switch (nCntT)
			{
			case 1:
				//���_���W D3DXVECTOR3(X,Y, 0.0f);
				//pVertex[0].pos = D3DXVECTOR3(g_Title[nCntT].pos.x - (BG_TITLE_WIDTH), g_Title[nCntT].pos.y + (BG_TITLE_HEIGHT), 0.0f);		 //TRIANGLESTRIP�Ŏl�p
				//pVertex[1].pos = D3DXVECTOR3(g_Title[nCntT].pos.x - (BG_TITLE_WIDTH), g_Title[nCntT].pos.y - (BG_TITLE_HEIGHT), 0.0f);
				//pVertex[2].pos = D3DXVECTOR3(g_Title[nCntT].pos.x + (BG_TITLE_WIDTH), g_Title[nCntT].pos.y + (BG_TITLE_HEIGHT), 0.0f);
				//pVertex[3].pos = D3DXVECTOR3(g_Title[nCntT].pos.x + (BG_TITLE_WIDTH), g_Title[nCntT].pos.y - (BG_TITLE_HEIGHT), 0.0f);

				pVertex[0].pos = D3DXVECTOR3(g_Title[nCntT].pos.x - (BG_TITLE_WIDTH * g_Title[nCntT].fShrink), g_Title[nCntT].pos.y + (BG_TITLE_HEIGHT * g_Title[nCntT].fShrink), 0.0f);		 //TRIANGLESTRIP�Ŏl�p
				pVertex[1].pos = D3DXVECTOR3(g_Title[nCntT].pos.x - (BG_TITLE_WIDTH * g_Title[nCntT].fShrink), g_Title[nCntT].pos.y - (BG_TITLE_HEIGHT * g_Title[nCntT].fShrink), 0.0f);
				pVertex[2].pos = D3DXVECTOR3(g_Title[nCntT].pos.x + (BG_TITLE_WIDTH * g_Title[nCntT].fShrink), g_Title[nCntT].pos.y + (BG_TITLE_HEIGHT * g_Title[nCntT].fShrink), 0.0f);
				pVertex[3].pos = D3DXVECTOR3(g_Title[nCntT].pos.x + (BG_TITLE_WIDTH * g_Title[nCntT].fShrink), g_Title[nCntT].pos.y - (BG_TITLE_HEIGHT * g_Title[nCntT].fShrink), 0.0f);

			case 2:
				pVertex[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Title[nCntT].fBlink);
				pVertex[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Title[nCntT].fBlink);
				pVertex[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Title[nCntT].fBlink);
				pVertex[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Title[nCntT].fBlink);
				break;

			default:
				break;
			}

			pVertex += 4;
		}

		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffTitle->Unlock();
	}

	if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_BACKSPACE) == true)
	{
		SetFade(FADE_OUT, MODE_GAME);
	}
}

//-----------------------------------------------------------------------------
//�^�C�g���̕`�揈��
//-----------------------------------------------------------------------------
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (int nCntTex = 0; nCntTex < BG_POLYGON; nCntTex++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTitle[nCntTex]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTex * 4, 4);
	}
}

void ReturnShrink(int nIdx)
{
	static bool bShrink = true;
	static int bCounter = 0;
	bCounter++;

	if (bCounter > 100)
	{
		bShrink = false;
	}
	if (bCounter > 200)
	{
		bShrink = true;
		bCounter = 0;
	}

	if (bShrink == true)
	{
		g_Title[nIdx].fShrink -= 0.0004f;
	}
	else
	{
		g_Title[nIdx].fShrink += 0.0004f;
	}
}

void ReturnBlink(int nIdx)
{
	static bool bBlink = true;
	static int bCounter = 0;
	bCounter++;

	if (bCounter > 50)
	{
		bBlink = false;
	}
	if (bCounter > 80)
	{
		bBlink = true;
	}
	if (bCounter > 120)
	{
		bCounter = 0;
	}

	if (bBlink == true)
	{
		g_Title[nIdx].fBlink = 1.0f;
	}
	else
	{
		g_Title[nIdx].fBlink = 0.0f;
	}
}