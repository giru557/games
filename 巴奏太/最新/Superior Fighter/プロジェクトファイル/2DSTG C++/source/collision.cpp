//=============================================================================
//
// 当たり判定 [collision.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "collision.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "life.h"
#include "particle.h"
#include "bullet.h"
#include "sound.h"
#include "score.h"
#include "beam.h"

//*****************************************************************************
// 当たり判定クラス
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CCollision::CCollision()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CCollision::~CCollision()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
void CCollision::Init()
{

}

//=============================================================================
// 終了処理
//=============================================================================
void CCollision::Uninit(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void CCollision::Update(void)
{
	// コンテナ初期化
	m_vecBullet.clear();
	m_vecEnemy.clear();
	m_vecBeam.clear();
	m_vecBeamBase.clear();
	m_vecBoss.clear();
	m_vecBossCore.clear();
	m_vecDebris.clear();

	// すべてのオブジェクトを検索して対応するコンテナに入れる
	for (int nCntPriority = 0; nCntPriority < SCENE_PRIORITY_MAX; nCntPriority++) {
		auto *vectorScene = CScene::GetSceneList(nCntPriority);
		for (int nCntScene = 0; nCntScene < (signed)vectorScene->size(); nCntScene++) {

			// 種類判定
			if ((*vectorScene)[nCntScene]->GetObjType() == CScene::OBJTYPE_ENEMY) {
				m_vecEnemy.push_back((*vectorScene)[nCntScene]);
			}
			else if ((*vectorScene)[nCntScene]->GetObjType() == CScene::OBJTYPE_BULLET) {
				m_vecBullet.push_back((*vectorScene)[nCntScene]);
			}
			else if ((*vectorScene)[nCntScene]->GetObjType() == CScene::OBJTYPE_BEAM) {
				m_vecBeam.push_back((*vectorScene)[nCntScene]);
			}
			else if ((*vectorScene)[nCntScene]->GetObjType() == CScene::OBJTYPE_BEAMBASE) {
				m_vecBeamBase.push_back((*vectorScene)[nCntScene]);
			}
			else if ((*vectorScene)[nCntScene]->GetObjType() == CScene::OBJTYPE_BOSS) {
				m_vecBoss.push_back((*vectorScene)[nCntScene]);
			}
			else if ((*vectorScene)[nCntScene]->GetObjType() == CScene::OBJTYPE_BOSSCORE) {
				m_vecBossCore.push_back((*vectorScene)[nCntScene]);
			}
			else if ((*vectorScene)[nCntScene]->GetObjType() == CScene::OBJTYPE_DEBRIS) {
				m_vecDebris.push_back((*vectorScene)[nCntScene]);
			}

		}
	}

	// 敵の当たり判定
	CollideEnemy();

	// ボスの当たり判定
	CollideBoss();
	CollideDebris();

	// 弾の当たり判定
	CollideBullet();

	// ビームの当たり判定
	CollideBeam();
}

//=============================================================================
// 敵の当たり判定
//=============================================================================
void CCollision::CollideEnemy(void)
{
	CEnemy *pEnemy[2];
	for (int nCntEnemy1 = 0; nCntEnemy1 < (signed)m_vecEnemy.size(); nCntEnemy1++) {
		pEnemy[0] = dynamic_cast<CEnemy*>(m_vecEnemy[nCntEnemy1]);

		// すべての敵同士を判定
		for (int nCntEnemy2 = 0; nCntEnemy2 < (signed)m_vecEnemy.size(); nCntEnemy2++) {

			pEnemy[1] = dynamic_cast<CEnemy*>(m_vecEnemy[nCntEnemy2]);
			if (CollisionObject(pEnemy[0], pEnemy[1])) {
				CEnemy::CollideEnemy(pEnemy[0], pEnemy[1]);
			}
		}

		// プレイヤーとの判定
		CPlayer *pPlayer = CGame::GetPlayer();
		if (CollisionObject(pEnemy[0], pPlayer)) {
			pPlayer->CollideEnemy(pEnemy[0]);
		}
	}
}

//=============================================================================
// ボスの当たり判定
//=============================================================================
void CCollision::CollideBoss(void)
{
	// タレットのプレイヤーとの判定
	CPlayer *pPlayer = CGame::GetPlayer();
	for (int nCntBoss = 0; nCntBoss < (signed)m_vecBoss.size(); nCntBoss++) {
		if (CollisionObject(dynamic_cast<CScene2D*>(m_vecBoss[nCntBoss]), pPlayer, D3DXVECTOR2(120, 120), pPlayer->GetSize())) {
			pPlayer->CollideEnemy(dynamic_cast<CEnemy*>(m_vecBoss[nCntBoss]));
			break;
		}
	}

	// コアのプレイヤーとの判定
	for (int nCntBossC = 0; nCntBossC < (signed)m_vecBossCore.size(); nCntBossC++){
		if (CollisionObject(dynamic_cast<CScene2D*> (m_vecBossCore[nCntBossC]), pPlayer, D3DXVECTOR2(120, 120), pPlayer->GetSize())) {
			pPlayer->CollideEnemy(dynamic_cast<CEnemy*> (m_vecBossCore[nCntBossC]));
		}
	}
}

//=============================================================================
// 弾の当たり判定
//=============================================================================
void CCollision::CollideBullet(void)
{
	for (int nCntBullet = 0; nCntBullet < (signed)m_vecBullet.size(); nCntBullet++) {
		CBullet *pBullet = dynamic_cast<CBullet*>(m_vecBullet[nCntBullet]);

		// 弾の所有者が敵の場合
		if (pBullet->GetOwner() == CScene::OBJTYPE_ENEMY ||
			pBullet->GetOwner() == CScene::OBJTYPE_BOSS) {

			// プレイヤーとの判定
			CPlayer *pPlayer = CGame::GetPlayer();
			if (CollisionObject(pPlayer, pBullet, pPlayer->GetSize(), D3DXVECTOR2(70, 70))) {
				pBullet->CollidePlayer();
			}

		}
		// 弾の所有者がプレイヤーの場合
		else if (pBullet->GetOwner() == CScene::OBJTYPE_PLAYER) {

			// 敵との判定
			for (int nCntEnemy = 0; nCntEnemy < (signed)m_vecEnemy.size(); nCntEnemy++) {
				if (CollisionObject(dynamic_cast<CEnemy*>(m_vecEnemy[nCntEnemy]), pBullet, dynamic_cast<CEnemy*>(m_vecEnemy[nCntEnemy])->GetSize(), D3DXVECTOR2(70, 70))) {
					pBullet->CollideEnemy(dynamic_cast<CEnemy*>(m_vecEnemy[nCntEnemy]));
				}
			}

			// ボスとの判定
			for (int nCntBoss = 0; nCntBoss < (signed)m_vecBoss.size(); nCntBoss++) {
				if (CollisionObject(dynamic_cast<CEnemy*>(m_vecBoss[nCntBoss]), pBullet, D3DXVECTOR2(120, 120), D3DXVECTOR2(70, 70))) {
					pBullet->CollideEnemy(dynamic_cast<CEnemy*>(m_vecBoss[nCntBoss]));
				}
			}
			for (int nCntBossC = 0; nCntBossC < (signed)m_vecBossCore.size(); nCntBossC++) {
				if (CollisionObject(dynamic_cast<CScene2D*>(m_vecBossCore[nCntBossC]), pBullet, D3DXVECTOR2(120, 120), D3DXVECTOR2(70, 70))) {
					pBullet->CollideEnemy(dynamic_cast<CEnemy*>(m_vecBossCore[nCntBossC]));
				}
			}
		}

	}

}

//=============================================================================
// ビームの当たり判定
//=============================================================================
void CCollision::CollideBeam(void)
{
	for (int nCntBeam = 0; nCntBeam < (signed)m_vecBeam.size(); nCntBeam++) {

		// プレイヤーとの判定
		CPlayer *pPlayer = CGame::GetPlayer();
		CScene2D *pBeam = dynamic_cast<CScene2D*>(m_vecBeam[nCntBeam]);

		// 射程距離内か判定
		if (CManager::GetDistanceAB(pPlayer->GetPos(), pBeam->GetPos()) < pBeam->GetSize().y / 2) {

			D3DXVECTOR2 beamVec = D3DXVECTOR2(sinf(pBeam->GetRot().z), cosf(pBeam->GetRot().z));	// ビームの向き
			D3DXVECTOR2 vecToPts[4];																// ビームからプレイヤーの4点への向き

			// 位置の設定、ベクトルの計算
			D3DXVECTOR2 sizeP = pPlayer->GetSize();
			vecToPts[0] = (D3DXVECTOR2)(pPlayer->GetPos() + D3DXVECTOR3(-sizeP.x / 2, sizeP.y / 2, 0));
			vecToPts[1] = (D3DXVECTOR2)(pPlayer->GetPos() + D3DXVECTOR3(-sizeP.x / 2, -sizeP.y / 2, 0));
			vecToPts[2] = (D3DXVECTOR2)(pPlayer->GetPos() + D3DXVECTOR3(sizeP.x / 2, sizeP.y / 2, 0));
			vecToPts[3] = (D3DXVECTOR2)(pPlayer->GetPos() + D3DXVECTOR3(sizeP.x / 2, -sizeP.y / 2, 0));
			for (int nCntPts = 0; nCntPts < 4; nCntPts++) {
				vecToPts[nCntPts] = (D3DXVECTOR2)CManager::GetVectorFromPointAtoB((D3DXVECTOR3)pBeam->GetPos(), (D3DXVECTOR3)vecToPts[nCntPts]);
			}

			// 4点すべてがベクトルの 右/左 にある場合は当たっていない。右左は2D外積を利用して計算する
			int nLeftCount = 0;
			for (int nCntPts = 0; nCntPts < 4; nCntPts++) {
				if (D3DXVec2CCW(&beamVec, &vecToPts[nCntPts]) > 0) {
					nLeftCount++;
				}
			}
			if (nLeftCount != 4 &&
				nLeftCount != 0) {
				dynamic_cast<CBeam*>(m_vecBeamBase[0])->HitPlayer(true);
			}

		}
	}
}

//=============================================================================
// 残骸の当たり判定
//=============================================================================
void CCollision::CollideDebris(void)
{
	// プレイヤーとの判定
	CPlayer *pPlayer = CGame::GetPlayer();
	for (int nCntDebris = 0; nCntDebris < (signed)m_vecDebris.size(); nCntDebris++) {
		CScene2D *pDebris = dynamic_cast<CScene2D*>(m_vecDebris[nCntDebris]);
		if (CollisionObject(pDebris, pPlayer, D3DXVECTOR2(120, 120), pPlayer->GetSize())) {
			if (!pPlayer->GetDashState()) {
				pPlayer->AddForce(CManager::GetVectorFromPointAtoB(pDebris->GetPos(), pPlayer->GetPos()), 5.0f);	// ノックバック
			}
			break;
		}
	}
}

//=============================================================================
// オブジェクト同士の当たり判定 (AABB)
//=============================================================================
bool CCollision::CollisionObject(CScene2D *pObjScene1, CScene2D *pObjScene2)
{
	// オブジェクトの位置を取得
	D3DXVECTOR3 objPos1, objPos2;
	objPos1 = pObjScene1->GetPos();
	objPos2 = pObjScene2->GetPos();

	// オブジェクトの前フレームの位置を取得
	D3DXVECTOR3 objPosPrev1;
	objPosPrev1 = pObjScene1->GetPosPrev();

	// オブジェクトのサイズを取得
	D3DXVECTOR2 objSize1, objSize2;
	objSize1 = pObjScene1->GetSize();
	objSize2 = pObjScene2->GetSize();

	/*
	1右
	objPos1.x + objSize1.x / 2
	1左
	objPos1.x - objSize1.x / 2
	1上
	objPos1.y - objSize1.y / 2
	1下
	objPos1.y + objSize1.y / 2
	2右
	objPos2.x + objSize2.x / 2
	2左
	objPos2.x - objSize2.x / 2

	2上
	objPos2.y - objSize2.y / 2
	2下
	objPos2.y + objSize2.y / 2
	prev右
	objPosPrev1.x + objSize1.x / 2
	prev左
	objPosPrev1.x - objSize1.x / 2
	prev上
	objPosPrev1.y - objSize1.y / 2
	prev下
	objPosPrev1.y + objSize1.y / 2
	*/

	if ((objPos1.x + objSize1.x / 2 > objPos1.x - objSize1.x / 2 && objPos1.x - objSize1.x / 2 < objPos2.x + objSize2.x / 2) &&
		(objPos1.y + objSize1.y / 2 > objPos2.y - objSize2.y / 2))
	{
		if (objPosPrev1.y + objSize1.y / 2 <= objPos2.y - objSize2.y / 2 &&
			objPosPrev1.x + objSize1.x / 2 >= objPos2.x - objSize2.x / 2 && objPosPrev1.x - objSize1.x / 2 <= objPos2.x + objSize2.x / 2)
		{
			return true;
		}
	}
	if ((objPos1.x + objSize1.x / 2 > objPos2.x - objSize2.x / 2 && objPos1.x - objSize1.x / 2 < objPos2.x + objSize2.x / 2) &&
		(objPos1.y + objSize1.y / 2 > objPos2.y + objSize2.y / 2 && objPos1.y - objSize1.y / 2 < objPos2.y + objSize2.y / 2))
	{
		if (objPosPrev1.y - objSize1.y / 2 >= objPos2.y + objSize2.y / 2 &&
			objPosPrev1.x + objSize1.x / 2 >= objPos2.x - objSize2.x / 2 && objPosPrev1.x - objSize1.x / 2 <= objPos2.x + objSize2.x / 2)
		{
			return true;
		}
	}
	if ((objPos1.y + objSize1.y / 2 > objPos2.y - objSize2.y / 2 && objPos1.y - objSize1.y / 2 < objPos2.y + objSize2.y / 2) &&
		(objPos1.x + objSize1.x / 2 > objPos2.x - objSize2.x / 2 && objPos1.x - objSize1.x / 2 < objPos2.x - objSize2.x / 2))
	{
		if (objPosPrev1.x <= objPos2.x &&
			objPosPrev1.y + objSize1.y / 2 >= objPos2.y - objSize2.y / 2 && objPosPrev1.y - objSize1.y / 2 <= objPos2.y + objSize2.y / 2)
		{
			return true;
		}
	}
	if ((objPos1.y + objSize1.y / 2 > objPos2.y - objSize2.y / 2 && objPos1.y - objSize1.y / 2 < objPos2.y + objSize2.y / 2) &&
		(objPos1.x + objSize1.x / 2 > objPos2.x + objSize2.x / 2 && objPos1.x - objSize1.x / 2 < objPos2.x + objSize2.x / 2))
	{
		if (objPosPrev1.x + objSize1.x / 2 >= objPos2.x + objSize2.x / 2 &&
			objPosPrev1.y + objSize1.y / 2 > objPos2.y - objSize2.y / 2 && objPosPrev1.y - objSize1.y / 2 < objPos2.y + objSize2.y / 2)
		{
			return true;
		}
	}

	return false;
}

bool CCollision::CollisionObject(CScene2D *pObjScene1, CScene2D *pObjScene2, D3DXVECTOR2 size1, D3DXVECTOR2 size2)
{
	// オブジェクトの位置を取得
	D3DXVECTOR3 objPos1, objPos2;
	objPos1 = pObjScene1->GetPos();
	objPos2 = pObjScene2->GetPos();

	// オブジェクトの前フレームの位置を取得
	D3DXVECTOR3 objPosPrev1;
	objPosPrev1 = pObjScene1->GetPosPrev();

	if ((objPos1.x + size1.x / 2 > objPos1.x - size1.x / 2 && objPos1.x - size1.x / 2 < objPos2.x + size2.x / 2) &&
		(objPos1.y + size1.y / 2 > objPos2.y - size2.y / 2))
	{
		if (objPosPrev1.y + size1.y / 2 <= objPos2.y - size2.y / 2 &&
			objPosPrev1.x + size1.x / 2 >= objPos2.x - size2.x / 2 && objPosPrev1.x - size1.x / 2 <= objPos2.x + size2.x / 2)
		{
			return true;
		}
	}
	if ((objPos1.x + size1.x / 2 > objPos2.x - size2.x / 2 && objPos1.x - size1.x / 2 < objPos2.x + size2.x / 2) &&
		(objPos1.y + size1.y / 2 > objPos2.y + size2.y / 2 && objPos1.y - size1.y / 2 < objPos2.y + size2.y / 2))
	{
		if (objPosPrev1.y - size1.y / 2 >= objPos2.y + size2.y / 2 &&
			objPosPrev1.x + size1.x / 2 >= objPos2.x - size2.x / 2 && objPosPrev1.x - size1.x / 2 <= objPos2.x + size2.x / 2)
		{
			return true;
		}
	}
	if ((objPos1.y + size1.y / 2 > objPos2.y - size2.y / 2 && objPos1.y - size1.y / 2 < objPos2.y + size2.y / 2) &&
		(objPos1.x + size1.x / 2 > objPos2.x - size2.x / 2 && objPos1.x - size1.x / 2 < objPos2.x - size2.x / 2))
	{
		if (objPosPrev1.x <= objPos2.x &&
			objPosPrev1.y + size1.y / 2 >= objPos2.y - size2.y / 2 && objPosPrev1.y - size1.y / 2 <= objPos2.y + size2.y / 2)
		{
			return true;
		}
	}
	if ((objPos1.y + size1.y / 2 > objPos2.y - size2.y / 2 && objPos1.y - size1.y / 2 < objPos2.y + size2.y / 2) &&
		(objPos1.x + size1.x / 2 > objPos2.x + size2.x / 2 && objPos1.x - size1.x / 2 < objPos2.x + size2.x / 2))
	{
		if (objPosPrev1.x + size1.x / 2 >= objPos2.x + size2.x / 2 &&
			objPosPrev1.y + size1.y / 2 > objPos2.y - size2.y / 2 && objPosPrev1.y - size1.y / 2 < objPos2.y + size2.y / 2)
		{
			return true;
		}
	}

	return false;
}