#define _CRT_SECURE_NO_WARNINGS
//-----------------------------------------------------------------------------
//
// �G�̏��� [enemy.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "enemy.h"
#include "camera.h"
#include "input.h"
#include "shadow.h"
#include "bullet.h"
#include "collision.h"
#include "fade.h"
#include "UI.h"
#include "result.h"
#include "sound.h"
#include <stdio.h>

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
#define PLAYER_MODEL_AMOUNT (10)
#define PLAYER_SMOOTHTURN_SPEED (0.1f)											// ���炩�ɐU��������x

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
void MoveEnemy(float fMoveAngleDegree, float fMoveSpeed);
void LoadXFileEnemy(const char* cXFileName, int nCountModel);
void EnemySmoothTurn(void);

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
Enemy g_aEnemy[ENEMY_AMOUNT_MAX];	// �G�̏��
int g_nEnemyAlive = 0;				// �G�̐�����
bool g_bEliminated;					// �S�ł�����
DWORD g_dwTimeTransition;			// ��ʑJ�ڎ��Ԍv�Z

//-----------------------------------------------------------------------------
// ����������
//-----------------------------------------------------------------------------
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�̎擾

	//X�t�@�C���̓ǂݍ���
	LoadXFileEnemy("data\\MODEL\\snake.x", 0);

	for (int nCntEnemy = 0; nCntEnemy < ENEMY_AMOUNT_MAX; nCntEnemy++)
	{
		//�����ݒ�
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu�̏����ݒ�
		g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����̏����ݒ�
		g_aEnemy[nCntEnemy].bUse = false;
		g_aEnemy[nCntEnemy].bInvincible = false;
		g_aEnemy[nCntEnemy].nLife = ENEMY_HP_MAX;
		g_aEnemy[nCntEnemy].nLifeMax = ENEMY_HP_MAX;
		g_aEnemy[nCntEnemy].bHit = false;
		g_aEnemy[nCntEnemy].bAttack = false;
		g_aEnemy[nCntEnemy].dwTimeAtk = NULL;
		g_aEnemy[nCntEnemy].dwTimeInv = NULL;
		g_aEnemy[nCntEnemy].nIdx = NULL;
	}
	g_nEnemyAlive = 0;
	g_dwTimeTransition = NULL;
	g_bEliminated = false;
}

