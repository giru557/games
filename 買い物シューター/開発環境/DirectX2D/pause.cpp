//-----------------------------------------------------------------------------
//
// �|�[�Y�̏��� [pause.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "pause.h"
#include "input.h"
#include "game.h"
#include "fade.h"

//-----------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------
#define POLYGON_AMOUNT (5)			//�|���S���̐�

#define PAUSE_MENU_WIDTH (200)		//�|�[�Y���j���[�w�i�̕�
#define PAUSE_MENU_HEIGHT (300)		//�|�[�Y���j���[�w�i�̍���
#define PAUSE_BUTTON_WIDTH (150)	//�|�[�Y���j���[�{�^���̕�
#define PAUSE_BUTTON_HEIGHT (50)	//�|�[�Y���j���[�{�^���̍���

//-----------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffPause = NULL;  //���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTexturePause[5] = { NULL };		//�|���S���̃e�N�X�`���ւ̃|�C���^
D3DXVECTOR3 g_posPolygon[5];					//�|���S���̈ʒu
PAUSE_MENU g_statePause;						//�|�[�Y�̏��
int g_nValuePause;								//�|�[�Y�̑I������

//-----------------------------------------------------------------------------
//�|�[�Y�̏���������
//-----------------------------------------------------------------------------
HRESULT InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	g_posPolygon[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_posPolygon[1] = D3DXVECTOR3(640, 360, 0.0f);
	g_posPolygon[2] = D3DXVECTOR3(640, 250, 0.0f);
	g_posPolygon[3] = D3DXVECTOR3(640, 400, 0.0f);
	g_posPolygon[4] = D3DXVECTOR3(640, 550, 0.0f);

	g_nValuePause = 0;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, NULL, &g_pTexturePause[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\pause100.png", &g_pTexturePause[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\pause000.png", &g_pTexturePause[2]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\pause001.png", &g_pTexturePause[3]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\pause002.png", &g_pTexturePause[4]);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * POLYGON_AMOUNT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPause, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntPause = 0; nCntPause < POLYGON_AMOUNT; nCntPause++)
	{
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

		switch (nCntPause)
		{
		case 0:
			//���_���W D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);		 //TRIANGLESTRIP�Ŏl�p
			pVertex[1].pos = D3DXVECTOR3(0, 0, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

			//���_�J���[�̐ݒ�
			pVertex[0].col = D3DCOLOR_RGBA(0, 0, 0, 100);  //D3DCOLOR_RGBA(��, ��, ��, �����x);
			pVertex[1].col = D3DCOLOR_RGBA(0, 0, 0, 100);
			pVertex[2].col = D3DCOLOR_RGBA(0, 0, 0, 100);
			pVertex[3].col = D3DCOLOR_RGBA(0, 0, 0, 100);
			break;

		case 1:
			//���_���W�̍X�V
			pVertex[0].pos = D3DXVECTOR3(g_posPolygon[nCntPause].x - PAUSE_MENU_WIDTH, g_posPolygon[nCntPause].y + PAUSE_MENU_HEIGHT, 0.0f);
			pVertex[1].pos = D3DXVECTOR3(g_posPolygon[nCntPause].x - PAUSE_MENU_WIDTH, g_posPolygon[nCntPause].y - PAUSE_MENU_HEIGHT, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_posPolygon[nCntPause].x + PAUSE_MENU_WIDTH, g_posPolygon[nCntPause].y + PAUSE_MENU_HEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_posPolygon[nCntPause].x + PAUSE_MENU_WIDTH, g_posPolygon[nCntPause].y - PAUSE_MENU_HEIGHT, 0.0f);

			//���_�J���[�̐ݒ�
			pVertex[0].col = D3DCOLOR_RGBA(255, 255, 255, 128);  //D3DCOLOR_RGBA(��, ��, ��, �����x);
			pVertex[1].col = D3DCOLOR_RGBA(255, 255, 255, 128);
			pVertex[2].col = D3DCOLOR_RGBA(255, 255, 255, 128);
			pVertex[3].col = D3DCOLOR_RGBA(255, 255, 255, 128);

			break;

		default:
			//���_���W�̍X�V
			pVertex[0].pos = D3DXVECTOR3(g_posPolygon[nCntPause].x - PAUSE_BUTTON_WIDTH, g_posPolygon[nCntPause].y + PAUSE_BUTTON_HEIGHT, 0.0f);
			pVertex[1].pos = D3DXVECTOR3(g_posPolygon[nCntPause].x - PAUSE_BUTTON_WIDTH, g_posPolygon[nCntPause].y - PAUSE_BUTTON_HEIGHT, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_posPolygon[nCntPause].x + PAUSE_BUTTON_WIDTH, g_posPolygon[nCntPause].y + PAUSE_BUTTON_HEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_posPolygon[nCntPause].x + PAUSE_BUTTON_WIDTH, g_posPolygon[nCntPause].y - PAUSE_BUTTON_HEIGHT, 0.0f);

			//���_�J���[�̐ݒ�
			pVertex[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);  //D3DCOLOR_RGBA(��, ��, ��, �����x);
			pVertex[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVertex[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVertex[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			break;
		}

		pVertex += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPause->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//�|�[�Y�̏I������
//-----------------------------------------------------------------------------
void UninitPause(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}

	//�e�N�X�`���̊J��
	for (int nCntP = 0; nCntP < POLYGON_AMOUNT; nCntP++)
	{
		if (g_pTexturePause[nCntP] != NULL)
		{
			g_pTexturePause[nCntP]->Release();
			g_pTexturePause[nCntP] = NULL;
		}
	}

}

//-----------------------------------------------------------------------------
//�|�[�Y�̍X�V����
//-----------------------------------------------------------------------------
void UpdatePause(void)
{
	VERTEX_2D *pVertex;

	if (GetKeyboardTrigger(DIK_S) == true)
	{
		g_nValuePause = (g_nValuePause + 1) % 3; //3�܂ł����Ƃ��܂�0���������ă��Z�b�g
	}
	else if (GetKeyboardTrigger(DIK_W) == true)
	{
		g_nValuePause--;

		if (g_nValuePause < 0)
		{
			g_nValuePause += 3;
		}
	}

	switch (g_nValuePause)
	{
	case 0:
		g_statePause = PAUSE_MENU_CONTINUE;
		break;

	case 1:
		g_statePause = PAUSE_MENU_RETRY;
		break;

	case 2:
		g_statePause = PAUSE_MENU_QUIT;
		break;

	default:
		break;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntPause = 0; nCntPause < POLYGON_AMOUNT; nCntPause++)
	{
		switch (nCntPause)
		{
		case 2:
			if (g_statePause == PAUSE_MENU_CONTINUE)
			{
				//���_�J���[�̐ݒ�
				pVertex[0].col = D3DCOLOR_RGBA(0, 255, 0, 255);  //D3DCOLOR_RGBA(��, ��, ��, �����x);
				pVertex[1].col = D3DCOLOR_RGBA(0, 255, 0, 255);
				pVertex[2].col = D3DCOLOR_RGBA(0, 255, 0, 255);
				pVertex[3].col = D3DCOLOR_RGBA(0, 255, 0, 255);
			}
			else
			{
				//���_�J���[�̐ݒ�
				pVertex[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);  //D3DCOLOR_RGBA(��, ��, ��, �����x);
				pVertex[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVertex[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVertex[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			}
			break;

		case 3:
			if (g_statePause == PAUSE_MENU_RETRY)
			{
				//���_�J���[�̐ݒ�
				pVertex[0].col = D3DCOLOR_RGBA(0, 255, 0, 255);  //D3DCOLOR_RGBA(��, ��, ��, �����x);
				pVertex[1].col = D3DCOLOR_RGBA(0, 255, 0, 255);
				pVertex[2].col = D3DCOLOR_RGBA(0, 255, 0, 255);
				pVertex[3].col = D3DCOLOR_RGBA(0, 255, 0, 255);
			}
			else
			{
				//���_�J���[�̐ݒ�
				pVertex[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);  //D3DCOLOR_RGBA(��, ��, ��, �����x);
				pVertex[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVertex[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVertex[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			}
			break;

		case 4:
			if (g_statePause == PAUSE_MENU_QUIT)
			{
				//���_�J���[�̐ݒ�
				pVertex[0].col = D3DCOLOR_RGBA(0, 255, 0, 255);  //D3DCOLOR_RGBA(��, ��, ��, �����x);
				pVertex[1].col = D3DCOLOR_RGBA(0, 255, 0, 255);
				pVertex[2].col = D3DCOLOR_RGBA(0, 255, 0, 255);
				pVertex[3].col = D3DCOLOR_RGBA(0, 255, 0, 255);
			}
			else
			{
				//���_�J���[�̐ݒ�
				pVertex[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);  //D3DCOLOR_RGBA(��, ��, ��, �����x);
				pVertex[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVertex[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVertex[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			}
			break;

		default:
			break;
		}
		
		//���_�|�C���^���炵
		pVertex += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPause->Unlock();

	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		switch (g_nValuePause)
		{
		case 0:
			SetPause(false);
			break;

		case 1:
			SetFade(FADE_OUT, MODE_GAME);
			break;

		case 2:
			SetFade(FADE_OUT, MODE_TITLE);
			break;

		default:
			break;
		}
	}
}

//-----------------------------------------------------------------------------
//�|�[�Y�̕`�揈��
//-----------------------------------------------------------------------------
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);



	for (int nCntPause = 0; nCntPause < POLYGON_AMOUNT; nCntPause++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePause[nCntPause]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * 4, 2);
	}
}