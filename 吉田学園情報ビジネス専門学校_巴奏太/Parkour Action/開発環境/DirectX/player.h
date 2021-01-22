//-----------------------------------------------------------------------------
//
// �v���C���[ �w�b�_�[ [player.h]
// Author: Souta Tomoe
//-----------------------------------------------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "main.h"
#include "block.h"


//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
#define PLAYER_POS_X (SCREEN_WIDTH / 2)					// �v���C���[�̍��W X
#define PLAYER_POS_Y (200)					// �v���C���[�̍��W Y
#define PLAYER_WIDTH ((495/ 2) * 0.15f)		// �v���C���[�̕�
#define PLAYER_HEIGHT (670 * 0.15f)			// �v���C���[�̍���
#define PLAYER_WIDTH_SLIDING ((495/2) * 0.175f) // �v���C���[�̕�(�X���C�f�B���O��)
#define PLAYER_HEIGHT_SLIDING (470 * 0.175f)	  // �v���C���[�̍���(�X���C�f�B���O��)
#define PLAYER_SPEED (2.0f)					// �v���C���[�̈ړ����x
#define PLAYER_SPEED_DECAY (0.25f);			// �v���C���[�̈ړ����x����	
#define PLAYER_GRAVITY (1.2f)				// �v���C���[�̏d��
#define PLAYER_TEX_SPLIT_V (1)				// �v���C���[�̃e�N�X�`���̕����� (�c)
#define PLAYER_TEX_SPLIT_H (12)				// �v���C���[�̃e�N�X�`���̕����� (��)
#define PLAYER_TEX_SPLIT_SLIDE_V (1)		// �X���C�f�B���O���̕�����
#define PLAYER_TEX_SPLIT_SLIDE_H (6)		// �X���C�f�B���O���̕�����
#define PLAYER_TEX_ANIMINTERVAL (5)		// �e�N�X�`���̃A�j���[�V�����̍X�V�p�x

//-----------------------------------------------------------------------------
// �X�N���[���X�e�[�g
//-----------------------------------------------------------------------------
typedef enum
{
	PLAYERSCROLL_NORMAL = 0,	// �ʏ펞
	PLAYERSCROLL_CENTER,		// �v���C���[�������ɂ���ꍇ
	PLAYERSCROLL_MAX
} PLAYERSCROLL;

//-----------------------------------------------------------------------------
// �v���C���[�̏��
//-----------------------------------------------------------------------------
typedef enum
{
	PLAYERSTATE_NORMAL = 0,		// �ʏ펞
	PLAYERSTATE_ONWALL,			// �ǒ���t����
	PLAYERSTATE_SLIDE,			// �X���C�f�B���O��
	PLAYERSTATE_LAND,			// ���n��
	PLAYERSTATE_MAX
} PLAYERSTATE;

//-----------------------------------------------------------------------------
// �v���C���[�̃_���[�W���
//-----------------------------------------------------------------------------
typedef enum
{
	DAMAGESTATE_NORMAL = 0,		// �ʏ펞
	DAMAGESTATE_HURT,			// �_���[�W��
	DAMAGESTATE_INV,			// ���G
	DAMAGESTATE_DIE,			// ���S
	DAMAGESTATE_MAX
} DAMAGESTATE;

//-----------------------------------------------------------------------------
// �\���̐錾
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	D3DXVECTOR3 posold; // �O��̈ʒu
	D3DXVECTOR3 move;	// �ړ�
	float fWidth;		// ��	
	float fHeight;		// ����
	float fSpeed;		// �ړ����x
	int nMaxPatternV;	// �A�j���[�V����������(�c)
	int nMaxPatternH;	// �A�j���[�V����������(��)
	int nPatternV;		// �A�j���[�V�����p�^�[�� (�c)
	int nPatternH;		// �A�j���[�V�����p�^�[�� (��)
	int nCounterAnim;	// �A�j���[�V�����J�E���^�[
	int nLastKey;		// �Ō�ɉ������L�[ (1��A 0��D)
	int nWaitSlide;		// �X���C�h�N�[���_�E��
	int nWait;			// �ҋ@����
	int nLife;			// �̗�
	bool bUse;			// �g�p����
	bool bJump;			// �W�����v����
	bool bOnWall;		// �ǂɒ���t���Ă���
	PLAYERSCROLL scrollState; // �X�N���[���̏��
	PLAYERSTATE state;		  // �v���C���[�̏��
	DAMAGESTATE damage;		  // �_���[�W���
} PLAYER;

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(void);

#endif