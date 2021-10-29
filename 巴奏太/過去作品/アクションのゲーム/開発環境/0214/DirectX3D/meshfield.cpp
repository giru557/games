//-----------------------------------------------------------------------------
//
// 地面の処理 [meshfield.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "meshfield.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define MESHFIELD_14 (14)
#define MESHFIELD_DIS (100)
#define MESHFIELD_ROW (2)
#define MESHFIELD_COLUMN (2)

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffMeshfield = NULL;				// 頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshfield = NULL;					// インデックスバッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureMeshfield = NULL;						// ポリゴンのテクスチャへのポインタ
D3DXVECTOR3 g_posMeshfield;											// 位置
D3DXVECTOR3 g_rotMeshfield;											// 向き
D3DXMATRIX g_mtxWorldMeshfield;										// ワールドマトリックス

//-----------------------------------------------------------------------------
// 地面の初期化処理
//-----------------------------------------------------------------------------
HRESULT InitMeshfield(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	g_posMeshfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	g_rotMeshfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\ground.jpg" , &g_pTextureMeshfield);

	//頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 9,					//サイズ
								D3DUSAGE_WRITEONLY,						//
								FVF_VERTEX_3D,							//フォーマット
								D3DPOOL_MANAGED,						//
								&g_pVtxBuffMeshfield,					//頂点バッファへのポインタ
								NULL);

	VERTEX_3D *pVertex;
	
	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffMeshfield->Lock(0, 0, (void**)&pVertex, 0);

	//頂点座標設定
	pVertex[0].pos = D3DXVECTOR3(g_posMeshfield.x - MESHFIELD_DIS, g_posMeshfield.y, g_posMeshfield.z + MESHFIELD_DIS);
	pVertex[1].pos = D3DXVECTOR3(g_posMeshfield.x, g_posMeshfield.y, g_posMeshfield.z + MESHFIELD_DIS);
	pVertex[2].pos = D3DXVECTOR3(g_posMeshfield.x + MESHFIELD_DIS, g_posMeshfield.y, g_posMeshfield.z + MESHFIELD_DIS);
	pVertex[3].pos = D3DXVECTOR3(g_posMeshfield.x - MESHFIELD_DIS, g_posMeshfield.y, g_posMeshfield.z);
	pVertex[4].pos = D3DXVECTOR3(g_posMeshfield.x, g_posMeshfield.y, g_posMeshfield.z);
	pVertex[5].pos = D3DXVECTOR3(g_posMeshfield.x + MESHFIELD_DIS, g_posMeshfield.y, g_posMeshfield.z);
	pVertex[6].pos = D3DXVECTOR3(g_posMeshfield.x - MESHFIELD_DIS, g_posMeshfield.y, g_posMeshfield.z - MESHFIELD_DIS);
	pVertex[7].pos = D3DXVECTOR3(g_posMeshfield.x, g_posMeshfield.y, g_posMeshfield.z - MESHFIELD_DIS);
	pVertex[8].pos = D3DXVECTOR3(g_posMeshfield.x + MESHFIELD_DIS, g_posMeshfield.y, g_posMeshfield.z - MESHFIELD_DIS);

	for (int nCntField = 0; nCntField < 9; nCntField++)
	{
		//法線ベクトルの設定
		pVertex[nCntField].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVertex[nCntField].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	//テクスチャ座標の設定
	pVertex[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVertex[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVertex[2].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVertex[3].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVertex[4].tex = D3DXVECTOR2(0.5f, 0.5f);
	pVertex[5].tex = D3DXVECTOR2(1.0f, 0.5f);
	pVertex[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVertex[7].tex = D3DXVECTOR2(0.5f, 1.0f);
	pVertex[8].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffMeshfield->Unlock();

	//インデックスバッファ生成
	pDevice->CreateIndexBuffer(
		sizeof(WORD) * 14,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshfield,
		NULL);

	WORD *pIdx;		// インデックス情報へのポインタ

	//インデックスバッファをロックし、番号データへのポインタを取得
	g_pIdxBuffMeshfield->Lock(0, 0, (void**)&pIdx, 0);

	//番号データの設定
	pIdx[0] = 3;
	pIdx[1] = 0;
	pIdx[2] = 4;
	pIdx[3] = 1;
	pIdx[4] = 5;
	pIdx[5] = 2;
	pIdx[6] = 2;
	pIdx[7] = 6;
	pIdx[8] = 6;
	pIdx[9] = 3;
	pIdx[10] = 7;
	pIdx[11] = 4;
	pIdx[12] = 8;
	pIdx[13] = 5;

	//インデックスバッファをアンロックする
	g_pIdxBuffMeshfield->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
// 地面の終了処理
//-----------------------------------------------------------------------------
void UninitMeshfield(void)
{
	//テクスチャの破棄
	if (g_pTextureMeshfield != NULL)
	{
		g_pTextureMeshfield->Release();
		g_pTextureMeshfield = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffMeshfield != NULL)
	{
		g_pVtxBuffMeshfield->Release();
		g_pVtxBuffMeshfield = NULL;
	}

	//インデックスバッファの破棄
	if (g_pIdxBuffMeshfield != NULL)
	{
		g_pIdxBuffMeshfield->Release();
		g_pIdxBuffMeshfield = NULL;
	}
}

//-----------------------------------------------------------------------------
// 地面の更新処理
//-----------------------------------------------------------------------------
void UpdateMeshfield(void)
{
}

//-----------------------------------------------------------------------------
// 地面の描画処理
//-----------------------------------------------------------------------------
void DrawMeshfield(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;		//計算用マトリックス

	//デバイス取得
	pDevice = GetDevice();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldMeshfield);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMeshfield.y, g_rotMeshfield.x, g_rotMeshfield.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshfield, &g_mtxWorldMeshfield, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posMeshfield.x, g_posMeshfield.y, g_posMeshfield.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshfield, &g_mtxWorldMeshfield, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshfield);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshfield, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshfield);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshfield);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		14,
		0,
		12);

}