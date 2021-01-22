#define _CRT_SECURE_NO_WARNINGS
//-----------------------------------------------------------------------------
//
// ランキングスコアの処理 [rankedscore.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "rankedscore.h"
#include "input.h"
#include "fade.h"
#include "timescore.h"
#include <stdio.h>

//-----------------------------------------------------------------------------
// マクロ
//-----------------------------------------------------------------------------
#define RSCORE_ROW (5)															//スコアの行
#define RSCORE_COLUMN (4)														//スコアの列
#define RSCORE_DIGIT (5)														//スコアの桁
#define RSCORE_POLYGON (RSCORE_ROW * RSCORE_COLUMN + 1)								//配置オブジェクト数
#define RSCORE_TEX (2)															//使用するテクスチャの数

#define RSCORE_POS_X (0)														//スコアの位置X
#define RSCORE_POS_Y (0)														//スコアの位置Y

#define RSCORE_WIDTH (50)														//文字の幅
#define RSCORE_HEIGHT (100)														//文字の高さ
#define RSCORE_SPACE_X (100)													//文字スペース 横
#define RSCORE_SPACE_Y (138)												//文字スペース 縦

#define RSCORE_NEW_WIDTH (200)
#define RSCORE_NEW_HEIGHT (100)

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
void SortScore(int nRow);
void ConvertTime(void);
void LoadRanking(void);
void SaveRanking(void);
void IncludeNewScore(void);

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffRScore = NULL;								//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureRScore[RSCORE_TEX] = { NULL };									//ポリゴンのテクスチャへのポインタ
D3DXVECTOR3 g_NewScorePos;
RSCORE g_aRscore[RSCORE_ROW][RSCORE_COLUMN];									//RSCOREの情報
SCORE g_aScore[RSCORE_ROW + 1];													//スコアの配列

