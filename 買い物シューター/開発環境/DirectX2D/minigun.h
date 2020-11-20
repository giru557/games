//-----------------------------------------------------------------------------
//
// �~�j�K���w�b�_�[ [minigun.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _MINIGUN_H_
#define _MINIGUN_H_
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
	MINIGUNSTATE_OWNER_PLAYER = 0,	//�v���C���[���e�����������
	MINIGUNSTATE_OWNER_ENEMY,		//�G���e�����������
	MINIGUNSTATE_MAX
} MINIGUNSTATE;

//�e�̎��
typedef enum
{
	MINIGUNTYPE_LEFT = 0,
	MINIGUNTYPE_RIGHT,
	MINIGUNTYPE_MAX
} MINIGUNTYPE;

//-----------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------
#define MAX_MINIGUN (256)	//�e�̍ő吔

//-----------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------
HRESULT InitMinigun(void);
void UninitMinigun(void);
void UpdateMinigun(void);
void DrawMinigun(void);
void SetMinigun(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, MINIGUNSTATE state);

#endif
