//-----------------------------------------------------------------------------
//
// �e�w�b�_�[ [bullet.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _BULLET_H_
#define _BULLET_H_
#include "main.h"
#include "player.h"
#include "enemy.h"
#include "explosion.h"
#include "score.h"
#include "sound.h"
#include "effect.h"
#include "barrier.h"

//�e�̃X�e�[�g�Ǘ�
typedef enum
{
	BULLETSTATE_OWNER_PLAYER = 0,	//�v���C���[���e�����������
	BULLETSTATE_OWNER_ENEMY,		//�G���e�����������
	BULLETSTATE_MAX
} BULLETSTATE;

//�e�̎��
typedef enum
{
	BULLETTYPE_NORMAL = 0,
	BULLETTYPE_RAPIDFIRE,
	BULLETTYPE_RAPIDFIRE1,
	BULLETTYPE_RAPIDFIRE2,
	BULLETTYPE_MAX
} BULLETTYPE;

//-----------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------
#define MAX_BULLET (256)	//�e�̍ő吔

//-----------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, BULLETSTATE state, BULLETTYPE type);

#endif
