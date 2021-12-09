//=============================================================================
//
// �X�R�A�̏��� [score.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "score.h"
#include "number.h"

//*****************************************************************************
// �X�R�A�N���X ( �p����: �I�u�W�F�N�g�N���X [scene] )
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScore::CScore()
{
	memset(&m_apNumber[0], NULL, sizeof(m_apNumber));
	m_nScore = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScore::~CScore()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CScore::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// �����������[�v
	// �����\���N���X�𐶐�
	for (int nCntNum = 0; nCntNum < SCORE_DIGIT; nCntNum++)
	{
		D3DXVECTOR3 posNum = D3DXVECTOR3(pos.x + nCntNum * 50, pos.y, 0);
		m_apNumber[nCntNum] = CNumber::Create(posNum, size, 0);
	}

	// �`��D��x�ݒ�
	SetPriority(4);

	// �X�R�A������
	m_nScore = 0;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CScore::Uninit(void)
{
	// �������[�v
	for (int nCntNum = 0; nCntNum < SCORE_DIGIT; nCntNum++)
	{
		if (m_apNumber[nCntNum] != NULL)
		{
			m_apNumber[nCntNum]->Uninit();
			delete m_apNumber[nCntNum];
		}
	}

	// �J��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CScore::Update(void)
{
	// �������[�v
	// �����̍X�V����
	for (int nCntNum = 0; nCntNum < SCORE_DIGIT; nCntNum++)
	{
		m_apNumber[nCntNum]->Update();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CScore::Draw(void)
{
	// �������[�v
	// �����̕`��
	for (int nCntNum = 0; nCntNum < SCORE_DIGIT; nCntNum++)
	{
		m_apNumber[nCntNum]->Draw();
	}
}

//=============================================================================
// �X�R�A�̐ݒ�
//=============================================================================
void CScore::SetScore(int nScore)
{
	m_nScore = nScore;

	//�X�R�A�̕���
	m_apNumber[0]->SetNumber(m_nScore % 100000000 / 10000000);
	m_apNumber[1]->SetNumber(m_nScore % 10000000 / 1000000);
	m_apNumber[2]->SetNumber(m_nScore % 1000000 / 100000);
	m_apNumber[3]->SetNumber(m_nScore % 100000 / 10000);
	m_apNumber[4]->SetNumber(m_nScore % 10000 / 1000);
	m_apNumber[5]->SetNumber(m_nScore % 1000 / 100);
	m_apNumber[6]->SetNumber(m_nScore % 100 / 10);
	m_apNumber[7]->SetNumber(m_nScore % 10 / 1);
}

//=============================================================================
// �X�R�A�̉��Z
//=============================================================================
void CScore::AddScore(int nValue)
{
	m_nScore += nValue;

	//�X�R�A�̕���
	m_apNumber[0]->SetNumber(m_nScore % 100000000 / 10000000);
	m_apNumber[1]->SetNumber(m_nScore % 10000000 / 1000000);
	m_apNumber[2]->SetNumber(m_nScore % 1000000 / 100000);
	m_apNumber[3]->SetNumber(m_nScore % 100000 / 10000);
	m_apNumber[4]->SetNumber(m_nScore % 10000 / 1000);
	m_apNumber[5]->SetNumber(m_nScore % 1000 / 100);
	m_apNumber[6]->SetNumber(m_nScore % 100 / 10);
	m_apNumber[7]->SetNumber(m_nScore % 10 / 1);
}

//=============================================================================
// �X�R�A�̎擾
//=============================================================================
int CScore::GetScore(void)
{
	return m_nScore;
}

//=============================================================================
// �����֐�
//=============================================================================
CScore *CScore::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// �����A������
	CScore *pScore;
	pScore = new CScore;
	if (pScore != NULL)
	{
		pScore->Init(pos, rot, size);
	}

	return pScore;
}