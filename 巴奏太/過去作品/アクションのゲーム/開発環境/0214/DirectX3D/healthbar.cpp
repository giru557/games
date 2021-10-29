//-----------------------------------------------------------------------------
//
// HPバーの処理 [healthbar.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "healthbar.h"

#include "input.h"
#include "enemy.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
void ShowHealthBarAtEnemy(void);

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
HealthBar g_aHealthBar[ENEMY_AMOUNT_MAX][HPBAR_LAYER];	// HPバーの情報
float g_fHPRatio = 1.0f;								// HPの割合

//-----------------------------------------------------------------------------
// HPバーの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitHP(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス取得

	g_fHPRatio = 1.0f;	// HPの割合

	//敵の数だけ用意する
	for (int nCntEnemy = 0; nCntEnemy < ENEMY_AMOUNT_MAX; nCntEnemy++)
	{
		/* ------HPバーの後ろ------ */
		VERTEX_3D *pVertexBack;

		//位置
		g_aHealthBar[nCntEnemy][0].pos = D3DXVECTOR3(0.0f, 50, 0.0f);

		//使用状況
		g_aHealthBar[nCntEnemy][0].bUse = false;

		//テクスチャ読み込み
		D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\healthbar_back.png", &g_aHealthBar[nCntEnemy][0].pTexture);

		//頂点バッファ生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VERTEX_AMOUNT,	//サイズ
			D3DUSAGE_WRITEONLY,											//
			FVF_VERTEX_3D,												//フォーマット
			D3DPOOL_MANAGED,											//
			&g_aHealthBar[nCntEnemy][0].pVtxBuff,									//頂点バッファへのポインタ
			NULL);

		//頂点バッファをロックし頂点情報へのポインタを取得
		g_aHealthBar[nCntEnemy][0].pVtxBuff->Lock(0, 0, (void**)&pVertexBack, 0);

		//頂点座標設定
		pVertexBack[0].pos = D3DXVECTOR3(-HPBAR_WIDTH / 2, -HPBAR_HEIGHT / 2, 0.0f);
		pVertexBack[1].pos = D3DXVECTOR3(-HPBAR_WIDTH / 2, HPBAR_HEIGHT / 2, 0.0f);
		pVertexBack[2].pos = D3DXVECTOR3(HPBAR_WIDTH / 2, -HPBAR_HEIGHT / 2, 0.0f);
		pVertexBack[3].pos = D3DXVECTOR3(HPBAR_WIDTH / 2, HPBAR_HEIGHT / 2, 0.0f);

		//法線ベクトルの設定
		pVertexBack[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVertexBack[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVertexBack[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVertexBack[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//頂点カラーの設定
		pVertexBack[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVertexBack[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVertexBack[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVertexBack[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVertexBack[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVertexBack[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVertexBack[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVertexBack[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		//頂点バッファをアンロックする
		g_aHealthBar[nCntEnemy][0].pVtxBuff->Unlock();


		/* ------HPバーの前------ */
		VERTEX_3D *pVertexFront;

		//位置
		g_aHealthBar[nCntEnemy][1].pos = g_aHealthBar[nCntEnemy][0].pos;

		//使用状況
		g_aHealthBar[nCntEnemy][1].bUse = false;

		//テクスチャ読み込み
		D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\healthbar_front.png", &g_aHealthBar[nCntEnemy][1].pTexture);

		//頂点バッファ生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VERTEX_AMOUNT,	//サイズ
			D3DUSAGE_WRITEONLY,											//
			FVF_VERTEX_3D,												//フォーマット
			D3DPOOL_MANAGED,											//
			&g_aHealthBar[nCntEnemy][1].pVtxBuff,									//頂点バッファへのポインタ
			NULL);

		//頂点バッファをロックし頂点情報へのポインタを取得
		g_aHealthBar[nCntEnemy][1].pVtxBuff->Lock(0, 0, (void**)&pVertexFront, 0);

		//頂点座標設定
		pVertexFront[0].pos = D3DXVECTOR3(-HPBAR_WIDTH / 2, -HPBAR_HEIGHT / 2, 0.0f);
		pVertexFront[1].pos = D3DXVECTOR3(-HPBAR_WIDTH / 2, HPBAR_HEIGHT / 2, 0.0f);
		pVertexFront[2].pos = D3DXVECTOR3(HPBAR_WIDTH / 2, -HPBAR_HEIGHT / 2, 0.0f);
		pVertexFront[3].pos = D3DXVECTOR3(HPBAR_WIDTH / 2, HPBAR_HEIGHT / 2, 0.0f);

		//法線ベクトルの設定
		pVertexFront[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVertexFront[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVertexFront[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVertexFront[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//頂点カラーの設定
		pVertexFront[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVertexFront[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVertexFront[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVertexFront[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVertexFront[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVertexFront[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVertexFront[2].tex = D3DXVECTOR2(0.5f, 1.0f);
		pVertexFront[3].tex = D3DXVECTOR2(0.5f, 0.0f);

		//頂点バッファをアンロックする
		g_aHealthBar[nCntEnemy][1].pVtxBuff->Unlock();
	}

	for (int nCntLayer = 0; nCntLayer < HPBAR_LAYER; nCntLayer++)
	{
		g_aHealthBar[0][nCntLayer].pos = D3DXVECTOR3(0.0f, 50.0f, 0.0f);
		g_aHealthBar[0][nCntLayer].bUse = true;

		g_aHealthBar[1][nCntLayer].pos = D3DXVECTOR3(50.0f, 50.0f, 0.0f);
		g_aHealthBar[1][nCntLayer].bUse = true;
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// HPバーの終了処理
//-----------------------------------------------------------------------------
void UninitHP(void)
{
	//敵の数だけ破棄する
	for (int nCntEnemy = 0; nCntEnemy < ENEMY_AMOUNT_MAX; nCntEnemy++)
	{
		for (int nCntLayer = 0; nCntLayer < HPBAR_LAYER; nCntLayer++)
		{
			//テクスチャの破棄
			if (g_aHealthBar[nCntEnemy][nCntLayer].pTexture != NULL)
			{
				g_aHealthBar[nCntEnemy][nCntLayer].pTexture->Release();
				g_aHealthBar[nCntEnemy][nCntLayer].pTexture = NULL;
			}

			//頂点バッファの破棄
			if (g_aHealthBar[nCntEnemy][nCntLayer].pVtxBuff != NULL)
			{
				g_aHealthBar[nCntEnemy][nCntLayer].pVtxBuff->Release();
				g_aHealthBar[nCntEnemy][nCntLayer].pVtxBuff = NULL;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// HPバー更新処理
//-----------------------------------------------------------------------------
void UpdateHP(void)
{
	VERTEX_3D *pVertexFront;	// 3Dフォーマット
	Enemy *pEnemy = GetEnemy();	// 敵の取得

	//敵にHPバーくっつける
	ShowHealthBarAtEnemy();

	//敵の分回す
	for (int nCntEnemy = 0; nCntEnemy < ENEMY_AMOUNT_MAX; nCntEnemy++)
	{
		//使用中の場合
		if (pEnemy[nCntEnemy].bUse == true)
		{
			//体力の割合
			g_fHPRatio = (float)pEnemy[nCntEnemy].nLife / (float)pEnemy[nCntEnemy].nLifeMax;

			//頂点バッファをロックし頂点情報へのポインタを取得
			g_aHealthBar[nCntEnemy][1].pVtxBuff->Lock(0, 0, (void**)&pVertexFront, 0);

			//頂点座標設定
			pVertexFront[0].pos = D3DXVECTOR3((-HPBAR_WIDTH / 2) * g_fHPRatio, -HPBAR_HEIGHT / 2, 0.0f);
			pVertexFront[1].pos = D3DXVECTOR3((-HPBAR_WIDTH / 2) * g_fHPRatio, HPBAR_HEIGHT / 2, 0.0f);
			pVertexFront[2].pos = D3DXVECTOR3((HPBAR_WIDTH / 2) * g_fHPRatio, -HPBAR_HEIGHT / 2, 0.0f);
			pVertexFront[3].pos = D3DXVECTOR3((HPBAR_WIDTH / 2) * g_fHPRatio, HPBAR_HEIGHT / 2, 0.0f);

			//頂点バッファをアンロックする
			g_aHealthBar[nCntEnemy][1].pVtxBuff->Unlock();
		}
	}
}

//-----------------------------------------------------------------------------
// HPバーの描画処理
//-----------------------------------------------------------------------------
void DrawHP(void)
{
	//敵の数回す
	for (int nCntEnemy = 0; nCntEnemy < ENEMY_AMOUNT_MAX; nCntEnemy++)
	{
		//レイヤーの数回す
		for (int nCntLayer = 0; nCntLayer < HPBAR_LAYER; nCntLayer++)
		{
			//使われていたら描画
			if (g_aHealthBar[nCntEnemy][nCntLayer].bUse == true)
			{
				LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスへのポインタ
				D3DXMATRIX mtxView;									// ビューマトリックス取得
				D3DXMATRIX mtxTrans;								// 計算用マトリックス

				//比較をやめて常に手前に描画
				pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

				//ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_aHealthBar[nCntEnemy][nCntLayer].mtxWorld);

				//ビューマトリックスを取得
				pDevice->GetTransform(D3DTS_VIEW, &mtxView);

				//ポリゴンをカメラに対して正面に向ける
				D3DXMatrixInverse(&g_aHealthBar[nCntEnemy][nCntLayer].mtxWorld, NULL, &mtxView);
				g_aHealthBar[nCntEnemy][nCntLayer].mtxWorld._41 = 0.0f;
				g_aHealthBar[nCntEnemy][nCntLayer].mtxWorld._42 = 0.0f;
				g_aHealthBar[nCntEnemy][nCntLayer].mtxWorld._43 = 0.0f;

				//位置を反映
				D3DXMatrixTranslation(&mtxTrans, g_aHealthBar[nCntEnemy][nCntLayer].pos.x, g_aHealthBar[nCntEnemy][nCntLayer].pos.y, g_aHealthBar[nCntEnemy][nCntLayer].pos.z);
				D3DXMatrixMultiply(&g_aHealthBar[nCntEnemy][nCntLayer].mtxWorld, &g_aHealthBar[nCntEnemy][nCntLayer].mtxWorld, &mtxTrans);

				//ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_aHealthBar[nCntEnemy][nCntLayer].mtxWorld);

				//頂点バッファをデータストリームに設定
				pDevice->SetStreamSource(0, g_aHealthBar[nCntEnemy][nCntLayer].pVtxBuff, 0, sizeof(VERTEX_3D));

				//頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_aHealthBar[nCntEnemy][nCntLayer].pTexture);

				//ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

				//初期値に戻す
				pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			}
		}
	}
}

void ShowHealthBarAtEnemy(void)
{
	Enemy *pEnemy = GetEnemy();	// 敵の取得

	//敵の数回す
	for (int nCntEnemy = 0; nCntEnemy < ENEMY_AMOUNT_MAX; nCntEnemy++)
	{
		//敵が使用中の場合
		if (pEnemy[nCntEnemy].bUse == true)
		{
			//レイヤー数回す
			for (int nCntLayer = 0; nCntLayer < HPBAR_LAYER; nCntLayer++)
			{
				//バーの位置を敵の位置に
				g_aHealthBar[nCntEnemy][nCntLayer].pos = pEnemy[nCntEnemy].pos;
				//高さ上げる
				g_aHealthBar[nCntEnemy][nCntLayer].pos.y += HPBAR_HEIGHT_FROM_OBJ;

				//使用中に移行
				g_aHealthBar[nCntEnemy][nCntLayer].bUse = true;
			}
		}
		//使用中でない場合
		else if (pEnemy[nCntEnemy].bUse == false)
		{
			//レイヤー数回す
			for (int nCntLayer = 0; nCntLayer < HPBAR_LAYER; nCntLayer++)
			{
				//消す
				g_aHealthBar[nCntEnemy][nCntLayer].bUse = false;
			}
		}
	}
}