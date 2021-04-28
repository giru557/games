//-----------------------------------------------------------------------------
//
// 敵の処理 [enemy.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "enemy.h"
#include "bullet.h"
#include "player.h"
#include "money.h"
#include "fade.h"
#include "complete.h"
#include <time.h>

//-----------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_apTextureEnemy[MAX_TYPE_ENEMY] = { NULL, NULL, NULL }; //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;					//頂点バッファへのポインタ
ENEMY g_aEnemy[MAX_ENEMY];										//敵の情報
int g_nEnemyAmount;												//敵の数
int g_nEnemyAmountGroup[2];										//グループごとの敵の数
int g_aIntervalCount[MAX_ENEMY];								//敵の攻撃インターバル
int g_aCounterEnemy[MAX_ENEMY];									//敵のアニメーションカウンター
int g_aPatternEnemy[MAX_ENEMY];									//パターン
int g_aTypeEnemy[MAX_TYPE_ENEMY];								//敵のタイプ
bool g_bActiveWave;												//ウェーブの有効化
bool g_bFlag;

//-----------------------------------------------------------------------------
//プロトタイプ宣言
void EnemyMove(int nIdx, float fEnemySpeed, int nLeftorRight);
void GroupMove(ENEMYGROUP group, int nLeftorRight, float fGroupSpeed);

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//敵の初期化処理
//-----------------------------------------------------------------------------
HRESULT InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;
	
	srand((unsigned int)time(0));

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\enemy-big.png", &g_apTextureEnemy[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\enemy-medium.png", &g_apTextureEnemy[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\enemy-small.png", &g_apTextureEnemy[2]);

	//ENEMYの初期化
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].nType = ENEMYTYPE_BIG;
		g_aEnemy[nCntEnemy].bUse = false;
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
		g_aEnemy[nCntEnemy].nCounterState = 0;
		g_aEnemy[nCntEnemy].nLife = 10;
		g_aEnemy[nCntEnemy].nAtkInterval = rand() % 1200 + 200;
	}
	g_nEnemyAmount = 0;
	g_nEnemyAmountGroup[0] = 0;
	g_bActiveWave = false;
	g_bFlag = false;

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * MAX_ENEMY, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffEnemy, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
		pVertex[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		 //TRIANGLESTRIPで四角
		pVertex[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVertex[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVertex[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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

	//頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();

	//敵生成
	for (int nCntEnemy = 1; nCntEnemy <= 10; nCntEnemy++)
	{
		SetEnemy(D3DXVECTOR3((nCntEnemy * 100.0f) + 85, 50, 0.0f), D3DXVECTOR3(sinf((-D3DX_PI / 2) * ENEMY_SPEED), 0.0f, 0.0f), ENEMYTYPE_BIG, ENEMYGROUP_A);
		SetEnemy(D3DXVECTOR3((nCntEnemy * 100.0f) + 85, 150, 0.0f), D3DXVECTOR3(sinf((D3DX_PI / 2) * ENEMY_SPEED), 0.0f, 0.0f), ENEMYTYPE_MEDIUM, ENEMYGROUP_B);
		SetEnemy(D3DXVECTOR3((nCntEnemy * 100.0f) + 85, 250.0f, 0.0f), D3DXVECTOR3(sinf((-D3DX_PI / 2) * ENEMY_SPEED), 0.0f, 0.0f), ENEMYTYPE_SMALL, ENEMYGROUP_C);
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
//敵の終了処理
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
	for (int nCntRelease = 0; nCntRelease < 3; nCntRelease++)
	{
		if (g_apTextureEnemy[nCntRelease] != NULL)
		{
			g_apTextureEnemy[nCntRelease]->Release();
			g_apTextureEnemy[nCntRelease] = NULL;
		}
	}
	
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		g_aEnemy[nCnt].bUse = false;
	}
}

//-----------------------------------------------------------------------------
//敵の更新処理
//-----------------------------------------------------------------------------
void UpdateEnemy(void)
{
	ENEMY *pEnemy;
	VERTEX_2D *pVertex;
	PLAYER *pPlayer;
	static int nWait = 0;
	
	pPlayer = GetPlayer();
	pEnemy = &g_aEnemy[0];

	if (g_pVtxBuffEnemy != NULL)
	{
		//頂点バッファのロック
		g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVertex, 0);

		for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
		{
			if (pEnemy->bUse == true)
			{
				g_aCounterEnemy[nCntEnemy]++;

				if (g_aCounterEnemy[nCntEnemy] % 10 == 0)
				{
					if (g_aPatternEnemy[nCntEnemy] == 0)
					{
						g_aPatternEnemy[nCntEnemy] = 1;
					}
					else if (g_aPatternEnemy[nCntEnemy] == 1)
					{
						g_aPatternEnemy[nCntEnemy] = 0;
					}
				}

				if (g_aPatternEnemy[nCntEnemy] == 0)
				{
					//アニメーション
					pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVertex[2].tex = D3DXVECTOR2(1.0f / 2, 1.0f);
					pVertex[3].tex = D3DXVECTOR2(1.0f / 2, 0.0f);
				}
				else
				{
					//アニメーション
					pVertex[0].tex = D3DXVECTOR2(0.5f, 1.0f);
					pVertex[1].tex = D3DXVECTOR2(0.5f, 0.0f);
					pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f);
					pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f);
				}

				//頂点座標の設定
				switch (g_aEnemy[nCntEnemy].nType)
				{
				case ENEMYTYPE_BIG:
					pVertex[0].pos = D3DXVECTOR3(-ENEMY_BIG_WIDTH + g_aEnemy[nCntEnemy].pos.x, ENEMY_BIG_HEIGHT + g_aEnemy[nCntEnemy].pos.y, 0.0f);
					pVertex[1].pos = D3DXVECTOR3(-ENEMY_BIG_WIDTH + g_aEnemy[nCntEnemy].pos.x, -ENEMY_BIG_HEIGHT + g_aEnemy[nCntEnemy].pos.y, 0.0f);
					pVertex[2].pos = D3DXVECTOR3(ENEMY_BIG_WIDTH + g_aEnemy[nCntEnemy].pos.x, ENEMY_BIG_HEIGHT + g_aEnemy[nCntEnemy].pos.y, 0.0f);
					pVertex[3].pos = D3DXVECTOR3(ENEMY_BIG_WIDTH + g_aEnemy[nCntEnemy].pos.x, -ENEMY_BIG_HEIGHT + g_aEnemy[nCntEnemy].pos.y, 0.0f);
					break;

				case ENEMYTYPE_MEDIUM:
					pVertex[0].pos = D3DXVECTOR3(-ENEMY_MEDIUM_WIDTH + g_aEnemy[nCntEnemy].pos.x, ENEMY_MEDIUM_HEIGHT + g_aEnemy[nCntEnemy].pos.y, 0.0f);
					pVertex[1].pos = D3DXVECTOR3(-ENEMY_MEDIUM_WIDTH + g_aEnemy[nCntEnemy].pos.x, -ENEMY_MEDIUM_HEIGHT + g_aEnemy[nCntEnemy].pos.y, 0.0f);
					pVertex[2].pos = D3DXVECTOR3(ENEMY_MEDIUM_WIDTH + g_aEnemy[nCntEnemy].pos.x, ENEMY_MEDIUM_HEIGHT + g_aEnemy[nCntEnemy].pos.y, 0.0f);
					pVertex[3].pos = D3DXVECTOR3(ENEMY_MEDIUM_WIDTH + g_aEnemy[nCntEnemy].pos.x, -ENEMY_MEDIUM_HEIGHT + g_aEnemy[nCntEnemy].pos.y, 0.0f);
					break;

				case ENEMYTYPE_SMALL:
					pVertex[0].pos = D3DXVECTOR3(-ENEMY_SMALL_WIDTH + g_aEnemy[nCntEnemy].pos.x, ENEMY_SMALL_HEIGHT + g_aEnemy[nCntEnemy].pos.y, 0.0f);
					pVertex[1].pos = D3DXVECTOR3(-ENEMY_SMALL_WIDTH + g_aEnemy[nCntEnemy].pos.x, -ENEMY_SMALL_HEIGHT + g_aEnemy[nCntEnemy].pos.y, 0.0f);
					pVertex[2].pos = D3DXVECTOR3(ENEMY_SMALL_WIDTH + g_aEnemy[nCntEnemy].pos.x, ENEMY_SMALL_HEIGHT + g_aEnemy[nCntEnemy].pos.y, 0.0f);
					pVertex[3].pos = D3DXVECTOR3(ENEMY_SMALL_WIDTH + g_aEnemy[nCntEnemy].pos.x, -ENEMY_SMALL_HEIGHT + g_aEnemy[nCntEnemy].pos.y, 0.0f);
					break;

				default:
					break;
				}

				if (pPlayer->state != PLAYERSTATE_WAIT)
				{
					g_aIntervalCount[nCntEnemy]++;

					//敵の位置更新
					g_aEnemy[nCntEnemy].pos.x += g_aEnemy[nCntEnemy].move.x;
					g_aEnemy[nCntEnemy].pos.y += g_aEnemy[nCntEnemy].move.y;

					//画面外判定
					if (g_aEnemy[nCntEnemy].pos.x < 0 - ENEMY_SIZE || g_aEnemy[nCntEnemy].pos.x > 1280 + ENEMY_SIZE)
					{
						g_aEnemy[nCntEnemy].bUse = false;
						g_nEnemyAmount--;
					}
					else if (g_aEnemy[nCntEnemy].pos.y > 720 + ENEMY_SIZE)
					{
						g_aEnemy[nCntEnemy].bUse = false;
						g_nEnemyAmount--;
					}

					//ステート
					switch (g_aEnemy[nCntEnemy].state)
					{
						//ダメージを受けた場合
					case ENEMYSTATE_DAMAGE:
						g_aEnemy[nCntEnemy].nCounterState--;
						if (g_aEnemy[nCntEnemy].nCounterState <= 0)
						{
							g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

							//ポリゴンの色を戻す
							pVertex[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);  //D3DCOLOR_RGBA(赤, 緑, 青, 透明度);
							pVertex[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
							pVertex[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
							pVertex[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						}
						break;
					}

					//グループごとの処理
					switch (g_aEnemy[nCntEnemy].group)
					{
					case ENEMYGROUP_A:
						if (g_aEnemy[nCntEnemy].pos.x - ENEMY_BIG_WIDTH < 0)
						{
							GroupMove(ENEMYGROUP_A, 1, ENEMY_SPEED);
						}
						else if (g_aEnemy[nCntEnemy].pos.x + ENEMY_BIG_WIDTH > SCREEN_WIDTH)
						{
							GroupMove(ENEMYGROUP_A, 0, ENEMY_SPEED);
						}
						break;

					case ENEMYGROUP_B:
						if (g_aEnemy[nCntEnemy].pos.x - ENEMY_MEDIUM_WIDTH < 0)
						{
							GroupMove(ENEMYGROUP_B, 1, ENEMY_SPEED);
						}
						else if (g_aEnemy[nCntEnemy].pos.x + ENEMY_MEDIUM_WIDTH > SCREEN_WIDTH)
						{
							GroupMove(ENEMYGROUP_B, 0, ENEMY_SPEED);
						}
						break;

					case ENEMYGROUP_C:
						if (g_aEnemy[nCntEnemy].pos.x - ENEMY_SMALL_WIDTH < 0)
						{
							GroupMove(ENEMYGROUP_C, 1, ENEMY_SPEED);
						}
						else if (g_aEnemy[nCntEnemy].pos.x + ENEMY_SMALL_WIDTH > SCREEN_WIDTH)
						{
							GroupMove(ENEMYGROUP_C, 0, ENEMY_SPEED);
						}
						break;

					case ENEMYGROUP_D:
						if (g_bFlag == true)
						{
							if (g_aEnemy[nCntEnemy].pos.y > 50)
							{
								GroupMove(ENEMYGROUP_D, 0, ENEMY_SPEED);
							}
						}
						if (g_aEnemy[nCntEnemy].pos.x - ENEMY_BIG_WIDTH < 0)
						{
							GroupMove(ENEMYGROUP_D, 1, ENEMY_SPEED);
						}
						else if (g_aEnemy[nCntEnemy].pos.x + ENEMY_BIG_WIDTH > SCREEN_WIDTH)
						{
							GroupMove(ENEMYGROUP_D, 0, ENEMY_SPEED);
						}
						break;

					case ENEMYGROUP_E:
						if (g_bFlag == true)
						{
							if (g_aEnemy[nCntEnemy].pos.y > 150)
							{
								GroupMove(ENEMYGROUP_E, 0, ENEMY_SPEED);
							}
						}
						if (g_aEnemy[nCntEnemy].pos.x - ENEMY_BIG_WIDTH < 0)
						{
							GroupMove(ENEMYGROUP_E, 1, ENEMY_SPEED);
						}
						else if (g_aEnemy[nCntEnemy].pos.x + ENEMY_BIG_WIDTH > SCREEN_WIDTH)
						{
							GroupMove(ENEMYGROUP_E, 0, ENEMY_SPEED);
						}
						break;

					case ENEMYGROUP_F:
						if (g_bFlag == true)
						{
							if (g_aEnemy[nCntEnemy].pos.y > 250)
							{
								GroupMove(ENEMYGROUP_F, 0, ENEMY_SPEED);
								g_bFlag = false;
							}
						}
						if (g_aEnemy[nCntEnemy].pos.x - ENEMY_MEDIUM_WIDTH < 0)
						{
							GroupMove(ENEMYGROUP_F, 1, ENEMY_SPEED);
						}
						else if (g_aEnemy[nCntEnemy].pos.x + ENEMY_MEDIUM_WIDTH > SCREEN_WIDTH)
						{
							GroupMove(ENEMYGROUP_F, 0, ENEMY_SPEED);
						}
						break;

					default:
						break;
					}

					if (g_aEnemy[nCntEnemy].pos.y > 0)
					{
						if (g_aEnemy[nCntEnemy].group == ENEMYGROUP_D ||
							g_aEnemy[nCntEnemy].group == ENEMYGROUP_E ||
							g_aEnemy[nCntEnemy].group == ENEMYGROUP_F)
						{
							g_aEnemy[nCntEnemy].nAtkInterval = 200;
						}
						//敵の攻撃間隔
						if (g_aIntervalCount[nCntEnemy] % g_aEnemy[nCntEnemy].nAtkInterval == 0)
						{
							if (g_aIntervalCount[nCntEnemy] > g_aEnemy[nCntEnemy].nAtkInterval)
							{
								g_aIntervalCount[nCntEnemy] = 0;
							}
							D3DXVECTOR3 test;
							test.x = sinf(atan2f(pPlayer->pos.x - pEnemy->pos.x, pPlayer->pos.y - pEnemy->pos.y));
							test.y = cosf(atan2f(pPlayer->pos.x - pEnemy->pos.x, pPlayer->pos.y - pEnemy->pos.y));

							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + 30, 0.0f), D3DXVECTOR3(test.x * 7, test.y * 7, 0.0f), 500, BULLETSTATE_OWNER_ENEMY, BULLETTYPE_NORMAL);
						}
					}
				}
			}

			pVertex += 4; //頂点のポインタをずらす
		}
		//頂点バッファのアンロック
		g_pVtxBuffEnemy->Unlock();

		//敵の数チェック
		if (g_nEnemyAmount <= 0)
		{
			if (g_bActiveWave == true)
			{
				nWait++;

				ShowStageComplete();

				if (nWait > 180)
				{
					SetFade(FADE_OUT, MODE_RESULT);
				}
			}
			else
			{
				for (int nCntEnemy = 1; nCntEnemy <= 10; nCntEnemy++)
				{
					/*SetEnemy(D3DXVECTOR3((nCntEnemy * 100.0f) + 95, 50, 0.0f), D3DXVECTOR3(sinf((-D3DX_PI / 2) * ENEMY_SPEED), 0.0f, 0.0f), ENEMYTYPE_BIG, ENEMYGROUP_D);
					SetEnemy(D3DXVECTOR3((nCntEnemy * 100.0f) + 95, 150, 0.0f), D3DXVECTOR3(sinf((D3DX_PI / 2) * ENEMY_SPEED), 0.0f, 0.0f), ENEMYTYPE_MEDIUM, ENEMYGROUP_E);
					SetEnemy(D3DXVECTOR3((nCntEnemy * 100.0f) + 95, 250.0f, 0.0f), D3DXVECTOR3(sinf((-D3DX_PI / 2) * ENEMY_SPEED), 0.0f, 0.0f), ENEMYTYPE_SMALL, ENEMYGROUP_F);*/
					SetEnemy(D3DXVECTOR3((nCntEnemy * 100.0f) + 85, -750, 0.0f), D3DXVECTOR3(0.0f, cosf(0) * 3, 0.0f), ENEMYTYPE_BIG, ENEMYGROUP_D);
					SetEnemy(D3DXVECTOR3((nCntEnemy * 100.0f) + 85, -650, 0.0f), D3DXVECTOR3(0.0f, cosf(0) * 3, 0.0f), ENEMYTYPE_BIG, ENEMYGROUP_E);
					SetEnemy(D3DXVECTOR3((nCntEnemy * 100.0f) + 85, -550.0f, 0.0f), D3DXVECTOR3(0.0f, cosf(0) * 3, 0.0f), ENEMYTYPE_MEDIUM, ENEMYGROUP_F);
				}
				g_bFlag = true;
				g_bActiveWave = true;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//敵の描画処理
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

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			switch (g_aEnemy[nCntEnemy].nType)
			{
			case ENEMYTYPE_BIG:
				//テクスチャの設定
				pDevice->SetTexture(0, g_apTextureEnemy[0]);
				break;

			case ENEMYTYPE_MEDIUM:
				//テクスチャの設定
				pDevice->SetTexture(0, g_apTextureEnemy[1]);
				break;

			case ENEMYTYPE_SMALL:
				//テクスチャの設定
				pDevice->SetTexture(0, g_apTextureEnemy[2]);
				break;

			default:
				break;
			}

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, 2);
		}
	}
}

//-----------------------------------------------------------------------------
//敵の設定
//-----------------------------------------------------------------------------
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, ENEMYTYPE nType, ENEMYGROUP group)
{
	ENEMY *pEnemy;
	VERTEX_2D *pVertex;

	g_nEnemyAmount++;

	pEnemy = &g_aEnemy[0];

	//頂点バッファのロック
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == false)
		{
			//位置を設定
			g_aEnemy[nCntEnemy].pos = pos;

			//敵の移動
			g_aEnemy[nCntEnemy].move = move;

			//種類
			g_aEnemy[nCntEnemy].nType = nType;

			//グループ
			g_aEnemy[nCntEnemy].group = group;

			//体力設定
			switch (g_aEnemy[nCntEnemy].nType)
			{
			case ENEMYTYPE_BIG:
				g_aEnemy[nCntEnemy].nLife = 15;
				break;

			case ENEMYTYPE_MEDIUM:
				g_aEnemy[nCntEnemy].nLife = 10;
				break;

			case ENEMYTYPE_SMALL:
				g_aEnemy[nCntEnemy].nLife = 5;
				break;

			default:
				break;
			}

			//頂点座標を設定
			pVertex[0].pos = D3DXVECTOR3(-ENEMY_SIZE + g_aEnemy[nCntEnemy].pos.x, ENEMY_SIZE + g_aEnemy[nCntEnemy].pos.y, 0.0f);
			pVertex[1].pos = D3DXVECTOR3(-ENEMY_SIZE + g_aEnemy[nCntEnemy].pos.x, -ENEMY_SIZE + g_aEnemy[nCntEnemy].pos.y, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(ENEMY_SIZE + g_aEnemy[nCntEnemy].pos.x, ENEMY_SIZE + g_aEnemy[nCntEnemy].pos.y, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(ENEMY_SIZE + g_aEnemy[nCntEnemy].pos.x, -ENEMY_SIZE + g_aEnemy[nCntEnemy].pos.y, 0.0f);

			pEnemy->bUse = true;
			break;
		}
		pVertex += 4; //頂点のポインタをずらす
	}
	g_pVtxBuffEnemy->Unlock();
}

