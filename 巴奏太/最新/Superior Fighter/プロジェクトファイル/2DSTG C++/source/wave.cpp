//=============================================================================
//
// �E�F�[�u�N���X [wave.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "wave.h"
#include "manager.h"
#include "renderer.h"
#include "enemy.h"
#include "enemy_gunner.h"
#include "enemy_charger.h"
#include "textureloader.h"
#include "game.h"
#include "input.h"
#include "boss_manager.h"
#include "life.h"

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//*****************************************************************************
// �E�F�[�u�N���X ( �h����: �I�u�W�F�N�g�N���X (scene) )
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CWave::CWave()
{
	m_bStart = false;
	m_nCurrentWave = 0;
	m_nWaitFrames = 999;
	for (auto eachPt : m_spawnPt)
		eachPt = VECTOR3_ZERO;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CWave::~CWave()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CWave::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// 2D�I�u�W�F�N�g�𐶐�
	m_pScene2D = CScene2D::Create(pos, rot, size);
	if (m_pScene2D != NULL)
	{
		m_pScene2D->SetConvertState(false);
		m_pScene2D->SetPriority(4);
	}

	// �ŏ��͉摜�𓧖��ɂ��Ă���
	m_pScene2D->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	// �G�̕����ʒu��ݒ�
	m_spawnPt[0] = WAVE_SPAWN_POINT_1;
	m_spawnPt[1] = WAVE_SPAWN_POINT_2;
	m_spawnPt[2] = WAVE_SPAWN_POINT_3;
	m_spawnPt[3] = WAVE_SPAWN_POINT_4;
	m_spawnPt[4] = WAVE_SPAWN_POINT_5;
	m_spawnPt[5] = WAVE_SPAWN_POINT_6;
	m_spawnPt[6] = WAVE_SPAWN_POINT_7;
	m_spawnPt[7] = WAVE_SPAWN_POINT_8;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CWave::Uninit(void)
{
	// �J������ (scene�̃|�C���^�z�񂩂�폜�j
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CWave::Update(void)
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN)) {
		m_bStart = true;
	}

	if (m_bStart) {
		if (m_nEnemyAlive <= 0 && m_nEnemySpawnCount <= 0) {
			// �G�̎c�蕦�������A���ݐ����Ă�G�����Ȃ��ꍇ
			WaveComplete();	// ���̃E�F�[�u�ɍs��
		}
		SpawnEnemies();		// �G�𕦂�����
		WaveImgWaiter();	// �摜�\�����ԊǗ�
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CWave::Draw(void)
{

}

//=============================================================================
// �����֐�
//=============================================================================
CWave *CWave::Create(void)
{
	// �����A������
	CWave *pWave;
	pWave = new CWave;
	if (pWave != NULL)
	{
		pWave->Init(WAVE_SPRITE_POSITION, VECTOR3_ZERO, WAVE_SPRITE_SIZE);
	}

	return pWave;
}

//=============================================================================
// �E�F�[�u�̉摜��\��
//=============================================================================
void CWave::ShowWaveImg(int waitFrames)
{
	m_nFrameCounter = 0;		// �t���[���J�E���g�����Z�b�g
	m_nWaitFrames = waitFrames;	// �ҋ@�t���[���w��

	switch (m_nCurrentWave + 1)	// �E�F�[�u���̃e�N�X�`���ݒ�
	{
	case 1:
		// 1�E�F�[�u�̃e�N�X�`���ݒ�
		m_pScene2D->BindTexture(CManager::GetTextureLoad()->m_TextureMp["WAVE1"]);
		break;
	case 2:
		// 2�E�F�[�u�̃e�N�X�`���ݒ�
		m_pScene2D->BindTexture(CManager::GetTextureLoad()->m_TextureMp["WAVE2"]);
		break;
	case 3:
		// 3�E�F�[�u�̃e�N�X�`���ݒ�
		m_pScene2D->BindTexture(CManager::GetTextureLoad()->m_TextureMp["WAVE3"]);
		break;
	default:
		break;
	}

	if (m_nCurrentWave >= WAVE_MAX)
		m_pScene2D->BindTexture(CManager::GetTextureLoad()->m_TextureMp["WAVECLEAR"]);
}

//=============================================================================
// �摜�\���̃t���[�����Ǘ�
//=============================================================================
void CWave::WaveImgWaiter(void)
{
	m_nFrameCounter++;
	if (m_nFrameCounter < m_nWaitFrames) {
		m_pScene2D->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// �F������
	}
	else {
		m_pScene2D->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));	// ������
	}
}

