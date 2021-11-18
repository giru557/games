//=============================================================================
//
// ビームの処理 [beam.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _BEAM_H_
#define _BEAM_H_

//*****************************************************************************
// ヘッダーのインクルード
//*****************************************************************************
#include "main.h"
#include "scene2d.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BEAM_DAMAGE_COOLTIME (10)
#define BEAM_OBJUSAGE (2)

//*****************************************************************************
// ビームクラス ( 継承元: オブジェクトクラス [scene] )
//*****************************************************************************
class CBeam : public CScene
{
public:
	typedef enum {	// ビームの状態
		BEAMSTATE_NONE = 0,
		BEAMSTATE_START,
		BEAMSTATE_END,
	} BEAMSTATE;

	CBeam();
	~CBeam();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPos(D3DXVECTOR3 pos) {}
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };
	void SetSize(D3DXVECTOR2 size) {};
	D3DXVECTOR3 GetPos(void) { return VECTOR3_ZERO; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXVECTOR2 GetSize(void) { return (D3DXVECTOR2)VECTOR3_ZERO; }

	void SetBeamState(BEAMSTATE state) { m_beamState = state; }
	void HitPlayer(bool bHit) { m_bHitPlayer = bHit; }
	bool GetHitPlayer(void) { return m_bHitPlayer; }

	static CBeam *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size, int nActiveFrames);

private:
	void Cooltime(void);

	CScene2D *m_apScene[BEAM_OBJUSAGE];
	BEAMSTATE m_beamState;
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR2 m_sizeV;
	D3DXVECTOR2 m_sizeH;
	float m_fBeamSizeMul;
	float m_fBeamColMul;
	int m_nActiveFrames;
	int m_nDamageCooltime;
	bool m_bHitPlayer;
};

#endif

