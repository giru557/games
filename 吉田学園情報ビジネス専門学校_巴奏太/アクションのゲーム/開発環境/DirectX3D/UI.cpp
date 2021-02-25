//-----------------------------------------------------------------------------
//
// UIの処理 [UI.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "UI.h"
#include "player.h"

//-----------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
HRESULT InitHPBar(void);			// HPバー
void UninitHPBar(void);				//
void UpdateHPBar(void);				//
void DrawHPBar(void);				//
HRESULT InitGameOver(void);			// ゲームオーバー表示
void UninitGameOver(void);			//
void UpdateGameOver(void);			//
void DrawGameOver(void);			//
HRESULT InitEnemyRemaining(void);	// 敵の残り数
void UninitEnemyRemaining(void);	//
void UpdateEnemyRemaining(void);	//
void DrawEnemyRemaining(void);		//

//-----------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------
HPBAR g_aHPBar[HEALTH_OBJ_AMOUNT];		// 体力バーの情報
GAMEOVER g_gameover;				// ゲームオーバー表示の情報
EnemyUI g_enemyUI;					// 敵の残り数の情報

//-----------------------------------------------------------------------------
//UIの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitUI(void)
{
	//HPバーの初期化
	HRESULT hrHPBar = InitHPBar();

	//ゲームオーバーの初期化
	HRESULT hrGameOver = InitGameOver();

	//敵の残り数の初期化
	//HRESULT hrEnemyRemaining = InitEnemyRemaining();

	return S_OK;
}

//-----------------------------------------------------------------------------
//UIの終了処理
//-----------------------------------------------------------------------------
void UninitUI(void)
{
	//敵の残り数の終了処理
	//UninitEnemyRemaining();

	//ゲームオーバーの終了処理
	UninitGameOver();

	//HPバーの終了処理
	UninitHPBar();
}

//-----------------------------------------------------------------------------
//UIの更新処理
//-----------------------------------------------------------------------------
void UpdateUI(void)
{
	//HPバーの更新処理
	UpdateHPBar();

	//ゲームオーバーの更新処理
	UpdateGameOver();

	//敵の残り数の更新処理
	//UpdateEnemyRemaining();
}

//-----------------------------------------------------------------------------
//UIの描画処理
//-----------------------------------------------------------------------------
void DrawUI(void)
{
	//HPバーの描画処理
	DrawHPBar();

	//ゲームオーバーの描画処理
	DrawGameOver();

	//敵の残り数の描画処理
	//DrawEnemyRemaining();
}

//-----------------------------------------------------------------------------
// HPバーの情報取得
//-----------------------------------------------------------------------------
HPBAR *GetHPBAR(void)
{
	return &g_aHPBar[0];
}

//-----------------------------------------------------------------------------
// ゲームオーバー画面の情報取得
//-----------------------------------------------------------------------------
GAMEOVER *GetGameOver(void)
{
	return &g_gameover;
}

