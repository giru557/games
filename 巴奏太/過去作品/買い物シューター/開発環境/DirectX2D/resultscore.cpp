#define _CRT_SECURE_NO_WARNINGS
//-----------------------------------------------------------------------------
//
// スコアの処理 [score.cpp]
// Author; Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "resultscore.h"
#include "score.h"
#include <stdio.h>

//-----------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------
#define MAX_SCORE (8) //スコアの桁数
#define SCORE_DISP (2)	//表示数
#define SCORE_CHAR_WIDTH (20)
#define SCORE_CHAR_HEIGHT (30)

//-----------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------
int ReturnHighestScore(void);
void InputHighestScore(int nScore);

//-----------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureRScore = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRScore = NULL; //バッファへのポインタ
D3DXVECTOR3 g_aPosRScore[MAX_SCORE][SCORE_DISP];				//スコアの位置
int g_aRScore[SCORE_DISP];									//スコア

//-----------------------------------------------------------------------------
//スコアの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitRScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number001.png", &g_pTextureRScore);

	//スコアの初期化
	g_aRScore[0] = GetScore();
	g_aRScore[1] = ReturnHighestScore();

	if (g_aRScore[0] > g_aRScore[1])
	{//ハイスコアより現在のスコアが良ければ
		InputHighestScore(g_aRScore[0]); //ハイスコアに現在のスコアを書き込み
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * (MAX_SCORE * SCORE_DISP), D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffRScore, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファのロック
	g_pVtxBuffRScore->Lock(0, 0, (void**)&pVertex, 0);

	//頂点座標 
	for (int nCntDisp = 0; nCntDisp < SCORE_DISP; nCntDisp++)
	{
		for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
		{
			switch (nCntDisp)
			{
			case 0:
				g_aPosRScore[nCntScore][nCntDisp] = D3DXVECTOR3(465, 322, 0.0f); //位置の設定
				break;

			case 1:
				g_aPosRScore[nCntScore][nCntDisp] = D3DXVECTOR3(465, 545, 0.0f); //位置の設定
				break;

			default:
				break;
			}

			//頂点座標の設定
			pVertex[0].pos = D3DXVECTOR3((g_aPosRScore[nCntScore][nCntDisp].x + -SCORE_CHAR_WIDTH) + nCntScore * 50, g_aPosRScore[nCntScore][nCntDisp].y + SCORE_CHAR_HEIGHT, 0.0f);
			pVertex[1].pos = D3DXVECTOR3((g_aPosRScore[nCntScore][nCntDisp].x + -SCORE_CHAR_WIDTH) + nCntScore * 50, g_aPosRScore[nCntScore][nCntDisp].y + -SCORE_CHAR_HEIGHT, 0.0f);
			pVertex[2].pos = D3DXVECTOR3((g_aPosRScore[nCntScore][nCntDisp].x + SCORE_CHAR_WIDTH) + nCntScore * 50, g_aPosRScore[nCntScore][nCntDisp].y + SCORE_CHAR_HEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3((g_aPosRScore[nCntScore][nCntDisp].x + SCORE_CHAR_WIDTH) + nCntScore * 50, g_aPosRScore[nCntScore][nCntDisp].y + -SCORE_CHAR_HEIGHT, 0.0f);

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
	}

	//頂点バッファのアンロック
	g_pVtxBuffRScore->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//スコアの終了処理
//-----------------------------------------------------------------------------
void UninitRScore(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffRScore != NULL)
	{
		g_pVtxBuffRScore->Release();
		g_pVtxBuffRScore = NULL;
	}

	//テクスチャの開放
	if (g_pTextureRScore != NULL)
	{
		g_pTextureRScore->Release();
		g_pTextureRScore = NULL;
	}
}

//-----------------------------------------------------------------------------
//スコアの更新処理
//-----------------------------------------------------------------------------
void UpdateRScore(void)
{
	VERTEX_2D *pVertex;
	int aNumber[MAX_SCORE][SCORE_DISP];

	//スコアの分解
	for (int nCntS = 0; nCntS < SCORE_DISP; nCntS++)
	{
		aNumber[0][nCntS] = g_aRScore[nCntS] % 100000000 / 10000000;
		aNumber[1][nCntS] = g_aRScore[nCntS] % 10000000 / 1000000;
		aNumber[2][nCntS] = g_aRScore[nCntS] % 1000000 / 100000;
		aNumber[3][nCntS] = g_aRScore[nCntS] % 100000 / 10000;
		aNumber[4][nCntS] = g_aRScore[nCntS] % 10000 / 1000;
		aNumber[5][nCntS] = g_aRScore[nCntS] % 1000 / 100;
		aNumber[6][nCntS] = g_aRScore[nCntS] % 100 / 10;
		aNumber[7][nCntS] = g_aRScore[nCntS] % 10 / 1;
	}

	if (g_pVtxBuffRScore != NULL)
	{
		//頂点バッファのロック
		g_pVtxBuffRScore->Lock(0, 0, (void**)&pVertex, 0);

		for (int nCntDisp = 0; nCntDisp < SCORE_DISP; nCntDisp++)
		{
			for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
			{
				//頂点座標の設定
				pVertex[0].pos = D3DXVECTOR3((g_aPosRScore[nCntScore][nCntDisp].x + -SCORE_CHAR_WIDTH) + nCntScore * 50, g_aPosRScore[nCntScore][nCntDisp].y + SCORE_CHAR_HEIGHT, 0.0f);
				pVertex[1].pos = D3DXVECTOR3((g_aPosRScore[nCntScore][nCntDisp].x + -SCORE_CHAR_WIDTH) + nCntScore * 50, g_aPosRScore[nCntScore][nCntDisp].y + -SCORE_CHAR_HEIGHT, 0.0f);
				pVertex[2].pos = D3DXVECTOR3((g_aPosRScore[nCntScore][nCntDisp].x + SCORE_CHAR_WIDTH) + nCntScore * 50, g_aPosRScore[nCntScore][nCntDisp].y + SCORE_CHAR_HEIGHT, 0.0f);
				pVertex[3].pos = D3DXVECTOR3((g_aPosRScore[nCntScore][nCntDisp].x + SCORE_CHAR_WIDTH) + nCntScore * 50, g_aPosRScore[nCntScore][nCntDisp].y + -SCORE_CHAR_HEIGHT, 0.0f);

				//テクスチャ座標の更新
				pVertex[0].tex = D3DXVECTOR2(0.1f * (aNumber[nCntScore][nCntDisp]), 1.0f);
				pVertex[1].tex = D3DXVECTOR2(0.1f * (aNumber[nCntScore][nCntDisp]), 0.0f);
				pVertex[2].tex = D3DXVECTOR2(0.1f * (aNumber[nCntScore][nCntDisp] + 1), 1.0f);
				pVertex[3].tex = D3DXVECTOR2(0.1f * (aNumber[nCntScore][nCntDisp] + 1), 0.0f);

				pVertex += 4;
			}
		}

		//頂点バッファのアンロック
		g_pVtxBuffRScore->Unlock();
	}
}

//-----------------------------------------------------------------------------
//スコアの描画処理
//-----------------------------------------------------------------------------
void DrawRScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRScore, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureRScore);

	for (int nCntScore = 0; nCntScore < (MAX_SCORE * SCORE_DISP); nCntScore++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}
}

//ハイスコア取得
int ReturnHighestScore(void)
{
	FILE *pFile;
	int nScore;

	//ファイル読み込み
	pFile = fopen("Highest.bin", "rb");
	if (pFile != NULL)
	{
		fscanf(pFile, "%d", &nScore);

		fclose(pFile);
	}

	return nScore;
}

//ハイスコア書き込み
void InputHighestScore(int nScore)
{
	FILE *pFile;

	//ファイル読み込み
	pFile = fopen("Highest.bin", "wb");
	if (pFile != NULL)
	{
		fprintf(pFile, "%d", nScore);

		fclose(pFile);
	}
}