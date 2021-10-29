//=============================================================================
//
// タイトル [title.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

//*****************************************************************************
// ヘッダーのインクルード
//*****************************************************************************
#include "main.h"
#include "scene2d.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TITLE_TEXTURE_COUNT (2)
#define TITLE_TEXTURE_TITLE_FILEPATH ("resource\\texture\\superiorfighter_title.jpg")
#define TITLE_TEXTURE_START_FILEPATH ("resource\\texture\\superiorfighter_start.png")
#define TITLE_TEXTURE_START_SIZE (D3DXVECTOR2(634.0f, 284.0f))
#define TITLE_TEXTURE_START_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, 900.0f, 0.0f))
#define TITLE_START_BLINK_FREQUENCY (60)

//*****************************************************************************
// タイトルクラス ( 継承元: オブジェクトクラス [scene] )
//*****************************************************************************
class CTitle : public CScene
{
public:
	CTitle();
	~CTitle();

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
	CScene2D *m_pSceneTitle;	// タイトル背景のオブジェクト
	CScene2D *m_pSceneStart;	// ゲームスタートボタンのオブジェクト
	int m_nBlinkCounter;		// 点滅用カウンター
	bool m_bAppear;				// 点滅用フラグ

	static LPDIRECT3DTEXTURE9 m_apTexture[TITLE_TEXTURE_COUNT];

	void BlinkStartObj(void);	// 点滅用関数
};
#endif
