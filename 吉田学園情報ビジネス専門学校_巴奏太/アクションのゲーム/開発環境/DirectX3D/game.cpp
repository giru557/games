//-----------------------------------------------------------------------------
//
// �Q�[���̏��� [game.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "game.h"
#include "fade.h"
#include "input.h"
#include "light.h"
#include "camera.h"
#include "model.h"
#include "player.h"
#include "polygon.h"
#include "meshfield.h"
#include "bullet.h"
#include "wall.h"
#include "billboard.h"
#include "shadow.h"
#include "motion.h"
#include "field.h"
#include "enemy.h"
#include "collision.h"
#include "healthbar.h"
#include "UI.h"

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// �Q�[���̏���������
//-----------------------------------------------------------------------------
HRESULT InitGame(void)
{
	//���C�g�̏���������
	InitLight();

	//�J�����̏���������
	InitCamera();

	//���f���̏���������
	InitModel();

	//���[�V�����̏���������
	InitMotion();

	//�v���C���[�̏���������
	InitPlayer();

	//�G�̏���������
	InitEnemy();

	//�|���S���̏���������
	//InitPolygon();

	//�n�ʂ̏���������
	//InitMeshfield();

	//test
	InitField();

	//�e�̏���������
	//InitBullet();

	//�ǂ̏���������
	InitWall();

	//�r���{�[�h�̏���������
	//InitBillboard();

	//HP�o�[�̏���������
	InitHP();

	//�e�̏���������
	InitShadow();

	//�R���W�����̏�����
	InitCollision();

	//UI�̏�����
	InitUI();

	//�J�[�\�����\��
	ShowCursor(FALSE);

	return S_OK;
}

//-----------------------------------------------------------------------------
// �Q�[���̏I������
//-----------------------------------------------------------------------------
void UninitGame(void)
{
	//UI�̏I������
	UninitUI();

	//�R���W�����̏I������
	InitCollision();

	//�e�̏I������
	UninitShadow();

	//HP�o�[�̏I������
	UninitHP();

	//�r���{�[�h�̏I������
	//UninitBillboard();

	//�ǂ̏I������
	UninitWall();

	//�e�̏I������
	UninitBullet();

	//test
	//UninitField();

	//�n�ʂ̏I������
	//UninitMeshfield();

	//�|���S���̏I������
	//UninitPolygon();

	//�G�̏I������
	UninitEnemy();

	//�v���C���[�̏I������
	UninitPlayer();

	//���f���̏I������
	UninitModel();

	//�J�����̏I������
	UninitCamera();

	//���C�g�̏I������
	UninitLight();

	//�J�[�\���\��
	ShowCursor(TRUE);
}

//-----------------------------------------------------------------------------
// �Q�[���̍X�V����
//-----------------------------------------------------------------------------
void UpdateGame(void)
{
	//���C�g�̍X�V����
	UpdateLight();

	//�J�����̍X�V����
	UpdateCamera();

	//���f���̍X�V����
	UpdateModel();

	//���[�V�����̍X�V����
	UpdateMotion();

	//�v���C���[�̍X�V����
	UpdatePlayer();

	//�G�̍X�V����
	UpdateEnemy();

	//�|���S���̍X�V����
	//UpdatePolygon();

	//�n�ʂ̍X�V����
	//UpdateMeshfield();

	//test
	UpdateField();

	//�e�̍X�V����
	//UpdateBullet();

	//�ǂ̍X�V����
	UpdateWall();

	//�r���{�[�h�̍X�V����
	//UpdateBillboard();

	//HP�o�[�̍X�V����
	UpdateHP();

	//�e�̍X�V����
	UpdateShadow();

	//�R���W�����̍X�V����
	UpdateCollision();

	//UI�̍X�V����
	UpdateUI();

	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		SetFade(FADE_OUT, MODE_RESULT);
	}
}

//-----------------------------------------------------------------------------
// �Q�[���̕`�揈��
//-----------------------------------------------------------------------------
void DrawGame(void)
{
	//�J�����̐ݒ�
	SetCamera();

	//���f���̕`�揈��
	DrawModel();

	//�v���C���[�̕`�揈��
	DrawPlayer();

	//�G�̕`�揈��
	DrawEnemy();

	//�|���S���̕`�揈��
	//DrawPolygon();

	//�n�ʂ̕`�揈��
	//DrawMeshfield();

	//test
	DrawField();

	//�e�̕`�揈��
	//DrawBullet();

	//�ǂ̕`�揈��
	DrawWall();

	//�r���{�[�h�̕`�揈��
	//DrawBillboard();

	//HP�o�[�̕`�揈��
	DrawHP();

	//�e�̕`�揈��
	DrawShadow();

	//�R���W�����̕`�揈��
	DrawCollision();

	//UI�̕`�揈��
	DrawUI();
}

//-----------------------------------------------------------------------------
// �|�[�Y�Z�b�g
//-----------------------------------------------------------------------------
void SetPause(bool bPause)
{

}