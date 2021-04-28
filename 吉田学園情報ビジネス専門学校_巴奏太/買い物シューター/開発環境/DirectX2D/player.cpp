//-----------------------------------------------------------------------------
//
// �v���C���[�̏��� [player.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "enemy.h"
#include "BG.h"
#include "sound.h"
#include "fade.h"
#include "minigun.h"
#include "effect.h"
#include "missile.h"
#include "shop.h"
#include <stdio.h>

//-----------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------
#define ANIMATION_INTERVAL (10)				//�A�j���[�V�����̍X�V�p�x

//-----------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffPlayer = NULL;   //���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;			//�|���S���̃e�N�X�`���ւ̃|�C���^
int g_nCounterAnim;									//�A�j���[�V�����J�E���^�[
int g_nPatternAnim;									//�A�j���[�V�����p�^�[���i���o�[
D3DXVECTOR3 g_rotPlayer;							//�|���S���̉�]�p
float g_fLengthPlayer;								//�|���S���̑Ίp���̒���
float g_fAnglePlayer;								//�|���S���̑Ίp���̊p�x
D3DXVECTOR3 g_movePlayer;							//�ړ���
PLAYER g_player;									//�v���C���[�̏��
int g_aPressCounter[2];
PLAYERWEAPON g_PlayerWeapon;						//�ǂ̕�����g�p���邩
//LPD3DXFONT g_pFontPlayer = NULL;							//�t�H���g�ւ̃|�C���^
bool g_bAnim;
int g_nWait;										//�ҋ@����

//�v���g�^�C�v�錾
void MovePlayer(void);
void CharaTex(float nValue, int nIdx);

