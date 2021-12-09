//=============================================================================
//
// �G�t�F�N�g�N���X [effect.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "effect.h"
#include "manager.h"
#include "renderer.h"
#include "textureloader.h"
#include "camera.h"

//*****************************************************************************
// �G�t�F�N�g�N���X ( �p����: 2D�|���S�� [scene2D] )
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEffect::CEffect(int nPriority) : CScene2D(nPriority)
{
	m_fAlpha = 1.0f;
	m_fShrink = 1.0f;
	m_fShrinkSpeed = 0.0f;
	m_fTransparentSpeed = 0.0f;
	m_blendDst = D3DBLEND_ONE;
	m_blendSrc = D3DBLEND_ONE;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEffect::~CEffect()
{

}

//=============================================================================
// ������
//=============================================================================
HRESULT CEffect::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// ����������
	if (FAILED(CScene2D::Init(pos, rot, size)))
	{
		return E_FAIL;
	}

	// �`��D��x��ݒ�
	this->SetPriority(EFFECT_DRAW_PRIORITY);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEffect::Uninit(void)
{
	// �I��
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEffect::Update(void)
{
	// �ʒu�̍X�V�ƌ����̐ݒ�
	D3DXVECTOR3 pos = GetPos();
	m_move.x += (0 - m_move.x) * 0.01f;
	m_move.y += (0 - m_move.y) * 0.01f;
	pos += m_move;
	SetPos(pos);

	// �k������
	D3DXVECTOR2 size = GetSize();
	this->SetSize(D3DXVECTOR2(size.x * m_fShrink, size.y * m_fShrink));
	m_fShrink -= m_fShrinkSpeed;
	if (m_fShrink <= 0.8f)
	{
		this->Uninit();
		return;
	}
	
	// ����������
	D3DXCOLOR col = GetColor();
	this->SetColor(D3DXCOLOR(col.r, col.g, col.b, col.a * m_fAlpha));
	m_fAlpha -= m_fTransparentSpeed;
	if (m_fAlpha <= 0.8f)
	{
		this->Uninit();
		return;
	}

	// ���_�X�V
	UpdateVertex();

	// ��ʊO�̂��`�悵�Ȃ�
	D3DXVECTOR3 screenPos = CCamera::WorldToScreenPosition(pos);
	if (screenPos.x > SCREEN_WIDTH + size.x ||
		screenPos.x < 0 - size.x ||
		screenPos.y > SCREEN_HEIGHT + size.y ||
		screenPos.y < 0 - size.y)
		this->SetDrawFlag(false);
	else {
		this->SetDrawFlag(true);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEffect::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	
	// �����_�[�X�e�[�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_SRCBLEND, m_blendSrc);		//�\�[�X�i�`�挳�j�̍������@�̐ݒ�
	pDevice->SetRenderState(D3DRS_DESTBLEND, m_blendDst);			//�f�X�e�B�l�[�V�����i�`���j�̍������@�̐ݒ�

	// �`��
	CScene2D::Draw();

	// �����_�[�X�e�[�g�����Ƃɖ߂�
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		//�\�[�X�i�`�挳�j�̍������@�̐ݒ�
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	//�f�X�e�B�l�[�V�����i�`���j�̍������@�̐ݒ�
}

//=============================================================================
// �e�N�X�`���̐ݒ�
//=============================================================================
void CEffect::SetTexture(EFFECT_TEX tex)
{
	switch (tex)
	{
	case EFFECT_TEX_DEFAULT:
		this->BindTexture(CManager::GetTextureLoad()->m_TextureMp["EFFECT"]);
		break;

	case EFFECT_TEX_PARTS1:
		this->BindTexture(CManager::GetTextureLoad()->m_TextureMp["PLAYERPARTS"]);
		break;

	case EFFECT_TEX_PARTS2:
		this->BindTexture(CManager::GetTextureLoad()->m_TextureMp["PLAYERPARTS2"]);
		break;

	case EFFECT_TEX_PARTS3:
		this->BindTexture(CManager::GetTextureLoad()->m_TextureMp["PLAYERPARTS3"]);
		break;

	case EFFECT_TEX_SMOKE:
		this->BindTexture(CManager::GetTextureLoad()->m_TextureMp["SMOKE"]);
		break;

	default:
		break;
	}
}

//=============================================================================
// �����֐�
//=============================================================================
CEffect *CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size, D3DXCOLOR col, EFFECT_TEX tex, float fShrinkSpd, float fTransparentSpd)
{
	CEffect *pEffect;
	pEffect = new CEffect;
	if (pEffect != NULL)
	{
		pEffect->Init(pos, rot, size);
		pEffect->SetTexture(tex);
		pEffect->SetColor(col);

		// �l���ǂ���������������ꍇ�����Ă��܂�
		if (fTransparentSpd <= 0.0f && fShrinkSpd <= 0.0f)
		{
			pEffect->Uninit();
			pEffect = NULL;
		}
		else
		{
			// ��������l���󂯎���đ���A�}�C�i�X�̏ꍇ�O�ɋ���
			pEffect->m_fTransparentSpeed = fTransparentSpd;
			pEffect->m_fShrinkSpeed = fShrinkSpd;
			if (pEffect->m_fTransparentSpeed <= 0.0f) pEffect->m_fTransparentSpeed = 0.0f;
			if (pEffect->m_fShrinkSpeed <= 0.0f) pEffect->m_fShrinkSpeed = 0.0f;
		}
	}

	return pEffect;
}