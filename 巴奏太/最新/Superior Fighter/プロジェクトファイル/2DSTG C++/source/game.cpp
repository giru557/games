//=============================================================================
//
// �Q�[����� [game.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "game.h"
#include "player.h"
#include "score.h"
#include "life.h"
#include "input.h"
#include "manager.h"
#include "fade.h"
#include "crosshair.h"
#include "sound.h"
#include "wave.h"
#include "help.h"
#include "minimap.h"
#include "particle.h"
#include "debugger.h"
#include "energy.h"
#include "boss_manager.h"
#include "collision.h"
#include "debris.h"
#include "bg3d.h"

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CGame::GAMESTATUS CGame::m_status = CGame::GAMESTATUS_NONE;
CPlayer *CGame::m_pPlayer = NULL;
CScore *CGame::m_pScore = NULL;
CLife *CGame::m_pLife = NULL;
CCrosshair *CGame::m_pCrosshair = NULL;
CWave *CGame::m_pWave = NULL;
CHelp *CGame::m_pHelp = NULL;
CMiniMap *CGame::m_pMap = NULL;
CEnergy *CGame::m_pEnergy = NULL;
CBossManager *CGame::m_pBoss = NULL;
CBackground3D *CGame::m_pBG3D = NULL;
CCollision *CGame::m_pCollision = NULL;

//*****************************************************************************
// �Q�[����ʃN���X ( �p����: �I�u�W�F�N�g�N���X [scene] )
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CGame::CGame()
{
	m_status = GAMESTATUS_INPROGRESS;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// ������
//=============================================================================
HRESULT CGame::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// �����蔻��N���X�𐶐�
	m_pCollision = new CCollision;
	if (m_pCollision != NULL) {
		m_pCollision->Init();
	}

	// �I�u�W�F�N�g�𐶐�
	m_pBG3D = CBackground3D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 0, SCREEN_HEIGHT / 2), VECTOR3_ZERO, BG_SIZE, BG_DIVISION_X, BG_DIVISION_Z);
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(GAME_FIELD_WIDTH / 2, GAME_FIELD_HEIGHT /2 , 0), VECTOR3_ZERO, D3DXVECTOR2(PLAYER_WIDTH, PLAYER_HEIGHT));
	m_pLife = CLife::Create(LIFE_POSITION, VECTOR3_ZERO);
	m_pScore = CScore::Create(SCORE_POSITION, VECTOR3_ZERO, D3DXVECTOR2(50, 50));
	m_pCrosshair = CCrosshair::Create();
	m_pWave = CWave::Create();
	m_pHelp = CHelp::Create();
	m_pMap = CMiniMap::Create();
	m_pEnergy = CEnergy::Create();
	m_pBoss = CBossManager::Create();

	CManager::GetSound()->Play(CSound::SOUND_LABEL_BGM_INGAME);	// BGM
	ShowCursor(false);	// �J�[�\����\��
	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CGame::Uninit(void)
{
	CManager::GetSound()->Stop();	// Stop BGM
	ShowCursor(true);	// �J�[�\���\��

	// �����蔻��N���X��j��
	if (m_pCollision != NULL) {
		m_pCollision->Uninit();
		delete m_pCollision;
		m_pCollision = NULL;
	}

	// ���̃I�u�W�F�N�g�̊J��
	this->Release();
}

//=============================================================================
// �X�V
//=============================================================================
void CGame::Update(void)
{
#ifdef _DEBUG
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	if (pKeyboard->GetTrigger(DIK_BACKSPACE))
	{
		// �t�F�[�h�A�E�g���Ď��̃��[�h
		CManager::GetFade()->FadeOUT(CManager::MODE_RESULT);
	}

	if (pKeyboard->GetTrigger(DIK_1))
		CManager::GetRenderer()->SetFeedbackEffectStatus(true);
	else if (pKeyboard->GetTrigger(DIK_2))
		CManager::GetRenderer()->SetFeedbackEffectStatus(false);
	
	if (pKeyboard->GetTrigger(DIK_3))
		CManager::GetRenderer()->SetFeedbackEffect(60);

	if (pKeyboard->GetTrigger(DIK_F)) {
		GetLife()->Damage(5);
	}
#endif

	// �����蔻��̍X�V
	m_pCollision->Update();

	// �Q�[���̐i�s�x���Ď�
	if (m_status == GAMESTATUS_CLEAR || m_status == GAMESTATUS_OVER)
	{
		// �t�F�[�h�A�E�g���Ď��̃��[�h
		CManager::GetFade()->FadeOUT(CManager::MODE_RESULT);
	}
}

//=============================================================================
// �`��
//=============================================================================
void CGame::Draw(void)
{

}