//-----------------------------------------------------------------------------
//�|���S���̏���������
//-----------------------------------------------------------------------------
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;
	
	g_nCounterAnim = 0;	//�A�j���[�V�����J�E���^�[������
	g_nPatternAnim = 0;	//�A�j���[�V�����p�^�[���i���o�[������
	g_nWait = 0;
	g_PlayerWeapon = PLAYERWEAPON_NORMAL;
	g_player.bUse = true;
	g_player.nCounterState = 0;
	g_player.nLife = 50;
	g_player.state = PLAYERSTATE_WAIT;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�t�H���g�̐���
	//D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFontPlayer);

	//�e�N�X�`���ǂݍ���
	//D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\ship.png", &g_pTexturePlayer);
	D3DXCreateTextureFromFileEx(pDevice, "data\\TEXTURE\\ship.png", 80, 48, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0xFF000000, NULL, NULL, &g_pTexturePlayer);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPlayer, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVertex, 0);

	//�v���C���[�̒��S�ʒu
	g_player.pos = D3DXVECTOR3(PLAYER_POS_X, PLAYER_POS_Y, 0.0f);

	//���_���W D3DXVECTOR3(X,Y, 0.0f);
	pVertex[0].pos = D3DXVECTOR3(g_player.pos.x - PLAYER_WIDTH, g_player.pos.y + PLAYER_HEIGHT, 0.0f);		 //TRIANGLESTRIP�Ŏl�p
	pVertex[1].pos = D3DXVECTOR3(g_player.pos.x - PLAYER_WIDTH, g_player.pos.y - PLAYER_HEIGHT, 0.0f);
	pVertex[2].pos = D3DXVECTOR3(g_player.pos.x + PLAYER_WIDTH, g_player.pos.y + PLAYER_HEIGHT, 0.0f);
	pVertex[3].pos = D3DXVECTOR3(g_player.pos.x + PLAYER_WIDTH, g_player.pos.y - PLAYER_HEIGHT, 0.0f);

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
	pVertex[2].tex = D3DXVECTOR2(0.125f, 1.0f);
	pVertex[3].tex = D3DXVECTOR2(0.125f, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();

	//�v���C���[��ҋ@��ԂɈڍs (2�b��)
	if (g_player.state == PLAYERSTATE_WAIT)
	{
		g_player.nCounterState = 120;
		//�|���S���̐F��߂�
		pVertex[0].col = D3DCOLOR_RGBA(255, 255, 255, 127);  //D3DCOLOR_RGBA(��, ��, ��, �����x);
		pVertex[1].col = D3DCOLOR_RGBA(255, 255, 255, 127);
		pVertex[2].col = D3DCOLOR_RGBA(255, 255, 255, 127);
		pVertex[3].col = D3DCOLOR_RGBA(255, 255, 255, 127);
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
//�|���S���̏I������
//-----------------------------------------------------------------------------
void UninitPlayer(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}
}

//-----------------------------------------------------------------------------
//�|���S���̍X�V����
//-----------------------------------------------------------------------------
void UpdatePlayer(void)
{
	VERTEX_2D *pVertex;
	g_nCounterAnim++; //�A�j���[�V�����J�E���^�[�X�V (�J�E���g�A�b�v)

	if (g_player.bUse == true)
	{
		//�|���S���̈ʒu�X�V
		g_player.pos.x += g_movePlayer.x;
		g_player.pos.y += g_movePlayer.y;

		//�ړ��ʂ̌���
		g_movePlayer.x += (0 - g_movePlayer.x) * 0.2f;
		g_movePlayer.y += (0 - g_movePlayer.y) * 0.2f;

		//�Ίp���̒���
		g_fLengthPlayer = sqrtf((PLAYER_WIDTH) * (PLAYER_WIDTH)+(-PLAYER_HEIGHT) * (-PLAYER_HEIGHT));

		//�Ίp���̊p�x
		g_fAnglePlayer = atan2f(PLAYER_WIDTH, -PLAYER_HEIGHT);


		//���_�o�b�t�@�����b�N���Ē��_���ւ̃|�C���^���擾
		g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVertex, 0);

		//���_���W�̍X�V
		pVertex[0].pos.x = g_player.pos.x + sinf(g_rotPlayer.z + (g_fAnglePlayer + D3DX_PI)) * g_fLengthPlayer;
		pVertex[0].pos.y = g_player.pos.y + cosf(g_rotPlayer.z + (g_fAnglePlayer + D3DX_PI)) * g_fLengthPlayer;
		pVertex[0].pos.z = 0.0f;

		pVertex[1].pos.x = g_player.pos.x + sinf(g_rotPlayer.z + (g_fAnglePlayer * -1)) * g_fLengthPlayer;
		pVertex[1].pos.y = g_player.pos.y + cosf(g_rotPlayer.z + (g_fAnglePlayer * -1)) * g_fLengthPlayer;
		pVertex[1].pos.z = 0.0f;

		pVertex[2].pos.x = g_player.pos.x + sinf(g_rotPlayer.z + (g_fAnglePlayer * -1 + D3DX_PI)) * g_fLengthPlayer;
		pVertex[2].pos.y = g_player.pos.y + cosf(g_rotPlayer.z + (g_fAnglePlayer * -1 + D3DX_PI)) * g_fLengthPlayer;
		pVertex[2].pos.z = 0.0f;

		pVertex[3].pos.x = g_player.pos.x + sinf(g_rotPlayer.z + g_fAnglePlayer) * g_fLengthPlayer;
		pVertex[3].pos.y = g_player.pos.y + cosf(g_rotPlayer.z + g_fAnglePlayer) * g_fLengthPlayer;
		pVertex[3].pos.z = 0.0f;

		//pVertex[3].pos = D3DXVECTOR3(sinf(g_fAnglePolygon) * g_fLengthPolygon, cosf(g_fAnglePolygon) * g_fLengthPolygon, 0.0f);
		//pVertex[0].pos = D3DXVECTOR3(-40 + g_posPolygon.x, 60 + g_posPolygon.y, 0.0f);
		//pVertex[1].pos = D3DXVECTOR3(-40 + g_posPolygon.x, -60 + g_posPolygon.y, 0.0f);
		//pVertex[2].pos = D3DXVECTOR3(40 + g_posPolygon.x, 60 + g_posPolygon.y, 0.0f);
		//pVertex[3].pos = D3DXVECTOR3(40 + g_posPolygon.x, -60 + g_posPolygon.y, 0.0f);

		//ANIMATION_INTERVAL�̐��Ŋ���؂ꂽ��
		if (g_nCounterAnim % ANIMATION_INTERVAL == 0)
		{
			//UV�ݒ�
			g_bAnim = g_bAnim ? false : true;

		}

		//�L�����A�j���[�V����(�펞)
		if (g_bAnim == true)
		{
			g_nPatternAnim = 0;
		}
		else
		{
			g_nPatternAnim = 1;
		}

		//�L�����A�j���[�V����(�ړ���(��))
		if (GetKeyboardPress(DIK_A) == true)
		{
			if (g_movePlayer.x > -5.5f)
			{
				//�L�����̃e�N�X�`���ݒ�
				CharaTex(1.0f, 0);
			}
			else
			{
				//�L�����̃e�N�X�`���ݒ�
				CharaTex(0.0f, 0);
			}
		}
		else
		{
			//�L�����̃e�N�X�`���ݒ�
			CharaTex(2.0f, 0);
		}

		//�L�����A�j���[�V����(�ړ���(�E))
		if (GetKeyboardPress(DIK_D) == true)
		{
			if (g_movePlayer.x > 5.5f)
			{
				//�L�����̃e�N�X�`���ݒ�
				CharaTex(4.0f, 0);
			}
			else
			{
				//�L�����̃e�N�X�`���ݒ�
				CharaTex(3.0f, 0);
			}
		}

		//�X�e�[�g����
		switch (g_player.state)
		{
		case PLAYERSTATE_DAMAGE: //�_���[�W��
			g_player.nCounterState--;

			if (g_player.nCounterState <= 0)
			{
				g_player.state = PLAYERSTATE_NORMAL;

				//�|���S���̐F��߂�
				pVertex[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);  //D3DCOLOR_RGBA(��, ��, ��, �����x);
				pVertex[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVertex[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVertex[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			}
			break;

		case PLAYERSTATE_WAIT:
			g_player.nCounterState--;

			if (g_player.nCounterState <= 0)
			{
				g_player.state = PLAYERSTATE_NORMAL;

				//�|���S���̐F��߂�
				pVertex[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);  //D3DCOLOR_RGBA(��, ��, ��, �����x);
				pVertex[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVertex[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVertex[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			}
			break;
		}

		//���_�o�b�t�@���A�����b�N
		g_pVtxBuffPlayer->Unlock();

		//�L�����ړ�
		MovePlayer();

		//�e�̔��˂ƕ���̑I��
		g_PlayerWeapon = GetWeapon();
		g_aPressCounter[1]++;
		if (g_player.state != PLAYERSTATE_WAIT)
		{
			switch (g_PlayerWeapon)
			{
			case PLAYERWEAPON_NORMAL: //�ʏ펞
				if (GetKeyboardTrigger(DIK_SPACE) == true)
				{
					//�T�E���h�Đ�
					PlaySound(SOUND_LABEL_SE_LASER2);

					//�e����
					SetBullet(g_player.pos, D3DXVECTOR3(0.0f, cosf(D3DX_PI) * 10, 0.0f), 50, BULLETSTATE_OWNER_PLAYER, BULLETTYPE_NORMAL);
				}
				break;

			case PLAYERWEAPON_MINIGUN: //�~�j�K��������
				if (GetKeyboardPress(DIK_SPACE) == true)
				{
					g_aPressCounter[0]++;

					if (g_aPressCounter[0] % 4 == 0)
					{
						//�T�E���h�Đ�
						PlaySound(SOUND_LABEL_SE_LASER3);

						//�e����
						SetMinigun(g_player.pos, D3DXVECTOR3(0.0f, cosf(D3DX_PI) * 15, 0.0f), 40, MINIGUNSTATE_OWNER_PLAYER);
					}
				}
				if (GetKeyboardRelease(DIK_SPACE) == true)
				{
					g_aPressCounter[0] = 0;
				}
				if (GetKeyboardTrigger(DIK_SPACE) == true)
				{
					//�T�E���h�Đ�
					PlaySound(SOUND_LABEL_SE_LASER3);
					SetMinigun(g_player.pos, D3DXVECTOR3(0.0f, cosf(D3DX_PI) * 15, 0.0f), 40, MINIGUNSTATE_OWNER_PLAYER);
				}
				break;

			case PLAYERWEAPON_MISSILE: //�~�T�C��������
				if (GetKeyboardTrigger(DIK_SPACE) == true)
				{
					if (g_aPressCounter[1] > 45)
					{
						SetMissile(g_player.pos, D3DXVECTOR3(0.0f, cosf(D3DX_PI) * 10, 0.0f), 40, MISSILESTATE_OWNER_PLAYER);
						g_aPressCounter[1] = 0;

						//�~�T�C�����ˉ�
						PlaySound(SOUND_LABEL_SE_MISSILE);
					}
				}
				break;

			default: //��O
				if (GetKeyboardTrigger(DIK_SPACE) == true)
				{
					SetBullet(g_player.pos, D3DXVECTOR3(0.0f, cosf(D3DX_PI) * 10, 0.0f), 50, BULLETSTATE_OWNER_PLAYER, BULLETTYPE_NORMAL);

					//�T�E���h�Đ�
					PlaySound(SOUND_LABEL_SE_LASER2);
				}
				break;
			}
		}

		//�ړ�����
		if (g_player.pos.y < 0 + PLAYER_HEIGHT)
		{
			g_player.pos.y = 0 + PLAYER_HEIGHT;
		}
		else if (g_player.pos.y > SCREEN_HEIGHT - PLAYER_HEIGHT)
		{
			g_player.pos.y = SCREEN_HEIGHT - PLAYER_HEIGHT;
		}
		if (g_player.pos.x < 0 + PLAYER_WIDTH)
		{
			g_player.pos.x = 0 + PLAYER_WIDTH;
		}
		else if (g_player.pos.x > SCREEN_WIDTH - PLAYER_WIDTH)
		{
			g_player.pos.x = SCREEN_WIDTH - PLAYER_WIDTH;
		}
	}
	

	//���S���ҋ@����
	if (g_player.bUse == false)
	{
		g_nWait++;
		if (g_nWait > 90)
		{
			SetFade(FADE_OUT, MODE_RESULT);
		}
	}

	//���U���g��ʂ�
	if (GetKeyboardTrigger(DIK_BACKSPACE) == true)
	{
		SetFade(FADE_OUT, MODE_RESULT);
	}
}

//-----------------------------------------------------------------------------
//�|���S���̕`�揈��
//-----------------------------------------------------------------------------
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturePlayer);

	if (g_player.bUse == true)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "move: %f \n", g_movePlayer.x);

	////�e�L�X�g�̕`��
	//g_pFontPlayer->DrawText(NULL, &aStr[0], -1, &rect, DT_BOTTOM, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//�q�b�g����
bool HitPlayer(int nDamage)
{
	VERTEX_2D *pVertex;
	g_player.nLife -= nDamage;

	//���_�o�b�t�@�����b�N���Ē��_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVertex, 0);

	if (g_player.nLife <= 0)
	{
		//����
		SetExplosion(g_player.pos, 50);

		//���S�T�E���h
		PlaySound(SOUND_LABEL_SE_DEAD);

		//�L������s�g�p��
		g_player.bUse = false;

		return true;
	}
	else
	{
		g_player.state = PLAYERSTATE_DAMAGE;
		g_player.nCounterState = 10;

		//�|���S���̐F��ύX
		pVertex[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);  //D3DCOLOR_RGBA(��, ��, ��, �����x);
		pVertex[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVertex[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVertex[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

		//�q�b�g��
		PlaySound(SOUND_LABEL_SE_HIT3);

		return false;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffPlayer->Unlock();
}

PLAYER *GetPlayer(void)
{
	return &g_player;
}

void SetWeapon(PLAYERWEAPON weapon)
{
	g_PlayerWeapon = weapon;
}

void MovePlayer()
{
	if (GetKeyboardPress(DIK_D) == true)
	{
		if (GetKeyboardPress(DIK_W) == true)
		{
			g_movePlayer.x += sinf(D3DX_PI * 0.75f) * PLAYER_SPEED;
			g_movePlayer.y += cosf(D3DX_PI * 0.75f) * PLAYER_SPEED;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{
			g_movePlayer.x += sinf(D3DX_PI * 0.25f) * PLAYER_SPEED;
			g_movePlayer.y += cosf(D3DX_PI * 0.25f) * PLAYER_SPEED;
		}
		else
		{
			g_movePlayer.x += sinf(D3DX_PI * 0.5f) * PLAYER_SPEED;
		}
	}
	else if (GetKeyboardPress(DIK_A) == true)
	{
		if (GetKeyboardPress(DIK_W) == true)
		{
			g_movePlayer.x += sinf(-D3DX_PI * 0.75f) * PLAYER_SPEED;
			g_movePlayer.y += cosf(-D3DX_PI * 0.75f) * PLAYER_SPEED;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{
			g_movePlayer.x += sinf(-D3DX_PI * 0.25f) * PLAYER_SPEED;
			g_movePlayer.y += cosf(-D3DX_PI * 0.25f) * PLAYER_SPEED;
		}
		else
		{
			g_movePlayer.x += sinf(-D3DX_PI * 0.5f) * PLAYER_SPEED;
		}
	}
	else if (GetKeyboardPress(DIK_W) == true)
	{
		if (GetKeyboardPress(DIK_S) == true)
		{
			g_movePlayer.y += cosf(D3DX_PI) * 0;
		}
		else
		{
			g_movePlayer.y += cosf(D3DX_PI) * PLAYER_SPEED;
		}
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{
		if (GetKeyboardPress(DIK_W) == true)
		{
			g_movePlayer.y += cosf(D3DX_PI) * 0;
		}
		else
		{
			g_movePlayer.y += cosf(0) * PLAYER_SPEED;
		}
	}
}

//�L�����e�N�X�`���ݒ�
void CharaTex(float nValue, int nIdx)
{
	VERTEX_2D *pVertex;

	//���_�o�b�t�@�����b�N���Ē��_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVertex, 0);

	pVertex += nIdx * 4;

	//UV�̐ݒ�
	pVertex[0].tex = D3DXVECTOR2((nValue) * (1.0f / TEXTURE_SPLIT_HORIZONTAL), (g_nPatternAnim + 1.0f) * (1.0f / TEXTURE_SPLIT_VERTICAL));
	pVertex[1].tex = D3DXVECTOR2((nValue) * (1.0f / TEXTURE_SPLIT_HORIZONTAL), (g_nPatternAnim) * (1.0f / TEXTURE_SPLIT_VERTICAL));
	pVertex[2].tex = D3DXVECTOR2((nValue + 1.0f) * (1.0f / TEXTURE_SPLIT_HORIZONTAL), (g_nPatternAnim + 1.0f) * (1.0f / TEXTURE_SPLIT_VERTICAL));
	pVertex[3].tex = D3DXVECTOR2((nValue + 1.0f) * (1.0f / TEXTURE_SPLIT_HORIZONTAL), (g_nPatternAnim) * (1.0f / TEXTURE_SPLIT_VERTICAL));

		//���_�o�b�t�@���A�����b�N
	g_pVtxBuffPlayer->Unlock();
}

//���G���Ԑݒ�
void SetInvincible(int nTime)
{
	for (int nCntTime = 1; nCntTime <= nTime; nCntTime++)
	{
		if (nCntTime == nTime)
		{
			//
		}
	}
}