//-----------------------------------------------------------------------------
//
// ミニガンの処理 [minigun.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "minigun.h"

//マクロ
#define MINIGUN_WIDTH (10 * 0.5f)
#define MINIGUN_HEIGHT (40 * 0.5f)

//-----------------------------------------------------------------------------
//構造体宣言
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;  //位置
	D3DXVECTOR3 move; //移動量
	int nLife;		  //寿命
	bool bUse;		  //使用しているか
	MINIGUNSTATE state; //弾の状態
	MINIGUNTYPE type;  //弾の種類
} MINIGUN;

//-----------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureMinigun = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMinigun = NULL;	//バッファへのポインタ
MINIGUN g_aMinigun[MAX_MINIGUN];						//弾の情報
int g_nCounter;

//-----------------------------------------------------------------------------
//ミニガンの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitMinigun(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\minigun.png", &g_pTextureMinigun);

	//BULLETの初期化
	for (int nCntMinigun = 0; nCntMinigun < MAX_MINIGUN; nCntMinigun++)
	{
		g_aMinigun[nCntMinigun].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMinigun[nCntMinigun].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMinigun[nCntMinigun].nLife = 0;
		g_aMinigun[nCntMinigun].bUse = false;
		g_aMinigun[nCntMinigun].state = MINIGUNSTATE_OWNER_PLAYER;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * MAX_MINIGUN, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffMinigun, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファのロック
	g_pVtxBuffMinigun->Lock(0, 0, (void**)&pVertex, 0);

	//頂点座標
	for (int nCntMinigun = 0; nCntMinigun < MAX_MINIGUN; nCntMinigun++)
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
		pVertex[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);  //D3DCOLOR_RGBA(赤, 緑, 青, 透明度);
		pVertex[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVertex[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVertex[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//UVの設定
		pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVertex += 4; //頂点のポインタをずらす
	}

	g_pVtxBuffMinigun->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//ミニガンの終了処理
//-----------------------------------------------------------------------------
void UninitMinigun(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffMinigun != NULL)
	{
		g_pVtxBuffMinigun->Release();
		g_pVtxBuffMinigun = NULL;
	}

	//テクスチャの開放
	if (g_pTextureMinigun != NULL)
	{
		g_pTextureMinigun->Release();
		g_pTextureMinigun = NULL;
	}
}

//-----------------------------------------------------------------------------
//ミニガンの更新処理
//-----------------------------------------------------------------------------
void UpdateMinigun(void)
{
	MINIGUN *pMinigun;
	ENEMY *pEnemy;
	PLAYER *pPlayer;
	VERTEX_2D *pVertex;

	pMinigun = &g_aMinigun[0];

	if (g_pVtxBuffMinigun != NULL)
	{
		//頂点バッファのロック
		g_pVtxBuffMinigun->Lock(0, 0, (void**)&pVertex, 0);

		for (int nCntMinigun = 0; nCntMinigun < MAX_MINIGUN; nCntMinigun++, pMinigun++)
		{
			if (pMinigun->bUse == true)
			{
				//位置更新
				g_aMinigun[nCntMinigun].pos.x += g_aMinigun[nCntMinigun].move.x;
				g_aMinigun[nCntMinigun].pos.y += g_aMinigun[nCntMinigun].move.y;

				pEnemy = GetEnemy();
				pPlayer = GetPlayer();

				SetEffect(g_aMinigun[nCntMinigun].pos, D3DXCOLOR(0.2f, 0.02f, 0.4f, 0.6f), 10, 0.12f);
				SetEffect(g_aMinigun[nCntMinigun].pos, D3DXCOLOR(0.2f, 0.02f, 0.4f, 1.0f), 5, 0.12f);

				//プレイヤーが弾を発射した場合
				if (g_aMinigun[nCntMinigun].state == MINIGUNSTATE_OWNER_PLAYER)
				{
					for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
					{
						if (pEnemy->bUse == true)
						{
							if (pMinigun->pos.x > pEnemy->pos.x - 30.0f && pMinigun->pos.x < pEnemy->pos.x + 30.0f &&
								pMinigun->pos.y > pEnemy->pos.y - 30.0f && pMinigun->pos.y < pEnemy->pos.y + 30.0f)
							{
								//ヒット判定
								HitEnemy(nCntEnemy, 2);

								//弾の破棄
								pMinigun->bUse = false;
							}
						}
					}
				}

				if (g_aMinigun[nCntMinigun].state == MINIGUNSTATE_OWNER_ENEMY)
				{
					if (pPlayer->bUse == true)
					{
						if (pMinigun->pos.x > pPlayer->pos.x - 40.0f && pMinigun->pos.x < pPlayer->pos.x + 60.0f &&
							pMinigun->pos.y > pPlayer->pos.y - 40.0f && pMinigun->pos.y < pPlayer->pos.y + 60.0f)
						{
							//ヒット判定
							HitPlayer(5);

							//弾の破棄
							pMinigun->bUse = false;
						}
					}
				}

				//頂点座標を設定
				pVertex[0].pos = D3DXVECTOR3(-MINIGUN_WIDTH + g_aMinigun[nCntMinigun].pos.x, MINIGUN_HEIGHT + g_aMinigun[nCntMinigun].pos.y, 0.0f);
				pVertex[1].pos = D3DXVECTOR3(-MINIGUN_WIDTH + g_aMinigun[nCntMinigun].pos.x, -MINIGUN_HEIGHT + g_aMinigun[nCntMinigun].pos.y, 0.0f);
				pVertex[2].pos = D3DXVECTOR3(MINIGUN_WIDTH + g_aMinigun[nCntMinigun].pos.x, MINIGUN_HEIGHT + g_aMinigun[nCntMinigun].pos.y, 0.0f);
				pVertex[3].pos = D3DXVECTOR3(MINIGUN_WIDTH + g_aMinigun[nCntMinigun].pos.x, -MINIGUN_HEIGHT + g_aMinigun[nCntMinigun].pos.y, 0.0f);

				//画面外チェック
				if (g_aMinigun[nCntMinigun].pos.x > SCREEN_WIDTH || g_aMinigun[nCntMinigun].pos.x < 0)
				{
					g_aMinigun[nCntMinigun].bUse = false;
				}
				if (g_aMinigun[nCntMinigun].pos.y > SCREEN_HEIGHT || g_aMinigun[nCntMinigun].pos.y < 0)
				{
					g_aMinigun[nCntMinigun].bUse = false;
				}
			}
			pVertex += 4; //頂点のポインタをずらす
		}

		//頂点バッファのアンロック
		g_pVtxBuffMinigun->Unlock();
	}
}

//-----------------------------------------------------------------------------
//ミニガンの描画処理
//-----------------------------------------------------------------------------
void DrawMinigun(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMinigun, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMinigun);

	for (int nCntMinigun = 0; nCntMinigun < MAX_MINIGUN; nCntMinigun++)
	{
		if (g_aMinigun[nCntMinigun].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntMinigun * 4, 2);
		}
	}
}

