//-----------------------------------------------------------------------------
//
// スコアの処理 [score.cpp]
// Author; Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "score.h"

//-----------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------
#define MAX_SCORE (8) //スコア数値の数

//-----------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL; //バッファへのポインタ
D3DXVECTOR3 g_aPosScore[MAX_SCORE];				//スコアの位置
int g_nScore;									//スコア

//-----------------------------------------------------------------------------
//スコアの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number001.png", &g_pTextureScore);

	//スコアの初期化
	g_nScore = 0;

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * MAX_SCORE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffScore, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファのロック
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVertex, 0);

	//頂点座標 
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		g_aPosScore[nCntScore] = D3DXVECTOR3(900, 30, 0.0f);

		//頂点座標の設定
		pVertex[0].pos = D3DXVECTOR3((g_aPosScore[nCntScore].x + -15) + nCntScore * 50, g_aPosScore[nCntScore].y + 20, 0.0f);
		pVertex[1].pos = D3DXVECTOR3((g_aPosScore[nCntScore].x + -15) + nCntScore * 50, g_aPosScore[nCntScore].y + -20, 0.0f);
		pVertex[2].pos = D3DXVECTOR3((g_aPosScore[nCntScore].x + 15) + nCntScore * 50, g_aPosScore[nCntScore].y + 20, 0.0f);
		pVertex[3].pos = D3DXVECTOR3((g_aPosScore[nCntScore].x + 15) + nCntScore * 50, g_aPosScore[nCntScore].y + -20, 0.0f);

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

		//UVの設定
		pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVertex[2].tex = D3DXVECTOR2(1.0f * 0.1f, 1.0f);
		pVertex[3].tex = D3DXVECTOR2(1.0f * 0.1f, 0.0f);

		pVertex += 4; //頂点のポインタをずらす
	}

	//頂点バッファのアンロック
	g_pVtxBuffScore->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//スコアの終了処理
//-----------------------------------------------------------------------------
void UninitScore(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}

	//テクスチャの開放
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}
}

//-----------------------------------------------------------------------------
//スコアの更新処理
//-----------------------------------------------------------------------------
void UpdateScore(void)
{
	VERTEX_2D *pVertex;
	int aNumber[8];

	//スコアの分解
	aNumber[0] = g_nScore % 100000000 / 10000000;
	aNumber[1] = g_nScore % 10000000 / 1000000;
	aNumber[2] = g_nScore % 1000000 / 100000;
	aNumber[3] = g_nScore % 100000 / 10000;
	aNumber[4] = g_nScore % 10000 / 1000;
	aNumber[5] = g_nScore % 1000 / 100;
	aNumber[6] = g_nScore % 100 / 10;
	aNumber[7] = g_nScore % 10 / 1;

	if (g_pVtxBuffScore != NULL)
	{
		//頂点バッファのロック
		g_pVtxBuffScore->Lock(0, 0, (void**)&pVertex, 0);

		for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
		{
			//頂点座標の設定
			pVertex[0].pos = D3DXVECTOR3((g_aPosScore[nCntScore].x + -15) + nCntScore * 50, g_aPosScore[nCntScore].y + 20, 0.0f);
			pVertex[1].pos = D3DXVECTOR3((g_aPosScore[nCntScore].x + -15) + nCntScore * 50, g_aPosScore[nCntScore].y + -20, 0.0f);
			pVertex[2].pos = D3DXVECTOR3((g_aPosScore[nCntScore].x + 15) + nCntScore * 50, g_aPosScore[nCntScore].y + 20, 0.0f);
			pVertex[3].pos = D3DXVECTOR3((g_aPosScore[nCntScore].x + 15) + nCntScore * 50, g_aPosScore[nCntScore].y + -20, 0.0f);

			//テクスチャ座標の更新
			pVertex[0].tex = D3DXVECTOR2(0.1f * (aNumber[nCntScore]), 1.0f);
			pVertex[1].tex = D3DXVECTOR2(0.1f * (aNumber[nCntScore]), 0.0f);
			pVertex[2].tex = D3DXVECTOR2(0.1f * (aNumber[nCntScore] + 1), 1.0f);
			pVertex[3].tex = D3DXVECTOR2(0.1f * (aNumber[nCntScore] + 1), 0.0f);

			pVertex += 4;
		}

		//頂点バッファのアンロック
		g_pVtxBuffScore->Unlock();
	}
}

//-----------------------------------------------------------------------------
//スコアの描画処理
//-----------------------------------------------------------------------------
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureScore);

	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}
}

//-----------------------------------------------------------------------------
//スコアの追加処理
//-----------------------------------------------------------------------------
void AddScore(int nValue)
{
	g_nScore += nValue;
}

//スコアの取得
int GetScore(void)
{
	return g_nScore;
}