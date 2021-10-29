//=============================================================================
//
// ���U���g��� [result.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "result.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "camera.h"
#include "game.h"

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CResult::m_apTexture[RESULT_TEXTURE_COUNT] = {};

//*****************************************************************************
// ���U���g�N���X ( �p����: �I�u�W�F�N�g�N���X [scene] )
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CResult::CResult()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
// ������
//=============================================================================
HRESULT CResult::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	if (FAILED(D3DXCreateTextureFromFile(pDevice, RESULT_TEXTURE_0_FILEPATH, &m_apTexture[0])) ||
		FAILED(D3DXCreateTextureFromFile(pDevice, RESULT_TEXTURE_1_FILEPATH, &m_apTexture[1])))
	{
		return E_FAIL;
	}

	// �I�u�W�F�N�g����
	m_pScene2D = CScene2D::Create((D3DXVECTOR3)SCREEN_CENTER, VECTOR3_ZERO, D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));

	// �Q�[���̏I���󋵂����ăe�N�X�`�������߂�
	CGame::GAMESTATUS status = CGame::GetGameStatus();
	if (status == CGame::GAMESTATUS_CLEAR)
	{
		m_pScene2D->BindTexture(m_apTexture[0]);
	}
	else if (status == CGame::GAMESTATUS_OVER)
	{
		m_pScene2D->BindTexture(m_apTexture[1]);
	}

	CCamera *pCamera = CManager::GetCamera();
	pCamera->SetCameraCenter((D3DXVECTOR3)SCREEN_CENTER);

	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CResult::Uninit(void)
{
	for (int nCntTex = 0; nCntTex < RESULT_TEXTURE_COUNT; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
	m_pScene2D->Uninit();
	m_pScene2D = NULL;
	Release();
}

//=============================================================================
// �X�V
//=============================================================================
void CResult::Update(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	if (pKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		// �t�F�[�h�A�E�g���Ď��̃��[�h
		CManager::GetFade()->FadeOUT(CManager::MODE_TITLE);
	}
}

//=============================================================================
// �`��
//=============================================================================
void CResult::Draw(void)
{

}
