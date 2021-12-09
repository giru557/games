//=============================================================================
//
// プレイヤー [player.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "manager.h"
#include "bullet.h"
#include "camera.h"
#include "effect.h"
#include "game.h"
#include "spritesheet.h"
#include "life.h"
#include "textureloader.h"
#include "debugger.h"
#include "sound.h"
#include "energy.h"
#include "particle.h"
#include "bg3d.h"

//*****************************************************************************
// プレイヤークラス ( 継承元: 2Dポリゴン [scene2D] )
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CPlayer::CPlayer(int nPriority) : CScene2D(nPriority)
{
	// クリア
	m_move = VECTOR3_ZERO;
	m_rotDest = VECTOR3_ZERO;
	m_bPowered = false;
	m_bToggleShoot = false;
	m_bToggleShoot4 = false;
	m_nShootCounter = 0;
	m_nShootCounter4 = 0;
	m_nShootSpeed = PLAYER_SHOOT_SPEED;
	m_nShootSpeed4 = PLAYER_SHOOT_SPEED4;
	m_fSpeedDecay = PLAYER_SPEED_DECAY;
}

//=============================================================================
// デストラクタ
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// 初期化処理
	if (FAILED(CScene2D::Init(pos, rot, size)))
	{
		E_FAIL;
	}
	
	// テクスチャを設定
	CScene2D::BindTexture(CManager::GetTextureLoad()->m_TextureMp["PLAYER"]);

	// オブジェクトの種類を設定
	SetObjType(OBJTYPE_PLAYER);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	CScene2D::Uninit();
}
//=============================================================================
// 更新処理
//=============================================================================
void CPlayer::Update(void)
{
	CScene2D::Update();

	// 必要な物を取得
	CInputMouse *pMouse = CManager::GetInputMouse();
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR2 cursorPos = pMouse->GetPos();
	D3DXVECTOR2 playerPosScreen = CCamera::WorldToScreenPosition(pos);

		// 位置の更新と減衰の設定
		m_move.x += (0 - m_move.x) * m_fSpeedDecay;
		m_move.y += (0 - m_move.y) * m_fSpeedDecay;
		pos += m_move;

		// 状態、フラグを管理する
		this->StateManage();

		// 動力がある場合
		if (m_bPowered) {
			CPlayer::Move();	// プレイヤーの推進
			this->Trail();		// 軌跡の生成
		}

		// 射撃
		if (m_bToggleShoot)
			this->FullAuto();
		else
			m_nShootCounter = 0;
		if (m_bToggleShoot4)
			this->FullAuto4();
		else
			m_nShootCounter4 = 0;

		// ダッシュ
		if (m_bDash)
			this->Dash();

		// 位置設定
		CScene2D::SetPos(pos);

		// 向き設定
		m_rotDest.z = D3DX_PI + atan2f(cursorPos.x - playerPosScreen.x, cursorPos.y - playerPosScreen.y);

		// 回転を滑らかに
		CPlayer::SmoothTurn();

		// カメラ位置の設定
		CCamera *pCamera = CManager::GetCamera();
		pCamera->SetCameraCenter(GetPos());

		// 死亡確認
		if (CGame::GetLife()->GetLife() <= 0) {
			this->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
			CSpriteSheet::Create(pos, VECTOR3_ZERO, D3DXVECTOR2(300.0f, 300.0f), CSpriteSheet::SPRITESHEET_EXPLOSION2);
			CManager::GetCamera()->Shake(5.0f, 60);
			Uninit();
		}
}

//=============================================================================
// 描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// 生成関数
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos)
{
	// プレイヤーの生成、初期化
	CPlayer *pPlayer = NULL;
	pPlayer = new CPlayer;
	if (pPlayer != NULL)
	{
		pPlayer->Init(pos, D3DXVECTOR3(0, 0, 0), D3DXVECTOR2(SCENE2D_WIDTH_DEFAULT, SCENE2D_HEIGHT_DEFAULT));
	}
	
	return pPlayer;
}

CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// プレイヤーの生成、初期化
	CPlayer *pPlayer = NULL;
	pPlayer = new CPlayer;
	if (pPlayer != NULL)
	{
		pPlayer->Init(pos, rot, size);
	}

	return pPlayer;
}

//=============================================================================
// 移動関数
//=============================================================================
void CPlayer::Move()
{
	// プレイヤーの向きに移動する
	D3DXVECTOR3 rotPlayer = GetRot();
	D3DXVECTOR3 move;
	move.x = sinf(rotPlayer.z + D3DX_PI);
	move.y = cosf(rotPlayer.z + D3DX_PI);
	m_move += move * PLAYER_SPEED;
}