//-----------------------------------------------------------------------------
// �I������
//-----------------------------------------------------------------------------
void UninitEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < ENEMY_AMOUNT_MAX; nCntEnemy++)
	{
		//���b�V���j��
		if (g_aEnemy[nCntEnemy].pMesh != NULL)
		{
			g_aEnemy[nCntEnemy].pMesh->Release();
			g_aEnemy[nCntEnemy].pMesh = NULL;
		}

		//�}�e���A���j��
		if (g_aEnemy[nCntEnemy].pBuffMat != NULL)
		{
			g_aEnemy[nCntEnemy].pBuffMat->Release();
			g_aEnemy[nCntEnemy].pBuffMat = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
// �X�V����
//-----------------------------------------------------------------------------
void UpdateEnemy(void)
{
	DWORD dwCurrentTime = timeGetTime();	// ���ݎ���

	EnemyUI *pEnemyUI = GetEnemyRemain();	// UI���擾
	pEnemyUI->nAmount = g_nEnemyAlive;		// �G�̎c�ʂ�UI���ɓn��

	//�g�p���̓G��T��
	for (int nCntEnemy = 0; nCntEnemy < ENEMY_AMOUNT_MAX; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			Player *pPlayer = GetPlayer();	// �v���C���[�̎擾

			//�ړ��ʒǉ�
			g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;

			//�ړ��ʌ���
			g_aEnemy[nCntEnemy].move.x = g_aEnemy[nCntEnemy].move.x * 0.2f;
			g_aEnemy[nCntEnemy].move.z = g_aEnemy[nCntEnemy].move.z * 0.2f;

			//�d��
			g_aEnemy[nCntEnemy].move.y -= ENEMY_FALLSPEED;

			//�̗͂Ȃ��Ȃ�����
			if (g_aEnemy[nCntEnemy].nLife <= 0)
			{
				VCollision *pVCol = GetVCol();

				//�g�p�󋵂�؂�ւ���
				g_aEnemy[nCntEnemy].bUse = false;

				//�����������Z
				g_nEnemyAlive--;

				//�G���j�\��������
				ShowEnemyDestroyed();

				//�����R���W��������
				pVCol[2].bUse = false;
			}

			//�v���C���[�Ƃ̋���
			float fDistanceToPlayer = sqrtf((pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x) * ((pPlayer->pos.x) - g_aEnemy[nCntEnemy].pos.x) + (pPlayer->pos.z - g_aEnemy[nCntEnemy].pos.z) * (pPlayer->pos.z - g_aEnemy[nCntEnemy].pos.z));

			//���������m�͈͈ȓ���������
			if (fDistanceToPlayer <= ENEMY_DETECT_RADIUS)
			{
				//�v���C���[�Ɍ������P�ʃx�N�g��
				D3DXVECTOR3 vecToPlayer = pPlayer->pos - g_aEnemy[nCntEnemy].pos;
				D3DXVec3Normalize(&vecToPlayer, &vecToPlayer);

				//�ړ������Z
				g_aEnemy[nCntEnemy].move += vecToPlayer * ENEMY_MOVESPEED;

				//�v���C���[�ւ̃��W�A���p
				float fRadianToPlayer = atan2f(g_aEnemy[nCntEnemy].pos.x - pPlayer->pos.x, g_aEnemy[nCntEnemy].pos.z - pPlayer->pos.z);
				g_aEnemy[nCntEnemy].rot.y = fRadianToPlayer;
			}

			//�������U���͈͓���������
			if (fDistanceToPlayer <= ENEMY_ATTACK_RADIUS)
			{
				//���łɍU�����Ă��Ȃ��ꍇ
				if (g_aEnemy[nCntEnemy].bAttack != true)
				{
					//�v���C���[�̖��G���Ԃ���Ȃ��ꍇ
					if (pPlayer->bInvincible != true)
					{
						//���ݎ��Ԏ擾�i���G���Ԍv�Z�j
						g_aEnemy[nCntEnemy].dwTimeAtk = timeGetTime();

						//���ݎ��Ԏ擾�i�U��CT�v�Z�j
						pPlayer->dwTime = timeGetTime();

						//�̗͌���
						pPlayer->nLife--;

						//�G���玩���Ɍ������x�N�g��
						D3DXVECTOR3 vecEnemyToPlayer = pPlayer->pos - g_aEnemy[nCntEnemy].pos;
						D3DXVec3Normalize(&vecEnemyToPlayer, &vecEnemyToPlayer);

						//�m�b�N�o�b�N
						pPlayer->move += vecEnemyToPlayer * ENEMY_KNOCKBACK;

						//���G�ɂȂ�
						pPlayer->bInvincible = true;

						//�G���U���N�[���^�C���ɓ���
						g_aEnemy[nCntEnemy].bAttack = true;

						//�_���[�W��
						PlaySound(SOUND_LABEL_SE_DMGPLAYER);
					}
				}
			}

			//���Ԍo�߂ōU��CT����
			if (dwCurrentTime - g_aEnemy[nCntEnemy].dwTimeAtk >= ENEMY_ATTACK_COOLTIME)
			{
				//CT����
				g_aEnemy[nCntEnemy].bAttack = false;
			}

			//���ɗ����Ă��������
			if (g_aEnemy[nCntEnemy].pos.y < -50.0f)
			{
				g_aEnemy[nCntEnemy].bUse = false;
			}
		}
	}

	//�S��
	if (g_nEnemyAlive <= 0)
	{
		if (g_bEliminated != true)
		{
			GAMEOVER *pGameOver = GetGameOver();
			RESULT *pResult = GetResult();

			//�Q�[���I�[�o�[��ʂ�`��
			pGameOver->bDraw = true;
			pGameOver->bGameOver = false;
			pResult->bGameOver = false;

			//���Ԏ擾
			g_dwTimeTransition = timeGetTime();

			g_bEliminated = true;
		}
		
		//1.5�b�o������
		if (dwCurrentTime - g_dwTimeTransition >= RESULT_WAITTIME)
		{
			//���U���g�ɑJ��
			SetFade(FADE_OUT, MODE_RESULT);
		}
	}
}

//-----------------------------------------------------------------------------
// �`�揈��
//-----------------------------------------------------------------------------
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();									// �f�o�C�X�擾

	for (int nCntEnemy = 0; nCntEnemy < ENEMY_AMOUNT_MAX; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			D3DXMATRIX mtxRot, mtxTrans;												// �v�Z�p�}�g���b�N�X
			D3DMATERIAL9 matDef;														// ���݂̃}�e���A���ۑ��p
			D3DXMATERIAL *pMat;															// �}�e���A���f�[�^�ւ̃|�C���^
			D3DXMATERIAL *pMatAlt;														// �ウ�̃}�e���A��
			//�v���C���[�̃��[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].mtxWorld);

			//�v���C���[�̌������f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].rot.y, g_aEnemy[nCntEnemy].rot.x, g_aEnemy[nCntEnemy].rot.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxRot);

			//�v���C���[�̈ʒu���f
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxTrans);

			//�v���C���[�̃��[���h�}�g���b�N�X�ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].mtxWorld);

			//���݂̃}�e���A���擾
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aEnemy[nCntEnemy].pBuffMat->GetBufferPointer();
			pMatAlt = (D3DXMATERIAL*)g_aEnemy[nCntEnemy].pBuffMat->GetBufferPointer();

			//���G��Ԃ����ă��f���̐F��ς���
			if (g_aEnemy[nCntEnemy].bInvincible == true)
			{//���G���
				pMatAlt->MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f);
				for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntEnemy].nNumMat; nCntMat++)
				{
					//�}�e���A���ݒ�
					pDevice->SetMaterial(&pMatAlt[nCntMat].MatD3D);

					//�e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_aEnemy[nCntEnemy].pTexture[nCntMat]);

					//���f���p�[�c�̕`��
					g_aEnemy[nCntEnemy].pMesh->DrawSubset(nCntMat);
				}
			}
			else if (g_aEnemy[nCntEnemy].bInvincible == false)
			{//�ʏ���
				pMat->MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0, 1.0f, 1.0f);
				for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntEnemy].nNumMat; nCntMat++)
				{
					//�}�e���A���ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					//�e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_aEnemy[nCntEnemy].pTexture[nCntMat]);

					//���f���p�[�c�̕`��
					g_aEnemy[nCntEnemy].pMesh->DrawSubset(nCntMat);
				}
			}

			//�ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//-----------------------------------------------------------------------------
