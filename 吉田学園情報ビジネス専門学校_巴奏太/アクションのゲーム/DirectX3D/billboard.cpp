//-----------------------------------------------------------------------------
//
// �r���{�[�h�̏��� [billboard.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "billboard.h"

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffBillboard = NULL;									// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureBillboard = NULL;											// �|���S���̃e�N�X�`���ւ̃|�C���^
D3DXVECTOR3 g_posBillboard;																// �ʒu
D3DXMATRIX g_mtxWorldBillboard;															// ���[���h�}�g���b�N�X

//-----------------------------------------------------------------------------
// �r���{�[�h�̏���������
//-----------------------------------------------------------------------------
HRESULT InitBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	g_posBillboard = D3DXVECTOR3(0.0f, 50, 25.0f);		// �ʒu

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\bullet000.png" , &g_pTextureBillboard);

	//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VERTEX_AMOUNT,					//�T�C�Y
								D3DUSAGE_WRITEONLY,									//
								FVF_VERTEX_3D,										//�t�H�[�}�b�g
								D3DPOOL_MANAGED,									//
								&g_pVtxBuffBillboard,									//���_�o�b�t�@�ւ̃|�C���^
								NULL);

	VERTEX_3D *pVertex;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVertex, 0);

	//���_���W�ݒ�
	pVertex[0].pos = D3DXVECTOR3(g_posBillboard.x - BILLBOARD_WIDTH, g_posBillboard.y, g_posBillboard.z);
	pVertex[1].pos = D3DXVECTOR3(g_posBillboard.x - BILLBOARD_WIDTH, g_posBillboard.y + BILLBOARD_HEIGHT, g_posBillboard.z);
	pVertex[2].pos = D3DXVECTOR3(g_posBillboard.x + BILLBOARD_WIDTH, g_posBillboard.y, g_posBillboard.z);
	pVertex[3].pos = D3DXVECTOR3(g_posBillboard.x + BILLBOARD_WIDTH, g_posBillboard.y + BILLBOARD_HEIGHT, g_posBillboard.z);

	//�@���x�N�g���̐ݒ�
	pVertex[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVertex[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVertex[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVertex[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//���_�J���[�̐ݒ�
	pVertex[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVertex[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVertex[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVertex[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBillboard->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
// �r���{�[�h�̏I������
//-----------------------------------------------------------------------------
void UninitBillboard(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBillboard != NULL)
	{
		g_pTextureBillboard->Release();
		g_pTextureBillboard = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//-----------------------------------------------------------------------------
// �|���S���̍X�V����
//-----------------------------------------------------------------------------
void UpdateBillboard(void)
{

}

//-----------------------------------------------------------------------------
// �r���{�[�h�̕`�揈��
//-----------------------------------------------------------------------------
void DrawBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxView;					// �r���[�}�g���b�N�X�擾
	D3DXMATRIX mtxTrans;				// �v�Z�p�}�g���b�N�X

	//�f�o�C�X�擾
	pDevice = GetDevice();

	//��r����߂ď�Ɏ�O�ɕ`��
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldBillboard);

	//�r���[�}�g���b�N�X���擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
	D3DXMatrixInverse(&g_mtxWorldBillboard, NULL, &mtxView);
	g_mtxWorldBillboard._41 = 0.0f;
	g_mtxWorldBillboard._42 = 0.0f;
	g_mtxWorldBillboard._43 = 0.0f;

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_posBillboard.x, g_posBillboard.y, g_posBillboard.z);
	D3DXMatrixMultiply(&g_mtxWorldBillboard, &g_mtxWorldBillboard, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBillboard);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBillboard);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//�����l�ɖ߂�
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

}