ENEMY *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//-----------------------------------------------------------------------------
//敵に当たった処理
//-----------------------------------------------------------------------------
bool HitEnemy(int nIdx, int nDamage)
{
	VERTEX_2D *pVertex;
	g_aEnemy[nIdx].nLife -= nDamage;

	//頂点バッファのロック
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVertex, 0);

	pVertex += 4 * nIdx;

	if (g_aEnemy[nIdx].nLife <= 0)
	{//敵が死んだ

		switch (g_aEnemy[nIdx].nType)
		{
		case ENEMYTYPE_BIG:
			//爆発アニメーション
			SetExplosion(g_aEnemy[nIdx].pos, ENEMY_BIG_WIDTH + 20);
			//スコア追加
			AddScore(350);
			//お金追加
			AddMoney(5);
			break;

		case ENEMYTYPE_MEDIUM:
			//爆発アニメーション
			SetExplosion(g_aEnemy[nIdx].pos, ENEMY_MEDIUM_WIDTH + 20);
			//スコア追加
			AddScore(150);
			//お金追加
			AddMoney(2);
			break;

		case ENEMYTYPE_SMALL:
			//爆発アニメーション
			SetExplosion(g_aEnemy[nIdx].pos, ENEMY_SMALL_WIDTH + 20);
			//スコア追加
			AddScore(75);
			//お金追加
			AddMoney(1);
			break;

		default:
			break;
		}

    	g_nEnemyAmount--;

		g_aEnemy[nIdx].bUse = false;

		//サウンドの再生
		PlaySound(SOUND_LABEL_SE_EXPLOSION);

		return true;
	}
	else
	{//敵がまだ生きてる

		//ヒット音再生
		PlaySound(SOUND_LABEL_SE_HIT2);

		g_aEnemy[nIdx].state = ENEMYSTATE_DAMAGE;
		g_aEnemy[nIdx].nCounterState = 10;

		//ポリゴンの色を変更
		pVertex[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);  //D3DCOLOR_RGBA(赤, 緑, 青, 透明度);
		pVertex[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVertex[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVertex[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

		return false;
	}
	g_pVtxBuffEnemy->Unlock();
}

//-----------------------------------------------------------------------------
//敵 左右移動
//-----------------------------------------------------------------------------
void EnemyMove(int nIdx, float fEnemySpeed, int nLeftorRight)
{
	switch (nLeftorRight)
	{
	case 0:
		g_aEnemy[nIdx].move = D3DXVECTOR3(sinf(-D3DX_PI / 2) * fEnemySpeed, 0.0f, 0.0f);
		break;

	case 1:
		g_aEnemy[nIdx].move = D3DXVECTOR3(sinf(D3DX_PI / 2) * fEnemySpeed, 0.0f, 0.0f);
		break;

	default:
		break;
	}
}

//敵グループ移動
void GroupMove(ENEMYGROUP group, int nLeftorRight, float fGroupSpeed)
{
	if (nLeftorRight == 0)
	{
		for (int nCntGroup = 0; nCntGroup < MAX_ENEMY; nCntGroup++)
		{//敵全体検索して
			if (g_aEnemy[nCntGroup].bUse == true)
			{//使用中の奴だけに絞って
				if (g_aEnemy[nCntGroup].group == group)
				{//グループも絞って
					EnemyMove(nCntGroup, fGroupSpeed, 0); //敵移動 右
				}
			}
		}
	}
	else if (nLeftorRight == 1)
	{
		for (int nCntGroup = 0; nCntGroup < MAX_ENEMY; nCntGroup++)
		{
			if (g_aEnemy[nCntGroup].bUse == true)
			{
				if (g_aEnemy[nCntGroup].group == group)
				{
					EnemyMove(nCntGroup, fGroupSpeed, 1); //敵移動 左
				}
			}
		}
	}
}