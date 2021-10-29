//=============================================================================
//
// �^���b�g�̎c�[ [debris.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "debris.h"
#include "manager.h"
#include "textureloader.h"
#include "particle.h"

//*****************************************************************************
// �c�[�N���X ( �p����: 2D�|���S�� [scene2D] )
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CDebris::CDebris()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CDebris::~CDebris()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CDebris::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// ����������
	if (FAILED(CScene2D::Init(pos, rot, size)))
	{
		E_FAIL;
	}

	// �e�N�X�`���̐ݒ�
	// ��ނ̐ݒ�
	BindTexture(CManager::GetTextureLoad()->m_TextureMp["BOSSTURRETBROKEN"]);
	SetObjType(OBJTYPE_DEBRIS);
	SetPriority(2);
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CDebris::Uninit(void)
{
	CScene2D::Uninit();
}
//=============================================================================
// �X�V����
//=============================================================================
void CDebris::Update(void)
{
	CScene2D::Update();

	static int nFrameCounter;
	nFrameCounter++;

	if (nFrameCounter > 60) {
		CParticle *pParticle = CParticle::Create(
			GetPos(),
			true,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			D3DXVECTOR2(80, 80),
			CEffect::EFFECT_TEX_SMOKE,
			1,
			3.0f,
			5,
			0.0005f,
			0.001f,
			0.0f,
			0.0f);
		pParticle->SetPriority(3);
		pParticle->SetRenderState(D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA);

		nFrameCounter = 0;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CDebris::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// �����֐�
//=============================================================================
CDebris *CDebris::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// �v���C���[�̐����A������
	CDebris *pDebris = NULL;
	pDebris = new CDebris;
	if (pDebris != NULL)
	{
		pDebris->Init(pos, rot, size);
	}

	return pDebris;
}