//-----------------------------------------------------------------------------
//
// リザルトの処理 [result.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "result.h"
#include "input.h"
#include "fade.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
RESULT g_result;	// リザルトの情報

//-----------------------------------------------------------------------------
// リザルトの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVertex;

	//リザルト構造体の初期化
	g_result.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\gameclear.jpg", &g_result.pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\gameover.jpg", &g_result.pTexture[1]);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_result.pVtxBuff, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_result.pVtxBuff->Lock(0, 0, (void**)&pVertex, 0);

	//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
	pVertex[0].pos = D3DXVECTOR3(g_result.pos.x, g_result.pos.y + SCREEN_HEIGHT, 0.0f);
	pVertex[1].pos = D3DXVECTOR3(g_result.pos.x, g_result.pos.y, 0.0f);
	pVertex[2].pos = D3DXVECTOR3(g_result.pos.x + SCREEN_WIDTH, g_result.pos.y + SCREEN_HEIGHT, 0.0f);
	pVertex[3].pos = D3DXVECTOR3(g_result.pos.x + SCREEN_WIDTH, g_result.pos.y, 0.0f);

	//rhwの設定
	for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertex[nCntVtx].rhw = 1.0f;	// 1.0f固定

	//頂点カラーの設定
	for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertex[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//頂点情報の設定
	pVertex[0].tex = D3DXVECTOR2(0.0, 1.0);
	pVertex[1].tex = D3DXVECTOR2(0.0, 0.0);
	pVertex[2].tex = D3DXVECTOR2(1.0, 1.0);
	pVertex[3].tex = D3DXVECTOR2(1.0, 0.0);

	//頂点バッファをアンロックする
	g_result.pVtxBuff->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
// リザルトの終了処理
//-----------------------------------------------------------------------------
void UninitResult(void)
{
	//頂点バッファの開放
	if (g_result.pVtxBuff != NULL)
	{
		g_result.pVtxBuff->Release();
		g_result.pVtxBuff = NULL;
	}

	for (int nCntTex = 0; nCntTex < 2; nCntTex++)
	{
		//テクスチャの開放
		if (g_result.pTexture[nCntTex] != NULL)
		{
			g_result.pTexture[nCntTex]->Release();
			g_result.pTexture[nCntTex] = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
// リザルトの更新処理
//-----------------------------------------------------------------------------
void UpdateResult(void)
{
	if (GetKeyboardTrigger(DIK_BACKSPACE) == true || GetKeyboardTrigger(DIK_RETURN) == true)
	{
		SetFade(FADE_OUT, MODE_TITLE);
	}
}

//-----------------------------------------------------------------------------
// タイトルの描画処理
//-----------------------------------------------------------------------------
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_result.pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//ゲームオーバーだったら
	if (g_result.bGameOver == true)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_result.pTexture[1]);
	}
	else
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_result.pTexture[0]);
	}

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//-----------------------------------------------------------------------------
// リザルト情報の取得
//-----------------------------------------------------------------------------
RESULT *GetResult(void)
{
	return &g_result;
}