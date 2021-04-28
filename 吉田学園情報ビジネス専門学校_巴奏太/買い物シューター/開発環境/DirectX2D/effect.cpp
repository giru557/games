//-----------------------------------------------------------------------------
//
// �����̏��� [explosion.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "effect.h"

//-----------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------
#define MAX_EFFECT (1280) //�G�t�F�N�g�̍ő吔

//-----------------------------------------------------------------------------
//�\���̐錾
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXCOLOR effCol;		//�G�t�F�N�g�̐F
	int nSize;				//�G�t�F�N�g�̃T�C�Y
	bool bUse;				//�g�p���Ă��邩
} EFFECT;

//-----------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;
EFFECT g_aEffect[MAX_EFFECT];						//�G�t�F�N�g�̏��
D3DXCOLOR g_colorEff;								//�G�t�F�N�g�F
float g_fAlphaShift;								//�G�t�F�N�g�������ɂȂ�X�s�[�h;

//-----------------------------------------------------------------------------
//�G�t�F�N�g�̏���������
//-----------------------------------------------------------------------------
HRESULT InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\effect000.jpg", &g_pTextureEffect);

	//EXPLOSION�̏�����
	for (int nCntEff = 0; nCntEff < MAX_EFFECT; nCntEff++)
	{
		g_aEffect[nCntEff].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEff].effCol = D3DXCOLOR(0.0f, 0.0f, 0.5f, 1.0f);
		g_aEffect[nCntEff].bUse = false;
	}

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * MAX_EFFECT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffEffect, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�̃��b�N
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVertex, 0);

	//���_���W
	for (int nCntEff = 0; nCntEff < MAX_EFFECT; nCntEff++)
	{
		//���_���W�̐ݒ�
		pVertex[0].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVertex[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVertex[2].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVertex[3].pos = D3DXVECTOR3(0, 0, 0.0f);

		//rhw�̐ݒ�
		pVertex[0].rhw = 1.0f;	//1.0f�Œ�
		pVertex[1].rhw = 1.0f;
		pVertex[2].rhw = 1.0f;
		pVertex[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVertex[0].col = D3DXCOLOR(0.0f, 0.0f, 0.5f, 1.0f);
		pVertex[1].col = D3DXCOLOR(0.0f, 0.0f, 0.5f, 1.0f);
		pVertex[2].col = D3DXCOLOR(0.0f, 0.0f, 0.5f, 1.0f);
		pVertex[3].col = D3DXCOLOR(0.0f, 0.0f, 0.5f, 1.0f);

		//UV�̐ݒ�
		pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVertex += 4; //���_�̃|�C���^�����炷
	}

	g_pVtxBuffEffect->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//�G�t�F�N�g�̏I������
//-----------------------------------------------------------------------------
void UninitEffect(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}
}

//-----------------------------------------------------------------------------
//�G�t�F�N�g�̍X�V����
//-----------------------------------------------------------------------------
void UpdateEffect(void)
{
	EFFECT *pEffect;
	VERTEX_2D *pVertex;

	pEffect = &g_aEffect[0];

	if (g_pVtxBuffEffect != NULL)
	{
		//���_�o�b�t�@�̃��b�N
		g_pVtxBuffEffect->Lock(0, 0, (void**)&pVertex, 0);

		for (int nCntEff = 0; nCntEff < MAX_EFFECT; nCntEff++, pEffect++)
		{
			if (pEffect->bUse == true)
			{
				//���_���W�̐ݒ�
				pVertex[0].pos = D3DXVECTOR3(g_aEffect[nCntEff].pos.x - g_aEffect[nCntEff].nSize, g_aEffect[nCntEff].pos.y + g_aEffect[nCntEff].nSize, 0.0f);
				pVertex[1].pos = D3DXVECTOR3(g_aEffect[nCntEff].pos.x - g_aEffect[nCntEff].nSize, g_aEffect[nCntEff].pos.y - g_aEffect[nCntEff].nSize, 0.0f);
				pVertex[2].pos = D3DXVECTOR3(g_aEffect[nCntEff].pos.x + g_aEffect[nCntEff].nSize, g_aEffect[nCntEff].pos.y + g_aEffect[nCntEff].nSize, 0.0f);
				pVertex[3].pos = D3DXVECTOR3(g_aEffect[nCntEff].pos.x + g_aEffect[nCntEff].nSize, g_aEffect[nCntEff].pos.y - g_aEffect[nCntEff].nSize, 0.0f);

				//���_�J���[�̐ݒ�
				pVertex[0].col = g_aEffect[nCntEff].effCol;
				pVertex[1].col = g_aEffect[nCntEff].effCol;
				pVertex[2].col = g_aEffect[nCntEff].effCol;
				pVertex[3].col = g_aEffect[nCntEff].effCol;

				g_aEffect[nCntEff].effCol.a -= g_fAlphaShift;
				
				if (g_aEffect[nCntEff].effCol.a < 0.0f)
				{
					g_aEffect[nCntEff].bUse = false;
					g_aEffect[nCntEff].effCol.a = 1.0f;
				}
			}
			pVertex += 4; //���_�̃|�C���^�����炷
		}

		//���_�o�b�t�@�̃A�����b�N
		g_pVtxBuffEffect->Unlock();
	}
}

//-----------------------------------------------------------------------------
//�G�t�F�N�g�̕`�揈��
//-----------------------------------------------------------------------------
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�����_�[�X�e�[�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		//�\�[�X�i�`�挳�j�̍������@�̐ݒ�
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);	//�f�X�e�B�l�[�V�����i�`���j�̍������@�̐ݒ�

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureEffect);

	for (int nCntExp = 0; nCntExp < MAX_EFFECT; nCntExp++)
	{
		if (g_aEffect[nCntExp].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExp * 4, 2);
		}
	}

	//�����_�[�X�e�[�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		//�\�[�X�i�`�挳�j�̍������@�̐ݒ�
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	//�f�X�e�B�l�[�V�����i�`���j�̍������@�̐ݒ�
}

