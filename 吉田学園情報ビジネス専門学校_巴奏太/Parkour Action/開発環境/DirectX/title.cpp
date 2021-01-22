//-----------------------------------------------------------------------------
//
// �^�C�g���̏��� [title.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "title.h"
#include "input.h"
#include "fade.h"

//-----------------------------------------------------------------------------
// �}�N��
//-----------------------------------------------------------------------------
#define TITLE_POLYGON (3)															//�^�C�g���̔z�u�I�u�W�F�N�g��

#define TITLE_LOGOSIZE_WIDTH (1076)													//�^�C�g�����S�̕�
#define TITLE_LOGOSIZE_HEIGHT (260)													//����
#define TITLE_LOGOPOS_X (SCREEN_WIDTH / 2)											//�^�C�g�����S�̈ʒuX
#define TITLE_LOGOPOS_Y (200)														//					Y

#define TITLE_PRESSENTER_WIDTH (822)
#define TITLE_PRESSENTER_HEIGHT (84)
#define TITLE_PRESSENTER_POS_X (SCREEN_WIDTH / 2)
#define TITLE_PRESSENTER_POS_Y (700)

#define TITLE_SHRINKSPD (0.0005f)													//�k���g��̑��x
#define TITLE_SHRINKRANGE (0.9f)													//�ǂ��܂ŏk�����邩 (0.0f ~ 1.0f)
#define TITLE_BLINKTIME (1.5f)															//�_�ŊԊu (�b)
#define TITLE_BLINKTIMEFIXED (TITLE_BLINKTIME * 1000)								//�b����~���b�ɕϊ�

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
float TitleShrink(float fShrink);
float TitleBlink(float fBlink);

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffTitle = NULL;					//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureTitle[TITLE_POLYGON] = { NULL };		//�|���S���̃e�N�X�`���ւ̃|�C���^
TITLE g_aTitle[TITLE_POLYGON];										//TITLE�\���̂̏��

