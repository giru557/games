#define _CRT_SECURE_NO_WARNINGS
//-----------------------------------------------------------------------------
//
// �����L���O�X�R�A�̏��� [rankedscore.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "rankedscore.h"
#include "input.h"
#include "fade.h"
#include "timescore.h"
#include <stdio.h>

//-----------------------------------------------------------------------------
// �}�N��
//-----------------------------------------------------------------------------
#define RSCORE_ROW (5)															//�X�R�A�̍s
#define RSCORE_COLUMN (4)														//�X�R�A�̗�
#define RSCORE_DIGIT (5)														//�X�R�A�̌�
#define RSCORE_POLYGON (RSCORE_ROW * RSCORE_COLUMN + 1)								//�z�u�I�u�W�F�N�g��
#define RSCORE_TEX (2)															//�g�p����e�N�X�`���̐�

#define RSCORE_POS_X (0)														//�X�R�A�̈ʒuX
#define RSCORE_POS_Y (0)														//�X�R�A�̈ʒuY

#define RSCORE_WIDTH (50)														//�����̕�
#define RSCORE_HEIGHT (100)														//�����̍���
#define RSCORE_SPACE_X (100)													//�����X�y�[�X ��
#define RSCORE_SPACE_Y (138)												//�����X�y�[�X �c

#define RSCORE_NEW_WIDTH (200)
#define RSCORE_NEW_HEIGHT (100)

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
void SortScore(int nRow);
void ConvertTime(void);
void LoadRanking(void);
void SaveRanking(void);
void IncludeNewScore(void);

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffRScore = NULL;								//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureRScore[RSCORE_TEX] = { NULL };									//�|���S���̃e�N�X�`���ւ̃|�C���^
D3DXVECTOR3 g_NewScorePos;
RSCORE g_aRscore[RSCORE_ROW][RSCORE_COLUMN];									//RSCORE�̏��
SCORE g_aScore[RSCORE_ROW + 1];													//�X�R�A�̔z��

