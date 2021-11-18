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

//*****************************************************************************
// �w�i�N���X ( �h����: �I�u�W�F�N�g�N���X (scene) )
//*****************************************************************************
class CBackground : public CScene
{
public:
	typedef enum {	// �w�i�̃t�F�[�h���
		BG_FADE_NONE = 0,
		BG_FADE_OUT,
		BG_FADE_IN,

		BG_FADE_MAX,
	} BG_FADE;

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

	void SetFade(BG_FADE fade);
	BG_FADE GetFade(void) { return m_fade; }
	static CBackground *Create(void);

private:
	CScene2D *m_pScene2D;	// 2D�|���S���N���X�̃|�C���^
	BG_FADE m_fade;			// ���݂̃t�F�[�h���
	float m_fFadeValue;		// �t�F�[�h�̒l
};

#endif
