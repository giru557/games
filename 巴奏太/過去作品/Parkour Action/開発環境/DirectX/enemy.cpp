//-----------------------------------------------------------------------------
//
// �G�̏��� [enemy.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "enemy.h"
#include "fade.h"
#include "player.h"
#include "timescore.h"
#include "block.h"
#include "sound.h"

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffEnemy = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureEnemy = NULL;				// �|���S���̃e�N�X�`���ւ̃|�C���^
ENEMY g_aEnemy[ENEMY_POLYGON];										// ENEMY�\���̂̏��

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
void EnemyMove(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight);

//-----------------------------------------------------------------------------
// �G�̏���������
//-----------------------------------------------------------------------------
HRESULT InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	//Enemy������
	for (int nCntENEM = 0; nCntENEM < ENEMY_POLYGON; nCntENEM++)
	{
		g_aEnemy[nCntENEM].bUse = false;
		g_aEnemy[nCntENEM].bJump = false;
		g_aEnemy[nCntENEM].fHeight = ENEMY_HEIGHT;
		g_aEnemy[nCntENEM].fWidth = ENEMY_WIDTH;
		g_aEnemy[nCntENEM].pos = D3DXVECTOR3(0, 0, 0.0f);
		g_aEnemy[nCntENEM].posold = D3DXVECTOR3(0, 0, 0);
		g_aEnemy[nCntENEM].scroll = D3DXVECTOR3(0, 0, 0.0f);
		g_aEnemy[nCntENEM].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntENEM].nPatternH = 0;
		g_aEnemy[nCntENEM].nPatternV = 0;
		g_aEnemy[nCntENEM].nMaxPatternH = ENEMY_TEX_SPLIT_H;
		g_aEnemy[nCntENEM].nMaxPatternV = ENEMY_TEX_SPLIT_V;
	}

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\golem1_sprite.png", &g_pTextureEnemy);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * ENEMY_POLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffEnemy, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntENEM = 0; nCntENEM < ENEMY_POLYGON; nCntENEM++)
	{
		//���_���W D3DXVECTOR3(X,Y, 0.0f);
		pVertex[0].pos = D3DXVECTOR3(g_aEnemy[nCntENEM].pos.x - ENEMY_WIDTH, g_aEnemy[nCntENEM].pos.y, 0.0f);
		pVertex[1].pos = D3DXVECTOR3(g_aEnemy[nCntENEM].pos.x - ENEMY_WIDTH, g_aEnemy[nCntENEM].pos.y - ENEMY_HEIGHT, 0.0f);
		pVertex[2].pos = D3DXVECTOR3(g_aEnemy[nCntENEM].pos.x + ENEMY_WIDTH, g_aEnemy[nCntENEM].pos.y, 0.0f);
		pVertex[3].pos = D3DXVECTOR3(g_aEnemy[nCntENEM].pos.x + ENEMY_WIDTH, g_aEnemy[nCntENEM].pos.y - ENEMY_HEIGHT, 0.0f);

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

		//���_���̐ݒ�
		pVertex[0].tex = D3DXVECTOR2((1.0f / (float)g_aEnemy[nCntENEM].nMaxPatternH) * (g_aEnemy[nCntENEM].nPatternH), (1.0f / g_aEnemy[nCntENEM].nMaxPatternV) * (g_aEnemy[nCntENEM].nPatternV + 1.0f));
		pVertex[1].tex = D3DXVECTOR2((1.0f / (float)g_aEnemy[nCntENEM].nMaxPatternH) * (g_aEnemy[nCntENEM].nPatternH), (1.0f / g_aEnemy[nCntENEM].nMaxPatternV) * (g_aEnemy[nCntENEM].nPatternV));
		pVertex[2].tex = D3DXVECTOR2((1.0f / (float)g_aEnemy[nCntENEM].nMaxPatternH) * (g_aEnemy[nCntENEM].nPatternH + 1.0f), (1.0f / g_aEnemy[nCntENEM].nMaxPatternV) * (g_aEnemy[nCntENEM].nPatternV + 1.0f));
		pVertex[3].tex = D3DXVECTOR2((1.0f / (float)g_aEnemy[nCntENEM].nMaxPatternH) * (g_aEnemy[nCntENEM].nPatternH + 1.0f), (1.0f / g_aEnemy[nCntENEM].nMaxPatternV) * (g_aEnemy[nCntENEM].nPatternV));

		pVertex += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
// �G�̏I������
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
	if (g_pTextureEnemy != NULL)
	{
		g_pTextureEnemy->Release();
		g_pTextureEnemy = NULL;
	}

}

