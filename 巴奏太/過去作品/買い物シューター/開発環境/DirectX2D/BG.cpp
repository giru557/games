//-----------------------------------------------------------------------------
//
// �w�i�̏��� [BG.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "BG.h"

//�}�N��
#define SCROLL_TEXTURE (2)



//-----------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffBG = NULL;  //���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureBG = NULL;		//�|���S���̃e�N�X�`���ւ̃|�C���^
BG g_BG;

//-----------------------------------------------------------------------------
//�w�i�̏���������
//-----------------------------------------------------------------------------
HRESULT InitBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\desert-backgorund.png", &g_pTextureBG);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBG, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVertex, 0);

	//���_���W D3DXVECTOR3(X,Y, 0.0f);
	pVertex[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);		 //TRIANGLESTRIP�Ŏl�p
	pVertex[1].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVertex[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	pVertex[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

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

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBG->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//�w�i�̏I������
//-----------------------------------------------------------------------------
void UninitBG(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffBG != NULL)
	{
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pTextureBG != NULL)
	{
		g_pTextureBG->Release();
		g_pTextureBG = NULL;
	}
}

//-----------------------------------------------------------------------------
//�w�i�̍X�V����
//-----------------------------------------------------------------------------
void UpdateBG(void)
{
	VERTEX_2D *pVertex;
	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVertex, 0);

	//���_���̐ݒ�
	pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f - g_BG.y);
	pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f - g_BG.y);
	pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f - g_BG.y);
	pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f - g_BG.y);

	g_BG.y += 0.001f;

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBG->Unlock();
}

//-----------------------------------------------------------------------------
//�w�i�̕`�揈��
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBG);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 4);
}

//GetBG
BG GetBG(void)
{
	return g_BG;
}