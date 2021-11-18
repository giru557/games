//=============================================================================
//
// �{�X�̊Ǘ� [boss_manager.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _BOSS_MANAGER_H_
#define _BOSS_MANAGER_H_

//*****************************************************************************
// �w�b�_�[�̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "scene.h"
#include "boss_turret.h"
#include "boss_core.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CBackground;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BOSS_SPAWNPOS_OFFSET (300)										// �X�|�[�����̃^���b�g�B�̃R�A����̃I�t�Z�b�g
#define BOSS_SPAWNCORE_ROTATION (D3DXVECTOR3(0.0f, 0.0f, D3DXToRadian(45.0f)))	// �R�A�̃X�|�[�����̉�]

//*****************************************************************************
// �{�X�Ǘ��N���X ( �p����: �I�u�W�F�N�g�N���X [ scene ])
//*****************************************************************************
class CBossManager : public CScene
{
public:
	CBossManager();
	~CBossManager();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPos(D3DXVECTOR3 pos) {};
	void SetRot(D3DXVECTOR3 rot) {};
	void SetSize(D3DXVECTOR2 size) {};
	D3DXVECTOR3 GetPos(void) { return VECTOR3_ZERO; }
	D3DXVECTOR3 GetRot(void) { return VECTOR3_ZERO; }
	D3DXVECTOR2 GetSize(void) { return (D3DXVECTOR2)VECTOR3_ZERO; }

	void SpawnBoss(void);

	static CBossManager *Create(void);

private:
	CBossCore *m_pSceneCore;
	CBossTurret *m_apSceneTurret[4];
	CBackground *m_pBG;
	int m_nBeamAttackActivateTime;
	int m_aChargedShotActivateTime[4];
	bool m_bActive;
};

#endif