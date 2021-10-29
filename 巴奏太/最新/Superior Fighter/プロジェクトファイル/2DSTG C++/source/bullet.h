//=============================================================================
//
// 弾の処理 [bullet.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

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
#define BULLET_WIDTH_DEFAULT (243 * 0.4f)
#define BULLET_HEIGHT_DEFAULT (140 * 0.4f)
#define BULLET_SPEED_DEFAULT (30)
#define BULLET_SPEED_ENEMY (5)
#define BULLET_SPEED_BOSSCORE (10)
#define BULLET_LIFE_DEFAULT (100)
#define BULLET_LIFE_PC (30)

//*****************************************************************************
// バレットクラス ( 継承元: 2Dポリゴン [scene2D] )
//*****************************************************************************
class CBullet : public CScene2D
{
public:
	CBullet();
	~CBullet();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size, D3DXVECTOR3 move, CScene::OBJTYPE owner);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void CollidePlayer(void);
	void CollideEnemy(CEnemy *pEnemy);
	OBJTYPE GetOwner(void) { return m_owner; }
	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size, D3DXVECTOR3 move, CScene::OBJTYPE owner);
	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, CScene::OBJTYPE owner);
private:
	OBJTYPE m_owner;		// 弾の所有者
	D3DXVECTOR3 m_move;		// 移動量
	int m_nLife;			// 寿命
};

#endif

