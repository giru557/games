//=============================================================================
//
// ウェーブクラス [wave.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _WAVE_H_
#define _WAVE_H_

//*****************************************************************************
// ヘッダーのインクルード
//*****************************************************************************
#include "scene2d.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define WAVE_SPRITE_POSITION (D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 300.0f, 0))	// ウェーブの画像を置く場所
#define WAVE_SPRITE_SIZE (D3DXVECTOR2(500, 150))											// ウェーブの画像のサイズ
#define WAVE_HEAL (7)				// ウェーブ終了時の回復量
#define WAVE_MAX (3)				// ウェーブ最大数
#define WAVE_NEXT_WAITFRAMES (180)	// 次のウェーブに行くまでの待機時間
#define WAVE_LAST_WAITFRAMES (240)	// クリア後にリザルト画面に行くまでの待機時間

#define WAVE_1_ENEMIES (40)		// ウェーブ1の敵の数
#define WAVE_2_ENEMIES (80)		// ウェーブ2
#define WAVE_3_ENEMIES (25)		// ウェーブ3
#define WAVE_BOSS_ENEMIES (5)	// ボスの数

#define WAVE_SPAWN_WAITTIME (30)		// 敵のスポーン間隔
#define WAVE_SPAWN_WAITTIME_BOSS (120)	// ボス戦時の敵スポーン間隔
#define WAVE_SPAWNPOINTS (8)			// 敵のスポーン位置の数
#define WAVE_SPAWN_POINT_1 (D3DXVECTOR3(-100.0f, -100.0f, 0.0f))	// 敵スポーン位置
#define WAVE_SPAWN_POINT_2 (D3DXVECTOR3(GAME_FIELD_WIDTH / 2, -100.0f, 0.0f))
#define WAVE_SPAWN_POINT_3 (D3DXVECTOR3(GAME_FIELD_WIDTH + 100.0f, -100.0f, 0.0f))
#define WAVE_SPAWN_POINT_4 (D3DXVECTOR3(-100.0f, GAME_FIELD_HEIGHT / 2, 0.0f))
#define WAVE_SPAWN_POINT_5 (D3DXVECTOR3(GAME_FIELD_WIDTH + 100.0f, GAME_FIELD_HEIGHT / 2, 0.0f))
#define WAVE_SPAWN_POINT_6 (D3DXVECTOR3(-100.0f, GAME_FIELD_HEIGHT + 100.0f, 0.0f))
#define WAVE_SPAWN_POINT_7 (D3DXVECTOR3(GAME_FIELD_WIDTH / 2, GAME_FIELD_HEIGHT + 100.0f, 0.0f))
#define WAVE_SPAWN_POINT_8 (D3DXVECTOR3(GAME_FIELD_WIDTH + 100.0f, GAME_FIELD_HEIGHT + 100.0f, 0.0f))

//*****************************************************************************
// ウェーブクラス ( 派生元: オブジェクトクラス (scene) )
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

	bool m_bStart;			// ウェーブを始めるためのフラグ
	bool m_bImgVisible;		// 画像を表示中かどうか
	bool m_bBossWave;
	int m_nCurrentWave;		// 現在のウェーブ数
	int m_nEnemySpawnCount;	// 現在のウェーブで沸く敵の残り数
	int m_nEnemySpawnWait;	// 敵が沸くまでの待機時間
	int m_nEnemyAlive;		// 現在のウェーブで生存している敵の数
	int m_nWaitFrames;		// 画像表示フレーム
	int m_nFrameCounter;	// フレームカウント用

	D3DXVECTOR3 m_spawnPt[WAVE_SPAWNPOINTS];
	CScene2D *m_pScene2D;
};

#endif

