//-----------------------------------------------------------------------------
//
// 体力の処理 [health.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "health.h"
#include "player.h"

//マクロ
#define HEALTH_POLYGON (2)
#define HEALTH_BAR_WIDTH (66 / 2 * HEALTH_SIZE_MULTIPLIER)
#define HEALTH_BAR_HEIGHT (19 / 2 * HEALTH_SIZE_MULTIPLIER)
#define HEALTH_WIDTH (51 * HEALTH_SIZE_MULTIPLIER)
#define HEALTH_HEIGHT (19 / 2 * HEALTH_SIZE_MULTIPLIER)
#define HEALTH_SIZE_MULTIPLIER (4)

//-----------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffHealth = NULL;  //頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureHealth[HEALTH_POLYGON] = { NULL };		//ポリゴンのテクスチャへのポインタ
D3DXVECTOR3 g_aPosHealth[HEALTH_POLYGON];							//体力の位置
int g_nOldLife;

//-----------------------------------------------------------------------------
//体力の初期化処理
//-----------------------------------------------------------------------------
HRESULT InitHealth(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;
	PLAYER *pPlayer;

	pPlayer = GetPlayer();

	g_aPosHealth[0] = D3DXVECTOR3((370), 600, 0.0f);
	g_aPosHealth[1] = D3DXVECTOR3((268 + 25), 600, 0);
	g_nOldLife = 50;
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	//D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\health_bar_decoration.png", &g_pTextureHealth[0]);
	D3DXCreateTextureFromFileEx(pDevice, "data\\TEXTURE\\health_bar_decoration.png", 66, 19, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0xFF000000, NULL, NULL, &g_pTextureHealth[0]);
	D3DXCreateTextureFromFileEx(pDevice, "data\\TEXTURE\\health_bar.png", 51, 19, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0xFF000000, NULL, NULL, &g_pTextureHealth[1]);


	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * HEALTH_POLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffHealth, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffHealth->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntH = 0; nCntH < HEALTH_POLYGON; nCntH++)
	{
		switch (nCntH)
		{
		case 0:
			//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(g_aPosHealth[nCntH].x - HEALTH_BAR_WIDTH, g_aPosHealth[nCntH].y + HEALTH_BAR_HEIGHT, 0.0f);		 //TRIANGLESTRIPで四角
			pVertex[1].pos = D3DXVECTOR3(g_aPosHealth[nCntH].x - HEALTH_BAR_WIDTH, g_aPosHealth[nCntH].y - HEALTH_BAR_HEIGHT, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_aPosHealth[nCntH].x + HEALTH_BAR_WIDTH, g_aPosHealth[nCntH].y + HEALTH_BAR_HEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_aPosHealth[nCntH].x + HEALTH_BAR_WIDTH, g_aPosHealth[nCntH].y - HEALTH_BAR_HEIGHT, 0.0f);
			break;

		case 1:
			//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(g_aPosHealth[nCntH].x - HEALTH_WIDTH, g_aPosHealth[nCntH].y + HEALTH_HEIGHT, 0.0f);		 //TRIANGLESTRIPで四角
			pVertex[1].pos = D3DXVECTOR3(g_aPosHealth[nCntH].x - HEALTH_WIDTH, g_aPosHealth[nCntH].y - HEALTH_HEIGHT, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_aPosHealth[nCntH].x + HEALTH_WIDTH, g_aPosHealth[nCntH].y + HEALTH_HEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_aPosHealth[nCntH].x + HEALTH_WIDTH, g_aPosHealth[nCntH].y - HEALTH_HEIGHT, 0.0f);
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
		pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVertex += 4; //ポインタずらし
	}

	//頂点バッファをアンロックする
	g_pVtxBuffHealth->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//体力の終了処理
//-----------------------------------------------------------------------------
void UninitHealth(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffHealth != NULL)
	{
		g_pVtxBuffHealth->Release();
		g_pVtxBuffHealth = NULL;
	}

	for (int nCntH = 0; nCntH < HEALTH_POLYGON; nCntH++)
	{
		//テクスチャの開放
		if (g_pTextureHealth[nCntH] != NULL)
		{
			g_pTextureHealth[nCntH]->Release();
			g_pTextureHealth[nCntH] = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
//体力の更新処理
//-----------------------------------------------------------------------------
void UpdateHealth(void)
{
	VERTEX_2D *pVertex;
	PLAYER *pPlayer;
	static float fValue = 1.0f;

	pPlayer = GetPlayer();

	fValue = (float)pPlayer->nLife / (float)g_nOldLife;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffHealth->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntH = 0; nCntH < HEALTH_POLYGON; nCntH++)
	{
		switch (nCntH)
		{
		case 1:
			////頂点座標 D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(g_aPosHealth[nCntH].x, g_aPosHealth[nCntH].y + HEALTH_HEIGHT, 0.0f);		 //TRIANGLESTRIPで四角
			pVertex[1].pos = D3DXVECTOR3(g_aPosHealth[nCntH].x, g_aPosHealth[nCntH].y - HEALTH_HEIGHT, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_aPosHealth[nCntH].x + (HEALTH_WIDTH * fValue), g_aPosHealth[nCntH].y + HEALTH_HEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_aPosHealth[nCntH].x + (HEALTH_WIDTH * fValue), g_aPosHealth[nCntH].y - HEALTH_HEIGHT, 0.0f);

			//頂点情報の設定
			pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVertex[2].tex = D3DXVECTOR2(1.0f * fValue, 1.0f);
			pVertex[3].tex = D3DXVECTOR2(1.0f * fValue, 0.0f);
			break;

		default:
			break;
		}


		pVertex += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffHealth->Unlock();
}

//-----------------------------------------------------------------------------
//体力の描画処理
void DrawHealth(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffHealth, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntH = 0; nCntH < HEALTH_POLYGON; nCntH++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureHealth[nCntH]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntH * 4, 4);
	}
}