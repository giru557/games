//-----------------------------------------------------------------------------
//
// プレイヤーの処理 [player.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "enemy.h"
#include "BG.h"
#include "sound.h"
#include "fade.h"
#include "minigun.h"
#include "effect.h"
#include "missile.h"
#include "shop.h"
#include <stdio.h>

//-----------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------
#define ANIMATION_INTERVAL (10)				//アニメーションの更新頻度

//-----------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffPlayer = NULL;   //頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;			//ポリゴンのテクスチャへのポインタ
int g_nCounterAnim;									//アニメーションカウンター
int g_nPatternAnim;									//アニメーションパターンナンバー
D3DXVECTOR3 g_rotPlayer;							//ポリゴンの回転角
float g_fLengthPlayer;								//ポリゴンの対角線の長さ
float g_fAnglePlayer;								//ポリゴンの対角線の角度
D3DXVECTOR3 g_movePlayer;							//移動量
PLAYER g_player;									//プレイヤーの情報
int g_aPressCounter[2];
PLAYERWEAPON g_PlayerWeapon;						//どの武器を使用するか
//LPD3DXFONT g_pFontPlayer = NULL;							//フォントへのポインタ
bool g_bAnim;
int g_nWait;										//待機時間

//プロトタイプ宣言
void MovePlayer(void);
void CharaTex(float nValue, int nIdx);

//-----------------------------------------------------------------------------
//ポリゴンの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;
	
	g_nCounterAnim = 0;	//アニメーションカウンター初期化
	g_nPatternAnim = 0;	//アニメーションパターンナンバー初期化
	g_nWait = 0;
	g_PlayerWeapon = PLAYERWEAPON_NORMAL;
	g_player.bUse = true;
	g_player.nCounterState = 0;
	g_player.nLife = 50;
	g_player.state = PLAYERSTATE_WAIT;

	//デバイスの取得
	pDevice = GetDevice();

	//フォントの生成
	//D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFontPlayer);

	//テクスチャ読み込み
	//D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\ship.png", &g_pTexturePlayer);
	D3DXCreateTextureFromFileEx(pDevice, "data\\TEXTURE\\ship.png", 80, 48, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0xFF000000, NULL, NULL, &g_pTexturePlayer);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPlayer, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVertex, 0);

	//プレイヤーの中心位置
	g_player.pos = D3DXVECTOR3(PLAYER_POS_X, PLAYER_POS_Y, 0.0f);

	//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
	pVertex[0].pos = D3DXVECTOR3(g_player.pos.x - PLAYER_WIDTH, g_player.pos.y + PLAYER_HEIGHT, 0.0f);		 //TRIANGLESTRIPで四角
	pVertex[1].pos = D3DXVECTOR3(g_player.pos.x - PLAYER_WIDTH, g_player.pos.y - PLAYER_HEIGHT, 0.0f);
	pVertex[2].pos = D3DXVECTOR3(g_player.pos.x + PLAYER_WIDTH, g_player.pos.y + PLAYER_HEIGHT, 0.0f);
	pVertex[3].pos = D3DXVECTOR3(g_player.pos.x + PLAYER_WIDTH, g_player.pos.y - PLAYER_HEIGHT, 0.0f);

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
	pVertex[2].tex = D3DXVECTOR2(0.125f, 1.0f);
	pVertex[3].tex = D3DXVECTOR2(0.125f, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();

	//プレイヤーを待機状態に移行 (2秒間)
	if (g_player.state == PLAYERSTATE_WAIT)
	{
		g_player.nCounterState = 120;
		//ポリゴンの色を戻す
		pVertex[0].col = D3DCOLOR_RGBA(255, 255, 255, 127);  //D3DCOLOR_RGBA(赤, 緑, 青, 透明度);
		pVertex[1].col = D3DCOLOR_RGBA(255, 255, 255, 127);
		pVertex[2].col = D3DCOLOR_RGBA(255, 255, 255, 127);
		pVertex[3].col = D3DCOLOR_RGBA(255, 255, 255, 127);
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
//ポリゴンの終了処理
//-----------------------------------------------------------------------------
void UninitPlayer(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}

	//テクスチャの開放
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}
}

