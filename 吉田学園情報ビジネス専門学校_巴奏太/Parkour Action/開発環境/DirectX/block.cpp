#define _CRT_SECURE_NO_WARNINGS
//-----------------------------------------------------------------------------
//
// �u���b�N�̏��� [block.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "block.h"
#include "player.h"
#include <cmath>
#include <stdio.h>

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
#define BLOCK_TEXTURE (6)

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffBlock = NULL;							// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureBlock[BLOCK_TEXTURE] = { NULL };									// �u���b�N�̃e�N�X�`���ւ̃|�C���^
LPD3DXFONT g_pFontBlock = NULL;												// �t�H���g�ւ̃|�C���^
BLOCK g_aBlock[BLOCK_MAX];													// �u���b�N�̏��

//-----------------------------------------------------------------------------
//�@�v���g�^�C�v�錾
//-----------------------------------------------------------------------------
void MoveBlock();
D3DXVECTOR3 g_startPos1;
D3DXVECTOR3 g_finishPos2;

//-----------------------------------------------------------------------------
// �u���b�N�̏���������
//-----------------------------------------------------------------------------
HRESULT InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	for (int nCntBlock = 0; nCntBlock < BLOCK_MAX; nCntBlock++)
	{
		g_aBlock[nCntBlock].bUse = false;
		g_aBlock[nCntBlock].bReturn = false;
		g_aBlock[nCntBlock].fHeight = 0;
		g_aBlock[nCntBlock].fWidth = 0;
		g_aBlock[nCntBlock].scroll = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].nTypeNum = 0;
		g_aBlock[nCntBlock].fMoveTime = 0;
		g_aBlock[nCntBlock].fMoveTimeFirst = 0;
		g_aBlock[nCntBlock].fMoveSpeed = 0;
		g_aBlock[nCntBlock].texture = BLOCKTEXTURE_NORMAL;
	}

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�t�H���g�̐���
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFontBlock);

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\block_normal.png", &g_pTextureBlock[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\block_top.png", &g_pTextureBlock[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\block_topright.png", &g_pTextureBlock[2]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\block_topleft.png", &g_pTextureBlock[3]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\block_right.png", &g_pTextureBlock[4]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\block_left.png", &g_pTextureBlock[5]);


	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * BLOCK_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBlock, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntBlock = 0; nCntBlock < BLOCK_MAX; nCntBlock++)
	{
		//���_���W D3DXVECTOR3(X,Y, 0.0f);
		pVertex[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight, 0.0f);
		pVertex[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y, 0.0f);
		pVertex[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth, g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight, 0.0f);
		pVertex[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth, g_aBlock[nCntBlock].pos.y, 0.0f);

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

		pVertex += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBlock->Unlock();

	//�u���b�N�z�u
	for (int nCnt = 0; nCnt < MAP_TILE_COLUMN; nCnt++) SetBlock(D3DXVECTOR3(nCnt * 50, 0.0f, 0.0f), 50, 50, 0, BLOCKTEXTURE_NORMAL, NULL, NULL);
	for (int nCnt = 0; nCnt < MAP_TILE_ROW; nCnt++) SetBlock(D3DXVECTOR3(-50.0f, nCnt * 50, 0.0f), 50, 50, 0, BLOCKTEXTURE_NORMAL, NULL, NULL);
	return S_OK;
}

//-----------------------------------------------------------------------------
// �u���b�N�̏I������
//-----------------------------------------------------------------------------
void UninitBlock(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}

	//�e�N�X�`���̊J��
	for (int nCntTEX = 0; nCntTEX < (int)BLOCKTEXTURE_MAX; nCntTEX++)
	{
		if (g_pTextureBlock[nCntTEX] != NULL)
		{
			g_pTextureBlock[nCntTEX]->Release();
			g_pTextureBlock[nCntTEX] = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
// �u���b�N�̍X�V����
//-----------------------------------------------------------------------------
void UpdateBlock(void)
{
	VERTEX_2D *pVertex;
	BLOCK *pBlock;
	PLAYER *pPlayer;
	pPlayer = GetPlayer();
	pBlock = &g_aBlock[0];

	//���_�o�b�t�@�̃��b�N
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntBLK = 0; nCntBLK < BLOCK_MAX; nCntBLK++, pBlock++)
	{
		if (pBlock->bUse == true)
		{
			//�ʒu�X�V
			pBlock->pos.x += pBlock->move.x;
			pBlock->pos.y += pBlock->move.y;

			//�X�N���[����
			pBlock->scroll.x = -pPlayer->move.x;

			////�u���b�N�̃X�N���[��
			//if (pPlayer->scrollState == PLAYERSCROLL_CENTER)
			//{
			//	pBlock->pos.x += pBlock->scroll.x;
			//	pBlock->pos.y += pBlock->scroll.y;
			//}

			//�u���b�N�̈ړ�
			MoveBlock();

			//���_���W D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(g_aBlock[nCntBLK].pos.x, g_aBlock[nCntBLK].pos.y + g_aBlock[nCntBLK].fHeight, 0.0f);
			pVertex[1].pos = D3DXVECTOR3(g_aBlock[nCntBLK].pos.x, g_aBlock[nCntBLK].pos.y, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_aBlock[nCntBLK].pos.x + g_aBlock[nCntBLK].fWidth, g_aBlock[nCntBLK].pos.y + g_aBlock[nCntBLK].fHeight, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_aBlock[nCntBLK].pos.x + g_aBlock[nCntBLK].fWidth, g_aBlock[nCntBLK].pos.y, 0.0f);

			//UV�̐ݒ�
			pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f);
		}
		pVertex += 4;
	}

	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffBlock->Unlock();

}

