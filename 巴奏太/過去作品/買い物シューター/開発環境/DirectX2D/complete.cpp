//-----------------------------------------------------------------------------
//
// ステージコンプリートの処理 [complete.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "complete.h"
#include "sound.h"

//マクロ
#define COMPLETE_POLYGON (2)

//-----------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffComp = NULL;						//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureComp[COMPLETE_POLYGON] = { NULL };		//ポリゴンのテクスチャへのポインタ
bool g_bDrawBG;														//ステージコンプリートを表示するか

//-----------------------------------------------------------------------------
//ステージコンプリートの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitComplete(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	g_bDrawBG = false;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\stage_completedBG.png", &g_pTextureComp[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\stage_completed.png", &g_pTextureComp[1]);


	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * COMPLETE_POLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffComp, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffComp->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntC = 0; nCntC < COMPLETE_POLYGON; nCntC++)
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

		pVertex += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffComp->Unlock();

	PlaySound(SOUND_LABEL_SE_STAGECOMP);

	return S_OK;
}

//-----------------------------------------------------------------------------
//ステージコンプリートの終了処理
//-----------------------------------------------------------------------------
void UninitComplete(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffComp != NULL)
	{
		g_pVtxBuffComp->Release();
		g_pVtxBuffComp = NULL;
	}

	for (int nCntC = 0; nCntC < COMPLETE_POLYGON; nCntC++)
	{
		//テクスチャの開放
		if (g_pTextureComp[nCntC] != NULL)
		{
			g_pTextureComp[nCntC]->Release();
			g_pTextureComp[nCntC] = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
//ステージコンプリートの更新処理
//-----------------------------------------------------------------------------
void UpdateComplete(void)
{
	VERTEX_2D *pVertex;
	bool bBlink;

	if (g_bDrawBG == true)
	{
		//頂点バッファをロックし頂点情報へのポインタを取得
		g_pVtxBuffComp->Lock(0, 0, (void**)&pVertex, 0);


		//頂点バッファをアンロックする
		g_pVtxBuffComp->Unlock();
	}
}

//-----------------------------------------------------------------------------
//ステージコンプリートの描画処理

//-----------------------------------------------------------------------------
void DrawComplete(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffComp, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntC = 0; nCntC < COMPLETE_POLYGON; nCntC++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureComp[nCntC]);

		if (g_bDrawBG == true)
		{
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntC * 4, 2);
		}
	}
}

//ステージコンプリート表示
void ShowStageComplete(void)
{
	g_bDrawBG = true;
}