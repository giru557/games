//-----------------------------------------------------------------------------
//
// �R�C���̏��� [coin.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "coin.h"

//�}�N��
#define COIN_POLYGON (1)
#define COIN_ANIM_SPLIT (8)
#define COIN_SIZE (16 * 1.5f)
#define COIN_ANIM_SPEED (13)

//-----------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffCoin = NULL;  //���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureCoin = NULL;		//�|���S���̃e�N�X�`���ւ̃|�C���^
D3DXVECTOR3 g_aPosCoin[COIN_POLYGON];			//�R�C���̈ʒu
int g_nPatternCoin;
int g_nCounterCoin;

//-----------------------------------------------------------------------------
//�R�C���̏���������
//-----------------------------------------------------------------------------
HRESULT InitCoin(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	g_aPosCoin[0] = D3DXVECTOR3(950, 600, 0.0f);
	
	g_nPatternCoin = 0;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\coins.png", &g_pTextureCoin);
	D3DXCreateTextureFromFileEx(pDevice, "data\\TEXTURE\\coins.png", 127, 16, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0xFF000000, NULL, NULL, &g_pTextureCoin);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * COIN_POLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffCoin, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffCoin->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntC = 0; nCntC < COIN_POLYGON; nCntC++)
	{
		//���_���W D3DXVECTOR3(X,Y, 0.0f);
		pVertex[0].pos = D3DXVECTOR3(g_aPosCoin[nCntC].x - COIN_SIZE, g_aPosCoin[nCntC].y + COIN_SIZE, 0.0f);		 //TRIANGLESTRIP�Ŏl�p
		pVertex[1].pos = D3DXVECTOR3(g_aPosCoin[nCntC].x - COIN_SIZE, g_aPosCoin[nCntC].y - COIN_SIZE, 0.0f);
		pVertex[2].pos = D3DXVECTOR3(g_aPosCoin[nCntC].x + COIN_SIZE, g_aPosCoin[nCntC].y + COIN_SIZE, 0.0f);
		pVertex[3].pos = D3DXVECTOR3(g_aPosCoin[nCntC].x + COIN_SIZE, g_aPosCoin[nCntC].y - COIN_SIZE, 0.0f);

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
		pVertex[0].tex = D3DXVECTOR2(g_nPatternCoin * (1.0f / COIN_ANIM_SPLIT), 1.0f);
		pVertex[1].tex = D3DXVECTOR2(g_nPatternCoin * (1.0f / COIN_ANIM_SPLIT), 0.0f);
		pVertex[2].tex = D3DXVECTOR2((g_nPatternCoin + 1) * (1.0f / COIN_ANIM_SPLIT), 1.0f);
		pVertex[3].tex = D3DXVECTOR2((g_nPatternCoin + 1) * (1.0f / COIN_ANIM_SPLIT), 0.0f);

		pVertex += 4; //�|�C���^���炷
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCoin->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//�R�C���̏I������
//-----------------------------------------------------------------------------
void UninitCoin(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffCoin != NULL)
	{
		g_pVtxBuffCoin->Release();
		g_pVtxBuffCoin = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pTextureCoin != NULL)
	{
		g_pTextureCoin->Release();
		g_pTextureCoin = NULL;
	}

}

//-----------------------------------------------------------------------------
//�R�C���̍X�V����
//-----------------------------------------------------------------------------
void UpdateCoin(void)
{
	VERTEX_2D *pVertex;

	//�J�E���g�A�b�v
	g_nCounterCoin++;

	//�p�^�[�����[�v
	if (g_nCounterCoin % COIN_ANIM_SPEED == 0)
	{
		g_nPatternCoin = (g_nPatternCoin + 1) % COIN_ANIM_SPLIT; //COIN_ANIM_SPLIT�܂ł����Ƃ��܂�0���������ă��Z�b�g
	}
	
	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffCoin->Lock(0, 0, (void**)&pVertex, 0);

	//���_���̐ݒ�
	pVertex[0].tex = D3DXVECTOR2(g_nPatternCoin * (1.0f / COIN_ANIM_SPLIT), 1.0f);
	pVertex[1].tex = D3DXVECTOR2(g_nPatternCoin * (1.0f / COIN_ANIM_SPLIT), 0.0f);
	pVertex[2].tex = D3DXVECTOR2((g_nPatternCoin + 1) * (1.0f / COIN_ANIM_SPLIT), 1.0f);
	pVertex[3].tex = D3DXVECTOR2((g_nPatternCoin + 1) * (1.0f / COIN_ANIM_SPLIT), 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCoin->Unlock();
}

//-----------------------------------------------------------------------------
//�R�C���̕`�揈��
//-----------------------------------------------------------------------------
void DrawCoin(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffCoin, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureCoin);

	for (int nCntC = 0; nCntC < COIN_POLYGON; nCntC++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntC * 4, 2);
	}
}
