//=============================================================================
//
// �p�[�e�B�N�� [minimap.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "particle.h"
#include "manager.h"
#include "renderer.h"
#include "effect.h"

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//*****************************************************************************
// �p�[�e�B�N���N���X ( �h����: �I�u�W�F�N�g�N���X (scene) )
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CParticle::CParticle()
{
	m_nFrameCounter = 0;
	m_blendSrc = D3DBLEND_ONE;
	m_blendDst = D3DBLEND_ONE;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CParticle::~CParticle()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CParticle::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	SetPos(pos);
	SetSize(size);
	SetRot(rot);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CParticle::Uninit(void)
{
	// �J������ (scene�̃|�C���^�z�񂩂�폜�j
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CParticle::Update(void)
{
	m_nFrameCounter++;

	if (m_nFrameCounter <= m_nFrames) {
		for (int nCntPt = 0; nCntPt < m_nQuantity; nCntPt++) {
			float fRandAngle, fRandForce = { 0.0f };
			if (m_fAngleWide <= 0.0f) {
				fRandAngle = (float)(rand()) / ((float)(RAND_MAX / 360));
				fRandForce = (float)(rand()) / ((float)(RAND_MAX / m_fForce));
			}
			else {
				float fAngleMax = m_fAngleDeg + m_fAngleWide / 2;
				float fAngleMin = m_fAngleDeg - m_fAngleWide / 2;
				fRandAngle = fAngleMin + (float) (rand()) / ((float) (RAND_MAX / (fAngleMax - fAngleMin)));
				
				fRandForce = (float)(rand()) / ((float)(RAND_MAX / m_fForce));
			}

			CEffect *pEffect;
			if (m_bRandRot) {
				pEffect = CEffect::Create(GetPos(), D3DXVECTOR3(0, 0, D3DXToRadian(rand() % 360)), GetSize(), m_col, m_tex, m_fShrinkSpeed, m_fFadeSpeed);
			}
			else {
				pEffect = CEffect::Create(GetPos(), GetRot(), GetSize(), m_col, m_tex, m_fShrinkSpeed, m_fFadeSpeed);
			}
			
			pEffect->AddForce(D3DXVECTOR3(sinf(D3DXToRadian(fRandAngle)), cosf(D3DXToRadian(fRandAngle)), 0), fRandForce);
			pEffect->SetRenderState(m_blendSrc, m_blendDst);
			pEffect->SetPriority(GetPriority());
			
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CParticle::Draw(void)
{

}

//=============================================================================
// �����֐�
//=============================================================================
CParticle *CParticle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, D3DXVECTOR2 size, CEffect::EFFECT_TEX tex, int nFrames, float fForce, int nQuantity, float fShrinkSpeed, float fFadeSpeed, float fAngleDeg, float fAngleWide)
{
	// �����A������
	CParticle *pParticle;
	pParticle = new CParticle;
	if (pParticle != NULL)
	{
		pParticle->Init(pos, rot, size);
		pParticle->m_nFrames = nFrames;
		pParticle->m_fForce = fForce;
		pParticle->m_nQuantity = nQuantity;
		pParticle->m_col = col;
		pParticle->m_fFadeSpeed = fFadeSpeed;
		pParticle->m_fShrinkSpeed = fShrinkSpeed;
		pParticle->m_fAngleDeg = fAngleDeg;
		pParticle->m_fAngleWide = fAngleWide;
		pParticle->m_tex = tex;
	}

	return pParticle;
}

CParticle *CParticle::Create(D3DXVECTOR3 pos, bool bRandRot, D3DXCOLOR col, D3DXVECTOR2 size, CEffect::EFFECT_TEX tex, int nFrames, float fForce, int nQuantity, float fShrinkSpeed, float fFadeSpeed, float fAngleDeg, float fAngleWide)
{
	// �����A������
	CParticle *pParticle;
	pParticle = new CParticle;
	if (pParticle != NULL)
	{
		pParticle->Init(pos, VECTOR3_ZERO, size);
		pParticle->m_nFrames = nFrames;
		pParticle->m_fForce = fForce;
		pParticle->m_nQuantity = nQuantity;
		pParticle->m_col = col;
		pParticle->m_fFadeSpeed = fFadeSpeed;
		pParticle->m_fShrinkSpeed = fShrinkSpeed;
		pParticle->m_fAngleDeg = fAngleDeg;
		pParticle->m_fAngleWide = fAngleWide;
		pParticle->m_tex = tex;
		pParticle->m_bRandRot = bRandRot;
	}

	return pParticle;
}