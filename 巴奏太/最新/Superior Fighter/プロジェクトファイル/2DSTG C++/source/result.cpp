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
#include "textureloader.h"

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

	// �I�u�W�F�N�g����
	m_pScene2D = CScene2D::Create((D3DXVECTOR3)SCREEN_CENTER, VECTOR3_ZERO, D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));

	// �Q�[���̏I���󋵂����ăe�N�X�`�������߂�
	if (CGame::GetGameStatus() == CGame::GAMESTATUS_CLEAR)
		m_pScene2D->BindTexture(CManager::GetTextureLoad()->m_TextureMp["GAMECLEAR"]);
	else if (CGame::GetGameStatus() == CGame::GAMESTATUS_OVER)
		m_pScene2D->BindTexture(CManager::GetTextureLoad()->m_TextureMp["GAMEOVER"]);

	// �J�����̒��S��߂�
	CCamera *pCamera = CManager::GetCamera();
	pCamera->SetCameraCenter((D3DXVECTOR3)SCREEN_CENTER);

	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CResult::Uninit(void)
{
	// �w�i�̃|���S���I�u�W�F�N�g��j��
	if (m_pScene2D != NULL) {
		m_pScene2D->Uninit();
		m_pScene2D = NULL;
	}

	// �J��
	Release();
}

//=============================================================================
// �X�V
//=============================================================================
void CResult::Update(void)
{
	// �G���^�[�L�[
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true)
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