//-----------------------------------------------------------------------------
// �^�C�g���̏���������
//-----------------------------------------------------------------------------
HRESULT InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	//TITLE�̏�����
	g_aTitle[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aTitle[1].pos = D3DXVECTOR3(TITLE_LOGOPOS_X, TITLE_LOGOPOS_Y, 0.0f);
	g_aTitle[2].pos = D3DXVECTOR3(TITLE_PRESSENTER_POS_X, TITLE_PRESSENTER_POS_Y, 0.0f);
	for (int nCntTITLE = 0; nCntTITLE < TITLE_POLYGON; nCntTITLE++)
	{
		g_aTitle[nCntTITLE].fShrink = 1.0f;
		g_aTitle[nCntTITLE].fBlink = 1.0f;
	}

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\bg_op.png", &g_pTextureTitle[0]);
	D3DXCreateTextureFromFileEx(pDevice, "data\\TEXTURE\\bg_op_title.png", TITLE_LOGOSIZE_WIDTH, TITLE_LOGOSIZE_HEIGHT, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0xFF000000, NULL, NULL, &g_pTextureTitle[1]);
	D3DXCreateTextureFromFileEx(pDevice, "data\\TEXTURE\\bg_op_pressenter.png", TITLE_PRESSENTER_WIDTH, TITLE_PRESSENTER_HEIGHT, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0xFF000000, NULL, NULL, &g_pTextureTitle[2]);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * TITLE_POLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTitle, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntT = 0; nCntT < TITLE_POLYGON; nCntT++)
	{
		switch (nCntT)
		{
		case 0:
			//���_���W D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(g_aTitle[nCntT].pos.x, g_aTitle[nCntT].pos.y +SCREEN_HEIGHT, 0.0f);
			pVertex[1].pos = D3DXVECTOR3(g_aTitle[nCntT].pos.x, g_aTitle[nCntT].pos.y, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_aTitle[nCntT].pos.x + SCREEN_WIDTH, g_aTitle[nCntT].pos.y + SCREEN_HEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_aTitle[nCntT].pos.x + SCREEN_WIDTH, g_aTitle[nCntT].pos.y, 0.0f);
			break;

		case 1:
			//���_���W D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(g_aTitle[nCntT].pos.x - (TITLE_LOGOSIZE_WIDTH / 2), g_aTitle[nCntT].pos.y + (TITLE_LOGOSIZE_WIDTH / 2), 0.0f);		 //TRIANGLESTRIP�Ŏl�p
			pVertex[1].pos = D3DXVECTOR3(g_aTitle[nCntT].pos.x - (TITLE_LOGOSIZE_WIDTH / 2), g_aTitle[nCntT].pos.y - (TITLE_LOGOSIZE_WIDTH / 2), 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_aTitle[nCntT].pos.x + (TITLE_LOGOSIZE_WIDTH / 2), g_aTitle[nCntT].pos.y + (TITLE_LOGOSIZE_WIDTH / 2), 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_aTitle[nCntT].pos.x + (TITLE_LOGOSIZE_WIDTH / 2), g_aTitle[nCntT].pos.y - (TITLE_LOGOSIZE_WIDTH / 2), 0.0f);
			break;

		case 2:
			//���_���W D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(g_aTitle[nCntT].pos.x - (TITLE_PRESSENTER_WIDTH / 2), g_aTitle[nCntT].pos.y + (TITLE_PRESSENTER_HEIGHT / 2), 0.0f);		 //TRIANGLESTRIP�Ŏl�p
			pVertex[1].pos = D3DXVECTOR3(g_aTitle[nCntT].pos.x - (TITLE_PRESSENTER_WIDTH / 2), g_aTitle[nCntT].pos.y - (TITLE_PRESSENTER_HEIGHT / 2), 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_aTitle[nCntT].pos.x + (TITLE_PRESSENTER_WIDTH / 2), g_aTitle[nCntT].pos.y + (TITLE_PRESSENTER_HEIGHT / 2), 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_aTitle[nCntT].pos.x + (TITLE_PRESSENTER_WIDTH / 2), g_aTitle[nCntT].pos.y - (TITLE_PRESSENTER_HEIGHT / 2), 0.0f);
			break;

		default:
			break;
		}

		//rhw�̐ݒ�
		pVertex[0].rhw = 1.0f;	//1.0f�Œ�
		pVertex[1].rhw = 1.0f;
		pVertex[2].rhw = 1.0f;
		pVertex[3].rhw = 1.0f;

		switch (nCntT)
		{
		case 1:
			//���_�J���[�̐ݒ�
			pVertex[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);  //D3DCOLOR_RGBA(��, ��, ��, �����x);
			pVertex[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVertex[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVertex[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			break;

		default:
			//���_�J���[�̐ݒ�
			pVertex[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);  //D3DCOLOR_RGBA(��, ��, ��, �����x);
			pVertex[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVertex[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVertex[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			break;
		}

		//���_���̐ݒ�
		pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVertex += 4; //���_���炵
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitle->Unlock();

	//�t�F�[�h�C��
	SetFade(FADE_IN, MODE_TITLE);

	return S_OK;
}

//-----------------------------------------------------------------------------
// �^�C�g���̏I������
//-----------------------------------------------------------------------------
void UninitTitle(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}

	for (int nCntTex = 0; nCntTex < TITLE_POLYGON; nCntTex++)
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
// �^�C�g���̍X�V����
//-----------------------------------------------------------------------------
void UpdateTitle(void)
{
	VERTEX_2D *pVertex;
	TITLE *pTitle;

	pTitle = &g_aTitle[0];

	if (g_pVtxBuffTitle != NULL)
	{
		//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
		g_pVtxBuffTitle->Lock(0, 0, (void**)&pVertex, 0);

		for (int nCntT = 0; nCntT < TITLE_POLYGON; nCntT++, pTitle++)
		{
			switch (nCntT)
			{
			case 1:
				//�^�C�g���̉��o
				pTitle->fShrink = TitleShrink(pTitle->fShrink);
				//pTitle->fBlink = TitleBlink(pTitle->fBlink);

				//���_���W D3DXVECTOR3(X,Y, 0.0f);
				pVertex[0].pos = D3DXVECTOR3(pTitle->pos.x - ((TITLE_LOGOSIZE_WIDTH / 2) * pTitle->fShrink), pTitle->pos.y + ((TITLE_LOGOSIZE_HEIGHT / 2) * pTitle->fShrink), 0.0f);		 //TRIANGLESTRIP�Ŏl�p
				pVertex[1].pos = D3DXVECTOR3(pTitle->pos.x - ((TITLE_LOGOSIZE_WIDTH / 2) * pTitle->fShrink), pTitle->pos.y - ((TITLE_LOGOSIZE_HEIGHT / 2) * pTitle->fShrink), 0.0f);
				pVertex[2].pos = D3DXVECTOR3(pTitle->pos.x + ((TITLE_LOGOSIZE_WIDTH / 2) * pTitle->fShrink), pTitle->pos.y + ((TITLE_LOGOSIZE_HEIGHT / 2) * pTitle->fShrink), 0.0f);
				pVertex[3].pos = D3DXVECTOR3(pTitle->pos.x + ((TITLE_LOGOSIZE_WIDTH / 2) * pTitle->fShrink), pTitle->pos.y - ((TITLE_LOGOSIZE_HEIGHT / 2) * pTitle->fShrink), 0.0f);

				//���_�J���[�̐ݒ�
				pVertex[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pTitle->fBlink);  //D3DCOLOR_RGBA(��, ��, ��, �����x);
				pVertex[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pTitle->fBlink);
				pVertex[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pTitle->fBlink);
				pVertex[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pTitle->fBlink);
				break;

			case 2:
				//�^�C�g���̉��o
				//pTitle->fShrink = TitleShrink(pTitle->fShrink);
				pTitle->fBlink = TitleBlink(pTitle->fBlink);

				//���_���W D3DXVECTOR3(X,Y, 0.0f);
				pVertex[0].pos = D3DXVECTOR3(pTitle->pos.x - ((TITLE_PRESSENTER_WIDTH / 2) * pTitle->fShrink), pTitle->pos.y + ((TITLE_PRESSENTER_HEIGHT / 2) * pTitle->fShrink), 0.0f);		 //TRIANGLESTRIP�Ŏl�p
				pVertex[1].pos = D3DXVECTOR3(pTitle->pos.x - ((TITLE_PRESSENTER_WIDTH / 2) * pTitle->fShrink), pTitle->pos.y - ((TITLE_PRESSENTER_HEIGHT / 2) * pTitle->fShrink), 0.0f);
				pVertex[2].pos = D3DXVECTOR3(pTitle->pos.x + ((TITLE_PRESSENTER_WIDTH / 2) * pTitle->fShrink), pTitle->pos.y + ((TITLE_PRESSENTER_HEIGHT / 2) * pTitle->fShrink), 0.0f);
				pVertex[3].pos = D3DXVECTOR3(pTitle->pos.x + ((TITLE_PRESSENTER_WIDTH / 2) * pTitle->fShrink), pTitle->pos.y - ((TITLE_PRESSENTER_HEIGHT / 2) * pTitle->fShrink), 0.0f);

				//���_�J���[�̐ݒ�
				pVertex[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pTitle->fBlink);  //D3DCOLOR_RGBA(��, ��, ��, �����x);
				pVertex[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pTitle->fBlink);
				pVertex[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pTitle->fBlink);
				pVertex[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pTitle->fBlink);
				break;
			}
			pVertex += 4;
		}

		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffTitle->Unlock();
	}

	if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_BACKSPACE) == true)
	{
		SetFade(FADE_OUT, MODE_TUTORIAL);
	}
}

//-----------------------------------------------------------------------------
// �^�C�g���̕`�揈��
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


	for (int nCntTex = 0; nCntTex < TITLE_POLYGON; nCntTex++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTitle[nCntTex]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTex * 4, 2);
	}
}

//-----------------------------------------------------------------------------
// �^�C�g���̉��o (�k���g��)
//-----------------------------------------------------------------------------
float TitleShrink(float fShrink)
{
	static bool bShrink = true;

	//�k���Ɗg��̐؂�ւ�
	if (bShrink == true)
	{
		fShrink -= TITLE_SHRINKSPD;
	}
	else if (bShrink == false)
	{
		fShrink += TITLE_SHRINKSPD;
	}

	//�k���g��̉������
	if (fShrink >= 1.0f)
	{
		bShrink = true;
	}
	else if (fShrink <= TITLE_SHRINKRANGE)
	{
		bShrink = false;
	}

	return fShrink;
}

//-----------------------------------------------------------------------------
// �^�C�g���̉��o (�_��)
//-----------------------------------------------------------------------------
float TitleBlink(float fBlink)
{
	static bool bBlink = false;
	static bool bOnce = false;
	static DWORD dwTime;
	static DWORD dwCTime;

	//���ݎ�
	dwCTime = timeGetTime();

	//�ŏ��̎��Ԃ��L��
	if (bOnce == false)
	{
		dwTime = timeGetTime();
		bOnce = true;
	}

	//�_�Ő؂�ւ�
	if (bBlink == true)
	{
		fBlink = 0.0f;
	}
	else if (bBlink == false)
	{
		fBlink = 1.0f;
	}

	//���ݎ�����ŏ����������Ĕ���
	if ((dwCTime - dwTime) >= TITLE_BLINKTIMEFIXED)
	{
		bBlink = !bBlink;
		dwTime = timeGetTime();
	}

	return fBlink;
}