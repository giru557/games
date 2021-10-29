//-----------------------------------------------------------------------------
//
// クリアタイムの処理 [timescore.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "timescore.h"
#include "player.h"
#include "goalflag.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define TIMES_POLYGON (1)		//使用するオブジェ数
#define TIMES_TEX (2)			//テクスチャ枚数
#define TIMES_DIGIT (4)			//数字の桁数

#define TIMES_CHARWIDTH (60 * 0.5f)	//文字の幅
#define TIMES_CHARHEIGHT (90 * 0.5f)	//文字の高さ
#define TIMES_CHARSPACE (150 * 0.5f)	//文字の間隔
#define TIMES_SPACEWIDTH (20 * 0.5f)
#define TIMES_SPACEHEIGHT (60 * 0.5f)


//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
void ConvertCurrentTime(void);

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffTimeS = NULL;					// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureTimeS[TIMES_TEX] = { NULL };			// ポリゴンのテクスチャへのポインタ
TIMESCORE g_aTimeScore[TIMES_DIGIT];								// TIMESCOREの情報
TIME g_Time;														// 時間情報
D3DXVECTOR3 g_SpacePos;												// スペースの場所

//-----------------------------------------------------------------------------
// クリアタイムの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitTimeScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;
	GOAL *pGoal;
	PLAYER *pPlayer;
	
	pPlayer = GetPlayer();
	pGoal = GetGoal();

	//TIMESCORE構造体の初期化
	for (int nCntTS = 0; nCntTS < TIMES_DIGIT; nCntTS++)
	{
		g_aTimeScore[nCntTS].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	g_SpacePos = D3DXVECTOR3(g_aTimeScore[0].pos.x + 120, g_aTimeScore[0].pos.y + 7, 0);

	//TIME構造体初期化
	if (pGoal->stage == STAGE_1 &&
		pPlayer->damage != DAMAGESTATE_DIE)
	{
		g_Time.bStartFlag = false;
		g_Time.dwCurrentTime = NULL;
		g_Time.dwFinishTime = NULL;
		g_Time.dwStartTime = NULL;
		g_Time.dwSecond = NULL;
		g_Time.dwMinute = NULL;
	}

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number000.png", &g_pTextureTimeS[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\_.png", &g_pTextureTimeS[1]);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * (TIMES_DIGIT + 1), D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTimeS, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffTimeS->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntChar = 0; nCntChar < TIMES_DIGIT + 1; nCntChar++)
	{
		if (nCntChar == TIMES_DIGIT)
		{
			//頂点座標の設定
			pVertex[0].pos = D3DXVECTOR3((g_SpacePos.x), g_SpacePos.y + TIMES_SPACEHEIGHT, 0.0f);
			pVertex[1].pos = D3DXVECTOR3((g_SpacePos.x), g_SpacePos.y, 0.0f);
			pVertex[2].pos = D3DXVECTOR3((g_SpacePos.x + TIMES_SPACEWIDTH), g_SpacePos.y + TIMES_SPACEHEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3((g_SpacePos.x + TIMES_SPACEWIDTH), g_SpacePos.y, 0.0f);
		}
		else
		{
			//頂点座標の設定
			pVertex[0].pos = D3DXVECTOR3((g_aTimeScore[nCntChar].pos.x) + nCntChar * TIMES_CHARSPACE, g_aTimeScore[nCntChar].pos.y + TIMES_CHARHEIGHT, 0.0f);
			pVertex[1].pos = D3DXVECTOR3((g_aTimeScore[nCntChar].pos.x) + nCntChar * TIMES_CHARSPACE, g_aTimeScore[nCntChar].pos.y, 0.0f);
			pVertex[2].pos = D3DXVECTOR3((g_aTimeScore[nCntChar].pos.x + TIMES_CHARWIDTH) + nCntChar * TIMES_CHARSPACE, g_aTimeScore[nCntChar].pos.y + TIMES_CHARHEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3((g_aTimeScore[nCntChar].pos.x + TIMES_CHARWIDTH) + nCntChar * TIMES_CHARSPACE, g_aTimeScore[nCntChar].pos.y, 0.0f);
		}

		//rhwの設定
		pVertex[0].rhw = 1.0f;	//1.0f固定
		pVertex[1].rhw = 1.0f;
		pVertex[2].rhw = 1.0f;
		pVertex[3].rhw = 1.0f;


		if (nCntChar == TIMES_DIGIT)
		{
			//頂点情報の設定
			pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f);
		}
		else
		{
			//頂点情報の設定
			pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVertex[2].tex = D3DXVECTOR2(1.0f * 0.1f, 1.0f);
			pVertex[3].tex = D3DXVECTOR2(1.0f * 0.1f, 0.0f);
		}

		//頂点カラーの設定
		pVertex[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);  //D3DCOLOR_RGBA(赤, 緑, 青, 透明度);
		pVertex[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVertex[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVertex[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//頂点バッファずらす
		pVertex += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTimeS->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
// クリアタイムの終了処理
//-----------------------------------------------------------------------------
void UninitTimeScore(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffTimeS != NULL)
	{
		g_pVtxBuffTimeS->Release();
		g_pVtxBuffTimeS = NULL;
	}

	for (int nCnt = 0; nCnt < TIMES_TEX; nCnt++)
	{
		//テクスチャの開放
		if (g_pTextureTimeS[nCnt] != NULL)
		{
			g_pTextureTimeS[nCnt]->Release();
			g_pTextureTimeS[nCnt] = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
// クリアタイムの更新処理
//-----------------------------------------------------------------------------
void UpdateTimeScore(void)
{
	VERTEX_2D *pVertex;
	PLAYER *pPlayer;
	GOAL *pGoal;
	int aNumber[TIMES_DIGIT];
	pGoal = GetGoal();
	pPlayer = GetPlayer();

	//開始時間
	if (pGoal->stage == STAGE_1 &&
		pPlayer->damage != DAMAGESTATE_DIE)
	{
		if (pPlayer->move.x != 0)
		{
			if (g_Time.bStartFlag == false)
			{
				g_Time.dwStartTime = timeGetTime();
				g_Time.bStartFlag = true;
			}
		}
	}

	//現在時間の変換
	if (g_Time.bStartFlag == true)
	{
		ConvertCurrentTime();
	}

	//時間を分解
	aNumber[0] = g_Time.dwMinute % 100 / 10;
	aNumber[1] = g_Time.dwMinute % 10 / 1;
	aNumber[2] = g_Time.dwSecond % 100 / 10;
	aNumber[3] = g_Time.dwSecond % 10 / 1;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffTimeS->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntChar = 0; nCntChar < TIMES_DIGIT + 1; nCntChar++)
	{
		if (nCntChar == TIMES_DIGIT)
		{
			//頂点座標の設定
			pVertex[0].pos = D3DXVECTOR3((g_SpacePos.x), g_SpacePos.y + TIMES_SPACEHEIGHT, 0.0f);
			pVertex[1].pos = D3DXVECTOR3((g_SpacePos.x), g_SpacePos.y, 0.0f);
			pVertex[2].pos = D3DXVECTOR3((g_SpacePos.x + TIMES_SPACEWIDTH), g_SpacePos.y + TIMES_SPACEHEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3((g_SpacePos.x + TIMES_SPACEWIDTH), g_SpacePos.y, 0.0f);
		}
		else
		{
			//頂点座標の設定
			pVertex[0].pos = D3DXVECTOR3((g_aTimeScore[nCntChar].pos.x) + nCntChar * TIMES_CHARSPACE, g_aTimeScore[nCntChar].pos.y + TIMES_CHARHEIGHT, 0.0f);
			pVertex[1].pos = D3DXVECTOR3((g_aTimeScore[nCntChar].pos.x) + nCntChar * TIMES_CHARSPACE, g_aTimeScore[nCntChar].pos.y, 0.0f);
			pVertex[2].pos = D3DXVECTOR3((g_aTimeScore[nCntChar].pos.x + TIMES_CHARWIDTH) + nCntChar * TIMES_CHARSPACE, g_aTimeScore[nCntChar].pos.y + TIMES_CHARHEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3((g_aTimeScore[nCntChar].pos.x + TIMES_CHARWIDTH) + nCntChar * TIMES_CHARSPACE, g_aTimeScore[nCntChar].pos.y, 0.0f);
		}

		if (nCntChar == TIMES_DIGIT)
		{
			//頂点情報の設定
			pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f);
		}
		else
		{
			//テクスチャ座標の更新
			pVertex[0].tex = D3DXVECTOR2(0.1f * (aNumber[nCntChar]), 1.0f);
			pVertex[1].tex = D3DXVECTOR2(0.1f * (aNumber[nCntChar]), 0.0f);
			pVertex[2].tex = D3DXVECTOR2(0.1f * (aNumber[nCntChar] + 1), 1.0f);
			pVertex[3].tex = D3DXVECTOR2(0.1f * (aNumber[nCntChar] + 1), 0.0f);
		}
		pVertex += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTimeS->Unlock();
}

//-----------------------------------------------------------------------------
// クリアタイムの描画処理
//-----------------------------------------------------------------------------
void DrawTimeScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTimeS, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntChar = 0; nCntChar < TIMES_DIGIT + 1; nCntChar++)
	{
		//テクスチャの設定
		if (nCntChar == TIMES_DIGIT)
		{
			pDevice->SetTexture(0, g_pTextureTimeS[1]);
		}
		else
		{
			pDevice->SetTexture(0, g_pTextureTimeS[0]);
		}

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntChar * 4, 2);
	}
}

//-----------------------------------------------------------------------------
// 現在時間の変換
//-----------------------------------------------------------------------------
void ConvertCurrentTime(void)
{
	//現在時間
	g_Time.dwCurrentTime = timeGetTime();
	g_Time.dwCurrentTime = (g_Time.dwCurrentTime - g_Time.dwStartTime) / 1000;

	//分秒に変換
	g_Time.dwMinute = g_Time.dwCurrentTime / 60;
	g_Time.dwSecond = g_Time.dwCurrentTime % 60;
}

//-----------------------------------------------------------------------------
// 時間情報取得
//-----------------------------------------------------------------------------
TIME *GetTimeScore(void)
{
	return &g_Time;
}