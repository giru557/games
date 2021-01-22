//-----------------------------------------------------------------------------
//
// 背景の処理 [BG.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "BG.h"
#include "player.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define BG_TEX (2)
#define BG_POLYGON (1)
#define BG_MOUNTAIN_WIDTH (1900)
#define BG_MOUNTAIN_HEIGHT (900)
#define BG_SKY_WIDTH (SCREEN_WIDTH / 2)
#define BG_SKY_HEIGHT (SCREEN_HEIGHT / 2)
#define BG_SCROLL_SPEED (0.0005f)

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffBG = NULL;					// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureBG[BG_TEX] = { NULL };				// ポリゴンのテクスチャへのポインタ
BG g_aBG[BG_POLYGON];											// BGの情報

//-----------------------------------------------------------------------------
// 背景の初期化処理
//-----------------------------------------------------------------------------
HRESULT InitBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	g_aBG[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	g_aBG[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - BG_MOUNTAIN_HEIGHT, 0.0f);

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Background.png", &g_pTextureBG[0]);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * BG_TEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBG, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntBG = 0; nCntBG < BG_POLYGON; nCntBG++)
	{
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

		switch (nCntBG)
		{
		case 0:
			//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(g_aBG[nCntBG].pos.x - BG_MOUNTAIN_WIDTH, g_aBG[nCntBG].pos.y + BG_MOUNTAIN_HEIGHT, 0.0f);
			pVertex[1].pos = D3DXVECTOR3(g_aBG[nCntBG].pos.x - BG_MOUNTAIN_WIDTH, g_aBG[nCntBG].pos.y - BG_MOUNTAIN_HEIGHT, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_aBG[nCntBG].pos.x + BG_MOUNTAIN_WIDTH, g_aBG[nCntBG].pos.y + BG_MOUNTAIN_HEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_aBG[nCntBG].pos.x + BG_MOUNTAIN_WIDTH, g_aBG[nCntBG].pos.y - BG_MOUNTAIN_HEIGHT, 0.0f);
			break;
		}

		//頂点バッファずらす
		pVertex += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBG->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
// 背景の終了処理
//-----------------------------------------------------------------------------
void UninitBG(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffBG != NULL)
	{
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG = NULL;
	}

	for (int nCntBG = 0; nCntBG < BG_TEX; nCntBG++)
	{
		//テクスチャの開放
		if (g_pTextureBG[nCntBG] != NULL)
		{
			g_pTextureBG[nCntBG]->Release();
			g_pTextureBG[nCntBG] = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
// 背景の更新処理
//-----------------------------------------------------------------------------
void UpdateBG(void)
{
	VERTEX_2D *pVertex;
	PLAYER *pPlayer;
	float fScrollSpeed = BG_SCROLL_SPEED;
	pPlayer = GetPlayer();

	if (pPlayer->move.x > 1)
	{
		g_aBG[0].scroll.x += fScrollSpeed;
	}
	else if (pPlayer->move.x < -1)
	{
		g_aBG[0].scroll.x -= fScrollSpeed;
	}
	else
	{
		g_aBG[0].scroll.x += 0;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntBG = 0; nCntBG < BG_POLYGON; nCntBG++)
	{
		switch (nCntBG)
		{
		case 0:
				//頂点情報の設定
				pVertex[0].tex = D3DXVECTOR2(0.0f + g_aBG[nCntBG].scroll.x, 1.0f);
				pVertex[1].tex = D3DXVECTOR2(0.0f + g_aBG[nCntBG].scroll.x, 0.0f);
				pVertex[2].tex = D3DXVECTOR2(1.0f + g_aBG[nCntBG].scroll.x, 1.0f);
				pVertex[3].tex = D3DXVECTOR2(1.0f + g_aBG[nCntBG].scroll.x, 0.0f);
			break;

		default:
			break;
		}
		pVertex += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBG->Unlock();
}

//-----------------------------------------------------------------------------
// 背景の描画処理
//-----------------------------------------------------------------------------
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBG = 0; nCntBG < BG_POLYGON; nCntBG++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBG[nCntBG]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBG * 4, 2);
	}
}