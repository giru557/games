//=============================================================================
//
// �̗͂̏��� [life.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _LIFE_H_
#define _LIFE_H_

//*****************************************************************************
// �w�b�_�[�̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "scene2d.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define LIFE_LAYER (4)		// �\������
#define LIFE_PRIORITY (5)	// �`��D��x
#define LIFE_MIDDLE_COLOR (D3DXCOLOR(0.7f, 0.0f, 0.0f, 1.0f))

#define LIFE_POSITION (D3DXVECTOR3(200.0f, 80.0f, 0.0f))	// �ʒu
#define LIFE_POSITION_OFFSET (D3DXVECTOR3(33.0f, 0.0f, 0.0f))
#define LIFE_POSITION_MIDDLE (D3DXVECTOR3(233, 80.0f, 0.0f))

#define LIFE_DMG_TRANSPARENCY (0.3f)
#define LIFE_DMG_TRANS_SPEED (0.05f)
#define LIFE_DMG_COLOR_TRANSPARENT (D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f))

#define LIFE_SIZE_SCALE (5.0f)					// �T�C�Y�̔{��
#define LIFE_SIZE_FRONT (D3DXVECTOR2(51.0f * LIFE_SIZE_SCALE, 19.0f * LIFE_SIZE_SCALE))	// �|���S���T�C�Y
#define LIFE_SIZE_MIDDLE (D3DXVECTOR2(245, 35))
#define LIFE_SIZE_BACK (D3DXVECTOR2(66.0f * LIFE_SIZE_SCALE, 19.0f * LIFE_SIZE_SCALE))	// 

#define LIFE_HP_MAX (25)		// �ő僉�C�t
#define LIFE_HP_DEFAULT (25)	// �������C�t

//*****************************************************************************
// ���C�t�N���X ( �p����: �I�u�W�F�N�g�N���X [scene] )
//*****************************************************************************
class CLife : public CScene
{
public:
	CLife();
	~CLife();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// Set
	void SetPos(D3DXVECTOR3 pos) {};
	void SetRot(D3DXVECTOR3 rot) {};
	void SetSize(D3DXVECTOR2 size) {};

	// Get
	D3DXVECTOR3 GetPos(void) { return VECTOR3_ZERO; }
	D3DXVECTOR3 GetRot(void) { return VECTOR3_ZERO; }
	D3DXVECTOR2 GetSize(void) { return (D3DXVECTOR2)VECTOR3_ZERO; }

	int GetLife(void) { return m_nLife; }
	void Damage(int nValue);
	void Heal(int nValue);

	static CLife *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

private:
	typedef enum
	{
		LIFE_IDX_MIDDLE = 0,
		LIFE_IDX_MIDDLE2,
		LIFE_IDX_BACK,
		LIFE_IDX_FRONT,
		LIFE_IDX_MAX,
	} LIFE_IDX;

	void Smoke(void);
	void BarProc(void);

	D3DXVECTOR3 m_pos;					// �ʒu
	CScene2D *m_apScene2D[LIFE_LAYER];	// 2D�|���S���N���X�̃|�C���^
	CScene2D *m_apSceneBG;				// �w�i�p�|���S��
	int m_nLife;						// �̗͂̒l
	float m_fLifeRatio;					// �̗͊���
	float m_fDmgTransparency;			// �_���[�W���̉�ʓ����x
	bool m_bDmg;						// �_���[�W�t���O
};

#endif