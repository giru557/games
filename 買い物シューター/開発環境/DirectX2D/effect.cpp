//-----------------------------------------------------------------------------
//
// 爆発の処理 [explosion.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "effect.h"

//-----------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------
#define MAX_EFFECT (1280) //エフェクトの最大数

//-----------------------------------------------------------------------------
//構造体宣言
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXCOLOR effCol;		//エフェクトの色
	int nSize;				//エフェクトのサイズ
	bool bUse;				//使用しているか
} EFFECT;

//-----------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;
EFFECT g_aEffect[MAX_EFFECT];						//エフェクトの情報
D3DXCOLOR g_colorEff;								//エフェクト色
float g_fAlphaShift;								//エフェクトが透明になるスピード;

//-----------------------------------------------------------------------------
//エフェクトの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\effect000.jpg", &g_pTextureEffect);

	//EXPLOSIONの初期化
	for (int nCntEff = 0; nCntEff < MAX_EFFECT; nCntEff++)
	{
		g_aEffect[nCntEff].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEff].effCol = D3DXCOLOR(0.0f, 0.0f, 0.5f, 1.0f);
		g_aEffect[nCntEff].bUse = false;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * MAX_EFFECT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffEffect, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファのロック
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVertex, 0);

	//頂点座標
	for (int nCntEff = 0; nCntEff < MAX_EFFECT; nCntEff++)
	{
		//頂点座標の設定
		pVertex[0].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVertex[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVertex[2].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVertex[3].pos = D3DXVECTOR3(0, 0, 0.0f);

		//rhwの設定
		pVertex[0].rhw = 1.0f;	//1.0f固定
		pVertex[1].rhw = 1.0f;
		pVertex[2].rhw = 1.0f;
		pVertex[3].rhw = 1.0f;

		//頂点カラーの設定
		pVertex[0].col = D3DXCOLOR(0.0f, 0.0f, 0.5f, 1.0f);
		pVertex[1].col = D3DXCOLOR(0.0f, 0.0f, 0.5f, 1.0f);
		pVertex[2].col = D3DXCOLOR(0.0f, 0.0f, 0.5f, 1.0f);
		pVertex[3].col = D3DXCOLOR(0.0f, 0.0f, 0.5f, 1.0f);

		//UVの設定
		pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVertex += 4; //頂点のポインタをずらす
	}

	g_pVtxBuffEffect->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//エフェクトの終了処理
//-----------------------------------------------------------------------------
void UninitEffect(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}

	//テクスチャの開放
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}
}

//-----------------------------------------------------------------------------
//エフェクトの更新処理
//-----------------------------------------------------------------------------
void UpdateEffect(void)
{
	EFFECT *pEffect;
	VERTEX_2D *pVertex;

	pEffect = &g_aEffect[0];

	if (g_pVtxBuffEffect != NULL)
	{
		//頂点バッファのロック
		g_pVtxBuffEffect->Lock(0, 0, (void**)&pVertex, 0);

		for (int nCntEff = 0; nCntEff < MAX_EFFECT; nCntEff++, pEffect++)
		{
			if (pEffect->bUse == true)
			{
				//頂点座標の設定
				pVertex[0].pos = D3DXVECTOR3(g_aEffect[nCntEff].pos.x - g_aEffect[nCntEff].nSize, g_aEffect[nCntEff].pos.y + g_aEffect[nCntEff].nSize, 0.0f);
				pVertex[1].pos = D3DXVECTOR3(g_aEffect[nCntEff].pos.x - g_aEffect[nCntEff].nSize, g_aEffect[nCntEff].pos.y - g_aEffect[nCntEff].nSize, 0.0f);
				pVertex[2].pos = D3DXVECTOR3(g_aEffect[nCntEff].pos.x + g_aEffect[nCntEff].nSize, g_aEffect[nCntEff].pos.y + g_aEffect[nCntEff].nSize, 0.0f);
				pVertex[3].pos = D3DXVECTOR3(g_aEffect[nCntEff].pos.x + g_aEffect[nCntEff].nSize, g_aEffect[nCntEff].pos.y - g_aEffect[nCntEff].nSize, 0.0f);

				//頂点カラーの設定
				pVertex[0].col = g_aEffect[nCntEff].effCol;
				pVertex[1].col = g_aEffect[nCntEff].effCol;
				pVertex[2].col = g_aEffect[nCntEff].effCol;
				pVertex[3].col = g_aEffect[nCntEff].effCol;

				g_aEffect[nCntEff].effCol.a -= g_fAlphaShift;
				
				if (g_aEffect[nCntEff].effCol.a < 0.0f)
				{
					g_aEffect[nCntEff].bUse = false;
					g_aEffect[nCntEff].effCol.a = 1.0f;
				}
			}
			pVertex += 4; //頂点のポインタをずらす
		}

		//頂点バッファのアンロック
		g_pVtxBuffEffect->Unlock();
	}
}

//-----------------------------------------------------------------------------
//エフェクトの描画処理
//-----------------------------------------------------------------------------
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//レンダーステートの設定
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		//ソース（描画元）の合成方法の設定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);	//デスティネーション（描画先）の合成方法の設定

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureEffect);

	for (int nCntExp = 0; nCntExp < MAX_EFFECT; nCntExp++)
	{
		if (g_aEffect[nCntExp].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExp * 4, 2);
		}
	}

	//レンダーステートの設定
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		//ソース（描画元）の合成方法の設定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	//デスティネーション（描画先）の合成方法の設定
}

//-----------------------------------------------------------------------------
//エフェクトの設定
//-----------------------------------------------------------------------------
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR effCol, int nSize, float fAlphaShift)
{
	EFFECT *pEffect;
	VERTEX_2D *pVertex;

	pEffect = &g_aEffect[0];

	//頂点バッファのロック
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntEff = 0; nCntEff < MAX_EFFECT; nCntEff++, pEffect++)
	{
		if (pEffect->bUse == false)
		{
			//位置を設定
			g_aEffect[nCntEff].pos = pos;

			//頂点座標の設定
			pVertex[0].pos = D3DXVECTOR3(g_aEffect[nCntEff].pos.x - g_aEffect[nCntEff].nSize, g_aEffect[nCntEff].pos.y + g_aEffect[nCntEff].nSize, 0.0f);
			pVertex[1].pos = D3DXVECTOR3(g_aEffect[nCntEff].pos.x - g_aEffect[nCntEff].nSize, g_aEffect[nCntEff].pos.y - g_aEffect[nCntEff].nSize, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_aEffect[nCntEff].pos.x + g_aEffect[nCntEff].nSize, g_aEffect[nCntEff].pos.y + g_aEffect[nCntEff].nSize, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_aEffect[nCntEff].pos.x + g_aEffect[nCntEff].nSize, g_aEffect[nCntEff].pos.y - g_aEffect[nCntEff].nSize, 0.0f);

			//エフェクトサイズの設定
			g_aEffect[nCntEff].nSize = nSize;

			//エフェクト色の設定
			g_aEffect[nCntEff].effCol = effCol;

			//エフェクトが透明になるスピードを設定
			g_fAlphaShift = fAlphaShift;

			//使用中に変更
			pEffect->bUse = true;

			break;

		}
		//頂点のポインタをずらす
		pVertex += 4;
	}
	g_pVtxBuffEffect->Unlock();
}