//-----------------------------------------------------------------------------
// �G�̍X�V����
//-----------------------------------------------------------------------------
void UpdateEnemy(void)
{
	VERTEX_2D *pVertex;
	PLAYER *pPlayer;
	ENEMY *pEnemy;

	pPlayer = GetPlayer();
	pEnemy = &g_aEnemy[0];

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntENEM = 0; nCntENEM < ENEMY_POLYGON; nCntENEM++, pEnemy++)
	{

		if (pEnemy->bUse == true)
		{
			//�A�j���[�V�����J�E���g�A�b�v
			pEnemy->nCounterAnim++;

			//�O��ʒu�ۑ�
			pEnemy->posold = pEnemy->pos;

			//�G�̈ړ�
			EnemyMove(&g_aEnemy[nCntENEM].pos, &g_aEnemy[nCntENEM].posold, &g_aEnemy[nCntENEM].move, g_aEnemy[nCntENEM].fWidth, g_aEnemy[nCntENEM].fHeight);

			//�d��
			pEnemy->move.y += PLAYER_GRAVITY;

			//�X�N���[����
			pEnemy->scroll.x = -pPlayer->move.x;

			//�ړ��l�ǉ�
			pEnemy->pos.x += pEnemy->move.x;
			pEnemy->pos.y += pEnemy->move.y;

			////�G�̃X�N���[��
			//if (pPlayer->scrollState == PLAYERSCROLL_CENTER)
			//{
			//	pEnemy->pos.x += pEnemy->scroll.x;
			//	pEnemy->pos.y += pEnemy->scroll.y;
			//}
		
			//�e�N�X�`���̃p�^�[�����A�j���[�V����������
			if (pEnemy->nCounterAnim % PLAYER_TEX_ANIMINTERVAL == 0)
			{
				pEnemy->nPatternH = (pEnemy->nPatternH + 1) % ENEMY_TEX_SPLIT_H;
				pEnemy->nCounterAnim = 0;
			}

			//�u���b�N�Ƃ̓����蔻��
			pEnemy->bJump = CollisionBlock(&g_aEnemy[nCntENEM].pos, &g_aEnemy[nCntENEM].posold, &g_aEnemy[nCntENEM].move, ENEMY_WIDTH, ENEMY_HEIGHT);

			//���_���W D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(pEnemy->pos.x - ENEMY_WIDTH, pEnemy->pos.y, 0.0f);
			pVertex[1].pos = D3DXVECTOR3(pEnemy->pos.x - ENEMY_WIDTH, pEnemy->pos.y - ENEMY_HEIGHT, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(pEnemy->pos.x + ENEMY_WIDTH, pEnemy->pos.y, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(pEnemy->pos.x + ENEMY_WIDTH, pEnemy->pos.y - ENEMY_HEIGHT, 0.0f);

			//���_���̐ݒ�
			if (pEnemy->move.x > 0)
			{
				pVertex[0].tex = D3DXVECTOR2((1.0f / (float)g_aEnemy[nCntENEM].nMaxPatternH) * (g_aEnemy[nCntENEM].nPatternH), (1.0f / g_aEnemy[nCntENEM].nMaxPatternV) * (g_aEnemy[nCntENEM].nPatternV + 1.0f));
				pVertex[1].tex = D3DXVECTOR2((1.0f / (float)g_aEnemy[nCntENEM].nMaxPatternH) * (g_aEnemy[nCntENEM].nPatternH), (1.0f / g_aEnemy[nCntENEM].nMaxPatternV) * (g_aEnemy[nCntENEM].nPatternV));
				pVertex[2].tex = D3DXVECTOR2((1.0f / (float)g_aEnemy[nCntENEM].nMaxPatternH) * (g_aEnemy[nCntENEM].nPatternH + 1.0f), (1.0f / g_aEnemy[nCntENEM].nMaxPatternV) * (g_aEnemy[nCntENEM].nPatternV + 1.0f));
				pVertex[3].tex = D3DXVECTOR2((1.0f / (float)g_aEnemy[nCntENEM].nMaxPatternH) * (g_aEnemy[nCntENEM].nPatternH + 1.0f), (1.0f / g_aEnemy[nCntENEM].nMaxPatternV) * (g_aEnemy[nCntENEM].nPatternV));
			}
			else if (pEnemy->move.x < 0)
			{
				pVertex[0].tex = D3DXVECTOR2((1.0f / (float)g_aEnemy[nCntENEM].nMaxPatternH) * (g_aEnemy[nCntENEM].nPatternH + 1.0f), (1.0f / g_aEnemy[nCntENEM].nMaxPatternV) * (g_aEnemy[nCntENEM].nPatternV + 1.0f));
				pVertex[1].tex = D3DXVECTOR2((1.0f / (float)g_aEnemy[nCntENEM].nMaxPatternH) * (g_aEnemy[nCntENEM].nPatternH + 1.0f), (1.0f / g_aEnemy[nCntENEM].nMaxPatternV) * (g_aEnemy[nCntENEM].nPatternV));
				pVertex[2].tex = D3DXVECTOR2((1.0f / (float)g_aEnemy[nCntENEM].nMaxPatternH) * (g_aEnemy[nCntENEM].nPatternH), (1.0f / g_aEnemy[nCntENEM].nMaxPatternV) * (g_aEnemy[nCntENEM].nPatternV + 1.0f));
				pVertex[3].tex = D3DXVECTOR2((1.0f / (float)g_aEnemy[nCntENEM].nMaxPatternH) * (g_aEnemy[nCntENEM].nPatternH), (1.0f / g_aEnemy[nCntENEM].nMaxPatternV) * (g_aEnemy[nCntENEM].nPatternV));

			}

		}
		pVertex += 4;
	}
			//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffEnemy->Unlock();
}

//-----------------------------------------------------------------------------
// �G�̕`�揈��
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

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureEnemy);

	for (int nCntENEM = 0; nCntENEM < ENEMY_POLYGON; nCntENEM++)
	{
		if (g_aEnemy[nCntENEM].bUse == true)
		{
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntENEM * 4, 2);
		}
	}
}

