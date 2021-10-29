//-----------------------------------------------------------------------------
//
// 敵の処理 [enemy.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "enemy.h"
#include "fade.h"
#include "player.h"
#include "timescore.h"
#include "block.h"
#include "sound.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffEnemy = NULL;					// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureEnemy = NULL;				// ポリゴンのテクスチャへのポインタ
ENEMY g_aEnemy[ENEMY_POLYGON];										// ENEMY構造体の情報

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
void EnemyMove(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight);

//-----------------------------------------------------------------------------
// 敵の初期化処理
//-----------------------------------------------------------------------------
HRESULT InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	//Enemy初期化
	for (int nCntENEM = 0; nCntENEM < ENEMY_POLYGON; nCntENEM++)
	{
		g_aEnemy[nCntENEM].bUse = false;
		g_aEnemy[nCntENEM].bJump = false;
		g_aEnemy[nCntENEM].fHeight = ENEMY_HEIGHT;
		g_aEnemy[nCntENEM].fWidth = ENEMY_WIDTH;
		g_aEnemy[nCntENEM].pos = D3DXVECTOR3(0, 0, 0.0f);
		g_aEnemy[nCntENEM].posold = D3DXVECTOR3(0, 0, 0);
		g_aEnemy[nCntENEM].scroll = D3DXVECTOR3(0, 0, 0.0f);
		g_aEnemy[nCntENEM].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntENEM].nPatternH = 0;
		g_aEnemy[nCntENEM].nPatternV = 0;
		g_aEnemy[nCntENEM].nMaxPatternH = ENEMY_TEX_SPLIT_H;
		g_aEnemy[nCntENEM].nMaxPatternV = ENEMY_TEX_SPLIT_V;
	}

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\golem1_sprite.png", &g_pTextureEnemy);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * ENEMY_POLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffEnemy, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntENEM = 0; nCntENEM < ENEMY_POLYGON; nCntENEM++)
	{
		//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
		pVertex[0].pos = D3DXVECTOR3(g_aEnemy[nCntENEM].pos.x - ENEMY_WIDTH, g_aEnemy[nCntENEM].pos.y, 0.0f);
		pVertex[1].pos = D3DXVECTOR3(g_aEnemy[nCntENEM].pos.x - ENEMY_WIDTH, g_aEnemy[nCntENEM].pos.y - ENEMY_HEIGHT, 0.0f);
		pVertex[2].pos = D3DXVECTOR3(g_aEnemy[nCntENEM].pos.x + ENEMY_WIDTH, g_aEnemy[nCntENEM].pos.y, 0.0f);
		pVertex[3].pos = D3DXVECTOR3(g_aEnemy[nCntENEM].pos.x + ENEMY_WIDTH, g_aEnemy[nCntENEM].pos.y - ENEMY_HEIGHT, 0.0f);

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
		pVertex[0].tex = D3DXVECTOR2((1.0f / (float)g_aEnemy[nCntENEM].nMaxPatternH) * (g_aEnemy[nCntENEM].nPatternH), (1.0f / g_aEnemy[nCntENEM].nMaxPatternV) * (g_aEnemy[nCntENEM].nPatternV + 1.0f));
		pVertex[1].tex = D3DXVECTOR2((1.0f / (float)g_aEnemy[nCntENEM].nMaxPatternH) * (g_aEnemy[nCntENEM].nPatternH), (1.0f / g_aEnemy[nCntENEM].nMaxPatternV) * (g_aEnemy[nCntENEM].nPatternV));
		pVertex[2].tex = D3DXVECTOR2((1.0f / (float)g_aEnemy[nCntENEM].nMaxPatternH) * (g_aEnemy[nCntENEM].nPatternH + 1.0f), (1.0f / g_aEnemy[nCntENEM].nMaxPatternV) * (g_aEnemy[nCntENEM].nPatternV + 1.0f));
		pVertex[3].tex = D3DXVECTOR2((1.0f / (float)g_aEnemy[nCntENEM].nMaxPatternH) * (g_aEnemy[nCntENEM].nPatternH + 1.0f), (1.0f / g_aEnemy[nCntENEM].nMaxPatternV) * (g_aEnemy[nCntENEM].nPatternV));

		pVertex += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
// 敵の終了処理
//-----------------------------------------------------------------------------
void UninitEnemy(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}


	//テクスチャの開放
	if (g_pTextureEnemy != NULL)
	{
		g_pTextureEnemy->Release();
		g_pTextureEnemy = NULL;
	}

}

