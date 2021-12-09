//=============================================================================
//
// スコアの処理 [score.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

//*****************************************************************************
// ヘッダーのインクルード
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCORE_DIGIT (8)	// スコアの桁数
#define SCORE_POSITION (D3DXVECTOR3(1500.0f, 80.0f, 0.0f))

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CNumber;

//*****************************************************************************
// スコアクラス ( 継承元: オブジェクトクラス [scene] )
//*****************************************************************************
class CScore : public CScene
{
public:
	CScore();
	~CScore();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetScore(int nScore);
	void AddScore(int nValue);
	int GetScore(void);

	static CScore *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);

private:
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 向き
	D3DXVECTOR2 m_size;					// サイズ
	CNumber *m_apNumber[SCORE_DIGIT];	// 数字表示クラスのポインタ
	int m_nScore;						// 実際のスコア
};
#endif

