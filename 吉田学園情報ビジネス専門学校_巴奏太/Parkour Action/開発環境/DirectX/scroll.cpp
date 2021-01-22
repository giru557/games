//-----------------------------------------------------------------------------
//
// �Q�[���̏��� [Scroll.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "scroll.h"
#include "player.h"
#include "enemy.h"
#include "block.h"
#include "goalflag.h"

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
D3DXVECTOR3 g_scrollAll;	//���ׂẴX�N���[�����܂Ƃ߂�
D3DXVECTOR3 g_scrollMove;	//�X�N���[���̈ړ��l

//-----------------------------------------------------------------------------
// �X�N���[���̏���������
//-----------------------------------------------------------------------------
HRESULT InitScroll(void)
{

	return S_OK;
}

//-----------------------------------------------------------------------------
// �X�N���[���̏I������
//-----------------------------------------------------------------------------
void UninitScroll(void)
{
}

//-----------------------------------------------------------------------------
// �X�N���[���̍X�V����
//-----------------------------------------------------------------------------
void UpdateScroll(void)
{
	PLAYER *pPlayer;
	ENEMY *pEnemy;
	BLOCK *pBlock;
	GOAL *pGoal;
	pGoal = GetGoal();
	pBlock = GetBlock();
	pEnemy = GetEnemy();
	pPlayer = GetPlayer();

	//�X�N���[���ړ��l���
	g_scrollAll.x = -pPlayer->move.x + g_scrollMove.x;
	g_scrollMove.x = 0;

	//�X�N���[��
	if (pPlayer->scrollState == PLAYERSCROLL_CENTER)
	{
		for (int nCnt = 0; nCnt < ENEMY_POLYGON; nCnt++, pEnemy++)
		{
			if (pEnemy->bUse == true) pEnemy->pos.x += g_scrollAll.x;
		}
		for (int nCnt = 0; nCnt < BLOCK_MAX; nCnt++, pBlock++)
		{
			if (pBlock->bUse == true) pBlock->pos.x += g_scrollAll.x;
		}
		pGoal->pos.x += g_scrollAll.x;
	}

	//�v���C���[��O
	//if (pPlayer->pos.y > SCREEN_HEIGHT + pPlayer->fHeight)
	//{
	//	for (int nCnt = 0; nCnt < ENEMY_POLYGON; nCnt++, pEnemy++)
	//	{
	//		if (pEnemy->bUse == true) pEnemy->pos.x += 200.0f;
	//	}
	//	for (int nCnt = 0; nCnt < BLOCK_MAX; nCnt++, pBlock++)
	//	{
	//		if (pBlock->bUse == true) pBlock->pos.x += 200.0f;
	//	}
	//	if (pGoal->bUse == true) pGoal->pos.x += 200.0f;
	//}

	//pEnemy = GetEnemy();
	//pGoal = GetGoal();

	//if (pPlayer->pos.y > SCREEN_HEIGHT + pPlayer->fHeight)
	//{
	//	pPlayer->pos.y = 0;
	//	for (int nCnt = 0; nCnt < BLOCK_MAX; nCnt++, pBlock++)
	//	{
	//		if (pBlock->bUse == true)
	//		{
	//			pBlock->pos.x += 200;
	//		}
	//	}
	//	for (int nCnt = 0; nCnt < ENEMY_POLYGON; nCnt++, pEnemy++)
	//	{
	//		if (pEnemy->bUse == true)
	//		{
	//			pEnemy->pos.x += 200; 
	//		}
	//	}
	//	if (pGoal->bUse == true)
	//	{
	//		pGoal->pos.x += 200;
	//	}
	//}
}

//-----------------------------------------------------------------------------
// �X�N���[���̕`�揈��
//-----------------------------------------------------------------------------
void DrawScroll(void)
{
}

D3DXVECTOR3 *GetScroll(void)
{
	return &g_scrollMove;
}