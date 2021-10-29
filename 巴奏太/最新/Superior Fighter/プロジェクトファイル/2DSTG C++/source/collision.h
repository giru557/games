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
#include "scene2d.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 当たり判定クラス ( 派生元: オブジェクトクラス (scene) )
//*****************************************************************************
class CCollision : public CScene
{
public:
	CCollision();
	~CCollision();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPos(D3DXVECTOR3 pos) { }
	void SetRot(D3DXVECTOR3 rot) {};
	void SetSize(D3DXVECTOR2 size) {};
	D3DXVECTOR3 GetPos(void) { return VECTOR3_ZERO; }
	D3DXVECTOR3 GetRot(void) { return VECTOR3_ZERO; }
	D3DXVECTOR2 GetSize(void) { return (D3DXVECTOR2)VECTOR3_ZERO; }

	static CCollision *Create(void);
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