//=============================================================================
//
// レンダリング [renderer.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

//*****************************************************************************
// ヘッダのインクルード
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TARGET_OFFSET (0.5f)
#define TARGET_SIZE (D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT))
#define TARGET_OPACITYDEF (0.75f)
#define TARGET_SIZEOFFSETDEF (-40.0f)

//*****************************************************************************
// レンダラークラス
//*****************************************************************************
class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }	// Direct3Dデバイスを取得

	void SetFeedbackEffect(int nFrames, float fOpacity, float fSizeOffset);		// フィードバックエフェクトを有効化 (フレーム数指定）
	void SetFeedbackEffect(int nFrames);										// 不透明度、サイズオフセットが指定されない場合は初期値で発動

	void SetFeedbackEffectStatus(bool bFlag) { m_FBParam.bActive = bFlag; }		// フィードバックエフェクトを有効/無効にする
	bool GetFeedbackEffectStatus(void) { return m_FBParam.bActive; }			// 現在のフィードバックエフェクトの状態を取得

	void SetBlurOpacity(float fValue) { m_FBParam.fOpacity = fValue; }			// フィードバック用ポリゴンの不透明度を設定
	float GetBlurOpacity(void) { return m_FBParam.fOpacity; }					//								　を返す

	void SetBlurSizeOffset(float fValue) { m_FBParam.fSizeOffset = fValue; }	// フィードバック用ポリゴンのサイズオフセットを設定
	float GetBlurSizeOffset(void) { return m_FBParam.fSizeOffset; }				// 　　

private:
	// フィードバックエフェクトの効果を変えるパラメーター
	typedef struct {
		bool  bActive;				// エフェクトを有効にするかどうか
		float fOpacity;				// ポリゴンの不透明度、低くするとブラーが薄くなる
		float fSizeOffset;			// サイズのオフセット、マイナスで外側、プラスで内側にぶれる
		int nFrames;				// 発動する時間 (フレーム数)
		int nCounter;
	} FEEDBACK_PARAM;

	void CountFrame(void);
	void TargetPolyDraw(int nTextureIdx);	// フィードバック用ポリゴンの描画

	LPDIRECT3D9 m_pD3D;				// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9 m_pD3DDevice;	// デバイスオブジェクト

	LPDIRECT3DTEXTURE9 m_apTextureMT[2];		// レンダリングターゲット用テクスチャ
	LPDIRECT3DSURFACE9 m_apRenderMT[2];			// テクスチャレンダリング用インターフェース
	LPDIRECT3DSURFACE9 m_apBuffMT[2];			// テクスチャレンダリング用Zバッファ
	D3DVIEWPORT9 m_viewportMT;					// テクスチャレンダリング用ビューポート
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffMT;		// フィードバック用ポリゴン頂点バッファ
	FEEDBACK_PARAM m_FBParam;					// フィードバックエフェクトのパラメーター
};
#endif