//=============================================================================
// キャラの回転をなめらかにする関数
//=============================================================================
void CPlayer::SmoothTurn(void)
{
	D3DXVECTOR3 RotDiff;				// 目標値と実際の回転の差分
	D3DXVECTOR3 PlayerRot = GetRot();	// 実際の回転値
	float fTurnValue = PLAYER_SMOOTHTURN_VALUE;
	if (m_bDash) fTurnValue = PLAYER_SMOOTHTURN_VALUE_DASH;

	//差分計算
	RotDiff.z = m_rotDest.z - PlayerRot.z;

	//差分がD3DX_PI以上(半周以上)の場合、逆回転
	if (RotDiff.z > D3DX_PI)
		PlayerRot.z -= ((D3DX_PI * 2) - RotDiff.z) * fTurnValue;
	else if (RotDiff.z < -D3DX_PI)
		PlayerRot.z += ((D3DX_PI * 2) + RotDiff.z) * fTurnValue;
	else
		PlayerRot.z += RotDiff.z * fTurnValue;

	// 回転の修正
	if (PlayerRot.z > D3DX_PI)
		PlayerRot.z -= D3DX_PI * 2.0f;
	else if (PlayerRot.z < -D3DX_PI)
		PlayerRot.z += D3DX_PI * 2.0f;
	if (m_rotDest.z > D3DX_PI)
		m_rotDest.z -= D3DX_PI * 2.0f;
	else if (m_rotDest.z < -D3DX_PI)
		m_rotDest.z += D3DX_PI * 2.0f;

	// 変更した回転を反映させる
	SetRot(PlayerRot);
}

//=============================================================================
// 軌跡の処理
//=============================================================================
void CPlayer::Trail(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	D3DXVECTOR3 trailPos = D3DXVECTOR3(pos.x + sinf(rot.z) * PLAYER_TRAIL_OFFSET, pos.y + cosf(rot.z) * PLAYER_TRAIL_OFFSET, 0.0f);
	if (m_bDash) {
		// ダッシュ時は軌跡を大きく赤くする
		CEffect::Create(trailPos, rot, PLAYER_TRAIL_SIZE_DASH, PLAYER_TRAIL_COLOR_DASH, CEffect::EFFECT_TEX_DEFAULT, PLAYER_TRAIL_SHRINKSPEED, PLAYER_TRAIL_FADESPEED);
	}
	else {
		// 通常時は青
		CEffect::Create(trailPos, rot, PLAYER_TRAIL_SIZE, PLAYER_TRAIL_COLOR, CEffect::EFFECT_TEX_DEFAULT, PLAYER_TRAIL_SHRINKSPEED, PLAYER_TRAIL_FADESPEED);
	}
	
}

//=============================================================================
// 特定の方向に加速させる関数
//=============================================================================
void CPlayer::AddForce(D3DXVECTOR3 vec, float fValue)
{
	m_move += vec * fValue;
}

//=============================================================================
// 弾の発射
//=============================================================================
void CPlayer::Shoot(int nType)
{
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 pos = GetPos();

	if (nType == 0) {
		// プレイヤーの向きに弾を発射
		D3DXVECTOR3 bulletMove;

		bulletMove.x = sinf(rot.z + D3DX_PI);
		bulletMove.y = cosf(rot.z + D3DX_PI);
		bulletMove = bulletMove * BULLET_SPEED_DEFAULT;

		// インスタンス生成
		CBullet::Create(pos, D3DXVECTOR3(0.0f, 0.0f, rot.z + D3DX_PI / 2), bulletMove, CScene::OBJTYPE_PLAYER);
	}
	else if (nType == 1) {
		// 4方向の角度
		float fRadAngBullet[4];
		fRadAngBullet[0] = rot.z + D3DXToRadian(45.0f);
		fRadAngBullet[1] = rot.z + D3DXToRadian(-45.0f);
		fRadAngBullet[2] = rot.z + D3DXToRadian(45.0f) + D3DX_PI;
		fRadAngBullet[3] = rot.z + D3DXToRadian(-45.0f) + D3DX_PI;

		for (int nCntBullet = 0; nCntBullet < 4; nCntBullet++) {
			// 角度を元に発射
			D3DXVECTOR3 moveBullet = D3DXVECTOR3(sinf(fRadAngBullet[nCntBullet]), cosf(fRadAngBullet[nCntBullet]), 0) * BULLET_SPEED_DEFAULT;
			CBullet::Create(pos, D3DXVECTOR3(0.0f, 0.0f, fRadAngBullet[nCntBullet] - D3DX_PI / 2), moveBullet, OBJTYPE_PLAYER);
		}
	}

}

//=============================================================================
// 連射用関数
//=============================================================================
void CPlayer::FullAuto(void)
{
	// カウント溜まったら発射
	m_nShootCounter++;
	if (m_nShootCounter > m_nShootSpeed)
	{
		// エネルギーが足りない場合撃てない
		if (CGame::GetEnergy()->UseEnergy(2)) {
			this->Shoot(0);
			m_nShootCounter = 0;
		}
	}
}

