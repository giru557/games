//=============================================================================
//
// ボスタレットの処理 [boss_turret.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _BOSS_TURRET_H_
#define _BOSS_TURRET_H_

//*****************************************************************************
// ヘッダーのインクルード
//*****************************************************************************
#include "main.h"
#include "scene2d.h"
#include "enemy.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BOSS_TURRET_ATTACK_ACTIVATETIME (rand() % 5 + 5)
#define BOSS_TURRET_OFFSET (D3DXVECTOR3(sinf(GetRot().z + D3DX_PI), cosf(GetRot().z + D3DX_PI), 0.0f) * 100.0f)
#define BOSS_TURRET_SIZE (D3DXVECTOR2(200, 200))
#define BOSS_TURRET_LIFE (25)
#define BOSS_TURRET_DETECT_DIST (700)
#define BOSS_TURRET_CHARGE_TIME (60)
#define BOSS_TURRET_CHARGE_SHOTS (3)
#define BOSS_TURRET_CHARGE_SHOT_SPEED (10)

//*****************************************************************************
// ボスタレットクラス ( 継承元: 敵クラス [ enemy ])
//*****************************************************************************
class CBossTurret : public CEnemy
{
public:
	CBossTurret();
	~CBossTurret();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetChargedShot() { m_bChargeShot = true; }
	bool GetChargedShotStatus(void) { return m_bChargeShot; }
	bool GetAimingState(void) { return m_bAiming; }
	static CBossTurret *Create(D3DXVECTOR3 pos);

private:
	void ChargedShot();
	void Detection();

	int m_nChargeTime;
	int m_nChargeShots;
	bool m_bChargeShot;
	bool m_bAiming;
};

#endif