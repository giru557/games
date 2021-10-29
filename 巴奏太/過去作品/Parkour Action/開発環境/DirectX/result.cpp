//-----------------------------------------------------------------------------
//
// リザルトの処理 [result.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "result.h"
#include "input.h"
#include "fade.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define RESULT_POLYGON (2)

#define RESULT_PRESSENTER_WIDTH (575)												// プレスエンターの幅
#define RESULT_PRESSENTER_HEIGHT (68)												// 高さ
#define RESULT_PRESSENTER_POS_X (1200)												// プレスエンターの場所X
#define RESULT_PRESSENTER_POS_Y (850)												// プレスエンターの場所Y

#define RESULT_SHRINKSPD (0.0005f)													//縮小拡大の速度
#define RESULT_SHRINKRANGE (0.9f)													//どこまで縮小するか (0.0f ~ 1.0f)
#define RESULT_BLINKTIME (1.5f)															//点滅間隔 (秒)
#define RESULT_BLINKTIMEFIXED (RESULT_BLINKTIME * 1000)								//秒からミリ秒に変換

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
float ResultShrink(float fShrink);
float ResultBlink(float fBlink);

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffResult = NULL;								//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_apTextureResult[RESULT_POLYGON] = { NULL };				//リザルトのテクスチャへのポインタ
RESULT g_aResult[RESULT_POLYGON];												//リザルト構造体の配列情報

