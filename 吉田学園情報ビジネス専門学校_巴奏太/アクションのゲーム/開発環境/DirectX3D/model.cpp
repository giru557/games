#define _CRT_SECURE_NO_WARNINGS
//-----------------------------------------------------------------------------
//
// ���f���̏��� [model.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "model.h"
#include "camera.h"
#include "input.h"
#include "shadow.h"
#include "player.h"
#include <stdio.h>
#include "collision.h"

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
#define READROW (5000)	// �ǂݍ��ލs��

//-----------------------------------------------------------------------------
// ���f�����̍\����
//-----------------------------------------------------------------------------
typedef struct
{
	int nNumTexture;		// �e�N�X�`���̐�
	int nNumModel;			// ���f���̐�
	char cTextureFile[MODEL_AMOUNT_MAX][128];	// �e�N�X�`���t�@�C����
	char cModelFile[MODEL_AMOUNT_MAX][128];	// ���f���t�@�C����
}ModelLoad;

//-----------------------------------------------------------------------------
// ���f���z�u���̍\����
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X
	int nType;				// ���f���̃^�C�v(�ԍ�)
	int nCollision;			// �R���W�����̗L��
	float fPos[3];			// �ʒu
	float fRot[3];			// ����
}ModelSet;

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
void ModelCollision(void);
void ReadModelInfo(void);

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
Model g_aModel[MODEL_AMOUNT_MAX];						// ���f���̏��
ModelLoad g_LoadModel;									// ���f���̓ǂݍ��ݏ��
ModelSet g_aModelSetInfo[MODEL_AMOUNT_MAX];				// ���f���̐ݒu���
int g_nCountModelSet = 0;

//-----------------------------------------------------------------------------
// ����������
//-----------------------------------------------------------------------------
void InitModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�擾

	//������
	g_nCountModelSet = 0;
	for (int nCntModel = 0; nCntModel < MODEL_AMOUNT_MAX; nCntModel++)
	{
		g_aModel[nCntModel].bUse = false;
		g_aModel[nCntModel].dwNumMat = 0;
		g_aModel[nCntModel].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aModel[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aModel[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aModel[nCntModel].vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aModel[nCntModel].vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		for (int nCntVector3 = 0; nCntVector3 < 3; nCntVector3++)
		{
			g_aModelSetInfo[nCntModel].fPos[nCntVector3] = 0.0f;
			g_aModelSetInfo[nCntModel].fPos[nCntVector3] = 0.0f;
		}
		g_aModelSetInfo[nCntModel].nCollision = 0;
		g_aModelSetInfo[nCntModel].nType = 0;

		g_LoadModel.cModelFile[nCntModel][0] = '\0';
		g_LoadModel.cTextureFile[nCntModel][0] = '\0';
	}
	g_LoadModel.nNumModel = 0;
	g_LoadModel.nNumTexture = 0;

	//���f���̏��ǂݎ��
	ReadModelInfo();

	//���f���̃��[�h
	for (int nCntModel = 0; nCntModel < g_LoadModel.nNumModel; nCntModel++)
	{
		//X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(
			&g_LoadModel.cModelFile[nCntModel][0],	// �t�@�C����
			D3DXMESH_SYSTEMMEM,						// �Œ�
			pDevice,								// �f�o�C�X
			NULL,									// NULL�Œ�
			&g_aModel[nCntModel].pBuffMat,			// �}�e���A��
			NULL,									// NULL�Œ�
			&g_aModel[nCntModel].dwNumMat,			// �}�e���A����
			&g_aModel[nCntModel].pMesh);			// ���b�V��

		int nNumVtx;	// ���_��
		DWORD sizeFVF;	// ���_�t�H�[�}�b�g�̃T�C�Y
		BYTE *pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^

		//���_�����擾
		nNumVtx = g_aModel[nCntModel].pMesh->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(g_aModel[nCntModel].pMesh->GetFVF());

		//���_�o�b�t�@�����b�N
		g_aModel[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	//���_���W���

			//�ő�l�ŏ��l�𔲂��o��
			if (vtx.x < g_aModel[nCntModel].vtxMin.x) g_aModel[nCntModel].vtxMin.x = vtx.x;	// X�ŏ��l
			if (vtx.y > g_aModel[nCntModel].vtxMax.y) g_aModel[nCntModel].vtxMax.y = vtx.y;	// Y�ő�l
			if (vtx.y < g_aModel[nCntModel].vtxMin.y) g_aModel[nCntModel].vtxMin.y = vtx.y;	// Y�ŏ��l
			if (vtx.z > g_aModel[nCntModel].vtxMax.z) g_aModel[nCntModel].vtxMax.z = vtx.z;	// Z�ő�l
			if (vtx.z < g_aModel[nCntModel].vtxMin.z) g_aModel[nCntModel].vtxMin.z = vtx.z;	// Z�ŏ��l
			if (vtx.x > g_aModel[nCntModel].vtxMax.x) g_aModel[nCntModel].vtxMax.x = vtx.x;	// X�ő�l

			pVtxBuff += sizeFVF;		// ���_�t�H�[�}�b�g�̃T�C�Y�������|�C���^��i�߂�
		}

		//���_�o�b�t�@���A�����b�N
		g_aModel[nCntModel].pMesh->UnlockVertexBuffer();

		//�}�e���A���ւ̃|�C���^
		D3DXMATERIAL *pMat;

		//�}�e���A�����ւ̃|�C���^
		pMat = (D3DXMATERIAL*)g_aModel[nCntModel].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aModel[nCntModel].dwNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				HRESULT hr = D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aModel[nCntModel].pTexture[nCntMat]);
			}
		}
	}
}

