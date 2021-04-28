//-----------------------------------------------------------------------------
//
// �~�T�C���̏��� [missile.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "missile.h"
#include <cmath>

//-----------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------
#define MISSILE_WIDTH ((41 / 2) * 0.8f)
#define MISSILE_HEIGHT ((92 / 2) * 0.8f)
#define MISSILE_EXPLOSIVE (100)

//-----------------------------------------------------------------------------
//�\���̐錾
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;  //�ʒu
	D3DXVECTOR3 move; //�ړ���
	int nLife;		  //����
	bool bUse;		  //�g�p���Ă��邩
	MISSILESTATE state; //�e�̏��
	int nCounterAnim; //�A�j���[�V�����J�E���^
	int nPatternAnim; //�A�j���[�V�����p�^�[��
} MISSILE;

//-----------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureMissile = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMissile = NULL;	//�o�b�t�@�ւ̃|�C���^
MISSILE g_aMissile[MAX_MISSILE];						//�e�̏��

//-----------------------------------------------------------------------------
//�~�T�C���̏���������
//-----------------------------------------------------------------------------
HRESULT InitMissile(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\missile2.png", &g_pTextureMissile);
	D3DXCreateTextureFromFileEx(pDevice, "data\\TEXTURE\\missile2.png", 82, 92, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0xFF000000, NULL, NULL, &g_pTextureMissile);

	//BULLET�̏�����
	for (int nCntMissile = 0; nCntMissile < MAX_MISSILE; nCntMissile++)
	{
		g_aMissile[nCntMissile].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMissile[nCntMissile].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMissile[nCntMissile].nLife = 0;
		g_aMissile[nCntMissile].bUse = false;
		g_aMissile[nCntMissile].state = MISSILESTATE_OWNER_PLAYER;
	}

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * MAX_MISSILE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffMissile, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�̃��b�N
	g_pVtxBuffMissile->Lock(0, 0, (void**)&pVertex, 0);

	//���_���W
	for (int nCntMissile = 0; nCntMissile < MAX_MISSILE; nCntMissile++)
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
		pVertex[2].tex = D3DXVECTOR2(1.0f / 2, 1.0f);
		pVertex[3].tex = D3DXVECTOR2(1.0f / 2, 0.0f);

		pVertex += 4; //���_�̃|�C���^�����炷
	}

	g_pVtxBuffMissile->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//�~�T�C���̏I������
//-----------------------------------------------------------------------------
void UninitMissile(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffMissile != NULL)
	{
		g_pVtxBuffMissile->Release();
		g_pVtxBuffMissile = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pTextureMissile != NULL)
	{
		g_pTextureMissile->Release();
		g_pTextureMissile = NULL;
	}
}

