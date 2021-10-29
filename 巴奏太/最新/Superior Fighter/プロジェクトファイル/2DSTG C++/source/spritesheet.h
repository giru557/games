//=============================================================================
//
// スプライトシートの処理 [spritesheet.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _SPRITESHEET_H_
#define _SPRITESHEET_H_

//*****************************************************************************
// ヘッダーのインクルード
//*****************************************************************************
#include "main.h"
#include "scene2d.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SPRITESHEET_DRAW_PRIORITY (2)

#define SPRITESHEET_EXPLOSION_COLUMN (7.0f)
#define SPRITESHEET_EXPLOSION_ROW (1.0f)
#define SPRITESHEET_EXPLOSION_ANIMSPEED (6)

#define SPRITESHEET_EXPLOSION2_COLUMN (30.0f)
#define SPRITESHEET_EXPLOSION2_ROW (1.0f)
#define SPRITESHEET_EXPLOSION2_ANIMSPEED (5)

#define SPRITESHEET_TEST_COLUMN (4.0f)
#define SPRITESHEET_TEST_ROW (2.0f)
#define SPRITESHEET_TEST_ANIMSPEED (30)

#define EXPLOSION_ANIM_SPEED (5)
#define EXPLOSION_ANIM_SPLIT (7.0f)
#define EXPLOSION_SIZE_DEFAULT (150.0f)

//*****************************************************************************
// スプライトシートクラス ( 継承元: 2Dポリゴン [scene2D] )
//*****************************************************************************
class CSpriteSheet : public CScene2D
{
public:
	// 使用可能な種類
	typedef enum
	{
		SPRITESHEET_EXPLOSION = 0,
		SPRITESHEET_EXPLOSION2,
		SPRITESHEET_TEST,
		SPRITESHEET_MAX,
	} SPRITESHEET;

	CSpriteSheet();
	~CSpriteSheet();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetSheetInfo(SPRITESHEET type);

	static CSpriteSheet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size, SPRITESHEET type);

private:
	int m_nCounterAnim;	// アニメーションのカウンター
	int m_nPatternH;	// 横方向のパターン
	int m_nPatternV;	// 縦方向のパターン
	int m_nAnimationSpeed;	// アニメーションの速度
	float m_fSheetColumn;	// スプライトシートの分割数 (横)
	float m_fSheetRow;		// スプライトシートの分割数 (縦)
	SPRITESHEET m_type;		// スプライトシートの種類
};

#endif