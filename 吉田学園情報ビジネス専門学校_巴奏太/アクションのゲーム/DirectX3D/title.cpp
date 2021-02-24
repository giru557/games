//-----------------------------------------------------------------------------
//
// �^�C�g���̏��� [title.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "title.h"
#include "input.h"
#include "fade.h"

//-----------------------------------------------------------------------------
// �}�N��
//-----------------------------------------------------------------------------
#define TITLE_OBJ_AMOUNT (3)														//�^�C�g���̃I�u�W�F�N�g��
#define TITLE_BLINKTIME (1.5f)														//�_�ŊԊu (�b)
#define TITLE_BLINKTIMEFIXED (TITLE_BLINKTIME * 1000)								//�b����~���b�ɕϊ�

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
float TitleBlink(float fBlink);

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
TITLE g_aTitle[TITLE_OBJ_AMOUNT];												// TITLE�\���̂̏��
float g_fBlink;																	// �_��

//-----------------------------------------------------------------------------
// �^�C�g���̏���������
//-----------------------------------------------------------------------------
HRESULT InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�擾

	//TITLE�̏�����
	g_aTitle[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aTitle[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) + TITLE_LOGO_OFFSET, 0.0f);
	g_aTitle[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) + TITLE_PRESS_OFFSET, 0.0f);
	g_fBlink = 1.0f;

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\title_bg.jpg", &g_aTitle[0].pTexture);
	D3DXCreateTextureFromFileEx(pDevice, "data\\TEXTURE\\title_logo.png", TITLE_LOGO_WIDTH, TITLE_LOGO_HEIGHT, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0xFF000000, NULL, NULL, &g_aTitle[1].pTexture);
	D3DXCreateTextureFromFileEx(pDevice, "data\\TEXTURE\\title_press.png", TITLE_PRESS_WIDTH, TITLE_PRESS_HEIGHT, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0xFF000000, NULL, NULL, &g_aTitle[2].pTexture);
	//�t�H�[�}�b�g�p��
	VERTEX_2D *pVertexBG;		// �w�i�p
	VERTEX_2D *pVertexLOGO;		// ���S�p
	VERTEX_2D *pVertexPRESS;	// PRESSENTER�p

	for (int nCntCreate = 0; nCntCreate < TITLE_OBJ_AMOUNT; nCntCreate++)
	{
		//���_�o�b�t�@�̐���
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_aTitle[nCntCreate].pVtxBuff, NULL)))
		{
			return E_FAIL;
		}
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_aTitle[0].pVtxBuff->Lock(0, 0, (void**)&pVertexBG, 0);

	//���_���W D3DXVECTOR3(X,Y, 0.0f);
	pVertexBG[0].pos = D3DXVECTOR3(g_aTitle[0].pos.x, g_aTitle[0].pos.y + SCREEN_HEIGHT, 0.0f);
	pVertexBG[1].pos = D3DXVECTOR3(g_aTitle[0].pos.x, g_aTitle[0].pos.y, 0.0f);
	pVertexBG[2].pos = D3DXVECTOR3(g_aTitle[0].pos.x + SCREEN_WIDTH, g_aTitle[0].pos.y + SCREEN_HEIGHT, 0.0f);
	pVertexBG[3].pos = D3DXVECTOR3(g_aTitle[0].pos.x + SCREEN_WIDTH, g_aTitle[0].pos.y, 0.0f);

	//rhw�̐ݒ�
	for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertexBG[nCntVtx].rhw = 1.0f;	// 1.0f�Œ�

	//���_�J���[�̐ݒ�
	for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertexBG[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`���̐ݒ�
	pVertexBG[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVertexBG[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVertexBG[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVertexBG[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_aTitle[0].pVtxBuff->Unlock();

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_aTitle[1].pVtxBuff->Lock(0, 0, (void**)&pVertexLOGO, 0);

	//���_���W D3DXVECTOR3(X,Y, 0.0f);
	pVertexLOGO[0].pos = D3DXVECTOR3(g_aTitle[1].pos.x - (TITLE_LOGO_WIDTH / 2), g_aTitle[1].pos.y + (TITLE_LOGO_HEIGHT / 2), 0.0f);
	pVertexLOGO[1].pos = D3DXVECTOR3(g_aTitle[1].pos.x - (TITLE_LOGO_WIDTH / 2), g_aTitle[1].pos.y - (TITLE_LOGO_HEIGHT / 2), 0.0f);
	pVertexLOGO[2].pos = D3DXVECTOR3(g_aTitle[1].pos.x + (TITLE_LOGO_WIDTH / 2), g_aTitle[1].pos.y + (TITLE_LOGO_HEIGHT / 2), 0.0f);
	pVertexLOGO[3].pos = D3DXVECTOR3(g_aTitle[1].pos.x + (TITLE_LOGO_WIDTH / 2), g_aTitle[1].pos.y - (TITLE_LOGO_HEIGHT / 2), 0.0f);

	//rhw�̐ݒ�
	for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertexLOGO[nCntVtx].rhw = 1.0f;	// 1.0f�Œ�

	//���_�J���[�̐ݒ�
	for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertexLOGO[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`���̐ݒ�
	pVertexLOGO[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVertexLOGO[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVertexLOGO[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVertexLOGO[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_aTitle[1].pVtxBuff->Unlock();

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_aTitle[2].pVtxBuff->Lock(0, 0, (void**)&pVertexPRESS, 0);

	//���_���W D3DXVECTOR3(X,Y, 0.0f);
	pVertexPRESS[0].pos = D3DXVECTOR3(g_aTitle[2].pos.x - (TITLE_PRESS_WIDTH / 2), g_aTitle[2].pos.y + (TITLE_PRESS_HEIGHT / 2), 0.0f);
	pVertexPRESS[1].pos = D3DXVECTOR3(g_aTitle[2].pos.x - (TITLE_PRESS_WIDTH / 2), g_aTitle[2].pos.y - (TITLE_PRESS_HEIGHT / 2), 0.0f);
	pVertexPRESS[2].pos = D3DXVECTOR3(g_aTitle[2].pos.x + (TITLE_PRESS_WIDTH / 2), g_aTitle[2].pos.y + (TITLE_PRESS_HEIGHT / 2), 0.0f);
	pVertexPRESS[3].pos = D3DXVECTOR3(g_aTitle[2].pos.x + (TITLE_PRESS_WIDTH / 2), g_aTitle[2].pos.y - (TITLE_PRESS_HEIGHT / 2), 0.0f);

	//rhw�̐ݒ�
	for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertexPRESS[nCntVtx].rhw = 1.0f;	// 1.0f�Œ�

	//���_�J���[�̐ݒ�
	for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertexPRESS[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`���̐ݒ�
	pVertexPRESS[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVertexPRESS[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVertexPRESS[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVertexPRESS[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_aTitle[2].pVtxBuff->Unlock();

	//�t�F�[�h�C��
	SetFade(FADE_IN, MODE_TITLE);

	return S_OK;
}

//-----------------------------------------------------------------------------
// �^�C�g���̏I������
//-----------------------------------------------------------------------------
void UninitTitle(void)
{
	for (int nCntObj = 0; nCntObj < TITLE_OBJ_AMOUNT; nCntObj++)
	{
		//���_�o�b�t�@�̊J��
		if (g_aTitle[nCntObj].pVtxBuff != NULL)
		{
			g_aTitle[nCntObj].pVtxBuff->Release();
			g_aTitle[nCntObj].pVtxBuff = NULL;
		}

		//�e�N�X�`���̊J��
		if (g_aTitle[nCntObj].pTexture != NULL)
		{
			g_aTitle[nCntObj].pTexture->Release();
			g_aTitle[nCntObj].pTexture = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
// �^�C�g���̍X�V����
//-----------------------------------------------------------------------------
void UpdateTitle(void)
{
	VERTEX_2D *pVertexPRESS;

	//�_��
	g_fBlink = TitleBlink(g_fBlink);

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_aTitle[2].pVtxBuff->Lock(0, 0, (void**)&pVertexPRESS, 0);

	//���_�J���[�̐ݒ�
	for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertexPRESS[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fBlink);

	//���_�o�b�t�@���A�����b�N����
	g_aTitle[2].pVtxBuff->Unlock();

	//�J��
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		SetFade(FADE_OUT, MODE_GAME);
	}
}

//-----------------------------------------------------------------------------
// �^�C�g���̕`�揈��
//-----------------------------------------------------------------------------
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�擾

	for (int nCntObj = 0; nCntObj < TITLE_OBJ_AMOUNT; nCntObj++)
	{
		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_aTitle[nCntObj].pVtxBuff, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_aTitle[nCntObj].pTexture);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

// �^�C�g���̉��o (�_��)
float TitleBlink(float fBlink)
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
	if ((dwCTime - dwTime) >= TITLE_BLINKTIMEFIXED)
	{
		bBlink = !bBlink;
		dwTime = timeGetTime();
	}

	return fBlink;
}