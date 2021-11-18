//-----------------------------------------------------------------------------
//
// ミサイルの処理 [missile.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "missile.h"
#include <cmath>

//-----------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------
#define MISSILE_WIDTH ((41 / 2) * 0.8f)
#define MISSILE_HEIGHT ((92 / 2) * 0.8f)
#define MISSILE_EXPLOSIVE (100)

//-----------------------------------------------------------------------------
//構造体宣言
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;  //位置
	D3DXVECTOR3 move; //移動量
	int nLife;		  //寿命
	bool bUse;		  //使用しているか
	MISSILESTATE state; //弾の状態
	int nCounterAnim; //アニメーションカウンタ
	int nPatternAnim; //アニメーションパターン
} MISSILE;

//-----------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureMissile = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMissile = NULL;	//バッファへのポインタ
MISSILE g_aMissile[MAX_MISSILE];						//弾の情報

//-----------------------------------------------------------------------------
//ミサイルの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitMissile(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\missile2.png", &g_pTextureMissile);
	D3DXCreateTextureFromFileEx(pDevice, "data\\TEXTURE\\missile2.png", 82, 92, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0xFF000000, NULL, NULL, &g_pTextureMissile);

	//BULLETの初期化
	for (int nCntMissile = 0; nCntMissile < MAX_MISSILE; nCntMissile++)
	{
		g_aMissile[nCntMissile].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMissile[nCntMissile].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMissile[nCntMissile].nLife = 0;
		g_aMissile[nCntMissile].bUse = false;
		g_aMissile[nCntMissile].state = MISSILESTATE_OWNER_PLAYER;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * MAX_MISSILE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffMissile, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファのロック
	g_pVtxBuffMissile->Lock(0, 0, (void**)&pVertex, 0);

	//頂点座標
	for (int nCntMissile = 0; nCntMissile < MAX_MISSILE; nCntMissile++)
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
		pVertex[2].tex = D3DXVECTOR2(1.0f / 2, 1.0f);
		pVertex[3].tex = D3DXVECTOR2(1.0f / 2, 0.0f);

		pVertex += 4; //頂点のポインタをずらす
	}

	g_pVtxBuffMissile->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//ミサイルの終了処理
//-----------------------------------------------------------------------------
void UninitMissile(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffMissile != NULL)
	{
		g_pVtxBuffMissile->Release();
		g_pVtxBuffMissile = NULL;
	}

	//テクスチャの開放
	if (g_pTextureMissile != NULL)
	{
		g_pTextureMissile->Release();
		g_pTextureMissile = NULL;
	}
}

