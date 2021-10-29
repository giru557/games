//=============================================================================
//
// ボスの管理 [boss_manager.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "boss_manager.h"
#include "manager.h"
#include "game.h"
#include "input.h"

//*****************************************************************************
// ボス管理クラス ( 継承元: オブジェクトクラス [ scene ] )
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CBossManager::CBossManager()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CBossManager::~CBossManager()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBossManager::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	for (int nCnt = 0; nCnt < 4; nCnt++) {
		m_aChargedShotActivateTime[nCnt] = BOSS_TURRET_ATTACK_ACTIVATETIME * 60;
	}
	m_nBeamAttackActivateTime = BOSS_CORE_BEAMATTACK_ACTIVATETIME * 60;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBossManager::Uninit(void)
{
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBossManager::Update(void)
{
	if (m_bActive) {
		static int nCounterBeamAttack;
		static int aCounterChargedShot[4];

		// 発動中以外カウントアップ
		for (int nCnt = 0; nCnt < 4; nCnt++) {
			if (m_apSceneTurret[nCnt]->GetChargedShotStatus() == false) {
				if (m_apSceneTurret[nCnt]->GetAimingState() == true) {
					aCounterChargedShot[nCnt]++;
				}
			}
		}
		nCounterBeamAttack++;


		// カウントが決まった値を超えたら
		for (int nCnt = 0; nCnt < 4; nCnt++) {
			if (aCounterChargedShot[nCnt] > m_aChargedShotActivateTime[nCnt]) {
				m_apSceneTurret[nCnt]->SetChargedShot();	// Attack
				
				aCounterChargedShot[nCnt] = 0;
				m_aChargedShotActivateTime[nCnt] = BOSS_TURRET_ATTACK_ACTIVATETIME * 60;
			}
		}
		if (nCounterBeamAttack > m_nBeamAttackActivateTime) {
			m_pSceneCore->SetBeamAttack();

			nCounterBeamAttack = 0;
			m_nBeamAttackActivateTime = BOSS_CORE_BEAMATTACK_ACTIVATETIME * 60;
		}

#ifdef _DEBUG
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_M)) {
			m_pSceneCore->SetBeamAttack();
		}
		if (CManager::GetInputKeyboard()->GetPress(DIK_E)) {
			D3DXVECTOR3 coreRot = m_pSceneCore->GetRotDest();
			m_pSceneCore->SetRotDest(D3DXVECTOR3(coreRot.x, coreRot.y, coreRot.z + 0.02f));
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_Q)) {
			D3DXVECTOR3 coreRot = m_pSceneCore->GetRotDest();
			m_pSceneCore->SetRotDest(D3DXVECTOR3(coreRot.x, coreRot.y, coreRot.z - 0.02f));
		}
#endif
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CBossManager::Draw(void)
{

}

//=============================================================================
// 生成関数
//=============================================================================
CBossManager *CBossManager::Create(void)
{
	// 生成、初期化
	CBossManager *pBossManager = new CBossManager;
	if (pBossManager != NULL)
	{
		pBossManager->Init(VECTOR3_ZERO, VECTOR3_ZERO, (D3DXVECTOR2)VECTOR3_ZERO);
	}

	return pBossManager;
}

//=============================================================================
// ボス召喚
//=============================================================================
void CBossManager::SpawnBoss(void)
{
	D3DXVECTOR3 fieldCenter = D3DXVECTOR3(GAME_FIELD_WIDTH / 2, GAME_FIELD_HEIGHT / 2, 0);
	m_pSceneCore = CBossCore::Create(fieldCenter, BOSS_SPAWNCORE_ROTATION);
	m_apSceneTurret[0] = CBossTurret::Create(fieldCenter + D3DXVECTOR3(-BOSS_SPAWNPOS_OFFSET, 0, 0));
	m_apSceneTurret[1] = CBossTurret::Create(fieldCenter + D3DXVECTOR3(0, -BOSS_SPAWNPOS_OFFSET, 0));
	m_apSceneTurret[2] = CBossTurret::Create(fieldCenter + D3DXVECTOR3(BOSS_SPAWNPOS_OFFSET, 0, 0));
	m_apSceneTurret[3] = CBossTurret::Create(fieldCenter + D3DXVECTOR3(0, BOSS_SPAWNPOS_OFFSET, 0));

	m_bActive = true;
}