//-----------------------------------------------------------------------------
//
// フェードの処理 [fade.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "fade.h"

//グローバル変数
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffFade = NULL;  //頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureFade = NULL;		//ポリゴンのテクスチャへのポインタ
FADE g_fade;	//フェードの状態
MODE g_modeNext; //次のモード(画面)
D3DXCOLOR g_colorFade; //フェード色

//-----------------------------------------------------------------------------
// フェードの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitFade(MODE modeNext)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	g_fade = FADE_IN;
	g_modeNext = modeNext;
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffFade, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVertex, 0);

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
	pVertex[0].col = g_colorFade;
	pVertex[1].col = g_colorFade;
	pVertex[2].col = g_colorFade;
	pVertex[3].col = g_colorFade;

	//頂点情報の設定
	pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffFade->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
// フェードの終了処理
//-----------------------------------------------------------------------------
void UninitFade(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}

	//テクスチャの開放
	if (g_pTextureFade != NULL)
	{
		g_pTextureFade->Release();
		g_pTextureFade = NULL;
	}
}

//-----------------------------------------------------------------------------
// フェードの更新処理
//-----------------------------------------------------------------------------
void UpdateFade(void)
{
	VERTEX_2D *pVertex;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVertex, 0);

	//頂点カラーの設定
	pVertex[0].col = g_colorFade;
	pVertex[1].col = g_colorFade;
	pVertex[2].col = g_colorFade;
	pVertex[3].col = g_colorFade;
	
	//頂点バッファをアンロックする
	g_pVtxBuffFade->Unlock();

	switch (g_fade)
	{
	case FADE_IN:
		g_colorFade.a -= 0.02f;

		if (g_colorFade.a <= 0.0f)
		{
			g_fade = FADE_NONE;
		}

		break;

	case FADE_OUT:
		g_colorFade.a += 0.02f;

		if (g_colorFade.a >= 1.0f)
		{
			SetMode(g_modeNext);

			g_fade = FADE_IN;
		}
		break;

	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// フェードの描画処理
//-----------------------------------------------------------------------------
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureFade);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//-----------------------------------------------------------------------------
// セットフェード
//-----------------------------------------------------------------------------
void SetFade(FADE fade, MODE modeNext)
{
	//フェードの種類を受け取る
	g_fade = fade;

	//次のモードの種類を受け取る
	g_modeNext = modeNext;
}