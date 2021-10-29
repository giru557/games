//=============================================================================
//
// ボスタレットの処理 [boss_turret.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "boss_turret.h"
#include "manager.h"
#include "game.h"
#include "textureloader.h"
#include "player.h"
#include "camera.h"
#include "wave.h"
#include "bullet.h"
#include "input.h"
#include "particle.h"
#include "debris.h"

//*****************************************************************************
// ボスタレットクラス ( 継承元: 敵クラス [ enemy ] )
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CBossTurret::CBossTurret()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CBossTurret::~CBossTurret()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBossTurret::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// 初期化
	if (FAILED(CScene2D::Init(pos, rot, size))) {
		return E_FAIL;
	}

	m_nLife = BOSS_TURRET_LIFE;
	SetPriority(ENEMY_DRAW_PRIORITY);
	SetPos(pos);
	SetRot(rot);
	SetSize(BOSS_TURRET_SIZE);
	SetObjType(OBJTYPE_BOSS);
	BindTexture(CManager::GetTextureLoad()->m_TextureMp["BOSSTURRET"]);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBossTurret::Uninit(void)
{
	CDebris::Create(GetPos(), VECTOR3_ZERO, D3DXVECTOR2(150, 150));

	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBossTurret::Update(void)
{
	CScene2D::Update();

	// 位置を取得
	D3DXVECTOR3 pos = this->GetPos();

	// クールタイム処理
	this->CollisionCooltime();

	// チャージショットの処理
	ChargedShot();

	// 索敵
	Detection();

	// 向き設定
	if (m_bAiming) {	// 索敵範囲内にいる場合のみ
		D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();
		D3DXVECTOR3 relPlayer = playerPos - pos;
		m_rotDest.z = atan2f(relPlayer.x, relPlayer.y) + D3DX_PI;
	}

	// ダメージ色を戻す
	FixColor();

	// 回転を滑らかにする
	this->SmoothTurn();

	// 画面外のやつを描画しない
	this->CheckScreen();

	// 死亡確認
	if (m_nLife <= 0)
	{
		Dead();
		Uninit();
		return;
	}

	UpdateVertex();

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_N)) {
		m_bChargeShot = true;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CBossTurret::Draw(void)
{
	CScene2D::Draw();

}

//=============================================================================
// 生成関数
//=============================================================================
CBossTurret *CBossTurret::Create(D3DXVECTOR3 pos)
{
	// 生成、初期化
	CBossTurret *pTurret = new CBossTurret;
	if (pTurret != NULL)
	{
		pTurret->Init(pos, VECTOR3_ZERO, (D3DXVECTOR2)VECTOR3_ZERO);
	}

	return pTurret;
}

//=============================================================================
// 溜め発射攻撃
//=============================================================================
void CBossTurret::ChargedShot(void)
{
	// チャージショットフラグが立っている場合
	if (m_bChargeShot) {
		// チャージ時間カウント開始
		m_nChargeTime++;

		if (m_nChargeTime < BOSS_TURRET_CHARGE_TIME) {
			CParticle::Create(	// Particle
				GetPos() + BOSS_TURRET_OFFSET,
				VECTOR3_ZERO,
				D3DXCOLOR(1, 0, 0, 1),
				D3DXVECTOR2(10, 10),
				CEffect::EFFECT_TEX_DEFAULT,
				1,
				5.0f,
				1,
				0.001f,
				0.001f,
				0,
				0);
		}

		// チャージ完了
		if (m_nChargeTime >= BOSS_TURRET_CHARGE_TIME) {
			// 連射用のカウント
			static int nCntFrame;
			nCntFrame++;
			// 決まった連射速度で
			if (nCntFrame >= BOSS_TURRET_CHARGE_SHOT_SPEED) {
				// プレイヤーに対して射撃をする
				D3DXVECTOR3 vecToPlayer = CManager::GetVectorFromPointAtoB(GetPos(), CGame::GetPlayer()->GetPos());
				D3DXVECTOR3 enemyPos = GetPos();
				D3DXVECTOR3 enemyRot = GetRot();
				D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();
				vecToPlayer = vecToPlayer * BULLET_SPEED_ENEMY;
				CBullet::Create(GetPos() + BOSS_TURRET_OFFSET, D3DXVECTOR3(0.0f, 0.0f, atan2f(playerPos.x - enemyPos.x, playerPos.y - enemyPos.y) - D3DX_PI / 2), D3DXVECTOR2(BULLET_WIDTH_DEFAULT, BULLET_HEIGHT_DEFAULT) * 1.5 , vecToPlayer, OBJTYPE_BOSS);

				// 撃った数カウントアップと連射カウントリセット
				m_nChargeShots++;
				nCntFrame = 0;

				// すべて打ち切った後
				if (m_nChargeShots >= BOSS_TURRET_CHARGE_SHOTS) {
					m_nChargeTime = 0;
					m_nChargeShots = 0;
					m_bChargeShot = false;
				}
			}
		}
	}
}

//=============================================================================
// 索敵範囲
//=============================================================================
void CBossTurret::Detection(void)
{
	if (CManager::GetDistanceAB(CGame::GetPlayer()->GetPos(), this->GetPos()) < BOSS_TURRET_DETECT_DIST) {
		m_bAiming = true;
	}
	else {
		m_bAiming = false;
	}
}