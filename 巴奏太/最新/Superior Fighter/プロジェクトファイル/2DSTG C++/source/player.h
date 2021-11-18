//=============================================================================
//
// プレイヤー [player.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************************************
// ヘッダーのインクルード
//*****************************************************************************
#include "main.h"
#include "scene2D.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CEnemy;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_WIDTH (75.0f)
#define PLAYER_HEIGHT (75.0f)
#define PLAYER_DASH_FRAMES (50)
#define PLAYER_DRAW_PRIORITY (3)
#define PLAYER_SMOKE_FREQ (60)

#define PLAYER_SHOOT_SPEED (5)
#define PLAYER_SHOOT_SPEED4 (15)

#define PLAYER_SMOOTHTURN_VALUE (0.05f)
#define PLAYER_SMOOTHTURN_VALUE_DASH (0.02f)

#define PLAYER_SPEED (2)
#define PLAYER_SPEED_DASH (3)
#define PLAYER_SPEED_DECAY (0.25f)
#define PLAYER_SPEED_DECAY_OFF (0.01f)

#define PLAYER_TRAIL_SIZE (D3DXVECTOR2(30.0f, 30.0f))
#define PLAYER_TRAIL_SIZE_DASH (D3DXVECTOR2(60.0f, 60.0f))
#define PLAYER_TRAIL_OFFSET (30.0f)
#define PLAYER_TRAIL_COLOR (D3DXCOLOR(0.0f, 0.8f, 0.8f, 1.0f))
#define PLAYER_TRAIL_COLOR_DASH (D3DXCOLOR(1.0f, 0.5f, 0.15f, 1.0f))
#define PLAYER_TRAIL_SHRINKSPEED (0.005f)
#define PLAYER_TRAIL_FADESPEED (0.001f)

//*****************************************************************************
// プレイヤークラス ( 継承元: 2Dポリゴン [scene2D] )
//*****************************************************************************
class CPlayer : public CScene2D
{
public:
	CPlayer(int nPriority = 4);
	~CPlayer();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Shoot(int nType);
	void AddForce(D3DXVECTOR3 vec, float fValue);
	bool GetDashState(void) { return m_bDash; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void CollideEnemy(CEnemy *pEnemy);

	static CPlayer *Create(D3DXVECTOR3 pos);
	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);

private:
	void StateManage(void);			// 状態、フラグ管理
	void Dash(void);				// ダッシュ
	void Move(void);				// プレイヤーの移動
	void SmoothTurn(void);			// 回転を滑らかにする関数
	void FullAuto(void);			// 連射用の関数
	void FullAuto4(void);			// 4方向射撃連射用
	void Trail(void);				// 軌跡を作る

	D3DXVECTOR3 m_move;		// プレイヤーの移動値
	D3DXVECTOR3 m_rotDest;	// プレイヤーの回転目標値
	bool m_bPowered;		// 推進力が与えられているか
	bool m_bToggleShoot;	// 射撃の切り替え
	bool m_bToggleShoot4;	// 4方向射撃の切り替え
	bool m_bDash;			// ダッシュ状態
	int m_nShootSpeed;		// 射撃スピード
	int m_nShootCounter;	// 射撃用カウンター
	int m_nShootSpeed4;		// 4方向射撃スピード
	int m_nShootCounter4;	// 4方向射撃カウンター
	float m_fSpeedDecay;	// 速度減衰値
};

#endif
