//=============================================================================
//
// �����\���N���X [font.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "font.h"
#include "manager.h"
#include "renderer.h"
#include "gamescene.h"

//*****************************************************************************
// �f�o�b�O�N���X
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CFont::CFont()
{
	
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CFont::~CFont()
{

}

//=============================================================================
// ������
//=============================================================================
HRESULT CFont::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �������p�̃t�H���g����
	if (FAILED(D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont[FONT_IDX_TEXT]))) {
		return E_FAIL;
	}

	// ���l�p�̃t�H���g����
	if (FAILED(D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont[FONT_IDX_VALUE]))) {
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CFont::Uninit(void)
{
	// �f�o�b�O���\���p�t�H���g�̔j��
	for (int nCnt = 0; nCnt < FONT_IDX_MAX; nCnt++) {
		if (m_pFont[nCnt] != NULL) {
			m_pFont[nCnt]->Release();
			m_pFont[nCnt] = NULL;
		}
	}
}

//=============================================================================
// �X�V
//=============================================================================
void CFont::Update(void)
{

}

//=============================================================================
// �`��
//=============================================================================
void CFont::Draw(void)
{
	// �����`��
	DrawTexts();
	DrawValues();
}

//=============================================================================
// �������̕`�揈��
//=============================================================================
void CFont::DrawTexts(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[512];

	int nNum = sprintf(&aStr[0], "[W] [A] [S] [D] : ���_�̔g�̌��_�ړ�\n");
	nNum += sprintf(&aStr[nNum], "[N] : ���_�̖@��������\n");
	nNum += sprintf(&aStr[nNum], "[M] : ���C���[�t���[��\n");
	nNum += sprintf(&aStr[nNum], "[F1] : �ۑ�\n");
	nNum += sprintf(&aStr[nNum], "[F5] : ���Z�b�g\n");

	// �e�L�X�g�`��
	m_pFont[FONT_IDX_TEXT]->DrawText(NULL, aStr, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}

//=============================================================================
// ���l�̕`�揈��
//=============================================================================
void CFont::DrawValues(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[512];

	// �l�擾
	D3DXVECTOR3 offsetPos = CManager::GetGame()->GetMeshfield()->GetVtxWaveOffset();
	CScene3D::WAVETYPE waveType = CManager::GetGame()->GetMeshfield()->GetVtxWaveType();
	int nTargetIdx = CManager::GetGame()->GetTargetTexIdx();
	int nXY = CManager::GetGame()->GetTargetXY();

	int nNum = sprintf(&aStr[0], "���݂̑Ώۃe�N�X�`�� [1] [2] : %d\n", nTargetIdx);
	nNum += sprintf(&aStr[nNum], "���݂̃e�N�X�`�������� [Z] [X] : %.2f\n", CManager::GetGame()->GetMeshfield()->GetTextureDiv(nTargetIdx));
	nNum += sprintf(&aStr[nNum], "���݂̃e�N�X�`���X�N���[���l(X) [Q] [E] : %.3f\n", CManager::GetGame()->GetMeshfield()->GetTextureScroll(nTargetIdx).x);
	nNum += sprintf(&aStr[nNum], "���݂̃e�N�X�`���X�N���[���l(Y) [R] [F]: %.3f\n", CManager::GetGame()->GetMeshfield()->GetTextureScroll(nTargetIdx).y);
	if (nXY == 0) 	nNum += sprintf(&aStr[nNum], "���ݕύX���Ă���e�N�X�`���g�̕��� [C] [V]: X\n");
	if (nXY == 1) 	nNum += sprintf(&aStr[nNum], "���ݕύX���Ă���e�N�X�`���g�̕��� [C] [V]: Y\n");
	nNum += sprintf(&aStr[nNum], "�e�N�X�`���̔g�̎��� [T] [G] : %.2f\n", CManager::GetGame()->GetMeshfield()->GetTextureWave(nTargetIdx, nXY).fFrequency);
	nNum += sprintf(&aStr[nNum], "�e�N�X�`���̔g�̑��� [Y] [H]: %.2f\n", CManager::GetGame()->GetMeshfield()->GetTextureWave(nTargetIdx, nXY).fSpeed);
	nNum += sprintf(&aStr[nNum], "�e�N�X�`���̔g�̑傫�� [U] [J]: %.2f\n", CManager::GetGame()->GetMeshfield()->GetTextureWave(nTargetIdx, nXY).fAmplitude);

	nNum += sprintf(&aStr[nNum], "\n");
	if (waveType == CScene3D::WAVETYPE_RIPPLE) nNum += sprintf(&aStr[nNum], "���_�̔g�̎�� [3] [4] [5]: �g��\n");
	if (waveType == CScene3D::WAVETYPE_X) nNum += sprintf(&aStr[nNum], "���_�̔g�̎�� [3] [4] [5]: X���W�\n");
	if (waveType == CScene3D::WAVETYPE_Z) nNum += sprintf(&aStr[nNum], "���_�̔g�̎�� [3] [4] [5]: Z���W�\n");
	nNum += sprintf(&aStr[nNum], "���_�̔g�̎��� [I] [K]: %.2f\n", CManager::GetGame()->GetMeshfield()->GetVtxWave().fFrequency);
	nNum += sprintf(&aStr[nNum], "���_�̔g�̑��� [O] [L]: %.2f\n", CManager::GetGame()->GetMeshfield()->GetVtxWave().fSpeed);
	nNum += sprintf(&aStr[nNum], "���_�̔g�̑傫�� [P] [;]: %.2f\n", CManager::GetGame()->GetMeshfield()->GetVtxWave().fAmplitude);
	nNum += sprintf(&aStr[nNum], "�g��̃I�t�Z�b�g�ʒu X:%.2f Z:%.2f\n", offsetPos.x, offsetPos.z);

	// �e�L�X�g�`��
	m_pFont[FONT_IDX_TEXT]->DrawText(NULL, aStr, -1, &rect, DT_RIGHT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}