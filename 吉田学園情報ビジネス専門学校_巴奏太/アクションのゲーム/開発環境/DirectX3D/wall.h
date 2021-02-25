//-----------------------------------------------------------------------------
//
// �ǂ̏��� �w�b�_�[ [wall.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _WALL_H_
#define _WALL_H_
#include "main.h"

//-----------------------------------------------------------------------------
// �ǂ̍\����
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXMATRIX mtxWorld;														// ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;															// �ʒu
	D3DXVECTOR3 rot;															// ����
	LPDIRECT3DVERTEXBUFFER9  pVtx;												// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 pIdxBuff;											// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	float fWidth;
	float fHeight;
	bool bUse;																	// �g�p���Ă��邩�ǂ���
} Wall;

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
#define WALL_MAX (32)															// �ǂ̐�
#define WALL_WIDTH (100.0f)													// �ǂ̕�
#define WALL_HEIGHT (100.0f)													// �ǂ̍���

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
HRESULT InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
Wall *GetWall(void);
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 nor, float fWidth, float fHeight);

#endif

