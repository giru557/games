//-----------------------------------------------------------------------------
//
// �Q�[���̏��� [game.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "game.h"
#include "input.h"
#include "sound.h"
#include "goalflag.h"
#include "player.h"
#include "block.h"
#include "enemy.h"
#include "timescore.h"
#include "scroll.h"

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
bool g_bPause = false;
bool g_bStartFlag;
DWORD g_dwStartTime;
DWORD g_dwFinishTime;


//-----------------------------------------------------------------------------
// �Q�[���̏���������
//-----------------------------------------------------------------------------
HRESULT InitGame(void)
{
	GOAL *pGoal;
	pGoal = GetGoal();

	//�w�i�̏���������
	InitBG();

	//�u���b�N�̏���������
	InitBlock();

	//�S�[���̏���������
	InitGoal();
	
	//�G�̏���������
	InitEnemy();

	//�}�b�v�f�[�^�ǂݍ���
	LoadMapData(pGoal->stage);

	//�N���A�^�C���̏���������
	InitTimeScore();

	//�v���C���[�̏���������
	InitPlayer();

	//�|�[�Y�̏���������
	InitPause();

	switch (pGoal->stage)
	{
	case STAGE_1:
		PlaySound(SOUND_LABEL_BGM_STAGE1);
		break;
	case STAGE_2:
		PlaySound(SOUND_LABEL_BGM_STAGE2);
		break;
	default:
		break;
	}

	//�|�[�Y����
	SetPause(false);

	return S_OK;
}

//-----------------------------------------------------------------------------
// �Q�[���̏I������
//-----------------------------------------------------------------------------
void UninitGame(void)
{
	//�N���A�^�C���̏I������
	UninitTimeScore();

	//�v���C���[�̏I������
	UninitPlayer();

	//�G�̏I������
	UninitEnemy();

	//�S�[���̏I������
	UninitGoal();

	//�u���b�N�̏I������
	UninitBlock();

	// �w�i�̏I������
	UninitBG();

	StopSound();
}

//-----------------------------------------------------------------------------
// �Q�[���̍X�V����
//-----------------------------------------------------------------------------
void UpdateGame(void)
{
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

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
		//g_bPause = g_bPause ? false : true; �Ȃ�Ŏg����
	}

	if (g_bPause == true)
	{//�|�[�Y��
		//�|�[�Y�̍X�V����
		UpdatePause();
	}
	else
	{//�|�[�Y���łȂ�
		//�X�N���[���̍X�V����
		UpdateScroll();

		//�w�i�̍X�V����
		UpdateBG();

		//�u���b�N�̍X�V����
		UpdateBlock();

		//�S�[���̍X�V����
		UpdateGoal();

		//�G�̍X�V����
		UpdateEnemy();

		//�v���C���[�̍X�V����
		UpdatePlayer();

		//�N���A�^�C���̍X�V����
		UpdateTimeScore();

	}
}

//-----------------------------------------------------------------------------
// �Q�[���̕`�揈��
//-----------------------------------------------------------------------------
void DrawGame(void)
{
	//�w�i�̕`�揈��
	DrawBG();

	//�u���b�N�̕`�揈��
	DrawBlock();

	//�S�[���̕`�揈��
	DrawGoal();

	//�G�̕`�揈��
	DrawEnemy();

	//�v���C���[�̕`�揈��
	DrawPlayer();

	//�N���A�^�C���̕`�揈��
	DrawTimeScore();

	//�|�[�Y���j���[�̕`��
	if (g_bPause == true)
	{
		DrawPause();
	}
}

//-----------------------------------------------------------------------------
// �|�[�Y�Z�b�g
//-----------------------------------------------------------------------------
void SetPause(bool bPause)
{
	g_bPause = bPause;
}