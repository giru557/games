//=============================================================================
//
// フェード処理 [fade.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

//*****************************************************************************
// ヘッダーのインクルード
//*****************************************************************************
#include "main.h"
#include "scene2D.h"
#include "manager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FADE_SPEED (0.02f)

//*****************************************************************************
// フェードクラス
//*****************************************************************************
class CFade
{
public:
	// フェードの状態
	typedef enum
	{
		FADE_NONE = 0,
		FADE_IN,
		FADE_OUT,

		FADE_MAX,
	} FADE;

	CFade();
	~CFade();
	
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void FadeIN(void);
	void FadeOUT(CManager::MODE modeNext);

private:
	LPDIRECT3DTEXTURE9 m_pTexture;			// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	CScene2D::VERTEX_2D *m_pVertex;			// 2Dポリゴンのポインタ
	float m_fAlpha;							// 透明度
	CManager::MODE m_modeNext;				// 次のモード
	FADE m_fadeState;						// フェードの状態
};

#endif
