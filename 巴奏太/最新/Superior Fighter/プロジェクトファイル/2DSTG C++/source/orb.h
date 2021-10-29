//=============================================================================
//
// �I�[�u�̏��� [orb.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _ORB_H_
#define _ORB_H_

//*****************************************************************************
// �w�b�_�[�̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "scene2d.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ORB_COLOR (D3DXCOLOR(0.5f, 1.0f, 1.0f, 1.0f))
#define ORB_FIND_TRIGGERDIST (200.0f)
#define ORB_GET_TRIGGERDIST (70.0f)
#define ORB_SIZE_DEFAULT (D3DXVECTOR2(50, 50))
#define ORB_ENERGY_GAIN (15)

//*****************************************************************************
// �I�[�u�N���X ( �p����: 2D�I�u�W�F�N�g�N���X [ scene2d ])
//*****************************************************************************
class COrb : public CScene2D
{
public:
	COrb();
	~COrb();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static COrb *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
	static COrb *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size, int nCreateChance);

private:
	void Apply(void);
	
	bool m_bTrigger;
};

#endif