//-----------------------------------------------------------------------------
// �u���b�N�̕`�揈��
//-----------------------------------------------------------------------------
void DrawBlock(void)
{

	LPDIRECT3DDEVICE9 pDevice;
	BLOCK *pBlock;
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT - 50 };
	char aStr[256];

	pBlock = &g_aBlock[0];

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBlock = 0; nCntBlock < BLOCK_MAX; nCntBlock++, pBlock++)
	{
		//�e�N�X�`���̐ݒ�
		switch (pBlock->texture)
		{
		case BLOCKTEXTURE_NORMAL:
			pDevice->SetTexture(0, g_pTextureBlock[0]);
			break;

		case BLOCKTEXTURE_TOP:
			pDevice->SetTexture(0, g_pTextureBlock[1]);
			break;

		case BLOCKTEXTURE_TOPRIGHT:
			pDevice->SetTexture(0, g_pTextureBlock[2]);
			break;

		case BLOCKTEXTURE_TOPLEFT:
			pDevice->SetTexture(0, g_pTextureBlock[3]);
			break;

		case BLOCKTEXTURE_RIGHT:
			pDevice->SetTexture(0, g_pTextureBlock[4]);
			break;

		case BLOCKTEXTURE_LEFT:
			pDevice->SetTexture(0, g_pTextureBlock[5]);
			break;
		}

		if (g_aBlock[nCntBlock].bUse == true)
		{
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBlock * 4, 2);
		}
	}

#ifdef _DEBUG

	//for (int nCntBlock = 0; nCntBlock < BLOCK_MAX; nCntBlock++, pBlock++)
	//{
		//if (pBlock->nTypeNum == 1)
		//{
		//	sprintf(&aStr[0], "fMoveX: %f \n", pBlock->fMoveTime);
		//}
	//}


	////�e�L�X�g�̕`��
	//g_pFontBlock->DrawText(NULL, &aStr[0], -1, &rect, DT_BOTTOM, D3DCOLOR_RGBA(255, 255, 255, 255));

#endif
}

//-----------------------------------------------------------------------------
// �u���b�N�̐ݒu
//-----------------------------------------------------------------------------
void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight, int nTypeNum, BLOCKTEXTURE texture, float fMoveTime, float fMoveSpeed)
{
	VERTEX_2D *pVertex;
	BLOCK *pBlock;

	pBlock = &g_aBlock[0];

	//���_�o�b�t�@�̃��b�N
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntBlock = 0; nCntBlock < BLOCK_MAX; nCntBlock++, pBlock++)
	{
		if (pBlock->bUse == false)
		{
			//�ʒu��ݒ�
			g_aBlock[nCntBlock].pos = pos;

			//�����ʒu��ݒ�
			g_aBlock[nCntBlock].firstpos = pos;

			//����ݒ�
			g_aBlock[nCntBlock].fWidth = fWidth;

			//������ݒ�
			g_aBlock[nCntBlock].fHeight = fHeight;

			//�ԍ�����U��
			g_aBlock[nCntBlock].nTypeNum = nTypeNum;

			//�e�N�X�`���ݒ�
			g_aBlock[nCntBlock].texture = texture;

			//�ړ����Ԑݒ�
			g_aBlock[nCntBlock].fMoveTime = fMoveTime;
			g_aBlock[nCntBlock].fMoveTimeFirst = fMoveTime;

			//�ړ����x�ݒ�
			g_aBlock[nCntBlock].fMoveSpeed = fMoveSpeed;

			//���_���W��ݒ�
			pVertex[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight, 0.0f);
			pVertex[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth, g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth, g_aBlock[nCntBlock].pos.y, 0.0f);

			pBlock->bUse = true;
			break;
		}
		pVertex += 4; // ���_���炷
	}
	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffBlock->Unlock();
}

