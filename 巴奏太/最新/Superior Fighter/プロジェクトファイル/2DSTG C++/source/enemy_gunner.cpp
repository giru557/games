//=============================================================================
//
// 敵派生クラス ガンナー [enemy_gunner.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "enemy_gunner.h"
#include "bullet.h"
#include "player.h"
#include "camera.h"
#include "manager.h"
#include "game.h"
#include "life.h"
#include "particle.h"

//*****************************************************************************
// 敵派生クラス ガンナー ( 継承元: 敵クラス [enemy] )
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CEnemyGunner::CEnemyGunner()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CEnemyGunner::~CEnemyGunner()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CEnemyGunner::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// 初期化処理
	if (FAILED(CEnemy::Init(pos, rot, size)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEnemyGunner::Uninit(void)
{
	CEnemy::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEnemyGunner::Update(void)
{
	//行動処理
	this->ActionProc();

	CEnemy::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CEnemyGunner::Draw(void)
{
	CEnemy::Draw();
}

//=============================================================================
// 生成関数
//=============================================================================
CEnemyGunner *CEnemyGunner::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	CEnemyGunner *pEnemy;
	pEnemy = new CEnemyGunner;
	if (pEnemy != NULL)
	{
		pEnemy->Init(pos, rot, size);
		pEnemy->SetTexture(ENEMY_TYPE_GUNNER);
	}

	return pEnemy;
}

//=============================================================================
// 行動の処理をまとめる関数
//=============================================================================
void CEnemyGunner::ActionProc(void)
{
	D3DXVECTOR3 enemyScreenPos = CCamera::WorldToScreenPosition(GetPos());	// 敵のスクリーン座標
	float fDistEnemyPlayer = CManager::GetDistanceAB(GetPos(), CGame::GetPlayer()->GetPos());

	// 検知範囲より遠い場合
	if (fDistEnemyPlayer >= ENEMY_GUNNER_DETECT_DISTANCE)
	{
		// 距離を詰めるだけの行動をする
		SetAction(ENEMY_ACTION_FOLLOW);
	}
	// 射撃検知範囲より遠い場合
	else if (fDistEnemyPlayer >= ENEMY_GUNNER_DETECT_SHOOT_DISTANCE)
	{
		// 距離を詰めながら射撃してくる
		SetAction(ENEMY_ACTION_FOLLOW_SHOOT);
	}
	else
	{
		// 止まって射撃してくる
		SetAction(ENEMY_ACTION_SHOOT);
	}
}