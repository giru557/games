//-----------------------------------------------------------------------------
//
// ポリゴンの処理 [polygon.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "polygon.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffPolygon = NULL;					// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTexturePolygon = NULL;						// ポリゴンのテクスチャへのポインタ
D3DXVECTOR3 g_posPolygon;											// 位置
D3DXVECTOR3 g_rotPolygon;											// 向き
D3DXMATRIX g_mtxWorldPolygon;										// ワールドマトリックス

//-----------------------------------------------------------------------------
// ポリゴンの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	g_posPolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	g_rotPolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\ground.jpg" , &g_pTexturePolygon);

	//頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VERTEX_AMOUNT,		//サイズ
								D3DUSAGE_WRITEONLY,						//
								FVF_VERTEX_3D,							//フォーマット
								D3DPOOL_MANAGED,						//
								&g_pVtxBuffPolygon,						//頂点バッファへのポインタ
								NULL);

	VERTEX_3D *pVertex;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVertex, 0);

	//頂点座標設定
	pVertex[0].pos = D3DXVECTOR3(g_posPolygon.x - 100.0f, g_posPolygon.y, g_posPolygon.z - 100.0f);
	pVertex[1].pos = D3DXVECTOR3(g_posPolygon.x - 100.0f, g_posPolygon.y, g_posPolygon.z + 100.0f);
	pVertex[2].pos = D3DXVECTOR3(g_posPolygon.x + 100.0f, g_posPolygon.y, g_posPolygon.z - 100.0f);
	pVertex[3].pos = D3DXVECTOR3(g_posPolygon.x + 100.0f, g_posPolygon.y, g_posPolygon.z + 100.0f);

	//法線ベクトルの設定
	pVertex[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVertex[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVertex[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVertex[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラーの設定
	pVertex[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVertex[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVertex[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVertex[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffPolygon->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
// ポリゴンの終了処理
//-----------------------------------------------------------------------------
void UninitPolygon(void)
{
	//テクスチャの破棄
	if (g_pTexturePolygon != NULL)
	{
		g_pTexturePolygon->Release();
		g_pTexturePolygon = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}
}

//-----------------------------------------------------------------------------
// ポリゴンの更新処理
//-----------------------------------------------------------------------------
void UpdatePolygon(void)
{

}

//-----------------------------------------------------------------------------
// ポリゴンの描画処理
//-----------------------------------------------------------------------------
void DrawPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;		//計算用マトリックス

	//デバイス取得
	pDevice = GetDevice();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldPolygon);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotPolygon.y, g_rotPolygon.x, g_rotPolygon.z);
	D3DXMatrixMultiply(&g_mtxWorldPolygon, &g_mtxWorldPolygon, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posPolygon.x, g_posPolygon.y, g_posPolygon.z);
	D3DXMatrixMultiply(&g_mtxWorldPolygon, &g_mtxWorldPolygon, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldPolygon);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePolygon);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}