//-----------------------------------------------------------------------------
// �����蔻�菈��
//-----------------------------------------------------------------------------
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{
	BLOCK *pBlock;
	bool bJump = true;
	pBlock = &g_aBlock[0];

	for (int nCntBLK = 0; nCntBLK < BLOCK_MAX; nCntBLK++, pBlock++)
	{
		if (pBlock->bUse == true)
		{
			if ((pPos->x + fWidth > pBlock->pos.x && pPos->x - fWidth < pBlock->pos.x + pBlock->fWidth) &&
				(pPos->y > pBlock->pos.y))
			{
				if (pPosOld->y <= pBlock->pos.y + 5&&
					pPosOld->x + fWidth >= pBlock->pos.x && pPosOld->x - fWidth <= pBlock->pos.x + pBlock->fWidth)
				{
					pPos->y = pBlock->pos.y;
					pMove->y = 0.0f + pBlock->move.y;
					bJump = false;
				}
			}
			if ((pPos->x + fWidth > pBlock->pos.x && pPos->x - fWidth < pBlock->pos.x + pBlock->fWidth) &&
				(pPos->y > pBlock->pos.y + pBlock->fHeight && pPos->y - fHeight < pBlock->pos.y + pBlock->fHeight))
			{
				if (pPosOld->y - fHeight >= pBlock->pos.y + pBlock->fHeight &&
					pPosOld->x + fWidth >= pBlock->pos.x && pPosOld->x - fWidth <= pBlock->pos.x + pBlock->fWidth)
				{
					pPos->y = pBlock->pos.y + pBlock->fHeight + fHeight;
					pMove->y = 0.0f + pBlock->move.y;
				}
			}
			if ((pPos->y > pBlock->pos.y && pPos->y - fHeight < pBlock->pos.y + pBlock->fHeight) &&
				(pPos->x + fWidth > pBlock->pos.x && pPos->x - fWidth < pBlock->pos.x))
			{
				if (pPosOld->x <= pBlock->pos.x &&
					pPosOld->y >= pBlock->pos.y && pPosOld->y - fHeight <= pBlock->pos.y + pBlock->fHeight)
				{
					pPos->x = pBlock->pos.x - fWidth;
					pMove->x = 0.0f;
				}
			}
			if ((pPos->y > pBlock->pos.y && pPos->y - fHeight < pBlock->pos.y + pBlock->fHeight) &&
				(pPos->x + fWidth > pBlock->pos.x + pBlock->fWidth && pPos->x - fWidth < pBlock->pos.x + pBlock->fWidth))
			{
				if (pPosOld->x + fWidth >= pBlock->pos.x + pBlock->fWidth &&
					pPosOld->y > pBlock->pos.y && pPosOld->y - fHeight < pBlock->pos.y + pBlock->fHeight)
				{
					pPos->x = pBlock->pos.x + (pBlock->fWidth + fWidth);
					pMove->x = 0.0f;
				}
			}
		}
	}
	
	return bJump;
}

//-----------------------------------------------------------------------------
// �u���b�N�̈ړ�
//-----------------------------------------------------------------------------
void MoveBlock()
{
	BLOCK *pBlock;
	pBlock = &g_aBlock[0];

	for (int nCntBLK = 0; nCntBLK < BLOCK_MAX; nCntBLK++, pBlock++)
	{
		if (pBlock->bUse == true)
		{
			switch (pBlock->nTypeNum)
			{
			case 1:
				if (pBlock->bReturn == false)
				{
					if (pBlock->fMoveTime >= 0)
					{
						pBlock->fMoveTime -= 0.020f;
						pBlock->move.x = pBlock->fMoveSpeed;
					}
					else if (pBlock->fMoveTime <= 0)
					{
						pBlock->bReturn = true;
					}
				}
				else if (pBlock->bReturn == true)
				{
					pBlock->fMoveTime += 0.020f;
					pBlock->move.x = -pBlock->fMoveSpeed;
						if (pBlock->fMoveTime >= pBlock->fMoveTimeFirst)
					{
						pBlock->bReturn = false;
					}
				}
				break;

			case 2:
				if (pBlock->bReturn == false)
				{
					if (pBlock->fMoveTime >= 0)
					{
						pBlock->fMoveTime -= 0.020f;
						pBlock->move.y = -pBlock->fMoveSpeed;
					}
					else if (pBlock->fMoveTime <= 0)
					{
						pBlock->bReturn = true;
					}
				}
				else if (pBlock->bReturn == true)
				{
					pBlock->fMoveTime += 0.020f;
					pBlock->move.y = pBlock->fMoveSpeed;
					if (pBlock->fMoveTime >= pBlock->fMoveTimeFirst)
					{
						pBlock->bReturn = false;
					}
				}
				break;

			default:
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// �u���b�N���̎擾
//-----------------------------------------------------------------------------
BLOCK *GetBlock(void)
{
	return &g_aBlock[0];
}