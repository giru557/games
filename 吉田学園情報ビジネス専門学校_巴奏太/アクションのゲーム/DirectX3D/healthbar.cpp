//-----------------------------------------------------------------------------
//
// HP�o�[�̏��� [healthbar.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "healthbar.h"

#include "input.h"
#include "enemy.h"

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
void ShowHealthBarAtEnemy(void);

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
HealthBar g_aHealthBar[ENEMY_AMOUNT_MAX][HPBAR_LAYER];	// HP�o�[�̏��
float g_fHPRatio = 1.0f;								// HP�̊���

//-----------------------------------------------------------------------------
// HP�o�[�̏���������
//-----------------------------------------------------------------------------
HRESULT InitHP(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�擾

	g_fHPRatio = 1.0f;	// HP�̊���

	//�G�̐������p�ӂ���
	for (int nCntEnemy = 0; nCntEnemy < ENEMY_AMOUNT_MAX; nCntEnemy++)
	{
		/* ------HP�o�[�̌��------ */
		VERTEX_3D *pVertexBack;

		//�ʒu
		g_aHealthBar[nCntEnemy][0].pos = D3DXVECTOR3(0.0f, 50, 0.0f);

		//�g�p��
		g_aHealthBar[nCntEnemy][0].bUse = false;

		//�e�N�X�`���ǂݍ���
		D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\healthbar_back.png", &g_aHealthBar[nCntEnemy][0].pTexture);

		//���_�o�b�t�@����
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VERTEX_AMOUNT,	//�T�C�Y
			D3DUSAGE_WRITEONLY,											//
			FVF_VERTEX_3D,												//�t�H�[�}�b�g
			D3DPOOL_MANAGED,											//
			&g_aHealthBar[nCntEnemy][0].pVtxBuff,									//���_�o�b�t�@�ւ̃|�C���^
			NULL);

		//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
		g_aHealthBar[nCntEnemy][0].pVtxBuff->Lock(0, 0, (void**)&pVertexBack, 0);

		//���_���W�ݒ�
		pVertexBack[0].pos = D3DXVECTOR3(-HPBAR_WIDTH / 2, -HPBAR_HEIGHT / 2, 0.0f);
		pVertexBack[1].pos = D3DXVECTOR3(-HPBAR_WIDTH / 2, HPBAR_HEIGHT / 2, 0.0f);
		pVertexBack[2].pos = D3DXVECTOR3(HPBAR_WIDTH / 2, -HPBAR_HEIGHT / 2, 0.0f);
		pVertexBack[3].pos = D3DXVECTOR3(HPBAR_WIDTH / 2, HPBAR_HEIGHT / 2, 0.0f);

		//�@���x�N�g���̐ݒ�
		pVertexBack[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVertexBack[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVertexBack[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVertexBack[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//���_�J���[�̐ݒ�
		pVertexBack[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVertexBack[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVertexBack[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVertexBack[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVertexBack[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVertexBack[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVertexBack[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVertexBack[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		//���_�o�b�t�@���A�����b�N����
		g_aHealthBar[nCntEnemy][0].pVtxBuff->Unlock();


		/* ------HP�o�[�̑O------ */
		VERTEX_3D *pVertexFront;

		//�ʒu
		g_aHealthBar[nCntEnemy][1].pos = g_aHealthBar[nCntEnemy][0].pos;

		//�g�p��
		g_aHealthBar[nCntEnemy][1].bUse = false;

		//�e�N�X�`���ǂݍ���
		D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\healthbar_front.png", &g_aHealthBar[nCntEnemy][1].pTexture);

		//���_�o�b�t�@����
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VERTEX_AMOUNT,	//�T�C�Y
			D3DUSAGE_WRITEONLY,											//
			FVF_VERTEX_3D,												//�t�H�[�}�b�g
			D3DPOOL_MANAGED,											//
			&g_aHealthBar[nCntEnemy][1].pVtxBuff,									//���_�o�b�t�@�ւ̃|�C���^
			NULL);

		//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
		g_aHealthBar[nCntEnemy][1].pVtxBuff->Lock(0, 0, (void**)&pVertexFront, 0);

		//���_���W�ݒ�
		pVertexFront[0].pos = D3DXVECTOR3(-HPBAR_WIDTH / 2, -HPBAR_HEIGHT / 2, 0.0f);
		pVertexFront[1].pos = D3DXVECTOR3(-HPBAR_WIDTH / 2, HPBAR_HEIGHT / 2, 0.0f);
		pVertexFront[2].pos = D3DXVECTOR3(HPBAR_WIDTH / 2, -HPBAR_HEIGHT / 2, 0.0f);
		pVertexFront[3].pos = D3DXVECTOR3(HPBAR_WIDTH / 2, HPBAR_HEIGHT / 2, 0.0f);

		//�@���x�N�g���̐ݒ�
		pVertexFront[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVertexFront[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVertexFront[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVertexFront[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//���_�J���[�̐ݒ�
		pVertexFront[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVertexFront[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVertexFront[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVertexFront[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVertexFront[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVertexFront[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVertexFront[2].tex = D3DXVECTOR2(0.5f, 1.0f);
		pVertexFront[3].tex = D3DXVECTOR2(0.5f, 0.0f);

		//���_�o�b�t�@���A�����b�N����
		g_aHealthBar[nCntEnemy][1].pVtxBuff->Unlock();
	}

	for (int nCntLayer = 0; nCntLayer < HPBAR_LAYER; nCntLayer++)
	{
		g_aHealthBar[0][nCntLayer].pos = D3DXVECTOR3(0.0f, 50.0f, 0.0f);
		g_aHealthBar[0][nCntLayer].bUse = true;

		g_aHealthBar[1][nCntLayer].pos = D3DXVECTOR3(50.0f, 50.0f, 0.0f);
		g_aHealthBar[1][nCntLayer].bUse = true;
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// HP�o�[�̏I������
//-----------------------------------------------------------------------------
void UninitHP(void)
{
	//�G�̐������j������
	for (int nCntEnemy = 0; nCntEnemy < ENEMY_AMOUNT_MAX; nCntEnemy++)
	{
		for (int nCntLayer = 0; nCntLayer < HPBAR_LAYER; nCntLayer++)
		{
			//�e�N�X�`���̔j��
			if (g_aHealthBar[nCntEnemy][nCntLayer].pTexture != NULL)
			{
				g_aHealthBar[nCntEnemy][nCntLayer].pTexture->Release();
				g_aHealthBar[nCntEnemy][nCntLayer].pTexture = NULL;
			}

			//���_�o�b�t�@�̔j��
			if (g_aHealthBar[nCntEnemy][nCntLayer].pVtxBuff != NULL)
			{
				g_aHealthBar[nCntEnemy][nCntLayer].pVtxBuff->Release();
				g_aHealthBar[nCntEnemy][nCntLayer].pVtxBuff = NULL;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// HP�o�[�X�V����
//-----------------------------------------------------------------------------
void UpdateHP(void)
{
	VERTEX_3D *pVertexFront;	// 3D�t�H�[�}�b�g
	Enemy *pEnemy = GetEnemy();	// �G�̎擾

	//�G��HP�o�[��������
	ShowHealthBarAtEnemy();

	//�G�̕���
	for (int nCntEnemy = 0; nCntEnemy < ENEMY_AMOUNT_MAX; nCntEnemy++)
	{
		//�g�p���̏ꍇ
		if (pEnemy[nCntEnemy].bUse == true)
		{
			//�̗͂̊���
			g_fHPRatio = (float)pEnemy[nCntEnemy].nLife / (float)pEnemy[nCntEnemy].nLifeMax;

			//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
			g_aHealthBar[nCntEnemy][1].pVtxBuff->Lock(0, 0, (void**)&pVertexFront, 0);

			//���_���W�ݒ�
			pVertexFront[0].pos = D3DXVECTOR3((-HPBAR_WIDTH / 2) * g_fHPRatio, -HPBAR_HEIGHT / 2, 0.0f);
			pVertexFront[1].pos = D3DXVECTOR3((-HPBAR_WIDTH / 2) * g_fHPRatio, HPBAR_HEIGHT / 2, 0.0f);
			pVertexFront[2].pos = D3DXVECTOR3((HPBAR_WIDTH / 2) * g_fHPRatio, -HPBAR_HEIGHT / 2, 0.0f);
			pVertexFront[3].pos = D3DXVECTOR3((HPBAR_WIDTH / 2) * g_fHPRatio, HPBAR_HEIGHT / 2, 0.0f);

			//���_�o�b�t�@���A�����b�N����
			g_aHealthBar[nCntEnemy][1].pVtxBuff->Unlock();
		}
	}
}

//-----------------------------------------------------------------------------
// HP�o�[�̕`�揈��
//-----------------------------------------------------------------------------
void DrawHP(void)
{
	//�G�̐���
	for (int nCntEnemy = 0; nCntEnemy < ENEMY_AMOUNT_MAX; nCntEnemy++)
	{
		//���C���[�̐���
		for (int nCntLayer = 0; nCntLayer < HPBAR_LAYER; nCntLayer++)
		{
			//�g���Ă�����`��
			if (g_aHealthBar[nCntEnemy][nCntLayer].bUse == true)
			{
				LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�ւ̃|�C���^
				D3DXMATRIX mtxView;									// �r���[�}�g���b�N�X�擾
				D3DXMATRIX mtxTrans;								// �v�Z�p�}�g���b�N�X

				//��r����߂ď�Ɏ�O�ɕ`��
				pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

				//���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_aHealthBar[nCntEnemy][nCntLayer].mtxWorld);

				//�r���[�}�g���b�N�X���擾
				pDevice->GetTransform(D3DTS_VIEW, &mtxView);

				//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
				D3DXMatrixInverse(&g_aHealthBar[nCntEnemy][nCntLayer].mtxWorld, NULL, &mtxView);
				g_aHealthBar[nCntEnemy][nCntLayer].mtxWorld._41 = 0.0f;
				g_aHealthBar[nCntEnemy][nCntLayer].mtxWorld._42 = 0.0f;
				g_aHealthBar[nCntEnemy][nCntLayer].mtxWorld._43 = 0.0f;

				//�ʒu�𔽉f
				D3DXMatrixTranslation(&mtxTrans, g_aHealthBar[nCntEnemy][nCntLayer].pos.x, g_aHealthBar[nCntEnemy][nCntLayer].pos.y, g_aHealthBar[nCntEnemy][nCntLayer].pos.z);
				D3DXMatrixMultiply(&g_aHealthBar[nCntEnemy][nCntLayer].mtxWorld, &g_aHealthBar[nCntEnemy][nCntLayer].mtxWorld, &mtxTrans);

				//���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_aHealthBar[nCntEnemy][nCntLayer].mtxWorld);

				//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
				pDevice->SetStreamSource(0, g_aHealthBar[nCntEnemy][nCntLayer].pVtxBuff, 0, sizeof(VERTEX_3D));

				//���_�t�H�[�}�b�g�̐ݒ�
				pDevice->SetFVF(FVF_VERTEX_3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aHealthBar[nCntEnemy][nCntLayer].pTexture);

				//�|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

				//�����l�ɖ߂�
				pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			}
		}
	}
}

void ShowHealthBarAtEnemy(void)
{
	Enemy *pEnemy = GetEnemy();	// �G�̎擾

	//�G�̐���
	for (int nCntEnemy = 0; nCntEnemy < ENEMY_AMOUNT_MAX; nCntEnemy++)
	{
		//�G���g�p���̏ꍇ
		if (pEnemy[nCntEnemy].bUse == true)
		{
			//���C���[����
			for (int nCntLayer = 0; nCntLayer < HPBAR_LAYER; nCntLayer++)
			{
				//�o�[�̈ʒu��G�̈ʒu��
				g_aHealthBar[nCntEnemy][nCntLayer].pos = pEnemy[nCntEnemy].pos;
				//�����グ��
				g_aHealthBar[nCntEnemy][nCntLayer].pos.y += HPBAR_HEIGHT_FROM_OBJ;

				//�g�p���Ɉڍs
				g_aHealthBar[nCntEnemy][nCntLayer].bUse = true;
			}
		}
		//�g�p���łȂ��ꍇ
		else if (pEnemy[nCntEnemy].bUse == false)
		{
			//���C���[����
			for (int nCntLayer = 0; nCntLayer < HPBAR_LAYER; nCntLayer++)
			{
				//����
				g_aHealthBar[nCntEnemy][nCntLayer].bUse = false;
			}
		}
	}
}