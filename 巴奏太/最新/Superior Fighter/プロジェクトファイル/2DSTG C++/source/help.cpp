//=============================================================================
//
// �w���v�\���N���X [help.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "help.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "textureloader.h"

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//*****************************************************************************
// �w���v�\���N���X ( �h����: �I�u�W�F�N�g�N���X (scene) )
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CHelp::CHelp()
{
	m_bHelpVisible = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CHelp::~CHelp()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CHelp::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// 2D�I�u�W�F�N�g�𐶐�
	for (int nCntScene = 0; nCntScene < HELP_SCENES; nCntScene++){
		m_pScene2D[nCntScene] = CScene2D::Create(pos, rot, size);
		m_pScene2D[nCntScene]->SetConvertState(false);
		m_pScene2D[nCntScene]->SetPriority(HELP_DRAW_PRIORITY);
	}

	m_pScene2D[0]->SetPos(HELP_OPEN_POSIITON);
	m_pScene2D[0]->SetDrawFlag(true);
	m_pScene2D[0]->SetSize(HELP_OPEN_SIZE);
	m_pScene2D[0]->BindTexture(CManager::GetTextureLoad()->m_TextureMp["HELPOPEN"]);

	m_pScene2D[1]->SetPos(HELP_POSIITON);
	m_pScene2D[1]->SetDrawFlag(false);
	m_pScene2D[1]->SetSize(HELP_SIZE);
	m_pScene2D[1]->BindTexture(CManager::GetTextureLoad()->m_TextureMp["HELP"]);

	SwitchVisible();
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CHelp::Uninit(void)
{
	// �J������ (scene�̃|�C���^�z�񂩂�폜�j
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CHelp::Update(void)
{
	// ���_�X�V
	for (int nCntScene = 0; nCntScene < HELP_SCENES; nCntScene++) {
		m_pScene2D[nCntScene]->UpdateVertex();
	}

	CInputKeyboard *pKey = CManager::GetInputKeyboard();

	// �L�[���͂ŕ\���E��\���؂�ւ�
	if (pKey->GetTrigger(DIK_H)) {
		SwitchVisible();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CHelp::Draw(void)
{
	
}

//=============================================================================
// �����֐�
//=============================================================================
CHelp *CHelp::Create(void)
{
	// �����A������
	CHelp *pWave;
	pWave = new CHelp;
	if (pWave != NULL)
	{
		pWave->Init(VECTOR3_ZERO, VECTOR3_ZERO, (D3DXVECTOR2)VECTOR3_ZERO);
	}

	return pWave;
}

//=============================================================================
// �\����Ԃ����ւ���
//=============================================================================
void CHelp::SwitchVisible()
{
	m_bHelpVisible = !m_bHelpVisible;

	if (m_bHelpVisible) {
		m_pScene2D[0]->SetDrawFlag(false);
		m_pScene2D[1]->SetDrawFlag(true);
	}
	else {
		m_pScene2D[0]->SetDrawFlag(true);
		m_pScene2D[1]->SetDrawFlag(false);
	}
}