//-----------------------------------------------------------------------------
//
// �e���� [bullet.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "bullet.h"

//�}�N��
#define BULLET_WIDTH (20)
#define BULLET_HEIGHT (20)

//-----------------------------------------------------------------------------
//�\���̐錾
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;  //�ʒu
	D3DXVECTOR3 move; //�ړ���
	int nLife;		  //����
	bool bUse;		  //�g�p���Ă��邩
	BULLETSTATE state; //�e�̏��
	BULLETTYPE type;  //�e�̎��
} BULLET;

//-----------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	//�o�b�t�@�ւ̃|�C���^
BULLET g_aBullet[MAX_BULLET];						//�e�̏��
bool bTest;

//-----------------------------------------------------------------------------
//�e�̏���������
//-----------------------------------------------------------------------------
HRESULT InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\laser-bolts.png", &g_pTextureBullet);

	//BULLET�̏�����
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = 0;
		g_aBullet[nCntBullet].bUse = false;
		g_aBullet[nCntBullet].state = BULLETSTATE_OWNER_PLAYER;
	}

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * MAX_BULLET, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBullet, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�̃��b�N
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVertex, 0);

	//���_���W
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//���_���W�̐ݒ�
		pVertex[0].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVertex[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVertex[2].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVertex[3].pos = D3DXVECTOR3(0, 0, 0.0f);

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
		pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVertex += 4; //���_�̃|�C���^�����炷
	}
	
	g_pVtxBuffBullet->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//�e�̏I������
//-----------------------------------------------------------------------------
void UninitBullet(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}
}

//-----------------------------------------------------------------------------
//�e�̍X�V����
//-----------------------------------------------------------------------------
void UpdateBullet(void)
{
	BULLET *pBullet;
	ENEMY *pEnemy;
	PLAYER *pPlayer;
	VERTEX_2D *pVertex;
	
	pBullet = &g_aBullet[0];

	if (g_pVtxBuffBullet != NULL)
	{
		//���_�o�b�t�@�̃��b�N
		g_pVtxBuffBullet->Lock(0, 0, (void**)&pVertex, 0);

		for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
		{
			if (pBullet->bUse == true)
			{
				//�ʒu�X�V
				g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
				g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;

				pEnemy = GetEnemy();
				pPlayer = GetPlayer();

				//�v���C���[���e�𔭎˂����ꍇ
				if (g_aBullet[nCntBullet].state == BULLETSTATE_OWNER_PLAYER)
				{
					//�G�t�F�N�g
					SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f), 10, 0.06f);

					for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
					{
						if (pEnemy->bUse == true)
						{
							//UV�̐ݒ�
							pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f / 2);
							pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
							pVertex[2].tex = D3DXVECTOR2(1.0f / 2, 1.0f / 2);
							pVertex[3].tex = D3DXVECTOR2(1.0f / 2, 0.0f);

							switch (pEnemy->nType)
							{
							case ENEMYTYPE_BIG:
								if (pBullet->pos.x > pEnemy->pos.x - ENEMY_BIG_WIDTH && pBullet->pos.x < pEnemy->pos.x + ENEMY_BIG_HEIGHT &&
									pBullet->pos.y > pEnemy->pos.y - ENEMY_BIG_WIDTH && pBullet->pos.y < pEnemy->pos.y + ENEMY_BIG_HEIGHT)
								{
									//�q�b�g����
									HitEnemy(nCntEnemy, 5);

									//�e�̔j��
									pBullet->bUse = false;
								}
								break;

							case ENEMYTYPE_MEDIUM:
								if (pBullet->pos.x > pEnemy->pos.x - ENEMY_MEDIUM_WIDTH && pBullet->pos.x < pEnemy->pos.x + ENEMY_MEDIUM_HEIGHT &&
									pBullet->pos.y > pEnemy->pos.y - ENEMY_MEDIUM_WIDTH && pBullet->pos.y < pEnemy->pos.y + ENEMY_MEDIUM_HEIGHT)
								{
									//�q�b�g����
									HitEnemy(nCntEnemy, 5);

									//�e�̔j��
									pBullet->bUse = false;
								}
								break;

							case ENEMYTYPE_SMALL:
								if (pBullet->pos.x > pEnemy->pos.x - ENEMY_BIG_WIDTH && pBullet->pos.x < pEnemy->pos.x + ENEMY_SMALL_HEIGHT &&
									pBullet->pos.y > pEnemy->pos.y - ENEMY_BIG_WIDTH && pBullet->pos.y < pEnemy->pos.y + ENEMY_SMALL_HEIGHT)
								{
									//�q�b�g����
									HitEnemy(nCntEnemy, 5);

									//�e�̔j��
									pBullet->bUse = false;
								}
								break;

							default:
								break;
							}


						}
					}
				}

				//�G���e�𔭎˂����ꍇ
				if (g_aBullet[nCntBullet].state == BULLETSTATE_OWNER_ENEMY)
				{
					if (pPlayer->bUse == true)
					{
						//UV�̐ݒ�
						pVertex[0].tex = D3DXVECTOR2(1.0f / 2, 1.0f / 2);
						pVertex[1].tex = D3DXVECTOR2(1.0f / 2, 0.0f);
						pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f / 2);
						pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f);
						
						//�G�t�F�N�g
						SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.6f, 0.2f, 0.2f, 1.0f), 10, 0.06f);

						if (pBullet->pos.x > pPlayer->pos.x - PLAYER_WIDTH && pBullet->pos.x < pPlayer->pos.x + PLAYER_HEIGHT &&
							pBullet->pos.y > pPlayer->pos.y - PLAYER_WIDTH && pBullet->pos.y < pPlayer->pos.y + PLAYER_HEIGHT)
						{
							if (CheckBarrierActive() == true)
							{
								BreakBarrier();

							}
							else
							{
								//�q�b�g����
								HitPlayer(5);
							}

							//�e�̔j��
							pBullet->bUse = false;
						}
					}
				}

				//���_���W�̍X�V
				pVertex[0].pos = D3DXVECTOR3(-BULLET_WIDTH + g_aBullet[nCntBullet].pos.x, BULLET_HEIGHT + g_aBullet[nCntBullet].pos.y, 0.0f);
				pVertex[1].pos = D3DXVECTOR3(-BULLET_WIDTH + g_aBullet[nCntBullet].pos.x, -BULLET_HEIGHT + g_aBullet[nCntBullet].pos.y, 0.0f);
				pVertex[2].pos = D3DXVECTOR3(BULLET_WIDTH + g_aBullet[nCntBullet].pos.x, BULLET_HEIGHT + g_aBullet[nCntBullet].pos.y, 0.0f);
				pVertex[3].pos = D3DXVECTOR3(BULLET_WIDTH + g_aBullet[nCntBullet].pos.x, -BULLET_HEIGHT + g_aBullet[nCntBullet].pos.y, 0.0f);


	
				//�����`�F�b�N
				g_aBullet[nCntBullet].nLife--;
				if (g_aBullet[nCntBullet].nLife < 0)
				{
					//SetExplosion(g_aBullet[nCntBullet].pos, 50);
					g_aBullet[nCntBullet].bUse = false;
				}

				//��ʊO�`�F�b�N
				if (g_aBullet[nCntBullet].pos.x > SCREEN_WIDTH || g_aBullet[nCntBullet].pos.x < 0)
				{
					g_aBullet[nCntBullet].bUse = false;
				}
				if (g_aBullet[nCntBullet].pos.y > SCREEN_HEIGHT || g_aBullet[nCntBullet].pos.y < 0)
				{
					g_aBullet[nCntBullet].bUse = false;
				}
			}
			pVertex += 4; //���_�̃|�C���^�����炷
		}

		//���_�o�b�t�@�̃A�����b�N
		g_pVtxBuffBullet->Unlock();
	}
}

