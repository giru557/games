//-----------------------------------------------------------------------------
//
// 爆発の処理 [explosion.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "explosion.h"

//-----------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------
#define MAX_EXPLOSION (256) //爆発の最大数
#define ANIM_SPLIT (7)		//アニメーションの分割数
#define ANIM_UPDATE (5)		//アニメーション更新頻度

//-----------------------------------------------------------------------------
//構造体宣言
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	int nSize;				//爆発のサイズ
	int nCounterAnim = 0;	//アニメーションカウンター初期化
	int nPatternAnim = 0;	//アニメーションパターンナンバー初期化
	bool bUse;				//使用しているか
} EXPLOSION;

//-----------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;
EXPLOSION g_aExplosion[MAX_EXPLOSION]; //爆発の情報


//-----------------------------------------------------------------------------
//爆発の初期化処理
//-----------------------------------------------------------------------------
HRESULT InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\explosion001.png", &g_pTextureExplosion);

	//EXPLOSIONの初期化
	for (int nCntExp = 0; nCntExp < MAX_EXPLOSION; nCntExp++)
	{
		g_aExplosion[nCntExp].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExp].bUse = false;
		g_aExplosion[nCntExp].nCounterAnim = 0;
		g_aExplosion[nCntExp].nPatternAnim = 0;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * MAX_EXPLOSION, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffExplosion, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファのロック
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVertex, 0);

	//頂点座標
	for (int nCntExp = 0; nCntExp < MAX_EXPLOSION; nCntExp++)
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
		pVertex[0].col = D3DCOLOR_RGBA(255, 165, 0, 255);  //D3DCOLOR_RGBA(赤, 緑, 青, 透明度);
		pVertex[1].col = D3DCOLOR_RGBA(255, 165, 0, 255);
		pVertex[2].col = D3DCOLOR_RGBA(255, 165, 0, 255);
		pVertex[3].col = D3DCOLOR_RGBA(255, 165, 0, 255);

		//UVの設定
		pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVertex[2].tex = D3DXVECTOR2(1.0f / ANIM_SPLIT, 1.0f);
		pVertex[3].tex = D3DXVECTOR2(1.0f / ANIM_SPLIT, 0.0f);

		pVertex += 4; //頂点のポインタをずらす
	}
	
	g_pVtxBuffExplosion->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//爆発の終了処理
//-----------------------------------------------------------------------------
void UninitExplosion(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}

	//テクスチャの開放
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}
}

