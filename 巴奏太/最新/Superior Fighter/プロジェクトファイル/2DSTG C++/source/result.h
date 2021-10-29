//=============================================================================
//
// リザルト画面 [result.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

//*****************************************************************************
// ヘッダーのインクルード
//*****************************************************************************
#include "main.h"
#include "scene2d.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RESULT_TEXTURE_COUNT (2)
#define RESULT_TEXTURE_0_FILEPATH ("resource\\texture\\gameclear.jpg")
#define RESULT_TEXTURE_1_FILEPATH ("resource\\texture\\gameover.jpg")

//*****************************************************************************
// リザルト画面クラス ( 継承元: オブジェクトクラス [scene] )
//*****************************************************************************
class CResult : public CScene
{
public:
	CResult();
	~CResult();

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

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[RESULT_TEXTURE_COUNT];
	CScene2D *m_pScene2D;	// 背景用

};
#endif


