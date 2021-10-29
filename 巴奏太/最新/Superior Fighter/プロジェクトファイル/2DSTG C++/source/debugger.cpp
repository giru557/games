//=============================================================================
//
// �f�o�b�O�p [debugger.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "debugger.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// �f�o�b�O�N���X
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
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
// �f�X�g���N�^
//=============================================================================
CDebug::~CDebug()
{

}

//=============================================================================
// ������
//=============================================================================
HRESULT CDebug::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �f�o�b�O���\���p�t�H���g�̐���
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
// �I��
//=============================================================================
void CDebug::Uninit(void)
{
	// �f�o�b�O���\���p�t�H���g�̔j��
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
}

//=============================================================================
// �X�V
//=============================================================================
void CDebug::Update(void)
{

}

//=============================================================================
// �`��
//=============================================================================
void CDebug::Draw(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	char amaster[300] = "FPS:%d\n";
	
	for (int i = 0; i < DEBUGGER_VALUEWATCH_COUNT; i++)
		strcat(amaster, &aChild[i][0]);
	
	sprintf(m_str, amaster, GetFPS(), m_afVal[0], m_afVal[1], m_afVal[2], m_afVal[3]);

	// �e�L�X�g�`��
	m_pFont->DrawText(NULL, m_str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	m_pFontStatic->DrawText(NULL, m_strStatic, -1, &rect, DT_RIGHT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}

//=============================================================================
// �f�o�b�O�����ɓ��e��ǉ�����
//=============================================================================
void CDebug::AddStringStatic(char str[])
{
	const char cNewline[] = "\n";	// ���s�p

	// ���݂̕�����ɐV�����������A������
	strcat(m_strStatic, &str[0]);

	// ���s������
	strcat(m_strStatic, &cNewline[0]);
}

//=============================================================================
// ������ݒ�
//=============================================================================
void CDebug::SetString(char str[], int nIdx)
{
	if (nIdx < DEBUGGER_VALUEWATCH_COUNT)
		strcpy(&aChild[nIdx][0], str);
}

//=============================================================================
// ���l�Ď��̕������X�V
//=============================================================================
void CDebug::UpdateValue(float fVal, int nIdx)
{
	if (nIdx < DEBUGGER_VALUEWATCH_COUNT)
		m_afVal[nIdx] = fVal;
}