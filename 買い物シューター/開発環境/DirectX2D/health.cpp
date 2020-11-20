//-----------------------------------------------------------------------------
//
// �̗͂̏��� [health.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "health.h"
#include "player.h"

//�}�N��
#define HEALTH_POLYGON (2)
#define HEALTH_BAR_WIDTH (66 / 2 * HEALTH_SIZE_MULTIPLIER)
#define HEALTH_BAR_HEIGHT (19 / 2 * HEALTH_SIZE_MULTIPLIER)
#define HEALTH_WIDTH (51 * HEALTH_SIZE_MULTIPLIER)
#define HEALTH_HEIGHT (19 / 2 * HEALTH_SIZE_MULTIPLIER)
#define HEALTH_SIZE_MULTIPLIER (4)

//-----------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffHealth = NULL;  //���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureHealth[HEALTH_POLYGON] = { NULL };		//�|���S���̃e�N�X�`���ւ̃|�C���^
D3DXVECTOR3 g_aPosHealth[HEALTH_POLYGON];							//�̗͂̈ʒu
int g_nOldLife;

//-----------------------------------------------------------------------------
//�̗͂̏���������
//-----------------------------------------------------------------------------
HRESULT InitHealth(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;
	PLAYER *pPlayer;

	pPlayer = GetPlayer();

	g_aPosHealth[0] = D3DXVECTOR3((370), 600, 0.0f);
	g_aPosHealth[1] = D3DXVECTOR3((268 + 25), 600, 0);
	g_nOldLife = 50;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	//D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\health_bar_decoration.png", &g_pTextureHealth[0]);
	D3DXCreateTextureFromFileEx(pDevice, "data\\TEXTURE\\health_bar_decoration.png", 66, 19, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0xFF000000, NULL, NULL, &g_pTextureHealth[0]);
	D3DXCreateTextureFromFileEx(pDevice, "data\\TEXTURE\\health_bar.png", 51, 19, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0xFF000000, NULL, NULL, &g_pTextureHealth[1]);


	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * HEALTH_POLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffHealth, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffHealth->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntH = 0; nCntH < HEALTH_POLYGON; nCntH++)
	{
		switch (nCntH)
		{
		case 0:
			//���_���W D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(g_aPosHealth[nCntH].x - HEALTH_BAR_WIDTH, g_aPosHealth[nCntH].y + HEALTH_BAR_HEIGHT, 0.0f);		 //TRIANGLESTRIP�Ŏl�p
			pVertex[1].pos = D3DXVECTOR3(g_aPosHealth[nCntH].x - HEALTH_BAR_WIDTH, g_aPosHealth[nCntH].y - HEALTH_BAR_HEIGHT, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_aPosHealth[nCntH].x + HEALTH_BAR_WIDTH, g_aPosHealth[nCntH].y + HEALTH_BAR_HEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_aPosHealth[nCntH].x + HEALTH_BAR_WIDTH, g_aPosHealth[nCntH].y - HEALTH_BAR_HEIGHT, 0.0f);
			break;

		case 1:
			//���_���W D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(g_aPosHealth[nCntH].x - HEALTH_WIDTH, g_aPosHealth[nCntH].y + HEALTH_HEIGHT, 0.0f);		 //TRIANGLESTRIP�Ŏl�p
			pVertex[1].pos = D3DXVECTOR3(g_aPosHealth[nCntH].x - HEALTH_WIDTH, g_aPosHealth[nCntH].y - HEALTH_HEIGHT, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_aPosHealth[nCntH].x + HEALTH_WIDTH, g_aPosHealth[nCntH].y + HEALTH_HEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_aPosHealth[nCntH].x + HEALTH_WIDTH, g_aPosHealth[nCntH].y - HEALTH_HEIGHT, 0.0f);
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

		pVertex += 4; //�|�C���^���炵
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffHealth->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//�̗͂̏I������
//-----------------------------------------------------------------------------
void UninitHealth(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffHealth != NULL)
	{
		g_pVtxBuffHealth->Release();
		g_pVtxBuffHealth = NULL;
	}

	for (int nCntH = 0; nCntH < HEALTH_POLYGON; nCntH++)
	{
		//�e�N�X�`���̊J��
		if (g_pTextureHealth[nCntH] != NULL)
		{
			g_pTextureHealth[nCntH]->Release();
			g_pTextureHealth[nCntH] = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
//�̗͂̍X�V����
//-----------------------------------------------------------------------------
void UpdateHealth(void)
{
	VERTEX_2D *pVertex;
	PLAYER *pPlayer;
	static float fValue = 1.0f;

	pPlayer = GetPlayer();

	fValue = (float)pPlayer->nLife / (float)g_nOldLife;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffHealth->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntH = 0; nCntH < HEALTH_POLYGON; nCntH++)
	{
		switch (nCntH)
		{
		case 1:
			////���_���W D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(g_aPosHealth[nCntH].x, g_aPosHealth[nCntH].y + HEALTH_HEIGHT, 0.0f);		 //TRIANGLESTRIP�Ŏl�p
			pVertex[1].pos = D3DXVECTOR3(g_aPosHealth[nCntH].x, g_aPosHealth[nCntH].y - HEALTH_HEIGHT, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_aPosHealth[nCntH].x + (HEALTH_WIDTH * fValue), g_aPosHealth[nCntH].y + HEALTH_HEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_aPosHealth[nCntH].x + (HEALTH_WIDTH * fValue), g_aPosHealth[nCntH].y - HEALTH_HEIGHT, 0.0f);

			//���_���̐ݒ�
			pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVertex[2].tex = D3DXVECTOR2(1.0f * fValue, 1.0f);
			pVertex[3].tex = D3DXVECTOR2(1.0f * fValue, 0.0f);
			break;

		default:
			break;
		}


		pVertex += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffHealth->Unlock();
}

//-----------------------------------------------------------------------------
//�̗͂̕`�揈��
void DrawHealth(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffHealth, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntH = 0; nCntH < HEALTH_POLYGON; nCntH++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureHealth[nCntH]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntH * 4, 4);
	}
}