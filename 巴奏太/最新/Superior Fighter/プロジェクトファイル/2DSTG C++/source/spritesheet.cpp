//=============================================================================
//
// �X�v���C�g�V�[�g�̏��� [spritesheet.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "spritesheet.h"
#include "manager.h"
#include "textureloader.h"

//*****************************************************************************
// �X�v���C�g�V�[�g�N���X ( �p����: 2D�|���S�� [scene2D] )
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CSpriteSheet::CSpriteSheet()
{
	m_nCounterAnim = 0;
	m_nPatternH = 0;
	m_nPatternV = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSpriteSheet::~CSpriteSheet()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CSpriteSheet::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// ����������
	if (FAILED(CScene2D::Init(pos, rot, size)))
	{
		return E_FAIL;
	}

	// �X�v���C�g�̏��ݒ�
	this->SetSheetInfo(m_type);

	// �e�N�X�`�����W���X�V
	this->SetTextureUV(
		D3DXVECTOR2(m_nPatternH / m_fSheetColumn, (m_nPatternV + 1.0f) / m_fSheetRow),
		D3DXVECTOR2(m_nPatternH / m_fSheetColumn, m_nPatternH / m_fSheetRow),
		D3DXVECTOR2((m_nPatternH + 1.0f) / m_fSheetColumn, (m_nPatternV + 1.0f) / m_fSheetRow),
		D3DXVECTOR2((m_nPatternH + 1.0f) / m_fSheetColumn, m_nPatternH / m_fSheetRow)
	);

	// �`��D��x
	this->SetPriority(SPRITESHEET_DRAW_PRIORITY);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CSpriteSheet::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CSpriteSheet::Update(void)
{
	CScene2D::Update();

	// �J�E���g�A�b�v
	m_nCounterAnim++;

	// ���t���[�����Ƃɏ��������s
	if (m_nCounterAnim % m_nAnimationSpeed == 0)
	{
		// �p�^�[�����X�V
		m_nPatternH++;

		// �A�j���[�V�������Ō�܂ōĐ����ꂽ��
		if (m_nPatternH >= m_fSheetColumn)
		{
			// �c�����Ō�܂ōĐ����ꂽ�����
			if (m_nPatternV >= m_fSheetRow - 1)
			{
				Uninit();
				return;
			}
			// �c��������ɂ��炷
			else
			{
				m_nPatternH = 0;
				m_nPatternV++;
			}
		}

		// �e�N�X�`�����W���X�V
		this->SetTextureUV(
			D3DXVECTOR2(m_nPatternH / m_fSheetColumn, (m_nPatternV + 1.0f) / m_fSheetRow),
			D3DXVECTOR2(m_nPatternH / m_fSheetColumn, m_nPatternV / m_fSheetRow),
			D3DXVECTOR2((m_nPatternH + 1.0f) / m_fSheetColumn, (m_nPatternV + 1.0f) / m_fSheetRow),
			D3DXVECTOR2((m_nPatternH + 1.0f) / m_fSheetColumn, m_nPatternV / m_fSheetRow)
		);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CSpriteSheet::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// �����֐�
//=============================================================================
CSpriteSheet *CSpriteSheet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size, SPRITESHEET type)
{
	// �����A������
	CSpriteSheet *pSpriteSheet;
	pSpriteSheet = new CSpriteSheet;
	if (pSpriteSheet != NULL)
	{
		pSpriteSheet->m_type = type;
		pSpriteSheet->Init(pos, rot, size);
	}

	return pSpriteSheet;
}

//=============================================================================
// �V�[�g�̏���ݒ�
//=============================================================================
void CSpriteSheet::SetSheetInfo(SPRITESHEET type)
{
	CTextureLoad *pTex = CManager::GetTextureLoad();
	switch (type)
	{
	case SPRITESHEET_EXPLOSION:
		this->BindTexture(pTex->m_TextureMp["EXPLOSION"]);			// Texture
		this->m_fSheetColumn = SPRITESHEET_EXPLOSION_COLUMN;		// Sprite Column
		this->m_fSheetRow = SPRITESHEET_EXPLOSION_ROW;				// Sprite Row
		this->m_nAnimationSpeed = SPRITESHEET_EXPLOSION_ANIMSPEED;	// Animation Speed
		break;

	case SPRITESHEET_TEST:
		this->BindTexture(pTex->m_TextureMp["TEST"]);
		this->m_fSheetColumn = SPRITESHEET_TEST_COLUMN;
		this->m_fSheetRow = SPRITESHEET_TEST_ROW;
		this->m_nAnimationSpeed = SPRITESHEET_TEST_ANIMSPEED;
		break;

	case SPRITESHEET_EXPLOSION2:
		this->BindTexture(pTex->m_TextureMp["EXPLOSION2"]);
		this->m_fSheetColumn = SPRITESHEET_EXPLOSION2_COLUMN;
		this->m_fSheetRow = SPRITESHEET_EXPLOSION2_ROW;
		this->m_nAnimationSpeed = SPRITESHEET_EXPLOSION2_ANIMSPEED;
		break;

	default:
		break;
	}
}