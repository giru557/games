//-----------------------------------------------------------------------------
//
// ���U���g�̏��� [result.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "result.h"
#include "input.h"
#include "fade.h"

//�}�N����`
#define RESULT_POLYGON (3)
#define RESULT_RESULT_WIDTH (520 / 2)
#define RESULT_RESULT_HEIGHT (82 / 2)
#define RESULT_PENTER_WIDTH (400 / 2)
#define RESULT_PENTER_HEIGHT (45 /2)

//�v���g�^�C�v�錾

float ReturnBlink(int nIdx);

//-----------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffResult = NULL;  //���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_apTextureResult[RESULT_POLYGON] = { NULL };		//���U���g�̃e�N�X�`���ւ̃|�C���^
D3DXVECTOR3 g_aPosResult[RESULT_POLYGON];								//�X�R�A�̈ʒu

//-----------------------------------------------------------------------------
//���U���g�̏���������
//-----------------------------------------------------------------------------
HRESULT InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	g_aPosResult[1] = D3DXVECTOR3(640, 105, 0.0f);
	g_aPosResult[2] = D3DXVECTOR3(1046, 650, 0.0f);

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\bg_result.png", &g_apTextureResult[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\result.png", &g_apTextureResult[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\press_enter_result.png", &g_apTextureResult[2]);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * RESULT_POLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffResult, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntR = 0; nCntR < RESULT_POLYGON; nCntR++)
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
		pVertex[0].tex = D3DXVECTOR2(0.0, 1.0);
		pVertex[1].tex = D3DXVECTOR2(0.0, 0.0);
		pVertex[2].tex = D3DXVECTOR2(1.0, 1.0);
		pVertex[3].tex = D3DXVECTOR2(1.0, 0.0);

		switch (nCntR)
		{
		case 0:
			//���_���W D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);		 //TRIANGLESTRIP�Ŏl�p
			pVertex[1].pos = D3DXVECTOR3(0, 0, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
			break;

		case 1:
			//���_���W D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(g_aPosResult[nCntR].x - RESULT_RESULT_WIDTH, g_aPosResult[nCntR].y + RESULT_RESULT_HEIGHT, 0.0f);		 //TRIANGLESTRIP�Ŏl�p
			pVertex[1].pos = D3DXVECTOR3(g_aPosResult[nCntR].x - RESULT_RESULT_WIDTH, g_aPosResult[nCntR].y - RESULT_RESULT_HEIGHT, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_aPosResult[nCntR].x + RESULT_RESULT_WIDTH, g_aPosResult[nCntR].y + RESULT_RESULT_HEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_aPosResult[nCntR].x + RESULT_RESULT_WIDTH, g_aPosResult[nCntR].y - RESULT_RESULT_HEIGHT, 0.0f);
			break;

		case 2:
			//���_���W D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(g_aPosResult[nCntR].x - RESULT_PENTER_WIDTH, g_aPosResult[nCntR].y + RESULT_PENTER_HEIGHT, 0.0f);		 //TRIANGLESTRIP�Ŏl�p
			pVertex[1].pos = D3DXVECTOR3(g_aPosResult[nCntR].x - RESULT_PENTER_WIDTH, g_aPosResult[nCntR].y - RESULT_PENTER_HEIGHT, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_aPosResult[nCntR].x + RESULT_PENTER_WIDTH, g_aPosResult[nCntR].y + RESULT_PENTER_HEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_aPosResult[nCntR].x + RESULT_PENTER_WIDTH, g_aPosResult[nCntR].y - RESULT_PENTER_HEIGHT, 0.0f);
			break;

		default:
			break;
		}

		pVertex += 4; //���_�|�C���^�����炷
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResult->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//���U���g�̏I������
//-----------------------------------------------------------------------------
void UninitResult(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}

	for (int nCntR = 0; nCntR < (RESULT_POLYGON); nCntR++)
	{
		//�e�N�X�`���̊J��
		if (g_apTextureResult[nCntR] != NULL)
		{
			g_apTextureResult[nCntR]->Release();
			g_apTextureResult[nCntR] = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
//���U���g�̍X�V����
//-----------------------------------------------------------------------------
void UpdateResult(void)
{
	VERTEX_2D *pVertex;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntR = 0; nCntR < RESULT_POLYGON; nCntR++)
	{
		switch (nCntR)
		{
		case 1:
			//���_���W D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(g_aPosResult[nCntR].x - RESULT_RESULT_WIDTH, g_aPosResult[nCntR].y + RESULT_RESULT_HEIGHT, ReturnBlink(300));		 //TRIANGLESTRIP�Ŏl�p
			pVertex[1].pos = D3DXVECTOR3(g_aPosResult[nCntR].x - RESULT_RESULT_WIDTH, g_aPosResult[nCntR].y - RESULT_RESULT_HEIGHT, ReturnBlink(300));
			pVertex[2].pos = D3DXVECTOR3(g_aPosResult[nCntR].x + RESULT_RESULT_WIDTH, g_aPosResult[nCntR].y + RESULT_RESULT_HEIGHT, ReturnBlink(300));
			pVertex[3].pos = D3DXVECTOR3(g_aPosResult[nCntR].x + RESULT_RESULT_WIDTH, g_aPosResult[nCntR].y - RESULT_RESULT_HEIGHT, ReturnBlink(300));
			break;

		case 2:
			//���_���W D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(g_aPosResult[nCntR].x - RESULT_PENTER_WIDTH, g_aPosResult[nCntR].y + RESULT_PENTER_HEIGHT, 0.0f);		 //TRIANGLESTRIP�Ŏl�p
			pVertex[1].pos = D3DXVECTOR3(g_aPosResult[nCntR].x - RESULT_PENTER_WIDTH, g_aPosResult[nCntR].y - RESULT_PENTER_HEIGHT, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_aPosResult[nCntR].x + RESULT_PENTER_WIDTH, g_aPosResult[nCntR].y + RESULT_PENTER_HEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_aPosResult[nCntR].x + RESULT_PENTER_WIDTH, g_aPosResult[nCntR].y - RESULT_PENTER_HEIGHT, 0.0f);
			break;

		default:
			break;
		}

		pVertex += 4; //���_�|�C���^�����炷
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResult->Unlock();

	if (GetKeyboardTrigger(DIK_BACKSPACE) == true || GetKeyboardTrigger(DIK_RETURN) == true)
	{
		SetFade(FADE_OUT, MODE_TITLE);
	}
}

//-----------------------------------------------------------------------------
//�^�C�g���̕`�揈��
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntR = 0; nCntR < RESULT_POLYGON; nCntR++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureResult[nCntR]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntR * 4, 4);
	}

}



float ReturnBlink(int nIdx)
{
	static bool bBlink = false;
	static int bCounter = 0;
	float fBlink = 1.0f;
	bCounter++;

	if (bCounter > nIdx * 1)
	{
		bBlink = true;
	}
	if (bCounter > nIdx * 1.3f)
	{
		bBlink = false;
	}
	if (bCounter > nIdx * 2.0f)
	{
		bCounter = 0;
	}

	if (bBlink == true)
	{
		fBlink = 1.0f;
	}
	else
	{
		fBlink = 0.0f;
	}

	return fBlink;
}