//-----------------------------------------------------------------------------
//
// �ǂ̏��� [wall.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "wall.h"
#include "input.h"

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
#define SPLIT_U (4.0f)
#define SPLIT_V (3.0f)
#define WALL_VERTICES (9)
//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
Wall g_aWall[WALL_MAX];															// �ǂ̏��
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;										// �ǂ̃e�N�X�`��
D3DXMATRIX g_mtxWorldWall;														// ���[���h�}�g���b�N�X

//-----------------------------------------------------------------------------
// �|���S���̏���������
//-----------------------------------------------------------------------------
HRESULT InitWall(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\skybox.png", &g_pTextureWall);

	for (int nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{
		g_aWall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCntWall].bUse = false;


		//���_�o�b�t�@����
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * WALL_VERTICES * WALL_MAX,		//�T�C�Y
			D3DUSAGE_WRITEONLY,									//
			FVF_VERTEX_3D,										//�t�H�[�}�b�g
			D3DPOOL_MANAGED,									//
			&g_aWall[nCntWall].pVtx,									//���_�o�b�t�@�ւ̃|�C���^
			NULL);

		VERTEX_3D *pVertex;

		//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
		g_aWall[nCntWall].pVtx->Lock(0, 0, (void**)&pVertex, 0);

		for (int nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
		{
			//���_���W�ݒ� (��������E��O�ɏ���)
			pVertex[0].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x - (WALL_WIDTH / 2), g_aWall[nCntWall].pos.y + WALL_HEIGHT, g_aWall[nCntWall].pos.z);
			pVertex[1].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y + WALL_HEIGHT, g_aWall[nCntWall].pos.z);
			pVertex[2].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x + (WALL_WIDTH / 2), g_aWall[nCntWall].pos.y + WALL_HEIGHT, g_aWall[nCntWall].pos.z);
			pVertex[3].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x - (WALL_WIDTH / 2), g_aWall[nCntWall].pos.y + (WALL_HEIGHT / 2), g_aWall[nCntWall].pos.z);
			pVertex[4].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y + (WALL_HEIGHT / 2), g_aWall[nCntWall].pos.z);
			pVertex[5].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x + (WALL_WIDTH / 2), g_aWall[nCntWall].pos.y + (WALL_HEIGHT / 2), g_aWall[nCntWall].pos.z);
			pVertex[6].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x - (WALL_WIDTH / 2), g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
			pVertex[7].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
			pVertex[8].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x + (WALL_WIDTH / 2), g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);

			for (int nCnt = 0; nCnt < 9; nCnt++)
			{
				//�@���̐ݒ�
				pVertex[nCnt].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

				//���_�J���[�̐ݒ�
				pVertex[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
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
		}

		//���_�o�b�t�@���A�����b�N����
		g_aWall[nCntWall].pVtx->Unlock();

		//�C���f�b�N�X�o�b�t�@����
		pDevice->CreateIndexBuffer(
			sizeof(WORD) * 14,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&g_aWall[nCntWall].pIdxBuff,
			NULL);

		WORD *pIdx;	// �C���f�b�N�X���ւ̃|�C���^

		//�C���f�b�N�X�o�b�t�@�����b�N���A�ԍ��f�[�^�ւ̃|�C���^���擾
		g_aWall[nCntWall].pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		//�ԍ��f�[�^�ݒ� (��M�����ɂȂ鏇��)
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

		//�C���f�b�N�X�o�b�t�@���A�����b�N
		g_aWall[nCntWall].pIdxBuff->Unlock();
	}

	SetWall(D3DXVECTOR3(0.0f, -500.0f, 1000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 2000, 2000);
	SetWall(D3DXVECTOR3(-1000.0f, -500.0f, 0.0f), D3DXVECTOR3(0.0f, D3DXToRadian(-90.0f), 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 2000, 2000);
	SetWall(D3DXVECTOR3(1000.0f, -500.0f, 0.0f), D3DXVECTOR3(0.0f, D3DXToRadian(90.0f), 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 2000, 2000);
	SetWall(D3DXVECTOR3(0.0f, -500.0f, -1000.0f), D3DXVECTOR3(0.0f, D3DXToRadian(180.0f), 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 2000, 2000);
	return S_OK;
}

//-----------------------------------------------------------------------------
// �|���S���̏I������
//-----------------------------------------------------------------------------
void UninitWall(void)
{
	//�e�N�X�`���̔j��
	//if (g_pTextureWall != NULL)
	//{
	//	g_pTextureWall->Release();
	//	g_pTextureWall = NULL;
	//}

	for (int nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{
		//���_�o�b�t�@�̔j��
		if (g_aWall[nCntWall].pVtx != NULL)
		{
			g_aWall[nCntWall].pVtx->Release();
			g_aWall[nCntWall].pVtx = NULL;
		}

		//�C���f�b�N�X�o�b�t�@�̔j��
		if (g_aWall[nCntWall].pIdxBuff != NULL)
		{
			g_aWall[nCntWall].pIdxBuff->Release();
			g_aWall[nCntWall].pIdxBuff = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
// �|���S���̍X�V����
//-----------------------------------------------------------------------------
void UpdateWall(void)
{
	for (int nCntWall = 0; nCntWall < 4; nCntWall++)
	{
		VERTEX_3D *pVertex;

		//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
		g_aWall[nCntWall].pVtx->Lock(0, 0, (void**)&pVertex, 0);

		switch (nCntWall)
		{
		case 0:
			//���ʂ̃e�N�X�`��
			pVertex[0].tex = D3DXVECTOR2(1.0f / SPLIT_U, 1.0f / SPLIT_V);
			pVertex[1].tex = D3DXVECTOR2(1.5f / SPLIT_U, 1.0f / SPLIT_V);
			pVertex[2].tex = D3DXVECTOR2(2.0f / SPLIT_U, 1.0f / SPLIT_V);
			pVertex[3].tex = D3DXVECTOR2(1.0f / SPLIT_U, 1.5f / SPLIT_V);
			pVertex[4].tex = D3DXVECTOR2(1.5f / SPLIT_U, 1.5f / SPLIT_V);
			pVertex[5].tex = D3DXVECTOR2(2.0f / SPLIT_U, 1.5f / SPLIT_V);
			pVertex[6].tex = D3DXVECTOR2(1.0f / SPLIT_U, 2.0f / SPLIT_V);
			pVertex[7].tex = D3DXVECTOR2(1.5f / SPLIT_U, 2.0f / SPLIT_V);
			pVertex[8].tex = D3DXVECTOR2(2.0f / SPLIT_U, 2.0f / SPLIT_V);
			break;
		
		case 1:
			//���̃e�N�X�`��
			pVertex[0].tex = D3DXVECTOR2(0.0f / SPLIT_U, 1.0f / SPLIT_V);
			pVertex[1].tex = D3DXVECTOR2(0.5f / SPLIT_U, 1.0f / SPLIT_V);
			pVertex[2].tex = D3DXVECTOR2(1.0f / SPLIT_U, 1.0f / SPLIT_V);
			pVertex[3].tex = D3DXVECTOR2(0.0f / SPLIT_U, 1.5f / SPLIT_V);
			pVertex[4].tex = D3DXVECTOR2(0.5f / SPLIT_U, 1.5f / SPLIT_V);
			pVertex[5].tex = D3DXVECTOR2(1.0f / SPLIT_U, 1.5f / SPLIT_V);
			pVertex[6].tex = D3DXVECTOR2(0.5f / SPLIT_U, 2.0f / SPLIT_V);
			pVertex[7].tex = D3DXVECTOR2(1.0f / SPLIT_U, 2.0f / SPLIT_V);
			pVertex[8].tex = D3DXVECTOR2(1.5f / SPLIT_U, 2.0f / SPLIT_V);
			break;

		case 2:
			//�E�̃e�N�X�`��
			pVertex[0].tex = D3DXVECTOR2(2.0f / SPLIT_U, 1.0f / SPLIT_V);
			pVertex[1].tex = D3DXVECTOR2(2.5f / SPLIT_U, 1.0f / SPLIT_V);
			pVertex[2].tex = D3DXVECTOR2(3.0f / SPLIT_U, 1.0f / SPLIT_V);
			pVertex[3].tex = D3DXVECTOR2(2.0f / SPLIT_U, 1.5f / SPLIT_V);
			pVertex[4].tex = D3DXVECTOR2(2.5f / SPLIT_U, 1.5f / SPLIT_V);
			pVertex[5].tex = D3DXVECTOR2(3.0f / SPLIT_U, 1.5f / SPLIT_V);
			pVertex[6].tex = D3DXVECTOR2(2.0f / SPLIT_U, 2.0f / SPLIT_V);
			pVertex[7].tex = D3DXVECTOR2(2.5f / SPLIT_U, 2.0f / SPLIT_V);
			pVertex[8].tex = D3DXVECTOR2(3.0f / SPLIT_U, 2.0f / SPLIT_V);
			break;

		case 3:
			//�E�̃e�N�X�`��
			pVertex[0].tex = D3DXVECTOR2(3.0f / SPLIT_U, 1.0f / SPLIT_V);
			pVertex[1].tex = D3DXVECTOR2(3.5f / SPLIT_U, 1.0f / SPLIT_V);
			pVertex[2].tex = D3DXVECTOR2(4.0f / SPLIT_U, 1.0f / SPLIT_V);
			pVertex[3].tex = D3DXVECTOR2(3.0f / SPLIT_U, 1.5f / SPLIT_V);
			pVertex[4].tex = D3DXVECTOR2(3.5f / SPLIT_U, 1.5f / SPLIT_V);
			pVertex[5].tex = D3DXVECTOR2(4.0f / SPLIT_U, 1.5f / SPLIT_V);
			pVertex[6].tex = D3DXVECTOR2(3.0f / SPLIT_U, 2.0f / SPLIT_V);
			pVertex[7].tex = D3DXVECTOR2(3.5f / SPLIT_U, 2.0f / SPLIT_V);
			pVertex[8].tex = D3DXVECTOR2(4.0f / SPLIT_U, 2.0f / SPLIT_V);
			break;

		default:
			break;
		}


		//�C���f�b�N�X�o�b�t�@���A�����b�N
		g_aWall[nCntWall].pVtx->Unlock();
	}
}

//-----------------------------------------------------------------------------
// �|���S���̕`�揈��
//-----------------------------------------------------------------------------
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�}�g���b�N�X

	//�f�o�C�X�擾
	pDevice = GetDevice();

	for (int nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aWall[nCntWall].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aWall[nCntWall].rot.y, g_aWall[nCntWall].rot.x, g_aWall[nCntWall].rot.z);
			D3DXMatrixMultiply(&g_aWall[nCntWall].mtxWorld, &g_aWall[nCntWall].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
			D3DXMatrixMultiply(&g_aWall[nCntWall].mtxWorld, &g_aWall[nCntWall].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aWall[nCntWall].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_aWall[nCntWall].pVtx, 0, sizeof(VERTEX_3D));

			//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_aWall[nCntWall].pIdxBuff);

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureWall);

			//�|���S���̕`��
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,
				0,
				0,
				14,
				0,
				12);
		}
	}
}

//-----------------------------------------------------------------------------
// �ǂ̏��擾
//-----------------------------------------------------------------------------
Wall *GetWall(void)
{
	return &g_aWall[0];
}

//-----------------------------------------------------------------------------
// �ǂ̐ݒu
//-----------------------------------------------------------------------------
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 nor, float fWidth, float fHeight)
{
	VERTEX_3D *pVertex;

	for (int nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == false)
		{
			//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
			g_aWall[nCntWall].pVtx->Lock(0, 0, (void**)&pVertex, 0);

			//���_���W�ݒ� (��������E��O�ɏ���)
			pVertex[0].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x - (fWidth / 2), g_aWall[nCntWall].pos.y + fHeight, g_aWall[nCntWall].pos.z);
			pVertex[1].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y + fHeight, g_aWall[nCntWall].pos.z);
			pVertex[2].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x + (fWidth / 2), g_aWall[nCntWall].pos.y + fHeight, g_aWall[nCntWall].pos.z);
			pVertex[3].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x - (fWidth / 2), g_aWall[nCntWall].pos.y + (fHeight / 2), g_aWall[nCntWall].pos.z);
			pVertex[4].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y + (fHeight / 2), g_aWall[nCntWall].pos.z);
			pVertex[5].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x + (fWidth / 2), g_aWall[nCntWall].pos.y + (fHeight / 2), g_aWall[nCntWall].pos.z);
			pVertex[6].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x - (fWidth / 2), g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
			pVertex[7].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
			pVertex[8].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x + (fWidth / 2), g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);


			for (int nCnt = 0; nCnt < 9; nCnt++)
			{
				//�@���̐ݒ�
				pVertex[nCnt].nor = nor;
			}

			//���_�o�b�t�@�A�����b�N
			g_aWall[nCntWall].pVtx->Unlock();

			//�ʒu�̐ݒ�
			g_aWall[nCntWall].pos = pos;

			//��]�̐ݒ�
			g_aWall[nCntWall].rot = rot;

			//���ݒ�
			g_aWall[nCntWall].fWidth = fWidth;

			//�����ݒ�
			g_aWall[nCntWall].fHeight = fHeight;



			//�g�p���ɕύX
			g_aWall[nCntWall].bUse = true;
			break;
		}
	}
}