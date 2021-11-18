//-----------------------------------------------------------------------------
//
// �Q�[���̏��� [game.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "main.h"
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "enemy.h"
#include "sound.h"
#include "game.h"
#include "effect.h"
#include "barrier.h"
#include "pause.h"
#include "shop.h"
#include "money.h"
#include "missile.h"
#include "BG.h"
#include "minigun.h"
#include "health.h"
#include "coin.h"
#include "complete.h"

//-----------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------
bool g_bPause = false;

//-----------------------------------------------------------------------------
//�Q�[���̏���������
//-----------------------------------------------------------------------------
HRESULT InitGame(void)
{
	//�w�i�̏���������
	InitBG();

	//�|���S���̏���������
	InitPlayer();

	//�G�̏���������
	InitEnemy();

	//�o���A�̏�����
	InitBarrier();

	//�����̏�����
	InitExplosion();

	//�G�t�F�N�g�̏�����
	InitEffect();

	//�e�̏���������
	InitBullet();
	InitMinigun();
	InitMissile();

	//�X�R�A�̏���������
	InitScore();

	//�V���b�v�̏���������
	InitShop();

	//�����̏���������
	InitMoney();

	//�̗͂̏���������
	InitHealth();

	//�R�C���̏���������
	InitCoin();

	//�X�e�[�W�R���v�̏���������
	InitComplete();

	//�|�[�Y�̏���������
	InitPause();

	//�T�E���h�Đ�
	PlaySound(SOUND_LABEL_BGM003);

	//�|�[�Y����
	SetPause(false);

	return S_OK;
}

//-----------------------------------------------------------------------------
//�Q�[���̏I������
//-----------------------------------------------------------------------------
void UninitGame(void)
{
	//�|�[�Y�̏I������
	UninitPause();

	//�X�e�[�W�R���v�̏I������
	UninitComplete();
	
	//�R�C���̏I������
	UninitCoin();

	//�̗͂̏I������
	UninitHealth();
	
	//�����̏I������
	UninitMoney();

	//�V���b�v�̏���������
	UninitShop();

	//�X�R�A�̏I������
	UninitScore();

	//�e�̏I������
	UninitMissile();
	UninitMinigun();
	UninitBullet();

	//�G�t�F�N�g�̏I������
	UninitEffect();

	//�����̏I������
	UninitExplosion();

	//�|���S���̏I������
	UninitPlayer();

	//�o���A�̏I������
	UninitBarrier();

	//�G�̏I������
	UninitEnemy();

	//�w�i�̏I������
	UninitBG();

	//�T�E���h�̏I��
	StopSound(SOUND_LABEL_BGM003);
}

//-----------------------------------------------------------------------------
//�Q�[���̍X�V����
//-----------------------------------------------------------------------------
void UpdateGame(void)
{
	//�|�[�Y�؂�ւ�
	if (GetKeyboardTrigger(DIK_ESCAPE) == true)
	{
		if (g_bPause == false)
		{
			g_bPause = true;
			//�|�[�Y���Đ�
			PlaySound(SOUND_LABEL_SE_SELECT3);
		}
		else
		{
			g_bPause = false;
			//�|�[�Y�������Đ�
			PlaySound(SOUND_LABLE_SE_SELECT4);
		}
		//g_bPause = g_bPause ? false : true;
	}

	if (g_bPause == true)
	{//�|�[�Y��
		//�|�[�Y�̍X�V����
		UpdatePause();
	}
	else
	{//�|�[�Y���łȂ�
		//�w�i�̍X�V����
		UpdateBG();

		//�|���S���̍X�V����
		UpdatePlayer();

		//�G�̍X�V����
		UpdateEnemy();

		//�o���A�̍X�V����
		UpdateBarrier();

		//�����̍X�V����
		UpdateExplosion();

		//�G�t�F�N�g�̍X�V����
		UpdateEffect();

		//�e�̍X�V����
		UpdateBullet();
		UpdateMinigun();
		UpdateMissile();

		//�X�R�A�̍X�V����
		UpdateScore();

		//�V���b�v�̍X�V����
		UpdateShop();

		//�����̍X�V����
		UpdateMoney();

		//�̗͂̍X�V����
		UpdateHealth();

		//�R�C���̍X�V����
		UpdateCoin();

		//�X�e�[�W�R���v�̍X�V����
		UpdateComplete();

	}
}

//-----------------------------------------------------------------------------
//�Q�[���̕`�揈��
//-----------------------------------------------------------------------------
void DrawGame(void)
{
	//BG�̕`�揈��
	DrawBG();

	//�|���S���̕`�揈��
	DrawPlayer();

	//�G�̕`�揈��
	DrawEnemy();

	//�o���A�̕`�揈��
	DrawBarrier();

	//�����̕`�揈��
	DrawExplosion();

	//�G�t�F�N�g�̕`�揈��
	DrawEffect();

	//�e�̕`�揈��
	DrawBullet();
	DrawMinigun();
	DrawMissile();

	//�X�R�A�̕`�揈��
	DrawScore();

	//�V���b�v�̕`�揈��
	DrawShop();

	//�����̕`�揈��
	DrawMoney();

	//�̗͂̕`�揈��
	DrawHealth();

	//�R�C���̕`�揈��
	DrawCoin();

	//�X�e�[�W�R���v�̕`�揈��
	DrawComplete();

	//�|�[�Y���j���[�̕`��
	if (g_bPause == true)
	{
		DrawPause();
	}
}

//
//�|�[�Y�Z�b�g
//
void SetPause(bool bPause)
{
	g_bPause = bPause;
}