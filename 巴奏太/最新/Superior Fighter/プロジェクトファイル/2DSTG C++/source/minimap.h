//=============================================================================
//
// ミニマップ [minimap.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _MINIMAP_H_
#define _MINIMAP_H_

//*****************************************************************************
// ヘッダーのインクルード
//*****************************************************************************
#include "scene2d.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MINIMAP_POSITION (D3DXVECTOR3(1500, 150, 0))
#define MINIMAP_SIZE_DEVIDE (15.0f)
#define MINIMAP_SIZE (D3DXVECTOR2(GAME_FIELD_WIDTH, GAME_FIELD_HEIGHT) / MINIMAP_SIZE_DEVIDE)
#define MINIMAP_DRAW_MAX (250)

//*****************************************************************************
// ミニマップ ( 派生元: オブジェクトクラス (scene) )
//*****************************************************************************
class CMiniMap : public CScene
{
public:
	CMiniMap();
	~CMiniMap();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) {};
	void SetSize(D3DXVECTOR2 size) {};
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return VECTOR3_ZERO; }
	D3DXVECTOR2 GetSize(void) { return (D3DXVECTOR2)VECTOR3_ZERO; }

	static CMiniMap *Create(void);

private:
	typedef enum
	{
		IDX_MAP = 0,
		IDX_PLAYER,
	} SCENEIDX;

	void AddToMap(D3DXVECTOR3 pos, D3DXCOLOR col);

	D3DXVECTOR3 m_pos;
	CScene2D *m_apScene[MINIMAP_DRAW_MAX];
	CScene2D *m_pSceneMap;
	CScene2D *m_pScenePC;
};

#endif

