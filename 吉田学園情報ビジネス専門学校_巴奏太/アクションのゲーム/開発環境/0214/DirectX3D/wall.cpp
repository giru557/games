//-----------------------------------------------------------------------------
//
// 壁の処理 [wall.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "wall.h"
#include "input.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define SPLIT_U (4.0f)
#define SPLIT_V (3.0f)
#define WALL_VERTICES (9)
//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
Wall g_aWall[WALL_MAX];															// 壁の情報
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;										// 壁のテクスチャ
D3DXMATRIX g_mtxWorldWall;														// ワールドマトリックス

//-----------------------------------------------------------------------------
// ポリゴンの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitWall(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\skybox.png", &g_pTextureWall);

	for (int nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{
		g_aWall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCntWall].bUse = false;


		//頂点バッファ生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * WALL_VERTICES * WALL_MAX,		//サイズ
			D3DUSAGE_WRITEONLY,									//
			FVF_VERTEX_3D,										//フォーマット
			D3DPOOL_MANAGED,									//
			&g_aWall[nCntWall].pVtx,									//頂点バッファへのポインタ
			NULL);

		VERTEX_3D *pVertex;

		//頂点バッファをロックし頂点情報へのポインタを取得
		g_aWall[nCntWall].pVtx->Lock(0, 0, (void**)&pVertex, 0);

		for (int nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
		{
			//頂点座標設定 (左奥から右手前に順番)
			pVertex[0].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x - (WALL_WIDTH / 2), g_aWall[nCntWall].pos.y + WALL_HEIGHT, g_aWall[nCntWall].pos.z);
			pVertex[1].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y + WALL_HEIGHT, g_aWall[nCntWall].pos.z);
			pVertex[2].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x + (WALL_WIDTH / 2), g_aWall[nCntWall].pos.y + WALL_HEIGHT, g_aWall[nCntWall].pos.z);
			pVertex[3].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x - (WALL_WIDTH / 2), g_aWall[nCntWall].pos.y + (WALL_HEIGHT / 2), g_aWall[nCntWall].pos.z);
			pVertex[4].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y + (WALL_HEIGHT / 2), g_aWall[nCntWall].pos.z);
			pVertex[5].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x + (WALL_WIDTH / 2), g_aWall[nCntWall].pos.y + (WALL_HEIGHT / 2), g_aWall[nCntWall].pos.z);
			pVertex[6].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x - (WALL_WIDTH / 2), g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
			pVertex[7].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
			pVertex[8].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x + (WALL_WIDTH / 2), g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);

			for (int nCnt = 0; nCnt < 9; nCnt++)
			{
				//法線の設定
				pVertex[nCnt].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

				//頂点カラーの設定
				pVertex[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
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
		}

		//頂点バッファをアンロックする
		g_aWall[nCntWall].pVtx->Unlock();

		//インデックスバッファ生成
		pDevice->CreateIndexBuffer(
			sizeof(WORD) * 14,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&g_aWall[nCntWall].pIdxBuff,
			NULL);

		WORD *pIdx;	// インデックス情報へのポインタ

		//インデックスバッファをロックし、番号データへのポインタを取得
		g_aWall[nCntWall].pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		//番号データ設定 (一筆書きになる順番)
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

		//インデックスバッファをアンロック
		g_aWall[nCntWall].pIdxBuff->Unlock();
	}

	SetWall(D3DXVECTOR3(0.0f, -500.0f, 1000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 2000, 2000);
	SetWall(D3DXVECTOR3(-1000.0f, -500.0f, 0.0f), D3DXVECTOR3(0.0f, D3DXToRadian(-90.0f), 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 2000, 2000);
	SetWall(D3DXVECTOR3(1000.0f, -500.0f, 0.0f), D3DXVECTOR3(0.0f, D3DXToRadian(90.0f), 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 2000, 2000);
	SetWall(D3DXVECTOR3(0.0f, -500.0f, -1000.0f), D3DXVECTOR3(0.0f, D3DXToRadian(180.0f), 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 2000, 2000);
	return S_OK;
}

//-----------------------------------------------------------------------------
// ポリゴンの終了処理
//-----------------------------------------------------------------------------
void UninitWall(void)
{
	//テクスチャの破棄
	//if (g_pTextureWall != NULL)
	//{
	//	g_pTextureWall->Release();
	//	g_pTextureWall = NULL;
	//}

	for (int nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{
		//頂点バッファの破棄
		if (g_aWall[nCntWall].pVtx != NULL)
		{
			g_aWall[nCntWall].pVtx->Release();
			g_aWall[nCntWall].pVtx = NULL;
		}

		//インデックスバッファの破棄
		if (g_aWall[nCntWall].pIdxBuff != NULL)
		{
			g_aWall[nCntWall].pIdxBuff->Release();
			g_aWall[nCntWall].pIdxBuff = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
// ポリゴンの更新処理
//-----------------------------------------------------------------------------
void UpdateWall(void)
{
	for (int nCntWall = 0; nCntWall < 4; nCntWall++)
	{
		VERTEX_3D *pVertex;

		//頂点バッファをロックし頂点情報へのポインタを取得
		g_aWall[nCntWall].pVtx->Lock(0, 0, (void**)&pVertex, 0);

		switch (nCntWall)
		{
		case 0:
			//正面のテクスチャ
			pVertex[0].tex = D3DXVECTOR2(1.0f / SPLIT_U, 1.0f / SPLIT_V);
			pVertex[1].tex = D3DXVECTOR2(1.5f / SPLIT_U, 1.0f / SPLIT_V);
			pVertex[2].tex = D3DXVECTOR2(2.0f / SPLIT_U, 1.0f / SPLIT_V);
			pVertex[3].tex = D3DXVECTOR2(1.0f / SPLIT_U, 1.5f / SPLIT_V);
			pVertex[4].tex = D3DXVECTOR2(1.5f / SPLIT_U, 1.5f / SPLIT_V);
			pVertex[5].tex = D3DXVECTOR2(2.0f / SPLIT_U, 1.5f / SPLIT_V);
			pVertex[6].tex = D3DXVECTOR2(1.0f / SPLIT_U, 2.0f / SPLIT_V);
			pVertex[7].tex = D3DXVECTOR2(1.5f / SPLIT_U, 2.0f / SPLIT_V);
			pVertex[8].tex = D3DXVECTOR2(2.0f / SPLIT_U, 2.0f / SPLIT_V);
			break;
		
		case 1:
			//左のテクスチャ
			pVertex[0].tex = D3DXVECTOR2(0.0f / SPLIT_U, 1.0f / SPLIT_V);
			pVertex[1].tex = D3DXVECTOR2(0.5f / SPLIT_U, 1.0f / SPLIT_V);
			pVertex[2].tex = D3DXVECTOR2(1.0f / SPLIT_U, 1.0f / SPLIT_V);
			pVertex[3].tex = D3DXVECTOR2(0.0f / SPLIT_U, 1.5f / SPLIT_V);
			pVertex[4].tex = D3DXVECTOR2(0.5f / SPLIT_U, 1.5f / SPLIT_V);
			pVertex[5].tex = D3DXVECTOR2(1.0f / SPLIT_U, 1.5f / SPLIT_V);
			pVertex[6].tex = D3DXVECTOR2(0.5f / SPLIT_U, 2.0f / SPLIT_V);
			pVertex[7].tex = D3DXVECTOR2(1.0f / SPLIT_U, 2.0f / SPLIT_V);
			pVertex[8].tex = D3DXVECTOR2(1.5f / SPLIT_U, 2.0f / SPLIT_V);
			break;

		case 2:
			//右のテクスチャ
			pVertex[0].tex = D3DXVECTOR2(2.0f / SPLIT_U, 1.0f / SPLIT_V);
			pVertex[1].tex = D3DXVECTOR2(2.5f / SPLIT_U, 1.0f / SPLIT_V);
			pVertex[2].tex = D3DXVECTOR2(3.0f / SPLIT_U, 1.0f / SPLIT_V);
			pVertex[3].tex = D3DXVECTOR2(2.0f / SPLIT_U, 1.5f / SPLIT_V);
			pVertex[4].tex = D3DXVECTOR2(2.5f / SPLIT_U, 1.5f / SPLIT_V);
			pVertex[5].tex = D3DXVECTOR2(3.0f / SPLIT_U, 1.5f / SPLIT_V);
			pVertex[6].tex = D3DXVECTOR2(2.0f / SPLIT_U, 2.0f / SPLIT_V);
			pVertex[7].tex = D3DXVECTOR2(2.5f / SPLIT_U, 2.0f / SPLIT_V);
			pVertex[8].tex = D3DXVECTOR2(3.0f / SPLIT_U, 2.0f / SPLIT_V);
			break;

		case 3:
			//右のテクスチャ
			pVertex[0].tex = D3DXVECTOR2(3.0f / SPLIT_U, 1.0f / SPLIT_V);
			pVertex[1].tex = D3DXVECTOR2(3.5f / SPLIT_U, 1.0f / SPLIT_V);
			pVertex[2].tex = D3DXVECTOR2(4.0f / SPLIT_U, 1.0f / SPLIT_V);
			pVertex[3].tex = D3DXVECTOR2(3.0f / SPLIT_U, 1.5f / SPLIT_V);
			pVertex[4].tex = D3DXVECTOR2(3.5f / SPLIT_U, 1.5f / SPLIT_V);
			pVertex[5].tex = D3DXVECTOR2(4.0f / SPLIT_U, 1.5f / SPLIT_V);
			pVertex[6].tex = D3DXVECTOR2(3.0f / SPLIT_U, 2.0f / SPLIT_V);
			pVertex[7].tex = D3DXVECTOR2(3.5f / SPLIT_U, 2.0f / SPLIT_V);
			pVertex[8].tex = D3DXVECTOR2(4.0f / SPLIT_U, 2.0f / SPLIT_V);
			break;

		default:
			break;
		}


		//インデックスバッファをアンロック
		g_aWall[nCntWall].pVtx->Unlock();
	}
}

//-----------------------------------------------------------------------------
// ポリゴンの描画処理
//-----------------------------------------------------------------------------
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;		//計算用マトリックス

	//デバイス取得
	pDevice = GetDevice();

	for (int nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aWall[nCntWall].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aWall[nCntWall].rot.y, g_aWall[nCntWall].rot.x, g_aWall[nCntWall].rot.z);
			D3DXMatrixMultiply(&g_aWall[nCntWall].mtxWorld, &g_aWall[nCntWall].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
			D3DXMatrixMultiply(&g_aWall[nCntWall].mtxWorld, &g_aWall[nCntWall].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aWall[nCntWall].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_aWall[nCntWall].pVtx, 0, sizeof(VERTEX_3D));

			//インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_aWall[nCntWall].pIdxBuff);

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureWall);

			//ポリゴンの描画
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,
				0,
				0,
				14,
				0,
				12);
		}
	}
}

//-----------------------------------------------------------------------------
// 壁の情報取得
//-----------------------------------------------------------------------------
Wall *GetWall(void)
{
	return &g_aWall[0];
}

//-----------------------------------------------------------------------------
// 壁の設置
//-----------------------------------------------------------------------------
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 nor, float fWidth, float fHeight)
{
	VERTEX_3D *pVertex;

	for (int nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == false)
		{
			//頂点バッファをロックし頂点情報へのポインタを取得
			g_aWall[nCntWall].pVtx->Lock(0, 0, (void**)&pVertex, 0);

			//頂点座標設定 (左奥から右手前に順番)
			pVertex[0].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x - (fWidth / 2), g_aWall[nCntWall].pos.y + fHeight, g_aWall[nCntWall].pos.z);
			pVertex[1].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y + fHeight, g_aWall[nCntWall].pos.z);
			pVertex[2].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x + (fWidth / 2), g_aWall[nCntWall].pos.y + fHeight, g_aWall[nCntWall].pos.z);
			pVertex[3].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x - (fWidth / 2), g_aWall[nCntWall].pos.y + (fHeight / 2), g_aWall[nCntWall].pos.z);
			pVertex[4].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y + (fHeight / 2), g_aWall[nCntWall].pos.z);
			pVertex[5].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x + (fWidth / 2), g_aWall[nCntWall].pos.y + (fHeight / 2), g_aWall[nCntWall].pos.z);
			pVertex[6].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x - (fWidth / 2), g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
			pVertex[7].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
			pVertex[8].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x + (fWidth / 2), g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);


			for (int nCnt = 0; nCnt < 9; nCnt++)
			{
				//法線の設定
				pVertex[nCnt].nor = nor;
			}

			//頂点バッファアンロック
			g_aWall[nCntWall].pVtx->Unlock();

			//位置の設定
			g_aWall[nCntWall].pos = pos;

			//回転の設定
			g_aWall[nCntWall].rot = rot;

			//幅設定
			g_aWall[nCntWall].fWidth = fWidth;

			//高さ設定
			g_aWall[nCntWall].fHeight = fHeight;



			//使用中に変更
			g_aWall[nCntWall].bUse = true;
			break;
		}
	}
}