//-----------------------------------------------------------------------------
//
// �u���b�N�̃w�b�_�[ [block.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _BLOCK_H_
#define _BLOCK_H_
#include "main.h"

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
#define BLOCK_MAX (1024)

//-----------------------------------------------------------------------------
// �u���b�N�̃e�N�X�`�����
//-----------------------------------------------------------------------------
typedef enum
{
	BLOCKTEXTURE_NORMAL = 0,	// �ʏ�u���b�N
	BLOCKTEXTURE_TOP,			// �㑤�u���b�N
	BLOCKTEXTURE_TOPRIGHT,		// �㑤�A�E�[�u���b�N
	BLOCKTEXTURE_TOPLEFT,		// �㑤�A���[�u���b�N
	BLOCKTEXTURE_RIGHT,			// �E�[�u���b�N
	BLOCKTEXTURE_LEFT,			// ���[�u���b�N
	BLOCKTEXTURE_MAX
} BLOCKTEXTURE;

//-----------------------------------------------------------------------------
// �u���b�N�̍\����
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 firstpos;	// �����ʒu
	D3DXVECTOR3 move;		// �ړ�
	D3DXVECTOR3 scroll;		// �X�N���[��
	BLOCKTEXTURE texture;	// �u���b�N�̃e�N�X�`��
	float fWidth;			// ��
	float fHeight;			// ����
	float fMoveTime;
	float fMoveTimeFirst;
	float fMoveSpeed;
	int nTypeNum;			// �u���b�N�ԍ�
	bool bReturn;
	bool bUse;				// �g�p���Ă��邩
} BLOCK;

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
HRESULT InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight, int nTypeNum, BLOCKTEXTURE texture, float fMoveTime, float fMoveSpeed);
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight);
BLOCK *GetBlock(void);

#endif