//-----------------------------------------------------------------------------
//
// �v���C���[ �w�b�_�[ [player.h]
// Author: Souta Tomoe
//-----------------------------------------------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "main.h"


//-----------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------
#define PLAYER_POS_X (640) //�v���C���[�̈ʒu X
#define PLAYER_POS_Y (400) //�v���C���[�̈ʒu Y
#define PLAYER_WIDTH (28)  //�v���C���[�̕�
#define PLAYER_HEIGHT (48) //�v���C���[�̍���
#define PLAYER_SPEED ((float)1.5)   //�v���C���[�̈ړ��̑���

#define TEXTURE_SPLIT_VERTICAL (2)
#define TEXTURE_SPLIT_HORIZONTAL (5)

//�X�e�[�g�Ǘ�
typedef enum
{
	PLAYERSTATE_APPEAR = 0,
	PLAYERSTATE_NORMAL,  	//�ʏ���
	PLAYERSTATE_DAMAGE,		//�_���[�W���
	PLAYERSTATE_WAIT,
	PLAYERSTATE_DEATH,
	PLAYERSTATE_MAX
} PLAYERSTATE;

//����I��
typedef enum
{
	PLAYERWEAPON_NORMAL = 0,	//�ʏ핐��
	PLAYERWEAPON_MINIGUN,		//�K�g�����O�K��
	PLAYERWEAPON_MISSILE,		//�~�T�C��
	PLAYERWEAPON_BARRIER,		//�o���A
	PLAYERWEAPON_MAX
} PLAYERWEAPON;

//�\���̐錾
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	PLAYERSTATE state;	//���
	int nCounterState;	//��ԃJ�E���^�[
	int nLife;			//�̗�
	bool bUse;			//�g�p����
} PLAYER;

//-----------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
bool HitPlayer(int nDamage);
PLAYER *GetPlayer(void);
void SetWeapon(int nWeaponValue);

#endif