//-----------------------------------------------------------------------------
// �����L���O�X�R�A�̏���������
//-----------------------------------------------------------------------------
HRESULT InitRScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;
	int aNumber[RSCORE_ROW][RSCORE_COLUMN];

	//������
	for (int nCntRow = 0; nCntRow < RSCORE_ROW; nCntRow++)
	{
		for (int nCntColumn = 0; nCntColumn < RSCORE_COLUMN; nCntColumn++)
		{
			g_aRscore[nCntRow][nCntColumn].pos = D3DXVECTOR3(580, 160, 0.0f);
		}
		g_aScore[nCntRow].score = 9999999;
		g_aScore[nCntRow].hour = 0;
		g_aScore[nCntRow].minute = 0;
		g_aScore[nCntRow].second = 0;
		g_NewScorePos = D3DXVECTOR3(200, 0, 0.0f);
	}
	
	//�����L���O�����[�h
	LoadRanking();

	//�X�R�A�̕��בւ�
	SortScore(RSCORE_ROW - 1);

	//�V�����X�R�A���܂�Ń\�[�g
	IncludeNewScore();

	//���ԕϊ�
	ConvertTime();

	//�X�R�A����
	for (int nCntRow = 0; nCntRow < RSCORE_ROW; nCntRow++)
	{
		aNumber[nCntRow][0] = g_aScore[nCntRow].minute % 100 / 10;
		aNumber[nCntRow][1] = g_aScore[nCntRow].minute % 10 / 1;
		aNumber[nCntRow][2] = g_aScore[nCntRow].second % 100 / 10;
		aNumber[nCntRow][3] = g_aScore[nCntRow].second % 10 / 1;
	}

	//�����L���O���Z�[�u
	SaveRanking();

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	//D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\new_ranking.png", &g_pTextureRScore[0]);
	D3DXCreateTextureFromFileEx(pDevice, "data\\TEXTURE\\new_ranking.png", 310, 80, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0xFF000000, NULL, NULL, &g_pTextureRScore[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number000.png", &g_pTextureRScore[1]);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * RSCORE_POLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffRScore, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffRScore->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].pos = D3DXVECTOR3(g_NewScorePos.x, g_NewScorePos.y + RSCORE_NEW_HEIGHT, 0.0f);
	pVertex[1].pos = D3DXVECTOR3(g_NewScorePos.x, g_NewScorePos.y, 0.0f);
	pVertex[2].pos = D3DXVECTOR3(g_NewScorePos.x + RSCORE_NEW_WIDTH, g_NewScorePos.y + RSCORE_NEW_HEIGHT, 0.0f);
	pVertex[3].pos = D3DXVECTOR3(g_NewScorePos.x + RSCORE_NEW_WIDTH, g_NewScorePos.y, 0.0f);

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

	//�e�N�X�`�����W�̍X�V
	//���_���̐ݒ�
	pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	pVertex += 4;

	for (int nCntRow = 0; nCntRow < RSCORE_ROW; nCntRow++)
	{
		for (int nCntColumn = 0; nCntColumn < RSCORE_COLUMN; nCntColumn++)
		{
			if (nCntColumn == 2)
			{
				//���_���W D3DXVECTOR3(X,Y, 0.0f);
				pVertex[0].pos = D3DXVECTOR3((g_aRscore[nCntRow][nCntColumn].pos.x) + (nCntColumn + 1) * RSCORE_SPACE_X, (g_aRscore[nCntRow][nCntColumn].pos.y + RSCORE_HEIGHT) + nCntRow * RSCORE_SPACE_Y, 0.0f);		 //TRIANGLESTRIP�Ŏl�p
				pVertex[1].pos = D3DXVECTOR3((g_aRscore[nCntRow][nCntColumn].pos.x) + (nCntColumn + 1) * RSCORE_SPACE_X, (g_aRscore[nCntRow][nCntColumn].pos.y) + nCntRow * RSCORE_SPACE_Y, 0.0f);
				pVertex[2].pos = D3DXVECTOR3((g_aRscore[nCntRow][nCntColumn].pos.x + RSCORE_WIDTH) + (nCntColumn + 1) * RSCORE_SPACE_X, (g_aRscore[nCntRow][nCntColumn].pos.y + RSCORE_HEIGHT) + nCntRow * RSCORE_SPACE_Y, 0.0f);
				pVertex[3].pos = D3DXVECTOR3((g_aRscore[nCntRow][nCntColumn].pos.x + RSCORE_WIDTH) + (nCntColumn + 1) * RSCORE_SPACE_X, (g_aRscore[nCntRow][nCntColumn].pos.y) + nCntRow * RSCORE_SPACE_Y, 0.0f);

			}
			else if (nCntColumn > 2)
			{
				//���_���W D3DXVECTOR3(X,Y, 0.0f);
				pVertex[0].pos = D3DXVECTOR3((g_aRscore[nCntRow][nCntColumn].pos.x) + (nCntColumn + 1) * RSCORE_SPACE_X, (g_aRscore[nCntRow][nCntColumn].pos.y + RSCORE_HEIGHT) + nCntRow * RSCORE_SPACE_Y, 0.0f);		 //TRIANGLESTRIP�Ŏl�p
				pVertex[1].pos = D3DXVECTOR3((g_aRscore[nCntRow][nCntColumn].pos.x) + (nCntColumn + 1) * RSCORE_SPACE_X, (g_aRscore[nCntRow][nCntColumn].pos.y) + nCntRow * RSCORE_SPACE_Y, 0.0f);
				pVertex[2].pos = D3DXVECTOR3((g_aRscore[nCntRow][nCntColumn].pos.x + RSCORE_WIDTH) + (nCntColumn + 1) * RSCORE_SPACE_X, (g_aRscore[nCntRow][nCntColumn].pos.y + RSCORE_HEIGHT) + nCntRow * RSCORE_SPACE_Y, 0.0f);
				pVertex[3].pos = D3DXVECTOR3((g_aRscore[nCntRow][nCntColumn].pos.x + RSCORE_WIDTH) + (nCntColumn + 1) * RSCORE_SPACE_X, (g_aRscore[nCntRow][nCntColumn].pos.y) + nCntRow * RSCORE_SPACE_Y, 0.0f);

			}
			else
			{
				//���_���W D3DXVECTOR3(X,Y, 0.0f);
				pVertex[0].pos = D3DXVECTOR3((g_aRscore[nCntRow][nCntColumn].pos.x) + nCntColumn * RSCORE_SPACE_X, (g_aRscore[nCntRow][nCntColumn].pos.y + RSCORE_HEIGHT) + nCntRow * RSCORE_SPACE_Y, 0.0f);		 //TRIANGLESTRIP�Ŏl�p
				pVertex[1].pos = D3DXVECTOR3((g_aRscore[nCntRow][nCntColumn].pos.x) + nCntColumn * RSCORE_SPACE_X, (g_aRscore[nCntRow][nCntColumn].pos.y) + nCntRow * RSCORE_SPACE_Y, 0.0f);
				pVertex[2].pos = D3DXVECTOR3((g_aRscore[nCntRow][nCntColumn].pos.x + RSCORE_WIDTH) + nCntColumn * RSCORE_SPACE_X, (g_aRscore[nCntRow][nCntColumn].pos.y + RSCORE_HEIGHT) + nCntRow * RSCORE_SPACE_Y, 0.0f);
				pVertex[3].pos = D3DXVECTOR3((g_aRscore[nCntRow][nCntColumn].pos.x + RSCORE_WIDTH) + nCntColumn * RSCORE_SPACE_X, (g_aRscore[nCntRow][nCntColumn].pos.y) + nCntRow * RSCORE_SPACE_Y, 0.0f);
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

			//�e�N�X�`�����W�̍X�V
			pVertex[0].tex = D3DXVECTOR2(0.1f * (aNumber[nCntRow][nCntColumn]), 1.0f);
			pVertex[1].tex = D3DXVECTOR2(0.1f * (aNumber[nCntRow][nCntColumn]), 0.0f);
			pVertex[2].tex = D3DXVECTOR2(0.1f * (aNumber[nCntRow][nCntColumn] + 1), 1.0f);
			pVertex[3].tex = D3DXVECTOR2(0.1f * (aNumber[nCntRow][nCntColumn] + 1), 0.0f);

			pVertex += 4; //���_���炵
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRScore->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
// �����L���O�X�R�A�̏I������
//-----------------------------------------------------------------------------
void UninitRScore(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffRScore != NULL)
	{
		g_pVtxBuffRScore->Release();
		g_pVtxBuffRScore = NULL;
	}

	for (int nCnt = 0; nCnt < RSCORE_TEX; nCnt++)
	{
		//�e�N�X�`���̊J��
		if (g_pTextureRScore[nCnt] != NULL)
		{
			g_pTextureRScore[nCnt]->Release();
			g_pTextureRScore[nCnt] = NULL;
		}
	}


}

//-----------------------------------------------------------------------------
// �����L���O�X�R�A�̍X�V����
//-----------------------------------------------------------------------------
void UpdateRScore(void)
{
}

//-----------------------------------------------------------------------------
// �����L���O�X�R�A�̕`�揈��
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



	for (int nCntTex = 0; nCntTex < RSCORE_POLYGON; nCntTex++)
	{
		switch (nCntTex)
		{
		case 0:
			//�e�N�X�`���ݒ�
			pDevice->SetTexture(0, g_pTextureRScore[0]);
			break;

		default:
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureRScore[1]);
			break;
		}
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTex * 4, 2);
	}
}

//-----------------------------------------------------------------------------
// �X�R�A���בւ�
//-----------------------------------------------------------------------------
void SortScore(int nRow)
{
	int nSubData;
	for (int nCnt1 = 0; nCnt1 < nRow + 1; nCnt1++)
	{
		for (int nCnt2 = nCnt1 + 1; nCnt2 < nRow + 1; nCnt2++)
		{
			if (g_aScore[nCnt1].score > g_aScore[nCnt2].score)
			{
				nSubData = g_aScore[nCnt1].score;
				g_aScore[nCnt1].score = g_aScore[nCnt2].score;
				g_aScore[nCnt2].score = nSubData;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// �b����ϊ�
//-----------------------------------------------------------------------------
void ConvertTime(void)
{
	for (int nCntRow = 0; nCntRow < RSCORE_ROW; nCntRow++)
	{
		g_aScore[nCntRow].minute = (g_aScore[nCntRow].score / 1000) / 60;
		g_aScore[nCntRow].second = (g_aScore[nCntRow].score / 1000) % 60;
	}

	for (int nCnt = 0; nCnt < RSCORE_ROW; nCnt++)
	{
		if (g_aScore[nCnt].minute >= 100)
		{
			g_aScore[nCnt].minute = 99;
			g_aScore[nCnt].second = 99;
		}
	}
}

//-----------------------------------------------------------------------------
// �����L���O�����[�h
//-----------------------------------------------------------------------------
void LoadRanking(void)
{
	FILE *pFile;

	pFile = fopen("leaderboard.txt", "r");
	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < RSCORE_ROW; nCnt++)
		{
			fscanf(pFile, "%d", &g_aScore[nCnt].score);
		}
	}
	fclose(pFile);
}

//-----------------------------------------------------------------------------
// �����L���O���Z�[�u
//-----------------------------------------------------------------------------
void SaveRanking(void)
{
	FILE *pFile;

	pFile = fopen("leaderboard.txt", "w");
	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < RSCORE_ROW; nCnt++)
		{
			fprintf(pFile, "%d\n", g_aScore[nCnt].score);
		}
	}
	fclose(pFile);
}

//-----------------------------------------------------------------------------
// �j���[�X�R�A���܂߂ă\�[�g
//-----------------------------------------------------------------------------
void IncludeNewScore(void)
{
	TIME *pTimeScore;
	int nNewScore;

	pTimeScore = GetTimeScore();

	LoadRanking();

	g_aScore[RSCORE_ROW].score = pTimeScore->dwFinishTime - pTimeScore->dwStartTime;
	nNewScore = pTimeScore->dwFinishTime - pTimeScore->dwStartTime;

	SortScore(RSCORE_ROW);

	for (int nCnt = 0; nCnt < RSCORE_ROW; nCnt++)
	{
		if (g_aScore[nCnt].score == nNewScore)
		{
			g_NewScorePos = D3DXVECTOR3(g_aRscore[0][0].pos.x + 500, g_aRscore[0][0].pos.y + nCnt * RSCORE_SPACE_Y, 0.0f);
			break;
		}
		else
		{
			g_NewScorePos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		}
	}



}