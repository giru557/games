//=============================================================================
//
// �^���J�������� [camera.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//*****************************************************************************
// �w�b�_�[�̃C���N���[�h
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �J�����N���X
//*****************************************************************************
class CCamera
{
public:
	CCamera();
	~CCamera();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Shake(float fMagnitude, int nFrame);
	void Shake(bool stop);
	
	static D3DXVECTOR3 WorldToScreenPosition(D3DXVECTOR3 worldPos);	// ���W����ʂ̍��W�ɕϊ�
	static void SetCameraCenter(D3DXVECTOR3 centerPos);				// �J�����̈ʒu��ݒ�
	static D3DXVECTOR3 GetCameraCenter(void) { return m_pos; }		// �J�����ʒu���擾
private:
	bool m_bShaking;		// �J�����̗h��t���O
	int m_nShakeFrame;		// �h�ꎞ��
	int m_nFrameCounter;	// �t���[���J�E���g�p
	float m_fMagnitude;		// �h��̋���

	static D3DXVECTOR3 m_pos;		// �J�����ʒu
	static D3DXVECTOR3 m_posDest;	// �J�����̖ڕW�ʒu

	void CheckViewArea(void);	// �J�����\���͈͂̔���
	void ShakeProc(void);		// ��ʗh�ꏈ��
};

#endif