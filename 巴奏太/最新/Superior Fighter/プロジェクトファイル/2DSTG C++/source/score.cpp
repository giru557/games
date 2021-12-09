//=============================================================================
//
// スコアの処理 [score.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "score.h"
#include "number.h"

//*****************************************************************************
// スコアクラス ( 継承元: オブジェクトクラス [scene] )
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CScore::CScore()
{
	memset(&m_apNumber[0], NULL, sizeof(m_apNumber));
	m_nScore = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CScore::~CScore()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScore::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// 桁数だけループ
	// 数字表示クラスを生成
	for (int nCntNum = 0; nCntNum < SCORE_DIGIT; nCntNum++)
	{
		D3DXVECTOR3 posNum = D3DXVECTOR3(pos.x + nCntNum * 50, pos.y, 0);
		m_apNumber[nCntNum] = CNumber::Create(posNum, size, 0);
	}

	// 描画優先度設定
	SetPriority(4);

	// スコア初期化
	m_nScore = 0;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CScore::Uninit(void)
{
	// 桁数ループ
	for (int nCntNum = 0; nCntNum < SCORE_DIGIT; nCntNum++)
	{
		if (m_apNumber[nCntNum] != NULL)
		{
			m_apNumber[nCntNum]->Uninit();
			delete m_apNumber[nCntNum];
		}
	}

	// 開放
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CScore::Update(void)
{
	// 桁数ループ
	// 数字の更新処理
	for (int nCntNum = 0; nCntNum < SCORE_DIGIT; nCntNum++)
	{
		m_apNumber[nCntNum]->Update();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CScore::Draw(void)
{
	// 桁数ループ
	// 数字の描画
	for (int nCntNum = 0; nCntNum < SCORE_DIGIT; nCntNum++)
	{
		m_apNumber[nCntNum]->Draw();
	}
}

//=============================================================================
// スコアの設定
//=============================================================================
void CScore::SetScore(int nScore)
{
	m_nScore = nScore;

	//スコアの分解
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
// スコアの加算
//=============================================================================
void CScore::AddScore(int nValue)
{
	m_nScore += nValue;

	//スコアの分解
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
// スコアの取得
//=============================================================================
int CScore::GetScore(void)
{
	return m_nScore;
}

//=============================================================================
// 生成関数
//=============================================================================
CScore *CScore::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// 生成、初期化
	CScore *pScore;
	pScore = new CScore;
	if (pScore != NULL)
	{
		pScore->Init(pos, rot, size);
	}

	return pScore;
}