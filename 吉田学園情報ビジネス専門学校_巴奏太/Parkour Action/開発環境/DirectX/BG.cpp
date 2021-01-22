//-----------------------------------------------------------------------------
//
// �w�i�̏��� [BG.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "BG.h"
#include "player.h"

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
#define BG_TEX (2)
#define BG_POLYGON (1)
#define BG_MOUNTAIN_WIDTH (1900)
#define BG_MOUNTAIN_HEIGHT (900)
#define BG_SKY_WIDTH (SCREEN_WIDTH / 2)
#define BG_SKY_HEIGHT (SCREEN_HEIGHT / 2)
#define BG_SCROLL_SPEED (0.0005f)

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffBG = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureBG[BG_TEX] = { NULL };				// �|���S���̃e�N�X�`���ւ̃|�C���^
BG g_aBG[BG_POLYGON];											// BG�̏��

//-----------------------------------------------------------------------------
// �w�i�̏���������
//-----------------------------------------------------------------------------
HRESULT InitBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	g_aBG[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	g_aBG[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - BG_MOUNTAIN_HEIGHT, 0.0f);

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Background.png", &g_pTextureBG[0]);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * BG_TEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBG, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntBG = 0; nCntBG < BG_POLYGON; nCntBG++)
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

		switch (nCntBG)
		{
		case 0:
			//���_���W D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(g_aBG[nCntBG].pos.x - BG_MOUNTAIN_WIDTH, g_aBG[nCntBG].pos.y + BG_MOUNTAIN_HEIGHT, 0.0f);
			pVertex[1].pos = D3DXVECTOR3(g_aBG[nCntBG].pos.x - BG_MOUNTAIN_WIDTH, g_aBG[nCntBG].pos.y - BG_MOUNTAIN_HEIGHT, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_aBG[nCntBG].pos.x + BG_MOUNTAIN_WIDTH, g_aBG[nCntBG].pos.y + BG_MOUNTAIN_HEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_aBG[nCntBG].pos.x + BG_MOUNTAIN_WIDTH, g_aBG[nCntBG].pos.y - BG_MOUNTAIN_HEIGHT, 0.0f);
			break;
		}

		//���_�o�b�t�@���炷
		pVertex += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBG->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
// �w�i�̏I������
//-----------------------------------------------------------------------------
void UninitBG(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffBG != NULL)
	{
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG = NULL;
	}

	for (int nCntBG = 0; nCntBG < BG_TEX; nCntBG++)
	{
		//�e�N�X�`���̊J��
		if (g_pTextureBG[nCntBG] != NULL)
		{
			g_pTextureBG[nCntBG]->Release();
			g_pTextureBG[nCntBG] = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
// �w�i�̍X�V����
//-----------------------------------------------------------------------------
void UpdateBG(void)
{
	VERTEX_2D *pVertex;
	PLAYER *pPlayer;
	float fScrollSpeed = BG_SCROLL_SPEED;
	pPlayer = GetPlayer();

	if (pPlayer->move.x > 1)
	{
		g_aBG[0].scroll.x += fScrollSpeed;
	}
	else if (pPlayer->move.x < -1)
	{
		g_aBG[0].scroll.x -= fScrollSpeed;
	}
	else
	{
		g_aBG[0].scroll.x += 0;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntBG = 0; nCntBG < BG_POLYGON; nCntBG++)
	{
		switch (nCntBG)
		{
		case 0:
				//���_���̐ݒ�
				pVertex[0].tex = D3DXVECTOR2(0.0f + g_aBG[nCntBG].scroll.x, 1.0f);
				pVertex[1].tex = D3DXVECTOR2(0.0f + g_aBG[nCntBG].scroll.x, 0.0f);
				pVertex[2].tex = D3DXVECTOR2(1.0f + g_aBG[nCntBG].scroll.x, 1.0f);
				pVertex[3].tex = D3DXVECTOR2(1.0f + g_aBG[nCntBG].scroll.x, 0.0f);
			break;

		default:
			break;
		}
		pVertex += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBG->Unlock();
}

//-----------------------------------------------------------------------------
// �w�i�̕`�揈��
//-----------------------------------------------------------------------------
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBG = 0; nCntBG < BG_POLYGON; nCntBG++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureBG[nCntBG]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBG * 4, 2);
	}
}