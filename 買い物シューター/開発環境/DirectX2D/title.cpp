//-----------------------------------------------------------------------------
//
// タイトルの処理 [title.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "title.h"
#include "input.h"
#include "fade.h"

//マクロ
#define BG_POLYGON (3) //テクスチャの枚数
#define BG_TITLE_WIDTH (933 / 2)	//タイトルロゴの横幅
#define BG_TITLE_HEIGHT (112 / 2)	//タイトルロゴの高さ
#define BG_PENTER_WIDTH (857 / 2)		//PRESSENTERの横幅
#define BG_PENTER_HEIGHT (72 / 2)		//PRESSENTERの高さ

//プロトタイプ宣言
void ReturnShrink(int nIdx);
void ReturnBlink(int nIdx);

//-----------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffTitle = NULL;				//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureTitle[BG_POLYGON] = { NULL };		//ポリゴンのテクスチャへのポインタ
TITLE g_Title[BG_POLYGON];											//TITLEの情報
TITLESTATE g_TitleState[BG_POLYGON];								//タイトルの状態

//-----------------------------------------------------------------------------
//タイトルの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	//位置初期化
	g_Title[1].pos = D3DXVECTOR3(640, 128, 0.0f);
	g_Title[2].pos = D3DXVECTOR3(640, 580, 0.0f);

	//TITLE初期化
	for (int nCntT = 0; nCntT < BG_POLYGON; nCntT++)
	{
		g_Title[nCntT].fBlink = 1.0f;
		g_Title[nCntT].fShrink = 1.0f;

		g_TitleState[nCntT] = TITLESTATE_NORMAL;
	}

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\space_bg.png", &g_pTextureTitle[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\title_logo.png", &g_pTextureTitle[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\press_enter.png", &g_pTextureTitle[2]);


	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * BG_POLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTitle, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntT = 0; nCntT < BG_POLYGON; nCntT++)
	{
		switch (nCntT)
		{
		case 0:
			//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);		 //TRIANGLESTRIPで四角
			pVertex[1].pos = D3DXVECTOR3(0, 0, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
			break;

		case 1:
			//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(g_Title[nCntT].pos.x - BG_TITLE_WIDTH, g_Title[nCntT].pos.y + BG_TITLE_HEIGHT, 0.0f);		 //TRIANGLESTRIPで四角
			pVertex[1].pos = D3DXVECTOR3(g_Title[nCntT].pos.x - BG_TITLE_WIDTH, g_Title[nCntT].pos.y - BG_TITLE_HEIGHT, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_Title[nCntT].pos.x + BG_TITLE_WIDTH, g_Title[nCntT].pos.y + BG_TITLE_HEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_Title[nCntT].pos.x + BG_TITLE_WIDTH, g_Title[nCntT].pos.y - BG_TITLE_HEIGHT, 0.0f);

			g_TitleState[nCntT] = TITLESTATE_SHRINK;
			break;

		case 2:
			//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(g_Title[nCntT].pos.x - BG_PENTER_WIDTH, g_Title[nCntT].pos.y + BG_PENTER_HEIGHT, 0.0f);		 //TRIANGLESTRIPで四角
			pVertex[1].pos = D3DXVECTOR3(g_Title[nCntT].pos.x - BG_PENTER_WIDTH, g_Title[nCntT].pos.y - BG_PENTER_HEIGHT, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_Title[nCntT].pos.x + BG_PENTER_WIDTH, g_Title[nCntT].pos.y + BG_PENTER_HEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_Title[nCntT].pos.x + BG_PENTER_WIDTH, g_Title[nCntT].pos.y - BG_PENTER_HEIGHT, 0.0f);

			g_TitleState[nCntT] = TITLESTATE_BLINK;
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

		pVertex += 4; //頂点ずらし
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();

	SetFade(FADE_IN, MODE_TITLE);
	return S_OK;
}

//-----------------------------------------------------------------------------
//タイトルの終了処理
//-----------------------------------------------------------------------------
void UninitTitle(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}

	for (int nCntTex = 0; nCntTex < BG_POLYGON; nCntTex++)
	{
		//テクスチャの開放
		if (g_pTextureTitle[nCntTex] != NULL)
		{
			g_pTextureTitle[nCntTex]->Release();
			g_pTextureTitle[nCntTex] = NULL;
		}
	}

}

//-----------------------------------------------------------------------------
//タイトルの更新処理
//-----------------------------------------------------------------------------
void UpdateTitle(void)
{
	VERTEX_2D *pVertex;

	if (g_pVtxBuffTitle != NULL)
	{
		//頂点バッファをロックし頂点情報へのポインタを取得
		g_pVtxBuffTitle->Lock(0, 0, (void**)&pVertex, 0);

		for (int nCntT = 0; nCntT < BG_POLYGON; nCntT++)
		{
			switch (g_TitleState[nCntT])
			{
			case TITLESTATE_NORMAL:
				break;

			case TITLESTATE_SHRINK:
				ReturnShrink(nCntT);
				break;

			case TITLESTATE_BLINK:
				ReturnBlink(nCntT);

			default:
				break;
			}

			switch (nCntT)
			{
			case 1:
				//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
				//pVertex[0].pos = D3DXVECTOR3(g_Title[nCntT].pos.x - (BG_TITLE_WIDTH), g_Title[nCntT].pos.y + (BG_TITLE_HEIGHT), 0.0f);		 //TRIANGLESTRIPで四角
				//pVertex[1].pos = D3DXVECTOR3(g_Title[nCntT].pos.x - (BG_TITLE_WIDTH), g_Title[nCntT].pos.y - (BG_TITLE_HEIGHT), 0.0f);
				//pVertex[2].pos = D3DXVECTOR3(g_Title[nCntT].pos.x + (BG_TITLE_WIDTH), g_Title[nCntT].pos.y + (BG_TITLE_HEIGHT), 0.0f);
				//pVertex[3].pos = D3DXVECTOR3(g_Title[nCntT].pos.x + (BG_TITLE_WIDTH), g_Title[nCntT].pos.y - (BG_TITLE_HEIGHT), 0.0f);

				pVertex[0].pos = D3DXVECTOR3(g_Title[nCntT].pos.x - (BG_TITLE_WIDTH * g_Title[nCntT].fShrink), g_Title[nCntT].pos.y + (BG_TITLE_HEIGHT * g_Title[nCntT].fShrink), 0.0f);		 //TRIANGLESTRIPで四角
				pVertex[1].pos = D3DXVECTOR3(g_Title[nCntT].pos.x - (BG_TITLE_WIDTH * g_Title[nCntT].fShrink), g_Title[nCntT].pos.y - (BG_TITLE_HEIGHT * g_Title[nCntT].fShrink), 0.0f);
				pVertex[2].pos = D3DXVECTOR3(g_Title[nCntT].pos.x + (BG_TITLE_WIDTH * g_Title[nCntT].fShrink), g_Title[nCntT].pos.y + (BG_TITLE_HEIGHT * g_Title[nCntT].fShrink), 0.0f);
				pVertex[3].pos = D3DXVECTOR3(g_Title[nCntT].pos.x + (BG_TITLE_WIDTH * g_Title[nCntT].fShrink), g_Title[nCntT].pos.y - (BG_TITLE_HEIGHT * g_Title[nCntT].fShrink), 0.0f);

			case 2:
				pVertex[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Title[nCntT].fBlink);
				pVertex[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Title[nCntT].fBlink);
				pVertex[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Title[nCntT].fBlink);
				pVertex[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Title[nCntT].fBlink);
				break;

			default:
				break;
			}

			pVertex += 4;
		}

		//頂点バッファをアンロックする
		g_pVtxBuffTitle->Unlock();
	}

	if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_BACKSPACE) == true)
	{
		SetFade(FADE_OUT, MODE_GAME);
	}
}

//-----------------------------------------------------------------------------
//タイトルの描画処理
//-----------------------------------------------------------------------------
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (int nCntTex = 0; nCntTex < BG_POLYGON; nCntTex++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTitle[nCntTex]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTex * 4, 4);
	}
}

void ReturnShrink(int nIdx)
{
	static bool bShrink = true;
	static int bCounter = 0;
	bCounter++;

	if (bCounter > 100)
	{
		bShrink = false;
	}
	if (bCounter > 200)
	{
		bShrink = true;
		bCounter = 0;
	}

	if (bShrink == true)
	{
		g_Title[nIdx].fShrink -= 0.0004f;
	}
	else
	{
		g_Title[nIdx].fShrink += 0.0004f;
	}
}

void ReturnBlink(int nIdx)
{
	static bool bBlink = true;
	static int bCounter = 0;
	bCounter++;

	if (bCounter > 50)
	{
		bBlink = false;
	}
	if (bCounter > 80)
	{
		bBlink = true;
	}
	if (bCounter > 120)
	{
		bCounter = 0;
	}

	if (bBlink == true)
	{
		g_Title[nIdx].fBlink = 1.0f;
	}
	else
	{
		g_Title[nIdx].fBlink = 0.0f;
	}
}