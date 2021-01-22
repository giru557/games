//-----------------------------------------------------------------------------
//
// ゴールの処理 [goalflag.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "goalflag.h"
#include "fade.h"
#include "player.h"
#include "timescore.h"
#include "goalflag.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define GOAL_POLYGON (1)
#define FLAG_WIDTH (100)
#define FLAG_HEIGHT (100)

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffGoal = NULL;					// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureGoal = NULL;				// ポリゴンのテクスチャへのポインタ
GOAL g_Goal;
bool g_fFlag;

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
void CaughtFlag(bool fFlag);

//-----------------------------------------------------------------------------
// ゴールの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitGoal(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	//GOAL初期化
	g_Goal.pos = D3DXVECTOR3(0, 0, 0.0f);
	g_Goal.fWidth = FLAG_WIDTH;
	g_Goal.fHeight = FLAG_HEIGHT;
	g_Goal.bUse = false;
	g_Goal.bGoal = false;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\gate.png", &g_pTextureGoal);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * GOAL_POLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffGoal, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffGoal->Lock(0, 0, (void**)&pVertex, 0);

	//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
	pVertex[0].pos = D3DXVECTOR3(g_Goal.pos.x, g_Goal.pos.y + FLAG_HEIGHT, 0.0f);
	pVertex[1].pos = D3DXVECTOR3(g_Goal.pos.x, g_Goal.pos.y, 0.0f);
	pVertex[2].pos = D3DXVECTOR3(g_Goal.pos.x + FLAG_WIDTH, g_Goal.pos.y + FLAG_HEIGHT, 0.0f);
	pVertex[3].pos = D3DXVECTOR3(g_Goal.pos.x + FLAG_WIDTH, g_Goal.pos.y, 0.0f);

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

	//頂点バッファをアンロックする
	g_pVtxBuffGoal->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
// ゴールの終了処理
//-----------------------------------------------------------------------------
void UninitGoal(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffGoal != NULL)
	{
		g_pVtxBuffGoal->Release();
		g_pVtxBuffGoal = NULL;
	}

	//テクスチャの開放
	if (g_pTextureGoal != NULL)
	{
		g_pTextureGoal->Release();
		g_pTextureGoal = NULL;
	}

}

//-----------------------------------------------------------------------------
// ゴールの更新処理
//-----------------------------------------------------------------------------
void UpdateGoal(void)
{
	VERTEX_2D *pVertex;
	PLAYER *pPlayer;

	pPlayer = GetPlayer();

	if (g_Goal.bUse == true)
	{
		//スクロール量
		g_Goal.scroll.x = -pPlayer->move.x;

		////ブロックのスクロール
		//if (pPlayer->scrollState == PLAYERSCROLL_CENTER)
		//{
		//	g_Goal.pos.x += g_Goal.scroll.x;
		//	g_Goal.pos.y += g_Goal.scroll.y;
		//}

		//頂点バッファをロックし頂点情報へのポインタを取得
		g_pVtxBuffGoal->Lock(0, 0, (void**)&pVertex, 0);

		//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
		pVertex[0].pos = D3DXVECTOR3(g_Goal.pos.x, g_Goal.pos.y + FLAG_HEIGHT, 0.0f);
		pVertex[1].pos = D3DXVECTOR3(g_Goal.pos.x, g_Goal.pos.y, 0.0f);
		pVertex[2].pos = D3DXVECTOR3(g_Goal.pos.x + FLAG_WIDTH, g_Goal.pos.y + FLAG_HEIGHT, 0.0f);
		pVertex[3].pos = D3DXVECTOR3(g_Goal.pos.x + FLAG_WIDTH, g_Goal.pos.y, 0.0f);

		//頂点バッファをアンロックする
		g_pVtxBuffGoal->Unlock();
	}
}

//-----------------------------------------------------------------------------
// ゴールの描画処理
//-----------------------------------------------------------------------------
void DrawGoal(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGoal, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureGoal);

	if (g_Goal.bUse == true)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//-----------------------------------------------------------------------------
