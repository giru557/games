#define _CRT_SECURE_NO_WARNINGS
//-----------------------------------------------------------------------------
//
// �X�R�A�̏��� [score.cpp]
// Author; Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "resultscore.h"
#include "score.h"
#include <stdio.h>

//-----------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------
#define MAX_SCORE (8) //�X�R�A�̌���
#define SCORE_DISP (2)	//�\����
#define SCORE_CHAR_WIDTH (20)
#define SCORE_CHAR_HEIGHT (30)

//-----------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------
int ReturnHighestScore(void);
void InputHighestScore(int nScore);

//-----------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureRScore = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRScore = NULL; //�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_aPosRScore[MAX_SCORE][SCORE_DISP];				//�X�R�A�̈ʒu
int g_aRScore[SCORE_DISP];									//�X�R�A

//-----------------------------------------------------------------------------
//�X�R�A�̏���������
//-----------------------------------------------------------------------------
HRESULT InitRScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number001.png", &g_pTextureRScore);

	//�X�R�A�̏�����
	g_aRScore[0] = GetScore();
	g_aRScore[1] = ReturnHighestScore();

	if (g_aRScore[0] > g_aRScore[1])
	{//�n�C�X�R�A��茻�݂̃X�R�A���ǂ����
		InputHighestScore(g_aRScore[0]); //�n�C�X�R�A�Ɍ��݂̃X�R�A����������
	}

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * (MAX_SCORE * SCORE_DISP), D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffRScore, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�̃��b�N
	g_pVtxBuffRScore->Lock(0, 0, (void**)&pVertex, 0);

	//���_���W 
	for (int nCntDisp = 0; nCntDisp < SCORE_DISP; nCntDisp++)
	{
		for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
		{
			switch (nCntDisp)
			{
			case 0:
				g_aPosRScore[nCntScore][nCntDisp] = D3DXVECTOR3(465, 322, 0.0f); //�ʒu�̐ݒ�
				break;

			case 1:
				g_aPosRScore[nCntScore][nCntDisp] = D3DXVECTOR3(465, 545, 0.0f); //�ʒu�̐ݒ�
				break;

			default:
				break;
			}

			//���_���W�̐ݒ�
			pVertex[0].pos = D3DXVECTOR3((g_aPosRScore[nCntScore][nCntDisp].x + -SCORE_CHAR_WIDTH) + nCntScore * 50, g_aPosRScore[nCntScore][nCntDisp].y + SCORE_CHAR_HEIGHT, 0.0f);
			pVertex[1].pos = D3DXVECTOR3((g_aPosRScore[nCntScore][nCntDisp].x + -SCORE_CHAR_WIDTH) + nCntScore * 50, g_aPosRScore[nCntScore][nCntDisp].y + -SCORE_CHAR_HEIGHT, 0.0f);
			pVertex[2].pos = D3DXVECTOR3((g_aPosRScore[nCntScore][nCntDisp].x + SCORE_CHAR_WIDTH) + nCntScore * 50, g_aPosRScore[nCntScore][nCntDisp].y + SCORE_CHAR_HEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3((g_aPosRScore[nCntScore][nCntDisp].x + SCORE_CHAR_WIDTH) + nCntScore * 50, g_aPosRScore[nCntScore][nCntDisp].y + -SCORE_CHAR_HEIGHT, 0.0f);

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
	}

	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffRScore->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//�X�R�A�̏I������
//-----------------------------------------------------------------------------
void UninitRScore(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffRScore != NULL)
	{
		g_pVtxBuffRScore->Release();
		g_pVtxBuffRScore = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pTextureRScore != NULL)
	{
		g_pTextureRScore->Release();
		g_pTextureRScore = NULL;
	}
}

//-----------------------------------------------------------------------------
//�X�R�A�̍X�V����
//-----------------------------------------------------------------------------
void UpdateRScore(void)
{
	VERTEX_2D *pVertex;
	int aNumber[MAX_SCORE][SCORE_DISP];

	//�X�R�A�̕���
	for (int nCntS = 0; nCntS < SCORE_DISP; nCntS++)
	{
		aNumber[0][nCntS] = g_aRScore[nCntS] % 100000000 / 10000000;
		aNumber[1][nCntS] = g_aRScore[nCntS] % 10000000 / 1000000;
		aNumber[2][nCntS] = g_aRScore[nCntS] % 1000000 / 100000;
		aNumber[3][nCntS] = g_aRScore[nCntS] % 100000 / 10000;
		aNumber[4][nCntS] = g_aRScore[nCntS] % 10000 / 1000;
		aNumber[5][nCntS] = g_aRScore[nCntS] % 1000 / 100;
		aNumber[6][nCntS] = g_aRScore[nCntS] % 100 / 10;
		aNumber[7][nCntS] = g_aRScore[nCntS] % 10 / 1;
	}

	if (g_pVtxBuffRScore != NULL)
	{
		//���_�o�b�t�@�̃��b�N
		g_pVtxBuffRScore->Lock(0, 0, (void**)&pVertex, 0);

		for (int nCntDisp = 0; nCntDisp < SCORE_DISP; nCntDisp++)
		{
			for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
			{
				//���_���W�̐ݒ�
				pVertex[0].pos = D3DXVECTOR3((g_aPosRScore[nCntScore][nCntDisp].x + -SCORE_CHAR_WIDTH) + nCntScore * 50, g_aPosRScore[nCntScore][nCntDisp].y + SCORE_CHAR_HEIGHT, 0.0f);
				pVertex[1].pos = D3DXVECTOR3((g_aPosRScore[nCntScore][nCntDisp].x + -SCORE_CHAR_WIDTH) + nCntScore * 50, g_aPosRScore[nCntScore][nCntDisp].y + -SCORE_CHAR_HEIGHT, 0.0f);
				pVertex[2].pos = D3DXVECTOR3((g_aPosRScore[nCntScore][nCntDisp].x + SCORE_CHAR_WIDTH) + nCntScore * 50, g_aPosRScore[nCntScore][nCntDisp].y + SCORE_CHAR_HEIGHT, 0.0f);
				pVertex[3].pos = D3DXVECTOR3((g_aPosRScore[nCntScore][nCntDisp].x + SCORE_CHAR_WIDTH) + nCntScore * 50, g_aPosRScore[nCntScore][nCntDisp].y + -SCORE_CHAR_HEIGHT, 0.0f);

				//�e�N�X�`�����W�̍X�V
				pVertex[0].tex = D3DXVECTOR2(0.1f * (aNumber[nCntScore][nCntDisp]), 1.0f);
				pVertex[1].tex = D3DXVECTOR2(0.1f * (aNumber[nCntScore][nCntDisp]), 0.0f);
				pVertex[2].tex = D3DXVECTOR2(0.1f * (aNumber[nCntScore][nCntDisp] + 1), 1.0f);
				pVertex[3].tex = D3DXVECTOR2(0.1f * (aNumber[nCntScore][nCntDisp] + 1), 0.0f);

				pVertex += 4;
			}
		}

		//���_�o�b�t�@�̃A�����b�N
		g_pVtxBuffRScore->Unlock();
	}
}

//-----------------------------------------------------------------------------
//�X�R�A�̕`�揈��
//-----------------------------------------------------------------------------
void DrawRScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRScore, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureRScore);

	for (int nCntScore = 0; nCntScore < (MAX_SCORE * SCORE_DISP); nCntScore++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}
}

//�n�C�X�R�A�擾
int ReturnHighestScore(void)
{
	FILE *pFile;
	int nScore;

	//�t�@�C���ǂݍ���
	pFile = fopen("Highest.bin", "rb");
	if (pFile != NULL)
	{
		fscanf(pFile, "%d", &nScore);

		fclose(pFile);
	}

	return nScore;
}

//�n�C�X�R�A��������
void InputHighestScore(int nScore)
{
	FILE *pFile;

	//�t�@�C���ǂݍ���
	pFile = fopen("Highest.bin", "wb");
	if (pFile != NULL)
	{
		fprintf(pFile, "%d", nScore);

		fclose(pFile);
	}
}