//-----------------------------------------------------------------------------
//�G�t�F�N�g�̐ݒ�
//-----------------------------------------------------------------------------
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR effCol, int nSize, float fAlphaShift)
{
	EFFECT *pEffect;
	VERTEX_2D *pVertex;

	pEffect = &g_aEffect[0];

	//���_�o�b�t�@�̃��b�N
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntEff = 0; nCntEff < MAX_EFFECT; nCntEff++, pEffect++)
	{
		if (pEffect->bUse == false)
		{
			//�ʒu��ݒ�
			g_aEffect[nCntEff].pos = pos;

			//���_���W�̐ݒ�
			pVertex[0].pos = D3DXVECTOR3(g_aEffect[nCntEff].pos.x - g_aEffect[nCntEff].nSize, g_aEffect[nCntEff].pos.y + g_aEffect[nCntEff].nSize, 0.0f);
			pVertex[1].pos = D3DXVECTOR3(g_aEffect[nCntEff].pos.x - g_aEffect[nCntEff].nSize, g_aEffect[nCntEff].pos.y - g_aEffect[nCntEff].nSize, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_aEffect[nCntEff].pos.x + g_aEffect[nCntEff].nSize, g_aEffect[nCntEff].pos.y + g_aEffect[nCntEff].nSize, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_aEffect[nCntEff].pos.x + g_aEffect[nCntEff].nSize, g_aEffect[nCntEff].pos.y - g_aEffect[nCntEff].nSize, 0.0f);

			//�G�t�F�N�g�T�C�Y�̐ݒ�
			g_aEffect[nCntEff].nSize = nSize;

			//�G�t�F�N�g�F�̐ݒ�
			g_aEffect[nCntEff].effCol = effCol;

			//�G�t�F�N�g�������ɂȂ�X�s�[�h��ݒ�
			g_fAlphaShift = fAlphaShift;

			//�g�p���ɕύX
			pEffect->bUse = true;

			break;

		}
		//���_�̃|�C���^�����炷
		pVertex += 4;
	}
	g_pVtxBuffEffect->Unlock();
}
