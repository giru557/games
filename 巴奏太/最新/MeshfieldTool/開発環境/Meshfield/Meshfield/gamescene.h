//=============================================================================
//
// ゲーム画面 [game.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************************************
// ヘッダーのインクルード
//*****************************************************************************
#include "main.h"
#include "scene3d.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MESH_VTX_OFFSET_MOVESPEED (2.0f)	// 頂点の波オフセット移動速度
#define MESH_VTX_FREQUENCY_ADD (0.01f)		// 頂点の波の周期変動
#define MESH_VTX_SPEED_ADD (0.05f)			// 頂点の波のスピード変動
#define MESH_VTX_AMPLITUDE_ADD (1.0f)		// 頂点の波の大きさ変動

#define MESH_TEX_DIVISION_ADD (1.0f)		// テクスチャの分割数の変動
#define MESH_TEX_SCROLL_ADD (0.005f)			// テクスチャのスクロール変動
#define MESH_TEX_AMPLITUDE_ADD (0.1f)		// テクスチャの波の大きさ変動
#define MESH_TEX_SPEED_ADD (0.1f)			// テクスチャの波のスピード変動
#define MESH_TEX_FREQUENCY_ADD (0.01f)		// テクスチャの波の周期変動

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CFont;

//*****************************************************************************
// ゲーム画面クラス ( 継承元: オブジェクトクラス [CScene] )
//*****************************************************************************
class CGame : public CScene
{
public:
	CGame();
	~CGame();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	int GetTargetTexIdx(void) { return m_nTargetTex; }	// 対象になっているテクスチャ番号を取得
	int GetTargetXY(void) { return m_nXY; }

	static CFont *GetFont(void) { return m_pFont; }
	static CScene3D *GetMeshfield(void) { return m_pMeshfield; }
private:
	HRESULT ReadConfig(char* pFilePath, CScene3D::MESHFIELD *meshInfo);		// メッシュフィールドの設定を読み込む
	HRESULT ReadWaveConfig(char* pFilePath, CScene3D::WAVE *waveInfo);		// 波の設定を読み込む
	HRESULT WriteWaveConfig(char* pFilePath, CScene3D::WAVE waveInfo);
	void SetWaveInfo(CScene3D::WAVE waveInfo);
	CScene3D::WAVE GetWaveInfo(void);

	int m_nTargetTex;		// 現在対象になっているテクスチャの番号
	int m_nXY;

	static CFont *m_pFont;			// 文字表示クラス
	static CScene3D *m_pMeshfield;	// メッシュフィールド
};
#endif

