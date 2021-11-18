// LINE TEST
#ifndef _LINE_H_
#define _LINE_H_

// INCLUDE
#include "main.h"
#include "scene.h"

// LINE CLASS
class CLine : public CScene
{
public:
	CLine();
	~CLine();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetDraw(bool bDraw) { m_bDraw = bDraw; }
	void SetPos(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2) { m_aPoints[0] = pos1; m_aPoints[1] = pos2; }

	static CLine *Create(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float dwWidth, D3DXCOLOR col);

private:
	D3DXMATRIX m_mtxWorld;		// ワールドマトリックス
	LPD3DXLINE m_pLine;			// 線へのポインタ
	D3DXVECTOR3 m_aPoints[2];	// 線を描く位置
	D3DXCOLOR m_col;			// 線の色
	DWORD m_dwVertices;			// 頂点数
	float m_fWidth;				// 線の太さ
	bool m_bDraw;				// 描画フラグ
};

#endif