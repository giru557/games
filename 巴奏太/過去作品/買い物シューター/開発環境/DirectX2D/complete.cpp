//-----------------------------------------------------------------------------
//
// �X�e�[�W�R���v���[�g�̏��� [complete.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "complete.h"
#include "sound.h"

//�}�N��
#define COMPLETE_POLYGON (2)

//-----------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffComp = NULL;						//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureComp[COMPLETE_POLYGON] = { NULL };		//�|���S���̃e�N�X�`���ւ̃|�C���^
bool g_bDrawBG;														//�X�e�[�W�R���v���[�g��\�����邩

//-----------------------------------------------------------------------------
//�X�e�[�W�R���v���[�g�̏���������
//-----------------------------------------------------------------------------
HRESULT InitComplete(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	g_bDrawBG = false;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\stage_completedBG.png", &g_pTextureComp[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\stage_completed.png", &g_pTextureComp[1]);


	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * COMPLETE_POLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffComp, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffComp->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntC = 0; nCntC < COMPLETE_POLYGON; nCntC++)
	{
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
		pVertex[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);  //D3DCOLOR_RGBA(��, ��, ��, �����x);
		pVertex[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVertex[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVertex[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//���_���̐ݒ�
		pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVertex += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffComp->Unlock();

	PlaySound(SOUND_LABEL_SE_STAGECOMP);

	return S_OK;
}

//-----------------------------------------------------------------------------
//�X�e�[�W�R���v���[�g�̏I������
//-----------------------------------------------------------------------------
void UninitComplete(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffComp != NULL)
	{
		g_pVtxBuffComp->Release();
		g_pVtxBuffComp = NULL;
	}

	for (int nCntC = 0; nCntC < COMPLETE_POLYGON; nCntC++)
	{
		//�e�N�X�`���̊J��
		if (g_pTextureComp[nCntC] != NULL)
		{
			g_pTextureComp[nCntC]->Release();
			g_pTextureComp[nCntC] = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
//�X�e�[�W�R���v���[�g�̍X�V����
//-----------------------------------------------------------------------------
void UpdateComplete(void)
{
	VERTEX_2D *pVertex;
	bool bBlink;

	if (g_bDrawBG == true)
	{
		//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
		g_pVtxBuffComp->Lock(0, 0, (void**)&pVertex, 0);


		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffComp->Unlock();
	}
}

//-----------------------------------------------------------------------------
//�X�e�[�W�R���v���[�g�̕`�揈��

//-----------------------------------------------------------------------------
void DrawComplete(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffComp, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntC = 0; nCntC < COMPLETE_POLYGON; nCntC++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureComp[nCntC]);

		if (g_bDrawBG == true)
		{
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntC * 4, 2);
		}
	}
}

//�X�e�[�W�R���v���[�g�\��
void ShowStageComplete(void)
{
	g_bDrawBG = true;
}