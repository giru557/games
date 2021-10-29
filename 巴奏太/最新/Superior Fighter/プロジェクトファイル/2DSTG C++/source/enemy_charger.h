//=============================================================================
//
// �G�h���N���X ( �`���[�W���[ ) [enemy_charger.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _ENEMY_CHARGER_H_
#define _ENEMY_CHARGER_H_

//*****************************************************************************
// �w�b�_�[�̃C���N���[�h
//*****************************************************************************
#include "enemy.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ENEMY_CHARGER_SIZE (D3DXVECTOR2(80.0f, 80.0f))
#define ENEMY_CHARGER_DAMAGE (2)
#define ENEMY_CHARGER_LIFE (1)

//*****************************************************************************
// �G�h���N���X �`���[�W���[ ( �p����: �G�N���X [enemy] )
//*****************************************************************************
class CEnemyCharger : public CEnemy
{
public:
	CEnemyCharger();
	~CEnemyCharger();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CEnemyCharger *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
private:

};
#endif