//-----------------------------------------------------------------------------
//ミサイルの更新処理
//-----------------------------------------------------------------------------
void UpdateMissile(void)
{
	MISSILE *pMissile;
	ENEMY *pEnemy;
	PLAYER *pPlayer;
	VERTEX_2D *pVertex;

	pMissile = &g_aMissile[0];

	if (g_pVtxBuffMissile != NULL)
	{
		//頂点バッファのロック
		g_pVtxBuffMissile->Lock(0, 0, (void**)&pVertex, 0);

		for (int nCntMissile = 0; nCntMissile < MAX_MISSILE; nCntMissile++, pMissile++)
		{
			if (pMissile->bUse == true)
			{
				//位置更新
				g_aMissile[nCntMissile].pos.x += g_aMissile[nCntMissile].move.x;
				g_aMissile[nCntMissile].pos.y += g_aMissile[nCntMissile].move.y;

				//頂点座標を設定
				pVertex[0].pos = D3DXVECTOR3(-MISSILE_WIDTH + g_aMissile[nCntMissile].pos.x, MISSILE_HEIGHT + g_aMissile[nCntMissile].pos.y, 0.0f);
				pVertex[1].pos = D3DXVECTOR3(-MISSILE_WIDTH + g_aMissile[nCntMissile].pos.x, -MISSILE_HEIGHT + g_aMissile[nCntMissile].pos.y, 0.0f);
				pVertex[2].pos = D3DXVECTOR3(MISSILE_WIDTH + g_aMissile[nCntMissile].pos.x, MISSILE_HEIGHT + g_aMissile[nCntMissile].pos.y, 0.0f);
				pVertex[3].pos = D3DXVECTOR3(MISSILE_WIDTH + g_aMissile[nCntMissile].pos.x, -MISSILE_HEIGHT + g_aMissile[nCntMissile].pos.y, 0.0f);

				//ANIMATION_INTERVALの数で割り切れた時
				g_aMissile[nCntMissile].nCounterAnim++;
				if (g_aMissile[nCntMissile].nCounterAnim % 5 == 0)
				{
					g_aMissile[nCntMissile].nPatternAnim = (g_aMissile[nCntMissile].nPatternAnim + 1) % 8; //8までいくとあまり0が代入されてリセット

															   //UV設定
					pVertex[0].tex = D3DXVECTOR2((float)g_aMissile[nCntMissile].nPatternAnim * 0.5f, 1.0f);
					pVertex[1].tex = D3DXVECTOR2((float)g_aMissile[nCntMissile].nPatternAnim * 0.5f, 0.0f);
					pVertex[2].tex = D3DXVECTOR2(((float)g_aMissile[nCntMissile].nPatternAnim + 1) * 0.5f, 1.0f);
					pVertex[3].tex = D3DXVECTOR2(((float)g_aMissile[nCntMissile].nPatternAnim + 1) * 0.5f, 0.0f);

				}
				////UVの設定
				//pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
				//pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
				//pVertex[2].tex = D3DXVECTOR2(1.0f / 2, 1.0f);
				//pVertex[3].tex = D3DXVECTOR2(1.0f / 2, 0.0f);

				//敵、プレイヤーへのポインタ取得
				pEnemy = GetEnemy();
				pPlayer = GetPlayer();

				//プレイヤーが弾を発射した場合
				if (g_aMissile[nCntMissile].state == MISSILESTATE_OWNER_PLAYER)
				{
					for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
					{
						if (pEnemy->bUse == true)
						{
							if (pMissile->pos.x > pEnemy->pos.x - ENEMY_SIZE && pMissile->pos.x < pEnemy->pos.x + ENEMY_SIZE &&
								pMissile->pos.y > pEnemy->pos.y - ENEMY_SIZE && pMissile->pos.y < pEnemy->pos.y + ENEMY_SIZE)
							{
								pEnemy = GetEnemy(); //ポインタ最初に戻す
								for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++) //範囲攻撃のため再検索
								{
									if (pEnemy->bUse == true)
									{
										if (abs(pEnemy->pos.x - pMissile->pos.x) < abs(ENEMY_SIZE + MISSILE_EXPLOSIVE - 25) &&
											abs(pEnemy->pos.y - pMissile->pos.y) < abs(ENEMY_SIZE + MISSILE_EXPLOSIVE - 25))
										{
											//爆発の設定
											SetExplosion(g_aMissile[nCntMissile].pos, MISSILE_EXPLOSIVE);

											//爆発音再生
											PlaySound(SOUND_LABEL_SE_EXPLOSION);

											//ヒット判定
											HitEnemy(nCntEnemy, 5);

											//弾の破棄
											pMissile->bUse = false;
										}
									}
								}
							}
						}
					}
				}

				//敵が弾を発射した場合
				if (g_aMissile[nCntMissile].state == MISSILESTATE_OWNER_ENEMY)
				{
					if (pPlayer->bUse == true)
					{
						if (pMissile->pos.x > pPlayer->pos.x - 40.0f && pMissile->pos.x < pPlayer->pos.x + 60.0f &&
							pMissile->pos.y > pPlayer->pos.y - 40.0f && pMissile->pos.y < pPlayer->pos.y + 60.0f)
						{
							//ヒット判定
							HitPlayer(5);

							//弾の破棄
							pMissile->bUse = false;
						}
					}
				}

				//寿命チェック
				g_aMissile[nCntMissile].nLife--;
				if (g_aMissile[nCntMissile].nLife < 0)
				{
					pEnemy = GetEnemy(); //ポインタ最初に戻す
					for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++) //範囲攻撃のため再検索
					{
						if (pEnemy->bUse == true)
						{
							if (abs(pEnemy->pos.x - pMissile->pos.x) < abs(ENEMY_SIZE + MISSILE_EXPLOSIVE) &&
								abs(pEnemy->pos.y - pMissile->pos.y) < abs(ENEMY_SIZE + MISSILE_EXPLOSIVE))
							{
								//爆発の設定
								SetExplosion(g_aMissile[nCntMissile].pos, MISSILE_EXPLOSIVE);

								//爆発音再生
								PlaySound(SOUND_LABEL_SE_EXPLOSION);

								//ヒット判定
								HitEnemy(nCntEnemy, 5);

								//弾の破棄
								pMissile->bUse = false;
							}
						}
					}
					SetExplosion(g_aMissile[nCntMissile].pos, MISSILE_EXPLOSIVE);
					g_aMissile[nCntMissile].bUse = false;
				}
				//画面外チェック
			}
			pVertex += 4; //頂点のポインタをずらす
		}

		//頂点バッファのアンロック
		g_pVtxBuffMissile->Unlock();
	}
}

//-----------------------------------------------------------------------------
//ミサイルの描画処理
//-----------------------------------------------------------------------------
void DrawMissile(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMissile, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMissile);

	for (int nCntMissile = 0; nCntMissile < MAX_MISSILE; nCntMissile++)
	{
		if (g_aMissile[nCntMissile].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntMissile * 4, 2);
		}
	}
}

//-----------------------------------------------------------------------------
//ミサイルの設定
//-----------------------------------------------------------------------------
void SetMissile(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, MISSILESTATE state)
{
	MISSILE *pMissile;
	VERTEX_2D *pVertex;

	pMissile = &g_aMissile[0];

	//頂点バッファのロック
	g_pVtxBuffMissile->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntMissile = 0; nCntMissile < MAX_MISSILE; nCntMissile++, pMissile++)
	{
		if (pMissile->bUse == false)
		{
			//位置を設定
			g_aMissile[nCntMissile].pos = pos;

			//頂点座標を設定
			pVertex[0].pos = D3DXVECTOR3(-MISSILE_WIDTH + g_aMissile[nCntMissile].pos.x, MISSILE_HEIGHT + g_aMissile[nCntMissile].pos.y, 0.0f);
			pVertex[1].pos = D3DXVECTOR3(-MISSILE_WIDTH + g_aMissile[nCntMissile].pos.x, -MISSILE_HEIGHT + g_aMissile[nCntMissile].pos.y, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(MISSILE_WIDTH + g_aMissile[nCntMissile].pos.x, MISSILE_HEIGHT + g_aMissile[nCntMissile].pos.y, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(MISSILE_WIDTH + g_aMissile[nCntMissile].pos.x, -MISSILE_HEIGHT + g_aMissile[nCntMissile].pos.y, 0.0f);

			//移動量を設定
			g_aMissile[nCntMissile].move = move;

			//寿命を設定
			g_aMissile[nCntMissile].nLife = nLife;

			//ステートを設定
			g_aMissile[nCntMissile].state = state;

			pMissile->bUse = true;
			break;
		}
		//頂点のポインタをずらす
		pVertex += 4;
	}
	g_pVtxBuffMissile->Unlock();
}