//-----------------------------------------------------------------------------
//�e�̕`�揈��
//-----------------------------------------------------------------------------
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBullet);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}
	}
}

//-----------------------------------------------------------------------------
//�e�̐ݒ�
//-----------------------------------------------------------------------------
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, BULLETSTATE state, BULLETTYPE type)
{
	BULLET *pBullet;
	VERTEX_2D *pVertex;

	pBullet = &g_aBullet[0];

	//���_�o�b�t�@�̃��b�N
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse == false)
		{
			//�ʒu��ݒ�
			g_aBullet[nCntBullet].pos = pos;

			//���_���W��ݒ�
			pVertex[0].pos = D3DXVECTOR3(-10 + g_aBullet[nCntBullet].pos.x, 10 + g_aBullet[nCntBullet].pos.y, 0.0f);
			pVertex[1].pos = D3DXVECTOR3(-10 + g_aBullet[nCntBullet].pos.x, -10 + g_aBullet[nCntBullet].pos.y, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(10 + g_aBullet[nCntBullet].pos.x, 10 + g_aBullet[nCntBullet].pos.y, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(10 + g_aBullet[nCntBullet].pos.x, -10 + g_aBullet[nCntBullet].pos.y, 0.0f);

			//�ړ��ʂ�ݒ�
			g_aBullet[nCntBullet].move = move;

			//������ݒ�
			g_aBullet[nCntBullet].nLife = nLife;

			//�X�e�[�g��ݒ�
			g_aBullet[nCntBullet].state = state;

			//�e�̎�ނ�ݒ�
			g_aBullet[nCntBullet].type = type;

			pBullet->bUse = true;
			break;

		}
		//���_�̃|�C���^�����炷
		pVertex += 4;
	}
	g_pVtxBuffBullet->Unlock();
}