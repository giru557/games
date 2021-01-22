//-----------------------------------------------------------------------------
//
// リザルトスコアの処理 [resultscore.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "resultscore.h"
#include "timescore.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define RESULTSCORE_POLYGON (4)		//使用するオブジェ数

#define RESULTSCORE_WIDTH (100)
#define RESULTSCORE_HEIGHT (150)
#define RESULTSCORE_SPACE (180)
#define RESULTSCORE_POS_X (400)
#define RESULTSCORE_POS_Y (390)

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffTimeR = NULL;					// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureTimeR = { NULL };			// ポリゴンのテクスチャへのポインタ
RESULTSCORE g_aResultScore[RESULTSCORE_POLYGON];

//-----------------------------------------------------------------------------
// リザルトスコアの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitResultScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	//TIMESCORE構造体の初期化
	for (int nCntRS = 0; nCntRS < RESULTSCORE_POLYGON; nCntRS++)
	{
		g_aResultScore[nCntRS].pos = D3DXVECTOR3(RESULTSCORE_POS_X, RESULTSCORE_POS_Y, 0.0f);
	}
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number000.png", &g_pTextureTimeR);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * RESULTSCORE_POLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTimeR, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffTimeR->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntChar = 0; nCntChar < RESULTSCORE_POLYGON; nCntChar++)
	{
		//頂点座標の設定
		pVertex[0].pos = D3DXVECTOR3((g_aResultScore[nCntChar].pos.x) + nCntChar * RESULTSCORE_SPACE, g_aResultScore[nCntChar].pos.y + RESULTSCORE_HEIGHT, 0.0f);
		pVertex[1].pos = D3DXVECTOR3((g_aResultScore[nCntChar].pos.x) + nCntChar * RESULTSCORE_SPACE, g_aResultScore[nCntChar].pos.y, 0.0f);
		pVertex[2].pos = D3DXVECTOR3((g_aResultScore[nCntChar].pos.x + RESULTSCORE_WIDTH) + nCntChar * RESULTSCORE_SPACE, g_aResultScore[nCntChar].pos.y + RESULTSCORE_HEIGHT, 0.0f);
		pVertex[3].pos = D3DXVECTOR3((g_aResultScore[nCntChar].pos.x + RESULTSCORE_WIDTH) + nCntChar * RESULTSCORE_SPACE, g_aResultScore[nCntChar].pos.y, 0.0f);

		//rhwの設定
		pVertex[0].rhw = 1.0f;	//1.0f固定
		pVertex[1].rhw = 1.0f;
		pVertex[2].rhw = 1.0f;
		pVertex[3].rhw = 1.0f;

		//頂点情報の設定
		pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVertex[2].tex = D3DXVECTOR2(1.0f * 0.1f, 1.0f);
		pVertex[3].tex = D3DXVECTOR2(1.0f * 0.1f, 0.0f);

		//頂点カラーの設定
		pVertex[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);  //D3DCOLOR_RGBA(赤, 緑, 青, 透明度);
		pVertex[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVertex[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVertex[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//頂点バッファずらす
		pVertex += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTimeR->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
// リザルトスコアの終了処理
//-----------------------------------------------------------------------------
void UninitResultScore(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffTimeR != NULL)
	{
		g_pVtxBuffTimeR->Release();
		g_pVtxBuffTimeR = NULL;
	}

	//テクスチャの開放
	if (g_pTextureTimeR != NULL)
	{
		g_pTextureTimeR->Release();
		g_pTextureTimeR = NULL;
	}
}

//-----------------------------------------------------------------------------
// リザルトスコアの更新処理
//-----------------------------------------------------------------------------
void UpdateResultScore(void)
{
	VERTEX_2D *pVertex;
	TIME *pTime;
	int nScore;
	int nMinute, nSecond;
	pTime = GetTimeScore();

	int aNumber[RESULTSCORE_POLYGON];

	//ゴールタイム
	nScore = pTime->dwFinishTime - pTime->dwStartTime;

	nMinute = (nScore / 1000) / 60;
	nSecond = (nScore / 1000) % 60;

	//時間を分解
	aNumber[0] = nMinute % 100 / 10;
	aNumber[1] = nMinute % 10 / 1;
	aNumber[2] = nSecond % 100 / 10;
	aNumber[3] = nSecond % 10 / 1;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffTimeR->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntChar = 0; nCntChar < RESULTSCORE_POLYGON; nCntChar++)
	{
		if (nCntChar == 2)
		{
			//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3((g_aResultScore[nCntChar].pos.x) + (nCntChar + 1) * RESULTSCORE_SPACE, (g_aResultScore[nCntChar].pos.y + RESULTSCORE_HEIGHT), 0.0f);		 //TRIANGLESTRIPで四角
			pVertex[1].pos = D3DXVECTOR3((g_aResultScore[nCntChar].pos.x) + (nCntChar + 1) * RESULTSCORE_SPACE, (g_aResultScore[nCntChar].pos.y), 0.0f);
			pVertex[2].pos = D3DXVECTOR3((g_aResultScore[nCntChar].pos.x + RESULTSCORE_WIDTH) + (nCntChar + 1) * RESULTSCORE_SPACE, (g_aResultScore[nCntChar].pos.y + RESULTSCORE_HEIGHT), 0.0f);
			pVertex[3].pos = D3DXVECTOR3((g_aResultScore[nCntChar].pos.x + RESULTSCORE_WIDTH) + (nCntChar + 1) * RESULTSCORE_SPACE, (g_aResultScore[nCntChar].pos.y), 0.0f);
		}
		else if (nCntChar > 2)
		{
			//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3((g_aResultScore[nCntChar].pos.x) + (nCntChar + 1) * RESULTSCORE_SPACE, (g_aResultScore[nCntChar].pos.y + RESULTSCORE_HEIGHT), 0.0f);		 //TRIANGLESTRIPで四角
			pVertex[1].pos = D3DXVECTOR3((g_aResultScore[nCntChar].pos.x) + (nCntChar + 1) * RESULTSCORE_SPACE, (g_aResultScore[nCntChar].pos.y), 0.0f);
			pVertex[2].pos = D3DXVECTOR3((g_aResultScore[nCntChar].pos.x + RESULTSCORE_WIDTH) + (nCntChar + 1) * RESULTSCORE_SPACE, (g_aResultScore[nCntChar].pos.y + RESULTSCORE_HEIGHT), 0.0f);
			pVertex[3].pos = D3DXVECTOR3((g_aResultScore[nCntChar].pos.x + RESULTSCORE_WIDTH) + (nCntChar + 1) * RESULTSCORE_SPACE, (g_aResultScore[nCntChar].pos.y), 0.0f);

		}
		else
		{
			//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3((g_aResultScore[nCntChar].pos.x) + nCntChar * RESULTSCORE_SPACE, (g_aResultScore[nCntChar].pos.y + RESULTSCORE_HEIGHT), 0.0f);		 //TRIANGLESTRIPで四角
			pVertex[1].pos = D3DXVECTOR3((g_aResultScore[nCntChar].pos.x) + nCntChar * RESULTSCORE_SPACE, (g_aResultScore[nCntChar].pos.y), 0.0f);
			pVertex[2].pos = D3DXVECTOR3((g_aResultScore[nCntChar].pos.x + RESULTSCORE_WIDTH) + nCntChar * RESULTSCORE_SPACE, (g_aResultScore[nCntChar].pos.y + RESULTSCORE_HEIGHT), 0.0f);
			pVertex[3].pos = D3DXVECTOR3((g_aResultScore[nCntChar].pos.x + RESULTSCORE_WIDTH) + nCntChar * RESULTSCORE_SPACE, (g_aResultScore[nCntChar].pos.y), 0.0f);
		}

		//テクスチャ座標の更新
		pVertex[0].tex = D3DXVECTOR2(0.1f * (aNumber[nCntChar]), 1.0f);
		pVertex[1].tex = D3DXVECTOR2(0.1f * (aNumber[nCntChar]), 0.0f);
		pVertex[2].tex = D3DXVECTOR2(0.1f * (aNumber[nCntChar] + 1), 1.0f);
		pVertex[3].tex = D3DXVECTOR2(0.1f * (aNumber[nCntChar] + 1), 0.0f);

		pVertex += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTimeR->Unlock();
}

//-----------------------------------------------------------------------------
// クリアタイムの描画処理
//-----------------------------------------------------------------------------
void DrawResultScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTimeR, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, g_pTextureTimeR);

	for (int nCnt = 0; nCnt < RESULTSCORE_POLYGON; nCnt++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}
}