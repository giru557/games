//=============================================================================
//
// パーティクル [particle.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

//*****************************************************************************
// ヘッダーのインクルード
//*****************************************************************************
#include "scene2d.h"
#include "effect.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// パーティクルクラス ( 派生元: オブジェクトクラス (scene) )
//*****************************************************************************
class CParticle : public CScene
{
public:
	CParticle();
	~CParticle();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetSize(D3DXVECTOR2 size) { m_size = size; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXVECTOR2 GetSize(void) { return m_size; }

	void SetRenderState(D3DBLEND blendSrc, D3DBLEND blendDst) { m_blendSrc = blendSrc, m_blendDst = blendDst; }

	static CParticle *Create(
		D3DXVECTOR3 pos,
		D3DXVECTOR3 rot,
		D3DXCOLOR col,
		D3DXVECTOR2 size,
		CEffect::EFFECT_TEX tex,
		int nFrames,
		float fForce,
		int nQuantity,
		float fShrinkSpeed,
		float fFadeSpeed,
		float fAngleDeg,
		float fAngleWide);

	static CParticle *Create(
		D3DXVECTOR3 pos,
		bool bRandRot,
		D3DXCOLOR col,
		D3DXVECTOR2 size,
		CEffect::EFFECT_TEX tex,
		int nFrames,
		float fForce,
		int nQuantity,
		float fShrinkSpeed,
		float fFadeSpeed,
		float fAngleDeg,
		float fAngleWide);

private:
	CEffect::EFFECT_TEX m_tex;
	D3DBLEND m_blendSrc, m_blendDst;
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR2 m_size;
	D3DXCOLOR m_col;
	int m_nFrames;
	int m_nFrameCounter;
	int m_nQuantity;
	float m_fForce;
	float m_fShrinkSpeed;
	float m_fFadeSpeed;
	float m_fAngleWide;
	float m_fAngleDeg;
	bool m_bRandRot;
};

#endif