//=============================================================================
// ���̃E�F�[�u���J�n����
//=============================================================================
void CWave::WaveNext(void)
{
	// ���̃E�F�[�u�̐ݒ�
	m_nCurrentWave++;
	m_bStart = true;

	switch (m_nCurrentWave)	// �E�F�[�u���̓G�̐�
	{
	case 1:
		// �E�F�[�u�P�̓G�̐�
		m_nEnemySpawnCount = WAVE_1_ENEMIES;
		break;
	case 2:
		// �E�F�[�u�Q�̓G�̐�
		m_nEnemySpawnCount = WAVE_2_ENEMIES;
		break;
	case 3:
		// �E�F�[�u�R�̓G�̐�
		m_nEnemySpawnCount = WAVE_3_ENEMIES;
		break;
	default:
		break;
	}

	// �{�X�E�F�[�u�̏ꍇ
	if (m_nCurrentWave >= WAVE_MAX) {
		m_bBossWave = true;
		m_nEnemySpawnCount = WAVE_BOSS_ENEMIES + m_nEnemySpawnCount;
	}
}

//=============================================================================
// ���݂̃E�F�[�u������������
//=============================================================================
void CWave::WaveComplete(void)
{
	m_bStart = false;

	// �E�F�[�u�N���A�̉摜��\��
	// 1�b���炢�ҋ@���ď���
	ShowWaveImg(WAVE_NEXT_WAITFRAMES);
	if (m_nCurrentWave >= WAVE_MAX) {
		m_bStart = true;
		static int nCountFrame = 0;
		nCountFrame++;
		if (nCountFrame > WAVE_LAST_WAITFRAMES) {
			// ���݂̃E�F�[�u���Ō�̃E�F�[�u�������ꍇ�Q�[���N���A��ʂ�\��������
			nCountFrame = 0;
			CGame::SetGameStatus(CGame::GAMESTATUS_CLEAR);
		}
	}
	else {
		// ���̃E�F�[�u���܂�����ꍇ
		WaveNext();
		CGame::GetLife()->Heal(WAVE_HEAL);
	}
}

//=============================================================================
// �G�𕦂�����
//=============================================================================
void CWave::SpawnEnemies(void)
{
	// ���� �G�̕����c�萔���c���Ă���ꍇ
	if (m_nEnemySpawnCount >= 1) {
			// ���ԑҋ@
			m_nEnemySpawnWait++;
			int nWaitTime = WAVE_SPAWN_WAITTIME;
			if (m_nCurrentWave >= WAVE_MAX)
				nWaitTime = WAVE_SPAWN_WAITTIME_BOSS;
			if (m_nEnemySpawnWait >= nWaitTime) {
				// �����_���ȃX�|�[���ʒu����G�𐶐�
				int nRandSpawn = rand() % WAVE_SPAWNPOINTS;
				int nRandType = rand() % 2;
				if (nRandType == 0)
					CEnemyGunner::Create(m_spawnPt[nRandSpawn], VECTOR3_ZERO, ENEMY_GUNNER_SIZE);
				else
					CEnemyCharger::Create(m_spawnPt[nRandSpawn], VECTOR3_ZERO, ENEMY_CHARGER_SIZE);

				// �G�̐������𑝂₷
				// �G�̕����c�萔�����炷
				m_nEnemyAlive++;
				m_nEnemySpawnCount--;

				m_nEnemySpawnWait = 0;

			if (m_bBossWave) {
				// �{�X����
				CGame::GetBoss()->SpawnBoss();

				m_nEnemyAlive += WAVE_BOSS_ENEMIES;
				m_nEnemySpawnCount -= WAVE_BOSS_ENEMIES;
				m_bBossWave = false;
			}
		}
	}
}