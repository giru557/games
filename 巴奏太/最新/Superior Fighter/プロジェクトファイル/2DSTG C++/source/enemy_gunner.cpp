//=============================================================================
//
// �G�h���N���X �K���i�[ [enemy_gunner.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "enemy_gunner.h"
#include "bullet.h"
#include "player.h"
#include "camera.h"
#include "manager.h"
#include "game.h"
#include "life.h"
#include "particle.h"

//*****************************************************************************
// �G�h���N���X �K���i�[ ( �p����: �G�N���X [enemy] )
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEnemyGunner::CEnemyGunner()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEnemyGunner::~CEnemyGunner()
{

}

//=============================================================================
// ������
//=============================================================================
HRESULT CEnemyGunner::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// ����������
	if (FAILED(CEnemy::Init(pos, rot, size)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEnemyGunner::Uninit(void)
{
	CEnemy::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEnemyGunner::Update(void)
{
	//�s������
	this->ActionProc();

	CEnemy::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEnemyGunner::Draw(void)
{
	CEnemy::Draw();
}

//=============================================================================
// �����֐�
//=============================================================================
CEnemyGunner *CEnemyGunner::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	CEnemyGunner *pEnemy;
	pEnemy = new CEnemyGunner;
	if (pEnemy != NULL)
	{
		pEnemy->Init(pos, rot, size);
		pEnemy->SetTexture(ENEMY_TYPE_GUNNER);
	}

	return pEnemy;
}

//=============================================================================
// �s���̏������܂Ƃ߂�֐�
//=============================================================================
void CEnemyGunner::ActionProc(void)
{
	D3DXVECTOR3 enemyScreenPos = CCamera::WorldToScreenPosition(GetPos());	// �G�̃X�N���[�����W
	float fDistEnemyPlayer = CManager::GetDistanceAB(GetPos(), CGame::GetPlayer()->GetPos());

	// ���m�͈͂�艓���ꍇ
	if (fDistEnemyPlayer >= ENEMY_GUNNER_DETECT_DISTANCE)
	{
		// �������l�߂邾���̍s��������
		SetAction(ENEMY_ACTION_FOLLOW);
	}
	// �ˌ����m�͈͂�艓���ꍇ
	else if (fDistEnemyPlayer >= ENEMY_GUNNER_DETECT_SHOOT_DISTANCE)
	{
		// �������l�߂Ȃ���ˌ����Ă���
		SetAction(ENEMY_ACTION_FOLLOW_SHOOT);
	}
	else
	{
		// �~�܂��Ďˌ����Ă���
		SetAction(ENEMY_ACTION_SHOOT);
	}
}