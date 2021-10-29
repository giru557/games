//=============================================================================
//
// デバッグ用 [debugger.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "debugger.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// デバッグクラス
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CDebug::CDebug()
{
	for (int nCnt = 0; nCnt < DEBUGGER_VALUEWATCH_COUNT; nCnt++)
	{
		char n[] = "\n";
		strcpy(&aChild[nCnt][0], n);
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CDebug::~CDebug()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CDebug::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// デバッグ情報表示用フォントの生成
	if (FAILED(D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont))) {
		return E_FAIL;
	}

	if (FAILED(D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFontStatic))) {
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CDebug::Uninit(void)
{
	// デバッグ情報表示用フォントの破棄
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
}

//=============================================================================
// 更新
//=============================================================================
void CDebug::Update(void)
{

}

//=============================================================================
// 描画
//=============================================================================
void CDebug::Draw(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	char amaster[300] = "FPS:%d\n";
	
	for (int i = 0; i < DEBUGGER_VALUEWATCH_COUNT; i++)
		strcat(amaster, &aChild[i][0]);
	
	sprintf(m_str, amaster, GetFPS(), m_afVal[0], m_afVal[1], m_afVal[2], m_afVal[3]);

	// テキスト描画
	m_pFont->DrawText(NULL, m_str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	m_pFontStatic->DrawText(NULL, m_strStatic, -1, &rect, DT_RIGHT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}

//=============================================================================
// デバッグ文字に内容を追加する
//=============================================================================
void CDebug::AddStringStatic(char str[])
{
	const char cNewline[] = "\n";	// 改行用

	// 現在の文字列に新しい文字列を連結する
	strcat(m_strStatic, &str[0]);

	// 改行させる
	strcat(m_strStatic, &cNewline[0]);
}

//=============================================================================
// 文字を設定
//=============================================================================
void CDebug::SetString(char str[], int nIdx)
{
	if (nIdx < DEBUGGER_VALUEWATCH_COUNT)
		strcpy(&aChild[nIdx][0], str);
}

//=============================================================================
// 数値監視の文字を更新
//=============================================================================
void CDebug::UpdateValue(float fVal, int nIdx)
{
	if (nIdx < DEBUGGER_VALUEWATCH_COUNT)
		m_afVal[nIdx] = fVal;
}