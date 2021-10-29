//-----------------------------------------------------------------------------
//
// �`���[�g���A���̏��� [Tutorial.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "Tutorial.h"
#include "input.h"
#include "fade.h"

//-----------------------------------------------------------------------------
// �}�N��
//-----------------------------------------------------------------------------
#define TUTORIAL_POLYGON (1)													// �g�p����|���S����
#define TUTORIAL_WIDTH (SCREEN_WIDTH)											// ��
#define TUTORIAL_HEIGHT (SCREEN_HEIGHT)											// ����

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffTutorial = NULL;								// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureTutorial = { NULL };									// �|���S���̃e�N�X�`���ւ̃|�C���^
D3DXVECTOR3 g_posTutorial;														// �`���[�g���A���摜�̈ʒu

//-----------------------------------------------------------------------------
// �`���[�g���A���̏���������
//-----------------------------------------------------------------------------
HRESULT InitTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\tutorial.png", &g_pTextureTutorial);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * TUTORIAL_POLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTutorial, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVertex, 0);

	//���_���W D3DXVECTOR3(X,Y, 0.0f);
	pVertex[0].pos = D3DXVECTOR3(g_posTutorial.x, g_posTutorial.y + TUTORIAL_HEIGHT, 0.0f);
	pVertex[1].pos = D3DXVECTOR3(g_posTutorial.x, g_posTutorial.y, 0.0f);
	pVertex[2].pos = D3DXVECTOR3(g_posTutorial.x + TUTORIAL_WIDTH, g_posTutorial.y + TUTORIAL_HEIGHT, 0.0f);
	pVertex[3].pos = D3DXVECTOR3(g_posTutorial.x + TUTORIAL_WIDTH, g_posTutorial.y, 0.0f);

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
	pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTutorial->Unlock();

	//�t�F�[�h�C��
	SetFade(FADE_IN, MODE_GAME);

	return S_OK;
}

//-----------------------------------------------------------------------------
// �`���[�g���A���̏I������
//-----------------------------------------------------------------------------
void UninitTutorial(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pTextureTutorial != NULL)
	{
		g_pTextureTutorial->Release();
		g_pTextureTutorial = NULL;
	}

}

//-----------------------------------------------------------------------------
// �`���[�g���A���̍X�V����
//-----------------------------------------------------------------------------
void UpdateTutorial(void)
{
	VERTEX_2D *pVertex;

	if (g_pVtxBuffTutorial != NULL)
	{
		//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
		g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVertex, 0);


		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffTutorial->Unlock();
	}

	//�L�[����������
	if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_BACKSPACE) == true)
	{
		//�t�F�[�h�A�E�g
		SetFade(FADE_OUT, MODE_GAME);
	}
}

//-----------------------------------------------------------------------------
// �`���[�g���A���̕`�揈��
//-----------------------------------------------------------------------------
void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTutorial);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}