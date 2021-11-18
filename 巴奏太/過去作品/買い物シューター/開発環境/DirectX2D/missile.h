//-----------------------------------------------------------------------------
//
// �~�T�C�� �w�b�_�[ [missile.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _MISSILE_H_
#define _MISSILE_H_
#include "main.h"
#include "player.h"
#include "enemy.h"
#include "explosion.h"
#include "score.h"
#include "sound.h"
#include "effect.h"

//�e�̃X�e�[�g�Ǘ�
typedef enum
{
	MISSILESTATE_OWNER_PLAYER = 0,	//�v���C���[���e�����������
	MISSILESTATE_OWNER_ENEMY,		//�G���e�����������
	MISSILESTATE_MAX
} MISSILESTATE;

//-----------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------
#define MAX_MISSILE (256)	//�e�̍ő吔

//-----------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------
HRESULT InitMissile(void);
void UninitMissile(void);
void UpdateMissile(void);
void DrawMissile(void);
void SetMissile(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, MISSILESTATE state);

#endif

