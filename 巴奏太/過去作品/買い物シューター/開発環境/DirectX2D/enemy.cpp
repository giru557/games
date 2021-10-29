//-----------------------------------------------------------------------------
//
// �G�̏��� [enemy.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "enemy.h"
#include "bullet.h"
#include "player.h"
#include "money.h"
#include "fade.h"
#include "complete.h"
#include <time.h>

//-----------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_apTextureEnemy[MAX_TYPE_ENEMY] = { NULL, NULL, NULL }; //�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;					//���_�o�b�t�@�ւ̃|�C���^
ENEMY g_aEnemy[MAX_ENEMY];										//�G�̏��
int g_nEnemyAmount;												//�G�̐�
int g_nEnemyAmountGroup[2];										//�O���[�v���Ƃ̓G�̐�
int g_aIntervalCount[MAX_ENEMY];								//�G�̍U���C���^�[�o��
int g_aCounterEnemy[MAX_ENEMY];									//�G�̃A�j���[�V�����J�E���^�[
int g_aPatternEnemy[MAX_ENEMY];									//�p�^�[��
int g_aTypeEnemy[MAX_TYPE_ENEMY];								//�G�̃^�C�v
bool g_bActiveWave;												//�E�F�[�u�̗L����
bool g_bFlag;

//-----------------------------------------------------------------------------
//�v���g�^�C�v�錾
void EnemyMove(int nIdx, float fEnemySpeed, int nLeftorRight);
void GroupMove(ENEMYGROUP group, int nLeftorRight, float fGroupSpeed);

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//�G�̏���������
//-----------------------------------------------------------------------------
HRESULT InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;
	
	srand((unsigned int)time(0));

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\enemy-big.png", &g_apTextureEnemy[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\enemy-medium.png", &g_apTextureEnemy[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\enemy-small.png", &g_apTextureEnemy[2]);

	//ENEMY�̏�����
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].nType = ENEMYTYPE_BIG;
		g_aEnemy[nCntEnemy].bUse = false;
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
		g_aEnemy[nCntEnemy].nCounterState = 0;
		g_aEnemy[nCntEnemy].nLife = 10;
		g_aEnemy[nCntEnemy].nAtkInterval = rand() % 1200 + 200;
	}
	g_nEnemyAmount = 0;
	g_nEnemyAmountGroup[0] = 0;
	g_bActiveWave = false;
	g_bFlag = false;

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * MAX_ENEMY, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffEnemy, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//���_���W D3DXVECTOR3(X,Y, 0.0f);
		pVertex[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		 //TRIANGLESTRIP�Ŏl�p
		pVertex[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVertex[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVertex[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhw�̐ݒ�
		pVertex[0].rhw = 1.0f;	//1.0f�Œ�
		pVertex[1].rhw = 1.0f;
		pVertex[2].rhw = 1.0f;
		pVertex[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVertex[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);  //D3DCOLOR_RGBA(��, ��, ��, �����x);
		pVertex[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVertex[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVertex[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//UV�̐ݒ�
		pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVertex[2].tex = D3DXVECTOR2(1.0f / 2, 1.0f);
		pVertex[3].tex = D3DXVECTOR2(1.0f / 2, 0.0f);

		pVertex += 4; //���_�̃|�C���^�����炷
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();

	//�G����
	for (int nCntEnemy = 1; nCntEnemy <= 10; nCntEnemy++)
	{
		SetEnemy(D3DXVECTOR3((nCntEnemy * 100.0f) + 85, 50, 0.0f), D3DXVECTOR3(sinf((-D3DX_PI / 2) * ENEMY_SPEED), 0.0f, 0.0f), ENEMYTYPE_BIG, ENEMYGROUP_A);
		SetEnemy(D3DXVECTOR3((nCntEnemy * 100.0f) + 85, 150, 0.0f), D3DXVECTOR3(sinf((D3DX_PI / 2) * ENEMY_SPEED), 0.0f, 0.0f), ENEMYTYPE_MEDIUM, ENEMYGROUP_B);
		SetEnemy(D3DXVECTOR3((nCntEnemy * 100.0f) + 85, 250.0f, 0.0f), D3DXVECTOR3(sinf((-D3DX_PI / 2) * ENEMY_SPEED), 0.0f, 0.0f), ENEMYTYPE_SMALL, ENEMYGROUP_C);
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
//�G�̏I������
//-----------------------------------------------------------------------------
void UninitEnemy(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}

	//�e�N�X�`���̊J��
	for (int nCntRelease = 0; nCntRelease < 3; nCntRelease++)
	{
		if (g_apTextureEnemy[nCntRelease] != NULL)
		{
			g_apTextureEnemy[nCntRelease]->Release();
			g_apTextureEnemy[nCntRelease] = NULL;
		}
	}
	
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		g_aEnemy[nCnt].bUse = false;
	}
}

//-----------------------------------------------------------------------------
//�G�̍X�V����
//-----------------------------------------------------------------------------
void UpdateEnemy(void)
{
	ENEMY *pEnemy;
	VERTEX_2D *pVertex;
	PLAYER *pPlayer;
	static int nWait = 0;
	
	pPlayer = GetPlayer();
	pEnemy = &g_aEnemy[0];

	if (g_pVtxBuffEnemy != NULL)
	{
		//���_�o�b�t�@�̃��b�N
		g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVertex, 0);

		for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
		{
			if (pEnemy->bUse == true)
			{
				g_aCounterEnemy[nCntEnemy]++;

				if (g_aCounterEnemy[nCntEnemy] % 10 == 0)
				{
					if (g_aPatternEnemy[nCntEnemy] == 0)
					{
						g_aPatternEnemy[nCntEnemy] = 1;
					}
					else if (g_aPatternEnemy[nCntEnemy] == 1)
					{
						g_aPatternEnemy[nCntEnemy] = 0;
					}
				}

				if (g_aPatternEnemy[nCntEnemy] == 0)
				{
					//�A�j���[�V����
					pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVertex[2].tex = D3DXVECTOR2(1.0f / 2, 1.0f);
					pVertex[3].tex = D3DXVECTOR2(1.0f / 2, 0.0f);
				}
				else
				{
					//�A�j���[�V����
					pVertex[0].tex = D3DXVECTOR2(0.5f, 1.0f);
					pVertex[1].tex = D3DXVECTOR2(0.5f, 0.0f);
					pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f);
					pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f);
				}

				//���_���W�̐ݒ�
				switch (g_aEnemy[nCntEnemy].nType)
				{
				case ENEMYTYPE_BIG:
					pVertex[0].pos = D3DXVECTOR3(-ENEMY_BIG_WIDTH + g_aEnemy[nCntEnemy].pos.x, ENEMY_BIG_HEIGHT + g_aEnemy[nCntEnemy].pos.y, 0.0f);
					pVertex[1].pos = D3DXVECTOR3(-ENEMY_BIG_WIDTH + g_aEnemy[nCntEnemy].pos.x, -ENEMY_BIG_HEIGHT + g_aEnemy[nCntEnemy].pos.y, 0.0f);
					pVertex[2].pos = D3DXVECTOR3(ENEMY_BIG_WIDTH + g_aEnemy[nCntEnemy].pos.x, ENEMY_BIG_HEIGHT + g_aEnemy[nCntEnemy].pos.y, 0.0f);
					pVertex[3].pos = D3DXVECTOR3(ENEMY_BIG_WIDTH + g_aEnemy[nCntEnemy].pos.x, -ENEMY_BIG_HEIGHT + g_aEnemy[nCntEnemy].pos.y, 0.0f);
					break;

				case ENEMYTYPE_MEDIUM:
					pVertex[0].pos = D3DXVECTOR3(-ENEMY_MEDIUM_WIDTH + g_aEnemy[nCntEnemy].pos.x, ENEMY_MEDIUM_HEIGHT + g_aEnemy[nCntEnemy].pos.y, 0.0f);
					pVertex[1].pos = D3DXVECTOR3(-ENEMY_MEDIUM_WIDTH + g_aEnemy[nCntEnemy].pos.x, -ENEMY_MEDIUM_HEIGHT + g_aEnemy[nCntEnemy].pos.y, 0.0f);
					pVertex[2].pos = D3DXVECTOR3(ENEMY_MEDIUM_WIDTH + g_aEnemy[nCntEnemy].pos.x, ENEMY_MEDIUM_HEIGHT + g_aEnemy[nCntEnemy].pos.y, 0.0f);
					pVertex[3].pos = D3DXVECTOR3(ENEMY_MEDIUM_WIDTH + g_aEnemy[nCntEnemy].pos.x, -ENEMY_MEDIUM_HEIGHT + g_aEnemy[nCntEnemy].pos.y, 0.0f);
					break;

				case ENEMYTYPE_SMALL:
					pVertex[0].pos = D3DXVECTOR3(-ENEMY_SMALL_WIDTH + g_aEnemy[nCntEnemy].pos.x, ENEMY_SMALL_HEIGHT + g_aEnemy[nCntEnemy].pos.y, 0.0f);
					pVertex[1].pos = D3DXVECTOR3(-ENEMY_SMALL_WIDTH + g_aEnemy[nCntEnemy].pos.x, -ENEMY_SMALL_HEIGHT + g_aEnemy[nCntEnemy].pos.y, 0.0f);
					pVertex[2].pos = D3DXVECTOR3(ENEMY_SMALL_WIDTH + g_aEnemy[nCntEnemy].pos.x, ENEMY_SMALL_HEIGHT + g_aEnemy[nCntEnemy].pos.y, 0.0f);
					pVertex[3].pos = D3DXVECTOR3(ENEMY_SMALL_WIDTH + g_aEnemy[nCntEnemy].pos.x, -ENEMY_SMALL_HEIGHT + g_aEnemy[nCntEnemy].pos.y, 0.0f);
					break;

				default:
					break;
				}

				if (pPlayer->state != PLAYERSTATE_WAIT)
				{
					g_aIntervalCount[nCntEnemy]++;

					//�G�̈ʒu�X�V
					g_aEnemy[nCntEnemy].pos.x += g_aEnemy[nCntEnemy].move.x;
					g_aEnemy[nCntEnemy].pos.y += g_aEnemy[nCntEnemy].move.y;

					//��ʊO����
					if (g_aEnemy[nCntEnemy].pos.x < 0 - ENEMY_SIZE || g_aEnemy[nCntEnemy].pos.x > 1280 + ENEMY_SIZE)
					{
						g_aEnemy[nCntEnemy].bUse = false;
						g_nEnemyAmount--;
					}
					else if (g_aEnemy[nCntEnemy].pos.y > 720 + ENEMY_SIZE)
					{
						g_aEnemy[nCntEnemy].bUse = false;
						g_nEnemyAmount--;
					}

					//�X�e�[�g
					switch (g_aEnemy[nCntEnemy].state)
					{
						//�_���[�W���󂯂��ꍇ
					case ENEMYSTATE_DAMAGE:
						g_aEnemy[nCntEnemy].nCounterState--;
						if (g_aEnemy[nCntEnemy].nCounterState <= 0)
						{
							g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

							//�|���S���̐F��߂�
							pVertex[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);  //D3DCOLOR_RGBA(��, ��, ��, �����x);
							pVertex[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
							pVertex[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
							pVertex[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						}
						break;
					}

					//�O���[�v���Ƃ̏���
					switch (g_aEnemy[nCntEnemy].group)
					{
					case ENEMYGROUP_A:
						if (g_aEnemy[nCntEnemy].pos.x - ENEMY_BIG_WIDTH < 0)
						{
							GroupMove(ENEMYGROUP_A, 1, ENEMY_SPEED);
						}
						else if (g_aEnemy[nCntEnemy].pos.x + ENEMY_BIG_WIDTH > SCREEN_WIDTH)
						{
							GroupMove(ENEMYGROUP_A, 0, ENEMY_SPEED);
						}
						break;

					case ENEMYGROUP_B:
						if (g_aEnemy[nCntEnemy].pos.x - ENEMY_MEDIUM_WIDTH < 0)
						{
							GroupMove(ENEMYGROUP_B, 1, ENEMY_SPEED);
						}
						else if (g_aEnemy[nCntEnemy].pos.x + ENEMY_MEDIUM_WIDTH > SCREEN_WIDTH)
						{
							GroupMove(ENEMYGROUP_B, 0, ENEMY_SPEED);
						}
						break;

					case ENEMYGROUP_C:
						if (g_aEnemy[nCntEnemy].pos.x - ENEMY_SMALL_WIDTH < 0)
						{
							GroupMove(ENEMYGROUP_C, 1, ENEMY_SPEED);
						}
						else if (g_aEnemy[nCntEnemy].pos.x + ENEMY_SMALL_WIDTH > SCREEN_WIDTH)
						{
							GroupMove(ENEMYGROUP_C, 0, ENEMY_SPEED);
						}
						break;

					case ENEMYGROUP_D:
						if (g_bFlag == true)
						{
							if (g_aEnemy[nCntEnemy].pos.y > 50)
							{
								GroupMove(ENEMYGROUP_D, 0, ENEMY_SPEED);
							}
						}
						if (g_aEnemy[nCntEnemy].pos.x - ENEMY_BIG_WIDTH < 0)
						{
							GroupMove(ENEMYGROUP_D, 1, ENEMY_SPEED);
						}
						else if (g_aEnemy[nCntEnemy].pos.x + ENEMY_BIG_WIDTH > SCREEN_WIDTH)
						{
							GroupMove(ENEMYGROUP_D, 0, ENEMY_SPEED);
						}
						break;

					case ENEMYGROUP_E:
						if (g_bFlag == true)
						{
							if (g_aEnemy[nCntEnemy].pos.y > 150)
							{
								GroupMove(ENEMYGROUP_E, 0, ENEMY_SPEED);
							}
						}
						if (g_aEnemy[nCntEnemy].pos.x - ENEMY_BIG_WIDTH < 0)
						{
							GroupMove(ENEMYGROUP_E, 1, ENEMY_SPEED);
						}
						else if (g_aEnemy[nCntEnemy].pos.x + ENEMY_BIG_WIDTH > SCREEN_WIDTH)
						{
							GroupMove(ENEMYGROUP_E, 0, ENEMY_SPEED);
						}
						break;

					case ENEMYGROUP_F:
						if (g_bFlag == true)
						{
							if (g_aEnemy[nCntEnemy].pos.y > 250)
							{
								GroupMove(ENEMYGROUP_F, 0, ENEMY_SPEED);
								g_bFlag = false;
							}
						}
						if (g_aEnemy[nCntEnemy].pos.x - ENEMY_MEDIUM_WIDTH < 0)
						{
							GroupMove(ENEMYGROUP_F, 1, ENEMY_SPEED);
						}
						else if (g_aEnemy[nCntEnemy].pos.x + ENEMY_MEDIUM_WIDTH > SCREEN_WIDTH)
						{
							GroupMove(ENEMYGROUP_F, 0, ENEMY_SPEED);
						}
						break;

					default:
						break;
					}

					if (g_aEnemy[nCntEnemy].pos.y > 0)
					{
						if (g_aEnemy[nCntEnemy].group == ENEMYGROUP_D ||
							g_aEnemy[nCntEnemy].group == ENEMYGROUP_E ||
							g_aEnemy[nCntEnemy].group == ENEMYGROUP_F)
						{
							g_aEnemy[nCntEnemy].nAtkInterval = 200;
						}
						//�G�̍U���Ԋu
						if (g_aIntervalCount[nCntEnemy] % g_aEnemy[nCntEnemy].nAtkInterval == 0)
						{
							if (g_aIntervalCount[nCntEnemy] > g_aEnemy[nCntEnemy].nAtkInterval)
							{
								g_aIntervalCount[nCntEnemy] = 0;
							}
							D3DXVECTOR3 test;
							test.x = sinf(atan2f(pPlayer->pos.x - pEnemy->pos.x, pPlayer->pos.y - pEnemy->pos.y));
							test.y = cosf(atan2f(pPlayer->pos.x - pEnemy->pos.x, pPlayer->pos.y - pEnemy->pos.y));

							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + 30, 0.0f), D3DXVECTOR3(test.x * 7, test.y * 7, 0.0f), 500, BULLETSTATE_OWNER_ENEMY, BULLETTYPE_NORMAL);
						}
					}
				}
			}

			pVertex += 4; //���_�̃|�C���^�����炷
		}
		//���_�o�b�t�@�̃A�����b�N
		g_pVtxBuffEnemy->Unlock();

		//�G�̐��`�F�b�N
		if (g_nEnemyAmount <= 0)
		{
			if (g_bActiveWave == true)
			{
				nWait++;

				ShowStageComplete();

				if (nWait > 180)
				{
					SetFade(FADE_OUT, MODE_RESULT);
				}
			}
			else
			{
				for (int nCntEnemy = 1; nCntEnemy <= 10; nCntEnemy++)
				{
					/*SetEnemy(D3DXVECTOR3((nCntEnemy * 100.0f) + 95, 50, 0.0f), D3DXVECTOR3(sinf((-D3DX_PI / 2) * ENEMY_SPEED), 0.0f, 0.0f), ENEMYTYPE_BIG, ENEMYGROUP_D);
					SetEnemy(D3DXVECTOR3((nCntEnemy * 100.0f) + 95, 150, 0.0f), D3DXVECTOR3(sinf((D3DX_PI / 2) * ENEMY_SPEED), 0.0f, 0.0f), ENEMYTYPE_MEDIUM, ENEMYGROUP_E);
					SetEnemy(D3DXVECTOR3((nCntEnemy * 100.0f) + 95, 250.0f, 0.0f), D3DXVECTOR3(sinf((-D3DX_PI / 2) * ENEMY_SPEED), 0.0f, 0.0f), ENEMYTYPE_SMALL, ENEMYGROUP_F);*/
					SetEnemy(D3DXVECTOR3((nCntEnemy * 100.0f) + 85, -750, 0.0f), D3DXVECTOR3(0.0f, cosf(0) * 3, 0.0f), ENEMYTYPE_BIG, ENEMYGROUP_D);
					SetEnemy(D3DXVECTOR3((nCntEnemy * 100.0f) + 85, -650, 0.0f), D3DXVECTOR3(0.0f, cosf(0) * 3, 0.0f), ENEMYTYPE_BIG, ENEMYGROUP_E);
					SetEnemy(D3DXVECTOR3((nCntEnemy * 100.0f) + 85, -550.0f, 0.0f), D3DXVECTOR3(0.0f, cosf(0) * 3, 0.0f), ENEMYTYPE_MEDIUM, ENEMYGROUP_F);
				}
				g_bFlag = true;
				g_bActiveWave = true;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//�G�̕`�揈��
//-----------------------------------------------------------------------------
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			switch (g_aEnemy[nCntEnemy].nType)
			{
			case ENEMYTYPE_BIG:
				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_apTextureEnemy[0]);
				break;

			case ENEMYTYPE_MEDIUM:
				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_apTextureEnemy[1]);
				break;

			case ENEMYTYPE_SMALL:
				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_apTextureEnemy[2]);
				break;

			default:
				break;
			}

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, 2);
		}
	}
}

//-----------------------------------------------------------------------------
//�G�̐ݒ�
//-----------------------------------------------------------------------------
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, ENEMYTYPE nType, ENEMYGROUP group)
{
	ENEMY *pEnemy;
	VERTEX_2D *pVertex;

	g_nEnemyAmount++;

	pEnemy = &g_aEnemy[0];

	//���_�o�b�t�@�̃��b�N
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == false)
		{
			//�ʒu��ݒ�
			g_aEnemy[nCntEnemy].pos = pos;

			//�G�̈ړ�
			g_aEnemy[nCntEnemy].move = move;

			//���
			g_aEnemy[nCntEnemy].nType = nType;

			//�O���[�v
			g_aEnemy[nCntEnemy].group = group;

			//�̗͐ݒ�
			switch (g_aEnemy[nCntEnemy].nType)
			{
			case ENEMYTYPE_BIG:
				g_aEnemy[nCntEnemy].nLife = 15;
				break;

			case ENEMYTYPE_MEDIUM:
				g_aEnemy[nCntEnemy].nLife = 10;
				break;

			case ENEMYTYPE_SMALL:
				g_aEnemy[nCntEnemy].nLife = 5;
				break;

			default:
				break;
			}

			//���_���W��ݒ�
			pVertex[0].pos = D3DXVECTOR3(-ENEMY_SIZE + g_aEnemy[nCntEnemy].pos.x, ENEMY_SIZE + g_aEnemy[nCntEnemy].pos.y, 0.0f);
			pVertex[1].pos = D3DXVECTOR3(-ENEMY_SIZE + g_aEnemy[nCntEnemy].pos.x, -ENEMY_SIZE + g_aEnemy[nCntEnemy].pos.y, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(ENEMY_SIZE + g_aEnemy[nCntEnemy].pos.x, ENEMY_SIZE + g_aEnemy[nCntEnemy].pos.y, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(ENEMY_SIZE + g_aEnemy[nCntEnemy].pos.x, -ENEMY_SIZE + g_aEnemy[nCntEnemy].pos.y, 0.0f);

			pEnemy->bUse = true;
			break;
		}
		pVertex += 4; //���_�̃|�C���^�����炷
	}
	g_pVtxBuffEnemy->Unlock();
}

ENEMY *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//-----------------------------------------------------------------------------
//�G�ɓ�����������
//-----------------------------------------------------------------------------
bool HitEnemy(int nIdx, int nDamage)
{
	VERTEX_2D *pVertex;
	g_aEnemy[nIdx].nLife -= nDamage;

	//���_�o�b�t�@�̃��b�N
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVertex, 0);

	pVertex += 4 * nIdx;

	if (g_aEnemy[nIdx].nLife <= 0)
	{//�G������

		switch (g_aEnemy[nIdx].nType)
		{
		case ENEMYTYPE_BIG:
			//�����A�j���[�V����
			SetExplosion(g_aEnemy[nIdx].pos, ENEMY_BIG_WIDTH + 20);
			//�X�R�A�ǉ�
			AddScore(350);
			//�����ǉ�
			AddMoney(5);
			break;

		case ENEMYTYPE_MEDIUM:
			//�����A�j���[�V����
			SetExplosion(g_aEnemy[nIdx].pos, ENEMY_MEDIUM_WIDTH + 20);
			//�X�R�A�ǉ�
			AddScore(150);
			//�����ǉ�
			AddMoney(2);
			break;

		case ENEMYTYPE_SMALL:
			//�����A�j���[�V����
			SetExplosion(g_aEnemy[nIdx].pos, ENEMY_SMALL_WIDTH + 20);
			//�X�R�A�ǉ�
			AddScore(75);
			//�����ǉ�
			AddMoney(1);
			break;

		default:
			break;
		}

    	g_nEnemyAmount--;

		g_aEnemy[nIdx].bUse = false;

		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_SE_EXPLOSION);

		return true;
	}
	else
	{//�G���܂������Ă�

		//�q�b�g���Đ�
		PlaySound(SOUND_LABEL_SE_HIT2);

		g_aEnemy[nIdx].state = ENEMYSTATE_DAMAGE;
		g_aEnemy[nIdx].nCounterState = 10;

		//�|���S���̐F��ύX
		pVertex[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);  //D3DCOLOR_RGBA(��, ��, ��, �����x);
		pVertex[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVertex[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVertex[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

		return false;
	}
	g_pVtxBuffEnemy->Unlock();
}

//-----------------------------------------------------------------------------
//�G ���E�ړ�
//-----------------------------------------------------------------------------
void EnemyMove(int nIdx, float fEnemySpeed, int nLeftorRight)
{
	switch (nLeftorRight)
	{
	case 0:
		g_aEnemy[nIdx].move = D3DXVECTOR3(sinf(-D3DX_PI / 2) * fEnemySpeed, 0.0f, 0.0f);
		break;

	case 1:
		g_aEnemy[nIdx].move = D3DXVECTOR3(sinf(D3DX_PI / 2) * fEnemySpeed, 0.0f, 0.0f);
		break;

	default:
		break;
	}
}

//�G�O���[�v�ړ�
void GroupMove(ENEMYGROUP group, int nLeftorRight, float fGroupSpeed)
{
	if (nLeftorRight == 0)
	{
		for (int nCntGroup = 0; nCntGroup < MAX_ENEMY; nCntGroup++)
		{//�G�S�̌�������
			if (g_aEnemy[nCntGroup].bUse == true)
			{//�g�p���̓z�����ɍi����
				if (g_aEnemy[nCntGroup].group == group)
				{//�O���[�v���i����
					EnemyMove(nCntGroup, fGroupSpeed, 0); //�G�ړ� �E
				}
			}
		}
	}
	else if (nLeftorRight == 1)
	{
		for (int nCntGroup = 0; nCntGroup < MAX_ENEMY; nCntGroup++)
		{
			if (g_aEnemy[nCntGroup].bUse == true)
			{
				if (g_aEnemy[nCntGroup].group == group)
				{
					EnemyMove(nCntGroup, fGroupSpeed, 1); //�G�ړ� ��
				}
			}
		}
	}
}