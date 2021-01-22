//-----------------------------------------------------------------------------
//
// タイトルの処理 [title.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "title.h"
#include "input.h"
#include "fade.h"

//-----------------------------------------------------------------------------
// マクロ
//-----------------------------------------------------------------------------
#define TITLE_POLYGON (3)															//タイトルの配置オブジェクト数

#define TITLE_LOGOSIZE_WIDTH (1076)													//タイトルロゴの幅
#define TITLE_LOGOSIZE_HEIGHT (260)													//高さ
#define TITLE_LOGOPOS_X (SCREEN_WIDTH / 2)											//タイトルロゴの位置X
#define TITLE_LOGOPOS_Y (200)														//					Y

#define TITLE_PRESSENTER_WIDTH (822)
#define TITLE_PRESSENTER_HEIGHT (84)
#define TITLE_PRESSENTER_POS_X (SCREEN_WIDTH / 2)
#define TITLE_PRESSENTER_POS_Y (700)

#define TITLE_SHRINKSPD (0.0005f)													//縮小拡大の速度
#define TITLE_SHRINKRANGE (0.9f)													//どこまで縮小するか (0.0f ~ 1.0f)
#define TITLE_BLINKTIME (1.5f)															//点滅間隔 (秒)
#define TITLE_BLINKTIMEFIXED (TITLE_BLINKTIME * 1000)								//秒からミリ秒に変換

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
float TitleShrink(float fShrink);
float TitleBlink(float fBlink);

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffTitle = NULL;					//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureTitle[TITLE_POLYGON] = { NULL };		//ポリゴンのテクスチャへのポインタ
TITLE g_aTitle[TITLE_POLYGON];										//TITLE構造体の情報

