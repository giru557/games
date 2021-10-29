//-----------------------------------------------------------------------------
//
// �n�ʂ̏��� [meshfield.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "meshfield.h"

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
#define MESHFIELD_14 (14)
#define MESHFIELD_DIS (100)
#define MESHFIELD_ROW (2)
#define MESHFIELD_COLUMN (2)

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffMeshfield = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshfield = NULL;					// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureMeshfield = NULL;						// �|���S���̃e�N�X�`���ւ̃|�C���^
D3DXVECTOR3 g_posMeshfield;											// �ʒu
D3DXVECTOR3 g_rotMeshfield;											// ����
D3DXMATRIX g_mtxWorldMeshfield;										// ���[���h�}�g���b�N�X

//-----------------------------------------------------------------------------
// �n�ʂ̏���������
//-----------------------------------------------------------------------------
HRESULT InitMeshfield(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	g_posMeshfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	g_rotMeshfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\ground.jpg" , &g_pTextureMeshfield);

	//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 9,					//�T�C�Y
								D3DUSAGE_WRITEONLY,						//
								FVF_VERTEX_3D,							//�t�H�[�}�b�g
								D3DPOOL_MANAGED,						//
								&g_pVtxBuffMeshfield,					//���_�o�b�t�@�ւ̃|�C���^
								NULL);

	VERTEX_3D *pVertex;
	
	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffMeshfield->Lock(0, 0, (void**)&pVertex, 0);

	//���_���W�ݒ�
	pVertex[0].pos = D3DXVECTOR3(g_posMeshfield.x - MESHFIELD_DIS, g_posMeshfield.y, g_posMeshfield.z + MESHFIELD_DIS);
	pVertex[1].pos = D3DXVECTOR3(g_posMeshfield.x, g_posMeshfield.y, g_posMeshfield.z + MESHFIELD_DIS);
	pVertex[2].pos = D3DXVECTOR3(g_posMeshfield.x + MESHFIELD_DIS, g_posMeshfield.y, g_posMeshfield.z + MESHFIELD_DIS);
	pVertex[3].pos = D3DXVECTOR3(g_posMeshfield.x - MESHFIELD_DIS, g_posMeshfield.y, g_posMeshfield.z);
	pVertex[4].pos = D3DXVECTOR3(g_posMeshfield.x, g_posMeshfield.y, g_posMeshfield.z);
	pVertex[5].pos = D3DXVECTOR3(g_posMeshfield.x + MESHFIELD_DIS, g_posMeshfield.y, g_posMeshfield.z);
	pVertex[6].pos = D3DXVECTOR3(g_posMeshfield.x - MESHFIELD_DIS, g_posMeshfield.y, g_posMeshfield.z - MESHFIELD_DIS);
	pVertex[7].pos = D3DXVECTOR3(g_posMeshfield.x, g_posMeshfield.y, g_posMeshfield.z - MESHFIELD_DIS);
	pVertex[8].pos = D3DXVECTOR3(g_posMeshfield.x + MESHFIELD_DIS, g_posMeshfield.y, g_posMeshfield.z - MESHFIELD_DIS);

	for (int nCntField = 0; nCntField < 9; nCntField++)
	{
		//�@���x�N�g���̐ݒ�
		pVertex[nCntField].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVertex[nCntField].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	//�e�N�X�`�����W�̐ݒ�
	pVertex[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVertex[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVertex[2].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVertex[3].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVertex[4].tex = D3DXVECTOR2(0.5f, 0.5f);
	pVertex[5].tex = D3DXVECTOR2(1.0f, 0.5f);
	pVertex[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVertex[7].tex = D3DXVECTOR2(0.5f, 1.0f);
	pVertex[8].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshfield->Unlock();

	//�C���f�b�N�X�o�b�t�@����
	pDevice->CreateIndexBuffer(
		sizeof(WORD) * 14,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshfield,
		NULL);

	WORD *pIdx;		// �C���f�b�N�X���ւ̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N���A�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshfield->Lock(0, 0, (void**)&pIdx, 0);

	//�ԍ��f�[�^�̐ݒ�
	pIdx[0] = 3;
	pIdx[1] = 0;
	pIdx[2] = 4;
	pIdx[3] = 1;
	pIdx[4] = 5;
	pIdx[5] = 2;
	pIdx[6] = 2;
	pIdx[7] = 6;
	pIdx[8] = 6;
	pIdx[9] = 3;
	pIdx[10] = 7;
	pIdx[11] = 4;
	pIdx[12] = 8;
	pIdx[13] = 5;

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffMeshfield->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
// �n�ʂ̏I������
//-----------------------------------------------------------------------------
void UninitMeshfield(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureMeshfield != NULL)
	{
		g_pTextureMeshfield->Release();
		g_pTextureMeshfield = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffMeshfield != NULL)
	{
		g_pVtxBuffMeshfield->Release();
		g_pVtxBuffMeshfield = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffMeshfield != NULL)
	{
		g_pIdxBuffMeshfield->Release();
		g_pIdxBuffMeshfield = NULL;
	}
}

//-----------------------------------------------------------------------------
// �n�ʂ̍X�V����
//-----------------------------------------------------------------------------
void UpdateMeshfield(void)
{
}

//-----------------------------------------------------------------------------
// �n�ʂ̕`�揈��
//-----------------------------------------------------------------------------
void DrawMeshfield(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�}�g���b�N�X

	//�f�o�C�X�擾
	pDevice = GetDevice();

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldMeshfield);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMeshfield.y, g_rotMeshfield.x, g_rotMeshfield.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshfield, &g_mtxWorldMeshfield, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_posMeshfield.x, g_posMeshfield.y, g_posMeshfield.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshfield, &g_mtxWorldMeshfield, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshfield);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMeshfield, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffMeshfield);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshfield);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		14,
		0,
		12);

}