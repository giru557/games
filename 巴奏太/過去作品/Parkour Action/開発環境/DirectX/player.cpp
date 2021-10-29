//-----------------------------------------------------------------------------
//
// プレイヤーの処理 [player.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include "player.h"
#include "input.h"
#include "goalflag.h"
#include "fade.h"
#include "enemy.h"
#include "scroll.h"
#include "sound.h"
#include <stdio.h>
#include <cmath>

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define PLAYER_TEX (3)	// テクスチャの使用枚数

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffPlayer = NULL;					// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTexturePlayer[PLAYER_TEX] = { NULL };			// ポリゴンのテクスチャへのポインタ
LPD3DXFONT g_pFontPlayer = NULL;									// フォントへのポインタ
PLAYER g_player;													// プレイヤーの情報
D3DXVECTOR3 g_startPos;
D3DXVECTOR3 g_finishPos;

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
void MovePlayer(void);
void WallJump(void);
void Scroll(void);
void TransferBlockMove();
void CharacterSlide(void);
void CharacterState(void);

//-----------------------------------------------------------------------------
// ポリゴンの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;
	PLAYER *pPlayer;

	pPlayer = &g_player;

	//プレイヤーの初期化
	pPlayer->pos = D3DXVECTOR3(PLAYER_POS_X, PLAYER_POS_Y, 0.0f);
	pPlayer->posold = pPlayer->pos;
	pPlayer->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pPlayer->fWidth = PLAYER_WIDTH;
	pPlayer->fHeight = PLAYER_HEIGHT;
	pPlayer->fSpeed = PLAYER_SPEED;
	pPlayer->nMaxPatternH = PLAYER_TEX_SPLIT_H;
	pPlayer->nMaxPatternV = PLAYER_TEX_SPLIT_V;
	pPlayer->nPatternH = 0;
	pPlayer->nPatternV = 0;
	pPlayer->nCounterAnim = 0;
	pPlayer->nLastKey = 0;
	pPlayer->nWaitSlide = 0;
	pPlayer->nWait = 0;
	pPlayer->nLife = 1;
	pPlayer->bUse = true;
	pPlayer->bJump = false;
	pPlayer->bOnWall = false;
	pPlayer->scrollState = PLAYERSCROLL_CENTER;
	pPlayer->state = PLAYERSTATE_NORMAL;
	pPlayer->damage = DAMAGESTATE_NORMAL;

	g_startPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_finishPos = D3DXVECTOR3(SCREEN_WIDTH + 1000, 0.0f, 0.0f);

	//デバイスの取得
	pDevice = GetDevice();

	//フォントの生成
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFontPlayer);

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\wizard_sprite.png", &g_pTexturePlayer[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\wizard_sliding.png", &g_pTexturePlayer[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\wizard_onwall.png", &g_pTexturePlayer[2]);
	//D3DXCreateTextureFromFileEx(pDevice, "data\\TEXTURE\\wizard_sprite.png", 6000, 900, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0xFF000000, NULL, NULL, &g_pTexturePlayer);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPlayer, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVertex, 0);

	//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
	pVertex[0].pos = D3DXVECTOR3(pPlayer->pos.x - pPlayer->fWidth, pPlayer->pos.y, 0.0f);
	pVertex[1].pos = D3DXVECTOR3(pPlayer->pos.x - pPlayer->fWidth, pPlayer->pos.y - pPlayer->fHeight, 0.0f);
	pVertex[2].pos = D3DXVECTOR3(pPlayer->pos.x + pPlayer->fWidth, pPlayer->pos.y, 0.0f);
	pVertex[3].pos = D3DXVECTOR3(pPlayer->pos.x + pPlayer->fWidth, pPlayer->pos.y - pPlayer->fHeight, 0.0f);

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
	pVertex[0].tex = D3DXVECTOR2((1.0f / (float)pPlayer->nMaxPatternH) * (pPlayer->nPatternH), (1.0f / pPlayer->nMaxPatternV) * (pPlayer->nPatternV + 1.0f));
	pVertex[1].tex = D3DXVECTOR2((1.0f / (float)pPlayer->nMaxPatternH) * (pPlayer->nPatternH), (1.0f / pPlayer->nMaxPatternV) * (pPlayer->nPatternV));
	pVertex[2].tex = D3DXVECTOR2((1.0f / (float)pPlayer->nMaxPatternH) * (pPlayer->nPatternH + 1.0f), (1.0f / pPlayer->nMaxPatternV) * (pPlayer->nPatternV + 1.0f));
	pVertex[3].tex = D3DXVECTOR2((1.0f / (float)pPlayer->nMaxPatternH) * (pPlayer->nPatternH + 1.0f), (1.0f / pPlayer->nMaxPatternV) * (pPlayer->nPatternV));

	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
// ポリゴンの終了処理
//-----------------------------------------------------------------------------
void UninitPlayer(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}


	for (int nCntTEX = 0; nCntTEX < PLAYER_TEX; nCntTEX++)
	{
		//テクスチャの開放
		if (g_pTexturePlayer[nCntTEX] != NULL)
		{
			g_pTexturePlayer[nCntTEX]->Release();
			g_pTexturePlayer[nCntTEX] = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
// ポリゴンの更新処理
//-----------------------------------------------------------------------------
void UpdatePlayer(void)
{
	VERTEX_2D *pVertex;
	BLOCK *pBlock;
	PLAYER *pPlayer;

	pBlock = GetBlock();
	pPlayer = &g_player;

	if (pPlayer->bUse == true)
	{
		//前回位置保存
		pPlayer->posold = pPlayer->pos;

		//プレイヤーの移動
		MovePlayer();

		//移動量の減衰
		pPlayer->move.x += (0 - pPlayer->move.x) * PLAYER_SPEED_DECAY;

		//重力
		pPlayer->move.y += PLAYER_GRAVITY;

		//開始位置と終了位置をプレイヤーの移動でずらす
		g_finishPos.x += -pPlayer->move.x;
		g_startPos.x += -pPlayer->move.x;

		////スクロールの状態設定
		//Scroll();

		//ポリゴンの位置更新
		pPlayer->pos.x += pPlayer->move.x;
		pPlayer->pos.y += pPlayer->move.y;


		//スクロールの状態から判定
		switch (pPlayer->scrollState)
		{
		case PLAYERSCROLL_NORMAL:
			pPlayer->fSpeed = PLAYER_SPEED;
			break;

		case PLAYERSCROLL_CENTER:
			if (pPlayer->pos.x > (SCREEN_WIDTH / 2) + 10)
			{
				pPlayer->pos.x = (SCREEN_WIDTH / 2) + 10;
				pPlayer->fSpeed = PLAYER_SPEED;
			}
			else if (pPlayer->pos.x < (SCREEN_WIDTH / 2) - 10)
			{
				pPlayer->pos.x = (SCREEN_WIDTH / 2) - 10;
				pPlayer->fSpeed = PLAYER_SPEED;
			}
			else
			{
				pPlayer->fSpeed = PLAYER_SPEED;
			}
			break;
		}

		// ダメージ状態
		switch (pPlayer->damage)
		{
		case DAMAGESTATE_HURT:
			//ダメージ
			pPlayer->nLife--;

			//状態を無敵に移行
			pPlayer->damage = DAMAGESTATE_INV;
			break;
			
		case DAMAGESTATE_INV:
			if (pPlayer->nLife <= 0)
			{
				pPlayer->damage = DAMAGESTATE_DIE;
			}
			pPlayer->nWait++;
			if (pPlayer->nWait > 60)
			{
				pPlayer->damage = DAMAGESTATE_NORMAL;
				pPlayer->nWait = 0;
			}

		case DAMAGESTATE_DIE:
			pPlayer->nWait++;
			pPlayer->move.x = 0;
			if (pPlayer->nWait > 30)
			{
				PlaySound(SOUND_LABEL_SE_DIE_PLAYER);
				SetFade(FADE_OUT, MODE_GAME);
				pPlayer->nWait = 0;
			}
			break;

		default:
			break;
		}
		////場外に落ちる
		if (pPlayer->pos.y > SCREEN_HEIGHT + pPlayer->fHeight)
		{
			GOAL *pGoal;
			ENEMY *pEnemy;
			pEnemy = GetEnemy();
			pGoal = GetGoal();
			pPlayer->pos.y = 100;
			for (int nCnt = 0; nCnt < BLOCK_MAX; nCnt++, pBlock++)
			{
				if (pBlock->bUse == true)
				{
					pBlock->pos.x += 200;
				}
			}
			for (int nCnt = 0; nCnt < ENEMY_POLYGON; nCnt++, pEnemy++)
			{
				if (pEnemy->bUse == true)
				{
					pEnemy->pos.x += 200;
				}
			}
			if (pGoal->bUse == true)
			{
				pGoal->pos.x += 200;
			}
		}


		//キャラクターの状態による処理
		CharacterState();

		//プレイヤーが乗ってるブロックの移動情報を引き継ぐ
		TransferBlockMove();

		//壁キック状態判定
		WallJump();

		//ブロックとの判定
		g_player.bJump = CollisionBlock(&g_player.pos, &g_player.posold, &g_player.move, g_player.fWidth, g_player.fHeight);

		//ゴールフラッグとの判定
		CollisionGoal(&g_player.pos, &g_player.posold, &g_player.move, g_player.fWidth, g_player.fHeight);

		//敵との当たり判定
		CollisionEnemy(&g_player.pos, &g_player.posold, &g_player.move, g_player.fWidth, g_player.fHeight);

		//頂点バッファをロックして頂点情報へのポインタを取得
		g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVertex, 0);

		//頂点情報の設定
		if (pPlayer->nLastKey == 0)
		{
			pVertex[0].tex = D3DXVECTOR2((1.0f / (float)pPlayer->nMaxPatternH) * (pPlayer->nPatternH), (1.0f / pPlayer->nMaxPatternV) * (pPlayer->nPatternV + 1.0f));
			pVertex[1].tex = D3DXVECTOR2((1.0f / (float)pPlayer->nMaxPatternH) * (pPlayer->nPatternH), (1.0f / pPlayer->nMaxPatternV) * (pPlayer->nPatternV));
			pVertex[2].tex = D3DXVECTOR2((1.0f / (float)pPlayer->nMaxPatternH) * (pPlayer->nPatternH + 1.0f), (1.0f / pPlayer->nMaxPatternV) * (pPlayer->nPatternV + 1.0f));
			pVertex[3].tex = D3DXVECTOR2((1.0f / (float)pPlayer->nMaxPatternH) * (pPlayer->nPatternH + 1.0f), (1.0f / pPlayer->nMaxPatternV) * (pPlayer->nPatternV));
		}
		else
		{
			pVertex[0].tex = D3DXVECTOR2((1.0f / (float)pPlayer->nMaxPatternH) * (pPlayer->nPatternH + 1.0f), (1.0f / pPlayer->nMaxPatternV) * (pPlayer->nPatternV + 1.0f));
			pVertex[1].tex = D3DXVECTOR2((1.0f / (float)pPlayer->nMaxPatternH) * (pPlayer->nPatternH + 1.0f), (1.0f / pPlayer->nMaxPatternV) * (pPlayer->nPatternV));
			pVertex[2].tex = D3DXVECTOR2((1.0f / (float)pPlayer->nMaxPatternH) * (pPlayer->nPatternH), (1.0f / pPlayer->nMaxPatternV) * (pPlayer->nPatternV + 1.0f));
			pVertex[3].tex = D3DXVECTOR2((1.0f / (float)pPlayer->nMaxPatternH) * (pPlayer->nPatternH), (1.0f / pPlayer->nMaxPatternV) * (pPlayer->nPatternV));


		}

		//頂点座標の更新
		pVertex[0].pos = D3DXVECTOR3(pPlayer->pos.x - pPlayer->fWidth, pPlayer->pos.y, 0.0f);
		pVertex[1].pos = D3DXVECTOR3(pPlayer->pos.x - pPlayer->fWidth, pPlayer->pos.y - pPlayer->fHeight, 0.0f);
		pVertex[2].pos = D3DXVECTOR3(pPlayer->pos.x + pPlayer->fWidth, pPlayer->pos.y, 0.0f);
		pVertex[3].pos = D3DXVECTOR3(pPlayer->pos.x + pPlayer->fWidth, pPlayer->pos.y - pPlayer->fHeight, 0.0f);

		//頂点カラーの設定
		if (pPlayer->damage == DAMAGESTATE_DIE)
		{
			pVertex[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);  //D3DCOLOR_RGBA(赤, 緑, 青, 透明度);
			pVertex[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVertex[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVertex[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		}
		else
		{
			pVertex[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);  //D3DCOLOR_RGBA(赤, 緑, 青, 透明度);
			pVertex[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVertex[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVertex[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}

		//頂点バッファをアンロック
		g_pVtxBuffPlayer->Unlock();

#ifdef _DEBUG
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{
			SetFade(FADE_OUT, MODE_RESULT);
		}
#endif
	}
}

//-----------------------------------------------------------------------------
// ポリゴンの描画処理
//-----------------------------------------------------------------------------
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	PLAYER *pPlayer;
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[256];

	pPlayer = &g_player;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	switch (pPlayer->state)
	{
	case PLAYERSTATE_NORMAL:
		pDevice->SetTexture(0, g_pTexturePlayer[0]);
		break;

	case PLAYERSTATE_SLIDE:
		pDevice->SetTexture(0, g_pTexturePlayer[1]);
		break;

	case PLAYERSTATE_ONWALL:
		pDevice->SetTexture(0, g_pTexturePlayer[2]);
		break;

	case PLAYERSTATE_LAND:
		pDevice->SetTexture(0, g_pTexturePlayer[0]);
		break;
	}

	if (pPlayer->bUse == true)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

#ifdef _DEBUG

	sprintf(&aStr[0], "move: %f \n", g_player.move.x);

	////テキストの描画
	g_pFontPlayer->DrawText(NULL, &aStr[0], -1, &rect, DT_BOTTOM, D3DCOLOR_RGBA(255, 255, 255, 255));

#endif
}

//-----------------------------------------------------------------------------
// プレイヤー情報の取得
//-----------------------------------------------------------------------------
PLAYER *GetPlayer(void)
{
	return &g_player;
}

//-----------------------------------------------------------------------------
// プレイヤーの移動処理
//-----------------------------------------------------------------------------
void MovePlayer()
{
	PLAYER *pPlayer;
	BLOCK *pBlock;

	pBlock = GetBlock();
	pPlayer = &g_player;
	
	//最後に押したキーを保存
	if (GetKeyboardPress(DIK_D) == true)
	{
		pPlayer->nLastKey = 0;
	}
	else if (GetKeyboardPress(DIK_A) == true)
	{
		pPlayer->nLastKey = 1;
	}

	if (pPlayer->state != PLAYERSTATE_SLIDE)
	{
		if (pPlayer->damage != DAMAGESTATE_DIE)
		{
			//左右移動
			if (GetKeyboardPress(DIK_D) == true)
			{
				pPlayer->nCounterAnim++;

				//テクスチャのパターンをアニメーションさせる
				if (pPlayer->nCounterAnim % PLAYER_TEX_ANIMINTERVAL == 0)
				{
					pPlayer->nPatternH = (pPlayer->nPatternH + 1) % PLAYER_TEX_SPLIT_H;
					pPlayer->nCounterAnim = 0;
				}

				//移動加算
				pPlayer->move.x += sinf(D3DX_PI * 0.5f) * pPlayer->fSpeed;
			}
			else if (GetKeyboardPress(DIK_A) == true)
			{
				pPlayer->nCounterAnim++;

				//テクスチャのパターンをアニメーションさせる
				if (pPlayer->nCounterAnim % PLAYER_TEX_ANIMINTERVAL == 0)
				{
					pPlayer->nPatternH = (pPlayer->nPatternH + 1) % PLAYER_TEX_SPLIT_H;
					pPlayer->nCounterAnim = 0;
				}

				//移動加算
				pPlayer->move.x += sinf(-D3DX_PI * 0.5f) * pPlayer->fSpeed;
			}

			//移動キーリリース時テクスチャパターン初期化
			if (GetKeyboardRelease(DIK_A) == true || GetKeyboardRelease(DIK_D) == true)
			{
				pPlayer->nPatternH = 0;
			}

			//ジャンプ
			if (pPlayer->bJump == false)
			{
				if (GetKeyboardTrigger(DIK_SPACE) == true)
				{
					//サウンド再生
					PlaySound(SOUND_LABEL_SE_JUMP);

					//移動加算
					pPlayer->move.y -= 20.0f;

					//ジャンプ状態に移行
					pPlayer->bJump = true;
				}
			}
			//ジャンプ中
			if (pPlayer->bJump == true)
			{
				//アニメーションパターン固定
				pPlayer->nPatternH = 1;
			}

			//壁ジャンプ
			if (pPlayer->bOnWall == true)
			{
				if (GetKeyboardTrigger(DIK_SPACE) == true)
				{
					if (GetKeyboardPress(DIK_D) == true)
					{
						//移動加算
						pPlayer->move.y -= 25.0f;
						pPlayer->move.x -= 40.0f;

						pPlayer->bOnWall = false;
					}
					else if (GetKeyboardPress(DIK_A) == true)
					{
						pPlayer->move.y -= 25.0f;
						pPlayer->move.x += 40.0f;

						pPlayer->bOnWall = false;
					}
				}
			}
		}
	}

	//スライディング中
	if (GetKeyboardTrigger(DIK_LSHIFT) == true)
	{
		pPlayer->nPatternH = 0;
		CharacterSlide();
	}
}

//-----------------------------------------------------------------------------
// 壁ジャンプ
//-----------------------------------------------------------------------------
void WallJump(void)
{
	BLOCK *pBlock;
	PLAYER *pPlayer;

	pPlayer = &g_player;
	pBlock = GetBlock();

	pPlayer->bOnWall = false;
	for (int nCntBLK = 0; nCntBLK < BLOCK_MAX; nCntBLK++, pBlock++)
	{
		if (g_player.bJump == true)
		{
			if ((pPlayer->pos.y - pPlayer->fHeight > pBlock->pos.y && pPlayer->pos.y - pPlayer->fHeight < pBlock->pos.y + pBlock->fHeight) &&
				(pPlayer->pos.x + pPlayer->fWidth > pBlock->pos.x && pPlayer->pos.x - pPlayer->fWidth < pBlock->pos.x + pBlock->fWidth))
			{
				if ((pPlayer->posold.x + pPlayer->fWidth <= pBlock->pos.x) ||
					(pPlayer->posold.x - pPlayer->fWidth >= pBlock->pos.x + pBlock->fWidth))
				{
					if (pPlayer->move.y > 0.0f)
					{
						pPlayer->move.y = 1.0f;
						pPlayer->bOnWall = true;
						//if ((pPlayer->pos.x + pPlayer->fWidth > pBlock->pos.x && pPlayer->pos.x - pPlayer->fWidth < pBlock->pos.x + pBlock->fWidth) &&
						//	pPlayer->pos.y - pPlayer->fHeight > pBlock->pos.y + pBlock->fHeight && pPlayer->pos.y - pPlayer->fHeight < pBlock->pos.y + pBlock->fHeight)
						//{
						//	pPlayer->move.y = 0.0f;
						//}
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 画面スクロール
//-----------------------------------------------------------------------------
void Scroll(void)
{
	PLAYER *pPlayer;
	pPlayer = &g_player;

	if (pPlayer->pos.x < (SCREEN_WIDTH / 2) + 10 &&
		pPlayer->pos.x >(SCREEN_WIDTH / 2) - 10)
	{
		pPlayer->scrollState = PLAYERSCROLL_CENTER;
	}
	else
	{
		pPlayer->scrollState = PLAYERSCROLL_NORMAL;
	}
}

//-----------------------------------------------------------------------------
// ブロック移動量受け渡し
//-----------------------------------------------------------------------------
void TransferBlockMove(void)
{
	BLOCK *pBlock;
	PLAYER *pPlayer;
	D3DXVECTOR3 *pScroll;

	pScroll = GetScroll();
	pPlayer = &g_player;
	pBlock = GetBlock();

	for (int nCntBLK = 0; nCntBLK < BLOCK_MAX; nCntBLK++, pBlock++)
	{
		if ((pPlayer->pos.x + pPlayer->fWidth > pBlock->pos.x && pPlayer->pos.x - pPlayer->fWidth < pBlock->pos.x + pBlock->fWidth) &&
			(pPlayer->pos.y > pBlock->pos.y && pPlayer->pos.y - pPlayer->fHeight < pBlock->pos.y + pBlock->fHeight))
		{
			if (pPlayer->posold.y <= pBlock->pos.y)
			{
				if (pBlock->nTypeNum == 1)
				{
					pScroll->x = -pBlock->move.x;
				}
				pPlayer->pos.y += pBlock->move.y;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// スライディング
//-----------------------------------------------------------------------------
void CharacterSlide(void)
{
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	if (pPlayer->bJump == false &&
		pPlayer->state == PLAYERSTATE_NORMAL)
	{
		pPlayer->state = PLAYERSTATE_SLIDE;

		//サウンド再生
		PlaySound(SOUND_LABEL_SE_SLIDE);

		//最後に押したキーからスライディングの方向を決定
		if (pPlayer->nLastKey == 1) // 左
		{
			pPlayer->move.x -= 60.0f;
		}
		else if (pPlayer->nLastKey == 0) // 右
		{
			pPlayer->move.x += 60.0f;
		}
	}
}

void CharacterState(void)
{
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	//プレイヤーの状態
	switch (pPlayer->state)
	{
	case PLAYERSTATE_NORMAL:
		//通常時のテクスチャ設定
		pPlayer->nMaxPatternH = PLAYER_TEX_SPLIT_H;
		pPlayer->nMaxPatternV = PLAYER_TEX_SPLIT_V;

		//壁張り付き状態のステート変更
		if (pPlayer->bOnWall == true)
		{
			pPlayer->state = PLAYERSTATE_ONWALL;
			pPlayer->fHeight = PLAYER_HEIGHT;
			pPlayer->fWidth = PLAYER_WIDTH;
			pPlayer->nMaxPatternH = 1;
			pPlayer->nMaxPatternV = 1;

			PlaySound(SOUND_LABEL_SE_ONWALL);
		}
		if (pPlayer->bJump == true)
		{
			pPlayer->state = PLAYERSTATE_LAND;
		}
		break;

	case PLAYERSTATE_ONWALL:
		pPlayer->fHeight = PLAYER_HEIGHT;
		pPlayer->fWidth = PLAYER_WIDTH;
		pPlayer->nMaxPatternH = 1;
		pPlayer->nMaxPatternV = 1;

		//頂点情報を元に戻す
		pPlayer->fWidth = PLAYER_WIDTH;
		pPlayer->fHeight = PLAYER_HEIGHT;

		if (pPlayer->bOnWall == false)
		{
			//ステートを通常時に戻す
			pPlayer->state = PLAYERSTATE_NORMAL;

			//テクスチャ設定を元に戻す
			pPlayer->nPatternH = 0;
			pPlayer->nMaxPatternH = PLAYER_TEX_SPLIT_H;
			pPlayer->nMaxPatternV = PLAYER_TEX_SPLIT_V;

			//頂点情報を元に戻す
			pPlayer->fWidth = PLAYER_WIDTH;
			pPlayer->fHeight = PLAYER_HEIGHT;
		}
		break;

	case PLAYERSTATE_SLIDE:
		//スライディング中のテクスチャ設定
		pPlayer->nMaxPatternH = PLAYER_TEX_SPLIT_SLIDE_H;
		pPlayer->nMaxPatternV = PLAYER_TEX_SPLIT_SLIDE_V;
		pPlayer->nCounterAnim++;
		//テクスチャのパターンをアニメーションさせる
		if (pPlayer->nCounterAnim % 2 == 0)
		{
			pPlayer->nPatternH = (pPlayer->nPatternH + 1) % PLAYER_TEX_SPLIT_SLIDE_H;
			pPlayer->nCounterAnim = 0;
		}

		//スライディング中の頂点情報設定
		pPlayer->fWidth = PLAYER_WIDTH_SLIDING;
		pPlayer->fHeight = PLAYER_HEIGHT_SLIDING;

		//スライディングの時間
		pPlayer->nWaitSlide++;
		if (pPlayer->nWaitSlide > 30)
		{
			//ステートを通常時に戻す
			pPlayer->state = PLAYERSTATE_NORMAL;

			//テクスチャ設定を元に戻す
			pPlayer->nPatternH = 0;
			pPlayer->nMaxPatternH = PLAYER_TEX_SPLIT_H;
			pPlayer->nMaxPatternV = PLAYER_TEX_SPLIT_V;

			//頂点情報を元に戻す
			pPlayer->fWidth = PLAYER_WIDTH;
			pPlayer->fHeight = PLAYER_HEIGHT;

			//待機時間リセット
			pPlayer->nWaitSlide = 0;
		}
		break;

	case PLAYERSTATE_LAND:
		//壁張り付き状態のステート変更
		if (pPlayer->bOnWall == true)
		{
			pPlayer->state = PLAYERSTATE_ONWALL;
			pPlayer->fHeight = PLAYER_HEIGHT;
			pPlayer->fWidth = PLAYER_WIDTH;
			pPlayer->nMaxPatternH = 1;
			pPlayer->nMaxPatternV = 1;

			PlaySound(SOUND_LABEL_SE_ONWALL);
		}

		if (pPlayer->bJump == false)
		{
			//ステートを通常時に戻す
			pPlayer->state = PLAYERSTATE_NORMAL;
			PlaySound(SOUND_LABEL_SE_LAND);
		}
		break;
	}
}