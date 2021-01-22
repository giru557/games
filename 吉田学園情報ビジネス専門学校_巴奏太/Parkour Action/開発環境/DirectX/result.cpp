//-----------------------------------------------------------------------------
//
// ���U���g�̏��� [result.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "result.h"
#include "input.h"
#include "fade.h"

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
#define RESULT_POLYGON (2)

#define RESULT_PRESSENTER_WIDTH (575)												// �v���X�G���^�[�̕�
#define RESULT_PRESSENTER_HEIGHT (68)												// ����
#define RESULT_PRESSENTER_POS_X (1200)												// �v���X�G���^�[�̏ꏊX
#define RESULT_PRESSENTER_POS_Y (850)												// �v���X�G���^�[�̏ꏊY

#define RESULT_SHRINKSPD (0.0005f)													//�k���g��̑��x
#define RESULT_SHRINKRANGE (0.9f)													//�ǂ��܂ŏk�����邩 (0.0f ~ 1.0f)
#define RESULT_BLINKTIME (1.5f)															//�_�ŊԊu (�b)
#define RESULT_BLINKTIMEFIXED (RESULT_BLINKTIME * 1000)								//�b����~���b�ɕϊ�

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
float ResultShrink(float fShrink);
float ResultBlink(float fBlink);

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffResult = NULL;								//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_apTextureResult[RESULT_POLYGON] = { NULL };				//���U���g�̃e�N�X�`���ւ̃|�C���^
RESULT g_aResult[RESULT_POLYGON];												//���U���g�\���̂̔z����

//-----------------------------------------------------------------------------
// ���U���g�̏���������
//-----------------------------------------------------------------------------
HRESULT InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	g_aResult[0].pos = D3DXVECTOR3(0, 0, 0);
	g_aResult[1].pos = D3DXVECTOR3(RESULT_PRESSENTER_POS_X, RESULT_PRESSENTER_POS_Y, 0.0f);
	for (int nCntRESULT = 0; nCntRESULT < RESULT_POLYGON; nCntRESULT++)
	{
		g_aResult[nCntRESULT].fShrink = 1.0f;
		g_aResult[nCntRESULT].fBlink = 1.0f;
	}

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\bg_result.png", &g_apTextureResult[0]);
	D3DXCreateTextureFromFileEx(pDevice, "data\\TEXTURE\\bg_result_pressenter.png", RESULT_PRESSENTER_WIDTH, RESULT_PRESSENTER_HEIGHT, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0xFF000000, NULL, NULL, &g_apTextureResult[1]);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * RESULT_POLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffResult, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntR = 0; nCntR < RESULT_POLYGON; nCntR++)
	{
		switch (nCntR)
		{
		case 0:
			//���_���W D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(g_aResult[nCntR].pos.x, g_aResult[nCntR].pos.y + SCREEN_HEIGHT, 0.0f);
			pVertex[1].pos = D3DXVECTOR3(g_aResult[nCntR].pos.x, g_aResult[nCntR].pos.y, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_aResult[nCntR].pos.x + SCREEN_WIDTH, g_aResult[nCntR].pos.y + SCREEN_HEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_aResult[nCntR].pos.x + SCREEN_WIDTH, g_aResult[nCntR].pos.y, 0.0f);
			break;

		case 1:
			//���_���W D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(g_aResult[nCntR].pos.x - (RESULT_PRESSENTER_WIDTH / 2), g_aResult[nCntR].pos.y + (RESULT_PRESSENTER_HEIGHT / 2), 0.0f);		 //TRIANGLESTRIP�Ŏl�p
			pVertex[1].pos = D3DXVECTOR3(g_aResult[nCntR].pos.x - (RESULT_PRESSENTER_WIDTH / 2), g_aResult[nCntR].pos.y - (RESULT_PRESSENTER_HEIGHT / 2), 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_aResult[nCntR].pos.x + (RESULT_PRESSENTER_WIDTH / 2), g_aResult[nCntR].pos.y + (RESULT_PRESSENTER_HEIGHT / 2), 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_aResult[nCntR].pos.x + (RESULT_PRESSENTER_WIDTH / 2), g_aResult[nCntR].pos.y - (RESULT_PRESSENTER_HEIGHT / 2), 0.0f);
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
		pVertex[0].tex = D3DXVECTOR2(0.0, 1.0);
		pVertex[1].tex = D3DXVECTOR2(0.0, 0.0);
		pVertex[2].tex = D3DXVECTOR2(1.0, 1.0);
		pVertex[3].tex = D3DXVECTOR2(1.0, 0.0);

		pVertex += 4; //���_�|�C���^�����炷
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResult->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
// ���U���g�̏I������
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
// ���U���g�̍X�V����
//-----------------------------------------------------------------------------
void UpdateResult(void)
{
	VERTEX_2D *pVertex;
	RESULT *pResult;
	pResult = &g_aResult[0];

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntR = 0; nCntR < RESULT_POLYGON; nCntR++, pResult++)
	{
		switch (nCntR)
		{
		case 1:
			//�^�C�g���̉��o
			pResult->fBlink = ResultBlink(pResult->fBlink);

			//���_���W D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(pResult->pos.x - ((RESULT_PRESSENTER_WIDTH / 2) * pResult->fShrink), pResult->pos.y + ((RESULT_PRESSENTER_HEIGHT / 2) * pResult->fShrink), 0.0f);		 //TRIANGLESTRIP�Ŏl�p
			pVertex[1].pos = D3DXVECTOR3(pResult->pos.x - ((RESULT_PRESSENTER_WIDTH / 2) * pResult->fShrink), pResult->pos.y - ((RESULT_PRESSENTER_HEIGHT / 2) * pResult->fShrink), 0.0f);
			pVertex[2].pos = D3DXVECTOR3(pResult->pos.x + ((RESULT_PRESSENTER_WIDTH / 2) * pResult->fShrink), pResult->pos.y + ((RESULT_PRESSENTER_HEIGHT / 2) * pResult->fShrink), 0.0f);
			pVertex[3].pos = D3DXVECTOR3(pResult->pos.x + ((RESULT_PRESSENTER_WIDTH / 2) * pResult->fShrink), pResult->pos.y - ((RESULT_PRESSENTER_HEIGHT / 2) * pResult->fShrink), 0.0f);

			//���_�J���[�̐ݒ�
			pVertex[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pResult->fBlink);  //D3DCOLOR_RGBA(��, ��, ��, �����x);
			pVertex[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pResult->fBlink);
			pVertex[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pResult->fBlink);
			pVertex[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pResult->fBlink);
			break;
		}
		pVertex += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResult->Unlock();

	if (GetKeyboardTrigger(DIK_BACKSPACE) == true || GetKeyboardTrigger(DIK_RETURN) == true)
	{
		SetFade(FADE_OUT, MODE_RANKING);
	}
}

//-----------------------------------------------------------------------------
// �^�C�g���̕`�揈��
//-----------------------------------------------------------------------------
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
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntR * 4, 2);
	}
}

