//=============================================================================
//
// �J����3D [camera3d.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _CAMERA3D_H_
#define _CAMERA3D_H_

//*****************************************************************************
// �w�b�_�[�̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
// �J�����N���X
//*****************************************************************************
class CCamera3D
{
public:
	CCamera3D();
	~CCamera3D();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(LPDIRECT3DDEVICE9 pDevice);

	D3DXVECTOR3 GetRot(void) { return m_rot; }

private:
	D3DXVECTOR3 m_posV;	// ���_
	D3DXVECTOR3 m_posR;	// �����_
	D3DXVECTOR3 m_vecU;	// ������x�N�g��
	D3DXVECTOR3 m_rot;	// ����
	D3DXMATRIX m_mtxProjection;		// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX m_mtxView;			// �r���[�}�g���b�N�X
};

#endif

