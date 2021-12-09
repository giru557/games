//=============================================================================
//
// �G�t�F�N�g�N���X [effect.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//*****************************************************************************
// �w�b�_�[�̃C���N���[�h
//*****************************************************************************
#include "scene2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define EFFECT_DRAW_PRIORITY (2)

//*****************************************************************************
// �G�t�F�N�g�N���X ( �p����: 2D�|���S�� [scene2d] )
//*****************************************************************************
class CEffect : public CScene2D
{
public:
	// �G�t�F�N�g�̃e�N�X�`���̎��
	typedef enum
	{
		EFFECT_TEX_DEFAULT = 0,
		EFFECT_TEX_PARTS1,
		EFFECT_TEX_PARTS2,
		EFFECT_TEX_PARTS3,
		EFFECT_TEX_SMOKE,
		EFFECT_TEX_MAX,
	} EFFECT_TEX;

	CEffect(int nPriority = EFFECT_DRAW_PRIORITY);
	~CEffect();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetTexture(EFFECT_TEX tex);
	void SetRenderState(D3DBLEND blendSrc, D3DBLEND blendDst) { m_blendSrc = blendSrc, m_blendDst = blendDst; }
	void AddForce(D3DXVECTOR3 dir, float fValue) { m_move += dir * fValue; }

	static CEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size, D3DXCOLOR col, EFFECT_TEX tex, float fShrinkSpd, float fTransparentSpd);

private:
	D3DXVECTOR3 m_move;
	D3DBLEND m_blendSrc;
	D3DBLEND m_blendDst;
	float m_fAlpha;				// �A���t�@�l�W��
	float m_fShrink;			// �k���p�W��
	float m_fShrinkSpeed;		// �k���X�s�[�h
	float m_fTransparentSpeed;	// �������X�s�[�h
};

#endif

