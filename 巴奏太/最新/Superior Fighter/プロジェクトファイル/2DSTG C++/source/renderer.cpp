//=============================================================================
//
// レンダリング [renderer.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "renderer.h"
#include "manager.h"
#include "fade.h"
#include "debugger.h"
#include "camera3d.h"
#include "game.h"

//*****************************************************************************
// レンダラークラス (基本)
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CRenderer::CRenderer()
{
	m_FBParam.bActive = false;
	m_FBParam.fOpacity = TARGET_OPACITYDEF;
	m_FBParam.fSizeOffset = TARGET_SIZEOFFSETDEF;
}

//=============================================================================
// デストラクタ
//=============================================================================
CRenderer::~CRenderer()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;										// バックバッファの数
	d3dpp.BackBufferWidth = SCREEN_WIDTH;							// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;							// カラーモードの指定
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						// 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil = TRUE;							// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;										// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル

	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &m_pD3DDevice)))
			{
				// 生成失敗
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);	// テクスチャ拡大時の補間設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);	// テクスチャ縮小時の補間設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャU値の繰り返し設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャV値の繰り返し設定

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	LPDIRECT3DSURFACE9 pRenderDef, pBuffDef;	// 保存用
	for (int nCnt = 0; nCnt < 2; nCnt++) {
		// レンダリングターゲット用テクスチャ生成
		m_pD3DDevice->CreateTexture(
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT,
			&m_apTextureMT[nCnt],
			NULL);

		// テクスチャレンダリング用インターフェース生成
		m_apTextureMT[nCnt]->GetSurfaceLevel(0, &m_apRenderMT[nCnt]);

		// テクスチャレンダリング用Zバッファ生成
		m_pD3DDevice->CreateDepthStencilSurface(
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			D3DFMT_D16,
			D3DMULTISAMPLE_NONE,
			0,
			TRUE,
			&m_apBuffMT[nCnt],
			NULL);

		// 現在のレンダリングターゲットを取得 (保存)
		m_pD3DDevice->GetRenderTarget(0, &pRenderDef);

		// 現在のZバッファを取得 (保存)
		m_pD3DDevice->GetDepthStencilSurface(&pBuffDef);

		// レンダリングターゲットを生成したテクスチャに設定
		m_pD3DDevice->SetRenderTarget(0, m_apRenderMT[nCnt]);

		// Zバッファを設定
		m_pD3DDevice->SetDepthStencilSurface(m_apBuffMT[nCnt]);

		// レンダリングターゲット用のサーフェイスをクリア
		m_pD3DDevice->Clear(
			0,
			NULL,
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_RGBA(0, 0, 0, 0),
			1.0f,
			0);

		// レンダリングターゲットを元に戻す
		m_pD3DDevice->SetRenderTarget(0, pRenderDef);

		// Zバッファを元に戻す
		m_pD3DDevice->SetDepthStencilSurface(pBuffDef);
	}

	// テクスチャレンダリング用ビューポートの設定
	m_viewportMT.X = 0;
	m_viewportMT.Y = 0;
	m_viewportMT.Width = SCREEN_WIDTH;
	m_viewportMT.Height = SCREEN_HEIGHT;
	m_viewportMT.MinZ = 0.0f;
	m_viewportMT.MaxZ = 1.0f;

	// 頂点バッファの生成
	if (FAILED(m_pD3DDevice->CreateVertexBuffer(sizeof(CScene2D::VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuffMT, NULL)))
	{
		return E_FAIL;
	}

	// 頂点バッファをロック
	CScene2D::VERTEX_2D *pVtx;
	m_pVtxBuffMT->Lock(0, 0, (void**)&pVtx, 0);

	float fOff = TARGET_OFFSET;
	D3DXVECTOR2 pos = SCREEN_CENTER;
	pVtx[0].pos = D3DXVECTOR3(-TARGET_SIZE.x / 2 + pos.x - fOff, TARGET_SIZE.y / 2 + pos.y - fOff, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(-TARGET_SIZE.x / 2 + pos.x - fOff, -TARGET_SIZE.y / 2 + pos.y - fOff, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TARGET_SIZE.x / 2 + pos.x - fOff, TARGET_SIZE.y / 2 + pos.y - fOff, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TARGET_SIZE.x / 2 + pos.x - fOff, -TARGET_SIZE.y / 2 + pos.y - fOff, 0.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++) {
		pVtx[nCntVtx].col = D3DXCOLOR(1, 1, 1, 1);
		pVtx[nCntVtx].rhw = 1.0f;
	}

	// 頂点バッファアンロック
	m_pVtxBuffMT->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CRenderer::Uninit(void)
{
	for (int nCntMT = 0; nCntMT < 2; nCntMT++) {
		// レンダリングターゲット用テクスチャを破棄
		if (m_apTextureMT[nCntMT] != NULL) {
			m_apTextureMT[nCntMT]->Release();
			m_apTextureMT[nCntMT] = NULL;
		}

		// テクスチャレンダリング用インターフェースを破棄
		if (m_apRenderMT[nCntMT] != NULL) {
			m_apRenderMT[nCntMT]->Release();
			m_apRenderMT[nCntMT] = NULL;
		}

		// テクスチャレンダリング用Zバッファを破棄
		if (m_apBuffMT[nCntMT] != NULL) {
			m_apBuffMT[nCntMT]->Release();
			m_apBuffMT[nCntMT] = NULL;
		}
	}

	// 頂点バッファ破棄
	if (m_pVtxBuffMT != NULL) {
		m_pVtxBuffMT->Release();
		m_pVtxBuffMT = NULL;
	}

	// デバイスの破棄
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CRenderer::Update(void)
{
	// エフェクト用のカウンター
	CountFrame();

	// すべてのオブジェクトの更新
	CScene::UpdateAll();
}

//=============================================================================
// 描画処理
//=============================================================================
void CRenderer::Draw(void)
{
	LPDIRECT3DSURFACE9 pRenderDef, pBuffDef;	// レンダーターゲット、Zバッファ保存用
	D3DVIEWPORT9 viewportDef;					// ビューポート保存用

	// バックバッファ＆Ｚバッファのクリア
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		// 現在のレンダーターゲットを保存
		m_pD3DDevice->GetRenderTarget(0, &pRenderDef);
		m_pD3DDevice->GetDepthStencilSurface(&pBuffDef);
		m_pD3DDevice->GetViewport(&viewportDef);

		// レンダーターゲットをテクスチャ0に設定
		m_pD3DDevice->SetRenderTarget(0, m_apRenderMT[0]);
		m_pD3DDevice->SetDepthStencilSurface(m_apBuffMT[0]);
		m_pD3DDevice->SetViewport(&m_viewportMT);

		// テクスチャ0をクリア
		m_pD3DDevice->Clear(
			0,
			NULL,
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_RGBA(0, 0, 0, 0),
			1.0f,
			0);

		// カメラを設置
		CManager::GetCamera3D()->SetCamera(m_pD3DDevice);

		// すべてのオブジェクトの描画
		CScene::DrawAll();

		// フェードの描画
		CManager::GetFade()->Draw();

#ifdef _DEBUG
		CManager::GetDebugger()->Draw();
#endif
		// フラグが立っていたらポリゴンにテクスチャ１を貼って描画
		if (m_FBParam.bActive)
			TargetPolyDraw(1);

		// レンダーターゲットを元に戻す
		m_pD3DDevice->SetRenderTarget(0, pRenderDef);
		m_pD3DDevice->SetDepthStencilSurface(pBuffDef);
		m_pD3DDevice->SetViewport(&viewportDef);

		// フィードバック用ポリゴンにテクスチャ０を貼り付けて描画
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_ADD);	// 加算合成
		TargetPolyDraw(0);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

		// テクスチャ0と1を入れ替える
		swap(m_apRenderMT[0], m_apRenderMT[1]);
		swap(m_apTextureMT[0], m_apTextureMT[1]);

		// Direct3Dによる描画の終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// フィードバックエフェクトを発動
//=============================================================================
void CRenderer::SetFeedbackEffect(int nFrames, float fOpacity, float fSizeOffset)
{
	// 新しく呼ばれたフレーム数が現在より大きい場合のみ更新
	if (m_FBParam.nFrames < nFrames) {
		m_FBParam.nFrames = nFrames;
		m_FBParam.fOpacity = fOpacity;
		m_FBParam.fSizeOffset = fSizeOffset;
		m_FBParam.nCounter = 0;
		m_FBParam.bActive = true;
	}
}
void CRenderer::SetFeedbackEffect(int nFrames)
{
	// 新しく呼ばれたフレーム数が現在より大きい場合のみ更新
	if (m_FBParam.nFrames < nFrames) {
		m_FBParam.nFrames = nFrames;
		m_FBParam.fOpacity = TARGET_OPACITYDEF;
		m_FBParam.fSizeOffset = TARGET_SIZEOFFSETDEF;
		m_FBParam.nCounter = 0;
		m_FBParam.bActive = true;
	}
}

//*****************************************************************************
// エフェクトを発動するフレームをカウントする
//*****************************************************************************
void CRenderer::CountFrame(void)
{
	// 発動中
	if (m_FBParam.bActive) {
		m_FBParam.nCounter++;	// カウントアップ
		
		// カウント終了
		if (m_FBParam.nCounter >= m_FBParam.nFrames) {
			m_FBParam.bActive = false;
			m_FBParam.nCounter = 0;
			m_FBParam.nFrames = 0;
		}
	}
}

//*****************************************************************************
// フィードバック用ポリゴンを描画
//*****************************************************************************
void CRenderer::TargetPolyDraw(int nTextureIdx)
{
	// 頂点バッファをロック
	CScene2D::VERTEX_2D *pVtx;
	m_pVtxBuffMT->Lock(0, 0, (void**)&pVtx, 0);

	float fOff = TARGET_OFFSET;
	D3DXVECTOR2 pos = SCREEN_CENTER;
	D3DXVECTOR2 size = TARGET_SIZE;

	if (nTextureIdx != 0)
		size = D3DXVECTOR2(size.x - m_FBParam.fSizeOffset, size.y - m_FBParam.fSizeOffset);

	pVtx[0].pos = D3DXVECTOR3(-size.x / 2 + pos.x - fOff, size.y / 2 + pos.y - fOff, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(-size.x / 2 + pos.x - fOff, -size.y / 2 + pos.y - fOff, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(size.x / 2 + pos.x - fOff, size.y / 2 + pos.y - fOff, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(size.x / 2 + pos.x - fOff, -size.y / 2 + pos.y - fOff, 0.0f);

	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++) {
		if (nTextureIdx == 0)
			pVtx[nCntVtx].col = D3DXCOLOR(1, 1, 1, 1);
		else
			pVtx[nCntVtx].col = D3DXCOLOR(1, 1, 1, m_FBParam.fOpacity);
	}
	// 頂点バッファアンロック
	m_pVtxBuffMT->Unlock();

	// フィードバック用ポリゴンにテクスチャを貼り付けて描画
	m_pD3DDevice->SetStreamSource(0, m_pVtxBuffMT, 0, sizeof(CScene2D::VERTEX_2D));
	m_pD3DDevice->SetFVF(FVF_VERTEX_2D);
	m_pD3DDevice->SetTexture(0, m_apTextureMT[nTextureIdx]);
	m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}