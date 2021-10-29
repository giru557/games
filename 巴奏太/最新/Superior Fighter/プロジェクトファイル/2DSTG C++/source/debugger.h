//=============================================================================
//
// デバッグ用 [debugger.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _DEBUGGER_H_
#define _DEBUGGER_H_

//*****************************************************************************
// ヘッダーのインクルード
//*****************************************************************************
#include "main.h"


//*****************************************************************************
// マクロ
//*****************************************************************************
#define DEBUGGER_STRING_MAX (1024)
#define DEBUGGER_VALUEWATCH_COUNT (4)

//*****************************************************************************
// デバッグクラス
//*****************************************************************************
class CDebug
{
public:
	CDebug();
	~CDebug();
	
	HRESULT Init(void);
	void Uninit();
	void Update();
	void Draw();

	void AddStringStatic(char str[]);
	void SetString(char str[], int nIdx);
	void UpdateValue(float fVal, int nIdx);

private:
	char m_str[DEBUGGER_STRING_MAX];	// デバッグ用の文字列
	char m_strStatic[DEBUGGER_STRING_MAX];

	char aChild[4][64];
	float m_afVal[4];

	LPD3DXFONT m_pFont;				// フォントへのポインタ
	LPD3DXFONT m_pFontStatic;
};

#endif
