//-----------------------------------------------------------------------------
//
// お金の処理 [money.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "money.h"

//マクロ
#define MONEY_DIGIT_MAX (2)
#define MONEY_LIMIT (99)
#define SPACE_BETWEEN_CHAR (35)

//-----------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffMoney = NULL;  //頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureMoney = NULL;		//ポリゴンのテクスチャへのポインタ
D3DXVECTOR3 g_aPosMoney[MONEY_DIGIT_MAX];		//数字のポリゴンの位置
int g_nMoney;									//現在の所持金

//-----------------------------------------------------------------------------
//お金の初期化処理
//-----------------------------------------------------------------------------
HRESULT InitMoney(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	//お金の初期化
	g_nMoney = 0;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number001.png", &g_pTextureMoney);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * MONEY_DIGIT_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffMoney, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffMoney->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntMoney = 0; nCntMoney < MONEY_DIGIT_MAX; nCntMoney++)
	{
		//位置
		g_aPosMoney[nCntMoney] = D3DXVECTOR3(1000, 600, 0.0f);

		//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
		pVertex[0].pos = D3DXVECTOR3((g_aPosMoney[nCntMoney].x + -15) + nCntMoney * SPACE_BETWEEN_CHAR, g_aPosMoney[nCntMoney].y + 20, 0.0f);
		pVertex[1].pos = D3DXVECTOR3((g_aPosMoney[nCntMoney].x + -15) + nCntMoney * SPACE_BETWEEN_CHAR, g_aPosMoney[nCntMoney].y + -20, 0.0f);
		pVertex[2].pos = D3DXVECTOR3((g_aPosMoney[nCntMoney].x + 15) + nCntMoney * SPACE_BETWEEN_CHAR, g_aPosMoney[nCntMoney].y + 20, 0.0f);
		pVertex[3].pos = D3DXVECTOR3((g_aPosMoney[nCntMoney].x + 15) + nCntMoney * SPACE_BETWEEN_CHAR, g_aPosMoney[nCntMoney].y + -20, 0.0f);

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
		pVertex[2].tex = D3DXVECTOR2(1.0f * 0.1f, 1.0f);
		pVertex[3].tex = D3DXVECTOR2(1.0f * 0.1f, 0.0f);

		pVertex += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffMoney->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//お金の終了処理
//-----------------------------------------------------------------------------
void UninitMoney(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffMoney != NULL)
	{
		g_pVtxBuffMoney->Release();
		g_pVtxBuffMoney = NULL;
	}

	//テクスチャの開放
	if (g_pTextureMoney != NULL)
	{
		g_pTextureMoney->Release();
		g_pTextureMoney = NULL;
	}
}

//-----------------------------------------------------------------------------
//お金の更新処理
//-----------------------------------------------------------------------------
void UpdateMoney(void)
{
	VERTEX_2D *pVertex;
	int aNumber[2];

	if (g_nMoney > 99)
	{
		g_nMoney = 99;
	}

	aNumber[0] = g_nMoney % 100 / 10;
	aNumber[1] = g_nMoney % 10 / 1;

	//頂点バッファをロックし頂点情報へのポインタを取得
	if (g_pVtxBuffMoney != NULL)
	{
		g_pVtxBuffMoney->Lock(0, 0, (void**)&pVertex, 0);

		for (int nCntMoney = 0; nCntMoney < MONEY_DIGIT_MAX; nCntMoney++)
		{
			//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3((g_aPosMoney[nCntMoney].x + -15) + nCntMoney * SPACE_BETWEEN_CHAR, g_aPosMoney[nCntMoney].y + 20, 0.0f);
			pVertex[1].pos = D3DXVECTOR3((g_aPosMoney[nCntMoney].x + -15) + nCntMoney * SPACE_BETWEEN_CHAR, g_aPosMoney[nCntMoney].y + -20, 0.0f);
			pVertex[2].pos = D3DXVECTOR3((g_aPosMoney[nCntMoney].x + 15) + nCntMoney * SPACE_BETWEEN_CHAR, g_aPosMoney[nCntMoney].y + 20, 0.0f);
			pVertex[3].pos = D3DXVECTOR3((g_aPosMoney[nCntMoney].x + 15) + nCntMoney * SPACE_BETWEEN_CHAR, g_aPosMoney[nCntMoney].y + -20, 0.0f);

			//頂点情報の設定
			pVertex[0].tex = D3DXVECTOR2(0.1f * (aNumber[nCntMoney]), 1.0f);
			pVertex[1].tex = D3DXVECTOR2(0.1f * (aNumber[nCntMoney]), 0.0f);
			pVertex[2].tex = D3DXVECTOR2(0.1f * (aNumber[nCntMoney] + 1), 1.0f);
			pVertex[3].tex = D3DXVECTOR2(0.1f * (aNumber[nCntMoney] + 1), 0.0f);

			pVertex += 4;	//頂点のポインタずらし
		}
		//頂点バッファをアンロックする
		g_pVtxBuffMoney->Unlock();
	}
}

//-----------------------------------------------------------------------------
//お金の描画処理
//-----------------------------------------------------------------------------
void DrawMoney(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMoney, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMoney);

	for (int nCntMoney = 0; nCntMoney < MONEY_DIGIT_MAX; nCntMoney++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntMoney * 4, 2);
	}
}

//お金の追加
void AddMoney(int nValue)
{
	g_nMoney += nValue;
}

bool UseMoney(int nValue)
{
	if ((g_nMoney - nValue) >= 0)
	{
		g_nMoney -= nValue;
	}
	else 
	{
		return false;
	}

	return true;
}