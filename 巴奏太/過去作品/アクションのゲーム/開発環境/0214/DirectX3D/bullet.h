//-----------------------------------------------------------------------------
//
// �e�̏��� �w�b�_�[ [bullet.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _BULLET_H_
#define _BULLET_H_
#include "main.h"

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
#define BULLET_MAX (256)														// �e�̍ő吔
#define BULLET_WIDTH (25 / 2)													// �e�̕�
#define BULLET_HEIGHT (25 / 2)													// �e�̍���

//-----------------------------------------------------------------------------
// �e�̍\����
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXMATRIX mtxWorld;														// ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;															// ����
	D3DXVECTOR3 move;															// �ړ�
	int nLifeTime;																// ����
	bool bUse;																	// �g�p���
	bool bHitWall;
} Bullet;

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move);

#endif

