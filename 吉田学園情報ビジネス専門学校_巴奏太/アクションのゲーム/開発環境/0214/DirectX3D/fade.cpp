//-----------------------------------------------------------------------------
//
// �t�F�[�h�̏��� [fade.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "fade.h"

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffFade = NULL;  //���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureFade = NULL;		//�|���S���̃e�N�X�`���ւ̃|�C���^
FADE g_fade;	//�t�F�[�h�̏��
MODE g_modeNext; //���̃��[�h(���)
D3DXCOLOR g_colorFade; //�t�F�[�h�F

//-----------------------------------------------------------------------------
// �t�F�[�h�̏���������
//-----------------------------------------------------------------------------
HRESULT InitFade(MODE modeNext)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	g_fade = FADE_IN;
	g_modeNext = modeNext;
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffFade, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVertex, 0);

	//���_���W D3DXVECTOR3(X,Y, 0.0f);
	pVertex[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);		 //TRIANGLESTRIP�Ŏl�p
	pVertex[1].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVertex[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	pVertex[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

	//rhw�̐ݒ�
	pVertex[0].rhw = 1.0f;	//1.0f�Œ�
	pVertex[1].rhw = 1.0f;
	pVertex[2].rhw = 1.0f;
	pVertex[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVertex[0].col = g_colorFade;
	pVertex[1].col = g_colorFade;
	pVertex[2].col = g_colorFade;
	pVertex[3].col = g_colorFade;

	//���_���̐ݒ�
	pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffFade->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
// �t�F�[�h�̏I������
//-----------------------------------------------------------------------------
void UninitFade(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pTextureFade != NULL)
	{
		g_pTextureFade->Release();
		g_pTextureFade = NULL;
	}
}

//-----------------------------------------------------------------------------
// �t�F�[�h�̍X�V����
//-----------------------------------------------------------------------------
void UpdateFade(void)
{
	VERTEX_2D *pVertex;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVertex, 0);

	//���_�J���[�̐ݒ�
	pVertex[0].col = g_colorFade;
	pVertex[1].col = g_colorFade;
	pVertex[2].col = g_colorFade;
	pVertex[3].col = g_colorFade;
	
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffFade->Unlock();

	switch (g_fade)
	{
	case FADE_IN:
		g_colorFade.a -= 0.02f;

		if (g_colorFade.a <= 0.0f)
		{
			g_fade = FADE_NONE;
		}

		break;

	case FADE_OUT:
		g_colorFade.a += 0.02f;

		if (g_colorFade.a >= 1.0f)
		{
			SetMode(g_modeNext);

			g_fade = FADE_IN;
		}
		break;

	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// �t�F�[�h�̕`�揈��
//-----------------------------------------------------------------------------
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureFade);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//-----------------------------------------------------------------------------
// �Z�b�g�t�F�[�h
//-----------------------------------------------------------------------------
void SetFade(FADE fade, MODE modeNext)
{
	//�t�F�[�h�̎�ނ��󂯎��
	g_fade = fade;

	//���̃��[�h�̎�ނ��󂯎��
	g_modeNext = modeNext;
}