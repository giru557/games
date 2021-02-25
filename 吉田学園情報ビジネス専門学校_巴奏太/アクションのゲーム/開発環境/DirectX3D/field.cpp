#define _CRT_SECURE_NO_WARNINGS
//-----------------------------------------------------------------------------
//
// �t�B�[���h�̏��� [.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "field.h"
#include "camera.h"
#include "input.h"
#include "shadow.h"
#include "player.h"
#include "enemy.h"
#include <stdio.h>

// �}�N����`
#define RAY_POS_Y (500)	// ���C�̎n�_

//
// �v���g�^�C�v�錾
//

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPD3DXMESH g_pMeshF = NULL;													// ���b�V��(���_���)�ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatF;													// �}�e���A��(�ގ�)�ւ̃|�C���^
DWORD g_nNumMatF = 0;														// �}�e���A���̐�
Field g_Field;																	// ���f���̏��
D3DXVECTOR3 g_posField;															// �ʒu
D3DXVECTOR3 g_rotField;															// ����
D3DXMATRIX g_mtxWorldField;														// ���[���h�}�g���b�N�X
D3DXVECTOR3 g_vtxMinField, g_vtxMaxField;										// ���f���̍ő�l�A�ŏ��l
LPDIRECT3DTEXTURE9 g_apTextureField[10] = { NULL };								// �e�N�X�`���ւ̃|�C���^

//-----------------------------------------------------------------------------
// ����������
//-----------------------------------------------------------------------------
void InitField(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();														// �f�o�C�X�̎擾

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(
		"data\\MODEL\\field1.1.x",			// �ǂݍ��ރt�@�C����
		D3DXMESH_SYSTEMMEM,						// �Œ�
		pDevice,								// �f�o�C�X
		NULL,									// NULL�Œ�
		&g_pBuffMatF,						// �}�e���A��
		NULL,									// NULL�Œ�
		&g_nNumMatF,						// �}�e���A����
		&g_pMeshF);							// ���b�V��

	//�����ʒu�A����
	g_Field.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �����ʒu
	g_Field.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ��������

	D3DXMATERIAL *pMat;	// �}�e���A���ւ̃|�C���^

	//�}�e���A�����ւ̃|�C���^
	pMat = (D3DXMATERIAL*)g_pBuffMatF->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_nNumMatF; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_apTextureField[nCntMat]);
		}
	}

	srand((unsigned int)time(NULL));	// rand������

	//�G�̔z�u
	for (int nCntEnemy = 0; nCntEnemy < 10; nCntEnemy++)
	{
		D3DXVECTOR3 rayPos;
		BOOL bHitGround;
		int nPosX, nPosZ;
		float fDistanceToGround;
		nPosX = rand() % 1001 - 500;
		nPosZ = rand() % 1001 - 500;
		
		//Vector3�^�ɓ����
		rayPos = D3DXVECTOR3((float)nPosX, (float)RAY_POS_Y, (float)nPosZ);
		
		//�����_���ȋ�̈ʒu���烌�C��΂�
		D3DXIntersect(g_pMeshF, &rayPos, &D3DXVECTOR3(0.0f, -1.0f, 0.0f), &bHitGround, NULL, NULL, NULL, &fDistanceToGround, NULL, NULL);

		//���C�����������n�ʂ̏ꏊ�ɓG������
		SetEnemy(D3DXVECTOR3(rayPos.x, RAY_POS_Y - fDistanceToGround, rayPos.z));
	}
}

//-----------------------------------------------------------------------------
// �I������
//-----------------------------------------------------------------------------
void UninitField(void)
{
	//���b�V���̔j��
	if (g_pMeshF != NULL)
	{
		g_pMeshF->Release();
		g_pMeshF = NULL;
	}

	//�}�e���A���̔j��
	if (g_pBuffMatF != NULL)
	{
		g_pBuffMatF->Release();
		g_pBuffMatF = NULL;
	}
}

//-----------------------------------------------------------------------------
// �X�V����
//-----------------------------------------------------------------------------
void UpdateField(void)
{
	Player *pPlayer = GetPlayer();
	Enemy *pEnemy = GetEnemy();
	float fU, fV = { 0.0f };
	float fHitDist[2];
	BOOL bHit[2] = { FALSE, FALSE };
	DWORD dwHitFace = -1;

	//���C�ƃ��b�V���̐ڐG����
	D3DXIntersect(g_pMeshF, &pPlayer->pos, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &bHit[0], &dwHitFace, &fU, &fV, &fHitDist[0], NULL, NULL);

	//�������Ă���
	if (bHit[0] == 1)
	{
		//�������v���C���[�̍����ȓ��������ꍇ
		if (fHitDist[0] <= PLAYER_HEIGHT)
		{
			//�n�ʂɗ�������
			pPlayer->pos.y += fHitDist[0];
			pPlayer->move.y = 0.0f;
		}
	}

	for (int nCntEnemy = 0; nCntEnemy < ENEMY_AMOUNT_MAX; nCntEnemy++)
	{
		//���C�ƃ��b�V���̐ڐG����
		D3DXIntersect(g_pMeshF, &pEnemy[nCntEnemy].pos, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &bHit[1], &dwHitFace, &fU, &fV, &fHitDist[1], NULL, NULL);

		if (bHit[1] == 1)
		{
			if (fHitDist[1] <= ENEMY_HEIGHT)
			{
				pEnemy[nCntEnemy].pos.y = pEnemy[nCntEnemy].pos.y + fHitDist[1];
				pEnemy[nCntEnemy].move.y = 0.0f;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// �`�揈��
//-----------------------------------------------------------------------------
void DrawField(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxRot, mtxTrans;												// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;														// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;															// �}�e���A���f�[�^�ւ̃|�C���^
	pDevice = GetDevice();														// �f�o�C�X�̎擾

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Field.mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Field.rot.y, g_Field.rot.x, g_Field.rot.z);
	D3DXMatrixMultiply(&g_Field.mtxWorld, &g_Field.mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_Field.pos.x, g_Field.pos.y, g_Field.pos.z);
	D3DXMatrixMultiply(&g_Field.mtxWorld, &g_Field.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_Field.mtxWorld);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatF->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_nNumMatF; nCntMat++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureField[nCntMat]);

		//���f���p�[�c�̕`��
		g_pMeshF->DrawSubset(nCntMat);
	}

	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}