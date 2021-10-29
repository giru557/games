//-----------------------------------------------------------------------------
//
// �v���C���[�̏��� [player.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include "player.h"
#include "input.h"
#include "goalflag.h"
#include "fade.h"
#include "enemy.h"
#include "scroll.h"
#include "sound.h"
#include <stdio.h>
#include <cmath>

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
#define PLAYER_TEX (3)	// �e�N�X�`���̎g�p����

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffPlayer = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTexturePlayer[PLAYER_TEX] = { NULL };			// �|���S���̃e�N�X�`���ւ̃|�C���^
LPD3DXFONT g_pFontPlayer = NULL;									// �t�H���g�ւ̃|�C���^
PLAYER g_player;													// �v���C���[�̏��
D3DXVECTOR3 g_startPos;
D3DXVECTOR3 g_finishPos;

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
void MovePlayer(void);
void WallJump(void);
void Scroll(void);
void TransferBlockMove();
void CharacterSlide(void);
void CharacterState(void);

//-----------------------------------------------------------------------------
// �|���S���̏���������
//-----------------------------------------------------------------------------
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;
	PLAYER *pPlayer;

	pPlayer = &g_player;

	//�v���C���[�̏�����
	pPlayer->pos = D3DXVECTOR3(PLAYER_POS_X, PLAYER_POS_Y, 0.0f);
	pPlayer->posold = pPlayer->pos;
	pPlayer->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pPlayer->fWidth = PLAYER_WIDTH;
	pPlayer->fHeight = PLAYER_HEIGHT;
	pPlayer->fSpeed = PLAYER_SPEED;
	pPlayer->nMaxPatternH = PLAYER_TEX_SPLIT_H;
	pPlayer->nMaxPatternV = PLAYER_TEX_SPLIT_V;
	pPlayer->nPatternH = 0;
	pPlayer->nPatternV = 0;
	pPlayer->nCounterAnim = 0;
	pPlayer->nLastKey = 0;
	pPlayer->nWaitSlide = 0;
	pPlayer->nWait = 0;
	pPlayer->nLife = 1;
	pPlayer->bUse = true;
	pPlayer->bJump = false;
	pPlayer->bOnWall = false;
	pPlayer->scrollState = PLAYERSCROLL_CENTER;
	pPlayer->state = PLAYERSTATE_NORMAL;
	pPlayer->damage = DAMAGESTATE_NORMAL;

	g_startPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_finishPos = D3DXVECTOR3(SCREEN_WIDTH + 1000, 0.0f, 0.0f);

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�t�H���g�̐���
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFontPlayer);

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\wizard_sprite.png", &g_pTexturePlayer[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\wizard_sliding.png", &g_pTexturePlayer[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\wizard_onwall.png", &g_pTexturePlayer[2]);
	//D3DXCreateTextureFromFileEx(pDevice, "data\\TEXTURE\\wizard_sprite.png", 6000, 900, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0xFF000000, NULL, NULL, &g_pTexturePlayer);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPlayer, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVertex, 0);

	//���_���W D3DXVECTOR3(X,Y, 0.0f);
	pVertex[0].pos = D3DXVECTOR3(pPlayer->pos.x - pPlayer->fWidth, pPlayer->pos.y, 0.0f);
	pVertex[1].pos = D3DXVECTOR3(pPlayer->pos.x - pPlayer->fWidth, pPlayer->pos.y - pPlayer->fHeight, 0.0f);
	pVertex[2].pos = D3DXVECTOR3(pPlayer->pos.x + pPlayer->fWidth, pPlayer->pos.y, 0.0f);
	pVertex[3].pos = D3DXVECTOR3(pPlayer->pos.x + pPlayer->fWidth, pPlayer->pos.y - pPlayer->fHeight, 0.0f);

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
	pVertex[0].tex = D3DXVECTOR2((1.0f / (float)pPlayer->nMaxPatternH) * (pPlayer->nPatternH), (1.0f / pPlayer->nMaxPatternV) * (pPlayer->nPatternV + 1.0f));
	pVertex[1].tex = D3DXVECTOR2((1.0f / (float)pPlayer->nMaxPatternH) * (pPlayer->nPatternH), (1.0f / pPlayer->nMaxPatternV) * (pPlayer->nPatternV));
	pVertex[2].tex = D3DXVECTOR2((1.0f / (float)pPlayer->nMaxPatternH) * (pPlayer->nPatternH + 1.0f), (1.0f / pPlayer->nMaxPatternV) * (pPlayer->nPatternV + 1.0f));
	pVertex[3].tex = D3DXVECTOR2((1.0f / (float)pPlayer->nMaxPatternH) * (pPlayer->nPatternH + 1.0f), (1.0f / pPlayer->nMaxPatternV) * (pPlayer->nPatternV));

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
// �|���S���̏I������
//-----------------------------------------------------------------------------
void UninitPlayer(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}


	for (int nCntTEX = 0; nCntTEX < PLAYER_TEX; nCntTEX++)
	{
		//�e�N�X�`���̊J��
		if (g_pTexturePlayer[nCntTEX] != NULL)
		{
			g_pTexturePlayer[nCntTEX]->Release();
			g_pTexturePlayer[nCntTEX] = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
// �|���S���̍X�V����
//-----------------------------------------------------------------------------
void UpdatePlayer(void)
{
	VERTEX_2D *pVertex;
	BLOCK *pBlock;
	PLAYER *pPlayer;

	pBlock = GetBlock();
	pPlayer = &g_player;

	if (pPlayer->bUse == true)
	{
		//�O��ʒu�ۑ�
		pPlayer->posold = pPlayer->pos;

		//�v���C���[�̈ړ�
		MovePlayer();

		//�ړ��ʂ̌���
		pPlayer->move.x += (0 - pPlayer->move.x) * PLAYER_SPEED_DECAY;

		//�d��
		pPlayer->move.y += PLAYER_GRAVITY;

		//�J�n�ʒu�ƏI���ʒu���v���C���[�̈ړ��ł��炷
		g_finishPos.x += -pPlayer->move.x;
		g_startPos.x += -pPlayer->move.x;

		////�X�N���[���̏�Ԑݒ�
		//Scroll();

		//�|���S���̈ʒu�X�V
		pPlayer->pos.x += pPlayer->move.x;
		pPlayer->pos.y += pPlayer->move.y;


		//�X�N���[���̏�Ԃ��画��
		switch (pPlayer->scrollState)
		{
		case PLAYERSCROLL_NORMAL:
			pPlayer->fSpeed = PLAYER_SPEED;
			break;

		case PLAYERSCROLL_CENTER:
			if (pPlayer->pos.x > (SCREEN_WIDTH / 2) + 10)
			{
				pPlayer->pos.x = (SCREEN_WIDTH / 2) + 10;
				pPlayer->fSpeed = PLAYER_SPEED;
			}
			else if (pPlayer->pos.x < (SCREEN_WIDTH / 2) - 10)
			{
				pPlayer->pos.x = (SCREEN_WIDTH / 2) - 10;
				pPlayer->fSpeed = PLAYER_SPEED;
			}
			else
			{
				pPlayer->fSpeed = PLAYER_SPEED;
			}
			break;
		}

		// �_���[�W���
		switch (pPlayer->damage)
		{
		case DAMAGESTATE_HURT:
			//�_���[�W
			pPlayer->nLife--;

			//��Ԃ𖳓G�Ɉڍs
			pPlayer->damage = DAMAGESTATE_INV;
			break;
			
		case DAMAGESTATE_INV:
			if (pPlayer->nLife <= 0)
			{
				pPlayer->damage = DAMAGESTATE_DIE;
			}
			pPlayer->nWait++;
			if (pPlayer->nWait > 60)
			{
				pPlayer->damage = DAMAGESTATE_NORMAL;
				pPlayer->nWait = 0;
			}

		case DAMAGESTATE_DIE:
			pPlayer->nWait++;
			pPlayer->move.x = 0;
			if (pPlayer->nWait > 30)
			{
				PlaySound(SOUND_LABEL_SE_DIE_PLAYER);
				SetFade(FADE_OUT, MODE_GAME);
				pPlayer->nWait = 0;
			}
			break;

		default:
			break;
		}
		////��O�ɗ�����
		if (pPlayer->pos.y > SCREEN_HEIGHT + pPlayer->fHeight)
		{
			GOAL *pGoal;
			ENEMY *pEnemy;
			pEnemy = GetEnemy();
			pGoal = GetGoal();
			pPlayer->pos.y = 100;
			for (int nCnt = 0; nCnt < BLOCK_MAX; nCnt++, pBlock++)
			{
				if (pBlock->bUse == true)
				{
					pBlock->pos.x += 200;
				}
			}
			for (int nCnt = 0; nCnt < ENEMY_POLYGON; nCnt++, pEnemy++)
			{
				if (pEnemy->bUse == true)
				{
					pEnemy->pos.x += 200;
				}
			}
			if (pGoal->bUse == true)
			{
				pGoal->pos.x += 200;
			}
		}


		//�L�����N�^�[�̏�Ԃɂ�鏈��
		CharacterState();

		//�v���C���[������Ă�u���b�N�̈ړ����������p��
		TransferBlockMove();

		//�ǃL�b�N��Ԕ���
		WallJump();

		//�u���b�N�Ƃ̔���
		g_player.bJump = CollisionBlock(&g_player.pos, &g_player.posold, &g_player.move, g_player.fWidth, g_player.fHeight);

		//�S�[���t���b�O�Ƃ̔���
		CollisionGoal(&g_player.pos, &g_player.posold, &g_player.move, g_player.fWidth, g_player.fHeight);

		//�G�Ƃ̓����蔻��
		CollisionEnemy(&g_player.pos, &g_player.posold, &g_player.move, g_player.fWidth, g_player.fHeight);

		//���_�o�b�t�@�����b�N���Ē��_���ւ̃|�C���^���擾
		g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVertex, 0);

		//���_���̐ݒ�
		if (pPlayer->nLastKey == 0)
		{
			pVertex[0].tex = D3DXVECTOR2((1.0f / (float)pPlayer->nMaxPatternH) * (pPlayer->nPatternH), (1.0f / pPlayer->nMaxPatternV) * (pPlayer->nPatternV + 1.0f));
			pVertex[1].tex = D3DXVECTOR2((1.0f / (float)pPlayer->nMaxPatternH) * (pPlayer->nPatternH), (1.0f / pPlayer->nMaxPatternV) * (pPlayer->nPatternV));
			pVertex[2].tex = D3DXVECTOR2((1.0f / (float)pPlayer->nMaxPatternH) * (pPlayer->nPatternH + 1.0f), (1.0f / pPlayer->nMaxPatternV) * (pPlayer->nPatternV + 1.0f));
			pVertex[3].tex = D3DXVECTOR2((1.0f / (float)pPlayer->nMaxPatternH) * (pPlayer->nPatternH + 1.0f), (1.0f / pPlayer->nMaxPatternV) * (pPlayer->nPatternV));
		}
		else
		{
			pVertex[0].tex = D3DXVECTOR2((1.0f / (float)pPlayer->nMaxPatternH) * (pPlayer->nPatternH + 1.0f), (1.0f / pPlayer->nMaxPatternV) * (pPlayer->nPatternV + 1.0f));
			pVertex[1].tex = D3DXVECTOR2((1.0f / (float)pPlayer->nMaxPatternH) * (pPlayer->nPatternH + 1.0f), (1.0f / pPlayer->nMaxPatternV) * (pPlayer->nPatternV));
			pVertex[2].tex = D3DXVECTOR2((1.0f / (float)pPlayer->nMaxPatternH) * (pPlayer->nPatternH), (1.0f / pPlayer->nMaxPatternV) * (pPlayer->nPatternV + 1.0f));
			pVertex[3].tex = D3DXVECTOR2((1.0f / (float)pPlayer->nMaxPatternH) * (pPlayer->nPatternH), (1.0f / pPlayer->nMaxPatternV) * (pPlayer->nPatternV));


		}

		//���_���W�̍X�V
		pVertex[0].pos = D3DXVECTOR3(pPlayer->pos.x - pPlayer->fWidth, pPlayer->pos.y, 0.0f);
		pVertex[1].pos = D3DXVECTOR3(pPlayer->pos.x - pPlayer->fWidth, pPlayer->pos.y - pPlayer->fHeight, 0.0f);
		pVertex[2].pos = D3DXVECTOR3(pPlayer->pos.x + pPlayer->fWidth, pPlayer->pos.y, 0.0f);
		pVertex[3].pos = D3DXVECTOR3(pPlayer->pos.x + pPlayer->fWidth, pPlayer->pos.y - pPlayer->fHeight, 0.0f);

		//���_�J���[�̐ݒ�
		if (pPlayer->damage == DAMAGESTATE_DIE)
		{
			pVertex[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);  //D3DCOLOR_RGBA(��, ��, ��, �����x);
			pVertex[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVertex[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVertex[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		}
		else
		{
			pVertex[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);  //D3DCOLOR_RGBA(��, ��, ��, �����x);
			pVertex[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVertex[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVertex[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}

		//���_�o�b�t�@���A�����b�N
		g_pVtxBuffPlayer->Unlock();

#ifdef _DEBUG
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{
			SetFade(FADE_OUT, MODE_RESULT);
		}
#endif
	}
}

//-----------------------------------------------------------------------------
// �|���S���̕`�揈��
//-----------------------------------------------------------------------------
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	PLAYER *pPlayer;
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[256];

	pPlayer = &g_player;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	switch (pPlayer->state)
	{
	case PLAYERSTATE_NORMAL:
		pDevice->SetTexture(0, g_pTexturePlayer[0]);
		break;

	case PLAYERSTATE_SLIDE:
		pDevice->SetTexture(0, g_pTexturePlayer[1]);
		break;

	case PLAYERSTATE_ONWALL:
		pDevice->SetTexture(0, g_pTexturePlayer[2]);
		break;

	case PLAYERSTATE_LAND:
		pDevice->SetTexture(0, g_pTexturePlayer[0]);
		break;
	}

	if (pPlayer->bUse == true)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

#ifdef _DEBUG

	sprintf(&aStr[0], "move: %f \n", g_player.move.x);

	////�e�L�X�g�̕`��
	g_pFontPlayer->DrawText(NULL, &aStr[0], -1, &rect, DT_BOTTOM, D3DCOLOR_RGBA(255, 255, 255, 255));

#endif
}

//-----------------------------------------------------------------------------
// �v���C���[���̎擾
//-----------------------------------------------------------------------------
PLAYER *GetPlayer(void)
{
	return &g_player;
}

//-----------------------------------------------------------------------------
// �v���C���[�̈ړ�����
//-----------------------------------------------------------------------------
void MovePlayer()
{
	PLAYER *pPlayer;
	BLOCK *pBlock;

	pBlock = GetBlock();
	pPlayer = &g_player;
	
	//�Ō�ɉ������L�[��ۑ�
	if (GetKeyboardPress(DIK_D) == true)
	{
		pPlayer->nLastKey = 0;
	}
	else if (GetKeyboardPress(DIK_A) == true)
	{
		pPlayer->nLastKey = 1;
	}

	if (pPlayer->state != PLAYERSTATE_SLIDE)
	{
		if (pPlayer->damage != DAMAGESTATE_DIE)
		{
			//���E�ړ�
			if (GetKeyboardPress(DIK_D) == true)
			{
				pPlayer->nCounterAnim++;

				//�e�N�X�`���̃p�^�[�����A�j���[�V����������
				if (pPlayer->nCounterAnim % PLAYER_TEX_ANIMINTERVAL == 0)
				{
					pPlayer->nPatternH = (pPlayer->nPatternH + 1) % PLAYER_TEX_SPLIT_H;
					pPlayer->nCounterAnim = 0;
				}

				//�ړ����Z
				pPlayer->move.x += sinf(D3DX_PI * 0.5f) * pPlayer->fSpeed;
			}
			else if (GetKeyboardPress(DIK_A) == true)
			{
				pPlayer->nCounterAnim++;

				//�e�N�X�`���̃p�^�[�����A�j���[�V����������
				if (pPlayer->nCounterAnim % PLAYER_TEX_ANIMINTERVAL == 0)
				{
					pPlayer->nPatternH = (pPlayer->nPatternH + 1) % PLAYER_TEX_SPLIT_H;
					pPlayer->nCounterAnim = 0;
				}

				//�ړ����Z
				pPlayer->move.x += sinf(-D3DX_PI * 0.5f) * pPlayer->fSpeed;
			}

			//�ړ��L�[�����[�X���e�N�X�`���p�^�[��������
			if (GetKeyboardRelease(DIK_A) == true || GetKeyboardRelease(DIK_D) == true)
			{
				pPlayer->nPatternH = 0;
			}

			//�W�����v
			if (pPlayer->bJump == false)
			{
				if (GetKeyboardTrigger(DIK_SPACE) == true)
				{
					//�T�E���h�Đ�
					PlaySound(SOUND_LABEL_SE_JUMP);

					//�ړ����Z
					pPlayer->move.y -= 20.0f;

					//�W�����v��ԂɈڍs
					pPlayer->bJump = true;
				}
			}
			//�W�����v��
			if (pPlayer->bJump == true)
			{
				//�A�j���[�V�����p�^�[���Œ�
				pPlayer->nPatternH = 1;
			}

			//�ǃW�����v
			if (pPlayer->bOnWall == true)
			{
				if (GetKeyboardTrigger(DIK_SPACE) == true)
				{
					if (GetKeyboardPress(DIK_D) == true)
					{
						//�ړ����Z
						pPlayer->move.y -= 25.0f;
						pPlayer->move.x -= 40.0f;

						pPlayer->bOnWall = false;
					}
					else if (GetKeyboardPress(DIK_A) == true)
					{
						pPlayer->move.y -= 25.0f;
						pPlayer->move.x += 40.0f;

						pPlayer->bOnWall = false;
					}
				}
			}
		}
	}

	//�X���C�f�B���O��
	if (GetKeyboardTrigger(DIK_LSHIFT) == true)
	{
		pPlayer->nPatternH = 0;
		CharacterSlide();
	}
}

//-----------------------------------------------------------------------------
// �ǃW�����v
//-----------------------------------------------------------------------------
void WallJump(void)
{
	BLOCK *pBlock;
	PLAYER *pPlayer;

	pPlayer = &g_player;
	pBlock = GetBlock();

	pPlayer->bOnWall = false;
	for (int nCntBLK = 0; nCntBLK < BLOCK_MAX; nCntBLK++, pBlock++)
	{
		if (g_player.bJump == true)
		{
			if ((pPlayer->pos.y - pPlayer->fHeight > pBlock->pos.y && pPlayer->pos.y - pPlayer->fHeight < pBlock->pos.y + pBlock->fHeight) &&
				(pPlayer->pos.x + pPlayer->fWidth > pBlock->pos.x && pPlayer->pos.x - pPlayer->fWidth < pBlock->pos.x + pBlock->fWidth))
			{
				if ((pPlayer->posold.x + pPlayer->fWidth <= pBlock->pos.x) ||
					(pPlayer->posold.x - pPlayer->fWidth >= pBlock->pos.x + pBlock->fWidth))
				{
					if (pPlayer->move.y > 0.0f)
					{
						pPlayer->move.y = 1.0f;
						pPlayer->bOnWall = true;
						//if ((pPlayer->pos.x + pPlayer->fWidth > pBlock->pos.x && pPlayer->pos.x - pPlayer->fWidth < pBlock->pos.x + pBlock->fWidth) &&
						//	pPlayer->pos.y - pPlayer->fHeight > pBlock->pos.y + pBlock->fHeight && pPlayer->pos.y - pPlayer->fHeight < pBlock->pos.y + pBlock->fHeight)
						//{
						//	pPlayer->move.y = 0.0f;
						//}
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
// ��ʃX�N���[��
//-----------------------------------------------------------------------------
void Scroll(void)
{
	PLAYER *pPlayer;
	pPlayer = &g_player;

	if (pPlayer->pos.x < (SCREEN_WIDTH / 2) + 10 &&
		pPlayer->pos.x >(SCREEN_WIDTH / 2) - 10)
	{
		pPlayer->scrollState = PLAYERSCROLL_CENTER;
	}
	else
	{
		pPlayer->scrollState = PLAYERSCROLL_NORMAL;
	}
}

//-----------------------------------------------------------------------------
// �u���b�N�ړ��ʎ󂯓n��
//-----------------------------------------------------------------------------
void TransferBlockMove(void)
{
	BLOCK *pBlock;
	PLAYER *pPlayer;
	D3DXVECTOR3 *pScroll;

	pScroll = GetScroll();
	pPlayer = &g_player;
	pBlock = GetBlock();

	for (int nCntBLK = 0; nCntBLK < BLOCK_MAX; nCntBLK++, pBlock++)
	{
		if ((pPlayer->pos.x + pPlayer->fWidth > pBlock->pos.x && pPlayer->pos.x - pPlayer->fWidth < pBlock->pos.x + pBlock->fWidth) &&
			(pPlayer->pos.y > pBlock->pos.y && pPlayer->pos.y - pPlayer->fHeight < pBlock->pos.y + pBlock->fHeight))
		{
			if (pPlayer->posold.y <= pBlock->pos.y)
			{
				if (pBlock->nTypeNum == 1)
				{
					pScroll->x = -pBlock->move.x;
				}
				pPlayer->pos.y += pBlock->move.y;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// �X���C�f�B���O
//-----------------------------------------------------------------------------
void CharacterSlide(void)
{
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	if (pPlayer->bJump == false &&
		pPlayer->state == PLAYERSTATE_NORMAL)
	{
		pPlayer->state = PLAYERSTATE_SLIDE;

		//�T�E���h�Đ�
		PlaySound(SOUND_LABEL_SE_SLIDE);

		//�Ō�ɉ������L�[����X���C�f�B���O�̕���������
		if (pPlayer->nLastKey == 1) // ��
		{
			pPlayer->move.x -= 60.0f;
		}
		else if (pPlayer->nLastKey == 0) // �E
		{
			pPlayer->move.x += 60.0f;
		}
	}
}

void CharacterState(void)
{
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	//�v���C���[�̏��
	switch (pPlayer->state)
	{
	case PLAYERSTATE_NORMAL:
		//�ʏ펞�̃e�N�X�`���ݒ�
		pPlayer->nMaxPatternH = PLAYER_TEX_SPLIT_H;
		pPlayer->nMaxPatternV = PLAYER_TEX_SPLIT_V;

		//�ǒ���t����Ԃ̃X�e�[�g�ύX
		if (pPlayer->bOnWall == true)
		{
			pPlayer->state = PLAYERSTATE_ONWALL;
			pPlayer->fHeight = PLAYER_HEIGHT;
			pPlayer->fWidth = PLAYER_WIDTH;
			pPlayer->nMaxPatternH = 1;
			pPlayer->nMaxPatternV = 1;

			PlaySound(SOUND_LABEL_SE_ONWALL);
		}
		if (pPlayer->bJump == true)
		{
			pPlayer->state = PLAYERSTATE_LAND;
		}
		break;

	case PLAYERSTATE_ONWALL:
		pPlayer->fHeight = PLAYER_HEIGHT;
		pPlayer->fWidth = PLAYER_WIDTH;
		pPlayer->nMaxPatternH = 1;
		pPlayer->nMaxPatternV = 1;

		//���_�������ɖ߂�
		pPlayer->fWidth = PLAYER_WIDTH;
		pPlayer->fHeight = PLAYER_HEIGHT;

		if (pPlayer->bOnWall == false)
		{
			//�X�e�[�g��ʏ펞�ɖ߂�
			pPlayer->state = PLAYERSTATE_NORMAL;

			//�e�N�X�`���ݒ�����ɖ߂�
			pPlayer->nPatternH = 0;
			pPlayer->nMaxPatternH = PLAYER_TEX_SPLIT_H;
			pPlayer->nMaxPatternV = PLAYER_TEX_SPLIT_V;

			//���_�������ɖ߂�
			pPlayer->fWidth = PLAYER_WIDTH;
			pPlayer->fHeight = PLAYER_HEIGHT;
		}
		break;

	case PLAYERSTATE_SLIDE:
		//�X���C�f�B���O���̃e�N�X�`���ݒ�
		pPlayer->nMaxPatternH = PLAYER_TEX_SPLIT_SLIDE_H;
		pPlayer->nMaxPatternV = PLAYER_TEX_SPLIT_SLIDE_V;
		pPlayer->nCounterAnim++;
		//�e�N�X�`���̃p�^�[�����A�j���[�V����������
		if (pPlayer->nCounterAnim % 2 == 0)
		{
			pPlayer->nPatternH = (pPlayer->nPatternH + 1) % PLAYER_TEX_SPLIT_SLIDE_H;
			pPlayer->nCounterAnim = 0;
		}

		//�X���C�f�B���O���̒��_���ݒ�
		pPlayer->fWidth = PLAYER_WIDTH_SLIDING;
		pPlayer->fHeight = PLAYER_HEIGHT_SLIDING;

		//�X���C�f�B���O�̎���
		pPlayer->nWaitSlide++;
		if (pPlayer->nWaitSlide > 30)
		{
			//�X�e�[�g��ʏ펞�ɖ߂�
			pPlayer->state = PLAYERSTATE_NORMAL;

			//�e�N�X�`���ݒ�����ɖ߂�
			pPlayer->nPatternH = 0;
			pPlayer->nMaxPatternH = PLAYER_TEX_SPLIT_H;
			pPlayer->nMaxPatternV = PLAYER_TEX_SPLIT_V;

			//���_�������ɖ߂�
			pPlayer->fWidth = PLAYER_WIDTH;
			pPlayer->fHeight = PLAYER_HEIGHT;

			//�ҋ@���ԃ��Z�b�g
			pPlayer->nWaitSlide = 0;
		}
		break;

	case PLAYERSTATE_LAND:
		//�ǒ���t����Ԃ̃X�e�[�g�ύX
		if (pPlayer->bOnWall == true)
		{
			pPlayer->state = PLAYERSTATE_ONWALL;
			pPlayer->fHeight = PLAYER_HEIGHT;
			pPlayer->fWidth = PLAYER_WIDTH;
			pPlayer->nMaxPatternH = 1;
			pPlayer->nMaxPatternV = 1;

			PlaySound(SOUND_LABEL_SE_ONWALL);
		}

		if (pPlayer->bJump == false)
		{
			//�X�e�[�g��ʏ펞�ɖ߂�
			pPlayer->state = PLAYERSTATE_NORMAL;
			PlaySound(SOUND_LABEL_SE_LAND);
		}
		break;
	}
}