//-----------------------------------------------------------------------------
// �I������
//-----------------------------------------------------------------------------
void UninitModel(void)
{
	for (int nCntModel = 0; nCntModel < MODEL_AMOUNT_MAX; nCntModel++)
	{
		//���b�V���̔j��
		if (g_aModel[nCntModel].pMesh != NULL)
		{
			g_aModel[nCntModel].pMesh->Release();
			g_aModel[nCntModel].pMesh = NULL;
		}

		//�}�e���A���̔j��
		if (g_aModel[nCntModel].pBuffMat != NULL)
		{
			g_aModel[nCntModel].pBuffMat->Release();
			g_aModel[nCntModel].pBuffMat = NULL;
		}

		for (int nCntMat = 0; nCntMat < (int)g_aModel[nCntMat].dwNumMat; nCntMat++)\
		{
			//�e�N�X�`���j��
			if (g_aModel[nCntModel].pTexture[nCntMat] != NULL)
			{
				g_aModel[nCntModel].pTexture[nCntMat]->Release();
				g_aModel[nCntModel].pTexture[nCntMat] = NULL;
			}
		}

	}
}

//-----------------------------------------------------------------------------
// �X�V����
//-----------------------------------------------------------------------------
void UpdateModel(void)
{
	//�����蔻��i�ړ��j
	ModelCollision();
}

