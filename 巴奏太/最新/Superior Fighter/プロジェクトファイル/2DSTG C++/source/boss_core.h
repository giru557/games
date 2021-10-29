//=============================================================================
//
// ボスコアの処理 [boss_core.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _BOSS_CORE_H_
#define _BOSS_CORE_H_

//*****************************************************************************
// ヘッダーのインクルード
//*****************************************************************************
#include "main.h"
#include "scene2d.h"
#include "enemy.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CBeam;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BOSS_CORE_BEAMATTACK_CHARGETIME (180)				// ボスのビームのチャージ時間
#define BOSS_CORE_BEAMATTACK_ACTIVATETIME (rand() % 5 + 12)	// ボスのビームの発動までの時間
#define BOSS_CORE_BEAMATTACK_ACTIVETIME (300)				// ボスのビームの発動時間
#define BOSS_CORE_BEAMATTACK_SIZE (D3DXVECTOR2(45, 3000))	// ビームのサイズ

#define BOSS_CORE_LIFE (40)						// ボスのコアの体力
#define BOSS_CORE_SIZE (D3DXVECTOR2(200, 200))	// ボスのコアのサイズ

//*****************************************************************************
// ボスコアクラス ( 継承元: 敵クラス [ enemy ])
//*****************************************************************************
class CBossCore : public CEnemy
{
public:
	CBossCore();
	~CBossCore();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetBeamAttack(void);

	static CBossCore *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

private:
	void BeamAttack(void);
	void PreAttack(void);

	CBeam *m_pBeam;
	bool m_bBeamAttack;
};

#endif