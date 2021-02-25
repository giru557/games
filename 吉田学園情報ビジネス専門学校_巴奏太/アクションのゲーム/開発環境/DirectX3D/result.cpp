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

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
RESULT g_result;	// ���U���g�̏��

//-----------------------------------------------------------------------------
// ���U���g�̏���������
//-----------------------------------------------------------------------------
HRESULT InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVertex;

	//���U���g�\���̂̏�����
	g_result.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\gameclear.jpg", &g_result.pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\gameover.jpg", &g_result.pTexture[1]);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_result.pVtxBuff, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_result.pVtxBuff->Lock(0, 0, (void**)&pVertex, 0);

	//���_���W D3DXVECTOR3(X,Y, 0.0f);
	pVertex[0].pos = D3DXVECTOR3(g_result.pos.x, g_result.pos.y + SCREEN_HEIGHT, 0.0f);
	pVertex[1].pos = D3DXVECTOR3(g_result.pos.x, g_result.pos.y, 0.0f);
	pVertex[2].pos = D3DXVECTOR3(g_result.pos.x + SCREEN_WIDTH, g_result.pos.y + SCREEN_HEIGHT, 0.0f);
	pVertex[3].pos = D3DXVECTOR3(g_result.pos.x + SCREEN_WIDTH, g_result.pos.y, 0.0f);

	//rhw�̐ݒ�
	for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertex[nCntVtx].rhw = 1.0f;	// 1.0f�Œ�

	//���_�J���[�̐ݒ�
	for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertex[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//���_���̐ݒ�
	pVertex[0].tex = D3DXVECTOR2(0.0, 1.0);
	pVertex[1].tex = D3DXVECTOR2(0.0, 0.0);
	pVertex[2].tex = D3DXVECTOR2(1.0, 1.0);
	pVertex[3].tex = D3DXVECTOR2(1.0, 0.0);

	//���_�o�b�t�@���A�����b�N����
	g_result.pVtxBuff->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
// ���U���g�̏I������
//-----------------------------------------------------------------------------
void UninitResult(void)
{
	//���_�o�b�t�@�̊J��
	if (g_result.pVtxBuff != NULL)
	{
		g_result.pVtxBuff->Release();
		g_result.pVtxBuff = NULL;
	}

	for (int nCntTex = 0; nCntTex < 2; nCntTex++)
	{
		//�e�N�X�`���̊J��
		if (g_result.pTexture[nCntTex] != NULL)
		{
			g_result.pTexture[nCntTex]->Release();
			g_result.pTexture[nCntTex] = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
// ���U���g�̍X�V����
//-----------------------------------------------------------------------------
void UpdateResult(void)
{
	if (GetKeyboardTrigger(DIK_BACKSPACE) == true || GetKeyboardTrigger(DIK_RETURN) == true)
	{
		SetFade(FADE_OUT, MODE_TITLE);
	}
}

//-----------------------------------------------------------------------------
// �^�C�g���̕`�揈��
//-----------------------------------------------------------------------------
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_result.pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�Q�[���I�[�o�[��������
	if (g_result.bGameOver == true)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_result.pTexture[1]);
	}
	else
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_result.pTexture[0]);
	}

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//-----------------------------------------------------------------------------
// ���U���g���̎擾
//-----------------------------------------------------------------------------
RESULT *GetResult(void)
{
	return &g_result;
}