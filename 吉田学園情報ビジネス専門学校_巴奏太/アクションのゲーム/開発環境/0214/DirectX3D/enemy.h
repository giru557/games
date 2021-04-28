//-----------------------------------------------------------------------------
//
// �G�̏��� �w�b�_�[ [enemy.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "player.h"
#include <time.h>

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
#define ENEMY_AMOUNT_MAX (32)		// �p�ӂ��Ă���G�̍ő吔
#define ENEMY_KNOCKBACK (10)		// �m�b�N�o�b�N��
#define ENEMY_INVINCIBLE_TIME (500)	// �U�����ꂽ���̖��G����
#define ENEMY_HEIGHT (20)			// �G�̍���
#define ENEMY_WIDTH (20)			// �G�̕�
#define ENEMY_FALLSPEED (0.4f)		// �������x
#define ENEMY_DETECT_RADIUS (200)	// �G���m����
#define ENEMY_MOVESPEED (0.2f)		// �ړ����x
#define ENEMY_ATTACK_RADIUS (35)	// �U���͈�
#define ENEMY_ATTACK_COOLTIME (1000) // �U��CT
#define ENEMY_HP_MAX (3)			// �G�̗̑�

//-----------------------------------------------------------------------------
// �G�̍\����
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;				// �ʒu
	D3DXVECTOR3 posOld;				// �O��̈ʒu
	D3DXVECTOR3 move;				// �ړ�
	D3DXVECTOR3 rot;				// ����
	D3DXVECTOR3 rotDest;			// �ڕW�̌���
	D3DXMATRIX mtxWorld;			// ���[���h�}�g���b�N�X
	LPD3DXMESH pMesh;				// ���b�V���ւ̃|�C���^
	LPD3DXBUFFER pBuffMat;			// �}�e���A���ւ̃|�C���^
	DWORD nNumMat;					// �}�e���A���̐�
	LPDIRECT3DTEXTURE9 pTexture[10];// �e�N�X�`���ւ̃|�C���^

	int nLifeMax;					// �ő�̗�
	int nLife;						// �̗�
	int nIdx;						// ������ID
	bool bUse;						// �g�p����
	bool bHit;						// �U�����蒆��
	bool bInvincible;				// ���G���
	bool bAttack;					// �U������
	DWORD dwTimeInv;				// ���G���Ԍv�Z�p
	DWORD dwTimeAtk;				// �U�����Ԍv�Z�p
} Enemy;

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
Enemy *GetEnemy(void);
void SetEnemy(D3DXVECTOR3 pos);

#endif

