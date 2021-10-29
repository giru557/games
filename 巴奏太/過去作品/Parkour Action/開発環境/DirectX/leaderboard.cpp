//-----------------------------------------------------------------------------
//
// ランキングの処理 [leaderboard.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "leaderboard.h"
#include "input.h"
#include "fade.h"

//-----------------------------------------------------------------------------
// マクロ
//-----------------------------------------------------------------------------
#define RANKING_POLYGON (1)

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffRanking = NULL;							//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureRanking[RANKING_POLYGON] = { NULL };			//ポリゴンのテクスチャへのポインタ

//-----------------------------------------------------------------------------
// ランキングの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\ranking.png", &g_pTextureRanking[0]);


	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * RANKING_POLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffRanking, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntT = 0; nCntT < RANKING_POLYGON; nCntT++)
	{
		//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
		pVertex[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);		 //TRIANGLESTRIPで四角
		pVertex[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVertex[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		pVertex[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

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
		pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVertex += 4; //頂点ずらし
	}

	//頂点バッファをアンロックする
	g_pVtxBuffRanking->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
// ランキングの終了処理
//-----------------------------------------------------------------------------
void UninitRanking(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffRanking != NULL)
	{
		g_pVtxBuffRanking->Release();
		g_pVtxBuffRanking = NULL;
	}

	for (int nCntTex = 0; nCntTex < RANKING_POLYGON; nCntTex++)
	{
		//テクスチャの開放
		if (g_pTextureRanking[nCntTex] != NULL)
		{
			g_pTextureRanking[nCntTex]->Release();
			g_pTextureRanking[nCntTex] = NULL;
		}
	}

}

//-----------------------------------------------------------------------------
// ランキングの更新処理
//-----------------------------------------------------------------------------
void UpdateRanking(void)
{
	VERTEX_2D *pVertex;

	if (g_pVtxBuffRanking != NULL)
	{
		//頂点バッファをロックし頂点情報へのポインタを取得
		g_pVtxBuffRanking->Lock(0, 0, (void**)&pVertex, 0);

		for (int nCntT = 0; nCntT < RANKING_POLYGON; nCntT++)
		{
			pVertex += 4;
		}

		//頂点バッファをアンロックする
		g_pVtxBuffRanking->Unlock();
	}

	if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_BACKSPACE) == true)
	{
		SetFade(FADE_OUT, MODE_TITLE);
	}
}

//-----------------------------------------------------------------------------
// ランキングの描画処理
//-----------------------------------------------------------------------------
void DrawRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRanking, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (int nCntTex = 0; nCntTex < RANKING_POLYGON; nCntTex++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureRanking[nCntTex]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTex * 4, 2);
	}
}