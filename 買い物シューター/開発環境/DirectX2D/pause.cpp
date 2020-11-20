//-----------------------------------------------------------------------------
//
// ポーズの処理 [pause.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "pause.h"
#include "input.h"
#include "game.h"
#include "fade.h"

//-----------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------
#define POLYGON_AMOUNT (5)			//ポリゴンの数

#define PAUSE_MENU_WIDTH (200)		//ポーズメニュー背景の幅
#define PAUSE_MENU_HEIGHT (300)		//ポーズメニュー背景の高さ
#define PAUSE_BUTTON_WIDTH (150)	//ポーズメニューボタンの幅
#define PAUSE_BUTTON_HEIGHT (50)	//ポーズメニューボタンの高さ

//-----------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffPause = NULL;  //頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTexturePause[5] = { NULL };		//ポリゴンのテクスチャへのポインタ
D3DXVECTOR3 g_posPolygon[5];					//ポリゴンの位置
PAUSE_MENU g_statePause;						//ポーズの状態
int g_nValuePause;								//ポーズの選択順番

//-----------------------------------------------------------------------------
//ポーズの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	g_posPolygon[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_posPolygon[1] = D3DXVECTOR3(640, 360, 0.0f);
	g_posPolygon[2] = D3DXVECTOR3(640, 250, 0.0f);
	g_posPolygon[3] = D3DXVECTOR3(640, 400, 0.0f);
	g_posPolygon[4] = D3DXVECTOR3(640, 550, 0.0f);

	g_nValuePause = 0;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, NULL, &g_pTexturePause[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\pause100.png", &g_pTexturePause[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\pause000.png", &g_pTexturePause[2]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\pause001.png", &g_pTexturePause[3]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\pause002.png", &g_pTexturePause[4]);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * POLYGON_AMOUNT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPause, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntPause = 0; nCntPause < POLYGON_AMOUNT; nCntPause++)
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

		switch (nCntPause)
		{
		case 0:
			//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);		 //TRIANGLESTRIPで四角
			pVertex[1].pos = D3DXVECTOR3(0, 0, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

			//頂点カラーの設定
			pVertex[0].col = D3DCOLOR_RGBA(0, 0, 0, 100);  //D3DCOLOR_RGBA(赤, 緑, 青, 透明度);
			pVertex[1].col = D3DCOLOR_RGBA(0, 0, 0, 100);
			pVertex[2].col = D3DCOLOR_RGBA(0, 0, 0, 100);
			pVertex[3].col = D3DCOLOR_RGBA(0, 0, 0, 100);
			break;

		case 1:
			//頂点座標の更新
			pVertex[0].pos = D3DXVECTOR3(g_posPolygon[nCntPause].x - PAUSE_MENU_WIDTH, g_posPolygon[nCntPause].y + PAUSE_MENU_HEIGHT, 0.0f);
			pVertex[1].pos = D3DXVECTOR3(g_posPolygon[nCntPause].x - PAUSE_MENU_WIDTH, g_posPolygon[nCntPause].y - PAUSE_MENU_HEIGHT, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_posPolygon[nCntPause].x + PAUSE_MENU_WIDTH, g_posPolygon[nCntPause].y + PAUSE_MENU_HEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_posPolygon[nCntPause].x + PAUSE_MENU_WIDTH, g_posPolygon[nCntPause].y - PAUSE_MENU_HEIGHT, 0.0f);

			//頂点カラーの設定
			pVertex[0].col = D3DCOLOR_RGBA(255, 255, 255, 128);  //D3DCOLOR_RGBA(赤, 緑, 青, 透明度);
			pVertex[1].col = D3DCOLOR_RGBA(255, 255, 255, 128);
			pVertex[2].col = D3DCOLOR_RGBA(255, 255, 255, 128);
			pVertex[3].col = D3DCOLOR_RGBA(255, 255, 255, 128);

			break;

		default:
			//頂点座標の更新
			pVertex[0].pos = D3DXVECTOR3(g_posPolygon[nCntPause].x - PAUSE_BUTTON_WIDTH, g_posPolygon[nCntPause].y + PAUSE_BUTTON_HEIGHT, 0.0f);
			pVertex[1].pos = D3DXVECTOR3(g_posPolygon[nCntPause].x - PAUSE_BUTTON_WIDTH, g_posPolygon[nCntPause].y - PAUSE_BUTTON_HEIGHT, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_posPolygon[nCntPause].x + PAUSE_BUTTON_WIDTH, g_posPolygon[nCntPause].y + PAUSE_BUTTON_HEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_posPolygon[nCntPause].x + PAUSE_BUTTON_WIDTH, g_posPolygon[nCntPause].y - PAUSE_BUTTON_HEIGHT, 0.0f);

			//頂点カラーの設定
			pVertex[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);  //D3DCOLOR_RGBA(赤, 緑, 青, 透明度);
			pVertex[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVertex[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVertex[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			break;
		}

		pVertex += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//ポーズの終了処理
//-----------------------------------------------------------------------------
void UninitPause(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}

	//テクスチャの開放
	for (int nCntP = 0; nCntP < POLYGON_AMOUNT; nCntP++)
	{
		if (g_pTexturePause[nCntP] != NULL)
		{
			g_pTexturePause[nCntP]->Release();
			g_pTexturePause[nCntP] = NULL;
		}
	}

}

//-----------------------------------------------------------------------------
//ポーズの更新処理
//-----------------------------------------------------------------------------
void UpdatePause(void)
{
	VERTEX_2D *pVertex;

	if (GetKeyboardTrigger(DIK_S) == true)
	{
		g_nValuePause = (g_nValuePause + 1) % 3; //3までいくとあまり0が代入されてリセット
	}
	else if (GetKeyboardTrigger(DIK_W) == true)
	{
		g_nValuePause--;

		if (g_nValuePause < 0)
		{
			g_nValuePause += 3;
		}
	}

	switch (g_nValuePause)
	{
	case 0:
		g_statePause = PAUSE_MENU_CONTINUE;
		break;

	case 1:
		g_statePause = PAUSE_MENU_RETRY;
		break;

	case 2:
		g_statePause = PAUSE_MENU_QUIT;
		break;

	default:
		break;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntPause = 0; nCntPause < POLYGON_AMOUNT; nCntPause++)
	{
		switch (nCntPause)
		{
		case 2:
			if (g_statePause == PAUSE_MENU_CONTINUE)
			{
				//頂点カラーの設定
				pVertex[0].col = D3DCOLOR_RGBA(0, 255, 0, 255);  //D3DCOLOR_RGBA(赤, 緑, 青, 透明度);
				pVertex[1].col = D3DCOLOR_RGBA(0, 255, 0, 255);
				pVertex[2].col = D3DCOLOR_RGBA(0, 255, 0, 255);
				pVertex[3].col = D3DCOLOR_RGBA(0, 255, 0, 255);
			}
			else
			{
				//頂点カラーの設定
				pVertex[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);  //D3DCOLOR_RGBA(赤, 緑, 青, 透明度);
				pVertex[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVertex[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVertex[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			}
			break;

		case 3:
			if (g_statePause == PAUSE_MENU_RETRY)
			{
				//頂点カラーの設定
				pVertex[0].col = D3DCOLOR_RGBA(0, 255, 0, 255);  //D3DCOLOR_RGBA(赤, 緑, 青, 透明度);
				pVertex[1].col = D3DCOLOR_RGBA(0, 255, 0, 255);
				pVertex[2].col = D3DCOLOR_RGBA(0, 255, 0, 255);
				pVertex[3].col = D3DCOLOR_RGBA(0, 255, 0, 255);
			}
			else
			{
				//頂点カラーの設定
				pVertex[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);  //D3DCOLOR_RGBA(赤, 緑, 青, 透明度);
				pVertex[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVertex[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVertex[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			}
			break;

		case 4:
			if (g_statePause == PAUSE_MENU_QUIT)
			{
				//頂点カラーの設定
				pVertex[0].col = D3DCOLOR_RGBA(0, 255, 0, 255);  //D3DCOLOR_RGBA(赤, 緑, 青, 透明度);
				pVertex[1].col = D3DCOLOR_RGBA(0, 255, 0, 255);
				pVertex[2].col = D3DCOLOR_RGBA(0, 255, 0, 255);
				pVertex[3].col = D3DCOLOR_RGBA(0, 255, 0, 255);
			}
			else
			{
				//頂点カラーの設定
				pVertex[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);  //D3DCOLOR_RGBA(赤, 緑, 青, 透明度);
				pVertex[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVertex[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVertex[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			}
			break;

		default:
			break;
		}
		
		//頂点ポインタずらし
		pVertex += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();

	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		switch (g_nValuePause)
		{
		case 0:
			SetPause(false);
			break;

		case 1:
			SetFade(FADE_OUT, MODE_GAME);
			break;

		case 2:
			SetFade(FADE_OUT, MODE_TITLE);
			break;

		default:
			break;
		}
	}
}

//-----------------------------------------------------------------------------
//ポーズの描画処理
//-----------------------------------------------------------------------------
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);



	for (int nCntPause = 0; nCntPause < POLYGON_AMOUNT; nCntPause++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePause[nCntPause]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * 4, 2);
	}
}