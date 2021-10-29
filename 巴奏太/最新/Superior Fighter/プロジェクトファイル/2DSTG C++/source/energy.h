//=============================================================================
//
// エネルギーの処理 [energy.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _ENERGY_H_
#define _ENERGY_H_

//*****************************************************************************
// ヘッダーのインクルード
//*****************************************************************************
#include "main.h"
#include "scene2d.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENERGY_LAYER (3)
#define ENERGY_DRAW_PRIORITY (4)
#define ENERGY_SIZE (D3DXVECTOR2(620, 45))
#define ENERGY_POSITION (D3DXVECTOR3(750, 80, 0))

#define ENERGY_POINTS_MAX (100)
#define ENERGY_POINTS_INIT (0)
#define ENERGY_POINTS_REGENERATE (2)

//*****************************************************************************
// エネルギークラス ( 継承元: オブジェクトクラス [scene] )
//*****************************************************************************
class CEnergy : public CScene
{
public:
	CEnergy();
	~CEnergy();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// Set
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetSize(D3DXVECTOR2 size) { m_size = size; }

	// Get
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXVECTOR2 GetSize(void) { return m_size; }

	bool UseEnergy(int nValue);
	void AddEnergy(int nValue);

	static CEnergy *Create(void);

private:
	typedef enum
	{
		ENERGY_IDX_BACK = 0,
		ENERGY_IDX_MIDDLE,
		ENERGY_IDX_FRONT,
		ENERGY_IDX_MAX,
	} ENERGY_IDX;

	void BarProc(void);
	void Regenerate(int nRegenSpeed);

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR2 m_size;
	CScene2D *m_apScene[ENERGY_LAYER];
	int m_nEnergy;
	float m_fEnergyRatio;
	float m_fRatioLerp;
};

#endif