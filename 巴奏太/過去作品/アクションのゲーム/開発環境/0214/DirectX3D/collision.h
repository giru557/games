//-----------------------------------------------------------------------------
//
// �Փ˔���̏��� �w�b�_�[ [collision.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "main.h"

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
#define MAX_VISIBLE_COLLISION (32)		// �����ł���R���W�����̍ő吔

//-----------------------------------------------------------------------------
// �����R���W���� �\����
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X	
	LPD3DXMESH pMesh;		// ���b�V��
	D3DMATERIAL9 mat;		// �}�e���A��
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// ����
	bool bUse;				// �g�p�����ǂ���
	int nIdx;				// �ԍ�
} VCollision;

// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
void InitCollision(void);
void UninitCollision(void);
void UpdateCollision(void);
void DrawCollision(void);
VCollision *GetVCol(void);
void SetSphere(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nSlices, int nIdx);
void SetBox(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, float fDepth, int nIdx);
void MoveVCollision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pRot, int nIdx);
bool CollisionBoxBox(D3DXVECTOR3 *pPosA, D3DXVECTOR3*pPosB);
bool CollisionBoxSphere(D3DXVECTOR3 *pBoxPos, D3DXVECTOR3 *pSpherePos, float fBoxWidth, float fBoxHeight, float fBoxDepth, float fSphereRadius);

#endif