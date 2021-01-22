//-----------------------------------------------------------------------------
//
// ���U���g�X�R�A�̏��� [resultscore.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "resultscore.h"
#include "timescore.h"

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
#define RESULTSCORE_POLYGON (4)		//�g�p����I�u�W�F��

#define RESULTSCORE_WIDTH (100)
#define RESULTSCORE_HEIGHT (150)
#define RESULTSCORE_SPACE (180)
#define RESULTSCORE_POS_X (400)
#define RESULTSCORE_POS_Y (390)

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffTimeR = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureTimeR = { NULL };			// �|���S���̃e�N�X�`���ւ̃|�C���^
RESULTSCORE g_aResultScore[RESULTSCORE_POLYGON];

//-----------------------------------------------------------------------------
// ���U���g�X�R�A�̏���������
//-----------------------------------------------------------------------------
HRESULT InitResultScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	//TIMESCORE�\���̂̏�����
	for (int nCntRS = 0; nCntRS < RESULTSCORE_POLYGON; nCntRS++)
	{
		g_aResultScore[nCntRS].pos = D3DXVECTOR3(RESULTSCORE_POS_X, RESULTSCORE_POS_Y, 0.0f);
	}
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number000.png", &g_pTextureTimeR);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * RESULTSCORE_POLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTimeR, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffTimeR->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntChar = 0; nCntChar < RESULTSCORE_POLYGON; nCntChar++)
	{
		//���_���W�̐ݒ�
		pVertex[0].pos = D3DXVECTOR3((g_aResultScore[nCntChar].pos.x) + nCntChar * RESULTSCORE_SPACE, g_aResultScore[nCntChar].pos.y + RESULTSCORE_HEIGHT, 0.0f);
		pVertex[1].pos = D3DXVECTOR3((g_aResultScore[nCntChar].pos.x) + nCntChar * RESULTSCORE_SPACE, g_aResultScore[nCntChar].pos.y, 0.0f);
		pVertex[2].pos = D3DXVECTOR3((g_aResultScore[nCntChar].pos.x + RESULTSCORE_WIDTH) + nCntChar * RESULTSCORE_SPACE, g_aResultScore[nCntChar].pos.y + RESULTSCORE_HEIGHT, 0.0f);
		pVertex[3].pos = D3DXVECTOR3((g_aResultScore[nCntChar].pos.x + RESULTSCORE_WIDTH) + nCntChar * RESULTSCORE_SPACE, g_aResultScore[nCntChar].pos.y, 0.0f);

		//rhw�̐ݒ�
		pVertex[0].rhw = 1.0f;	//1.0f�Œ�
		pVertex[1].rhw = 1.0f;
		pVertex[2].rhw = 1.0f;
		pVertex[3].rhw = 1.0f;

		//���_���̐ݒ�
		pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVertex[2].tex = D3DXVECTOR2(1.0f * 0.1f, 1.0f);
		pVertex[3].tex = D3DXVECTOR2(1.0f * 0.1f, 0.0f);

		//���_�J���[�̐ݒ�
		pVertex[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);  //D3DCOLOR_RGBA(��, ��, ��, �����x);
		pVertex[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVertex[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVertex[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//���_�o�b�t�@���炷
		pVertex += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTimeR->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
// ���U���g�X�R�A�̏I������
//-----------------------------------------------------------------------------
void UninitResultScore(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffTimeR != NULL)
	{
		g_pVtxBuffTimeR->Release();
		g_pVtxBuffTimeR = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pTextureTimeR != NULL)
	{
		g_pTextureTimeR->Release();
		g_pTextureTimeR = NULL;
	}
}

//-----------------------------------------------------------------------------
// ���U���g�X�R�A�̍X�V����
//-----------------------------------------------------------------------------
void UpdateResultScore(void)
{
	VERTEX_2D *pVertex;
	TIME *pTime;
	int nScore;
	int nMinute, nSecond;
	pTime = GetTimeScore();

	int aNumber[RESULTSCORE_POLYGON];

	//�S�[���^�C��
	nScore = pTime->dwFinishTime - pTime->dwStartTime;

	nMinute = (nScore / 1000) / 60;
	nSecond = (nScore / 1000) % 60;

	//���Ԃ𕪉�
	aNumber[0] = nMinute % 100 / 10;
	aNumber[1] = nMinute % 10 / 1;
	aNumber[2] = nSecond % 100 / 10;
	aNumber[3] = nSecond % 10 / 1;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffTimeR->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntChar = 0; nCntChar < RESULTSCORE_POLYGON; nCntChar++)
	{
		if (nCntChar == 2)
		{
			//���_���W D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3((g_aResultScore[nCntChar].pos.x) + (nCntChar + 1) * RESULTSCORE_SPACE, (g_aResultScore[nCntChar].pos.y + RESULTSCORE_HEIGHT), 0.0f);		 //TRIANGLESTRIP�Ŏl�p
			pVertex[1].pos = D3DXVECTOR3((g_aResultScore[nCntChar].pos.x) + (nCntChar + 1) * RESULTSCORE_SPACE, (g_aResultScore[nCntChar].pos.y), 0.0f);
			pVertex[2].pos = D3DXVECTOR3((g_aResultScore[nCntChar].pos.x + RESULTSCORE_WIDTH) + (nCntChar + 1) * RESULTSCORE_SPACE, (g_aResultScore[nCntChar].pos.y + RESULTSCORE_HEIGHT), 0.0f);
			pVertex[3].pos = D3DXVECTOR3((g_aResultScore[nCntChar].pos.x + RESULTSCORE_WIDTH) + (nCntChar + 1) * RESULTSCORE_SPACE, (g_aResultScore[nCntChar].pos.y), 0.0f);
		}
		else if (nCntChar > 2)
		{
			//���_���W D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3((g_aResultScore[nCntChar].pos.x) + (nCntChar + 1) * RESULTSCORE_SPACE, (g_aResultScore[nCntChar].pos.y + RESULTSCORE_HEIGHT), 0.0f);		 //TRIANGLESTRIP�Ŏl�p
			pVertex[1].pos = D3DXVECTOR3((g_aResultScore[nCntChar].pos.x) + (nCntChar + 1) * RESULTSCORE_SPACE, (g_aResultScore[nCntChar].pos.y), 0.0f);
			pVertex[2].pos = D3DXVECTOR3((g_aResultScore[nCntChar].pos.x + RESULTSCORE_WIDTH) + (nCntChar + 1) * RESULTSCORE_SPACE, (g_aResultScore[nCntChar].pos.y + RESULTSCORE_HEIGHT), 0.0f);
			pVertex[3].pos = D3DXVECTOR3((g_aResultScore[nCntChar].pos.x + RESULTSCORE_WIDTH) + (nCntChar + 1) * RESULTSCORE_SPACE, (g_aResultScore[nCntChar].pos.y), 0.0f);

		}
		else
		{
			//���_���W D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3((g_aResultScore[nCntChar].pos.x) + nCntChar * RESULTSCORE_SPACE, (g_aResultScore[nCntChar].pos.y + RESULTSCORE_HEIGHT), 0.0f);		 //TRIANGLESTRIP�Ŏl�p
			pVertex[1].pos = D3DXVECTOR3((g_aResultScore[nCntChar].pos.x) + nCntChar * RESULTSCORE_SPACE, (g_aResultScore[nCntChar].pos.y), 0.0f);
			pVertex[2].pos = D3DXVECTOR3((g_aResultScore[nCntChar].pos.x + RESULTSCORE_WIDTH) + nCntChar * RESULTSCORE_SPACE, (g_aResultScore[nCntChar].pos.y + RESULTSCORE_HEIGHT), 0.0f);
			pVertex[3].pos = D3DXVECTOR3((g_aResultScore[nCntChar].pos.x + RESULTSCORE_WIDTH) + nCntChar * RESULTSCORE_SPACE, (g_aResultScore[nCntChar].pos.y), 0.0f);
		}

		//�e�N�X�`�����W�̍X�V
		pVertex[0].tex = D3DXVECTOR2(0.1f * (aNumber[nCntChar]), 1.0f);
		pVertex[1].tex = D3DXVECTOR2(0.1f * (aNumber[nCntChar]), 0.0f);
		pVertex[2].tex = D3DXVECTOR2(0.1f * (aNumber[nCntChar] + 1), 1.0f);
		pVertex[3].tex = D3DXVECTOR2(0.1f * (aNumber[nCntChar] + 1), 0.0f);

		pVertex += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTimeR->Unlock();
}

//-----------------------------------------------------------------------------
// �N���A�^�C���̕`�揈��
//-----------------------------------------------------------------------------
void DrawResultScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTimeR, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, g_pTextureTimeR);

	for (int nCnt = 0; nCnt < RESULTSCORE_POLYGON; nCnt++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}
}