//-----------------------------------------------------------------------------
//
// �v���C���[�̏��� �w�b�_�[ [player.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
#define PLAYER_MOVESPEED (0.5f)													// �v���C���[�̈ړ����x
#define PLAYER_ATTACK_RADIUS (12.0f)											// �v���C���[�̍U���͈�
#define PlAYER_WIDTH (7.5f)
#define PLAYER_HEIGHT (50.0f)
#define PLAYER_INVINCIBLE_TIME (500)	// �U�����ꂽ���̖��G����
#define PLAYER_HEALTH (10)				// �v���C���[�̗̑�

//-----------------------------------------------------------------------------
// ���[�V�����̎��
//-----------------------------------------------------------------------------
typedef enum
{
	MOTIONTYPE_NEUTRAL = 0,			// �j���[�g�������[�V����
	MOTIONTYPE_ATTACK,				// �U�����[�V����
	MOTIONTYPE_WALK,				// �������[�V����
	MOTIONTYPE_MAX
} MOTIONTYPE;

//-----------------------------------------------------------------------------
// �L�[�̍\����
//-----------------------------------------------------------------------------
typedef struct
{
	float fPosX;		// �ʒuX
	float fPosY;		// �ʒuY
	float fPosZ;		// �ʒuZ
	float fRotX;		// ����X
	float fRotY;		// ����Y
	float fRotZ;		// ����Z
} KEY;

//-----------------------------------------------------------------------------
// �L�[���̍\����
//-----------------------------------------------------------------------------
typedef struct
{
	int nFrame;			// �Đ��t���[��
	KEY aKey[10];		// �L�[ (�p�[�c�ő吔��)
} KEY_INFO;

//-----------------------------------------------------------------------------
// ���[�V�������̍\����
//-----------------------------------------------------------------------------
typedef struct
{
	bool bLoop;					// ���[�v�̗L��
	int nNumKey;				// �L�[�̑���
	KEY_INFO aKeyInfo[10];		// �L�[���	(�L�[�̍ő吔)
} MOTION_INFO;

//-----------------------------------------------------------------------------
// ���f���̍\����
//-----------------------------------------------------------------------------
typedef struct
{
	LPD3DXMESH pMeshModel;		// ���b�V���ւ̃|�C���^
	LPD3DXBUFFER pBuffMatModel;	// �}�e���A���ւ̃|�C���^
	DWORD nNumMatModel;			// �}�e���A���̐�
	D3DXVECTOR3 pos;			// �ʒu(�I�t�Z�b�g)
	D3DXVECTOR3 rot;			// ����
	D3DXMATRIX mtxWorld;		// ���[���h�}�g���b�N�X
	int nIdxModelParent;		// �e���f���̃C���f�b�N�X
	int nMotionCount;
} PlayerModel;

//-----------------------------------------------------------------------------
// �v���C���[�̍\����
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;				// �ʒu
	D3DXVECTOR3 posOld;				// �O��̈ʒu
	D3DXVECTOR3 move;				// �ړ��l
	D3DXVECTOR3 rot;				// ����
	D3DXVECTOR3 rotDest;			// �ڕW�̌���
	D3DXMATRIX mtxWorld;			// ���[���h�}�g���b�N�X
	PlayerModel aModel[10];			// ���f��(�p�[�c)
	int nNumModel;					// ���f��(�p�[�c)��

	MOTION_INFO aMotionInfo[10];	// ���[�V�������	 (���[�V�����̍ő吔)
	MOTIONTYPE motionType;			// ���[�V�����^�C�v
	bool bLoopMotion;				// ���[�v�̗L��
	int nCurrentMotion;				// ���݂̃��[�V����
	int nNumMotion;					// ���[�V������
	int nNumKey;					// �L�[��
	int nKey;						// �L�[�i���o�[
	int nCounterMotion;				// ���[�V�����J�E���^�[
	bool bPlayMotion;				// ���[�V�����Đ����

	int nLifeMax;					// �ő�̗�
	int nLife;						// �̗�
	bool bHit;						// �U������Ă�����
	bool bInvincible;				// ���G���
	DWORD dwTime;					// ���Ԍv�Z�p
} Player;

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
KEY KeyPosRot(float posX, float posY, float posZ, float rotX, float rotY, float rotZ);

#endif