//=============================================================================
// 4方向射撃連射用
//=============================================================================
void CPlayer::FullAuto4(void)
{
	// カウント溜まったら発射
	m_nShootCounter4++;
	if (m_nShootCounter4 > m_nShootSpeed4) {
		// エネルギー足りてないと打てない
		if (CGame::GetEnergy()->UseEnergy(10)) {
			this->Shoot(1);

			m_nShootCounter4 = 0;
		}
	}
}

//=============================================================================
// キャラのダッシュ
//=============================================================================
void CPlayer::Dash(void)
{
	D3DXVECTOR3 rot = GetRot();
	
	// ダッシュ時のカウントが溜まったらダッシュフラグを切る
	static int nDashCounter;
	nDashCounter++;
	if (nDashCounter > PLAYER_DASH_FRAMES) {
		m_bDash = false;
		nDashCounter = 0;
	}

	// ダッシュ時にする処理
	CManager::GetCamera()->Shake(2.0f, PLAYER_DASH_FRAMES);
	D3DXVECTOR3 dirForward = D3DXVECTOR3(sinf(rot.z + D3DX_PI), cosf(rot.z + D3DX_PI), 0.0f);
	AddForce(dirForward, PLAYER_SPEED_DASH);
}

//=============================================================================
// 状態、フラグの管理
//=============================================================================
void CPlayer::StateManage(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CInputMouse *pMouse = CManager::GetInputMouse();

	// Shiftを押されている間エンジンを切る
	m_bPowered = true;
	if (!m_bDash) {
		if (pKeyboard->GetPress(DIK_LSHIFT)) m_bPowered = false;
	}

	// クリックしている間、射撃トグルをONにする
	m_bToggleShoot = false;
	m_bToggleShoot4 = false;
	if (pMouse->GetPress(CInputMouse::MOUSESTATE_LCLICK)) m_bToggleShoot = true;
	if (pMouse->GetPress(CInputMouse::MOUSESTATE_RCLICK)) m_bToggleShoot4 = true;

	// スペースキーでダッシュを有効にする
	if (m_bPowered) {
		if (pKeyboard->GetTrigger(DIK_SPACE)) {
			if (GetDashState() == false) {
				if (CGame::GetEnergy()->UseEnergy(20)) {
					m_bDash = true;

					// ダッシュ時の効果
					CManager::GetRenderer()->SetFeedbackEffect(PLAYER_DASH_FRAMES);
					CGame::GetBG3D()->SetRippleFrequency(0.07f, PLAYER_DASH_FRAMES);
					CGame::GetBG3D()->SetRippleAmplitude(25.0f, PLAYER_DASH_FRAMES);
					CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_DASH00);
				}
			}
		}
	}

	// エンジンを切っている時は移動が減衰しないようにする
	if (m_bPowered) m_fSpeedDecay = PLAYER_SPEED_DECAY;
	else m_fSpeedDecay = PLAYER_SPEED_DECAY_OFF;
}

//=============================================================================
// 敵に衝突時
//=============================================================================
void CPlayer::CollideEnemy(CEnemy *pEnemy)
{
	if (GetDashState() == false) {	// プレイヤーがダッシュ中でない場合
		AddForce(CManager::GetVectorFromPointAtoB(pEnemy->GetPos(), GetPos()), 5.0f);	// ノックバック
		if (!pEnemy->GetHit()) {
			// 衝突クールダウンが解消されている場合

			CGame::GetLife()->Damage(1);									// プレイヤーにダメージ
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_IMPACT00);	// 効果音
			CGame::GetBG3D()->SetRippleFrequency(0.07f, 1);					// 波紋エフェクト
			CParticle *pParticle = CParticle::Create(						// パーティクル
				GetPos(),
				true,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				D3DXVECTOR2(40, 40),
				CEffect::EFFECT_TEX_PARTS1,
				1,
				7.0f,
				5,
				0.005f,
				0.01f,
				0.0f,
				0.0f);
			pParticle->SetPriority(4);
			pParticle->SetRenderState(D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA);

			pEnemy->SetHit(true);	// 衝突クールダウンを設定
		}
	}
	else {
		// プレイヤーがダッシュ中

		if (!pEnemy->GetHit()) {
			// 衝突クールダウンが解消されている場合

			CParticle::Create(	// 突撃パーティクル
				GetPos() + D3DXVECTOR3(sinf(GetRot().z + D3DX_PI), cosf(GetRot().z + D3DX_PI), 0) * 50,
				VECTOR3_ZERO,
				D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f),
				D3DXVECTOR2(30, 30),
				CEffect::EFFECT_TEX_DEFAULT,
				1,
				10.0f,
				20,
				0.01f,
				0.01f,
				D3DXToDegree(GetRot().z),
				160);
			pEnemy->Damage(1);				// 敵にダメージ

			pEnemy->SetHit(true);	// 衝突クールダウン設定
		}
	}
}
