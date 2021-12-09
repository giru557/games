//=============================================================================
//
// 3D背景 [bg3D.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _BG_3D_H_
#define _BG_3D_H_

//*****************************************************************************
// ヘッダーのインクルード
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BG_WAVE_FREQUENCY (0.05f)	// 揺れの周期
#define BG_WAVE_SPEED (0.1f)		// 揺れの速さ
#define BG_WAVE_AMPLITUDE (15.0f)	// 揺れの大きさ
#define BG_SIZE (D3DXVECTOR2(800, 500))		// ポリゴンの幅高さ
#define BG_DIVISION_X (64)					// ポリゴンの横分割数
#define BG_DIVISION_Z (64)					//			縦分割数

//*****************************************************************************
// 3D背景クラス (派生元：オブジェクトクラス (CScene) ）
//*****************************************************************************
class CBackground3D : public CScene
{
public:
	// 3Dの頂点フォーマット
	typedef struct
	{
		D3DXVECTOR3 pos;	// 頂点座標
		D3DXVECTOR3 nor;	// 法線ベクトル
		D3DCOLOR col;		// 頂点カラー
		D3DXVECTOR2 tex;	// テクスチャ座標
		D3DXVECTOR2 tex2;	// テクスチャ座標２
	} VERTEX_3D;

	CBackground3D(int nPriority = 3);
	~CBackground3D();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 位置向きサイズ
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetSize(D3DXVECTOR2 size) { m_size = D3DXVECTOR2(size.x, size.y); }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXVECTOR2 GetSize(void) { return m_size; }

	// 波紋の設定
	void SetRippleFrequency(float fValue, int nFrames) { m_ripple.fFrequency = fValue; m_ripple.nCountFreq = nFrames; }
	void SetRippleSpeed(float fValue, int nFrames) { m_ripple.fSpeed = fValue; m_ripple.nCountSpd = nFrames; }
	void SetRippleAmplitude(float fValue, int nFrames) { m_ripple.fAmplitude = fValue; m_ripple.nCountAmp = nFrames; }

	// テクスチャ設定
	void SetBossTexture(void) { m_bBossTex = true; }
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture1, LPDIRECT3DTEXTURE9 pTexture2, LPDIRECT3DTEXTURE9 pTexture3);

	static CBackground3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size, int nXDivision, int nZDivision);

private:
	// 三角形フォーマット
	typedef struct {
		int nV1;
		int nV2;
		int nV3;
		D3DXVECTOR3 nor;
	} TRIANGLE;

	// 波紋の数値
	typedef struct {
		float fFrequency;
		float fSpeed;
		float fAmplitude;
		int nCountFreq;
		int nCountSpd;
		int nCountAmp;
	} RIPPLE_PARAM;

	void RippleTimer(void);	// 波紋の値変更タイマー

	D3DXVECTOR3 m_pos;	// Position
	D3DXVECTOR3 m_rot;	// Rotation
	D3DXVECTOR2 m_size;	// Size

	D3DXMATRIX m_mtxWorld;					// ワールドマトリックス
	LPDIRECT3DTEXTURE9 m_apTexture[3];		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;		// インデックスバッファへのポインタ
	RIPPLE_PARAM m_ripple;					// 波紋の値
	bool m_bBossTex;						// ボス用のテクスチャに変更フラグ
	int m_nXdivision;						// X軸の分割数
	int m_nZdivision;						// Z軸の分割数
	int m_nVertices;						// 頂点数
	int m_nIndices;							// インデックス数
	int m_nTriangles;						// 三角形数
	VERTEX_3D *m_pVertex;					// 頂点情報へのポインタ
	TRIANGLE *m_pTri;						// 三角形の定義とその面法線
	vector<int> *m_pSharedVtx;				// 頂点共有リスト
};

#endif