//=============================================================================
//
// �N���X�w�A�N���X [crosshair.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _CROSSHAIR_H_
#define _CROSSHAIR_H_

//*****************************************************************************
// �w�b�_�[�̃C���N���[�h
//*****************************************************************************
#include "scene2d.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CROSSHAIR_SIZE (D3DXVECTOR2(144.0f, 144.0f))

//*****************************************************************************
// �N���X�w�A�N���X ( �h����: �I�u�W�F�N�g�N���X (scene) )
//*****************************************************************************
class CCrosshair : public CScene
{
public:
	CCrosshair();
	~CCrosshair();

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

	static CCrosshair *Create(void);

private:
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR2 m_size;

	CScene2D *m_pScene2D;	// �N���X�w�A�p��2D�I�u�W�F�N�g
};

#endif

