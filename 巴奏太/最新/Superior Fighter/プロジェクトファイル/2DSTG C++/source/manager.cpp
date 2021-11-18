//=============================================================================
//
// �}�l�[�W�� [manager.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "fade.h"
#include "camera.h"
#include "textureloader.h"
#include "debugger.h"
#include "pause.h"
#include "camera3d.h"
#include "light.h"

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CRenderer *CManager::m_pRenderer = NULL;
CInputKeyboard *CManager::m_pInputKeyboard = NULL;
CInputMouse *CManager::m_pInputMouse = NULL;
CSound *CManager::m_pSound = NULL;
CTitle *CManager::m_pTitle = NULL;
CGame *CManager::m_pGame = NULL;
CResult *CManager::m_pResult = NULL;
CFade  *CManager::m_pFade = NULL;
CCamera *CManager::m_pCamera = NULL;
CPause *CManager::m_pPause = NULL;
CTextureLoad *CManager::m_pTextureLoad = NULL;
CCamera3D *CManager::m_pCamera3D = NULL;
CLight *CManager::m_pLight = NULL;
CManager::MODE CManager::m_mode = CManager::MODE_TITLE;
#ifdef _DEBUG
CDebug *CManager::m_pDebug = NULL;
#endif

//*****************************************************************************
// �}�l�[�W���N���X
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CManager::CManager()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// �����_���̐����A������
	m_pRenderer = new CRenderer;
	if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
	{
		return E_FAIL;
	}

	// ���C�g�𐶐�
	m_pLight = new CLight;
	if (m_pLight != NULL) {
		m_pLight->Init();
	}

	// 3D�J�����𐶐�
	m_pCamera3D = new CCamera3D;
	if (m_pCamera3D != NULL) {
		m_pCamera3D->Init();
	}

	// �L�[�{�[�h�N���X�̐����A������
	m_pInputKeyboard = new CInputKeyboard;
	if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// �}�E�X�N���X�̐����A������
	m_pInputMouse = new CInputMouse;
	if (FAILED(m_pInputMouse->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// �T�E���h�N���X�̐����A������
	m_pSound = new CSound;
	if (FAILED(m_pSound->Init(hWnd)))
	{
		return E_FAIL;
	}

	// �t�F�[�h�N���X�̐����A������
	m_pFade = new CFade;
	if (FAILED(m_pFade->Init()))
	{
		return E_FAIL;
	}

	// �J�����N���X�̐����A������
	m_pCamera = new CCamera;
	if (m_pCamera != NULL)
	{
		m_pCamera->Init();
	}

	// �|�[�Y�N���X�̐���
	m_pPause = new CPause;
	if (m_pPause != NULL) {
		m_pPause->Init();
	}

	// �e�N�X�`���ǂݍ��݃N���X
	m_pTextureLoad = new CTextureLoad;
	if (FAILED(m_pTextureLoad->Init()))
	{
		return E_FAIL;
	}

#ifdef _DEBUG
	m_pDebug = new CDebug;
	if (FAILED(m_pDebug->Init())) {
		return E_FAIL;
	}
#endif

	// ���[�h�I��
	SetMode(MODE_TITLE);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CManager::Uninit(void)
{
	// �I�u�W�F�N�g�����ׂĔj��
	CScene::ReleaseAll();

#ifdef _DEBUG
	if (m_pDebug != NULL) {
		m_pDebug->Uninit();
		delete m_pDebug;
		m_pDebug = NULL;
	}
#endif

	// �e�N�X�`����j��
	if (m_pTextureLoad != NULL)
	{
		m_pTextureLoad->Uninit();
		delete m_pTextureLoad;
		m_pTextureLoad = NULL;
	}

	// �|�[�Y�̔j��
	if (m_pPause != NULL) {
		m_pPause->Uninit();
		delete m_pPause;
		m_pPause = NULL;
	}

	// �J�����̔j��
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}
	
	// �t�F�[�h�N���X�̔j��
	if (m_pFade != NULL)
	{
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = NULL;
	}

	// �T�E���h�N���X�̔j��
	if (m_pSound != NULL)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}

	// �L�[�{�[�h�̔j��
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	// �}�E�X�̔j��
	if (m_pInputMouse != NULL)
	{
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}

	// �J�����j��
	if (m_pCamera3D != NULL) {
		m_pCamera3D->Uninit();
		delete m_pCamera3D;
		m_pCamera3D = NULL;
	}

	// ���C�g��j��
	if (m_pLight != NULL) {
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}

	// �����_���̔j��
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CManager::Update(void)
{
	// �L�[�{�[�h�̍X�V�Ăяo��
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}

	// �}�E�X�̍X�V�Ăяo��
	if (m_pInputMouse != NULL)
	{
		m_pInputMouse->Update();
	}

	// �����_���̍X�V�Ăяo��
	if (m_pRenderer != NULL)
	{
		// �|�[�Y��Ԃ���Ȃ��Ƃ������X�V
		if (!m_pPause->GetPauseState()) {
			m_pRenderer->Update();
		}
	}

	// �t�F�[�h�̍X�V
	if (m_pFade != NULL)
	{
		m_pFade->Update();
	}

	// �J�����̍X�V
	if (m_pCamera != NULL)
	{
		m_pCamera->Update();
	}

	if (m_pCamera3D != NULL) {
		m_pCamera3D->Update();
	}

	// ���C�g�̍X�V
	if (m_pLight != NULL) {
		m_pLight->Update();
	}

	// �|�[�Y�̍X�V
	if (m_pPause != NULL) {
		m_pPause->Update();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CManager::Draw(void)
{
	// �����_���̕`��Ăяo��
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}
}

//=============================================================================
// ���[�h�̐ݒ�
//=============================================================================
void CManager::SetMode(MODE mode)
{
	// ���݃��[�h�̔j��
	switch (m_mode)
	{
	case MODE_TITLE:
		if (m_pTitle != NULL)
		{
			m_pTitle->Uninit();
			m_pTitle = NULL;
		}
		break;

	case MODE_GAME:
		if (m_pGame != NULL)
		{
			m_pGame->Uninit();
			m_pGame = NULL;
		}
		break;

	case MODE_RESULT:
		if (m_pResult != NULL)
		{
			m_pResult->Uninit();
			m_pResult = NULL;
		}
		break;

	default:
		break;
	}

	// �S�I�u�W�F�N�g�̊J��
	CScene::ReleaseAll();

	// �w�胂�[�h�̐���
	m_mode = mode;
	switch (mode)
	{
	case MODE_TITLE:
		m_pTitle = new CTitle;
		if (m_pTitle != NULL)
		{
			m_pTitle->Init(VECTOR3_ZERO, VECTOR3_ZERO, (D3DXVECTOR2)VECTOR3_ZERO);
		}
		break;

	case MODE_GAME:
		m_pGame = new CGame;
		if (m_pGame != NULL)
		{
			m_pGame->Init(VECTOR3_ZERO, VECTOR3_ZERO, (D3DXVECTOR2)VECTOR3_ZERO);
		}
		break;

	case MODE_RESULT:
		m_pResult = new CResult;
		if (m_pResult != NULL)
		{
			m_pResult->Init(VECTOR3_ZERO, VECTOR3_ZERO, (D3DXVECTOR2)VECTOR3_ZERO);
		}
		break;
	default:
		break;
	}
}

//=============================================================================
// A����B�Ɍ������P�ʃx�N�g�����擾
//=============================================================================
D3DXVECTOR3 CManager::GetVectorFromPointAtoB(D3DXVECTOR3 pointA, D3DXVECTOR3 pointB)
{
	D3DXVECTOR2 vec;

	vec = (D3DXVECTOR2)pointB - (D3DXVECTOR2)pointA;
	D3DXVec2Normalize(&vec, &vec);
	
	return (D3DXVECTOR3)vec;
}

//=============================================================================
// �ʒuA��B�̊Ԃ̋������擾
//=============================================================================
float CManager::GetDistanceAB(D3DXVECTOR3 posA, D3DXVECTOR3 posB)
{
	D3DXVECTOR3 diff;

	diff.x = posA.x - posB.x;
	diff.y = posA.y - posB.y;

	return sqrtf(diff.x * diff.x + diff.y * diff.y);
}