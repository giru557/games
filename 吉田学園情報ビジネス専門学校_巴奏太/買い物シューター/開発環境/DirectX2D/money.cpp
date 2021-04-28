//-----------------------------------------------------------------------------
//
// �����̏��� [money.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "money.h"

//�}�N��
#define MONEY_DIGIT_MAX (2)
#define MONEY_LIMIT (99)
#define SPACE_BETWEEN_CHAR (35)

//-----------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffMoney = NULL;  //���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureMoney = NULL;		//�|���S���̃e�N�X�`���ւ̃|�C���^
D3DXVECTOR3 g_aPosMoney[MONEY_DIGIT_MAX];		//�����̃|���S���̈ʒu
int g_nMoney;									//���݂̏�����

//-----------------------------------------------------------------------------
//�����̏���������
//-----------------------------------------------------------------------------
HRESULT InitMoney(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	//�����̏�����
	g_nMoney = 0;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number001.png", &g_pTextureMoney);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * MONEY_DIGIT_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffMoney, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffMoney->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntMoney = 0; nCntMoney < MONEY_DIGIT_MAX; nCntMoney++)
	{
		//�ʒu
		g_aPosMoney[nCntMoney] = D3DXVECTOR3(1000, 600, 0.0f);

		//���_���W D3DXVECTOR3(X,Y, 0.0f);
		pVertex[0].pos = D3DXVECTOR3((g_aPosMoney[nCntMoney].x + -15) + nCntMoney * SPACE_BETWEEN_CHAR, g_aPosMoney[nCntMoney].y + 20, 0.0f);
		pVertex[1].pos = D3DXVECTOR3((g_aPosMoney[nCntMoney].x + -15) + nCntMoney * SPACE_BETWEEN_CHAR, g_aPosMoney[nCntMoney].y + -20, 0.0f);
		pVertex[2].pos = D3DXVECTOR3((g_aPosMoney[nCntMoney].x + 15) + nCntMoney * SPACE_BETWEEN_CHAR, g_aPosMoney[nCntMoney].y + 20, 0.0f);
		pVertex[3].pos = D3DXVECTOR3((g_aPosMoney[nCntMoney].x + 15) + nCntMoney * SPACE_BETWEEN_CHAR, g_aPosMoney[nCntMoney].y + -20, 0.0f);

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
		pVertex[2].tex = D3DXVECTOR2(1.0f * 0.1f, 1.0f);
		pVertex[3].tex = D3DXVECTOR2(1.0f * 0.1f, 0.0f);

		pVertex += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMoney->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//�����̏I������
//-----------------------------------------------------------------------------
void UninitMoney(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffMoney != NULL)
	{
		g_pVtxBuffMoney->Release();
		g_pVtxBuffMoney = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pTextureMoney != NULL)
	{
		g_pTextureMoney->Release();
		g_pTextureMoney = NULL;
	}
}

//-----------------------------------------------------------------------------
//�����̍X�V����
//-----------------------------------------------------------------------------
void UpdateMoney(void)
{
	VERTEX_2D *pVertex;
	int aNumber[2];

	if (g_nMoney > 99)
	{
		g_nMoney = 99;
	}

	aNumber[0] = g_nMoney % 100 / 10;
	aNumber[1] = g_nMoney % 10 / 1;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	if (g_pVtxBuffMoney != NULL)
	{
		g_pVtxBuffMoney->Lock(0, 0, (void**)&pVertex, 0);

		for (int nCntMoney = 0; nCntMoney < MONEY_DIGIT_MAX; nCntMoney++)
		{
			//���_���W D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3((g_aPosMoney[nCntMoney].x + -15) + nCntMoney * SPACE_BETWEEN_CHAR, g_aPosMoney[nCntMoney].y + 20, 0.0f);
			pVertex[1].pos = D3DXVECTOR3((g_aPosMoney[nCntMoney].x + -15) + nCntMoney * SPACE_BETWEEN_CHAR, g_aPosMoney[nCntMoney].y + -20, 0.0f);
			pVertex[2].pos = D3DXVECTOR3((g_aPosMoney[nCntMoney].x + 15) + nCntMoney * SPACE_BETWEEN_CHAR, g_aPosMoney[nCntMoney].y + 20, 0.0f);
			pVertex[3].pos = D3DXVECTOR3((g_aPosMoney[nCntMoney].x + 15) + nCntMoney * SPACE_BETWEEN_CHAR, g_aPosMoney[nCntMoney].y + -20, 0.0f);

			//���_���̐ݒ�
			pVertex[0].tex = D3DXVECTOR2(0.1f * (aNumber[nCntMoney]), 1.0f);
			pVertex[1].tex = D3DXVECTOR2(0.1f * (aNumber[nCntMoney]), 0.0f);
			pVertex[2].tex = D3DXVECTOR2(0.1f * (aNumber[nCntMoney] + 1), 1.0f);
			pVertex[3].tex = D3DXVECTOR2(0.1f * (aNumber[nCntMoney] + 1), 0.0f);

			pVertex += 4;	//���_�̃|�C���^���炵
		}
		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffMoney->Unlock();
	}
}

//-----------------------------------------------------------------------------
//�����̕`�揈��
//-----------------------------------------------------------------------------
void DrawMoney(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMoney, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMoney);

	for (int nCntMoney = 0; nCntMoney < MONEY_DIGIT_MAX; nCntMoney++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntMoney * 4, 2);
	}
}

//�����̒ǉ�
void AddMoney(int nValue)
{
	g_nMoney += nValue;
}

bool UseMoney(int nValue)
{
	if ((g_nMoney - nValue) >= 0)
	{
		g_nMoney -= nValue;
	}
	else 
	{
		return false;
	}

	return true;
}