//-----------------------------------------------------------------------------
//爆発の更新処理
//-----------------------------------------------------------------------------
void UpdateExplosion(void)
{
	EXPLOSION *pExplosion;
	VERTEX_2D *pVertex;

	pExplosion = &g_aExplosion[0];

	if (g_pVtxBuffExplosion != NULL)
	{
		//頂点バッファのロック
		g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVertex, 0);

		for (int nCntExp = 0; nCntExp < MAX_EXPLOSION; nCntExp++, pExplosion++)
		{
			if (pExplosion->bUse == true)
			{
				//頂点座標の設定
				pVertex[0].pos = D3DXVECTOR3(-g_aExplosion[nCntExp].nSize + g_aExplosion[nCntExp].pos.x, g_aExplosion[nCntExp].nSize + g_aExplosion[nCntExp].pos.y, 0.0f);
				pVertex[1].pos = D3DXVECTOR3(-g_aExplosion[nCntExp].nSize + g_aExplosion[nCntExp].pos.x, -g_aExplosion[nCntExp].nSize + g_aExplosion[nCntExp].pos.y, 0.0f);
				pVertex[2].pos = D3DXVECTOR3(g_aExplosion[nCntExp].nSize + g_aExplosion[nCntExp].pos.x, g_aExplosion[nCntExp].nSize + g_aExplosion[nCntExp].pos.y, 0.0f);
				pVertex[3].pos = D3DXVECTOR3(g_aExplosion[nCntExp].nSize + g_aExplosion[nCntExp].pos.x, -g_aExplosion[nCntExp].nSize + g_aExplosion[nCntExp].pos.y, 0.0f);

				//アニメーションカウンター
				g_aExplosion[nCntExp].nCounterAnim++;

				//アニメーションの更新頻度
				if (g_aExplosion[nCntExp].nCounterAnim % ANIM_UPDATE == 0)
				{
					//パターン更新
					g_aExplosion[nCntExp].nPatternAnim++;

					//UV設定
					pVertex[0].tex = D3DXVECTOR2((float)g_aExplosion[nCntExp].nPatternAnim * (1.0f / ANIM_SPLIT), 1.0f);
					pVertex[1].tex = D3DXVECTOR2((float)g_aExplosion[nCntExp].nPatternAnim * (1.0f / ANIM_SPLIT), 0.0f);
					pVertex[2].tex = D3DXVECTOR2(((float)g_aExplosion[nCntExp].nPatternAnim + 1) * (1.0f / ANIM_SPLIT), 1.0f);
					pVertex[3].tex = D3DXVECTOR2(((float)g_aExplosion[nCntExp].nPatternAnim + 1) * (1.0f / ANIM_SPLIT), 0.0f);

					//アニメーションが終わったとき
					//if (g_aExplosion[nCntExp].nCounterAnim > ANIM_UPDATE * ANIM_SPLIT)
					if (g_aExplosion[nCntExp].nPatternAnim >= ANIM_SPLIT)
					{
						//爆発の破棄
						pExplosion->bUse = false;

						//アニメーションカウンターの初期化
						g_aExplosion[nCntExp].nCounterAnim = 0;
						g_aExplosion[nCntExp].nPatternAnim = 0;
					}
				}
			}
			pVertex += 4; //頂点のポインタをずらす
		}

		//頂点バッファのアンロック
		g_pVtxBuffExplosion->Unlock();
	}
}

//-----------------------------------------------------------------------------
//爆発の描画処理
//-----------------------------------------------------------------------------
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureExplosion);

	for (int nCntExp = 0; nCntExp < MAX_EXPLOSION; nCntExp++)
	{
		if (g_aExplosion[nCntExp].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExp * 4, 2);
		}
	}
}

//-----------------------------------------------------------------------------
//爆発の設定
//-----------------------------------------------------------------------------
void SetExplosion(D3DXVECTOR3 pos, int nExplosionSize)
{
	EXPLOSION *pExplosion;
	VERTEX_2D *pVertex;

	pExplosion = &g_aExplosion[0];

	//頂点バッファのロック
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntExp = 0; nCntExp < MAX_EXPLOSION; nCntExp++, pExplosion++)
	{
		if (pExplosion->bUse == false)
		{
			//位置を設定
			g_aExplosion[nCntExp].pos = pos;

			//爆発のサイズ設定
			g_aExplosion[nCntExp].nSize = nExplosionSize;

			//頂点座標を設定
			pVertex[0].pos = D3DXVECTOR3(-g_aExplosion[nCntExp].nSize + g_aExplosion[nCntExp].pos.x, g_aExplosion[nCntExp].nSize + g_aExplosion[nCntExp].pos.y, 0.0f);
			pVertex[1].pos = D3DXVECTOR3(-g_aExplosion[nCntExp].nSize + g_aExplosion[nCntExp].pos.x, -g_aExplosion[nCntExp].nSize + g_aExplosion[nCntExp].pos.y, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_aExplosion[nCntExp].nSize + g_aExplosion[nCntExp].pos.x, g_aExplosion[nCntExp].nSize + g_aExplosion[nCntExp].pos.y, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_aExplosion[nCntExp].nSize + g_aExplosion[nCntExp].pos.x, -g_aExplosion[nCntExp].nSize + g_aExplosion[nCntExp].pos.y, 0.0f);

			//使用中に変更
			pExplosion->bUse = true;

			break;

		}
		//頂点のポインタをずらす
		pVertex += 4;
	}
	g_pVtxBuffExplosion->Unlock();
}