//=============================================================================
//
// 敵クラス [enemy.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************************************
// ヘッダーのインクルード
//*****************************************************************************
#include "scene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMY_DRAW_PRIORITY (3)
#define ENEMY_MAX (128 )
#define ENEMY_LIFE_DEFAULT (2)
#define ENEMY_SMOOTHTURN_VALUE (0.02f)

#define ENEMY_TRAIL_TICK (5)
#define ENEMY_TRAIL_SIZE (D3DXVECTOR2(15, 15))
#define ENEMY_TRAIL_COLOR (D3DXCOLOR(1.0f, 0.7f, 0.7f, 1.0f))

#define ENEMY_SPEED_DEFAULT (4)
#define ENEMY_SPEED_DECAY (0.03f)

#define ENEMY_SHOOT_SPEED_RANGE (360)
#define ENEMY_SHOOT_SPEED (rand() % ENEMY_SHOOT_SPEED_RANGE)

#define ENEMY_COLLISION_COOLTIME (60)

//*****************************************************************************
// 敵クラス ( 継承元: 2Dポリゴン [scene2d] )
//*****************************************************************************
class CEnemy : public CScene2D
{
public:
	// 敵の種類
	typedef enum
	{
		ENEMY_TYPE_NONE = 0,	// 種類なし
		ENEMY_TYPE_GUNNER,		// ガンナータイプ
		ENEMY_TYPE_CHARGER,		// チャージャータイプ
		ENEMY_TYPE_MAX			// 種類の最大数
	} ENEMY_TYPE;

	// 敵の行動タイプ
	typedef enum
	{
		ENEMY_ACTION_NONE = 0,				// 行動なし
		ENEMY_ACTION_FOLLOW,				// 追尾
		ENEMY_ACTION_FOLLOW_FACEPC,			// 追尾、プレイヤーの方向を向く
		ENEMY_ACTION_FOLLOW_SHOOT,			// 追尾、射撃
		ENEMY_ACTION_FOLLOW_SHOOT_FACEPC,	// 追尾、射撃、プレイヤーの方向を向く
		ENEMY_ACTION_SHOOT,					// 射撃
		ENEMY_ACTION_SHOOT_FACEPC,			// 射撃、プレイヤーの方向を向く
		ENEMY_ACTION_MAX,					// 行動の最大数
	} ENEMY_ACTION;

	CEnemy();
	~CEnemy();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Damage(int nValue);											// ダメージを与える
	void AddForce(D3DXVECTOR3 vec, float fValue);						// 特定の方向に加速させる関数

	void SetRotDest(D3DXVECTOR3 rotDest) { m_rotDest = rotDest; }		// 目標回転値を設定
	D3DXVECTOR3 GetRotDest(void) { return m_rotDest; }					// 目標回転値を取得
	void SetLife(int nValue) { m_nLife = nValue; }						// 体力を設定する
	int GetLife(void) { return m_nLife; }								// 体力を取得
	void SetAction(ENEMY_ACTION enemyAct) { m_act = enemyAct; }			// 行動状態を設定
	ENEMY_ACTION GetAction(void) { return m_act; }						// 行動状態を取得
	bool GetHit(void) { return m_bHit; }								// ヒット状態を取得
	void SetHit(bool bHit) { m_bHit = bHit; }							// ヒット状態を変える
	void ToggleShooting(bool toggle) { m_bToggleShooting = toggle; }	// 射撃のトグル
	void SetTexture(ENEMY_TYPE enemyType);								// テクスチャを設定する

	static void CollideEnemy(CEnemy* pEnemy1, CEnemy* pEnemy2);

protected:
	int m_nLife;			// 体力
	D3DXVECTOR3 m_rotDest;	// 回転の目標値
	bool m_bHit;			// 衝突フラグ

	void CollisionCooltime(void);	// 当たり判定時のクールタイムカウンター
	void FacePlayer(void);			// プレイヤーの方向を向かせる関数
	void SmoothTurn(void);			// 回転を滑らかにする関数
	void CheckScreen(void);			// 画面外判定
	void Dead(void);				// 死亡時処理
	void FixColor(void);			// 色を直す

private:
	float m_fSpeed;			// 敵の移動速度
	float m_fColorGradient;
	D3DXVECTOR3 m_move;		// 移動値,加速度
	int m_nShootSpeed;		// 射撃の早さ
	int m_nShootCounter;	// 射撃用のカウンター
	int m_nHitCounter;		// 判定用カウンター
	bool m_bToggleShooting;	// 射撃状態
	ENEMY_TYPE m_enemyType;	// 敵の種類
	ENEMY_ACTION m_act;		// 行動

	void ActionProc(void);			// 行動の内容を書く関数
	void ShootPlayer(void);			// プレイヤーに対して射撃する関数
	void Trail(void);				// 軌跡の処理
};

#endif