// ゴールとの当たり判定
//-----------------------------------------------------------------------------
void CollisionGoal(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{
	if (g_Goal.bUse == true)
	{
		if ((pPos->x + fWidth > g_Goal.pos.x && pPos->x - fWidth < g_Goal.pos.x + g_Goal.fWidth) &&
			(pPos->y > g_Goal.pos.y && pPos->y - fHeight < g_Goal.pos.y))
		{
			if (pPosOld->y <= g_Goal.pos.y &&
				pPosOld->x + fWidth >= g_Goal.pos.x && pPosOld->x - fWidth <= g_Goal.pos.x + g_Goal.fWidth)
			{
				CaughtFlag(true);
			}
		}
		if ((pPos->x + fWidth > g_Goal.pos.x && pPos->x - fWidth < g_Goal.pos.x + g_Goal.fWidth) &&
			(pPos->y > g_Goal.pos.y + g_Goal.fHeight && pPos->y - fHeight < g_Goal.pos.y + g_Goal.fHeight))
		{
			if (pPosOld->y - fHeight >= g_Goal.pos.y + g_Goal.fHeight &&
				pPosOld->x + fWidth >= g_Goal.pos.x && pPosOld->x - fWidth <= g_Goal.pos.x + g_Goal.fWidth)
			{
				CaughtFlag(true);
			}
		}
		if ((pPos->y > g_Goal.pos.y && pPos->y - fHeight < g_Goal.pos.y + g_Goal.fHeight) &&
			(pPos->x + fWidth > g_Goal.pos.x && pPos->x - fWidth < g_Goal.pos.x))
		{
			if (pPosOld->x <= g_Goal.pos.x &&
				pPosOld->y >= g_Goal.pos.y && pPosOld->y - fHeight <= g_Goal.pos.y + g_Goal.fHeight)
			{
				CaughtFlag(true);
			}
		}
		if ((pPos->y > g_Goal.pos.y && pPos->y - fHeight < g_Goal.pos.y + g_Goal.fHeight) &&
			(pPos->x + fWidth > g_Goal.pos.x + g_Goal.fWidth && pPos->x - fWidth < g_Goal.pos.x + g_Goal.fWidth))
		{
			if (pPosOld->x + fWidth >= g_Goal.pos.x + g_Goal.fWidth &&
				pPosOld->y > g_Goal.pos.y && pPosOld->y - fHeight < g_Goal.pos.y + g_Goal.fHeight)
			{
				CaughtFlag(true);
			}
		}
	}
}

//-----------------------------------------------------------------------------
// フラッグ取得
//-----------------------------------------------------------------------------
void CaughtFlag(bool fFlag)
{
	PLAYER *pPlayer;
	TIME *pTimeScore;
	pPlayer = GetPlayer();
	pTimeScore = GetTimeScore();

	g_Goal.bGoal = fFlag;

	if (fFlag == true &&
		pPlayer->bUse == true)
	{
		switch (g_Goal.stage)
		{
		case STAGE_1:
			SetFade(FADE_OUT, MODE_GAME);
			g_Goal.stage = STAGE_2;
			break;

		case STAGE_2:
			pTimeScore->dwFinishTime = timeGetTime();
			SetFade(FADE_OUT, MODE_RESULT);
			g_Goal.stage = STAGE_1;
			break;
		default:
			break;
		}

		pPlayer->move = D3DXVECTOR3(0, 0, 0);
		pPlayer->bUse = false;
	}
}

void SetFlag(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	VERTEX_2D *pVertex;

	if (g_Goal.bUse == false)
	{
		//位置の設定
		g_Goal.pos = pos;

		//幅の設定
		g_Goal.fWidth = fWidth;

		//高さの設定
		g_Goal.fHeight = fHeight;

		//頂点バッファをロックし頂点情報へのポインタを取得
		g_pVtxBuffGoal->Lock(0, 0, (void**)&pVertex, 0);

		//頂点座標 D3DXVECTOR3(X,Y, 0.0f);	
		pVertex[0].pos = D3DXVECTOR3(g_Goal.pos.x, g_Goal.pos.y + FLAG_HEIGHT, 0.0f);
		pVertex[1].pos = D3DXVECTOR3(g_Goal.pos.x, g_Goal.pos.y, 0.0f);
		pVertex[2].pos = D3DXVECTOR3(g_Goal.pos.x + FLAG_WIDTH, g_Goal.pos.y + FLAG_HEIGHT, 0.0f);
		pVertex[3].pos = D3DXVECTOR3(g_Goal.pos.x + FLAG_WIDTH, g_Goal.pos.y, 0.0f);

		//頂点バッファをアンロックする
		g_pVtxBuffGoal->Unlock();

		g_Goal.bUse = true;
	}
}

GOAL *GetGoal(void)
{
	return &g_Goal;
}