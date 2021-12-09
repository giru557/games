//=============================================================================
//
// �w���v�\���N���X [help.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _HELP_H_
#define _HELP_H_

//*****************************************************************************
// �w�b�_�[�̃C���N���[�h
//*****************************************************************************
#include "scene2d.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define HELP_SCENES (2)
#define HELP_OPEN_POSIITON (D3DXVECTOR3(150, 300, 0))
#define HELP_POSIITON (D3DXVECTOR3(500, 300,0))
#define HELP_SIZE (D3DXVECTOR2(1847, 1100) * 0.5f)
#define HELP_OPEN_SIZE (D3DXVECTOR2(464, 64) * 0.5f)
#define HELP_DRAW_PRIORITY (5)

//*****************************************************************************
// �w���v�\���N���X ( �h����: �I�u�W�F�N�g�N���X (scene) )
//*****************************************************************************
class CHelp : public CScene
{
public:
	CHelp();
	~CHelp();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SwitchVisible();

	static CHelp *Create(void);

private:
	bool m_bHelpVisible;
	CScene2D *m_pScene2D[2];
};

#endif

