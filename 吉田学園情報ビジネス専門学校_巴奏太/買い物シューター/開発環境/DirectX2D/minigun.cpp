//-----------------------------------------------------------------------------
//
// �~�j�K���̏��� [minigun.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "minigun.h"

//�}�N��
#define MINIGUN_WIDTH (10 * 0.5f)
#define MINIGUN_HEIGHT (40 * 0.5f)

//-----------------------------------------------------------------------------
//�\���̐錾
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;  //�ʒu
	D3DXVECTOR3 move; //�ړ���
	int nLife;		  //����
	bool bUse;		  //�g�p���Ă��邩
	MINIGUNSTATE state; //�e�̏��
	MINIGUNTYPE type;  //�e�̎��
} MINIGUN;

//-----------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureMinigun = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMinigun = NULL;	//�o�b�t�@�ւ̃|�C���^
MINIGUN g_aMinigun[MAX_MINIGUN];						//�e�̏��
int g_nCounter;

//-----------------------------------------------------------------------------
//�~�j�K���̏���������
//-----------------------------------------------------------------------------
HRESULT InitMinigun(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\minigun.png", &g_pTextureMinigun);

	//BULLET�̏�����
	for (int nCntMinigun = 0; nCntMinigun < MAX_MINIGUN; nCntMinigun++)
	{
		g_aMinigun[nCntMinigun].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMinigun[nCntMinigun].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMinigun[nCntMinigun].nLife = 0;
		g_aMinigun[nCntMinigun].bUse = false;
		g_aMinigun[nCntMinigun].state = MINIGUNSTATE_OWNER_PLAYER;
	}

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * MAX_MINIGUN, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffMinigun, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�̃��b�N
	g_pVtxBuffMinigun->Lock(0, 0, (void**)&pVertex, 0);

	//���_���W
	for (int nCntMinigun = 0; nCntMinigun < MAX_MINIGUN; nCntMinigun++)
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

	g_pVtxBuffMinigun->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//�~�j�K���̏I������
//-----------------------------------------------------------------------------
void UninitMinigun(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffMinigun != NULL)
	{
		g_pVtxBuffMinigun->Release();
		g_pVtxBuffMinigun = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pTextureMinigun != NULL)
	{
		g_pTextureMinigun->Release();
		g_pTextureMinigun = NULL;
	}
}

//-----------------------------------------------------------------------------
//�~�j�K���̍X�V����
//-----------------------------------------------------------------------------
void UpdateMinigun(void)
{
	MINIGUN *pMinigun;
	ENEMY *pEnemy;
	PLAYER *pPlayer;
	VERTEX_2D *pVertex;

	pMinigun = &g_aMinigun[0];

	if (g_pVtxBuffMinigun != NULL)
	{
		//���_�o�b�t�@�̃��b�N
		g_pVtxBuffMinigun->Lock(0, 0, (void**)&pVertex, 0);

		for (int nCntMinigun = 0; nCntMinigun < MAX_MINIGUN; nCntMinigun++, pMinigun++)
		{
			if (pMinigun->bUse == true)
			{
				//�ʒu�X�V
				g_aMinigun[nCntMinigun].pos.x += g_aMinigun[nCntMinigun].move.x;
				g_aMinigun[nCntMinigun].pos.y += g_aMinigun[nCntMinigun].move.y;

				pEnemy = GetEnemy();
				pPlayer = GetPlayer();

				SetEffect(g_aMinigun[nCntMinigun].pos, D3DXCOLOR(0.2f, 0.02f, 0.4f, 0.6f), 10, 0.12f);
				SetEffect(g_aMinigun[nCntMinigun].pos, D3DXCOLOR(0.2f, 0.02f, 0.4f, 1.0f), 5, 0.12f);

				//�v���C���[���e�𔭎˂����ꍇ
				if (g_aMinigun[nCntMinigun].state == MINIGUNSTATE_OWNER_PLAYER)
				{
					for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
					{
						if (pEnemy->bUse == true)
						{
							if (pMinigun->pos.x > pEnemy->pos.x - 30.0f && pMinigun->pos.x < pEnemy->pos.x + 30.0f &&
								pMinigun->pos.y > pEnemy->pos.y - 30.0f && pMinigun->pos.y < pEnemy->pos.y + 30.0f)
							{
								//�q�b�g����
								HitEnemy(nCntEnemy, 2);

								//�e�̔j��
								pMinigun->bUse = false;
							}
						}
					}
				}

				if (g_aMinigun[nCntMinigun].state == MINIGUNSTATE_OWNER_ENEMY)
				{
					if (pPlayer->bUse == true)
					{
						if (pMinigun->pos.x > pPlayer->pos.x - 40.0f && pMinigun->pos.x < pPlayer->pos.x + 60.0f &&
							pMinigun->pos.y > pPlayer->pos.y - 40.0f && pMinigun->pos.y < pPlayer->pos.y + 60.0f)
						{
							//�q�b�g����
							HitPlayer(5);

							//�e�̔j��
							pMinigun->bUse = false;
						}
					}
				}

				//���_���W��ݒ�
				pVertex[0].pos = D3DXVECTOR3(-MINIGUN_WIDTH + g_aMinigun[nCntMinigun].pos.x, MINIGUN_HEIGHT + g_aMinigun[nCntMinigun].pos.y, 0.0f);
				pVertex[1].pos = D3DXVECTOR3(-MINIGUN_WIDTH + g_aMinigun[nCntMinigun].pos.x, -MINIGUN_HEIGHT + g_aMinigun[nCntMinigun].pos.y, 0.0f);
				pVertex[2].pos = D3DXVECTOR3(MINIGUN_WIDTH + g_aMinigun[nCntMinigun].pos.x, MINIGUN_HEIGHT + g_aMinigun[nCntMinigun].pos.y, 0.0f);
				pVertex[3].pos = D3DXVECTOR3(MINIGUN_WIDTH + g_aMinigun[nCntMinigun].pos.x, -MINIGUN_HEIGHT + g_aMinigun[nCntMinigun].pos.y, 0.0f);

				//��ʊO�`�F�b�N
				if (g_aMinigun[nCntMinigun].pos.x > SCREEN_WIDTH || g_aMinigun[nCntMinigun].pos.x < 0)
				{
					g_aMinigun[nCntMinigun].bUse = false;
				}
				if (g_aMinigun[nCntMinigun].pos.y > SCREEN_HEIGHT || g_aMinigun[nCntMinigun].pos.y < 0)
				{
					g_aMinigun[nCntMinigun].bUse = false;
				}
			}
			pVertex += 4; //���_�̃|�C���^�����炷
		}

		//���_�o�b�t�@�̃A�����b�N
		g_pVtxBuffMinigun->Unlock();
	}
}

