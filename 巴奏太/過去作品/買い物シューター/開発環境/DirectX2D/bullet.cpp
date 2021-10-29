//-----------------------------------------------------------------------------
//
// 弾処理 [bullet.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "bullet.h"

//マクロ
#define BULLET_WIDTH (20)
#define BULLET_HEIGHT (20)

//-----------------------------------------------------------------------------
//構造体宣言
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;  //位置
	D3DXVECTOR3 move; //移動量
	int nLife;		  //寿命
	bool bUse;		  //使用しているか
	BULLETSTATE state; //弾の状態
	BULLETTYPE type;  //弾の種類
} BULLET;

//-----------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	//バッファへのポインタ
BULLET g_aBullet[MAX_BULLET];						//弾の情報
bool bTest;

//-----------------------------------------------------------------------------
//弾の初期化処理
//-----------------------------------------------------------------------------
HRESULT InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\laser-bolts.png", &g_pTextureBullet);

	//BULLETの初期化
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = 0;
		g_aBullet[nCntBullet].bUse = false;
		g_aBullet[nCntBullet].state = BULLETSTATE_OWNER_PLAYER;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * MAX_BULLET, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBullet, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファのロック
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVertex, 0);

	//頂点座標
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
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
	
	g_pVtxBuffBullet->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//弾の終了処理
//-----------------------------------------------------------------------------
void UninitBullet(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}

	//テクスチャの開放
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}
}

//-----------------------------------------------------------------------------
//弾の更新処理
//-----------------------------------------------------------------------------
void UpdateBullet(void)
{
	BULLET *pBullet;
	ENEMY *pEnemy;
	PLAYER *pPlayer;
	VERTEX_2D *pVertex;
	
	pBullet = &g_aBullet[0];

	if (g_pVtxBuffBullet != NULL)
	{
		//頂点バッファのロック
		g_pVtxBuffBullet->Lock(0, 0, (void**)&pVertex, 0);

		for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
		{
			if (pBullet->bUse == true)
			{
				//位置更新
				g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
				g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;

				pEnemy = GetEnemy();
				pPlayer = GetPlayer();

				//プレイヤーが弾を発射した場合
				if (g_aBullet[nCntBullet].state == BULLETSTATE_OWNER_PLAYER)
				{
					//エフェクト
					SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f), 10, 0.06f);

					for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
					{
						if (pEnemy->bUse == true)
						{
							//UVの設定
							pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f / 2);
							pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
							pVertex[2].tex = D3DXVECTOR2(1.0f / 2, 1.0f / 2);
							pVertex[3].tex = D3DXVECTOR2(1.0f / 2, 0.0f);

							switch (pEnemy->nType)
							{
							case ENEMYTYPE_BIG:
								if (pBullet->pos.x > pEnemy->pos.x - ENEMY_BIG_WIDTH && pBullet->pos.x < pEnemy->pos.x + ENEMY_BIG_HEIGHT &&
									pBullet->pos.y > pEnemy->pos.y - ENEMY_BIG_WIDTH && pBullet->pos.y < pEnemy->pos.y + ENEMY_BIG_HEIGHT)
								{
									//ヒット判定
									HitEnemy(nCntEnemy, 5);

									//弾の破棄
									pBullet->bUse = false;
								}
								break;

							case ENEMYTYPE_MEDIUM:
								if (pBullet->pos.x > pEnemy->pos.x - ENEMY_MEDIUM_WIDTH && pBullet->pos.x < pEnemy->pos.x + ENEMY_MEDIUM_HEIGHT &&
									pBullet->pos.y > pEnemy->pos.y - ENEMY_MEDIUM_WIDTH && pBullet->pos.y < pEnemy->pos.y + ENEMY_MEDIUM_HEIGHT)
								{
									//ヒット判定
									HitEnemy(nCntEnemy, 5);

									//弾の破棄
									pBullet->bUse = false;
								}
								break;

							case ENEMYTYPE_SMALL:
								if (pBullet->pos.x > pEnemy->pos.x - ENEMY_BIG_WIDTH && pBullet->pos.x < pEnemy->pos.x + ENEMY_SMALL_HEIGHT &&
									pBullet->pos.y > pEnemy->pos.y - ENEMY_BIG_WIDTH && pBullet->pos.y < pEnemy->pos.y + ENEMY_SMALL_HEIGHT)
								{
									//ヒット判定
									HitEnemy(nCntEnemy, 5);

									//弾の破棄
									pBullet->bUse = false;
								}
								break;

							default:
								break;
							}


						}
					}
				}

				//敵が弾を発射した場合
				if (g_aBullet[nCntBullet].state == BULLETSTATE_OWNER_ENEMY)
				{
					if (pPlayer->bUse == true)
					{
						//UVの設定
						pVertex[0].tex = D3DXVECTOR2(1.0f / 2, 1.0f / 2);
						pVertex[1].tex = D3DXVECTOR2(1.0f / 2, 0.0f);
						pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f / 2);
						pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f);
						
						//エフェクト
						SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.6f, 0.2f, 0.2f, 1.0f), 10, 0.06f);

						if (pBullet->pos.x > pPlayer->pos.x - PLAYER_WIDTH && pBullet->pos.x < pPlayer->pos.x + PLAYER_HEIGHT &&
							pBullet->pos.y > pPlayer->pos.y - PLAYER_WIDTH && pBullet->pos.y < pPlayer->pos.y + PLAYER_HEIGHT)
						{
							if (CheckBarrierActive() == true)
							{
								BreakBarrier();

							}
							else
							{
								//ヒット判定
								HitPlayer(5);
							}

							//弾の破棄
							pBullet->bUse = false;
						}
					}
				}

				//頂点座標の更新
				pVertex[0].pos = D3DXVECTOR3(-BULLET_WIDTH + g_aBullet[nCntBullet].pos.x, BULLET_HEIGHT + g_aBullet[nCntBullet].pos.y, 0.0f);
				pVertex[1].pos = D3DXVECTOR3(-BULLET_WIDTH + g_aBullet[nCntBullet].pos.x, -BULLET_HEIGHT + g_aBullet[nCntBullet].pos.y, 0.0f);
				pVertex[2].pos = D3DXVECTOR3(BULLET_WIDTH + g_aBullet[nCntBullet].pos.x, BULLET_HEIGHT + g_aBullet[nCntBullet].pos.y, 0.0f);
				pVertex[3].pos = D3DXVECTOR3(BULLET_WIDTH + g_aBullet[nCntBullet].pos.x, -BULLET_HEIGHT + g_aBullet[nCntBullet].pos.y, 0.0f);


	
				//寿命チェック
				g_aBullet[nCntBullet].nLife--;
				if (g_aBullet[nCntBullet].nLife < 0)
				{
					//SetExplosion(g_aBullet[nCntBullet].pos, 50);
					g_aBullet[nCntBullet].bUse = false;
				}

				//画面外チェック
				if (g_aBullet[nCntBullet].pos.x > SCREEN_WIDTH || g_aBullet[nCntBullet].pos.x < 0)
				{
					g_aBullet[nCntBullet].bUse = false;
				}
				if (g_aBullet[nCntBullet].pos.y > SCREEN_HEIGHT || g_aBullet[nCntBullet].pos.y < 0)
				{
					g_aBullet[nCntBullet].bUse = false;
				}
			}
			pVertex += 4; //頂点のポインタをずらす
		}

		//頂点バッファのアンロック
		g_pVtxBuffBullet->Unlock();
	}
}

