//=============================================================================
//
// �e�̏��� [bullet.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

//*****************************************************************************
// �w�b�_�[�̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "scene2D.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CEnemy;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BULLET_WIDTH_DEFAULT (243 * 0.4f)
#define BULLET_HEIGHT_DEFAULT (140 * 0.4f)
#define BULLET_SPEED_DEFAULT (30)
#define BULLET_SPEED_ENEMY (5)
#define BULLET_SPEED_BOSSCORE (10)
#define BULLET_LIFE_DEFAULT (100)
#define BULLET_LIFE_PC (30)

//*****************************************************************************
// �o���b�g�N���X ( �p����: 2D�|���S�� [scene2D] )
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
	OBJTYPE m_owner;		// �e�̏��L��
	D3DXVECTOR3 m_move;		// �ړ���
	int m_nLife;			// ����
};

#endif

