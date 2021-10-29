//-----------------------------------------------------------------------------
//
// �G �w�b�_�[ [enemy.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "main.h"

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
#define ENEMY_POLYGON (64)		// �I�u�W�F�N�g�g�p��
#define ENEMY_WIDTH ((500 / 2) * 0.1f)	// �G�̕�
#define ENEMY_HEIGHT (670 * 0.1f)		// �G�̍���

#define ENEMY_TEX_SPLIT_V (1)
#define ENEMY_TEX_SPLIT_H (12)
#define ENEMY_TEX_ANIMINTERVAL (5)


//-----------------------------------------------------------------------------
// ENEMY�\����
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 posold;		// �O��ʒu
	D3DXVECTOR3 move;		// �ړ��l
	D3DXVECTOR3 scroll;		// �X�N���[��
	float fWidth;			// ��
	float fHeight;			// ����
	int nPatternH;			// �A�j���[�V�����p�^�[��
	int nPatternV;			// �A�j���[�V�����p�^�[��
	int nMaxPatternH;		// �A�j���[�V�����p�^�[���ő吔
	int nMaxPatternV;		// �A�j���[�V�����p�^�[���ő吔
	int nCounterAnim;		// �A�j���[�V�����J�E���^
	bool bUse;				// �g�p���t���O
	bool bJump;				// �󒆂ɂ��邩�ǂ���
} ENEMY;

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight);
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move);
ENEMY *GetEnemy(void);

#endif