//-----------------------------------------------------------------------------
//ポリゴンの更新処理
//-----------------------------------------------------------------------------
void UpdatePlayer(void)
{
	VERTEX_2D *pVertex;
	g_nCounterAnim++; //アニメーションカウンター更新 (カウントアップ)

	if (g_player.bUse == true)
	{
		//ポリゴンの位置更新
		g_player.pos.x += g_movePlayer.x;
		g_player.pos.y += g_movePlayer.y;

		//移動量の減衰
		g_movePlayer.x += (0 - g_movePlayer.x) * 0.2f;
		g_movePlayer.y += (0 - g_movePlayer.y) * 0.2f;

		//対角線の長さ
		g_fLengthPlayer = sqrtf((PLAYER_WIDTH) * (PLAYER_WIDTH)+(-PLAYER_HEIGHT) * (-PLAYER_HEIGHT));

		//対角線の角度
		g_fAnglePlayer = atan2f(PLAYER_WIDTH, -PLAYER_HEIGHT);


		//頂点バッファをロックして頂点情報へのポインタを取得
		g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVertex, 0);

		//頂点座標の更新
		pVertex[0].pos.x = g_player.pos.x + sinf(g_rotPlayer.z + (g_fAnglePlayer + D3DX_PI)) * g_fLengthPlayer;
		pVertex[0].pos.y = g_player.pos.y + cosf(g_rotPlayer.z + (g_fAnglePlayer + D3DX_PI)) * g_fLengthPlayer;
		pVertex[0].pos.z = 0.0f;

		pVertex[1].pos.x = g_player.pos.x + sinf(g_rotPlayer.z + (g_fAnglePlayer * -1)) * g_fLengthPlayer;
		pVertex[1].pos.y = g_player.pos.y + cosf(g_rotPlayer.z + (g_fAnglePlayer * -1)) * g_fLengthPlayer;
		pVertex[1].pos.z = 0.0f;

		pVertex[2].pos.x = g_player.pos.x + sinf(g_rotPlayer.z + (g_fAnglePlayer * -1 + D3DX_PI)) * g_fLengthPlayer;
		pVertex[2].pos.y = g_player.pos.y + cosf(g_rotPlayer.z + (g_fAnglePlayer * -1 + D3DX_PI)) * g_fLengthPlayer;
		pVertex[2].pos.z = 0.0f;

		pVertex[3].pos.x = g_player.pos.x + sinf(g_rotPlayer.z + g_fAnglePlayer) * g_fLengthPlayer;
		pVertex[3].pos.y = g_player.pos.y + cosf(g_rotPlayer.z + g_fAnglePlayer) * g_fLengthPlayer;
		pVertex[3].pos.z = 0.0f;

		//pVertex[3].pos = D3DXVECTOR3(sinf(g_fAnglePolygon) * g_fLengthPolygon, cosf(g_fAnglePolygon) * g_fLengthPolygon, 0.0f);
		//pVertex[0].pos = D3DXVECTOR3(-40 + g_posPolygon.x, 60 + g_posPolygon.y, 0.0f);
		//pVertex[1].pos = D3DXVECTOR3(-40 + g_posPolygon.x, -60 + g_posPolygon.y, 0.0f);
		//pVertex[2].pos = D3DXVECTOR3(40 + g_posPolygon.x, 60 + g_posPolygon.y, 0.0f);
		//pVertex[3].pos = D3DXVECTOR3(40 + g_posPolygon.x, -60 + g_posPolygon.y, 0.0f);

		//ANIMATION_INTERVALの数で割り切れた時
		if (g_nCounterAnim % ANIMATION_INTERVAL == 0)
		{
			//UV設定
			g_bAnim = g_bAnim ? false : true;

		}

		//キャラアニメーション(常時)
		if (g_bAnim == true)
		{
			g_nPatternAnim = 0;
		}
		else
		{
			g_nPatternAnim = 1;
		}

		//キャラアニメーション(移動時(左))
		if (GetKeyboardPress(DIK_A) == true)
		{
			if (g_movePlayer.x > -5.5f)
			{
				//キャラのテクスチャ設定
				CharaTex(1.0f, 0);
			}
			else
			{
				//キャラのテクスチャ設定
				CharaTex(0.0f, 0);
			}
		}
		else
		{
			//キャラのテクスチャ設定
			CharaTex(2.0f, 0);
		}

		//キャラアニメーション(移動時(右))
		if (GetKeyboardPress(DIK_D) == true)
		{
			if (g_movePlayer.x > 5.5f)
			{
				//キャラのテクスチャ設定
				CharaTex(4.0f, 0);
			}
			else
			{
				//キャラのテクスチャ設定
				CharaTex(3.0f, 0);
			}
		}

		//ステート判定
		switch (g_player.state)
		{
		case PLAYERSTATE_DAMAGE: //ダメージ時
			g_player.nCounterState--;

			if (g_player.nCounterState <= 0)
			{
				g_player.state = PLAYERSTATE_NORMAL;

				//ポリゴンの色を戻す
				pVertex[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);  //D3DCOLOR_RGBA(赤, 緑, 青, 透明度);
				pVertex[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVertex[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVertex[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			}
			break;

		case PLAYERSTATE_WAIT:
			g_player.nCounterState--;

			if (g_player.nCounterState <= 0)
			{
				g_player.state = PLAYERSTATE_NORMAL;

				//ポリゴンの色を戻す
				pVertex[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);  //D3DCOLOR_RGBA(赤, 緑, 青, 透明度);
				pVertex[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVertex[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVertex[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			}
			break;
		}

		//頂点バッファをアンロック
		g_pVtxBuffPlayer->Unlock();

		//キャラ移動
		MovePlayer();

		//弾の発射と武器の選択
		g_PlayerWeapon = GetWeapon();
		g_aPressCounter[1]++;
		if (g_player.state != PLAYERSTATE_WAIT)
		{
			switch (g_PlayerWeapon)
			{
			case PLAYERWEAPON_NORMAL: //通常時
				if (GetKeyboardTrigger(DIK_SPACE) == true)
				{
					//サウンド再生
					PlaySound(SOUND_LABEL_SE_LASER2);

					//弾発射
					SetBullet(g_player.pos, D3DXVECTOR3(0.0f, cosf(D3DX_PI) * 10, 0.0f), 50, BULLETSTATE_OWNER_PLAYER, BULLETTYPE_NORMAL);
				}
				break;

			case PLAYERWEAPON_MINIGUN: //ミニガン装備時
				if (GetKeyboardPress(DIK_SPACE) == true)
				{
					g_aPressCounter[0]++;

					if (g_aPressCounter[0] % 4 == 0)
					{
						//サウンド再生
						PlaySound(SOUND_LABEL_SE_LASER3);

						//弾発射
						SetMinigun(g_player.pos, D3DXVECTOR3(0.0f, cosf(D3DX_PI) * 15, 0.0f), 40, MINIGUNSTATE_OWNER_PLAYER);
					}
				}
				if (GetKeyboardRelease(DIK_SPACE) == true)
				{
					g_aPressCounter[0] = 0;
				}
				if (GetKeyboardTrigger(DIK_SPACE) == true)
				{
					//サウンド再生
					PlaySound(SOUND_LABEL_SE_LASER3);
					SetMinigun(g_player.pos, D3DXVECTOR3(0.0f, cosf(D3DX_PI) * 15, 0.0f), 40, MINIGUNSTATE_OWNER_PLAYER);
				}
				break;

			case PLAYERWEAPON_MISSILE: //ミサイル装備時
				if (GetKeyboardTrigger(DIK_SPACE) == true)
				{
					if (g_aPressCounter[1] > 45)
					{
						SetMissile(g_player.pos, D3DXVECTOR3(0.0f, cosf(D3DX_PI) * 10, 0.0f), 40, MISSILESTATE_OWNER_PLAYER);
						g_aPressCounter[1] = 0;

						//ミサイル発射音
						PlaySound(SOUND_LABEL_SE_MISSILE);
					}
				}
				break;

			default: //例外
				if (GetKeyboardTrigger(DIK_SPACE) == true)
				{
					SetBullet(g_player.pos, D3DXVECTOR3(0.0f, cosf(D3DX_PI) * 10, 0.0f), 50, BULLETSTATE_OWNER_PLAYER, BULLETTYPE_NORMAL);

					//サウンド再生
					PlaySound(SOUND_LABEL_SE_LASER2);
				}
				break;
			}
		}

		//移動制限
		if (g_player.pos.y < 0 + PLAYER_HEIGHT)
		{
			g_player.pos.y = 0 + PLAYER_HEIGHT;
		}
		else if (g_player.pos.y > SCREEN_HEIGHT - PLAYER_HEIGHT)
		{
			g_player.pos.y = SCREEN_HEIGHT - PLAYER_HEIGHT;
		}
		if (g_player.pos.x < 0 + PLAYER_WIDTH)
		{
			g_player.pos.x = 0 + PLAYER_WIDTH;
		}
		else if (g_player.pos.x > SCREEN_WIDTH - PLAYER_WIDTH)
		{
			g_player.pos.x = SCREEN_WIDTH - PLAYER_WIDTH;
		}
	}
	

	//死亡時待機時間
	if (g_player.bUse == false)
	{
		g_nWait++;
		if (g_nWait > 90)
		{
			SetFade(FADE_OUT, MODE_RESULT);
		}
	}

	//リザルト画面へ
	if (GetKeyboardTrigger(DIK_BACKSPACE) == true)
	{
		SetFade(FADE_OUT, MODE_RESULT);
	}
}

//-----------------------------------------------------------------------------
//ポリゴンの描画処理
//-----------------------------------------------------------------------------
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePlayer);

	if (g_player.bUse == true)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "move: %f \n", g_movePlayer.x);

	////テキストの描画
	//g_pFontPlayer->DrawText(NULL, &aStr[0], -1, &rect, DT_BOTTOM, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//ヒット判定
bool HitPlayer(int nDamage)
{
	VERTEX_2D *pVertex;
	g_player.nLife -= nDamage;

	//頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVertex, 0);

	if (g_player.nLife <= 0)
	{
		//爆発
		SetExplosion(g_player.pos, 50);

		//死亡サウンド
		PlaySound(SOUND_LABEL_SE_DEAD);

		//キャラを不使用に
		g_player.bUse = false;

		return true;
	}
	else
	{
		g_player.state = PLAYERSTATE_DAMAGE;
		g_player.nCounterState = 10;

		//ポリゴンの色を変更
		pVertex[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);  //D3DCOLOR_RGBA(赤, 緑, 青, 透明度);
		pVertex[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVertex[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVertex[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

		//ヒット音
		PlaySound(SOUND_LABEL_SE_HIT3);

		return false;
	}

	//頂点バッファをアンロック
	g_pVtxBuffPlayer->Unlock();
}

PLAYER *GetPlayer(void)
{
	return &g_player;
}

void SetWeapon(PLAYERWEAPON weapon)
{
	g_PlayerWeapon = weapon;
}

void MovePlayer()
{
	if (GetKeyboardPress(DIK_D) == true)
	{
		if (GetKeyboardPress(DIK_W) == true)
		{
			g_movePlayer.x += sinf(D3DX_PI * 0.75f) * PLAYER_SPEED;
			g_movePlayer.y += cosf(D3DX_PI * 0.75f) * PLAYER_SPEED;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{
			g_movePlayer.x += sinf(D3DX_PI * 0.25f) * PLAYER_SPEED;
			g_movePlayer.y += cosf(D3DX_PI * 0.25f) * PLAYER_SPEED;
		}
		else
		{
			g_movePlayer.x += sinf(D3DX_PI * 0.5f) * PLAYER_SPEED;
		}
	}
	else if (GetKeyboardPress(DIK_A) == true)
	{
		if (GetKeyboardPress(DIK_W) == true)
		{
			g_movePlayer.x += sinf(-D3DX_PI * 0.75f) * PLAYER_SPEED;
			g_movePlayer.y += cosf(-D3DX_PI * 0.75f) * PLAYER_SPEED;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{
			g_movePlayer.x += sinf(-D3DX_PI * 0.25f) * PLAYER_SPEED;
			g_movePlayer.y += cosf(-D3DX_PI * 0.25f) * PLAYER_SPEED;
		}
		else
		{
			g_movePlayer.x += sinf(-D3DX_PI * 0.5f) * PLAYER_SPEED;
		}
	}
	else if (GetKeyboardPress(DIK_W) == true)
	{
		if (GetKeyboardPress(DIK_S) == true)
		{
			g_movePlayer.y += cosf(D3DX_PI) * 0;
		}
		else
		{
			g_movePlayer.y += cosf(D3DX_PI) * PLAYER_SPEED;
		}
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{
		if (GetKeyboardPress(DIK_W) == true)
		{
			g_movePlayer.y += cosf(D3DX_PI) * 0;
		}
		else
		{
			g_movePlayer.y += cosf(0) * PLAYER_SPEED;
		}
	}
}

//キャラテクスチャ設定
void CharaTex(float nValue, int nIdx)
{
	VERTEX_2D *pVertex;

	//頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVertex, 0);

	pVertex += nIdx * 4;

	//UVの設定
	pVertex[0].tex = D3DXVECTOR2((nValue) * (1.0f / TEXTURE_SPLIT_HORIZONTAL), (g_nPatternAnim + 1.0f) * (1.0f / TEXTURE_SPLIT_VERTICAL));
	pVertex[1].tex = D3DXVECTOR2((nValue) * (1.0f / TEXTURE_SPLIT_HORIZONTAL), (g_nPatternAnim) * (1.0f / TEXTURE_SPLIT_VERTICAL));
	pVertex[2].tex = D3DXVECTOR2((nValue + 1.0f) * (1.0f / TEXTURE_SPLIT_HORIZONTAL), (g_nPatternAnim + 1.0f) * (1.0f / TEXTURE_SPLIT_VERTICAL));
	pVertex[3].tex = D3DXVECTOR2((nValue + 1.0f) * (1.0f / TEXTURE_SPLIT_HORIZONTAL), (g_nPatternAnim) * (1.0f / TEXTURE_SPLIT_VERTICAL));

		//頂点バッファをアンロック
	g_pVtxBuffPlayer->Unlock();
}

//無敵時間設定
void SetInvincible(int nTime)
{
	for (int nCntTime = 1; nCntTime <= nTime; nCntTime++)
	{
		if (nCntTime == nTime)
		{
			//
		}
	}
}