//-----------------------------------------------------------------------------
// ランキングスコアの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitRScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;
	int aNumber[RSCORE_ROW][RSCORE_COLUMN];

	//初期化
	for (int nCntRow = 0; nCntRow < RSCORE_ROW; nCntRow++)
	{
		for (int nCntColumn = 0; nCntColumn < RSCORE_COLUMN; nCntColumn++)
		{
			g_aRscore[nCntRow][nCntColumn].pos = D3DXVECTOR3(580, 160, 0.0f);
		}
		g_aScore[nCntRow].score = 9999999;
		g_aScore[nCntRow].hour = 0;
		g_aScore[nCntRow].minute = 0;
		g_aScore[nCntRow].second = 0;
		g_NewScorePos = D3DXVECTOR3(200, 0, 0.0f);
	}
	
	//ランキングをロード
	LoadRanking();

	//スコアの並べ替え
	SortScore(RSCORE_ROW - 1);

	//新しいスコアを含んでソート
	IncludeNewScore();

	//時間変換
	ConvertTime();

	//スコア分解
	for (int nCntRow = 0; nCntRow < RSCORE_ROW; nCntRow++)
	{
		aNumber[nCntRow][0] = g_aScore[nCntRow].minute % 100 / 10;
		aNumber[nCntRow][1] = g_aScore[nCntRow].minute % 10 / 1;
		aNumber[nCntRow][2] = g_aScore[nCntRow].second % 100 / 10;
		aNumber[nCntRow][3] = g_aScore[nCntRow].second % 10 / 1;
	}

	//ランキングをセーブ
	SaveRanking();

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	//D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\new_ranking.png", &g_pTextureRScore[0]);
	D3DXCreateTextureFromFileEx(pDevice, "data\\TEXTURE\\new_ranking.png", 310, 80, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0xFF000000, NULL, NULL, &g_pTextureRScore[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number000.png", &g_pTextureRScore[1]);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * RSCORE_POLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffRScore, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffRScore->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].pos = D3DXVECTOR3(g_NewScorePos.x, g_NewScorePos.y + RSCORE_NEW_HEIGHT, 0.0f);
	pVertex[1].pos = D3DXVECTOR3(g_NewScorePos.x, g_NewScorePos.y, 0.0f);
	pVertex[2].pos = D3DXVECTOR3(g_NewScorePos.x + RSCORE_NEW_WIDTH, g_NewScorePos.y + RSCORE_NEW_HEIGHT, 0.0f);
	pVertex[3].pos = D3DXVECTOR3(g_NewScorePos.x + RSCORE_NEW_WIDTH, g_NewScorePos.y, 0.0f);

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

	//テクスチャ座標の更新
	//頂点情報の設定
	pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	pVertex += 4;

	for (int nCntRow = 0; nCntRow < RSCORE_ROW; nCntRow++)
	{
		for (int nCntColumn = 0; nCntColumn < RSCORE_COLUMN; nCntColumn++)
		{
			if (nCntColumn == 2)
			{
				//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
				pVertex[0].pos = D3DXVECTOR3((g_aRscore[nCntRow][nCntColumn].pos.x) + (nCntColumn + 1) * RSCORE_SPACE_X, (g_aRscore[nCntRow][nCntColumn].pos.y + RSCORE_HEIGHT) + nCntRow * RSCORE_SPACE_Y, 0.0f);		 //TRIANGLESTRIPで四角
				pVertex[1].pos = D3DXVECTOR3((g_aRscore[nCntRow][nCntColumn].pos.x) + (nCntColumn + 1) * RSCORE_SPACE_X, (g_aRscore[nCntRow][nCntColumn].pos.y) + nCntRow * RSCORE_SPACE_Y, 0.0f);
				pVertex[2].pos = D3DXVECTOR3((g_aRscore[nCntRow][nCntColumn].pos.x + RSCORE_WIDTH) + (nCntColumn + 1) * RSCORE_SPACE_X, (g_aRscore[nCntRow][nCntColumn].pos.y + RSCORE_HEIGHT) + nCntRow * RSCORE_SPACE_Y, 0.0f);
				pVertex[3].pos = D3DXVECTOR3((g_aRscore[nCntRow][nCntColumn].pos.x + RSCORE_WIDTH) + (nCntColumn + 1) * RSCORE_SPACE_X, (g_aRscore[nCntRow][nCntColumn].pos.y) + nCntRow * RSCORE_SPACE_Y, 0.0f);

			}
			else if (nCntColumn > 2)
			{
				//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
				pVertex[0].pos = D3DXVECTOR3((g_aRscore[nCntRow][nCntColumn].pos.x) + (nCntColumn + 1) * RSCORE_SPACE_X, (g_aRscore[nCntRow][nCntColumn].pos.y + RSCORE_HEIGHT) + nCntRow * RSCORE_SPACE_Y, 0.0f);		 //TRIANGLESTRIPで四角
				pVertex[1].pos = D3DXVECTOR3((g_aRscore[nCntRow][nCntColumn].pos.x) + (nCntColumn + 1) * RSCORE_SPACE_X, (g_aRscore[nCntRow][nCntColumn].pos.y) + nCntRow * RSCORE_SPACE_Y, 0.0f);
				pVertex[2].pos = D3DXVECTOR3((g_aRscore[nCntRow][nCntColumn].pos.x + RSCORE_WIDTH) + (nCntColumn + 1) * RSCORE_SPACE_X, (g_aRscore[nCntRow][nCntColumn].pos.y + RSCORE_HEIGHT) + nCntRow * RSCORE_SPACE_Y, 0.0f);
				pVertex[3].pos = D3DXVECTOR3((g_aRscore[nCntRow][nCntColumn].pos.x + RSCORE_WIDTH) + (nCntColumn + 1) * RSCORE_SPACE_X, (g_aRscore[nCntRow][nCntColumn].pos.y) + nCntRow * RSCORE_SPACE_Y, 0.0f);

			}
			else
			{
				//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
				pVertex[0].pos = D3DXVECTOR3((g_aRscore[nCntRow][nCntColumn].pos.x) + nCntColumn * RSCORE_SPACE_X, (g_aRscore[nCntRow][nCntColumn].pos.y + RSCORE_HEIGHT) + nCntRow * RSCORE_SPACE_Y, 0.0f);		 //TRIANGLESTRIPで四角
				pVertex[1].pos = D3DXVECTOR3((g_aRscore[nCntRow][nCntColumn].pos.x) + nCntColumn * RSCORE_SPACE_X, (g_aRscore[nCntRow][nCntColumn].pos.y) + nCntRow * RSCORE_SPACE_Y, 0.0f);
				pVertex[2].pos = D3DXVECTOR3((g_aRscore[nCntRow][nCntColumn].pos.x + RSCORE_WIDTH) + nCntColumn * RSCORE_SPACE_X, (g_aRscore[nCntRow][nCntColumn].pos.y + RSCORE_HEIGHT) + nCntRow * RSCORE_SPACE_Y, 0.0f);
				pVertex[3].pos = D3DXVECTOR3((g_aRscore[nCntRow][nCntColumn].pos.x + RSCORE_WIDTH) + nCntColumn * RSCORE_SPACE_X, (g_aRscore[nCntRow][nCntColumn].pos.y) + nCntRow * RSCORE_SPACE_Y, 0.0f);
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

			//テクスチャ座標の更新
			pVertex[0].tex = D3DXVECTOR2(0.1f * (aNumber[nCntRow][nCntColumn]), 1.0f);
			pVertex[1].tex = D3DXVECTOR2(0.1f * (aNumber[nCntRow][nCntColumn]), 0.0f);
			pVertex[2].tex = D3DXVECTOR2(0.1f * (aNumber[nCntRow][nCntColumn] + 1), 1.0f);
			pVertex[3].tex = D3DXVECTOR2(0.1f * (aNumber[nCntRow][nCntColumn] + 1), 0.0f);

			pVertex += 4; //頂点ずらし
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffRScore->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
// ランキングスコアの終了処理
//-----------------------------------------------------------------------------
void UninitRScore(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffRScore != NULL)
	{
		g_pVtxBuffRScore->Release();
		g_pVtxBuffRScore = NULL;
	}

	for (int nCnt = 0; nCnt < RSCORE_TEX; nCnt++)
	{
		//テクスチャの開放
		if (g_pTextureRScore[nCnt] != NULL)
		{
			g_pTextureRScore[nCnt]->Release();
			g_pTextureRScore[nCnt] = NULL;
		}
	}


}

//-----------------------------------------------------------------------------
// ランキングスコアの更新処理
//-----------------------------------------------------------------------------
void UpdateRScore(void)
{
}

//-----------------------------------------------------------------------------
// ランキングスコアの描画処理
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



	for (int nCntTex = 0; nCntTex < RSCORE_POLYGON; nCntTex++)
	{
		switch (nCntTex)
		{
		case 0:
			//テクスチャ設定
			pDevice->SetTexture(0, g_pTextureRScore[0]);
			break;

		default:
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureRScore[1]);
			break;
		}
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTex * 4, 2);
	}
}

//-----------------------------------------------------------------------------
// スコア並べ替え
//-----------------------------------------------------------------------------
void SortScore(int nRow)
{
	int nSubData;
	for (int nCnt1 = 0; nCnt1 < nRow + 1; nCnt1++)
	{
		for (int nCnt2 = nCnt1 + 1; nCnt2 < nRow + 1; nCnt2++)
		{
			if (g_aScore[nCnt1].score > g_aScore[nCnt2].score)
			{
				nSubData = g_aScore[nCnt1].score;
				g_aScore[nCnt1].score = g_aScore[nCnt2].score;
				g_aScore[nCnt2].score = nSubData;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 秒数を変換
//-----------------------------------------------------------------------------
void ConvertTime(void)
{
	for (int nCntRow = 0; nCntRow < RSCORE_ROW; nCntRow++)
	{
		g_aScore[nCntRow].minute = (g_aScore[nCntRow].score / 1000) / 60;
		g_aScore[nCntRow].second = (g_aScore[nCntRow].score / 1000) % 60;
	}

	for (int nCnt = 0; nCnt < RSCORE_ROW; nCnt++)
	{
		if (g_aScore[nCnt].minute >= 100)
		{
			g_aScore[nCnt].minute = 99;
			g_aScore[nCnt].second = 99;
		}
	}
}

//-----------------------------------------------------------------------------
// ランキングをロード
//-----------------------------------------------------------------------------
void LoadRanking(void)
{
	FILE *pFile;

	pFile = fopen("leaderboard.txt", "r");
	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < RSCORE_ROW; nCnt++)
		{
			fscanf(pFile, "%d", &g_aScore[nCnt].score);
		}
	}
	fclose(pFile);
}

//-----------------------------------------------------------------------------
// ランキングをセーブ
//-----------------------------------------------------------------------------
void SaveRanking(void)
{
	FILE *pFile;

	pFile = fopen("leaderboard.txt", "w");
	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < RSCORE_ROW; nCnt++)
		{
			fprintf(pFile, "%d\n", g_aScore[nCnt].score);
		}
	}
	fclose(pFile);
}

//-----------------------------------------------------------------------------
// ニュースコアを含めてソート
//-----------------------------------------------------------------------------
void IncludeNewScore(void)
{
	TIME *pTimeScore;
	int nNewScore;

	pTimeScore = GetTimeScore();

	LoadRanking();

	g_aScore[RSCORE_ROW].score = pTimeScore->dwFinishTime - pTimeScore->dwStartTime;
	nNewScore = pTimeScore->dwFinishTime - pTimeScore->dwStartTime;

	SortScore(RSCORE_ROW);

	for (int nCnt = 0; nCnt < RSCORE_ROW; nCnt++)
	{
		if (g_aScore[nCnt].score == nNewScore)
		{
			g_NewScorePos = D3DXVECTOR3(g_aRscore[0][0].pos.x + 500, g_aRscore[0][0].pos.y + nCnt * RSCORE_SPACE_Y, 0.0f);
			break;
		}
		else
		{
			g_NewScorePos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		}
	}



}