// �v���C���[�̏����擾
//-----------------------------------------------------------------------------
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//-----------------------------------------------------------------------------
// �G��z�u����
//-----------------------------------------------------------------------------
void SetEnemy(D3DXVECTOR3 pos)
{
	for (int nCntEnemy = 0; nCntEnemy < ENEMY_AMOUNT_MAX; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{
			//�ʒu���w��
			g_aEnemy[nCntEnemy].pos = pos;

			//�ԍ��t��
			g_aEnemy[nCntEnemy].nIdx = nCntEnemy;

			//�������ɉ��Z
			g_nEnemyAlive++;

			//�g�p���Ɉڍs
			g_aEnemy[nCntEnemy].bUse = true;
			break;
		}
	}
}

/* ���f�����ړ�������֐� */
void MoveEnemy(float fMoveAngleDegree, float fMoveSpeed)
{
	/*Camera *pCamera = GetCamera();	// �J�����̏��擾

	g_enemy.pos.x += sinf(pCamera->rot.y + D3DXToRadian(fMoveAngleDegree)) * fMoveSpeed;
	g_enemy.pos.z += cosf(pCamera->rot.y + D3DXToRadian(fMoveAngleDegree)) * fMoveSpeed;
	g_enemy.rotDest.y = pCamera->rot.y + D3DXToRadian(fMoveAngleDegree) + D3DX_PI;*/
}

/* X�t�@�C�����烂�f����ǂݍ��ފ֐�*/
void LoadXFileEnemy(const char* cXFileName, int nCountModel)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�擾

	for (int nCntEnemy = 0; nCntEnemy < ENEMY_AMOUNT_MAX; nCntEnemy++)
	{
		//X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(
			cXFileName,										// �ǂݍ��ރt�@�C����
			D3DXMESH_SYSTEMMEM,								// �Œ�
			pDevice,										// �f�o�C�X
			NULL,											// NULL�Œ�
			&g_aEnemy[nCntEnemy].pBuffMat,					// �}�e���A��
			NULL,											// NULL�Œ�
			&g_aEnemy[nCntEnemy].nNumMat,					// �}�e���A����
			&g_aEnemy[nCntEnemy].pMesh);					// ���b�V��

		//�}�e���A�����ւ̃|�C���^
		D3DXMATERIAL *pMat = (D3DXMATERIAL*)g_aEnemy[nCntEnemy].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntEnemy].nNumMat; nCntMat++)
		{
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aEnemy[nCntEnemy].pTexture[nCntMat]);
		}
	}
}

/* �L�����̉�]�����炩�ɂ���֐� */
void EnemySmoothTurn(void)
{
	/*D3DXVECTOR3 RotDiff;

	//�����v�Z
	RotDiff.y = g_enemy.rotDest.y - g_enemy.rot.y;

	//������D3DX_PI�ȏ�(�����ȏ�)�̏ꍇ�A�t��]
	if (RotDiff.y > D3DX_PI)
	{
		g_enemy.rot.y -= ((D3DX_PI * 2) - RotDiff.y) * PLAYER_SMOOTHTURN_SPEED;
	}
	else if (RotDiff.y < -D3DX_PI)
	{
		g_enemy.rot.y += ((D3DX_PI * 2) + RotDiff.y) * PLAYER_SMOOTHTURN_SPEED;
	}
	else
	{
		g_enemy.rot.y += RotDiff.y * PLAYER_SMOOTHTURN_SPEED;
	}

	// ��]�̏C�� (3.14��������}�t��)
	if (g_enemy.rot.y > D3DX_PI)
	{
		g_enemy.rot.y -= D3DX_PI * 2.0f;
	}
	else if (g_enemy.rot.y < -D3DX_PI)
	{
		g_enemy.rot.y += D3DX_PI * 2.0f;
	}
	if (g_enemy.rotDest.y > D3DX_PI)
	{
		g_enemy.rotDest.y -= D3DX_PI * 2.0f;
	}
	else if (g_enemy.rotDest.y < -D3DX_PI)
	{
		g_enemy.rotDest.y += D3DX_PI * 2.0f;
	}*/
}