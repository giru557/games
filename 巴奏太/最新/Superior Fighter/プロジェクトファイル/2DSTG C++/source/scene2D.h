//=============================================================================
//
// 2Dポリゴン [scene2D.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _SCENE_2D_H_
#define _SCENE_2D_H_

//*****************************************************************************
// ヘッダーのインクルード
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCENE2D_WIDTH_DEFAULT (200)		// ポリゴンの幅 (デフォ)
#define SCENE2D_HEIGHT_DEFAULT (200)	// ポリゴンの高さ (デフォ)

//*****************************************************************************
// ポリゴンクラス (派生元：オブジェクトクラス (scene) ）
//*****************************************************************************
class CScene2D : public CScene
{
public:
	// 2Dの頂点フォーマット
	typedef struct
	{
		D3DXVECTOR3 pos;	// 頂点座標
		float rhw;			// 1.0固定
		D3DCOLOR col;		// 頂点カラー
		D3DXVECTOR2 tex;	// テクスチャ座標
	} VERTEX_2D;

	CScene2D(int nPriority = 3);
	~CScene2D();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void UpdateVertex(void);

	// Set
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetSize(D3DXVECTOR2 size) { m_size = D3DXVECTOR2(size.x, size.y); }
	void SetTextureUV(D3DXVECTOR2 Vertex_0, D3DXVECTOR2 Vertex_1, D3DXVECTOR2 Vertex_2, D3DXVECTOR2 Vertex_3);
	void SetVertices(D3DXVECTOR3 Vertex_0, D3DXVECTOR3 Vertex_1, D3DXVECTOR3 Vertex_2, D3DXVECTOR3 Vertex_3);
	void SetColor(D3DXCOLOR col);
	void SetConvertState(bool bConvertFlag) { m_bConvertPosition = bConvertFlag; }
	void SetDrawFlag(bool bDraw) { m_bDraw = bDraw; }

	// Get
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetPosPrev(void) { return m_posPrev; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXVECTOR2 GetSize(void) { return m_size; }
	D3DXCOLOR GetColor(void) { return m_col; }
	D3DXVECTOR3 **GetVertices(void);
	bool GetDrawFlag(void) { return m_bDraw; }

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

	static CScene2D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);

private:
	LPDIRECT3DTEXTURE9 m_pTexture;			// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	D3DXCOLOR m_col;						// 色
	D3DXVECTOR3 m_pos;						// 位置
	D3DXVECTOR3 m_posPrev;					// １フレーム前の位置
	D3DXVECTOR3 m_rot;						// 向き
	D3DXVECTOR2 m_size;						// 大きさ
	float m_fLength;						// 中心から頂点までの長さ
	float m_fAngle;							// 中心から頂点への角度
	bool m_bConvertPosition;				// ワールド座標からスクリーン座標に変換するかのフラグ
	bool m_bDraw;							// 描画フラグ
	VERTEX_2D *m_pVertex;					// 2Dポリゴンのポインタ

};

#endif