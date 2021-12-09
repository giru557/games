//=============================================================================
//
// �^���J�����̏��� [camera.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "manager.h"
#include "game.h"
#include "player.h"

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
D3DXVECTOR3 CCamera::m_pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);
D3DXVECTOR3 CCamera::m_posDest = CCamera::m_pos;

//*****************************************************************************
// �J�����N���X
//*****************************************************************************


//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCamera::CCamera()
{
	m_shake.bActive = false;
	m_shake.nFrames = 0;
	m_shake.nCounter = 0;
	m_shake.fMagnitude = 0.0f;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
// ������
//=============================================================================
void CCamera::Init(void)
{

}

//=============================================================================
// �I��
//=============================================================================
void CCamera::Uninit(void)
{

}

//=============================================================================
// �X�V
//=============================================================================
void CCamera::Update(void)
{
	// �\���͈͐���
	this->CheckViewArea();

	// ��ʗh��
	this->ShakeProc();
}

//=============================================================================
// �`��
//=============================================================================
void CCamera::Draw(void)
{

}

//=============================================================================
// ���[���h���W����X�N���[�����W�ɕϊ�
//=============================================================================
D3DXVECTOR3 CCamera::WorldToScreenPosition(D3DXVECTOR3 WorldPos)
{
	// �X�N���[�����W�̌��_
	D3DXVECTOR3 screenOrigin = D3DXVECTOR3(m_pos.x - SCREEN_WIDTH / 2.0f, m_pos.y - SCREEN_HEIGHT / 2.0f, 0.0f);

	// ���[���h���W����X�N���[�����W��
	D3DXVECTOR3 screenPos = D3DXVECTOR3(WorldPos.x - screenOrigin.x, WorldPos.y - screenOrigin.y, 0.0f);

	return screenPos;
}

//=============================================================================
// �J�����̒��S�ʒu��ݒ�
//=============================================================================
void CCamera::SetCameraCenter(D3DXVECTOR3 centerPos)
{
	m_posDest = centerPos;
}

//=============================================================================
// �J�����\���͈͂̔���
//=============================================================================
void CCamera::CheckViewArea(void)
{
	// ��ʊO���� X��
	if (m_posDest.x - SCREEN_WIDTH / 2 <= 0.0f)
	{
		m_posDest.x = SCREEN_WIDTH / 2;
	}
	else if (m_posDest.x + SCREEN_WIDTH / 2 >= GAME_FIELD_WIDTH)
	{
		m_posDest.x = GAME_FIELD_WIDTH - SCREEN_WIDTH / 2;
	}

	// ��ʊO���� Y��
	if (m_posDest.y - SCREEN_HEIGHT / 2 <= 0.0f)
	{
		m_posDest.y = SCREEN_HEIGHT / 2;
	}
	else if (m_posDest.y + SCREEN_HEIGHT / 2 >= GAME_FIELD_HEIGHT)
	{
		m_posDest.y = GAME_FIELD_HEIGHT - SCREEN_HEIGHT / 2;
	}
}

//=============================================================================
// ��ʗh�ꏈ��
//=============================================================================
void CCamera::ShakeProc(void)
{
	// �h�炷�t���O�������Ă鎞
	if (m_shake.bActive)
	{
		m_shake.nCounter++;	// �t���[�����J�E���g����

		// �����_�������ɃJ���������炷
		float fRandAngle = D3DXToRadian(rand() % 360);
		D3DXVECTOR3 randVec = D3DXVECTOR3(sinf(fRandAngle) * m_shake.fMagnitude, cosf(fRandAngle) * m_shake.fMagnitude, 0.0f);
		m_pos = m_posDest + randVec;

		// �w��t���[�����𒴂�����t���O���낷
		if (m_shake.nCounter >= m_shake.nFrames) m_shake.bActive = false;
	}
	else
	{
		// ���ꂽ�܂܂ɂȂ�Ȃ��悤�ɖ߂�
		m_pos = m_posDest;
		this->Shake(false);
	}
}

//=============================================================================
// ��ʗh��Ăяo��
//=============================================================================
void CCamera::Shake(float fMagnitude, int nFrame)
{
	if (!m_shake.bActive)
	{
		m_shake.bActive = true;
		m_shake.fMagnitude = fMagnitude;
		m_shake.nFrames = nFrame;
	}
}

void CCamera::Shake(bool stop)
{
	if (!stop)
	{
		m_shake.bActive = false;
		m_shake.fMagnitude = 0.0f;
		m_shake.nCounter = 0;
		m_shake.nFrames = 0;
	}
}