//-----------------------------------------------------------------------------
// �`�揈��
//-----------------------------------------------------------------------------
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntModel = 0; nCntModel < g_nCountModelSet; nCntModel++)
	{
		D3DXMATRIX mtxRot, mtxTrans;												// �v�Z�p�}�g���b�N�X
		D3DMATERIAL9 matDef;														// ���݂̃}�e���A���ۑ��p
		D3DXMATERIAL *pMat;															// �}�e���A���f�[�^�ւ̃|�C���^
		D3DXVECTOR3 pos = D3DXVECTOR3(g_aModelSetInfo[nCntModel].fPos[0], g_aModelSetInfo[nCntModel].fPos[1], g_aModelSetInfo[nCntModel].fPos[2]);	// �z�u���f���̈ʒu
		D3DXVECTOR3 rot = D3DXVECTOR3(D3DXToRadian(g_aModelSetInfo[nCntModel].fRot[0]), D3DXToRadian(g_aModelSetInfo[nCntModel].fRot[1]), D3DXToRadian(g_aModelSetInfo[nCntModel].fRot[2]));	// �z�u���f���̌���

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aModelSetInfo[nCntModel].mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
		D3DXMatrixMultiply(&g_aModelSetInfo[nCntModel].mtxWorld, &g_aModelSetInfo[nCntModel].mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
		D3DXMatrixMultiply(&g_aModelSetInfo[nCntModel].mtxWorld, &g_aModelSetInfo[nCntModel].mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aModelSetInfo[nCntModel].mtxWorld);

		//���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);
		
		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_aModel[g_aModelSetInfo[nCntModel].nType].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aModel[g_aModelSetInfo[nCntModel].nType].dwNumMat; nCntMat++)
		{
			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_aModel[g_aModelSetInfo[nCntModel].nType].pTexture[nCntMat]);

			//���f���p�[�c�̕`��
			g_aModel[g_aModelSetInfo[nCntModel].nType].pMesh->DrawSubset(nCntMat);
		}

		//�ۑ����Ă����}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}
}

//-----------------------------------------------------------------------------
// ���f���̏��擾
//-----------------------------------------------------------------------------
Model *GetModel(void)
{
	return &g_aModel[0];
}

//-----------------------------------------------------------------------------
// ���f���̔z�u
//-----------------------------------------------------------------------------
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCntModel = 0; nCntModel < MODEL_AMOUNT_MAX; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == false)
		{
			//�ʒu�̐ݒ�
			g_aModel[nCntModel].pos = pos;

			//�����̐ݒ�
			g_aModel[nCntModel].rot = rot;

			//�g�p���Ɉڍs
			g_aModel[nCntModel].bUse = true;
			break;
		}
	}
}

void ModelCollision(void)
{
	Player *pPlayer = GetPlayer();
	float fPwidth = PlAYER_WIDTH;

	for (int nCntModel = 0; nCntModel < g_nCountModelSet; nCntModel++)
	{
		//���f���̔z�u�ꏊ
		D3DXVECTOR3 modelPos = D3DXVECTOR3(g_aModelSetInfo[nCntModel].fPos[0], g_aModelSetInfo[nCntModel].fPos[1], g_aModelSetInfo[nCntModel].fPos[2]);

		// �����` �� ��` �����蔻�� 2d �����߂荞�ݖ߂�
		//�E����
		if (pPlayer->pos.x - fPwidth < modelPos.x + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMax.x && pPlayer->pos.x - fPwidth > modelPos.x + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMin.x &&
			pPlayer->pos.z + fPwidth > modelPos.z + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMin.z && pPlayer->pos.z - fPwidth < modelPos.z + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMax.z)
		{
			if (pPlayer->posOld.x + fPwidth > modelPos.x + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMax.x &&
				pPlayer->posOld.z + fPwidth > modelPos.z + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMin.z && pPlayer->posOld.z - fPwidth < modelPos.z + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMax.z)
			{
				pPlayer->pos.x = modelPos.x + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMax.x + fPwidth;
			}
		}
		//������
		if (pPlayer->pos.x + fPwidth > modelPos.x + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMin.x && pPlayer->pos.x + fPwidth < modelPos.x + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMax.x &&
			pPlayer->pos.z + fPwidth > modelPos.z + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMin.z && pPlayer->pos.z - fPwidth < modelPos.z + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMax.z)
		{
			if (pPlayer->posOld.x - fPwidth < modelPos.x + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMin.x &&
				pPlayer->posOld.z + fPwidth > modelPos.z + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMin.z && pPlayer->posOld.z - fPwidth < modelPos.z + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMax.z)
			{
				pPlayer->pos.x = modelPos.x + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMin.x - fPwidth;
			}

		}
		//������
		if (pPlayer->pos.z - fPwidth < modelPos.z + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMax.z && pPlayer->pos.z - fPwidth > modelPos.z + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMin.z &&
			pPlayer->pos.x + fPwidth > modelPos.x + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMin.x && pPlayer->pos.x - fPwidth < modelPos.x + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMax.x)
		{
			if (pPlayer->posOld.z + fPwidth > modelPos.z + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMax.z &&
				pPlayer->posOld.x + fPwidth > modelPos.x + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMin.x && pPlayer->posOld.x - fPwidth < modelPos.x + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMax.x)
			{
				pPlayer->pos.z = modelPos.z + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMax.z + fPwidth;
			}
		}
		//��O����
		if (pPlayer->pos.z + fPwidth > modelPos.z + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMin.z && pPlayer->pos.z + fPwidth < modelPos.z + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMax.z &&
			pPlayer->pos.x + fPwidth > modelPos.x + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMin.x && pPlayer->pos.x - fPwidth < modelPos.x + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMax.x)
		{
			if (pPlayer->posOld.z - fPwidth < modelPos.z + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMin.z &&
				pPlayer->posOld.x + fPwidth > modelPos.x + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMin.x && pPlayer->posOld.x - fPwidth < modelPos.x + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMax.x)
			{
				pPlayer->pos.z = modelPos.z + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMin.z - fPwidth;
			}
		}
	}
}

