//-----------------------------------------------------------------------------
//
// ���f���̏��� �w�b�_�[ [model.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
#define MODEL_MOVESPEED (1.0f)													// ���f���̈ړ����x
#define MODEL_SMOOTHTURNSPEED (0.1f)											// ���f�������炩�ɉ�]�����鑬�x (0.0f ~ 1.0f)
#define MODEL_AMOUNT_MAX (64)													// ���f���̍ő吔

//-----------------------------------------------------------------------------
// ���f���̍\����
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXMATRIX mtxWorld;				// ���[���h�}�g���b�N�X
	LPD3DXMESH pMesh;					// ���b�V���ւ̃|�C���^
	LPD3DXBUFFER pBuffMat;				// �}�e���A���ւ̃|�C���^
	DWORD dwNumMat;						// �}�e���A���̐�
	LPDIRECT3DTEXTURE9 pTexture[MODEL_AMOUNT_MAX];	// �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 pos;					// �ʒu
	D3DXVECTOR3 move;					// �ړ��l
	D3DXVECTOR3 rot;					// ����
	D3DXVECTOR3 vtxMin;					// ���_�̍ŏ��l
	D3DXVECTOR3 vtxMax;					// ���_�̍ő�l

	bool bUse;							// �g�p��
} Model;

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
Model *GetModel(void);
#endif