//-----------------------------------------------------------------------------
// タイトルの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	//TITLEの初期化
	g_aTitle[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aTitle[1].pos = D3DXVECTOR3(TITLE_LOGOPOS_X, TITLE_LOGOPOS_Y, 0.0f);
	g_aTitle[2].pos = D3DXVECTOR3(TITLE_PRESSENTER_POS_X, TITLE_PRESSENTER_POS_Y, 0.0f);
	for (int nCntTITLE = 0; nCntTITLE < TITLE_POLYGON; nCntTITLE++)
	{
		g_aTitle[nCntTITLE].fShrink = 1.0f;
		g_aTitle[nCntTITLE].fBlink = 1.0f;
	}

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\bg_op.png", &g_pTextureTitle[0]);
	D3DXCreateTextureFromFileEx(pDevice, "data\\TEXTURE\\bg_op_title.png", TITLE_LOGOSIZE_WIDTH, TITLE_LOGOSIZE_HEIGHT, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0xFF000000, NULL, NULL, &g_pTextureTitle[1]);
	D3DXCreateTextureFromFileEx(pDevice, "data\\TEXTURE\\bg_op_pressenter.png", TITLE_PRESSENTER_WIDTH, TITLE_PRESSENTER_HEIGHT, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0xFF000000, NULL, NULL, &g_pTextureTitle[2]);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * TITLE_POLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTitle, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntT = 0; nCntT < TITLE_POLYGON; nCntT++)
	{
		switch (nCntT)
		{
		case 0:
			//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(g_aTitle[nCntT].pos.x, g_aTitle[nCntT].pos.y +SCREEN_HEIGHT, 0.0f);
			pVertex[1].pos = D3DXVECTOR3(g_aTitle[nCntT].pos.x, g_aTitle[nCntT].pos.y, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_aTitle[nCntT].pos.x + SCREEN_WIDTH, g_aTitle[nCntT].pos.y + SCREEN_HEIGHT, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_aTitle[nCntT].pos.x + SCREEN_WIDTH, g_aTitle[nCntT].pos.y, 0.0f);
			break;

		case 1:
			//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(g_aTitle[nCntT].pos.x - (TITLE_LOGOSIZE_WIDTH / 2), g_aTitle[nCntT].pos.y + (TITLE_LOGOSIZE_WIDTH / 2), 0.0f);		 //TRIANGLESTRIPで四角
			pVertex[1].pos = D3DXVECTOR3(g_aTitle[nCntT].pos.x - (TITLE_LOGOSIZE_WIDTH / 2), g_aTitle[nCntT].pos.y - (TITLE_LOGOSIZE_WIDTH / 2), 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_aTitle[nCntT].pos.x + (TITLE_LOGOSIZE_WIDTH / 2), g_aTitle[nCntT].pos.y + (TITLE_LOGOSIZE_WIDTH / 2), 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_aTitle[nCntT].pos.x + (TITLE_LOGOSIZE_WIDTH / 2), g_aTitle[nCntT].pos.y - (TITLE_LOGOSIZE_WIDTH / 2), 0.0f);
			break;

		case 2:
			//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(g_aTitle[nCntT].pos.x - (TITLE_PRESSENTER_WIDTH / 2), g_aTitle[nCntT].pos.y + (TITLE_PRESSENTER_HEIGHT / 2), 0.0f);		 //TRIANGLESTRIPで四角
			pVertex[1].pos = D3DXVECTOR3(g_aTitle[nCntT].pos.x - (TITLE_PRESSENTER_WIDTH / 2), g_aTitle[nCntT].pos.y - (TITLE_PRESSENTER_HEIGHT / 2), 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_aTitle[nCntT].pos.x + (TITLE_PRESSENTER_WIDTH / 2), g_aTitle[nCntT].pos.y + (TITLE_PRESSENTER_HEIGHT / 2), 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_aTitle[nCntT].pos.x + (TITLE_PRESSENTER_WIDTH / 2), g_aTitle[nCntT].pos.y - (TITLE_PRESSENTER_HEIGHT / 2), 0.0f);
			break;

		default:
			break;
		}

		//rhwの設定
		pVertex[0].rhw = 1.0f;	//1.0f固定
		pVertex[1].rhw = 1.0f;
		pVertex[2].rhw = 1.0f;
		pVertex[3].rhw = 1.0f;

		switch (nCntT)
		{
		case 1:
			//頂点カラーの設定
			pVertex[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);  //D3DCOLOR_RGBA(赤, 緑, 青, 透明度);
			pVertex[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVertex[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVertex[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			break;

		default:
			//頂点カラーの設定
			pVertex[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);  //D3DCOLOR_RGBA(赤, 緑, 青, 透明度);
			pVertex[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVertex[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVertex[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			break;
		}

		//頂点情報の設定
		pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVertex += 4; //頂点ずらし
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();

	//フェードイン
	SetFade(FADE_IN, MODE_TITLE);

	return S_OK;
}

//-----------------------------------------------------------------------------
// タイトルの終了処理
//-----------------------------------------------------------------------------
void UninitTitle(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}

	for (int nCntTex = 0; nCntTex < TITLE_POLYGON; nCntTex++)
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
// タイトルの更新処理
//-----------------------------------------------------------------------------
void UpdateTitle(void)
{
	VERTEX_2D *pVertex;
	TITLE *pTitle;

	pTitle = &g_aTitle[0];

	if (g_pVtxBuffTitle != NULL)
	{
		//頂点バッファをロックし頂点情報へのポインタを取得
		g_pVtxBuffTitle->Lock(0, 0, (void**)&pVertex, 0);

		for (int nCntT = 0; nCntT < TITLE_POLYGON; nCntT++, pTitle++)
		{
			switch (nCntT)
			{
			case 1:
				//タイトルの演出
				pTitle->fShrink = TitleShrink(pTitle->fShrink);
				//pTitle->fBlink = TitleBlink(pTitle->fBlink);

				//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
				pVertex[0].pos = D3DXVECTOR3(pTitle->pos.x - ((TITLE_LOGOSIZE_WIDTH / 2) * pTitle->fShrink), pTitle->pos.y + ((TITLE_LOGOSIZE_HEIGHT / 2) * pTitle->fShrink), 0.0f);		 //TRIANGLESTRIPで四角
				pVertex[1].pos = D3DXVECTOR3(pTitle->pos.x - ((TITLE_LOGOSIZE_WIDTH / 2) * pTitle->fShrink), pTitle->pos.y - ((TITLE_LOGOSIZE_HEIGHT / 2) * pTitle->fShrink), 0.0f);
				pVertex[2].pos = D3DXVECTOR3(pTitle->pos.x + ((TITLE_LOGOSIZE_WIDTH / 2) * pTitle->fShrink), pTitle->pos.y + ((TITLE_LOGOSIZE_HEIGHT / 2) * pTitle->fShrink), 0.0f);
				pVertex[3].pos = D3DXVECTOR3(pTitle->pos.x + ((TITLE_LOGOSIZE_WIDTH / 2) * pTitle->fShrink), pTitle->pos.y - ((TITLE_LOGOSIZE_HEIGHT / 2) * pTitle->fShrink), 0.0f);

				//頂点カラーの設定
				pVertex[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pTitle->fBlink);  //D3DCOLOR_RGBA(赤, 緑, 青, 透明度);
				pVertex[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pTitle->fBlink);
				pVertex[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pTitle->fBlink);
				pVertex[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pTitle->fBlink);
				break;

			case 2:
				//タイトルの演出
				//pTitle->fShrink = TitleShrink(pTitle->fShrink);
				pTitle->fBlink = TitleBlink(pTitle->fBlink);

				//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
				pVertex[0].pos = D3DXVECTOR3(pTitle->pos.x - ((TITLE_PRESSENTER_WIDTH / 2) * pTitle->fShrink), pTitle->pos.y + ((TITLE_PRESSENTER_HEIGHT / 2) * pTitle->fShrink), 0.0f);		 //TRIANGLESTRIPで四角
				pVertex[1].pos = D3DXVECTOR3(pTitle->pos.x - ((TITLE_PRESSENTER_WIDTH / 2) * pTitle->fShrink), pTitle->pos.y - ((TITLE_PRESSENTER_HEIGHT / 2) * pTitle->fShrink), 0.0f);
				pVertex[2].pos = D3DXVECTOR3(pTitle->pos.x + ((TITLE_PRESSENTER_WIDTH / 2) * pTitle->fShrink), pTitle->pos.y + ((TITLE_PRESSENTER_HEIGHT / 2) * pTitle->fShrink), 0.0f);
				pVertex[3].pos = D3DXVECTOR3(pTitle->pos.x + ((TITLE_PRESSENTER_WIDTH / 2) * pTitle->fShrink), pTitle->pos.y - ((TITLE_PRESSENTER_HEIGHT / 2) * pTitle->fShrink), 0.0f);

				//頂点カラーの設定
				pVertex[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pTitle->fBlink);  //D3DCOLOR_RGBA(赤, 緑, 青, 透明度);
				pVertex[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pTitle->fBlink);
				pVertex[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pTitle->fBlink);
				pVertex[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pTitle->fBlink);
				break;
			}
			pVertex += 4;
		}

		//頂点バッファをアンロックする
		g_pVtxBuffTitle->Unlock();
	}

	if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_BACKSPACE) == true)
	{
		SetFade(FADE_OUT, MODE_TUTORIAL);
	}
}

//-----------------------------------------------------------------------------
// タイトルの描画処理
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


	for (int nCntTex = 0; nCntTex < TITLE_POLYGON; nCntTex++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTitle[nCntTex]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTex * 4, 2);
	}
}

//-----------------------------------------------------------------------------
// タイトルの演出 (縮小拡大)
//-----------------------------------------------------------------------------
float TitleShrink(float fShrink)
{
	static bool bShrink = true;

	//縮小と拡大の切り替え
	if (bShrink == true)
	{
		fShrink -= TITLE_SHRINKSPD;
	}
	else if (bShrink == false)
	{
		fShrink += TITLE_SHRINKSPD;
	}

	//縮小拡大の下限上限
	if (fShrink >= 1.0f)
	{
		bShrink = true;
	}
	else if (fShrink <= TITLE_SHRINKRANGE)
	{
		bShrink = false;
	}

	return fShrink;
}

//-----------------------------------------------------------------------------
// タイトルの演出 (点滅)
//-----------------------------------------------------------------------------
float TitleBlink(float fBlink)
{
	static bool bBlink = false;
	static bool bOnce = false;
	static DWORD dwTime;
	static DWORD dwCTime;

	//現在時
	dwCTime = timeGetTime();

	//最初の時間を記憶
	if (bOnce == false)
	{
		dwTime = timeGetTime();
		bOnce = true;
	}

	//点滅切り替え
	if (bBlink == true)
	{
		fBlink = 0.0f;
	}
	else if (bBlink == false)
	{
		fBlink = 1.0f;
	}

	//現在時から最初時を引いて判定
	if ((dwCTime - dwTime) >= TITLE_BLINKTIMEFIXED)
	{
		bBlink = !bBlink;
		dwTime = timeGetTime();
	}

	return fBlink;
}