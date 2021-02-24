//-----------------------------------------------------------------------------
//
// �e�̏��� [bullet.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "bullet.h"
#include "input.h"
#include "model.h"
#include "shadow.h"
#include "player.h"
#include "wall.h"

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffBullet = NULL;									// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;											// �|���S���̃e�N�X�`���ւ̃|�C���^
Bullet g_aBullet[BULLET_MAX];														// �e�̏��
int g_aShadowIdxBullet[SHADOW_MAX];													// �e�̉e�ԍ�

//-----------------------------------------------------------------------------
// �e�̏���������
//-----------------------------------------------------------------------------
HRESULT InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�e�̏��z���������
	for (int nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		g_aBullet[nCntBullet].bUse = false;
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLifeTime = 60;
	}

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\bullet000.png" , &g_pTextureBullet);

	//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VERTEX_AMOUNT * BULLET_MAX,		//�T�C�Y
								D3DUSAGE_WRITEONLY,									//
								FVF_VERTEX_3D,										//�t�H�[�}�b�g
								D3DPOOL_MANAGED,									//
								&g_pVtxBuffBullet,									//���_�o�b�t�@�ւ̃|�C���^
								NULL);

	VERTEX_3D *pVertex;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++, pVertex+=4)
	{
		//���_���W�ݒ� �|���S���̒��S����c�ɗ��ĂĐݒ�
		pVertex[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLET_WIDTH, g_aBullet[nCntBullet].pos.y - BULLET_HEIGHT, g_aBullet[nCntBullet].pos.z);
		pVertex[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLET_WIDTH, g_aBullet[nCntBullet].pos.y + BULLET_HEIGHT, g_aBullet[nCntBullet].pos.z);
		pVertex[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLET_WIDTH, g_aBullet[nCntBullet].pos.y - BULLET_HEIGHT, g_aBullet[nCntBullet].pos.z);
		pVertex[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLET_WIDTH, g_aBullet[nCntBullet].pos.y + BULLET_HEIGHT, g_aBullet[nCntBullet].pos.z);

		//�@���x�N�g���̐ݒ�
		pVertex[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVertex[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVertex[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVertex[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
// �e�̏I������
//-----------------------------------------------------------------------------
void UninitBullet(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//-----------------------------------------------------------------------------
// �e�̍X�V����
//-----------------------------------------------------------------------------
void UpdateBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	Model *pModel = GetModel();

	//�X�y�[�X�L�[�������ꂽ��
	if (GetKeyboardTrigger(DIK_SPACE) == true)
	{
		//SetBullet(D3DXVECTOR3(pModel->pos.x, 20.0f, pModel->pos.z), D3DXVECTOR3(sinf(pModel->rot.y) * 2, 0.0f, cosf(pModel->rot.y) * 2));
	}

	//�g�p���̒e��������
	for (int nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			//�e�̈ʒu���X�V
			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;

			//�e�̉e�̈ʒu�X�V
			SetPositionShadow(g_aShadowIdxBullet[nCntBullet], D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, 0.0f, g_aBullet[nCntBullet].pos.z));

			//�����蔻��e�X�g
			Player *pPlayer = GetPlayer();
			Wall *pWall = GetWall();
			D3DXVECTOR3 vecA;
			D3DXVECTOR3 vecB;
			D3DXVECTOR3 aPos[2];

			//�ǂ̈ʒu��������̍��E��100�ړ������ꏊ��pos��z�u
			aPos[0] = pWall->pos + D3DXVECTOR3(sinf(pWall->rot.y + -D3DX_PI / 2) * 100, 0.0f, cosf(pWall->rot.y + -D3DX_PI / 2) * 100);
			aPos[1] = pWall->pos + D3DXVECTOR3(sinf(pWall->rot.y + D3DX_PI / 2) * 100, 0.0f, cosf(pWall->rot.y + D3DX_PI / 2) * 100);

			//pos0����1�ւ̃x�N�g��
			vecA = aPos[1] - aPos[0];

			//pos0����e�̈ʒu�ւ̃x�N�g��
			vecB = g_aBullet[nCntBullet].pos - aPos[0];

			//�O�ό��ʂ��}�C�i�X�������ꍇ
			if ((vecA.z * vecB.x) - (vecA.x  * vecB.z) <= 0)
			{
				D3DXVECTOR3 vecA, vecB;
				float fDot;

				//�e�̈ړ��x�N�g��
				vecA = g_aBullet[nCntBullet].move;

				//�ǂ̖@���x�N�g��
				vecB = D3DXVECTOR3(sinf(pWall->rot.y), 0.0f, cosf(pWall->rot.y));

				//���K��
				D3DXVec3Normalize(&vecB, &vecB);

				//���ό��ʂ��傫��
				fDot = ((vecA.x * -1) * vecB.x) + ((vecA.z * -1) * vecB.z);

				//�@���ɓ��ό��ʂ̑傫����2�{��������
				vecB = vecB * (fDot * 2);

				//�e�̈ړ��x�N�g��(vecA)�Ɩ@���̕����Ɍ������傫��fDot�̃x�N�g��(vecB)�𑫂�
				g_aBullet[nCntBullet].move = vecA + vecB;
			}

			//����
			//g_aBullet[nCntBullet].nLifeTime--;
			//if (g_aBullet[nCntBullet].nLifeTime <= 0)
			//{
			//	g_aBullet[nCntBullet].bUse = false;
			//}
		}
	}
}

//-----------------------------------------------------------------------------
// �e�̕`�揈��
//-----------------------------------------------------------------------------
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxView;					// �r���[�}�g���b�N�X�擾
	D3DXMATRIX mtxTrans;				// �v�Z�p�}�g���b�N�X

	//�f�o�C�X�擾
	pDevice = GetDevice();

	for (int nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			//�����F��`�悵�Ȃ�(�A���t�@�e�X�g)
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// �A���t�@�e�X�g�L����
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// �A���t�@�e�X�g��r�A�l�����傫���ꍇ
			pDevice->SetRenderState(D3DRS_ALPHAREF, 127);				// �A���t�@�l�Q��

			//��r����߂ď�Ɏ�O�ɕ`��
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aBullet[nCntBullet].mtxWorld);

			//�r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);
			
			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aBullet[nCntBullet].mtxWorld, NULL, &mtxView);
			g_aBullet[nCntBullet].mtxWorld._41 = 0.0f;
			g_aBullet[nCntBullet].mtxWorld._42 = 0.0f;
			g_aBullet[nCntBullet].mtxWorld._43 = 0.0f;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y, g_aBullet[nCntBullet].pos.z);
			D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCntBullet].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBullet);
			
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntBullet, 2);

			//�����l�ɖ߂�
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		}
	}

}

//-----------------------------------------------------------------------------
// �e�̐ݒu�A�ݒ�
//-----------------------------------------------------------------------------
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	for (int nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{
			//�e�̈ʒu�ݒ�
			g_aBullet[nCntBullet].pos = pos;

			//�e�̈ړ��ݒ�
			g_aBullet[nCntBullet].move = move;

			//�e�̎g�p��Ԃ��g�p����
			g_aBullet[nCntBullet].bUse = true;

			//�e�̉e������
			g_aShadowIdxBullet[nCntBullet] = SetShadow(pos, 50, 50);

			break;
		}
	}
}