//-----------------------------------------------------------------------------
//�~�T�C���̍X�V����
//-----------------------------------------------------------------------------
void UpdateMissile(void)
{
	MISSILE *pMissile;
	ENEMY *pEnemy;
	PLAYER *pPlayer;
	VERTEX_2D *pVertex;

	pMissile = &g_aMissile[0];

	if (g_pVtxBuffMissile != NULL)
	{
		//���_�o�b�t�@�̃��b�N
		g_pVtxBuffMissile->Lock(0, 0, (void**)&pVertex, 0);

		for (int nCntMissile = 0; nCntMissile < MAX_MISSILE; nCntMissile++, pMissile++)
		{
			if (pMissile->bUse == true)
			{
				//�ʒu�X�V
				g_aMissile[nCntMissile].pos.x += g_aMissile[nCntMissile].move.x;
				g_aMissile[nCntMissile].pos.y += g_aMissile[nCntMissile].move.y;

				//���_���W��ݒ�
				pVertex[0].pos = D3DXVECTOR3(-MISSILE_WIDTH + g_aMissile[nCntMissile].pos.x, MISSILE_HEIGHT + g_aMissile[nCntMissile].pos.y, 0.0f);
				pVertex[1].pos = D3DXVECTOR3(-MISSILE_WIDTH + g_aMissile[nCntMissile].pos.x, -MISSILE_HEIGHT + g_aMissile[nCntMissile].pos.y, 0.0f);
				pVertex[2].pos = D3DXVECTOR3(MISSILE_WIDTH + g_aMissile[nCntMissile].pos.x, MISSILE_HEIGHT + g_aMissile[nCntMissile].pos.y, 0.0f);
				pVertex[3].pos = D3DXVECTOR3(MISSILE_WIDTH + g_aMissile[nCntMissile].pos.x, -MISSILE_HEIGHT + g_aMissile[nCntMissile].pos.y, 0.0f);

				//ANIMATION_INTERVAL�̐��Ŋ���؂ꂽ��
				g_aMissile[nCntMissile].nCounterAnim++;
				if (g_aMissile[nCntMissile].nCounterAnim % 5 == 0)
				{
					g_aMissile[nCntMissile].nPatternAnim = (g_aMissile[nCntMissile].nPatternAnim + 1) % 8; //8�܂ł����Ƃ��܂�0���������ă��Z�b�g

															   //UV�ݒ�
					pVertex[0].tex = D3DXVECTOR2((float)g_aMissile[nCntMissile].nPatternAnim * 0.5f, 1.0f);
					pVertex[1].tex = D3DXVECTOR2((float)g_aMissile[nCntMissile].nPatternAnim * 0.5f, 0.0f);
					pVertex[2].tex = D3DXVECTOR2(((float)g_aMissile[nCntMissile].nPatternAnim + 1) * 0.5f, 1.0f);
					pVertex[3].tex = D3DXVECTOR2(((float)g_aMissile[nCntMissile].nPatternAnim + 1) * 0.5f, 0.0f);

				}
				////UV�̐ݒ�
				//pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
				//pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
				//pVertex[2].tex = D3DXVECTOR2(1.0f / 2, 1.0f);
				//pVertex[3].tex = D3DXVECTOR2(1.0f / 2, 0.0f);

				//�G�A�v���C���[�ւ̃|�C���^�擾
				pEnemy = GetEnemy();
				pPlayer = GetPlayer();

				//�v���C���[���e�𔭎˂����ꍇ
				if (g_aMissile[nCntMissile].state == MISSILESTATE_OWNER_PLAYER)
				{
					for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
					{
						if (pEnemy->bUse == true)
						{
							if (pMissile->pos.x > pEnemy->pos.x - ENEMY_SIZE && pMissile->pos.x < pEnemy->pos.x + ENEMY_SIZE &&
								pMissile->pos.y > pEnemy->pos.y - ENEMY_SIZE && pMissile->pos.y < pEnemy->pos.y + ENEMY_SIZE)
							{
								pEnemy = GetEnemy(); //�|�C���^�ŏ��ɖ߂�
								for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++) //�͈͍U���̂��ߍČ���
								{
									if (pEnemy->bUse == true)
									{
										if (abs(pEnemy->pos.x - pMissile->pos.x) < abs(ENEMY_SIZE + MISSILE_EXPLOSIVE - 25) &&
											abs(pEnemy->pos.y - pMissile->pos.y) < abs(ENEMY_SIZE + MISSILE_EXPLOSIVE - 25))
										{
											//�����̐ݒ�
											SetExplosion(g_aMissile[nCntMissile].pos, MISSILE_EXPLOSIVE);

											//�������Đ�
											PlaySound(SOUND_LABEL_SE_EXPLOSION);

											//�q�b�g����
											HitEnemy(nCntEnemy, 5);

											//�e�̔j��
											pMissile->bUse = false;
										}
									}
								}
							}
						}
					}
				}

				//�G���e�𔭎˂����ꍇ
				if (g_aMissile[nCntMissile].state == MISSILESTATE_OWNER_ENEMY)
				{
					if (pPlayer->bUse == true)
					{
						if (pMissile->pos.x > pPlayer->pos.x - 40.0f && pMissile->pos.x < pPlayer->pos.x + 60.0f &&
							pMissile->pos.y > pPlayer->pos.y - 40.0f && pMissile->pos.y < pPlayer->pos.y + 60.0f)
						{
							//�q�b�g����
							HitPlayer(5);

							//�e�̔j��
							pMissile->bUse = false;
						}
					}
				}

				//�����`�F�b�N
				g_aMissile[nCntMissile].nLife--;
				if (g_aMissile[nCntMissile].nLife < 0)
				{
					pEnemy = GetEnemy(); //�|�C���^�ŏ��ɖ߂�
					for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++) //�͈͍U���̂��ߍČ���
					{
						if (pEnemy->bUse == true)
						{
							if (abs(pEnemy->pos.x - pMissile->pos.x) < abs(ENEMY_SIZE + MISSILE_EXPLOSIVE) &&
								abs(pEnemy->pos.y - pMissile->pos.y) < abs(ENEMY_SIZE + MISSILE_EXPLOSIVE))
							{
								//�����̐ݒ�
								SetExplosion(g_aMissile[nCntMissile].pos, MISSILE_EXPLOSIVE);

								//�������Đ�
								PlaySound(SOUND_LABEL_SE_EXPLOSION);

								//�q�b�g����
								HitEnemy(nCntEnemy, 5);

								//�e�̔j��
								pMissile->bUse = false;
							}
						}
					}
					SetExplosion(g_aMissile[nCntMissile].pos, MISSILE_EXPLOSIVE);
					g_aMissile[nCntMissile].bUse = false;
				}
				//��ʊO�`�F�b�N
			}
			pVertex += 4; //���_�̃|�C���^�����炷
		}

		//���_�o�b�t�@�̃A�����b�N
		g_pVtxBuffMissile->Unlock();
	}
}

