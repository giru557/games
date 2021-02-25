//-----------------------------------------------------------------------------
//
// 影の処理 [shadow.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "shadow.h"
#include "model.h"
#include "player.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 影の構造体
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXMATRIX mtxWorld;														// ワールドマトリックス
	D3DXVECTOR3 pos;															// 位置
	D3DXVECTOR3 rot;															// 向き
	bool bUse;																	// 使用しているかどうか
} Shadow;

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffShadow = NULL;								// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;										// ポリゴンのテクスチャへのポインタ
Shadow g_aShadow[SHADOW_MAX];													// 影の情報
D3DXVECTOR3 g_posShadow;														// 位置
D3DXVECTOR3 g_rotShadow;														// 向き
D3DXMATRIX g_mtxWorldShadow;													// ワールドマトリックス
int g_aShadowNum[SHADOW_MAX];

//-----------------------------------------------------------------------------
// 影の初期化処理
//-----------------------------------------------------------------------------
HRESULT InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	for (int nCntShadow = 0; nCntShadow < SHADOW_MAX; nCntShadow++)
	{
		g_aShadow[nCntShadow].bUse = false;
		g_aShadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	//g_posShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	//g_rotShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\shadow000.jpg", &g_pTextureShadow);

	//頂点バッファ生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * VERTEX_AMOUNT * SHADOW_MAX,		//サイズ
		D3DUSAGE_WRITEONLY,									//
		FVF_VERTEX_3D,										//フォーマット
		D3DPOOL_MANAGED,									//
		&g_pVtxBuffShadow,									//頂点バッファへのポインタ
		NULL);

	VERTEX_3D *pVertex;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntShadow = 0; nCntShadow < SHADOW_MAX; nCntShadow++, pVertex+=4)
	{
		//頂点座標設定
		pVertex[0].pos = D3DXVECTOR3(g_posShadow.x - 10.0f, g_posShadow.y, g_posShadow.z - 10.0f);
		pVertex[1].pos = D3DXVECTOR3(g_posShadow.x - 10.0f, g_posShadow.y, g_posShadow.z + 10.0f);
		pVertex[2].pos = D3DXVECTOR3(g_posShadow.x + 10.0f, g_posShadow.y, g_posShadow.z - 10.0f);
		pVertex[3].pos = D3DXVECTOR3(g_posShadow.x + 10.0f, g_posShadow.y, g_posShadow.z + 10.0f);

		//法線ベクトルの設定
		pVertex[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVertex[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVertex[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVertex[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVertex[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVertex[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVertex[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVertex[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

		//テクスチャ座標の設定
		pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();

	//SetShadow(D3DXVECTOR3(0.0f, 0.0f, 0.0f), NULL, NULL);
	//SetShadow(D3DXVECTOR3(25.0f, 0.0f, 0.0f), NULL, NULL);
	g_aShadowNum[0] = SetShadow(D3DXVECTOR3(0.0f, 0.0f, 0.0f), NULL, NULL);

	return S_OK;
}

//-----------------------------------------------------------------------------
// 影の終了処理
//-----------------------------------------------------------------------------
void UninitShadow(void)
{
	//テクスチャの破棄
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//-----------------------------------------------------------------------------
// 影の更新処理
//-----------------------------------------------------------------------------
void UpdateShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	Player *pPlayer = GetPlayer();
	D3DXVECTOR3 playerBodyPos;
	
	playerBodyPos = D3DXVECTOR3(pPlayer->aModel[0].mtxWorld._41, 0.0f, pPlayer->aModel[0].mtxWorld._43);
	

	SetPositionShadow(g_aShadowNum[0], playerBodyPos);
}

//-----------------------------------------------------------------------------
// 影の描画処理
//-----------------------------------------------------------------------------
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;		//計算用マトリックス

	//デバイス取得
	pDevice = GetDevice();

	for (int nCntShadow = 0; nCntShadow < SHADOW_MAX; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == true)
		{
			//減算合成の設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			//ワールドマトリックスの初期化
			//D3DXMatrixIdentity(&g_mtxWorldShadow);
			D3DXMatrixIdentity(&g_aShadow[nCntShadow].mtxWorld);

			//向きを反映
			//D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotShadow.y, g_rotShadow.x, g_rotShadow.z);
			//D3DXMatrixMultiply(&g_mtxWorldShadow, &g_mtxWorldShadow, &mtxRot);
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxRot);

			//位置を反映
			//D3DXMatrixTranslation(&mtxTrans, g_posShadow.x, g_posShadow.y, g_posShadow.z);
			//D3DXMatrixMultiply(&g_mtxWorldShadow, &g_mtxWorldShadow, &mtxTrans);
			D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCntShadow].pos.x, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			//pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldShadow);
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCntShadow].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureShadow);

			//ポリゴンの描画
			//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntShadow * 4, 2);

			//通常合成に戻す
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
	}
}

//-----------------------------------------------------------------------------
// 影の設定
//-----------------------------------------------------------------------------
int SetShadow(D3DXVECTOR3 pos, float fWidth, float fDepth)
{
	int nIdx = -1;

	for (int nCntShadow = 0; nCntShadow < SHADOW_MAX; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == false)
		{
			//影の位置
			g_aShadow[nCntShadow].pos = pos;

			//影の幅

			//影の深さ

			//
			g_aShadow[nCntShadow].bUse = true;

			//影の番号割り振り
			nIdx = nCntShadow;
			break;
		}
	}

	return nIdx;
}

//-----------------------------------------------------------------------------
// 影の位置移動
//-----------------------------------------------------------------------------
void SetPositionShadow(int nIdx, D3DXVECTOR3 pos)
{
	g_aShadow[nIdx].pos = pos;
}

//-----------------------------------------------------------------------------
// 影を消す
//-----------------------------------------------------------------------------
void RemoveShadow(int nIdx)
{
}