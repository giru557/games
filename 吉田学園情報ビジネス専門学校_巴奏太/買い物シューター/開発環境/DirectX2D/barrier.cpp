//-----------------------------------------------------------------------------
//
// バリアの処理 [barrier.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "barrier.h"
#include "player.h"

//-----------------------------------------------------------------------------
//マクロ
//-----------------------------------------------------------------------------
#define BARRIER_SIZE (100)

//-----------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffBarrier = NULL;  //頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureBarrier = NULL;		//ポリゴンのテクスチャへのポインタ
bool g_bBought;										//購入済みか
bool g_bActive;										//バリアが有効かどうか
D3DXVECTOR3 g_PosBarrier;							//バリアの位置
int g_nCounterBarrier;								//バリアのカウンタ

//-----------------------------------------------------------------------------
//Barrierの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitBarrier(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	g_bBought = false;
	g_bActive = false;
	g_nCounterBarrier = 0;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\barrier.png", &g_pTextureBarrier);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBarrier, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffBarrier->Lock(0, 0, (void**)&pVertex, 0);

	//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
	pVertex[0].pos = D3DXVECTOR3(0, 0, 0.0f);		 //TRIANGLESTRIPで四角
	pVertex[1].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVertex[2].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVertex[3].pos = D3DXVECTOR3(0, 0, 0.0f);

	//rhwの設定
	pVertex[0].rhw = 1.0f;	//1.0f固定
	pVertex[1].rhw = 1.0f;
	pVertex[2].rhw = 1.0f;
	pVertex[3].rhw = 1.0f;

	//頂点カラーの設定
	pVertex[0].col = D3DCOLOR_RGBA(255, 255, 255, 128);  //D3DCOLOR_RGBA(赤, 緑, 青, 透明度);
	pVertex[1].col = D3DCOLOR_RGBA(255, 255, 255, 128);
	pVertex[2].col = D3DCOLOR_RGBA(255, 255, 255, 128);
	pVertex[3].col = D3DCOLOR_RGBA(255, 255, 255, 128);

	//頂点情報の設定
	pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffBarrier->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//Barrierの終了処理
//-----------------------------------------------------------------------------
void UninitBarrier(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffBarrier != NULL)
	{
		g_pVtxBuffBarrier->Release();
		g_pVtxBuffBarrier = NULL;
	}

	//テクスチャの開放
	if (g_pTextureBarrier != NULL)
	{
		g_pTextureBarrier->Release();
		g_pTextureBarrier = NULL;
	}
}

//-----------------------------------------------------------------------------
//Barrierの更新処理
//-----------------------------------------------------------------------------
void UpdateBarrier(void)
{
	VERTEX_2D *pVertex;
	PLAYER *pPlayer;

	pPlayer = GetPlayer();
	g_PosBarrier = pPlayer->pos;

	if (g_pVtxBuffBarrier != NULL)
	{
		//頂点バッファをロックし頂点情報へのポインタを取得
		g_pVtxBuffBarrier->Lock(0, 0, (void**)&pVertex, 0);

		//頂点座標の更新
		pVertex[0].pos = D3DXVECTOR3(g_PosBarrier.x - BARRIER_SIZE, g_PosBarrier.y + BARRIER_SIZE, 0.0f);
		pVertex[1].pos = D3DXVECTOR3(g_PosBarrier.x - BARRIER_SIZE, g_PosBarrier.y - BARRIER_SIZE, 0.0f);
		pVertex[2].pos = D3DXVECTOR3(g_PosBarrier.x + BARRIER_SIZE, g_PosBarrier.y + BARRIER_SIZE, 0.0f);
		pVertex[3].pos = D3DXVECTOR3(g_PosBarrier.x + BARRIER_SIZE, g_PosBarrier.y - BARRIER_SIZE, 0.0f);

		if (g_bActive == false)
		{
			//頂点カラーの設定
			pVertex[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);  //D3DCOLOR_RGBA(赤, 緑, 青, 透明度);
			pVertex[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
			pVertex[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
			pVertex[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
		}
		else
		{
			//頂点カラーの設定
			pVertex[0].col = D3DCOLOR_RGBA(255, 255, 255, 128);  //D3DCOLOR_RGBA(赤, 緑, 青, 透明度);
			pVertex[1].col = D3DCOLOR_RGBA(255, 255, 255, 128);
			pVertex[2].col = D3DCOLOR_RGBA(255, 255, 255, 128);
			pVertex[3].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		}

		if (g_bBought == true)
		{
			if (g_bActive == false)
			{
				g_nCounterBarrier++;

				if (g_nCounterBarrier % 300 == 0)
				{
					g_bActive = true;
					g_nCounterBarrier = 0;
				}
			}
		}

		//頂点バッファをアンロックする
		g_pVtxBuffBarrier->Unlock();
	}
}

//-----------------------------------------------------------------------------
//Barrierの描画処理
//-----------------------------------------------------------------------------
void DrawBarrier(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBarrier, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBarrier);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 4);
}

//-----------------------------------------------------------------------------
//セットバリア
//-----------------------------------------------------------------------------
void SetBarrier(void)
{
	g_bActive = true;
	g_bBought = true;
}

//-----------------------------------------------------------------------------
//バリア削除
//-----------------------------------------------------------------------------
void RemoveBarrier(void)
{
	g_bActive = false;
	g_bBought = false;
}

//-----------------------------------------------------------------------------
//バリア破壊
//-----------------------------------------------------------------------------
void BreakBarrier(void)
{
	g_bActive = false;
}
//-----------------------------------------------------------------------------
//バリアチェック
//-----------------------------------------------------------------------------
bool CheckBarrier(void)
{
	if (g_bBought == true)
	{
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
//バリア有効チェック
//-----------------------------------------------------------------------------
bool CheckBarrierActive(void)

{
	if (g_bActive == true)
	{
		return true;
	}

	return false;
}