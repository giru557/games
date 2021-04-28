//-----------------------------------------------------------------------------
//
// コインの処理 [coin.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "coin.h"

//マクロ
#define COIN_POLYGON (1)
#define COIN_ANIM_SPLIT (8)
#define COIN_SIZE (16 * 1.5f)
#define COIN_ANIM_SPEED (13)

//-----------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffCoin = NULL;  //頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureCoin = NULL;		//ポリゴンのテクスチャへのポインタ
D3DXVECTOR3 g_aPosCoin[COIN_POLYGON];			//コインの位置
int g_nPatternCoin;
int g_nCounterCoin;

//-----------------------------------------------------------------------------
//コインの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitCoin(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	g_aPosCoin[0] = D3DXVECTOR3(950, 600, 0.0f);
	
	g_nPatternCoin = 0;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\coins.png", &g_pTextureCoin);
	D3DXCreateTextureFromFileEx(pDevice, "data\\TEXTURE\\coins.png", 127, 16, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0xFF000000, NULL, NULL, &g_pTextureCoin);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * COIN_POLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffCoin, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffCoin->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntC = 0; nCntC < COIN_POLYGON; nCntC++)
	{
		//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
		pVertex[0].pos = D3DXVECTOR3(g_aPosCoin[nCntC].x - COIN_SIZE, g_aPosCoin[nCntC].y + COIN_SIZE, 0.0f);		 //TRIANGLESTRIPで四角
		pVertex[1].pos = D3DXVECTOR3(g_aPosCoin[nCntC].x - COIN_SIZE, g_aPosCoin[nCntC].y - COIN_SIZE, 0.0f);
		pVertex[2].pos = D3DXVECTOR3(g_aPosCoin[nCntC].x + COIN_SIZE, g_aPosCoin[nCntC].y + COIN_SIZE, 0.0f);
		pVertex[3].pos = D3DXVECTOR3(g_aPosCoin[nCntC].x + COIN_SIZE, g_aPosCoin[nCntC].y - COIN_SIZE, 0.0f);

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
		pVertex[0].tex = D3DXVECTOR2(g_nPatternCoin * (1.0f / COIN_ANIM_SPLIT), 1.0f);
		pVertex[1].tex = D3DXVECTOR2(g_nPatternCoin * (1.0f / COIN_ANIM_SPLIT), 0.0f);
		pVertex[2].tex = D3DXVECTOR2((g_nPatternCoin + 1) * (1.0f / COIN_ANIM_SPLIT), 1.0f);
		pVertex[3].tex = D3DXVECTOR2((g_nPatternCoin + 1) * (1.0f / COIN_ANIM_SPLIT), 0.0f);

		pVertex += 4; //ポインタずらす
	}
	//頂点バッファをアンロックする
	g_pVtxBuffCoin->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//コインの終了処理
//-----------------------------------------------------------------------------
void UninitCoin(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffCoin != NULL)
	{
		g_pVtxBuffCoin->Release();
		g_pVtxBuffCoin = NULL;
	}

	//テクスチャの開放
	if (g_pTextureCoin != NULL)
	{
		g_pTextureCoin->Release();
		g_pTextureCoin = NULL;
	}

}

//-----------------------------------------------------------------------------
//コインの更新処理
//-----------------------------------------------------------------------------
void UpdateCoin(void)
{
	VERTEX_2D *pVertex;

	//カウントアップ
	g_nCounterCoin++;

	//パターンループ
	if (g_nCounterCoin % COIN_ANIM_SPEED == 0)
	{
		g_nPatternCoin = (g_nPatternCoin + 1) % COIN_ANIM_SPLIT; //COIN_ANIM_SPLITまでいくとあまり0が代入されてリセット
	}
	
	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffCoin->Lock(0, 0, (void**)&pVertex, 0);

	//頂点情報の設定
	pVertex[0].tex = D3DXVECTOR2(g_nPatternCoin * (1.0f / COIN_ANIM_SPLIT), 1.0f);
	pVertex[1].tex = D3DXVECTOR2(g_nPatternCoin * (1.0f / COIN_ANIM_SPLIT), 0.0f);
	pVertex[2].tex = D3DXVECTOR2((g_nPatternCoin + 1) * (1.0f / COIN_ANIM_SPLIT), 1.0f);
	pVertex[3].tex = D3DXVECTOR2((g_nPatternCoin + 1) * (1.0f / COIN_ANIM_SPLIT), 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffCoin->Unlock();
}

//-----------------------------------------------------------------------------
//コインの描画処理
//-----------------------------------------------------------------------------
void DrawCoin(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCoin, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureCoin);

	for (int nCntC = 0; nCntC < COIN_POLYGON; nCntC++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntC * 4, 2);
	}
}
