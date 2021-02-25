//-----------------------------------------------------------------------------
//
// �e�̏��� [shadow.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "shadow.h"
#include "model.h"
#include "player.h"

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// �e�̍\����
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXMATRIX mtxWorld;														// ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;															// �ʒu
	D3DXVECTOR3 rot;															// ����
	bool bUse;																	// �g�p���Ă��邩�ǂ���
} Shadow;

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffShadow = NULL;								// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;										// �|���S���̃e�N�X�`���ւ̃|�C���^
Shadow g_aShadow[SHADOW_MAX];													// �e�̏��
D3DXVECTOR3 g_posShadow;														// �ʒu
D3DXVECTOR3 g_rotShadow;														// ����
D3DXMATRIX g_mtxWorldShadow;													// ���[���h�}�g���b�N�X
int g_aShadowNum[SHADOW_MAX];

//-----------------------------------------------------------------------------
// �e�̏���������
//-----------------------------------------------------------------------------
HRESULT InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCntShadow = 0; nCntShadow < SHADOW_MAX; nCntShadow++)
	{
		g_aShadow[nCntShadow].bUse = false;
		g_aShadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	//g_posShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	//g_rotShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\shadow000.jpg", &g_pTextureShadow);

	//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * VERTEX_AMOUNT * SHADOW_MAX,		//�T�C�Y
		D3DUSAGE_WRITEONLY,									//
		FVF_VERTEX_3D,										//�t�H�[�}�b�g
		D3DPOOL_MANAGED,									//
		&g_pVtxBuffShadow,									//���_�o�b�t�@�ւ̃|�C���^
		NULL);

	VERTEX_3D *pVertex;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntShadow = 0; nCntShadow < SHADOW_MAX; nCntShadow++, pVertex+=4)
	{
		//���_���W�ݒ�
		pVertex[0].pos = D3DXVECTOR3(g_posShadow.x - 10.0f, g_posShadow.y, g_posShadow.z - 10.0f);
		pVertex[1].pos = D3DXVECTOR3(g_posShadow.x - 10.0f, g_posShadow.y, g_posShadow.z + 10.0f);
		pVertex[2].pos = D3DXVECTOR3(g_posShadow.x + 10.0f, g_posShadow.y, g_posShadow.z - 10.0f);
		pVertex[3].pos = D3DXVECTOR3(g_posShadow.x + 10.0f, g_posShadow.y, g_posShadow.z + 10.0f);

		//�@���x�N�g���̐ݒ�
		pVertex[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVertex[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVertex[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVertex[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVertex[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVertex[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVertex[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVertex[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

		//�e�N�X�`�����W�̐ݒ�
		pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f);
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffShadow->Unlock();

	//SetShadow(D3DXVECTOR3(0.0f, 0.0f, 0.0f), NULL, NULL);
	//SetShadow(D3DXVECTOR3(25.0f, 0.0f, 0.0f), NULL, NULL);
	g_aShadowNum[0] = SetShadow(D3DXVECTOR3(0.0f, 0.0f, 0.0f), NULL, NULL);

	return S_OK;
}

//-----------------------------------------------------------------------------
// �e�̏I������
//-----------------------------------------------------------------------------
void UninitShadow(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//-----------------------------------------------------------------------------
// �e�̍X�V����
//-----------------------------------------------------------------------------
void UpdateShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	Player *pPlayer = GetPlayer();
	D3DXVECTOR3 playerBodyPos;
	
	playerBodyPos = D3DXVECTOR3(pPlayer->aModel[0].mtxWorld._41, 0.0f, pPlayer->aModel[0].mtxWorld._43);
	

	SetPositionShadow(g_aShadowNum[0], playerBodyPos);
}

//-----------------------------------------------------------------------------
// �e�̕`�揈��
//-----------------------------------------------------------------------------
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�}�g���b�N�X

	//�f�o�C�X�擾
	pDevice = GetDevice();

	for (int nCntShadow = 0; nCntShadow < SHADOW_MAX; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == true)
		{
			//���Z�����̐ݒ�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			//���[���h�}�g���b�N�X�̏�����
			//D3DXMatrixIdentity(&g_mtxWorldShadow);
			D3DXMatrixIdentity(&g_aShadow[nCntShadow].mtxWorld);

			//�����𔽉f
			//D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotShadow.y, g_rotShadow.x, g_rotShadow.z);
			//D3DXMatrixMultiply(&g_mtxWorldShadow, &g_mtxWorldShadow, &mtxRot);
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			//D3DXMatrixTranslation(&mtxTrans, g_posShadow.x, g_posShadow.y, g_posShadow.z);
			//D3DXMatrixMultiply(&g_mtxWorldShadow, &g_mtxWorldShadow, &mtxTrans);
			D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCntShadow].pos.x, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			//pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldShadow);
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCntShadow].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureShadow);

			//�|���S���̕`��
			//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntShadow * 4, 2);

			//�ʏ퍇���ɖ߂�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
	}
}

//-----------------------------------------------------------------------------
// �e�̐ݒ�
//-----------------------------------------------------------------------------
int SetShadow(D3DXVECTOR3 pos, float fWidth, float fDepth)
{
	int nIdx = -1;

	for (int nCntShadow = 0; nCntShadow < SHADOW_MAX; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == false)
		{
			//�e�̈ʒu
			g_aShadow[nCntShadow].pos = pos;

			//�e�̕�

			//�e�̐[��

			//
			g_aShadow[nCntShadow].bUse = true;

			//�e�̔ԍ�����U��
			nIdx = nCntShadow;
			break;
		}
	}

	return nIdx;
}

//-----------------------------------------------------------------------------
// �e�̈ʒu�ړ�
//-----------------------------------------------------------------------------
void SetPositionShadow(int nIdx, D3DXVECTOR3 pos)
{
	g_aShadow[nIdx].pos = pos;
}

//-----------------------------------------------------------------------------
// �e������
//-----------------------------------------------------------------------------
void RemoveShadow(int nIdx)
{
}