//-----------------------------------------------------------------------------
// リザルトの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	//デバイスの取得
	pDevice = GetDevice();

	g_aResult[0].pos = D3DXVECTOR3(0, 0, 0);
	g_aResult[1].pos = D3DXVECTOR3(RESULT_PRESSENTER_POS_X, RESULT_PRESSENTER_POS_Y, 0.0f);
	for (int nCntRESULT = 0; nCntRESULT < RESULT_POLYGON; nCntRESULT++)
	{
		g_aResult[nCntRESULT].fShrink = 1.0f;
		g_aResult[nCntRESULT].fBlink = 1.0f;
	}

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\bg_result.png", &g_apTextureResult[0]);
	D3DXCreateTextureFromFileEx(pDevice, "data\\TEXTURE\\bg_result_pressenter.png", RESULT_PRESSENTER_WIDTH, RESULT_PRESSENTER_HEIGHT, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0xFF000000, NULL, NULL, &g_apTextureResult[1]);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * RESULT_POLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffResult, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntR = 0; nCntR < RESULT_POLYGON; nCntR++)
	{
		switch (nCntR)
		{
		case 0:
			//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(g_aResult[nCntR].pos.x, g_aResult[nCntR].pos.y + SCREEN_HEIGHT, 0.0f);
			pVertex[1].pos = D3DXVECTOR3(g_aResult[nCntR].pos.x, g_aResult[nCntR].pos.y, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_aResult[nCntR].pos.x + SCREEN_WIDTH, g_aResult[nCntR].pos.y + SCREEN_HEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_aResult[nCntR].pos.x + SCREEN_WIDTH, g_aResult[nCntR].pos.y, 0.0f);
			break;

		case 1:
			//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(g_aResult[nCntR].pos.x - (RESULT_PRESSENTER_WIDTH / 2), g_aResult[nCntR].pos.y + (RESULT_PRESSENTER_HEIGHT / 2), 0.0f);		 //TRIANGLESTRIPで四角
			pVertex[1].pos = D3DXVECTOR3(g_aResult[nCntR].pos.x - (RESULT_PRESSENTER_WIDTH / 2), g_aResult[nCntR].pos.y - (RESULT_PRESSENTER_HEIGHT / 2), 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_aResult[nCntR].pos.x + (RESULT_PRESSENTER_WIDTH / 2), g_aResult[nCntR].pos.y + (RESULT_PRESSENTER_HEIGHT / 2), 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_aResult[nCntR].pos.x + (RESULT_PRESSENTER_WIDTH / 2), g_aResult[nCntR].pos.y - (RESULT_PRESSENTER_HEIGHT / 2), 0.0f);
			break;

		default:
			break;
		}

		//rhwの設定
		pVertex[0].rhw = 1.0f;	//1.0f固定
		pVertex[1].rhw = 1.0f;
		pVertex[2].rhw = 1.0f;
		pVertex[3].rhw = 1.0f;

		//頂点カラーの設定
		pVertex[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);  //D3DCOLOR_RGBA(赤, 緑, 青, 透明度);
		pVertex[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVertex[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVertex[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//頂点情報の設定
		pVertex[0].tex = D3DXVECTOR2(0.0, 1.0);
		pVertex[1].tex = D3DXVECTOR2(0.0, 0.0);
		pVertex[2].tex = D3DXVECTOR2(1.0, 1.0);
		pVertex[3].tex = D3DXVECTOR2(1.0, 0.0);

		pVertex += 4; //頂点ポインタをずらす
	}

	//頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
// リザルトの終了処理
//-----------------------------------------------------------------------------
void UninitResult(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}

	for (int nCntR = 0; nCntR < (RESULT_POLYGON); nCntR++)
	{
		//テクスチャの開放
		if (g_apTextureResult[nCntR] != NULL)
		{
			g_apTextureResult[nCntR]->Release();
			g_apTextureResult[nCntR] = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
// リザルトの更新処理
//-----------------------------------------------------------------------------
void UpdateResult(void)
{
	VERTEX_2D *pVertex;
	RESULT *pResult;
	pResult = &g_aResult[0];

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntR = 0; nCntR < RESULT_POLYGON; nCntR++, pResult++)
	{
		switch (nCntR)
		{
		case 1:
			//タイトルの演出
			pResult->fBlink = ResultBlink(pResult->fBlink);

			//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(pResult->pos.x - ((RESULT_PRESSENTER_WIDTH / 2) * pResult->fShrink), pResult->pos.y + ((RESULT_PRESSENTER_HEIGHT / 2) * pResult->fShrink), 0.0f);		 //TRIANGLESTRIPで四角
			pVertex[1].pos = D3DXVECTOR3(pResult->pos.x - ((RESULT_PRESSENTER_WIDTH / 2) * pResult->fShrink), pResult->pos.y - ((RESULT_PRESSENTER_HEIGHT / 2) * pResult->fShrink), 0.0f);
			pVertex[2].pos = D3DXVECTOR3(pResult->pos.x + ((RESULT_PRESSENTER_WIDTH / 2) * pResult->fShrink), pResult->pos.y + ((RESULT_PRESSENTER_HEIGHT / 2) * pResult->fShrink), 0.0f);
			pVertex[3].pos = D3DXVECTOR3(pResult->pos.x + ((RESULT_PRESSENTER_WIDTH / 2) * pResult->fShrink), pResult->pos.y - ((RESULT_PRESSENTER_HEIGHT / 2) * pResult->fShrink), 0.0f);

			//頂点カラーの設定
			pVertex[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pResult->fBlink);  //D3DCOLOR_RGBA(赤, 緑, 青, 透明度);
			pVertex[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pResult->fBlink);
			pVertex[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pResult->fBlink);
			pVertex[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pResult->fBlink);
			break;
		}
		pVertex += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();

	if (GetKeyboardTrigger(DIK_BACKSPACE) == true || GetKeyboardTrigger(DIK_RETURN) == true)
	{
		SetFade(FADE_OUT, MODE_RANKING);
	}
}

//-----------------------------------------------------------------------------
// タイトルの描画処理
//-----------------------------------------------------------------------------
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntR = 0; nCntR < RESULT_POLYGON; nCntR++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureResult[nCntR]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntR * 4, 2);
	}
}

//-----------------------------------------------------------------------------
// リザルトの演出 (縮小拡大)
//-----------------------------------------------------------------------------
float ResultShrink(float fShrink)
{
	static bool bShrink = true;

	//縮小と拡大の切り替え
	if (bShrink == true)
	{
		fShrink -= RESULT_SHRINKSPD;
	}
	else if (bShrink == false)
	{
		fShrink += RESULT_SHRINKSPD;
	}

	//縮小拡大の下限上限
	if (fShrink >= 1.0f)
	{
		bShrink = true;
	}
	else if (fShrink <= RESULT_SHRINKRANGE)
	{
		bShrink = false;
	}

	return fShrink;
}

//-----------------------------------------------------------------------------
// リザルトの演出 (点滅)
//-----------------------------------------------------------------------------
float ResultBlink(float fBlink)
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
	if ((dwCTime - dwTime) >= RESULT_BLINKTIMEFIXED)
	{
		bBlink = !bBlink;
		dwTime = timeGetTime();
	}

	return fBlink;
}