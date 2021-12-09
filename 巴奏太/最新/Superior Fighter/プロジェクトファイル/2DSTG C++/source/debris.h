//=============================================================================
//
// �^���b�g�̎c�[ [debris.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _DEBRIS_H_
#define _DEBRIS_H_

//*****************************************************************************
// �w�b�_�[�̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "scene2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define DEBRIS_DRAWPRIORITY (2)

//*****************************************************************************
// �c�[�N���X ( �p����: 2D�|���S�� [scene2D] )
//*****************************************************************************
class CDebris : public CScene2D
{
public:
	CDebris(int nPriority = DEBRIS_DRAWPRIORITY);
	~CDebris();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CDebris *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);

private:
};

#endif

