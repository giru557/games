#define _CRT_SECURE_NO_WARNINGS
//-----------------------------------------------------------------------------
//
// フィールドの処理 [.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "field.h"
#include "camera.h"
#include "input.h"
#include "shadow.h"
#include "player.h"
#include "enemy.h"
#include <stdio.h>

// マクロ定義
#define RAY_POS_Y (500)	// レイの始点

//
// プロトタイプ宣言
//

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
LPD3DXMESH g_pMeshF = NULL;													// メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER g_pBuffMatF;													// マテリアル(材質)へのポインタ
DWORD g_nNumMatF = 0;														// マテリアルの数
Field g_Field;																	// モデルの情報
D3DXVECTOR3 g_posField;															// 位置
D3DXVECTOR3 g_rotField;															// 向き
D3DXMATRIX g_mtxWorldField;														// ワールドマトリックス
D3DXVECTOR3 g_vtxMinField, g_vtxMaxField;										// モデルの最大値、最小値
LPDIRECT3DTEXTURE9 g_apTextureField[10] = { NULL };								// テクスチャへのポインタ

//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
void InitField(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();														// デバイスの取得

	//Xファイルの読み込み
	D3DXLoadMeshFromX(
		"data\\MODEL\\field1.1.x",			// 読み込むファイル名
		D3DXMESH_SYSTEMMEM,						// 固定
		pDevice,								// デバイス
		NULL,									// NULL固定
		&g_pBuffMatF,						// マテリアル
		NULL,									// NULL固定
		&g_nNumMatF,						// マテリアル数
		&g_pMeshF);							// メッシュ

	//初期位置、向き
	g_Field.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 初期位置
	g_Field.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 初期向き

	D3DXMATERIAL *pMat;	// マテリアルへのポインタ

	//マテリアル情報へのポインタ
	pMat = (D3DXMATERIAL*)g_pBuffMatF->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_nNumMatF; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_apTextureField[nCntMat]);
		}
	}

	srand((unsigned int)time(NULL));	// rand初期化

	//敵の配置
	for (int nCntEnemy = 0; nCntEnemy < 10; nCntEnemy++)
	{
		D3DXVECTOR3 rayPos;
		BOOL bHitGround;
		int nPosX, nPosZ;
		float fDistanceToGround;
		nPosX = rand() % 1001 - 500;
		nPosZ = rand() % 1001 - 500;
		
		//Vector3型に入れる
		rayPos = D3DXVECTOR3((float)nPosX, (float)RAY_POS_Y, (float)nPosZ);
		
		//ランダムな空の位置からレイ飛ばす
		D3DXIntersect(g_pMeshF, &rayPos, &D3DXVECTOR3(0.0f, -1.0f, 0.0f), &bHitGround, NULL, NULL, NULL, &fDistanceToGround, NULL, NULL);

		//レイがあたった地面の場所に敵沸かす
		SetEnemy(D3DXVECTOR3(rayPos.x, RAY_POS_Y - fDistanceToGround, rayPos.z));
	}
}

//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void UninitField(void)
{
	//メッシュの破棄
	if (g_pMeshF != NULL)
	{
		g_pMeshF->Release();
		g_pMeshF = NULL;
	}

	//マテリアルの破棄
	if (g_pBuffMatF != NULL)
	{
		g_pBuffMatF->Release();
		g_pBuffMatF = NULL;
	}
}

//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void UpdateField(void)
{
	Player *pPlayer = GetPlayer();
	Enemy *pEnemy = GetEnemy();
	float fU, fV = { 0.0f };
	float fHitDist[2];
	BOOL bHit[2] = { FALSE, FALSE };
	DWORD dwHitFace = -1;

	//レイとメッシュの接触判定
	D3DXIntersect(g_pMeshF, &pPlayer->pos, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &bHit[0], &dwHitFace, &fU, &fV, &fHitDist[0], NULL, NULL);

	//あたってたら
	if (bHit[0] == 1)
	{
		//距離がプレイヤーの高さ以内だった場合
		if (fHitDist[0] <= PLAYER_HEIGHT)
		{
			//地面に立たせる
			pPlayer->pos.y += fHitDist[0];
			pPlayer->move.y = 0.0f;
		}
	}

	for (int nCntEnemy = 0; nCntEnemy < ENEMY_AMOUNT_MAX; nCntEnemy++)
	{
		//レイとメッシュの接触判定
		D3DXIntersect(g_pMeshF, &pEnemy[nCntEnemy].pos, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &bHit[1], &dwHitFace, &fU, &fV, &fHitDist[1], NULL, NULL);

		if (bHit[1] == 1)
		{
			if (fHitDist[1] <= ENEMY_HEIGHT)
			{
				pEnemy[nCntEnemy].pos.y = pEnemy[nCntEnemy].pos.y + fHitDist[1];
				pEnemy[nCntEnemy].move.y = 0.0f;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void DrawField(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxRot, mtxTrans;												// 計算用マトリックス
	D3DMATERIAL9 matDef;														// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;															// マテリアルデータへのポインタ
	pDevice = GetDevice();														// デバイスの取得

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_Field.mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Field.rot.y, g_Field.rot.x, g_Field.rot.z);
	D3DXMatrixMultiply(&g_Field.mtxWorld, &g_Field.mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_Field.pos.x, g_Field.pos.y, g_Field.pos.z);
	D3DXMatrixMultiply(&g_Field.mtxWorld, &g_Field.mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_Field.mtxWorld);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatF->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_nNumMatF; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureField[nCntMat]);

		//モデルパーツの描画
		g_pMeshF->DrawSubset(nCntMat);
	}

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}