//-----------------------------------------------------------------------------
//�~�T�C���̕`�揈��
//-----------------------------------------------------------------------------
void DrawMissile(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMissile, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMissile);

	for (int nCntMissile = 0; nCntMissile < MAX_MISSILE; nCntMissile++)
	{
		if (g_aMissile[nCntMissile].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntMissile * 4, 2);
		}
	}
}

//-----------------------------------------------------------------------------
//�~�T�C���̐ݒ�
//-----------------------------------------------------------------------------
void SetMissile(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, MISSILESTATE state)
{
	MISSILE *pMissile;
	VERTEX_2D *pVertex;

	pMissile = &g_aMissile[0];

	//���_�o�b�t�@�̃��b�N
	g_pVtxBuffMissile->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntMissile = 0; nCntMissile < MAX_MISSILE; nCntMissile++, pMissile++)
	{
		if (pMissile->bUse == false)
		{
			//�ʒu��ݒ�
			g_aMissile[nCntMissile].pos = pos;

			//���_���W��ݒ�
			pVertex[0].pos = D3DXVECTOR3(-MISSILE_WIDTH + g_aMissile[nCntMissile].pos.x, MISSILE_HEIGHT + g_aMissile[nCntMissile].pos.y, 0.0f);
			pVertex[1].pos = D3DXVECTOR3(-MISSILE_WIDTH + g_aMissile[nCntMissile].pos.x, -MISSILE_HEIGHT + g_aMissile[nCntMissile].pos.y, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(MISSILE_WIDTH + g_aMissile[nCntMissile].pos.x, MISSILE_HEIGHT + g_aMissile[nCntMissile].pos.y, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(MISSILE_WIDTH + g_aMissile[nCntMissile].pos.x, -MISSILE_HEIGHT + g_aMissile[nCntMissile].pos.y, 0.0f);

			//�ړ��ʂ�ݒ�
			g_aMissile[nCntMissile].move = move;

			//������ݒ�
			g_aMissile[nCntMissile].nLife = nLife;

			//�X�e�[�g��ݒ�
			g_aMissile[nCntMissile].state = state;

			pMissile->bUse = true;
			break;
		}
		//���_�̃|�C���^�����炷
		pVertex += 4;
	}
	g_pVtxBuffMissile->Unlock();
}