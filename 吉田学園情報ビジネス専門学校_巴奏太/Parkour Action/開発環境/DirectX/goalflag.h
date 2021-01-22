//-----------------------------------------------------------------------------
//
// �S�[�� �w�b�_�[ [goalflag.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _GOAL_H_
#define _GOAL_H_
#include "main.h"

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// GOAL�\����
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 scroll;		// �X�N���[��
	float fWidth;			// ��
	float fHeight;			// ����
	bool bUse;				// �g�p���t���O
	bool bGoal;				// �S�[���������ǂ���
	STAGE stage;
} GOAL;

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
HRESULT InitGoal(void);
void UninitGoal(void);
void UpdateGoal(void);
void DrawGoal(void);
void CollisionGoal(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight);
void SetFlag(D3DXVECTOR3 pos, float fWidth, float fHeight);
GOAL *GetGoal(void);

#endif