//-----------------------------------------------------------------------------
// 敵の更新処理
//-----------------------------------------------------------------------------
void UpdateEnemy(void)
{
	VERTEX_2D *pVertex;
	PLAYER *pPlayer;
	ENEMY *pEnemy;

	pPlayer = GetPlayer();
	pEnemy = &g_aEnemy[0];

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntENEM = 0; nCntENEM < ENEMY_POLYGON; nCntENEM++, pEnemy++)
	{

		if (pEnemy->bUse == true)
		{
			//アニメーションカウントアップ
			pEnemy->nCounterAnim++;

			//前回位置保存
			pEnemy->posold = pEnemy->pos;

			//敵の移動
			EnemyMove(&g_aEnemy[nCntENEM].pos, &g_aEnemy[nCntENEM].posold, &g_aEnemy[nCntENEM].move, g_aEnemy[nCntENEM].fWidth, g_aEnemy[nCntENEM].fHeight);

			//重力
			pEnemy->move.y += PLAYER_GRAVITY;

			//スクロール量
			pEnemy->scroll.x = -pPlayer->move.x;

			//移動値追加
			pEnemy->pos.x += pEnemy->move.x;
			pEnemy->pos.y += pEnemy->move.y;

			////敵のスクロール
			//if (pPlayer->scrollState == PLAYERSCROLL_CENTER)
			//{
			//	pEnemy->pos.x += pEnemy->scroll.x;
			//	pEnemy->pos.y += pEnemy->scroll.y;
			//}
		
			//テクスチャのパターンをアニメーションさせる
			if (pEnemy->nCounterAnim % PLAYER_TEX_ANIMINTERVAL == 0)
			{
				pEnemy->nPatternH = (pEnemy->nPatternH + 1) % ENEMY_TEX_SPLIT_H;
				pEnemy->nCounterAnim = 0;
			}

			//ブロックとの当たり判定
			pEnemy->bJump = CollisionBlock(&g_aEnemy[nCntENEM].pos, &g_aEnemy[nCntENEM].posold, &g_aEnemy[nCntENEM].move, ENEMY_WIDTH, ENEMY_HEIGHT);

			//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(pEnemy->pos.x - ENEMY_WIDTH, pEnemy->pos.y, 0.0f);
			pVertex[1].pos = D3DXVECTOR3(pEnemy->pos.x - ENEMY_WIDTH, pEnemy->pos.y - ENEMY_HEIGHT, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(pEnemy->pos.x + ENEMY_WIDTH, pEnemy->pos.y, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(pEnemy->pos.x + ENEMY_WIDTH, pEnemy->pos.y - ENEMY_HEIGHT, 0.0f);

			//頂点情報の設定
			if (pEnemy->move.x > 0)
			{
				pVertex[0].tex = D3DXVECTOR2((1.0f / (float)g_aEnemy[nCntENEM].nMaxPatternH) * (g_aEnemy[nCntENEM].nPatternH), (1.0f / g_aEnemy[nCntENEM].nMaxPatternV) * (g_aEnemy[nCntENEM].nPatternV + 1.0f));
				pVertex[1].tex = D3DXVECTOR2((1.0f / (float)g_aEnemy[nCntENEM].nMaxPatternH) * (g_aEnemy[nCntENEM].nPatternH), (1.0f / g_aEnemy[nCntENEM].nMaxPatternV) * (g_aEnemy[nCntENEM].nPatternV));
				pVertex[2].tex = D3DXVECTOR2((1.0f / (float)g_aEnemy[nCntENEM].nMaxPatternH) * (g_aEnemy[nCntENEM].nPatternH + 1.0f), (1.0f / g_aEnemy[nCntENEM].nMaxPatternV) * (g_aEnemy[nCntENEM].nPatternV + 1.0f));
				pVertex[3].tex = D3DXVECTOR2((1.0f / (float)g_aEnemy[nCntENEM].nMaxPatternH) * (g_aEnemy[nCntENEM].nPatternH + 1.0f), (1.0f / g_aEnemy[nCntENEM].nMaxPatternV) * (g_aEnemy[nCntENEM].nPatternV));
			}
			else if (pEnemy->move.x < 0)
			{
				pVertex[0].tex = D3DXVECTOR2((1.0f / (float)g_aEnemy[nCntENEM].nMaxPatternH) * (g_aEnemy[nCntENEM].nPatternH + 1.0f), (1.0f / g_aEnemy[nCntENEM].nMaxPatternV) * (g_aEnemy[nCntENEM].nPatternV + 1.0f));
				pVertex[1].tex = D3DXVECTOR2((1.0f / (float)g_aEnemy[nCntENEM].nMaxPatternH) * (g_aEnemy[nCntENEM].nPatternH + 1.0f), (1.0f / g_aEnemy[nCntENEM].nMaxPatternV) * (g_aEnemy[nCntENEM].nPatternV));
				pVertex[2].tex = D3DXVECTOR2((1.0f / (float)g_aEnemy[nCntENEM].nMaxPatternH) * (g_aEnemy[nCntENEM].nPatternH), (1.0f / g_aEnemy[nCntENEM].nMaxPatternV) * (g_aEnemy[nCntENEM].nPatternV + 1.0f));
				pVertex[3].tex = D3DXVECTOR2((1.0f / (float)g_aEnemy[nCntENEM].nMaxPatternH) * (g_aEnemy[nCntENEM].nPatternH), (1.0f / g_aEnemy[nCntENEM].nMaxPatternV) * (g_aEnemy[nCntENEM].nPatternV));

			}

		}
		pVertex += 4;
	}
			//頂点バッファをアンロックする
		g_pVtxBuffEnemy->Unlock();
}

//-----------------------------------------------------------------------------
// 敵の描画処理
//-----------------------------------------------------------------------------
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureEnemy);

	for (int nCntENEM = 0; nCntENEM < ENEMY_POLYGON; nCntENEM++)
	{
		if (g_aEnemy[nCntENEM].bUse == true)
		{
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntENEM * 4, 2);
		}
	}
}

