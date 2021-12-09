//=============================================================================
//
// タレットの残骸 [debris.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _DEBRIS_H_
#define _DEBRIS_H_

//*****************************************************************************
// ヘッダーのインクルード
//*****************************************************************************
#include "main.h"
#include "scene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define DEBRIS_DRAWPRIORITY (2)

//*****************************************************************************
// 残骸クラス ( 継承元: 2Dポリゴン [scene2D] )
//*****************************************************************************
class CDebris : public CScene2D
{
public:
	CDebris(int nPriority = DEBRIS_DRAWPRIORITY);
	~CDebris();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CDebris *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);

private:
};

#endif