//-----------------------------------------------------------------------------
//ミニガンの設定
//-----------------------------------------------------------------------------
void SetMinigun(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, MINIGUNSTATE state)
{
	MINIGUN *pMinigun;
	VERTEX_2D *pVertex;

	pMinigun = &g_aMinigun[0];

	//頂点バッファのロック
	g_pVtxBuffMinigun->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntMinigun = 0; nCntMinigun < MAX_MINIGUN; nCntMinigun++, pMinigun++)
	{
		if (pMinigun->bUse == false)
		{
			//位置を設定
			g_aMinigun[nCntMinigun].pos = pos;

			//頂点座標を設定
			pVertex[0].pos = D3DXVECTOR3(-10 + g_aMinigun[nCntMinigun].pos.x, 10 + g_aMinigun[nCntMinigun].pos.y, 0.0f);
			pVertex[1].pos = D3DXVECTOR3(-10 + g_aMinigun[nCntMinigun].pos.x, -10 + g_aMinigun[nCntMinigun].pos.y, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(10 + g_aMinigun[nCntMinigun].pos.x, 10 + g_aMinigun[nCntMinigun].pos.y, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(10 + g_aMinigun[nCntMinigun].pos.x, -10 + g_aMinigun[nCntMinigun].pos.y, 0.0f);

			//移動量を設定
			g_aMinigun[nCntMinigun].move = move;

			//寿命を設定
			g_aMinigun[nCntMinigun].nLife = nLife;

			//ステートを設定
			g_aMinigun[nCntMinigun].state = state;

			//弾の種類を設定
			g_nCounter++;
			if (g_nCounter % 2 == 0)
			{
				g_aMinigun[nCntMinigun].type = MINIGUNTYPE_LEFT;
				
				g_aMinigun[nCntMinigun].pos.x += 15;
			}
			else
			{
				g_aMinigun[nCntMinigun].type = MINIGUNTYPE_RIGHT;
				
				g_aMinigun[nCntMinigun].pos.x -= 15;
			}

			pMinigun->bUse = true;
			break;
		}
		//頂点のポインタをずらす
		pVertex += 4;
	}
	g_pVtxBuffMinigun->Unlock();
}