void ReadModelInfo(void)
{
	FILE *pFile;				// �t�@�C���ւ̃|�C���^
	char cRowChar[READROW][64];	// �ǂݍ��ޕ�����
	char cEqual[2];				// �C�R�[����r

	int nCntTextureName = 0;	// �J�E���g�p�i��ň�ɂ܂Ƃ߂�j
	int nCntModelFileName = 0;	// 
	int nCntCol = 0;			// 
	int nCntType = 0;			// 
	int nCntPos = 0;			// 
	int nCntRot = 0;			// 

	//�t�@�C�����[�Ղ�
	pFile = fopen("model.txt", "r");
	if (pFile != NULL)
	{
		//READROW�s�T��
		for (int nCntRow = 0; nCntRow < READROW; nCntRow++)
		{
			//������ǂݍ���
			fscanf(pFile, "%s", &cRowChar[nCntRow][0]);

			//SCRIPT�̕��������������
			if (strcmp(&cRowChar[nCntRow][0], "SCRIPT") == 0)
			{
				for (int nCntRow = 0; nCntRow < READROW; nCntRow++)
				{
					//���̕�����
					fscanf(pFile, "%s", &cRowChar[nCntRow][0]);

					//NUM_TEXTURE
					if (strcmp(&cRowChar[nCntRow][0], "NUM_TEXTURE") == 0)
					{
						//���̕�����
						fscanf(pFile, "%s", &cEqual[0]);

						//�C�R�[��������
						if (strcmp(&cEqual[0], "=") == 0)
						{
							//�C�R�[���̐�̐���ϐ��ɓ����
							fscanf(pFile, "%d", &g_LoadModel.nNumTexture);
						}
					}

					//TEXTURE_FILENAME
					if (strcmp(&cRowChar[nCntRow][0], "TEXTURE_FILENAME") == 0)
					{
						//���̕���
						fscanf(pFile, "%s", &cEqual[0]);

						//�C�R�[����������
						if (strcmp(&cEqual[0], "=") == 0)
						{
							//�C�R�[���̎��̐���ϐ��ɓ����
							fscanf(pFile, "%s", &g_LoadModel.cTextureFile[nCntTextureName][0]);

							//�J�E���g�A�b�v
							nCntTextureName++;
						}
					}

					//NUM_MODEL
					if (strcmp(&cRowChar[nCntRow][0], "NUM_MODEL") == 0)
					{
						//���̕�����
						fscanf(pFile, "%s", &cEqual[0]);

						//�C�R�[��������
						if (strcmp(&cEqual[0], "=") == 0)
						{
							//�C�R�[���̐�̐���ϐ��ɓ����
							fscanf(pFile, "%d", &g_LoadModel.nNumModel);
						}
					}

					//MODEL_FILENAME
					if (strcmp(&cRowChar[nCntRow][0], "MODEL_FILENAME") == 0)
					{
						//���̕���
						fscanf(pFile, "%s", &cEqual[0]);

						//�C�R�[����������
						if (strcmp(&cEqual[0], "=") == 0)
						{
							//�C�R�[���̎��̐���ϐ��ɓ����
							fscanf(pFile, "%s", &g_LoadModel.cModelFile[nCntModelFileName][0]);

							//�J�E���g�A�b�v
							nCntModelFileName++;
						}
					}

					//MODELSET
					if (strcmp(&cRowChar[nCntRow][0], "MODELSET") == 0)
					{
						g_nCountModelSet++;
						for (int nCntMSET = 0; nCntMSET < READROW; nCntMSET++)
						{
							//���̕�����
							fscanf(pFile, "%s", &cRowChar[nCntMSET][0]);

							//TYPE
							if (strcmp(&cRowChar[nCntMSET][0], "TYPE") == 0)
							{
								//���̕�����
								fscanf(pFile, "%s", &cEqual[0]);

								//�C�R�[��������
								if (strcmp(&cEqual[0], "=") == 0)
								{
									//�C�R�[���̐�̐���ϐ��ɓ����
									fscanf(pFile, "%d", &g_aModelSetInfo[nCntType].nType);

									//�J�E���g�A�b�v
									nCntType++;
								}
							}

							//POS
							if (strcmp(&cRowChar[nCntMSET][0], "POS") == 0)
							{
								//���̕�����
								fscanf(pFile, "%s", &cEqual[0]);

								//�C�R�[����������
								if (strcmp(&cEqual[0], "=") == 0)
								{
									for (int nCntVector3 = 0; nCntVector3 < 3; nCntVector3++)
									{
										//���̒l��ϐ��ɓ����
										fscanf(pFile, "%f", &g_aModelSetInfo[nCntPos].fPos[nCntVector3]);
									}

									//�J�E���g�A�b�v
									nCntPos++;
								}
							}

							//ROT
							if (strcmp(&cRowChar[nCntMSET][0], "ROT") == 0)
							{
								//���̕�����
								fscanf(pFile, "%s", &cEqual[0]);

								//�C�R�[����������
								if (strcmp(&cEqual[0], "=") == 0)
								{
									for (int nCntVector3 = 0; nCntVector3 < 3; nCntVector3++)
									{
										fscanf(pFile, "%f", &g_aModelSetInfo[nCntRot].fRot[nCntVector3]);
									}
									nCntRot++;
								}
							}

							//COLLISION
							if (strcmp(&cRowChar[nCntMSET][0], "COLLISION") == 0)
							{
								//���̕�����
								fscanf(pFile, "%s", &cEqual[0]);

								//�C�R�[��������
								if (strcmp(&cEqual[0], "=") == 0)
								{
									//�C�R�[���̐�̐���ϐ��ɓ����
									fscanf(pFile, "%d", &g_aModelSetInfo[nCntCol].nCollision);

									//�J�E���g�A�b�v
									nCntCol++;
								}
							}

							//END_MODELSET�������烋�[�v������
							if (strcmp(&cRowChar[nCntMSET][0], "END_MODELSET") == 0)
							{
								break;
							}
						}
					}

					//END_SCRIPT�������烋�[�v������
					if (strcmp(&cRowChar[nCntRow][0], "END_SCRIPT") == 0)
					{
						break;
					}
				}
				break;
			}
		}
	}
}