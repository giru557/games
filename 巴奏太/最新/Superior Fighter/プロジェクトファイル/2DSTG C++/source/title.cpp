//=============================================================================
//
// �^�C�g�� [title.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "title.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CTitle::m_apTexture[TITLE_TEXTURE_COUNT] = {};

//*****************************************************************************
// �^�C�g���N���X ( �p����: �I�u�W�F�N�g�N���X [scene] )
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTitle::CTitle()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
// ������
//=============================================================================
HRESULT CTitle::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	if (FAILED(D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURE_TITLE_FILEPATH, &m_apTexture[0])) ||
		FAILED(D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURE_START_FILEPATH, &m_apTexture[1])))
	{
		return E_FAIL;
	}

	// �I�u�W�F�N�g�̃C���X�^���X����
	m_pSceneTitle = CScene2D::Create((D3DXVECTOR3)SCREEN_CENTER, VECTOR3_ZERO, D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));
	m_pSceneStart = CScene2D::Create(TITLE_TEXTURE_START_POS, VECTOR3_ZERO, TITLE_TEXTURE_START_SIZE);
	
	// �e�N�X�`���\��t��
	m_pSceneTitle->BindTexture(m_apTexture[0]);
	m_pSceneStart->BindTexture(m_apTexture[1]);

	// �t�F�[�h�C��
	CManager::GetFade()->FadeIN();

	CManager::GetSound()->Play(CSound::SOUND_LABEL_BGM_TITLE);

	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CTitle::Uninit(void)
{
	CManager::GetSound()->Stop();

	// Release all textures
	for (int nCntTex = 0; nCntTex < TITLE_TEXTURE_COUNT; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}

	// Release objects
	m_pSceneStart->Uninit();
	m_pSceneTitle->Uninit();
	m_pSceneStart = NULL;
	m_pSceneTitle = NULL;

	// Release this object
	this->Release();
}

//=============================================================================
// �X�V
//=============================================================================
void CTitle::Update(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	m_nBlinkCounter++;	// �J�E���g�A�b�v

	// �G���^�[�L�[������
	if (pKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		// �t�F�[�h�A�E�g���Ď��̃��[�h
		CManager::GetFade()->FadeOUT(CManager::MODE_GAME);
	}

	// �X�^�[�g�{�^����_�ł�����
	this->BlinkStartObj();
}

//=============================================================================
// �`��
//=============================================================================
void CTitle::Draw(void)
{

}

//=============================================================================
// �X�^�[�g�{�^���̃I�u�W�F��_�ł�����֐�
//=============================================================================
void CTitle::BlinkStartObj(void)
{
	// �t���[���J�E���g����萔�𒴂�����
	if (m_nBlinkCounter > TITLE_START_BLINK_FREQUENCY)
	{
		m_bAppear = !m_bAppear;	// �t���O���]
		m_nBlinkCounter = 0;	// �J�E���^�[������
	}

	// �^�ŕ\���A�U�Ŕ�\��
	if (m_bAppear)
	{
		m_pSceneStart->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else
	{
		m_pSceneStart->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	}
}