//=============================================================================
//
// 弾の処理 [bullet.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "bullet.h"
#include "manager.h"
#include "spritesheet.h"
#include "enemy.h"
#include "life.h"
#include "score.h"
#include "game.h"
#include "textureloader.h"
#include "sound.h"
#include "particle.h"
#include "effect.h"
#include "boss_turret.h"

//*****************************************************************************
// バレットクラス ( 継承元: 2Dポリゴン [scene2D] )
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CBullet::CBullet()
{
	m_move = VECTOR3_ZERO;
	m_nLife = BULLET_LIFE_DEFAULT;
}

//=============================================================================
// デストラクタ
//=============================================================================
CBullet::~CBullet()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBullet::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size, D3DXVECTOR3 move, CScene::OBJTYPE owner)
{
	// 初期化処理
	if (FAILED(CScene2D::Init(pos, rot, size)))
	{
		E_FAIL;
	}

	// オブジェクトの種類を設定
	// テクスチャの設定
	// 移動値の設定
	// 所有者の設定
	SetObjType(OBJTYPE_BULLET);
	BindTexture(CManager::GetTextureLoad()->m_TextureMp["BULLET"]);
	m_move = move;
	m_owner = owner;
	if (m_owner == OBJTYPE_PLAYER) {
		m_nLife = BULLET_LIFE_PC;
	}
	else if (m_owner == OBJTYPE_ENEMY) {
		this->SetColor(D3DXCOLOR(1, 0.3f, 1, 1.0f));
	}
	else if (m_owner == OBJTYPE_BOSS) {
		this->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}

	CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_LASER00);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBullet::Uninit(void)
{
	CScene2D::Uninit();
}
//=============================================================================
// 更新処理
//=============================================================================
void CBullet::Update(void)
{
	CScene2D::Update();

	// ライフ減少
	m_nLife--;

	// 位置取得
	D3DXVECTOR3 pos = GetPos();

	// 移動量を足す
	pos += m_move;

	// 位置を更新
	SetPos(pos);

	// 頂点を更新
	CScene2D::Update();

	// ライフ0の時
	if (m_nLife <= 0)
	{
		Uninit();
		return;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CBullet::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// 生成関数
//=============================================================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size, D3DXVECTOR3 move, CScene::OBJTYPE owner)
{
	// プレイヤーの生成、初期化
	CBullet *pBullet = NULL;
	pBullet = new CBullet;
	if (pBullet != NULL)
	{
		pBullet->Init(pos, rot, size, move, owner);
	}

	return pBullet;
}
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, CScene::OBJTYPE owner)
{
	// プレイヤーの生成、初期化
	CBullet *pBullet = NULL;
	pBullet = new CBullet;
	if (pBullet != NULL)
	{
		pBullet->Init(pos, rot, D3DXVECTOR2(BULLET_WIDTH_DEFAULT, BULLET_HEIGHT_DEFAULT), move, owner);
	}

	return pBullet;
}

//=============================================================================
// プレイヤーに当たった処理
//=============================================================================
void CBullet::CollidePlayer(void)
{
	CGame::GetLife()->Damage(1);								// ダメージ
	CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_HIT00);	// 音声
	CParticle::Create(											// パーティクル
		GetPos(),
		VECTOR3_ZERO,
		D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f),
		D3DXVECTOR2(25, 25),
		CEffect::EFFECT_TEX_DEFAULT,
		1,
		5.0f,
		35,
		0.007f,
		0.007f,
		D3DXToDegree(GetRot().z - D3DX_PI / 2),
		90.0f);

	Uninit();	// 弾破棄
}

//=============================================================================
// 敵に当たった処理
//=============================================================================
void CBullet::CollideEnemy(CEnemy *pEnemy)
{
	CGame::GetScore()->AddScore(100);							// スコア加算
	pEnemy->Damage(1);	// 敵にダメージ
	CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_HIT03);	// 音再生
	CParticle::Create(											// パーティクル
		GetPos(),
		VECTOR3_ZERO,
		D3DXCOLOR(0.5f, 0.7f, 1.0f, 1.0f),
		D3DXVECTOR2(25, 25),
		CEffect::EFFECT_TEX_DEFAULT,
		1,
		5.0f,
		20,
		0.01f,
		0.01f,
		D3DXToDegree(GetRot().z - D3DX_PI / 2),
		45.0f);

	Uninit();	// 弾破棄
}