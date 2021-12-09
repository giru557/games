//=============================================================================
//
// 当たり判定 [collision.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _COLLISION_H_
#define _COLLISION_H_

//*****************************************************************************
// ヘッダーのインクルード
//*****************************************************************************
#include "main.h"
#include "scene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 当たり判定クラス
//*****************************************************************************
class CCollision
{
public:
	CCollision();
	~CCollision();

	void Init();
	void Uninit(void);
	void Update(void);

	static bool CollisionObject(CScene2D *pObjScene1, CScene2D *pObjScene2);
	static bool CollisionObject(CScene2D *pObjScene1, CScene2D *pObjScene2, D3DXVECTOR2 size1, D3DXVECTOR2 size2);

private:
	void CollideEnemy(void);
	void CollideBoss(void);
	void CollideBullet(void);
	void CollideBeam(void);
	void CollideDebris(void);
	
	vector<CScene*> m_vecEnemy;
	vector<CScene*> m_vecBullet;
	vector<CScene*> m_vecBeam;
	vector<CScene*> m_vecBoss;
	vector<CScene*> m_vecBossCore;
	vector<CScene*> m_vecBeamBase;
	vector<CScene*> m_vecDebris;

};

#endif