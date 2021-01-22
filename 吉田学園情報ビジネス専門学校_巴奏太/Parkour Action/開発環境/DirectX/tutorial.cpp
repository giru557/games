//-----------------------------------------------------------------------------
//
// チュートリアルの処理 [Tutorial.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "Tutorial.h"
#include "input.h"
#include "fade.h"

//-----------------------------------------------------------------------------
// マクロ
//-----------------------------------------------------------------------------
#define TUTORIAL_POLYGON (1)													// 使用するポリゴン数
#define TUTORIAL_WIDTH (SCREEN_WIDTH)											// 幅
#define TUTORIAL_HEIGHT (SCREEN_HEIGHT)											// 高さ

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffTutorial = NULL;								// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureTutorial = { NULL };									// ポリゴンのテクスチャへのポインタ
D3DXVECTOR3 g_posTutorial;														// チュートリアル画像の位置

//-----------------------------------------------------------------------------
// チュートリアルの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\tutorial.png", &g_pTextureTutorial);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * TUTORIAL_POLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTutorial, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVertex, 0);

	//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
	pVertex[0].pos = D3DXVECTOR3(g_posTutorial.x, g_posTutorial.y + TUTORIAL_HEIGHT, 0.0f);
	pVertex[1].pos = D3DXVECTOR3(g_posTutorial.x, g_posTutorial.y, 0.0f);
	pVertex[2].pos = D3DXVECTOR3(g_posTutorial.x + TUTORIAL_WIDTH, g_posTutorial.y + TUTORIAL_HEIGHT, 0.0f);
	pVertex[3].pos = D3DXVECTOR3(g_posTutorial.x + TUTORIAL_WIDTH, g_posTutorial.y, 0.0f);

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

	//頂点バッファをアンロックする
	g_pVtxBuffTutorial->Unlock();

	//フェードイン
	SetFade(FADE_IN, MODE_GAME);

	return S_OK;
}

//-----------------------------------------------------------------------------
// チュートリアルの終了処理
//-----------------------------------------------------------------------------
void UninitTutorial(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}

	//テクスチャの開放
	if (g_pTextureTutorial != NULL)
	{
		g_pTextureTutorial->Release();
		g_pTextureTutorial = NULL;
	}

}

//-----------------------------------------------------------------------------
// チュートリアルの更新処理
//-----------------------------------------------------------------------------
void UpdateTutorial(void)
{
	VERTEX_2D *pVertex;

	if (g_pVtxBuffTutorial != NULL)
	{
		//頂点バッファをロックし頂点情報へのポインタを取得
		g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVertex, 0);


		//頂点バッファをアンロックする
		g_pVtxBuffTutorial->Unlock();
	}

	//キーを押したら
	if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_BACKSPACE) == true)
	{
		//フェードアウト
		SetFade(FADE_OUT, MODE_GAME);
	}
}

//-----------------------------------------------------------------------------
// チュートリアルの描画処理
//-----------------------------------------------------------------------------
void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTutorial);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}