//-----------------------------------------------------------------------------
//弾の描画処理
//-----------------------------------------------------------------------------
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBullet);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}
	}
}

//-----------------------------------------------------------------------------
//弾の設定
//-----------------------------------------------------------------------------
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, BULLETSTATE state, BULLETTYPE type)
{
	BULLET *pBullet;
	VERTEX_2D *pVertex;

	pBullet = &g_aBullet[0];

	//頂点バッファのロック
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse == false)
		{
			//位置を設定
			g_aBullet[nCntBullet].pos = pos;

			//頂点座標を設定
			pVertex[0].pos = D3DXVECTOR3(-10 + g_aBullet[nCntBullet].pos.x, 10 + g_aBullet[nCntBullet].pos.y, 0.0f);
			pVertex[1].pos = D3DXVECTOR3(-10 + g_aBullet[nCntBullet].pos.x, -10 + g_aBullet[nCntBullet].pos.y, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(10 + g_aBullet[nCntBullet].pos.x, 10 + g_aBullet[nCntBullet].pos.y, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(10 + g_aBullet[nCntBullet].pos.x, -10 + g_aBullet[nCntBullet].pos.y, 0.0f);

			//移動量を設定
			g_aBullet[nCntBullet].move = move;

			//寿命を設定
			g_aBullet[nCntBullet].nLife = nLife;

			//ステートを設定
			g_aBullet[nCntBullet].state = state;

			//弾の種類を設定
			g_aBullet[nCntBullet].type = type;

			pBullet->bUse = true;
			break;

		}
		//頂点のポインタをずらす
		pVertex += 4;
	}
	g_pVtxBuffBullet->Unlock();
}