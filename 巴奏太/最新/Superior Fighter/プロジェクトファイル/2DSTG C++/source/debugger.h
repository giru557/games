//=============================================================================
//
// �f�o�b�O�p [debugger.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _DEBUGGER_H_
#define _DEBUGGER_H_

//*****************************************************************************
// �w�b�_�[�̃C���N���[�h
//*****************************************************************************
#include "main.h"


//*****************************************************************************
// �}�N��
//*****************************************************************************
#define DEBUGGER_STRING_MAX (1024)
#define DEBUGGER_VALUEWATCH_COUNT (4)

//*****************************************************************************
// �f�o�b�O�N���X
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
	char m_str[DEBUGGER_STRING_MAX];	// �f�o�b�O�p�̕�����
	char m_strStatic[DEBUGGER_STRING_MAX];

	char aChild[4][64];
	float m_afVal[4];

	LPD3DXFONT m_pFont;				// �t�H���g�ւ̃|�C���^
	LPD3DXFONT m_pFontStatic;
};

#endif