//-----------------------------------------------------------------------------
// 敵との当たり判定
//-----------------------------------------------------------------------------
void CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	for (int nCntENEM = 0; nCntENEM < ENEMY_POLYGON; nCntENEM++)
	{
		if (g_aEnemy[nCntENEM].bUse == true)
		{
			if ((pPos->x + fWidth > g_aEnemy[nCntENEM].pos.x - g_aEnemy[nCntENEM].fWidth && pPos->x - fWidth < g_aEnemy[nCntENEM].pos.x + g_aEnemy[nCntENEM].fWidth) &&
				(pPos->y > g_aEnemy[nCntENEM].pos.y - g_aEnemy[nCntENEM].fHeight && pPos->y - fHeight < -g_aEnemy[nCntENEM].fHeight))
			{
				if (pPosOld->y <= g_aEnemy[nCntENEM].pos.y - g_aEnemy[nCntENEM].fHeight&&
					pPosOld->x + fWidth >= g_aEnemy[nCntENEM].pos.x - g_aEnemy[nCntENEM].fWidth && pPosOld->x - fWidth <= g_aEnemy[nCntENEM].pos.x + g_aEnemy[nCntENEM].fWidth)
				{
					if (pPlayer->damage == DAMAGESTATE_NORMAL)
					{
						if (pPlayer->state != PLAYERSTATE_SLIDE)
						{
							pPlayer->damage = DAMAGESTATE_HURT;
						}
						else
						{
							PlaySound(SOUND_LABEL_SE_DIE_ENEMY);
							g_aEnemy[nCntENEM].bUse = false;
						}

						if (pPlayer->pos.x > g_aEnemy[nCntENEM].pos.x)
						{
							pPlayer->move.y -= 5;
							pPlayer->move.x += 10;
						}
						else if (pPlayer->pos.x < g_aEnemy[nCntENEM].pos.x)
						{
							pPlayer->move.y -= 5;
							pPlayer->move.x -= 10;
						}
					}
				}
			}
			if ((pPos->x + fWidth > g_aEnemy[nCntENEM].pos.x - g_aEnemy[nCntENEM].fWidth && pPos->x - fWidth < g_aEnemy[nCntENEM].pos.x + g_aEnemy[nCntENEM].fWidth) &&
				(pPos->y > g_aEnemy[nCntENEM].pos.y && pPos->y - fHeight < g_aEnemy[nCntENEM].pos.y))
			{
				if (pPosOld->y - fHeight >= g_aEnemy[nCntENEM].pos.y &&
					pPosOld->x + fWidth >= g_aEnemy[nCntENEM].pos.x - g_aEnemy[nCntENEM].fWidth && pPosOld->x - fWidth <= g_aEnemy[nCntENEM].pos.x + g_aEnemy[nCntENEM].fWidth)
				{
					if (pPlayer->damage == DAMAGESTATE_NORMAL)
					{
						if (pPlayer->state != PLAYERSTATE_SLIDE)
						{
							pPlayer->damage = DAMAGESTATE_HURT;
						}
						else
						{
							PlaySound(SOUND_LABEL_SE_DIE_ENEMY);
							g_aEnemy[nCntENEM].bUse = false;
						}

						if (pPlayer->pos.x > g_aEnemy[nCntENEM].pos.x)
						{
							pPlayer->move.y -= 5;
							pPlayer->move.x += 10;
						}
						else if (pPlayer->pos.x < g_aEnemy[nCntENEM].pos.x)
						{
							pPlayer->move.y -= 5;
							pPlayer->move.x -= 10;
						}
					}
				}
			}
			if ((pPos->y > g_aEnemy[nCntENEM].pos.y - g_aEnemy[nCntENEM].fHeight && pPos->y - fHeight < g_aEnemy[nCntENEM].pos.y) &&
				(pPos->x + fWidth > g_aEnemy[nCntENEM].pos.x - g_aEnemy[nCntENEM].fWidth && pPos->x - fWidth < g_aEnemy[nCntENEM].pos.x - g_aEnemy[nCntENEM].fWidth))
			{
				if (pPosOld->x <= g_aEnemy[nCntENEM].pos.x - g_aEnemy[nCntENEM].fWidth &&
					pPosOld->y >= g_aEnemy[nCntENEM].pos.y - g_aEnemy[nCntENEM].fHeight && pPosOld->y - fHeight <= g_aEnemy[nCntENEM].pos.y)
				{
					if (pPlayer->damage == DAMAGESTATE_NORMAL)
					{
						if (pPlayer->state != PLAYERSTATE_SLIDE)
						{
							pPlayer->damage = DAMAGESTATE_HURT;
						}
						else
						{
							PlaySound(SOUND_LABEL_SE_DIE_ENEMY);
							g_aEnemy[nCntENEM].bUse = false;
						}

						if (pPlayer->pos.x > g_aEnemy[nCntENEM].pos.x)
						{
							pPlayer->move.y -= 5;
							pPlayer->move.x += 10;
						}
						else if (pPlayer->pos.x < g_aEnemy[nCntENEM].pos.x)
						{
							pPlayer->move.y -= 5;
							pPlayer->move.x -= 10;
						}
					}
				}
			}
			if ((pPos->y > g_aEnemy[nCntENEM].pos.y - g_aEnemy[nCntENEM].fHeight && pPos->y - fHeight < g_aEnemy[nCntENEM].pos.y) &&
				(pPos->x + fWidth > g_aEnemy[nCntENEM].pos.x + g_aEnemy[nCntENEM].fWidth && pPos->x - fWidth < g_aEnemy[nCntENEM].pos.x + g_aEnemy[nCntENEM].fWidth))
			{
				if (pPosOld->x + fWidth >= g_aEnemy[nCntENEM].pos.x + g_aEnemy[nCntENEM].fWidth &&
					pPosOld->y > g_aEnemy[nCntENEM].pos.y - g_aEnemy[nCntENEM].fHeight && pPosOld->y - fHeight < g_aEnemy[nCntENEM].pos.y)
				{
					if (pPlayer->damage == DAMAGESTATE_NORMAL)
					{
						if (pPlayer->state != PLAYERSTATE_SLIDE)
						{
							pPlayer->damage = DAMAGESTATE_HURT;
						}
						else
						{
							PlaySound(SOUND_LABEL_SE_DIE_ENEMY);
							g_aEnemy[nCntENEM].bUse = false;
						}

						if (pPlayer->pos.x > g_aEnemy[nCntENEM].pos.x)
						{
							pPlayer->move.y -= 5;
							pPlayer->move.x += 10;
						}
						else if (pPlayer->pos.x < g_aEnemy[nCntENEM].pos.x)
						{
							pPlayer->move.y -= 5;
							pPlayer->move.x -= 10;
						}
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 敵の情報を取得
//-----------------------------------------------------------------------------
ENEMY *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//-----------------------------------------------------------------------------
// 敵の配置
//-----------------------------------------------------------------------------
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	VERTEX_2D *pVertex;
	ENEMY *pEnemy;
	pEnemy = &g_aEnemy[0];

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVertex, 0);
	
	for (int nCntENEM = 0; nCntENEM < ENEMY_POLYGON; nCntENEM++, pEnemy++)
	{
		if (pEnemy->bUse == false)
		{
			//敵の位置設定
			pEnemy->pos = pos;

			//敵の移動値設定
			pEnemy->move = move;

			//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
			pVertex[0].pos = D3DXVECTOR3(g_aEnemy[nCntENEM].pos.x - ENEMY_WIDTH, g_aEnemy[nCntENEM].pos.y, 0.0f);
			pVertex[1].pos = D3DXVECTOR3(g_aEnemy[nCntENEM].pos.x - ENEMY_WIDTH, g_aEnemy[nCntENEM].pos.y - ENEMY_HEIGHT, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_aEnemy[nCntENEM].pos.x + ENEMY_WIDTH, g_aEnemy[nCntENEM].pos.y, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_aEnemy[nCntENEM].pos.x + ENEMY_WIDTH, g_aEnemy[nCntENEM].pos.y - ENEMY_HEIGHT, 0.0f);

			pEnemy->bUse = true;
			break;
		}
		pVertex += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
}

//-----------------------------------------------------------------------------
// 敵の移動
//-----------------------------------------------------------------------------
void EnemyMove(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{
	ENEMY *pEnemy;
	BLOCK *pBlock;
	pBlock = GetBlock();
	pEnemy = GetEnemy();

	//反転条件
		for (int nCntBLK = 0; nCntBLK < BLOCK_MAX; nCntBLK++, pBlock++)
		{
			if (pBlock->bUse == true)
			{
				if ((pPos->y > pBlock->pos.y && pPos->y - pEnemy->fHeight < pPos->y + pBlock->fHeight) &&
					(pPos->x + fWidth >= pBlock->pos.x && pPos->x - fWidth <= pBlock->pos.x))
				{
					if (pPosOld->x <= pBlock->pos.x &&
						pPosOld->y >= pBlock->pos.y && pPosOld->y - fHeight <= pBlock->pos.y + pBlock->fHeight)
					{
						pMove->x = -2.0f;
					}
				}
				if ((pPos->y > pBlock->pos.y && pPos->y - fHeight < pBlock->pos.y + pBlock->fHeight) &&
					(pPos->x + fWidth >= pBlock->pos.x + pBlock->fWidth && pPos->x - fWidth <= pBlock->pos.x + pBlock->fWidth))
				{
					if (pPosOld->x + fWidth >= pBlock->pos.x + pBlock->fWidth &&
						pPosOld->y > pBlock->pos.y && pPosOld->y - fHeight < pBlock->pos.y + pBlock->fHeight)
					{
						pMove->x = 2.0f;
					}
				}
			}
		}
}