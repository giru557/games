//-----------------------------------------------------------------------------
//
// タイトルの処理 [title.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "title.h"
#include "input.h"
#include "fade.h"

//-----------------------------------------------------------------------------
// マクロ
//-----------------------------------------------------------------------------
#define TITLE_OBJ_AMOUNT (3)														//タイトルのオブジェクト数
#define TITLE_BLINKTIME (1.5f)														//点滅間隔 (秒)
#define TITLE_BLINKTIMEFIXED (TITLE_BLINKTIME * 1000)								//秒からミリ秒に変換

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
float TitleBlink(float fBlink);

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
TITLE g_aTitle[TITLE_OBJ_AMOUNT];												// TITLE構造体の情報
float g_fBlink;																	// 点滅

//-----------------------------------------------------------------------------
// タイトルの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス取得

	//TITLEの初期化
	g_aTitle[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aTitle[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) + TITLE_LOGO_OFFSET, 0.0f);
	g_aTitle[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) + TITLE_PRESS_OFFSET, 0.0f);
	g_fBlink = 1.0f;

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\title_bg.jpg", &g_aTitle[0].pTexture);
	D3DXCreateTextureFromFileEx(pDevice, "data\\TEXTURE\\title_logo.png", TITLE_LOGO_WIDTH, TITLE_LOGO_HEIGHT, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0xFF000000, NULL, NULL, &g_aTitle[1].pTexture);
	D3DXCreateTextureFromFileEx(pDevice, "data\\TEXTURE\\title_press.png", TITLE_PRESS_WIDTH, TITLE_PRESS_HEIGHT, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0xFF000000, NULL, NULL, &g_aTitle[2].pTexture);
	//フォーマット用意
	VERTEX_2D *pVertexBG;		// 背景用
	VERTEX_2D *pVertexLOGO;		// ロゴ用
	VERTEX_2D *pVertexPRESS;	// PRESSENTER用

	for (int nCntCreate = 0; nCntCreate < TITLE_OBJ_AMOUNT; nCntCreate++)
	{
		//頂点バッファの生成
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_aTitle[nCntCreate].pVtxBuff, NULL)))
		{
			return E_FAIL;
		}
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_aTitle[0].pVtxBuff->Lock(0, 0, (void**)&pVertexBG, 0);

	//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
	pVertexBG[0].pos = D3DXVECTOR3(g_aTitle[0].pos.x, g_aTitle[0].pos.y + SCREEN_HEIGHT, 0.0f);
	pVertexBG[1].pos = D3DXVECTOR3(g_aTitle[0].pos.x, g_aTitle[0].pos.y, 0.0f);
	pVertexBG[2].pos = D3DXVECTOR3(g_aTitle[0].pos.x + SCREEN_WIDTH, g_aTitle[0].pos.y + SCREEN_HEIGHT, 0.0f);
	pVertexBG[3].pos = D3DXVECTOR3(g_aTitle[0].pos.x + SCREEN_WIDTH, g_aTitle[0].pos.y, 0.0f);

	//rhwの設定
	for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertexBG[nCntVtx].rhw = 1.0f;	// 1.0f固定

	//頂点カラーの設定
	for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertexBG[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャの設定
	pVertexBG[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVertexBG[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVertexBG[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVertexBG[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//頂点バッファをアンロックする
	g_aTitle[0].pVtxBuff->Unlock();

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_aTitle[1].pVtxBuff->Lock(0, 0, (void**)&pVertexLOGO, 0);

	//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
	pVertexLOGO[0].pos = D3DXVECTOR3(g_aTitle[1].pos.x - (TITLE_LOGO_WIDTH / 2), g_aTitle[1].pos.y + (TITLE_LOGO_HEIGHT / 2), 0.0f);
	pVertexLOGO[1].pos = D3DXVECTOR3(g_aTitle[1].pos.x - (TITLE_LOGO_WIDTH / 2), g_aTitle[1].pos.y - (TITLE_LOGO_HEIGHT / 2), 0.0f);
	pVertexLOGO[2].pos = D3DXVECTOR3(g_aTitle[1].pos.x + (TITLE_LOGO_WIDTH / 2), g_aTitle[1].pos.y + (TITLE_LOGO_HEIGHT / 2), 0.0f);
	pVertexLOGO[3].pos = D3DXVECTOR3(g_aTitle[1].pos.x + (TITLE_LOGO_WIDTH / 2), g_aTitle[1].pos.y - (TITLE_LOGO_HEIGHT / 2), 0.0f);

	//rhwの設定
	for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertexLOGO[nCntVtx].rhw = 1.0f;	// 1.0f固定

	//頂点カラーの設定
	for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertexLOGO[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャの設定
	pVertexLOGO[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVertexLOGO[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVertexLOGO[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVertexLOGO[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//頂点バッファをアンロックする
	g_aTitle[1].pVtxBuff->Unlock();

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_aTitle[2].pVtxBuff->Lock(0, 0, (void**)&pVertexPRESS, 0);

	//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
	pVertexPRESS[0].pos = D3DXVECTOR3(g_aTitle[2].pos.x - (TITLE_PRESS_WIDTH / 2), g_aTitle[2].pos.y + (TITLE_PRESS_HEIGHT / 2), 0.0f);
	pVertexPRESS[1].pos = D3DXVECTOR3(g_aTitle[2].pos.x - (TITLE_PRESS_WIDTH / 2), g_aTitle[2].pos.y - (TITLE_PRESS_HEIGHT / 2), 0.0f);
	pVertexPRESS[2].pos = D3DXVECTOR3(g_aTitle[2].pos.x + (TITLE_PRESS_WIDTH / 2), g_aTitle[2].pos.y + (TITLE_PRESS_HEIGHT / 2), 0.0f);
	pVertexPRESS[3].pos = D3DXVECTOR3(g_aTitle[2].pos.x + (TITLE_PRESS_WIDTH / 2), g_aTitle[2].pos.y - (TITLE_PRESS_HEIGHT / 2), 0.0f);

	//rhwの設定
	for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertexPRESS[nCntVtx].rhw = 1.0f;	// 1.0f固定

	//頂点カラーの設定
	for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertexPRESS[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャの設定
	pVertexPRESS[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVertexPRESS[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVertexPRESS[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVertexPRESS[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//頂点バッファをアンロックする
	g_aTitle[2].pVtxBuff->Unlock();

	//フェードイン
	SetFade(FADE_IN, MODE_TITLE);

	return S_OK;
}

//-----------------------------------------------------------------------------
// タイトルの終了処理
//-----------------------------------------------------------------------------
void UninitTitle(void)
{
	for (int nCntObj = 0; nCntObj < TITLE_OBJ_AMOUNT; nCntObj++)
	{
		//頂点バッファの開放
		if (g_aTitle[nCntObj].pVtxBuff != NULL)
		{
			g_aTitle[nCntObj].pVtxBuff->Release();
			g_aTitle[nCntObj].pVtxBuff = NULL;
		}

		//テクスチャの開放
		if (g_aTitle[nCntObj].pTexture != NULL)
		{
			g_aTitle[nCntObj].pTexture->Release();
			g_aTitle[nCntObj].pTexture = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
// タイトルの更新処理
//-----------------------------------------------------------------------------
void UpdateTitle(void)
{
	VERTEX_2D *pVertexPRESS;

	//点滅
	g_fBlink = TitleBlink(g_fBlink);

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_aTitle[2].pVtxBuff->Lock(0, 0, (void**)&pVertexPRESS, 0);

	//頂点カラーの設定
	for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertexPRESS[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fBlink);

	//頂点バッファをアンロックする
	g_aTitle[2].pVtxBuff->Unlock();

	//遷移
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		SetFade(FADE_OUT, MODE_GAME);
	}
}

//-----------------------------------------------------------------------------
// タイトルの描画処理
//-----------------------------------------------------------------------------
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス取得

	for (int nCntObj = 0; nCntObj < TITLE_OBJ_AMOUNT; nCntObj++)
	{
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_aTitle[nCntObj].pVtxBuff, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_aTitle[nCntObj].pTexture);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

// タイトルの演出 (点滅)
float TitleBlink(float fBlink)
{
	static bool bBlink = false;
	static bool bOnce = false;
	static DWORD dwTime;
	static DWORD dwCTime;

	//現在時
	dwCTime = timeGetTime();

	//最初の時間を記憶
	if (bOnce == false)
	{
		dwTime = timeGetTime();
		bOnce = true;
	}

	//点滅切り替え
	if (bBlink == true)
	{
		fBlink = 0.0f;
	}
	else if (bBlink == false)
	{
		fBlink = 1.0f;
	}

	//現在時から最初時を引いて判定
	if ((dwCTime - dwTime) >= TITLE_BLINKTIMEFIXED)
	{
		bBlink = !bBlink;
		dwTime = timeGetTime();
	}

	return fBlink;
}