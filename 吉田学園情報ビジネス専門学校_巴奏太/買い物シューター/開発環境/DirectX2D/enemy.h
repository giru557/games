//-----------------------------------------------------------------------------
//
// �G�w�b�_�[ [enemy.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "main.h"

//-----------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------
#define MAX_ENEMY (256) //�G�̍ő吔
#define MAX_TYPE_ENEMY (3) //�G�̎�ސ�
#define ENEMY_BIG_WIDTH (32)
#define ENEMY_BIG_HEIGHT (32)
#define ENEMY_MEDIUM_WIDTH (32)
#define ENEMY_MEDIUM_HEIGHT (16)
#define ENEMY_SMALL_WIDTH (16)
#define ENEMY_SMALL_HEIGHT (16)
#define ENEMY_SIZE (30)
#define ENEMY_SPEED (0.5f)

//-----------------------------------------------------------------------------
//�X�e�[�g�Ǘ�
//-----------------------------------------------------------------------------
typedef enum
{
	ENEMYSTATE_NORMAL = 0,	//�ʏ���
	ENEMYSTATE_DAMAGE,		//�_���[�W���
	ENEMYSTATE_MAX
} ENEMYSTATE;

//-----------------------------------------------------------------------------
//�G���
//-----------------------------------------------------------------------------
typedef enum
{
	ENEMYTYPE_BIG = 0,	//�傫���G
	ENEMYTYPE_MEDIUM,	//�����炢�̓G
	ENEMYTYPE_SMALL,	//�������G
	ENEMYTYPE_MAX
} ENEMYTYPE;

//�G�̃O���[�v
typedef enum
{
	ENEMYGROUP_A = 0,	//�G�̃O���[�vA
	ENEMYGROUP_B,		//�O���[�vB
	ENEMYGROUP_C,		//�O���[�vC
	ENEMYGROUP_D,		//�O���[�vD
	ENEMYGROUP_E,		//�O���[�vE
	ENEMYGROUP_F,		//�O���[�vF
	ENEMYGROUP_MAX
} ENEMYGROUP;

//-----------------------------------------------------------------------------
//�\���̐錾
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ�
	ENEMYTYPE nType;	//���
	bool bUse;			//�g�p���Ă��邩
	ENEMYSTATE state;	//���
	int nCounterState;	//��ԃJ�E���^�[
	int nLife;			//�̗�
	int nAtkInterval;	//�U���C���^�[�o��
	ENEMYGROUP group;	//�O���[�v
} ENEMY;

//-----------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, ENEMYTYPE nType, ENEMYGROUP group);
ENEMY *GetEnemy(void);
bool HitEnemy(int nIdx, int nDamage);

#endif