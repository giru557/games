//=============================================================================
//
// �E�F�[�u�N���X [wave.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _WAVE_H_
#define _WAVE_H_

//*****************************************************************************
// �w�b�_�[�̃C���N���[�h
//*****************************************************************************
#include "scene2d.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define WAVE_SPRITE_POSITION (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 300.0f, 0))	// �E�F�[�u�̉摜��u���ꏊ
#define WAVE_SPRITE_SIZE (D3DXVECTOR2(500, 150))											// �E�F�[�u�̉摜�̃T�C�Y
#define WAVE_HEAL (7)				// �E�F�[�u�I�����̉񕜗�
#define WAVE_MAX (3)				// �E�F�[�u�ő吔
#define WAVE_NEXT_WAITFRAMES (180)	// ���̃E�F�[�u�ɍs���܂ł̑ҋ@����
#define WAVE_LAST_WAITFRAMES (240)	// �N���A��Ƀ��U���g��ʂɍs���܂ł̑ҋ@����

#define WAVE_1_ENEMIES (40)		// �E�F�[�u1�̓G�̐�
#define WAVE_2_ENEMIES (80)		// �E�F�[�u2
#define WAVE_3_ENEMIES (25)		// �E�F�[�u3
#define WAVE_BOSS_ENEMIES (5)	// �{�X�̐�

#define WAVE_SPAWN_WAITTIME (30)		// �G�̃X�|�[���Ԋu
#define WAVE_SPAWN_WAITTIME_BOSS (120)	// �{�X�펞�̓G�X�|�[���Ԋu
#define WAVE_SPAWNPOINTS (8)			// �G�̃X�|�[���ʒu�̐�
#define WAVE_SPAWN_POINT_1 (D3DXVECTOR3(-100.0f, -100.0f, 0.0f))	// �G�X�|�[���ʒu
#define WAVE_SPAWN_POINT_2 (D3DXVECTOR3(GAME_FIELD_WIDTH / 2, -100.0f, 0.0f))
#define WAVE_SPAWN_POINT_3 (D3DXVECTOR3(GAME_FIELD_WIDTH + 100.0f, -100.0f, 0.0f))
#define WAVE_SPAWN_POINT_4 (D3DXVECTOR3(-100.0f, GAME_FIELD_HEIGHT / 2, 0.0f))
#define WAVE_SPAWN_POINT_5 (D3DXVECTOR3(GAME_FIELD_WIDTH + 100.0f, GAME_FIELD_HEIGHT / 2, 0.0f))
#define WAVE_SPAWN_POINT_6 (D3DXVECTOR3(-100.0f, GAME_FIELD_HEIGHT + 100.0f, 0.0f))
#define WAVE_SPAWN_POINT_7 (D3DXVECTOR3(GAME_FIELD_WIDTH / 2, GAME_FIELD_HEIGHT + 100.0f, 0.0f))
#define WAVE_SPAWN_POINT_8 (D3DXVECTOR3(GAME_FIELD_WIDTH + 100.0f, GAME_FIELD_HEIGHT + 100.0f, 0.0f))

//*****************************************************************************
// �E�F�[�u�N���X ( �h����: �I�u�W�F�N�g�N���X (scene) )
//*****************************************************************************
class CWave : public CScene
{
public:
	CWave();
	~CWave();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPos(D3DXVECTOR3 pos) {};
	void SetRot(D3DXVECTOR3 rot) {};
	void SetSize(D3DXVECTOR2 size) {};
	D3DXVECTOR3 GetPos(void) { return VECTOR3_ZERO; }
	D3DXVECTOR3 GetRot(void) { return VECTOR3_ZERO; }
	D3DXVECTOR2 GetSize(void) { return (D3DXVECTOR2)VECTOR3_ZERO; }

	void WaveStart(void) { m_bStart = true; }
	void EnemyDied(void) { m_nEnemyAlive--; }
	static CWave *Create(void);

private:
	void ShowWaveImg(int waitFrames);
	void WaveImgWaiter(void);
	void WaveNext(void);
	void WaveComplete(void);
	void SpawnEnemies(void);

	bool m_bStart;			// �E�F�[�u���n�߂邽�߂̃t���O
	bool m_bImgVisible;		// �摜��\�������ǂ���
	bool m_bBossWave;
	int m_nCurrentWave;		// ���݂̃E�F�[�u��
	int m_nEnemySpawnCount;	// ���݂̃E�F�[�u�ŕ����G�̎c�萔
	int m_nEnemySpawnWait;	// �G�������܂ł̑ҋ@����
	int m_nEnemyAlive;		// ���݂̃E�F�[�u�Ő������Ă���G�̐�
	int m_nWaitFrames;		// �摜�\���t���[��
	int m_nFrameCounter;	// �t���[���J�E���g�p

	D3DXVECTOR3 m_spawnPt[WAVE_SPAWNPOINTS];
	CScene2D *m_pScene2D;
};

#endif