//-----------------------------------------------------------------------------
// �G�Ƃ̓����蔻��
//-----------------------------------------------------------------------------
void CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	for (int nCntENEM = 0; nCntENEM < ENEMY_POLYGON; nCntENEM++)
	{
		if (g_aEnemy[nCntENEM].bUse == true)
		{
			if ((pPos->x + fWidth > g_aEnemy[nCntENEM].pos.x - g_aEnemy[nCntENEM].fWidth && pPos->x - fWidth < g_aEnemy[nCntENEM].pos.x + g_aEnemy[nCntENEM].fWidth) &&
				(pPos->y > g_aEnemy[nCntENEM].pos.y - g_aEnemy[nCntENEM].fHeight && pPos->y - fHeight < -g_aEnemy[nCntENEM].fHeight))
			{
				if (pPosOld->y <= g_aEnemy[nCntENEM].pos.y - g_aEnemy[nCntENEM].fHeight&&
					pPosOld->x + fWidth >= g_aEnemy[nCntENEM].pos.x - g_aEnemy[nCntENEM].fWidth && pPosOld->x - fWidth <= g_aEnemy[nCntENEM].pos.x + g_aEnemy[nCntENEM].fWidth)
				{
					if (pPlayer->damage == DAMAGESTATE_NORMAL)
					{
						if (pPlayer->state != PLAYERSTATE_SLIDE)
						{
							pPlayer->damage = DAMAGESTATE_HURT;
						}
						else
						{
							PlaySound(SOUND_LABEL_SE_DIE_ENEMY);
							g_aEnemy[nCntENEM].bUse = false;
						}

						if (pPlayer->pos.x > g_aEnemy[nCntENEM].pos.x)
						{
							pPlayer->move.y -= 5;
							pPlayer->move.x += 10;
						}
						else if (pPlayer->pos.x < g_aEnemy[nCntENEM].pos.x)
						{
							pPlayer->move.y -= 5;
							pPlayer->move.x -= 10;
						}
					}
				}
			}
			if ((pPos->x + fWidth > g_aEnemy[nCntENEM].pos.x - g_aEnemy[nCntENEM].fWidth && pPos->x - fWidth < g_aEnemy[nCntENEM].pos.x + g_aEnemy[nCntENEM].fWidth) &&
				(pPos->y > g_aEnemy[nCntENEM].pos.y && pPos->y - fHeight < g_aEnemy[nCntENEM].pos.y))
			{
				if (pPosOld->y - fHeight >= g_aEnemy[nCntENEM].pos.y &&
					pPosOld->x + fWidth >= g_aEnemy[nCntENEM].pos.x - g_aEnemy[nCntENEM].fWidth && pPosOld->x - fWidth <= g_aEnemy[nCntENEM].pos.x + g_aEnemy[nCntENEM].fWidth)
				{
					if (pPlayer->damage == DAMAGESTATE_NORMAL)
					{
						if (pPlayer->state != PLAYERSTATE_SLIDE)
						{
							pPlayer->damage = DAMAGESTATE_HURT;
						}
						else
						{
							PlaySound(SOUND_LABEL_SE_DIE_ENEMY);
							g_aEnemy[nCntENEM].bUse = false;
						}

						if (pPlayer->pos.x > g_aEnemy[nCntENEM].pos.x)
						{
							pPlayer->move.y -= 5;
							pPlayer->move.x += 10;
						}
						else if (pPlayer->pos.x < g_aEnemy[nCntENEM].pos.x)
						{
							pPlayer->move.y -= 5;
							pPlayer->move.x -= 10;
						}
					}
				}
			}
			if ((pPos->y > g_aEnemy[nCntENEM].pos.y - g_aEnemy[nCntENEM].fHeight && pPos->y - fHeight < g_aEnemy[nCntENEM].pos.y) &&
				(pPos->x + fWidth > g_aEnemy[nCntENEM].pos.x - g_aEnemy[nCntENEM].fWidth && pPos->x - fWidth < g_aEnemy[nCntENEM].pos.x - g_aEnemy[nCntENEM].fWidth))
			{
				if (pPosOld->x <= g_aEnemy[nCntENEM].pos.x - g_aEnemy[nCntENEM].fWidth &&
					pPosOld->y >= g_aEnemy[nCntENEM].pos.y - g_aEnemy[nCntENEM].fHeight && pPosOld->y - fHeight <= g_aEnemy[nCntENEM].pos.y)
				{
					if (pPlayer->damage == DAMAGESTATE_NORMAL)
					{
						if (pPlayer->state != PLAYERSTATE_SLIDE)
						{
							pPlayer->damage = DAMAGESTATE_HURT;
						}
						else
						{
							PlaySound(SOUND_LABEL_SE_DIE_ENEMY);
							g_aEnemy[nCntENEM].bUse = false;
						}

						if (pPlayer->pos.x > g_aEnemy[nCntENEM].pos.x)
						{
							pPlayer->move.y -= 5;
							pPlayer->move.x += 10;
						}
						else if (pPlayer->pos.x < g_aEnemy[nCntENEM].pos.x)
						{
							pPlayer->move.y -= 5;
							pPlayer->move.x -= 10;
						}
					}
				}
			}
			if ((pPos->y > g_aEnemy[nCntENEM].pos.y - g_aEnemy[nCntENEM].fHeight && pPos->y - fHeight < g_aEnemy[nCntENEM].pos.y) &&
				(pPos->x + fWidth > g_aEnemy[nCntENEM].pos.x + g_aEnemy[nCntENEM].fWidth && pPos->x - fWidth < g_aEnemy[nCntENEM].pos.x + g_aEnemy[nCntENEM].fWidth))
			{
				if (pPosOld->x + fWidth >= g_aEnemy[nCntENEM].pos.x + g_aEnemy[nCntENEM].fWidth &&
					pPosOld->y > g_aEnemy[nCntENEM].pos.y - g_aEnemy[nCntENEM].fHeight && pPosOld->y - fHeight < g_aEnemy[nCntENEM].pos.y)
				{
					if (pPlayer->damage == DAMAGESTATE_NORMAL)
					{
						if (pPlayer->state != PLAYERSTATE_SLIDE)
						{
							pPlayer->damage = DAMAGESTATE_HURT;
						}
						else
						{
							PlaySound(SOUND_LABEL_SE_DIE_ENEMY);
							g_aEnemy[nCntENEM].bUse = false;
						}

						if (pPlayer->pos.x > g_aEnemy[nCntENEM].pos.x)
						{
							pPlayer->move.y -= 5;
							pPlayer->move.x += 10;
						}
						else if (pPlayer->pos.x < g_aEnemy[nCntENEM].pos.x)
						{
							pPlayer->move.y -= 5;
							pPlayer->move.x -= 10;
						}
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
// �G�̏����擾
//-----------------------------------------------------------------------------
ENEMY *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//-----------------------------------------------------------------------------
// �G�̔z�u
//-----------------------------------------------------------------------------
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	VERTEX_2D *pVertex;
	ENEMY *pEnemy;
	pEnemy = &g_aEnemy[0];

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVertex, 0);
	
	for (int nCntENEM = 0; nCntENEM < ENEMY_POLYGON; nCntENEM++, pEnemy++)
	{
		if (pEnemy->bUse == false)
		{
			//�G�̈ʒu�ݒ�
			pEnemy->pos = pos;

			//�G�̈ړ��l�ݒ�
			pEnemy->move = move;

			//���_���W D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(g_aEnemy[nCntENEM].pos.x - ENEMY_WIDTH, g_aEnemy[nCntENEM].pos.y, 0.0f);
			pVertex[1].pos = D3DXVECTOR3(g_aEnemy[nCntENEM].pos.x - ENEMY_WIDTH, g_aEnemy[nCntENEM].pos.y - ENEMY_HEIGHT, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_aEnemy[nCntENEM].pos.x + ENEMY_WIDTH, g_aEnemy[nCntENEM].pos.y, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_aEnemy[nCntENEM].pos.x + ENEMY_WIDTH, g_aEnemy[nCntENEM].pos.y - ENEMY_HEIGHT, 0.0f);

			pEnemy->bUse = true;
			break;
		}
		pVertex += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();
}

//-----------------------------------------------------------------------------
// �G�̈ړ�
//-----------------------------------------------------------------------------
void EnemyMove(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{
	ENEMY *pEnemy;
	BLOCK *pBlock;
	pBlock = GetBlock();
	pEnemy = GetEnemy();

	//���]����
		for (int nCntBLK = 0; nCntBLK < BLOCK_MAX; nCntBLK++, pBlock++)
		{
			if (pBlock->bUse == true)
			{
				if ((pPos->y > pBlock->pos.y && pPos->y - pEnemy->fHeight < pPos->y + pBlock->fHeight) &&
					(pPos->x + fWidth >= pBlock->pos.x && pPos->x - fWidth <= pBlock->pos.x))
				{
					if (pPosOld->x <= pBlock->pos.x &&
						pPosOld->y >= pBlock->pos.y && pPosOld->y - fHeight <= pBlock->pos.y + pBlock->fHeight)
					{
						pMove->x = -2.0f;
					}
				}
				if ((pPos->y > pBlock->pos.y && pPos->y - fHeight < pBlock->pos.y + pBlock->fHeight) &&
					(pPos->x + fWidth >= pBlock->pos.x + pBlock->fWidth && pPos->x - fWidth <= pBlock->pos.x + pBlock->fWidth))
				{
					if (pPosOld->x + fWidth >= pBlock->pos.x + pBlock->fWidth &&
						pPosOld->y > pBlock->pos.y && pPosOld->y - fHeight < pBlock->pos.y + pBlock->fHeight)
					{
						pMove->x = 2.0f;
					}
				}
			}
		}
}