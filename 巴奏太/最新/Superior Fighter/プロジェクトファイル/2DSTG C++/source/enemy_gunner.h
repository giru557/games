//=============================================================================
//
// �G�h���N���X ( �K���i�[ ) [enemy_gunner.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _ENEMY_GUNNER_H_
#define _ENEMY_GUNNER_H_

//*****************************************************************************
// �w�b�_�[�̃C���N���[�h
//*****************************************************************************
#include "enemy.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ENEMY_GUNNER_SIZE (D3DXVECTOR2(80.0f, 80.0f))

#define ENEMY_GUNNER_DETECT_DISTANCE (2000.0f)
#define ENEMY_GUNNER_DETECT_SHOOT_DISTANCE (1000.0f)

#define ENEMY_GUNNER_COLLISION_DAMAGE (1)
#define ENEMY_GUNNER_COLLISION_KNOCKBACK (5.0f)

//*****************************************************************************
// �G�h���N���X �K���i�[ ( �p����: �G�N���X [enemy] )
//*****************************************************************************
class CEnemyGunner : public CEnemy
{
public:
	CEnemyGunner();
	~CEnemyGunner();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CEnemyGunner *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);

private:
	
	void ActionProc(void);
};
#endif