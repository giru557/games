//-----------------------------------------------------------------------------
//
// ビルボードの処理 [billboard.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "billboard.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffBillboard = NULL;									// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureBillboard = NULL;											// ポリゴンのテクスチャへのポインタ
D3DXVECTOR3 g_posBillboard;																// 位置
D3DXMATRIX g_mtxWorldBillboard;															// ワールドマトリックス

//-----------------------------------------------------------------------------
// ビルボードの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	g_posBillboard = D3DXVECTOR3(0.0f, 50, 25.0f);		// 位置

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\bullet000.png" , &g_pTextureBillboard);

	//頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VERTEX_AMOUNT,					//サイズ
								D3DUSAGE_WRITEONLY,									//
								FVF_VERTEX_3D,										//フォーマット
								D3DPOOL_MANAGED,									//
								&g_pVtxBuffBillboard,									//頂点バッファへのポインタ
								NULL);

	VERTEX_3D *pVertex;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVertex, 0);

	//頂点座標設定
	pVertex[0].pos = D3DXVECTOR3(g_posBillboard.x - BILLBOARD_WIDTH, g_posBillboard.y, g_posBillboard.z);
	pVertex[1].pos = D3DXVECTOR3(g_posBillboard.x - BILLBOARD_WIDTH, g_posBillboard.y + BILLBOARD_HEIGHT, g_posBillboard.z);
	pVertex[2].pos = D3DXVECTOR3(g_posBillboard.x + BILLBOARD_WIDTH, g_posBillboard.y, g_posBillboard.z);
	pVertex[3].pos = D3DXVECTOR3(g_posBillboard.x + BILLBOARD_WIDTH, g_posBillboard.y + BILLBOARD_HEIGHT, g_posBillboard.z);

	//法線ベクトルの設定
	pVertex[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVertex[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVertex[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVertex[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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
	g_pVtxBuffBillboard->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
// ビルボードの終了処理
//-----------------------------------------------------------------------------
void UninitBillboard(void)
{
	//テクスチャの破棄
	if (g_pTextureBillboard != NULL)
	{
		g_pTextureBillboard->Release();
		g_pTextureBillboard = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//-----------------------------------------------------------------------------
// ポリゴンの更新処理
//-----------------------------------------------------------------------------
void UpdateBillboard(void)
{

}

//-----------------------------------------------------------------------------
// ビルボードの描画処理
//-----------------------------------------------------------------------------
void DrawBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスへのポインタ
	D3DXMATRIX mtxView;					// ビューマトリックス取得
	D3DXMATRIX mtxTrans;				// 計算用マトリックス

	//デバイス取得
	pDevice = GetDevice();

	//比較をやめて常に手前に描画
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldBillboard);

	//ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&g_mtxWorldBillboard, NULL, &mtxView);
	g_mtxWorldBillboard._41 = 0.0f;
	g_mtxWorldBillboard._42 = 0.0f;
	g_mtxWorldBillboard._43 = 0.0f;

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posBillboard.x, g_posBillboard.y, g_posBillboard.z);
	D3DXMatrixMultiply(&g_mtxWorldBillboard, &g_mtxWorldBillboard, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBillboard);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBillboard);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//初期値に戻す
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

}