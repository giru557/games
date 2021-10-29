//=============================================================================
//
// ウェーブクラス [wave.cpp]
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
// 静的メンバ変数
//*****************************************************************************

//*****************************************************************************
// ウェーブクラス ( 派生元: オブジェクトクラス (scene) )
//*****************************************************************************

//=============================================================================
// コンストラクタ
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
// デストラクタ
//=============================================================================
CWave::~CWave()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CWave::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// 2Dオブジェクトを生成
	m_pScene2D = CScene2D::Create(pos, rot, size);
	if (m_pScene2D != NULL)
	{
		m_pScene2D->SetConvertState(false);
		m_pScene2D->SetPriority(4);
	}

	// 最初は画像を透明にしておく
	m_pScene2D->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	// 敵の沸き位置を設定
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
// 終了処理
//=============================================================================
void CWave::Uninit(void)
{
	// 開放処理 (sceneのポインタ配列から削除）
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CWave::Update(void)
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN)) {
		m_bStart = true;
	}

	if (m_bStart) {
		if (m_nEnemyAlive <= 0 && m_nEnemySpawnCount <= 0) {
			// 敵の残り沸き数も、現在生きてる敵もいない場合
			WaveComplete();	// 次のウェーブに行く
		}
		SpawnEnemies();		// 敵を沸かせる
		WaveImgWaiter();	// 画像表示時間管理
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CWave::Draw(void)
{

}

//=============================================================================
// 生成関数
//=============================================================================
CWave *CWave::Create(void)
{
	// 生成、初期化
	CWave *pWave;
	pWave = new CWave;
	if (pWave != NULL)
	{
		pWave->Init(WAVE_SPRITE_POSITION, VECTOR3_ZERO, WAVE_SPRITE_SIZE);
	}

	return pWave;
}

//=============================================================================
// ウェーブの画像を表示
//=============================================================================
void CWave::ShowWaveImg(int waitFrames)
{
	m_nFrameCounter = 0;		// フレームカウントをリセット
	m_nWaitFrames = waitFrames;	// 待機フレーム指定

	switch (m_nCurrentWave + 1)	// ウェーブ毎のテクスチャ設定
	{
	case 1:
		// 1ウェーブのテクスチャ設定
		m_pScene2D->BindTexture(CManager::GetTextureLoad()->m_TextureMp["WAVE1"]);
		break;
	case 2:
		// 2ウェーブのテクスチャ設定
		m_pScene2D->BindTexture(CManager::GetTextureLoad()->m_TextureMp["WAVE2"]);
		break;
	case 3:
		// 3ウェーブのテクスチャ設定
		m_pScene2D->BindTexture(CManager::GetTextureLoad()->m_TextureMp["WAVE3"]);
		break;
	default:
		break;
	}

	if (m_nCurrentWave >= WAVE_MAX)
		m_pScene2D->BindTexture(CManager::GetTextureLoad()->m_TextureMp["WAVECLEAR"]);
}

//=============================================================================
// 画像表示のフレーム数管理
//=============================================================================
void CWave::WaveImgWaiter(void)
{
	m_nFrameCounter++;
	if (m_nFrameCounter < m_nWaitFrames) {
		m_pScene2D->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// 色初期化
	}
	else {
		m_pScene2D->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));	// 透明化
	}
}

//=============================================================================
// 次のウェーブを開始する
//=============================================================================
void CWave::WaveNext(void)
{
	// 次のウェーブの設定
	m_nCurrentWave++;
	m_bStart = true;

	switch (m_nCurrentWave)	// ウェーブ毎の敵の数
	{
	case 1:
		// ウェーブ１の敵の数
		m_nEnemySpawnCount = WAVE_1_ENEMIES;
		break;
	case 2:
		// ウェーブ２の敵の数
		m_nEnemySpawnCount = WAVE_2_ENEMIES;
		break;
	case 3:
		// ウェーブ３の敵の数
		m_nEnemySpawnCount = WAVE_3_ENEMIES;
		break;
	default:
		break;
	}

	// ボスウェーブの場合
	if (m_nCurrentWave >= WAVE_MAX) {
		m_bBossWave = true;
		m_nEnemySpawnCount = WAVE_BOSS_ENEMIES + m_nEnemySpawnCount;
	}
}

//=============================================================================
// 現在のウェーブを完了させる
//=============================================================================
void CWave::WaveComplete(void)
{
	m_bStart = false;

	// ウェーブクリアの画像を表示
	// 1秒くらい待機して消す
	ShowWaveImg(WAVE_NEXT_WAITFRAMES);
	if (m_nCurrentWave >= WAVE_MAX) {
		m_bStart = true;
		static int nCountFrame = 0;
		nCountFrame++;
		if (nCountFrame > WAVE_LAST_WAITFRAMES) {
			// 現在のウェーブが最後のウェーブだった場合ゲームクリア画面を表示させる
			nCountFrame = 0;
			CGame::SetGameStatus(CGame::GAMESTATUS_CLEAR);
		}
	}
	else {
		// 次のウェーブがまだある場合
		WaveNext();
		CGame::GetLife()->Heal(WAVE_HEAL);
	}
}

//=============================================================================
// 敵を沸かせる
//=============================================================================
void CWave::SpawnEnemies(void)
{
	// もし 敵の沸き残り数が残っている場合
	if (m_nEnemySpawnCount >= 1) {
			// 時間待機
			m_nEnemySpawnWait++;
			int nWaitTime = WAVE_SPAWN_WAITTIME;
			if (m_nCurrentWave >= WAVE_MAX)
				nWaitTime = WAVE_SPAWN_WAITTIME_BOSS;
			if (m_nEnemySpawnWait >= nWaitTime) {
				// ランダムなスポーン位置から敵を生成
				int nRandSpawn = rand() % WAVE_SPAWNPOINTS;
				int nRandType = rand() % 2;
				if (nRandType == 0)
					CEnemyGunner::Create(m_spawnPt[nRandSpawn], VECTOR3_ZERO, ENEMY_GUNNER_SIZE);
				else
					CEnemyCharger::Create(m_spawnPt[nRandSpawn], VECTOR3_ZERO, ENEMY_CHARGER_SIZE);

				// 敵の生存数を増やす
				// 敵の沸き残り数を減らす
				m_nEnemyAlive++;
				m_nEnemySpawnCount--;

				m_nEnemySpawnWait = 0;

			if (m_bBossWave) {
				// ボス召喚
				CGame::GetBoss()->SpawnBoss();

				m_nEnemyAlive += WAVE_BOSS_ENEMIES;
				m_nEnemySpawnCount -= WAVE_BOSS_ENEMIES;
				m_bBossWave = false;
			}
		}
	}
}