//-----------------------------------------------------------------------------
//HPバーの初期化
HRESULT InitHPBar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス取得

	//初期化
	g_aHPBar[0].pos = D3DXVECTOR3((200), 70, 0.0f);
	g_aHPBar[1].pos = D3DXVECTOR3((105), 70, 0.0f);

	//テクスチャ読み込み
	D3DXCreateTextureFromFileEx(pDevice, "data\\TEXTURE\\health_bar_decoration.png", 66, 19, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0xFF000000, NULL, NULL, &g_aHPBar[0].pTexture);
	D3DXCreateTextureFromFileEx(pDevice, "data\\TEXTURE\\health_bar.png", 51, 19, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0xFF000000, NULL, NULL, &g_aHPBar[1].pTexture);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_aHPBar[0].pVtxBuff, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVertexBack;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_aHPBar[0].pVtxBuff->Lock(0, 0, (void**)&pVertexBack, 0);

	//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
	pVertexBack[0].pos = D3DXVECTOR3(g_aHPBar[0].pos.x - HEALTH_BAR_WIDTH, g_aHPBar[1].pos.y + HEALTH_BAR_HEIGHT, 0.0f);		 //TRIANGLESTRIPで四角
	pVertexBack[1].pos = D3DXVECTOR3(g_aHPBar[0].pos.x - HEALTH_BAR_WIDTH, g_aHPBar[1].pos.y - HEALTH_BAR_HEIGHT, 0.0f);
	pVertexBack[2].pos = D3DXVECTOR3(g_aHPBar[0].pos.x + HEALTH_BAR_WIDTH, g_aHPBar[1].pos.y + HEALTH_BAR_HEIGHT, 0.0f);
	pVertexBack[3].pos = D3DXVECTOR3(g_aHPBar[0].pos.x + HEALTH_BAR_WIDTH, g_aHPBar[1].pos.y - HEALTH_BAR_HEIGHT, 0.0f);

	//rhwの設定
	for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertexBack[nCntVtx].rhw = 1.0f;

	//頂点カラーの設定
	for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertexBack[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//頂点情報の設定
	pVertexBack[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVertexBack[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVertexBack[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVertexBack[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//頂点バッファをアンロックする
	g_aHPBar[0].pVtxBuff->Unlock();

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_aHPBar[1].pVtxBuff, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVertexFront;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_aHPBar[1].pVtxBuff->Lock(0, 0, (void**)&pVertexFront, 0);

	//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
	pVertexFront[0].pos = D3DXVECTOR3(g_aHPBar[1].pos.x - HEALTH_WIDTH, g_aHPBar[1].pos.y + HEALTH_HEIGHT, 0.0f);
	pVertexFront[1].pos = D3DXVECTOR3(g_aHPBar[1].pos.x - HEALTH_WIDTH, g_aHPBar[1].pos.y - HEALTH_HEIGHT, 0.0f);
	pVertexFront[2].pos = D3DXVECTOR3(g_aHPBar[1].pos.x + HEALTH_WIDTH, g_aHPBar[1].pos.y + HEALTH_HEIGHT, 0.0f);
	pVertexFront[3].pos = D3DXVECTOR3(g_aHPBar[1].pos.x + HEALTH_WIDTH, g_aHPBar[1].pos.y - HEALTH_HEIGHT, 0.0f);

	//rhwの設定
	for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertexFront[nCntVtx].rhw = 1.0f;

	//頂点カラーの設定
	for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertexFront[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//頂点情報の設定
	pVertexFront[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVertexFront[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVertexFront[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVertexFront[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//頂点バッファアンロック
	g_aHPBar[1].pVtxBuff->Unlock();

	return S_OK;
}

//HPバーの終了処理
void UninitHPBar(void)
{
	for (int nCntH = 0; nCntH < HEALTH_OBJ_AMOUNT; nCntH++)
	{
		//頂点バッファの開放
		if (g_aHPBar[nCntH].pVtxBuff != NULL)
		{
			g_aHPBar[nCntH].pVtxBuff->Release();
			g_aHPBar[nCntH].pVtxBuff = NULL;
		}

		//テクスチャの開放
		if (g_aHPBar[nCntH].pTexture != NULL)
		{
			g_aHPBar[nCntH].pTexture->Release();
			g_aHPBar[nCntH].pTexture = NULL;
		}
	}
}

//HPバーの更新処理
void UpdateHPBar(void)
{
	VERTEX_2D *pVertex;
	Player *pPlayer = GetPlayer();
	static float fValue = 1.0f;

	//最大体力と現在体力の割合
	fValue = (float)pPlayer->nLife / (float)pPlayer->nLifeMax;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_aHPBar[1].pVtxBuff->Lock(0, 0, (void**)&pVertex, 0);

	////頂点座標 D3DXVECTOR3(X,Y, 0.0f);
	pVertex[0].pos = D3DXVECTOR3(g_aHPBar[1].pos.x, g_aHPBar[1].pos.y + HEALTH_HEIGHT, 0.0f);		 //TRIANGLESTRIPで四角
	pVertex[1].pos = D3DXVECTOR3(g_aHPBar[1].pos.x, g_aHPBar[1].pos.y - HEALTH_HEIGHT, 0.0f);
	pVertex[2].pos = D3DXVECTOR3(g_aHPBar[1].pos.x + (HEALTH_WIDTH * fValue), g_aHPBar[1].pos.y + HEALTH_HEIGHT, 0.0f);
	pVertex[3].pos = D3DXVECTOR3(g_aHPBar[1].pos.x + (HEALTH_WIDTH * fValue), g_aHPBar[1].pos.y - HEALTH_HEIGHT, 0.0f);

	//頂点情報の設定
	pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVertex[2].tex = D3DXVECTOR2(1.0f * fValue, 1.0f);
	pVertex[3].tex = D3DXVECTOR2(1.0f * fValue, 0.0f);

	//頂点バッファをアンロックする
	g_aHPBar[1].pVtxBuff->Unlock();
}

//HPバーの描画処理
void DrawHPBar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス取得

	for (int nCntH = 0; nCntH < HEALTH_OBJ_AMOUNT; nCntH++)
	{
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_aHPBar[nCntH].pVtxBuff, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_aHPBar[nCntH].pTexture);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 4);
	}
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//ゲームオーバーの初期化
HRESULT InitGameOver(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス取得

	//初期化
	g_gameover.bDraw = false;
	g_gameover.bGameOver = false;
	g_gameover.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\stage_completedBG.png", &g_gameover.pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\stage_completed.png", &g_gameover.pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, NULL, &g_gameover.pTexture[2]);

	//背景用
	VERTEX_2D *pVertexBG;

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_gameover.pVtxBuff[0], NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_gameover.pVtxBuff[0]->Lock(0, 0, (void**)&pVertexBG, 0);

	//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
	pVertexBG[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);		 //TRIANGLESTRIPで四角
	pVertexBG[1].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVertexBG[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	pVertexBG[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

	//rhwの設定
	for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertexBG[nCntVtx].rhw = 1.0f;	// 1.0固定

	//頂点カラーの設定
	for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertexBG[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//頂点情報の設定
	pVertexBG[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVertexBG[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVertexBG[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVertexBG[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//頂点バッファアンロック
	g_gameover.pVtxBuff[0]->Unlock();

	return S_OK;
}

//ゲームオーバーの終了処理
void UninitGameOver(void)
{
	//頂点バッファの破棄
	for (int nCntVtxBuff = 0; nCntVtxBuff < 2; nCntVtxBuff++)
	{
		if (g_gameover.pVtxBuff[nCntVtxBuff] != NULL)
		{
			g_gameover.pVtxBuff[nCntVtxBuff]->Release();
			g_gameover.pVtxBuff[nCntVtxBuff] = NULL;
		}
	}

	//テクスチャの破棄
	for (int nCntTex = 0; nCntTex < 3; nCntTex++)
	{
		if (g_gameover.pTexture[nCntTex] != NULL)
		{
			g_gameover.pTexture[nCntTex]->Release();
			g_gameover.pTexture[nCntTex] = NULL;
		}
	}
}

//ゲームオーバーの更新処理
void UpdateGameOver(void)
{

}

//ゲームオーバーの描画処理
void DrawGameOver(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス取得

	if (g_gameover.bDraw == true)
	{
		for (int nCntVtxBuff = 0; nCntVtxBuff < 2; nCntVtxBuff++)
		{
			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_gameover.pVtxBuff[nCntVtxBuff], 0, sizeof(VERTEX_2D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			switch (nCntVtxBuff)
			{
			case 0:
				//テクスチャの設定
				pDevice->SetTexture(0, g_gameover.pTexture[0]);
				break;
			case 1:
				if (g_gameover.bGameOver == true)
				{
					//テクスチャの設定
					pDevice->SetTexture(0, g_gameover.pTexture[2]);
				}
				else
				{
					//テクスチャの設定
					pDevice->SetTexture(0, g_gameover.pTexture[1]);
				}
			default:
				break;
			}

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		}
	}
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//敵残量の初期化
HRESULT InitEnemyRemaining(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス取得

	//初期化
	g_enemyUI.pos[0] = D3DXVECTOR3(SCREEN_WIDTH / 2, 0.0f, 0.0f);
	g_enemyUI.pos[1] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number000.png", &g_enemyUI.pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, NULL, &g_enemyUI.pTexture[1]);

	for (int nCntObj = 0; nCntObj < ENEMYUI_OBJ_AMOUNT; nCntObj++)
	{
		//頂点バッファの生成
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * ENEMYUI_SHOWDIGIT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_enemyUI.pVtxBuff[nCntObj], NULL)))
		{
			return E_FAIL;
		}
	}

	//数字用
	VERTEX_2D *pVertexNUM;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_enemyUI.pVtxBuff[0]->Lock(0, 0, (void**)&pVertexNUM, 0);

	for (int nCntNum = 0; nCntNum < ENEMYUI_SHOWDIGIT; nCntNum++, pVertexNUM += 4)
	{
		//頂点座標の設定
		pVertexNUM[0].pos = D3DXVECTOR3((g_enemyUI.pos[0].x) + nCntNum * ENEMYUI_SPACE_BETWEEN_CHAR, g_enemyUI.pos[0].y + ENEMYUI_CHAR_HEIGHT, 0.0f);
		pVertexNUM[1].pos = D3DXVECTOR3((g_enemyUI.pos[0].x) + nCntNum * ENEMYUI_SPACE_BETWEEN_CHAR, g_enemyUI.pos[0].y, 0.0f);
		pVertexNUM[2].pos = D3DXVECTOR3((g_enemyUI.pos[0].x + ENEMYUI_CHAR_WIDTH) + nCntNum * ENEMYUI_SPACE_BETWEEN_CHAR, g_enemyUI.pos[0].y + ENEMYUI_CHAR_HEIGHT, 0.0f);
		pVertexNUM[3].pos = D3DXVECTOR3((g_enemyUI.pos[0].x + ENEMYUI_CHAR_WIDTH) + nCntNum * ENEMYUI_SPACE_BETWEEN_CHAR, g_enemyUI.pos[0].y, 0.0f);

		//rhwの設定
		for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertexNUM[nCntVtx].rhw = 1.0f;	// 1.0f固定

		//頂点カラーの設定
		for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertexNUM[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//頂点情報の設定
		pVertexNUM[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVertexNUM[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVertexNUM[2].tex = D3DXVECTOR2(1.0f * 0.1f, 1.0f);
		pVertexNUM[3].tex = D3DXVECTOR2(1.0f * 0.1f, 0.0f);
	}

	//アンロック
	g_enemyUI.pVtxBuff[0]->Unlock();

	//画像用
	VERTEX_2D *pVertexPIC;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_enemyUI.pVtxBuff[1]->Lock(0, 0, (void**)&pVertexPIC, 0);

	//頂点座標の設定
	pVertexPIC[0].pos = D3DXVECTOR3(g_enemyUI.pos[1].x, g_enemyUI.pos[1].y + ENEMYUI_PICTURE_HEIGHT, 0.0f);
	pVertexPIC[1].pos = D3DXVECTOR3(g_enemyUI.pos[1].x, g_enemyUI.pos[1].y, 0.0f);
	pVertexPIC[2].pos = D3DXVECTOR3(g_enemyUI.pos[1].x + ENEMYUI_PICTURE_WIDTH, g_enemyUI.pos[1].y + ENEMYUI_PICTURE_HEIGHT, 0.0f);
	pVertexPIC[3].pos = D3DXVECTOR3(g_enemyUI.pos[1].x + ENEMYUI_PICTURE_WIDTH, g_enemyUI.pos[1].y, 0.0f);

	//rhw設定
	for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertexPIC[nCntVtx].rhw = 1.0f;	// 1.0f固定

	//頂点カラーの設定
	for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertexPIC[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	
	//頂点情報の設定
	pVertexPIC[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVertexPIC[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVertexPIC[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVertexPIC[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//アンロック
	g_enemyUI.pVtxBuff[1]->Unlock();

	return S_OK;
}

//敵残量の終了処理
void UninitEnemyRemaining(void)
{
	for (int nCntObj = 0; nCntObj < ENEMYUI_OBJ_AMOUNT; nCntObj++)
	{
		//頂点バッファの開放
		if (g_enemyUI.pVtxBuff[nCntObj] != NULL)
		{
			g_enemyUI.pVtxBuff[nCntObj]->Release();
			g_enemyUI.pVtxBuff[nCntObj] = NULL;
		}

		//テクスチャの開放
		if (g_enemyUI.pTexture[nCntObj] != NULL)
		{
			g_enemyUI.pTexture[nCntObj]->Release();
			g_enemyUI.pTexture[nCntObj] = NULL;
		}
	}
}

//敵残量の更新処理
void UpdateEnemyRemaining(void)
{
	VERTEX_2D *pVertexNUM;				// 数字用
	int aNumAmount[ENEMYUI_SHOWDIGIT];	// 一文字ずつ配列で

	//時間を分解
	aNumAmount[0] = g_enemyUI.nAmount % 100 / 10;
	aNumAmount[1] = g_enemyUI.nAmount % 10 / 1;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_enemyUI.pVtxBuff[0]->Lock(0, 0, (void**)&pVertexNUM, 0);

	for (int nCntNum = 0; nCntNum < ENEMYUI_SHOWDIGIT; nCntNum++)
	{
		//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
		pVertexNUM[0].pos = D3DXVECTOR3((g_enemyUI.pos[0].x) + (nCntNum + 1) * ENEMYUI_SPACE_BETWEEN_CHAR, (g_enemyUI.pos[0].y + ENEMYUI_CHAR_HEIGHT), 0.0f);		 //TRIANGLESTRIPで四角
		pVertexNUM[1].pos = D3DXVECTOR3((g_enemyUI.pos[0].x) + (nCntNum + 1) * ENEMYUI_SPACE_BETWEEN_CHAR, (g_enemyUI.pos[0].y), 0.0f);
		pVertexNUM[2].pos = D3DXVECTOR3((g_enemyUI.pos[0].x + ENEMYUI_CHAR_WIDTH) + (nCntNum + 1) * ENEMYUI_SPACE_BETWEEN_CHAR, (g_enemyUI.pos[0].y + ENEMYUI_CHAR_HEIGHT), 0.0f);
		pVertexNUM[3].pos = D3DXVECTOR3((g_enemyUI.pos[0].x + ENEMYUI_CHAR_WIDTH) + (nCntNum + 1) * ENEMYUI_SPACE_BETWEEN_CHAR, (g_enemyUI.pos[0].y), 0.0f);

		//テクスチャ座標の更新
		pVertexNUM[0].tex = D3DXVECTOR2(0.1f * (aNumAmount[nCntNum]), 1.0f);
		pVertexNUM[1].tex = D3DXVECTOR2(0.1f * (aNumAmount[nCntNum]), 0.0f);
		pVertexNUM[2].tex = D3DXVECTOR2(0.1f * (aNumAmount[nCntNum] + 1), 1.0f);
		pVertexNUM[3].tex = D3DXVECTOR2(0.1f * (aNumAmount[nCntNum] + 1), 0.0f);
	}

	//頂点バッファをアンロックする
	g_enemyUI.pVtxBuff[0]->Unlock();

	g_enemyUI.nAmount++;
	if (g_enemyUI.nAmount >= 100) g_enemyUI.nAmount = 0;
}

//敵残量の描画処理
void DrawEnemyRemaining(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス取得

	for (int nCntObj = 0; nCntObj < ENEMYUI_OBJ_AMOUNT; nCntObj++)
	{
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_enemyUI.pVtxBuff[nCntObj], 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		pDevice->SetTexture(0, g_enemyUI.pTexture[nCntObj]);

		//画像か数字でスイッチ
		switch (nCntObj)
		{
			//数字
		case 0:
			for (int nCntNum = 0; nCntNum < ENEMYUI_SHOWDIGIT; nCntNum++)
			{
				//ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntNum * 4, 2);
			}
			break;

			//画像
		case 1:
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
			break;

		default:
			break;
		}
	}
}
