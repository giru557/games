//=============================================================================
//
// 数字表示処理 [number.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

//*****************************************************************************
// ヘッダーのインクルード
//*****************************************************************************
#include "main.h"
#include "scene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 数字表示クラス ( 基本 )
//*****************************************************************************
class CNumber
{
public:
	CNumber();
	~CNumber();

	// 2Dの頂点フォーマット
	typedef struct
	{
		D3DXVECTOR3 pos;	// 頂点座標
		float rhw;			// 1.0固定
		D3DCOLOR col;		// 頂点カラー
		D3DXVECTOR2 tex;	// テクスチャ座標
	} VERTEX_2D;

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nInitNum);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetNumber(int nNumber);

	static CNumber *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nInitNum);

private:
	D3DXVECTOR3 m_pos;						// 位置
	D3DXVECTOR3 m_rot;						// 向き
	D3DXVECTOR2 m_size;						// サイズ
	float m_fLength;
	float m_fAngle;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファ
	VERTEX_2D *m_pVertex;
};
#endif