//-----------------------------------------------------------------------------
//�~�j�K���̕`�揈��
//-----------------------------------------------------------------------------
void DrawMinigun(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMinigun, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMinigun);

	for (int nCntMinigun = 0; nCntMinigun < MAX_MINIGUN; nCntMinigun++)
	{
		if (g_aMinigun[nCntMinigun].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntMinigun * 4, 2);
		}
	}
}

//-----------------------------------------------------------------------------
//�~�j�K���̐ݒ�
//-----------------------------------------------------------------------------
void SetMinigun(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, MINIGUNSTATE state)
{
	MINIGUN *pMinigun;
	VERTEX_2D *pVertex;

	pMinigun = &g_aMinigun[0];

	//���_�o�b�t�@�̃��b�N
	g_pVtxBuffMinigun->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntMinigun = 0; nCntMinigun < MAX_MINIGUN; nCntMinigun++, pMinigun++)
	{
		if (pMinigun->bUse == false)
		{
			//�ʒu��ݒ�
			g_aMinigun[nCntMinigun].pos = pos;

			//���_���W��ݒ�
			pVertex[0].pos = D3DXVECTOR3(-10 + g_aMinigun[nCntMinigun].pos.x, 10 + g_aMinigun[nCntMinigun].pos.y, 0.0f);
			pVertex[1].pos = D3DXVECTOR3(-10 + g_aMinigun[nCntMinigun].pos.x, -10 + g_aMinigun[nCntMinigun].pos.y, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(10 + g_aMinigun[nCntMinigun].pos.x, 10 + g_aMinigun[nCntMinigun].pos.y, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(10 + g_aMinigun[nCntMinigun].pos.x, -10 + g_aMinigun[nCntMinigun].pos.y, 0.0f);

			//�ړ��ʂ�ݒ�
			g_aMinigun[nCntMinigun].move = move;

			//������ݒ�
			g_aMinigun[nCntMinigun].nLife = nLife;

			//�X�e�[�g��ݒ�
			g_aMinigun[nCntMinigun].state = state;

			//�e�̎�ނ�ݒ�
			g_nCounter++;
			if (g_nCounter % 2 == 0)
			{
				g_aMinigun[nCntMinigun].type = MINIGUNTYPE_LEFT;
				
				g_aMinigun[nCntMinigun].pos.x += 15;
			}
			else
			{
				g_aMinigun[nCntMinigun].type = MINIGUNTYPE_RIGHT;
				
				g_aMinigun[nCntMinigun].pos.x -= 15;
			}

			pMinigun->bUse = true;
			break;
		}
		//���_�̃|�C���^�����炷
		pVertex += 4;
	}
	g_pVtxBuffMinigun->Unlock();
}