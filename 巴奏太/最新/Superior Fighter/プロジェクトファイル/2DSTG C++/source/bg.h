//=============================================================================
//
// �w�i�N���X [bg.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _BG_H_
#define _BG_H_

//*****************************************************************************
// �w�b�_�[�̃C���N���[�h
//*****************************************************************************
#include "scene2d.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BG_DRAW_PRIORITY (0)
#define BG_TEXTURE_NUM (1)
#define BG_TEXTURE_0_PATH ("resource\\texture\\bg000.png")
#define BG_TEXTURE_0_SPEED (0.005f)
#define BG_TEXTURE_1_SPEED (0.002f)
#define BG_TEXTURE_2_SPEED (0.0007f)

//*****************************************************************************
// �w�i�N���X ( �h����: �I�u�W�F�N�g�N���X (scene) )
//*****************************************************************************
class CBackground : public CScene
{
public:
	CBackground();
	~CBackground();

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

	static CBackground *Create(void);

private:
	CScene2D *m_pScene2D;	// 2D�|���S���N���X�̃|�C���^
};

#endif