//-----------------------------------------------------------------------------
// ���U���g�̉��o (�k���g��)
//-----------------------------------------------------------------------------
float ResultShrink(float fShrink)
{
	static bool bShrink = true;

	//�k���Ɗg��̐؂�ւ�
	if (bShrink == true)
	{
		fShrink -= RESULT_SHRINKSPD;
	}
	else if (bShrink == false)
	{
		fShrink += RESULT_SHRINKSPD;
	}

	//�k���g��̉������
	if (fShrink >= 1.0f)
	{
		bShrink = true;
	}
	else if (fShrink <= RESULT_SHRINKRANGE)
	{
		bShrink = false;
	}

	return fShrink;
}

//-----------------------------------------------------------------------------
// ���U���g�̉��o (�_��)
//-----------------------------------------------------------------------------
float ResultBlink(float fBlink)
{
	static bool bBlink = false;
	static bool bOnce = false;
	static DWORD dwTime;
	static DWORD dwCTime;

	//���ݎ�
	dwCTime = timeGetTime();

	//�ŏ��̎��Ԃ��L��
	if (bOnce == false)
	{
		dwTime = timeGetTime();
		bOnce = true;
	}

	//�_�Ő؂�ւ�
	if (bBlink == true)
	{
		fBlink = 0.0f;
	}
	else if (bBlink == false)
	{
		fBlink = 1.0f;
	}

	//���ݎ�����ŏ����������Ĕ���
	if ((dwCTime - dwTime) >= RESULT_BLINKTIMEFIXED)
	{
		bBlink = !bBlink;
		dwTime = timeGetTime();
	}

	return fBlink;
}