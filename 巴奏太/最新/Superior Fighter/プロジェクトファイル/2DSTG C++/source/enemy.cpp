//=============================================================================
//
// 敵クラス [enemy.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "enemy.h"
#include "manager.h"
#include "input.h"
#include "bullet.h"
#include "spritesheet.h"
#include "player.h"
#include "life.h"
#include "camera.h"
#include "game.h"
#include "textureloader.h"
#include "sound.h"
#include "wave.h"
#include "particle.h"
#include "effect.h"
#include "orb.h"

//*****************************************************************************
// 敵クラス ( 継承元: 2Dポリゴン [scene2D] )
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CEnemy::CEnemy()
{
	// initialize
	m_nLife = ENEMY_LIFE_DEFAULT;
	m_move = VECTOR3_ZERO;
	m_rotDest = VECTOR3_ZERO;
	m_fSpeed = ENEMY_SPEED_DEFAULT;
	m_enemyType = ENEMY_TYPE_NONE;
	m_act = ENEMY_ACTION_NONE;
	m_nShootSpeed = ENEMY_SHOOT_SPEED;
	m_nShootCounter = 0;
	m_fColorGradient = 1.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CEnemy::~CEnemy()
{
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// 初期化処理
	if (FAILED(CScene2D::Init(pos, rot, size)))
	{
		return E_FAIL;
	}

	SetPriority(ENEMY_DRAW_PRIORITY);

	// オブジェクトの種類を設定
	if (GetObjType() == OBJTYPE_NONE)
	{
		SetObjType(OBJTYPE_ENEMY);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEnemy::Uninit(void)
{
	// 終了
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEnemy::Update(void)
{
	CScene2D::Update();

	// 位置を取得
	D3DXVECTOR3 pos = this->GetPos();

	// 位置の更新と減衰の設定
	m_move.x += (0 - m_move.x) * ENEMY_SPEED_DECAY;
	m_move.y += (0 - m_move.y) * ENEMY_SPEED_DECAY;
	pos += m_move;

	// 位置を反映
	this->SetPos(pos);

	// 現在の行動タイプを元に敵を動かす
	this->ActionProc();

	// 射撃の処理
	this->ShootPlayer();

	// クールタイム処理
	this->CollisionCooltime();

	// 軌跡
	this->Trail();

	// ダメージ色を修正
	this->FixColor();

	// 向き設定
	D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();
	D3DXVECTOR3 relPlayer = playerPos - pos;
	m_rotDest.z = atan2f(relPlayer.x, relPlayer.y) + D3DX_PI;

	// 回転を滑らかにする
	this->SmoothTurn();

	// 画面外は描画しない
	this->CheckScreen();

	// 死亡確認
	if (m_nLife <= 0)
	{
		Dead();
		return;
	}

	UpdateVertex();
}

//=============================================================================
// 描画処理
//=============================================================================
void CEnemy::Draw(void)
{
	// 描画
	CScene2D::Draw();
}

//=============================================================================
// ダメージを与える
//=============================================================================
void CEnemy::Damage(int nValue)
{
	// 画面を揺らす
	CManager::GetCamera()->Shake(3.0f, 5);

	// ダメージ色に染める
	m_fColorGradient = 0.0f;

	// 体力からダメージ分を引く
	m_nLife -= nValue;
}

//=============================================================================
// テクスチャの設定
//=============================================================================
void CEnemy::SetTexture(ENEMY_TYPE enemyType)
{
	CTextureLoad *pTex = CManager::GetTextureLoad();

	switch (enemyType)
	{
	case ENEMY_TYPE_NONE:
		this->BindTexture(pTex->m_TextureMp["ENEMY"]);
		break;

	case ENEMY_TYPE_GUNNER:
		this->BindTexture(pTex->m_TextureMp["ENEMY_GUNNER"]);
		break;

	case ENEMY_TYPE_CHARGER:
		this->BindTexture(pTex->m_TextureMp["ENEMY_CHARGER"]);
		break;
	
	default:
		break;
	}
}

//=============================================================================
// 行動の内容を書く関数
//=============================================================================
void CEnemy::ActionProc(void)
{
	CPlayer *pPlayer = CGame::GetPlayer();
	D3DXVECTOR3 playerPos = pPlayer->GetPos();
	D3DXVECTOR3 playerRot = pPlayer->GetRot();
	D3DXVECTOR3 enemyPos = GetPos();
	D3DXVECTOR3 enemyRot = GetRot();

	ToggleShooting(false);
	switch (m_act)
	{
	case ENEMY_ACTION_NONE:
		// 何もしない
		break;

	case ENEMY_ACTION_FOLLOW:
		// プレイヤーを追尾する、向きは変えない
		m_move.x = sinf(atan2f(playerPos.x - enemyPos.x, playerPos.y - enemyPos.y));
		m_move.y = cosf(atan2f(playerPos.x - enemyPos.x, playerPos.y - enemyPos.y));
		m_move = m_move * m_fSpeed;
		
		break;

	case ENEMY_ACTION_FOLLOW_FACEPC:
		// プレイヤーを追尾する、プレイヤーの方を向く
		this->FacePlayer();
		m_move.x = sinf(enemyRot.z + D3DX_PI);
		m_move.y = cosf(enemyRot.z + D3DX_PI);
		m_move = m_move * PLAYER_SPEED;
		break;

	case ENEMY_ACTION_FOLLOW_SHOOT:
		// プレイヤーを追尾、射撃する。 向きは変わらない
		m_move.x = sinf(atan2f(playerPos.x - enemyPos.x, playerPos.y - enemyPos.y));
		m_move.y = cosf(atan2f(playerPos.x - enemyPos.x, playerPos.y - enemyPos.y));
		m_move = m_move * m_fSpeed;

		break;

	case ENEMY_ACTION_FOLLOW_SHOOT_FACEPC:
		// プレイヤーを追尾、射撃する。 プレイヤーの方を向く
		this->FacePlayer();
		this->ToggleShooting(true);
		m_move.x = sinf(atan2f(playerPos.x - enemyPos.x, playerPos.y - enemyPos.y));
		m_move.y = cosf(atan2f(playerPos.x - enemyPos.x, playerPos.y - enemyPos.y));
		m_move = m_move * m_fSpeed;

		break;

	case ENEMY_ACTION_SHOOT:
		// プレイヤーを射撃する。 向きは変わらない
		this->ToggleShooting(true);

		break;

	case ENEMY_ACTION_SHOOT_FACEPC:
		// プレイヤーを射撃する。 プレイヤーの方を向く
		this->FacePlayer();
		this->ToggleShooting(true);

		break;

	default:
		break;
	}
}

//=============================================================================
// キャラの回転をなめらかにする関数
//=============================================================================
void CEnemy::SmoothTurn(void)
{
	D3DXVECTOR3 RotDiff;				// 目標値と実際の回転の差分
	D3DXVECTOR3 EnemyRot = GetRot();	// 実際の回転値

	//差分計算
	RotDiff.z = m_rotDest.z - EnemyRot.z;

	//差分がD3DX_PI以上(半周以上)の場合、逆回転
	if (RotDiff.z > D3DX_PI)
	{
		EnemyRot.z -= ((D3DX_PI * 2) - RotDiff.z) * ENEMY_SMOOTHTURN_VALUE;
	}
	else if (RotDiff.z < -D3DX_PI)
	{
		EnemyRot.z += ((D3DX_PI * 2) + RotDiff.z) * ENEMY_SMOOTHTURN_VALUE;
	}
	else
	{
		EnemyRot.z += RotDiff.z * ENEMY_SMOOTHTURN_VALUE;
	}

	// 回転の修正 (3.14超えたら±逆に)
	if (EnemyRot.z > D3DX_PI)
	{
		EnemyRot.z -= D3DX_PI * 2.0f;
	}
	else if (EnemyRot.z < -D3DX_PI)
	{
		EnemyRot.z += D3DX_PI * 2.0f;
	}
	if (m_rotDest.z > D3DX_PI)
	{
		m_rotDest.z -= D3DX_PI * 2.0f;
	}
	else if (m_rotDest.z < -D3DX_PI)
	{
		m_rotDest.z += D3DX_PI * 2.0f;
	}

	// 変更した回転を反映させる
	SetRot(EnemyRot);
}

//=============================================================================
// プレイヤーの方向に向きを変える関数
//=============================================================================
void CEnemy::FacePlayer(void)
{
	D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();
	D3DXVECTOR3 enemyPos = GetPos();
	m_rotDest.z = D3DX_PI + atan2f(playerPos.x - enemyPos.x, playerPos.y - enemyPos.y);
}

//=============================================================================
// プレイヤーに対して射撃する関数
//=============================================================================
void CEnemy::ShootPlayer(void)
{
	// 射撃トグルがオンの場合
	if (m_bToggleShooting == true)
	{
		m_nShootCounter++;	// 射撃カウントアップ

		// カウントが指定されたフレーム数を超えたら
		if (m_nShootCounter >= m_nShootSpeed)
		{
			// プレイヤーに対して射撃をする
			D3DXVECTOR3 vecToPlayer = CManager::GetVectorFromPointAtoB(GetPos(), CGame::GetPlayer()->GetPos());
			D3DXVECTOR3 enemyPos = GetPos();
			D3DXVECTOR3 enemyRot = GetRot();
			D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();
			vecToPlayer = vecToPlayer * 5;

			CBullet::Create(GetPos(), D3DXVECTOR3(0.0f, 0.0f, atan2f(playerPos.x - enemyPos.x, playerPos.y - enemyPos.y) - D3DX_PI / 2), vecToPlayer, GetObjType());
			m_nShootCounter = 0;
			m_nShootSpeed = ENEMY_SHOOT_SPEED;
		}
	}
	else
	{
		m_nShootCounter = 0;
	}
}

//=============================================================================
// 当たり判定時のクールタイム
//=============================================================================
void CEnemy::CollisionCooltime(void)
{
	if (m_bHit)
	{
		m_nHitCounter++;
		if (m_nHitCounter > ENEMY_COLLISION_COOLTIME)
		{
			m_bHit = false;
			m_nHitCounter = 0;
		}
	}
}

//=============================================================================
// 特定の方向に加速させる関数
//=============================================================================
void CEnemy::AddForce(D3DXVECTOR3 vec, float fValue)
{
	m_move = m_move + vec * fValue;
}

//=============================================================================
// 軌跡の処理
//=============================================================================
void CEnemy::Trail(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	static int nTickTrail;
	nTickTrail++;
	if (nTickTrail >= ENEMY_TRAIL_TICK) {
		D3DXVECTOR3 trailPos = D3DXVECTOR3(pos.x + sinf(rot.z) * PLAYER_TRAIL_OFFSET, pos.y + cosf(rot.z) * PLAYER_TRAIL_OFFSET, 0.0f);
		CEffect::Create(trailPos, rot, ENEMY_TRAIL_SIZE, ENEMY_TRAIL_COLOR, CEffect::EFFECT_TEX_DEFAULT, PLAYER_TRAIL_SHRINKSPEED, PLAYER_TRAIL_FADESPEED);

		nTickTrail = 0;
	}
}

//=============================================================================
// 死亡時処理
//=============================================================================
void CEnemy::Dead(void)
{
	CSpriteSheet::Create(this->GetPos(), VECTOR3_ZERO, D3DXVECTOR2(EXPLOSION_SIZE_DEFAULT, EXPLOSION_SIZE_DEFAULT), CSpriteSheet::SPRITESHEET_EXPLOSION);	// 爆破
	CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_EXPLOSION00);	// 音
	CGame::GetWave()->EnemyDied(); // ウェーブ側の敵総数
	COrb::Create(GetPos(), VECTOR3_ZERO, ORB_SIZE_DEFAULT, 33);
	CParticle::Create(	// パーティクル
		GetPos(),
		VECTOR3_ZERO,
		D3DXCOLOR(1.0f, 1.0f, 0.4f, 1.0f),
		D3DXVECTOR2(35, 35),
		CEffect::EFFECT_TEX_DEFAULT,
		1,
		4.0f,
		35,
		0.001f,
		0.001f,
		0.0f,
		0.0f);

	Uninit();
}

//=============================================================================
// 画面内か判定して描画
//=============================================================================
void CEnemy::CheckScreen(void)
{
	// 画面外のやつを描画しない
	D3DXVECTOR3 screenPos = CCamera::WorldToScreenPosition(GetPos());
	D3DXVECTOR2 size = GetSize();
	if (screenPos.x > SCREEN_WIDTH + size.x ||
		screenPos.x < 0 - size.x ||
		screenPos.y > SCREEN_HEIGHT + size.y ||
		screenPos.y < 0 - size.y)
		this->SetDrawFlag(false);
	else {
		this->SetDrawFlag(true);
	}
}

//=============================================================================
// 衝突処理
//=============================================================================
void CEnemy::CollideEnemy(CEnemy *pEnemy1, CEnemy *pEnemy2)
{
	// お互いをノックバックさせる
	pEnemy1->AddForce(CManager::GetVectorFromPointAtoB(pEnemy2->GetPos(), pEnemy1->GetPos()), 1.0f);
	pEnemy2->AddForce(CManager::GetVectorFromPointAtoB(pEnemy1->GetPos(), pEnemy2->GetPos()), 1.0f);
}

//=============================================================================
// 色を直す
//=============================================================================
void CEnemy::FixColor(void)
{
	if (m_fColorGradient < 1.0f) {
		m_fColorGradient += 0.1f;
	}
	else if (m_fColorGradient >= 1.0f) {
		m_fColorGradient = 1.0f;
	}
	SetColor(D3DXCOLOR(1.0f, m_fColorGradient, m_fColorGradient, 1.0f));
}