//-----------------------------------------------------------------------------
//
// �N���A�^�C���̏��� [timescore.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "timescore.h"
#include "player.h"
#include "goalflag.h"

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
#define TIMES_POLYGON (1)		//�g�p����I�u�W�F��
#define TIMES_TEX (2)			//�e�N�X�`������
#define TIMES_DIGIT (4)			//�����̌���

#define TIMES_CHARWIDTH (60 * 0.5f)	//�����̕�
#define TIMES_CHARHEIGHT (90 * 0.5f)	//�����̍���
#define TIMES_CHARSPACE (150 * 0.5f)	//�����̊Ԋu
#define TIMES_SPACEWIDTH (20 * 0.5f)
#define TIMES_SPACEHEIGHT (60 * 0.5f)


//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
void ConvertCurrentTime(void);

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffTimeS = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureTimeS[TIMES_TEX] = { NULL };			// �|���S���̃e�N�X�`���ւ̃|�C���^
TIMESCORE g_aTimeScore[TIMES_DIGIT];								// TIMESCORE�̏��
TIME g_Time;														// ���ԏ��
D3DXVECTOR3 g_SpacePos;												// �X�y�[�X�̏ꏊ

//-----------------------------------------------------------------------------
// �N���A�^�C���̏���������
//-----------------------------------------------------------------------------
HRESULT InitTimeScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;
	GOAL *pGoal;
	PLAYER *pPlayer;
	
	pPlayer = GetPlayer();
	pGoal = GetGoal();

	//TIMESCORE�\���̂̏�����
	for (int nCntTS = 0; nCntTS < TIMES_DIGIT; nCntTS++)
	{
		g_aTimeScore[nCntTS].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	g_SpacePos = D3DXVECTOR3(g_aTimeScore[0].pos.x + 120, g_aTimeScore[0].pos.y + 7, 0);

	//TIME�\���̏�����
	if (pGoal->stage == STAGE_1 &&
		pPlayer->damage != DAMAGESTATE_DIE)
	{
		g_Time.bStartFlag = false;
		g_Time.dwCurrentTime = NULL;
		g_Time.dwFinishTime = NULL;
		g_Time.dwStartTime = NULL;
		g_Time.dwSecond = NULL;
		g_Time.dwMinute = NULL;
	}

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number000.png", &g_pTextureTimeS[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\_.png", &g_pTextureTimeS[1]);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * (TIMES_DIGIT + 1), D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTimeS, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffTimeS->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntChar = 0; nCntChar < TIMES_DIGIT + 1; nCntChar++)
	{
		if (nCntChar == TIMES_DIGIT)
		{
			//���_���W�̐ݒ�
			pVertex[0].pos = D3DXVECTOR3((g_SpacePos.x), g_SpacePos.y + TIMES_SPACEHEIGHT, 0.0f);
			pVertex[1].pos = D3DXVECTOR3((g_SpacePos.x), g_SpacePos.y, 0.0f);
			pVertex[2].pos = D3DXVECTOR3((g_SpacePos.x + TIMES_SPACEWIDTH), g_SpacePos.y + TIMES_SPACEHEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3((g_SpacePos.x + TIMES_SPACEWIDTH), g_SpacePos.y, 0.0f);
		}
		else
		{
			//���_���W�̐ݒ�
			pVertex[0].pos = D3DXVECTOR3((g_aTimeScore[nCntChar].pos.x) + nCntChar * TIMES_CHARSPACE, g_aTimeScore[nCntChar].pos.y + TIMES_CHARHEIGHT, 0.0f);
			pVertex[1].pos = D3DXVECTOR3((g_aTimeScore[nCntChar].pos.x) + nCntChar * TIMES_CHARSPACE, g_aTimeScore[nCntChar].pos.y, 0.0f);
			pVertex[2].pos = D3DXVECTOR3((g_aTimeScore[nCntChar].pos.x + TIMES_CHARWIDTH) + nCntChar * TIMES_CHARSPACE, g_aTimeScore[nCntChar].pos.y + TIMES_CHARHEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3((g_aTimeScore[nCntChar].pos.x + TIMES_CHARWIDTH) + nCntChar * TIMES_CHARSPACE, g_aTimeScore[nCntChar].pos.y, 0.0f);
		}

		//rhw�̐ݒ�
		pVertex[0].rhw = 1.0f;	//1.0f�Œ�
		pVertex[1].rhw = 1.0f;
		pVertex[2].rhw = 1.0f;
		pVertex[3].rhw = 1.0f;


		if (nCntChar == TIMES_DIGIT)
		{
			//���_���̐ݒ�
			pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f);
		}
		else
		{
			//���_���̐ݒ�
			pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVertex[2].tex = D3DXVECTOR2(1.0f * 0.1f, 1.0f);
			pVertex[3].tex = D3DXVECTOR2(1.0f * 0.1f, 0.0f);
		}

		//���_�J���[�̐ݒ�
		pVertex[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);  //D3DCOLOR_RGBA(��, ��, ��, �����x);
		pVertex[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVertex[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVertex[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//���_�o�b�t�@���炷
		pVertex += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTimeS->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
// �N���A�^�C���̏I������
//-----------------------------------------------------------------------------
void UninitTimeScore(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffTimeS != NULL)
	{
		g_pVtxBuffTimeS->Release();
		g_pVtxBuffTimeS = NULL;
	}

	for (int nCnt = 0; nCnt < TIMES_TEX; nCnt++)
	{
		//�e�N�X�`���̊J��
		if (g_pTextureTimeS[nCnt] != NULL)
		{
			g_pTextureTimeS[nCnt]->Release();
			g_pTextureTimeS[nCnt] = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
// �N���A�^�C���̍X�V����
//-----------------------------------------------------------------------------
void UpdateTimeScore(void)
{
	VERTEX_2D *pVertex;
	PLAYER *pPlayer;
	GOAL *pGoal;
	int aNumber[TIMES_DIGIT];
	pGoal = GetGoal();
	pPlayer = GetPlayer();

	//�J�n����
	if (pGoal->stage == STAGE_1 &&
		pPlayer->damage != DAMAGESTATE_DIE)
	{
		if (pPlayer->move.x != 0)
		{
			if (g_Time.bStartFlag == false)
			{
				g_Time.dwStartTime = timeGetTime();
				g_Time.bStartFlag = true;
			}
		}
	}

	//���ݎ��Ԃ̕ϊ�
	if (g_Time.bStartFlag == true)
	{
		ConvertCurrentTime();
	}

	//���Ԃ𕪉�
	aNumber[0] = g_Time.dwMinute % 100 / 10;
	aNumber[1] = g_Time.dwMinute % 10 / 1;
	aNumber[2] = g_Time.dwSecond % 100 / 10;
	aNumber[3] = g_Time.dwSecond % 10 / 1;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffTimeS->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntChar = 0; nCntChar < TIMES_DIGIT + 1; nCntChar++)
	{
		if (nCntChar == TIMES_DIGIT)
		{
			//���_���W�̐ݒ�
			pVertex[0].pos = D3DXVECTOR3((g_SpacePos.x), g_SpacePos.y + TIMES_SPACEHEIGHT, 0.0f);
			pVertex[1].pos = D3DXVECTOR3((g_SpacePos.x), g_SpacePos.y, 0.0f);
			pVertex[2].pos = D3DXVECTOR3((g_SpacePos.x + TIMES_SPACEWIDTH), g_SpacePos.y + TIMES_SPACEHEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3((g_SpacePos.x + TIMES_SPACEWIDTH), g_SpacePos.y, 0.0f);
		}
		else
		{
			//���_���W�̐ݒ�
			pVertex[0].pos = D3DXVECTOR3((g_aTimeScore[nCntChar].pos.x) + nCntChar * TIMES_CHARSPACE, g_aTimeScore[nCntChar].pos.y + TIMES_CHARHEIGHT, 0.0f);
			pVertex[1].pos = D3DXVECTOR3((g_aTimeScore[nCntChar].pos.x) + nCntChar * TIMES_CHARSPACE, g_aTimeScore[nCntChar].pos.y, 0.0f);
			pVertex[2].pos = D3DXVECTOR3((g_aTimeScore[nCntChar].pos.x + TIMES_CHARWIDTH) + nCntChar * TIMES_CHARSPACE, g_aTimeScore[nCntChar].pos.y + TIMES_CHARHEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3((g_aTimeScore[nCntChar].pos.x + TIMES_CHARWIDTH) + nCntChar * TIMES_CHARSPACE, g_aTimeScore[nCntChar].pos.y, 0.0f);
		}

		if (nCntChar == TIMES_DIGIT)
		{
			//���_���̐ݒ�
			pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f);
		}
		else
		{
			//�e�N�X�`�����W�̍X�V
			pVertex[0].tex = D3DXVECTOR2(0.1f * (aNumber[nCntChar]), 1.0f);
			pVertex[1].tex = D3DXVECTOR2(0.1f * (aNumber[nCntChar]), 0.0f);
			pVertex[2].tex = D3DXVECTOR2(0.1f * (aNumber[nCntChar] + 1), 1.0f);
			pVertex[3].tex = D3DXVECTOR2(0.1f * (aNumber[nCntChar] + 1), 0.0f);
		}
		pVertex += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTimeS->Unlock();
}

//-----------------------------------------------------------------------------
// �N���A�^�C���̕`�揈��
//-----------------------------------------------------------------------------
void DrawTimeScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTimeS, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntChar = 0; nCntChar < TIMES_DIGIT + 1; nCntChar++)
	{
		//�e�N�X�`���̐ݒ�
		if (nCntChar == TIMES_DIGIT)
		{
			pDevice->SetTexture(0, g_pTextureTimeS[1]);
		}
		else
		{
			pDevice->SetTexture(0, g_pTextureTimeS[0]);
		}

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntChar * 4, 2);
	}
}

//-----------------------------------------------------------------------------
// ���ݎ��Ԃ̕ϊ�
//-----------------------------------------------------------------------------
void ConvertCurrentTime(void)
{
	//���ݎ���
	g_Time.dwCurrentTime = timeGetTime();
	g_Time.dwCurrentTime = (g_Time.dwCurrentTime - g_Time.dwStartTime) / 1000;

	//���b�ɕϊ�
	g_Time.dwMinute = g_Time.dwCurrentTime / 60;
	g_Time.dwSecond = g_Time.dwCurrentTime % 60;
}

//-----------------------------------------------------------------------------
// ���ԏ��擾
//-----------------------------------------------------------------------------
TIME *GetTimeScore(void)
{
	return &g_Time;
}