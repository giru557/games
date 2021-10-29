//=============================================================================
//
// �{�X�R�A�̏��� [boss_core.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _BOSS_CORE_H_
#define _BOSS_CORE_H_

//*****************************************************************************
// �w�b�_�[�̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "scene2d.h"
#include "enemy.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CBeam;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BOSS_CORE_BEAMATTACK_CHARGETIME (180)				// �{�X�̃r�[���̃`���[�W����
#define BOSS_CORE_BEAMATTACK_ACTIVATETIME (rand() % 5 + 12)	// �{�X�̃r�[���̔����܂ł̎���
#define BOSS_CORE_BEAMATTACK_ACTIVETIME (300)				// �{�X�̃r�[���̔�������
#define BOSS_CORE_BEAMATTACK_SIZE (D3DXVECTOR2(45, 3000))	// �r�[���̃T�C�Y

#define BOSS_CORE_LIFE (40)						// �{�X�̃R�A�̗̑�
#define BOSS_CORE_SIZE (D3DXVECTOR2(200, 200))	// �{�X�̃R�A�̃T�C�Y

//*****************************************************************************
// �{�X�R�A�N���X ( �p����: �G�N���X [ enemy ])
//*****************************************************************************
class CBossCore : public CEnemy
{
public:
	CBossCore();
	~CBossCore();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetBeamAttack(void);

	static CBossCore *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

private:
	void BeamAttack(void);
	void PreAttack(void);

	CBeam *m_pBeam;
	bool m_bBeamAttack;
};

#endif