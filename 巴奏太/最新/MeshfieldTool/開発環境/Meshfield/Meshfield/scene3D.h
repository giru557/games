//=============================================================================
//
// 3Dポリゴン [scene3D.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _SCENE_3D_H_
#define _SCENE_3D_H_

//*****************************************************************************
// ヘッダーのインクルード
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CLine;

//*****************************************************************************
// ポリゴンクラス (派生元：オブジェクトクラス (scene) ）
//*****************************************************************************
class CScene3D : public CScene
{
public:
	// 3Dの頂点フォーマット
	typedef struct {
		D3DXVECTOR3 pos;	// 頂点座標
		D3DXVECTOR3 nor;	// 法線ベクトル
		D3DCOLOR col;		// 頂点カラー
		D3DXVECTOR2 tex;	// テクスチャ座標
		D3DXVECTOR2 tex2;	// テクスチャ座標2
	} VERTEX_3D;

	// テクスチャの構造体
	typedef struct {
		LPDIRECT3DTEXTURE9 pTexture;	// テクスチャへのポインタ
		D3DTEXTUREOP blendState;		// ブレンドの種類
		float fDivision;				// 分割の倍率
	} MESHFIELD_TEXTURE;

	// 波のパラメーター構造体
	typedef struct {
		float fFrequency;
		float fSpeed;
		float fAmplitude;
	} WAVEPARAM;

	// 波の種類
	typedef enum {
		WAVETYPE_RIPPLE = 0,
		WAVETYPE_X,
		WAVETYPE_Z,

		WAVETYPE_MAX,
	}WAVETYPE;

	// 波構造体
	typedef struct {
		WAVETYPE type;
		WAVEPARAM texParam[2][2];
		WAVEPARAM vtxParam;
		D3DXVECTOR3 rippleOffset;
		D3DXVECTOR2 texScroll[2];
	} WAVE;

	// メッシュフィールドの初期化情報
	typedef struct {
		float fSizeX;	// 大きさ X
		float fSizeZ;	// 大きさ Z
		int nXDivision;	// 分割数 X
		int nZDivision;	// 分割数 Z
		vector<MESHFIELD_TEXTURE> vTexture;	// テクスチャ
	} MESHFIELD;

	CScene3D();
	~CScene3D();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// テクスチャの分割設定
	void SetTextureDiv(int nIdx, float fDivision) { m_MeshField.vTexture[nIdx].fDivision = fDivision; }
	float GetTextureDiv(int nIdx) { return m_MeshField.vTexture[nIdx].fDivision; }

	// テクスチャのスクロール設定
	void SetTextureScroll(int nIdx, D3DXVECTOR2 scroll) { m_wave.texScroll[nIdx] = scroll; }
	D3DXVECTOR2 GetTextureScroll(int nIdx) { return m_wave.texScroll[nIdx]; }

	// テクスチャの波設定
	void SetTextureWave(int nIdx, WAVEPARAM param, int XorY);
	WAVEPARAM GetTextureWave(int nIdx, int XorY);

	// 頂点の波オフセット位置設定
	void SetVtxWaveOffset(D3DXVECTOR3 offset) { m_wave.rippleOffset = offset; }
	D3DXVECTOR3 GetVtxWaveOffset(void) { return m_wave.rippleOffset; }

	// 頂点の波のパラメーター設定
	void SetVtxWave(WAVEPARAM param) { m_wave.vtxParam = param; }
	WAVEPARAM GetVtxWave(void) { return m_wave.vtxParam; }

	// 頂点の波の種類設定
	void SetVtxWaveType(WAVETYPE waveType) { m_wave.type = waveType; }
	WAVETYPE GetVtxWaveType(void) { return m_wave.type; }

	// 法線表示切替
	void ToggleNormal(void) { m_bShowNormal = !m_bShowNormal; }

	// 生成
	static CScene3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, MESHFIELD meshInfo);

private:
	// 三角形フォーマット
	typedef struct {
		int nV1;
		int nV2;
		int nV3;
		D3DXVECTOR3 nor;
	} TRIANGLE;

	D3DXMATRIX m_mtxWorld;					// ワールドマトリックス
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;		// インデックスバッファへのポインタ
	MESHFIELD m_MeshField;					// メッシュフィールドの初期化情報
	WAVE m_wave;
	//D3DXVECTOR2 m_texScroll[2];				// テクスチャ座標のスクロール値
	//WAVEPARAM m_aTexWave[2][2];				// テクスチャの波のパラメーター
	//WAVEPARAM m_vtxWave;					// 頂点の波のパラメーター
	//WAVETYPE m_waveType;					// 現在の波の種類
	//D3DXVECTOR3 m_offset;					// 波の開始位置オフセット
	int m_nVertices;						// 頂点数
	int m_nIndices;							// インデックス数
	int m_nTriangles;						// 三角形数
	bool m_bShowNormal;						// 法線可視化フラグ
	TRIANGLE *m_pTri;						// 三角形フォーマットのポインタ
	VERTEX_3D *m_pVertex;					// 頂点のポインタ
	vector<int> *m_pSharedVtx;				// 共有頂点リストのポインタ
	vector<CLine*> m_vLine;
};

#endif