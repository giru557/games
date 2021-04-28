//-----------------------------------------------------------------------------
//
// �X�R�A�̏��� [score.cpp]
// Author; Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "score.h"

//-----------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------
#define MAX_SCORE (8) //�X�R�A���l�̐�

//-----------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL; //�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_aPosScore[MAX_SCORE];				//�X�R�A�̈ʒu
int g_nScore;									//�X�R�A

//-----------------------------------------------------------------------------
//�X�R�A�̏���������
//-----------------------------------------------------------------------------
HRESULT InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number001.png", &g_pTextureScore);

	//�X�R�A�̏�����
	g_nScore = 0;

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * MAX_SCORE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffScore, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�̃��b�N
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVertex, 0);

	//���_���W 
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		g_aPosScore[nCntScore] = D3DXVECTOR3(900, 30, 0.0f);

		//���_���W�̐ݒ�
		pVertex[0].pos = D3DXVECTOR3((g_aPosScore[nCntScore].x + -15) + nCntScore * 50, g_aPosScore[nCntScore].y + 20, 0.0f);
		pVertex[1].pos = D3DXVECTOR3((g_aPosScore[nCntScore].x + -15) + nCntScore * 50, g_aPosScore[nCntScore].y + -20, 0.0f);
		pVertex[2].pos = D3DXVECTOR3((g_aPosScore[nCntScore].x + 15) + nCntScore * 50, g_aPosScore[nCntScore].y + 20, 0.0f);
		pVertex[3].pos = D3DXVECTOR3((g_aPosScore[nCntScore].x + 15) + nCntScore * 50, g_aPosScore[nCntScore].y + -20, 0.0f);

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

		//UV�̐ݒ�
		pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVertex[2].tex = D3DXVECTOR2(1.0f * 0.1f, 1.0f);
		pVertex[3].tex = D3DXVECTOR2(1.0f * 0.1f, 0.0f);

		pVertex += 4; //���_�̃|�C���^�����炷
	}

	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffScore->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//�X�R�A�̏I������
//-----------------------------------------------------------------------------
void UninitScore(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}
}

//-----------------------------------------------------------------------------
//�X�R�A�̍X�V����
//-----------------------------------------------------------------------------
void UpdateScore(void)
{
	VERTEX_2D *pVertex;
	int aNumber[8];

	//�X�R�A�̕���
	aNumber[0] = g_nScore % 100000000 / 10000000;
	aNumber[1] = g_nScore % 10000000 / 1000000;
	aNumber[2] = g_nScore % 1000000 / 100000;
	aNumber[3] = g_nScore % 100000 / 10000;
	aNumber[4] = g_nScore % 10000 / 1000;
	aNumber[5] = g_nScore % 1000 / 100;
	aNumber[6] = g_nScore % 100 / 10;
	aNumber[7] = g_nScore % 10 / 1;

	if (g_pVtxBuffScore != NULL)
	{
		//���_�o�b�t�@�̃��b�N
		g_pVtxBuffScore->Lock(0, 0, (void**)&pVertex, 0);

		for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
		{
			//���_���W�̐ݒ�
			pVertex[0].pos = D3DXVECTOR3((g_aPosScore[nCntScore].x + -15) + nCntScore * 50, g_aPosScore[nCntScore].y + 20, 0.0f);
			pVertex[1].pos = D3DXVECTOR3((g_aPosScore[nCntScore].x + -15) + nCntScore * 50, g_aPosScore[nCntScore].y + -20, 0.0f);
			pVertex[2].pos = D3DXVECTOR3((g_aPosScore[nCntScore].x + 15) + nCntScore * 50, g_aPosScore[nCntScore].y + 20, 0.0f);
			pVertex[3].pos = D3DXVECTOR3((g_aPosScore[nCntScore].x + 15) + nCntScore * 50, g_aPosScore[nCntScore].y + -20, 0.0f);

			//�e�N�X�`�����W�̍X�V
			pVertex[0].tex = D3DXVECTOR2(0.1f * (aNumber[nCntScore]), 1.0f);
			pVertex[1].tex = D3DXVECTOR2(0.1f * (aNumber[nCntScore]), 0.0f);
			pVertex[2].tex = D3DXVECTOR2(0.1f * (aNumber[nCntScore] + 1), 1.0f);
			pVertex[3].tex = D3DXVECTOR2(0.1f * (aNumber[nCntScore] + 1), 0.0f);

			pVertex += 4;
		}

		//���_�o�b�t�@�̃A�����b�N
		g_pVtxBuffScore->Unlock();
	}
}

//-----------------------------------------------------------------------------
//�X�R�A�̕`�揈��
//-----------------------------------------------------------------------------
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureScore);

	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}
}

//-----------------------------------------------------------------------------
//�X�R�A�̒ǉ�����
//-----------------------------------------------------------------------------
void AddScore(int nValue)
{
	g_nScore += nValue;
}

//�X�R�A�̎擾
int GetScore(void)
{
	return g_nScore;
}