#define _CRT_SECURE_NO_WARNINGS
//-----------------------------------------------------------------------------
//
// モデルの処理 [model.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "model.h"
#include "camera.h"
#include "input.h"
#include "shadow.h"
#include "player.h"
#include <stdio.h>
#include "collision.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define READROW (5000)	// 読み込む行数

//-----------------------------------------------------------------------------
// モデル情報の構造体
//-----------------------------------------------------------------------------
typedef struct
{
	int nNumTexture;		// テクスチャの数
	int nNumModel;			// モデルの数
	char cTextureFile[MODEL_AMOUNT_MAX][128];	// テクスチャファイル名
	char cModelFile[MODEL_AMOUNT_MAX][128];	// モデルファイル名
}ModelLoad;

//-----------------------------------------------------------------------------
// モデル配置情報の構造体
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	int nType;				// モデルのタイプ(番号)
	int nCollision;			// コリジョンの有無
	float fPos[3];			// 位置
	float fRot[3];			// 向き
}ModelSet;

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
void ModelCollision(void);
void ReadModelInfo(void);

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
Model g_aModel[MODEL_AMOUNT_MAX];						// モデルの情報
ModelLoad g_LoadModel;									// モデルの読み込み情報
ModelSet g_aModelSetInfo[MODEL_AMOUNT_MAX];				// モデルの設置情報
int g_nCountModelSet = 0;

//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
void InitModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス取得

	//初期化
	g_nCountModelSet = 0;
	for (int nCntModel = 0; nCntModel < MODEL_AMOUNT_MAX; nCntModel++)
	{
		g_aModel[nCntModel].bUse = false;
		g_aModel[nCntModel].dwNumMat = 0;
		g_aModel[nCntModel].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aModel[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aModel[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aModel[nCntModel].vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aModel[nCntModel].vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		for (int nCntVector3 = 0; nCntVector3 < 3; nCntVector3++)
		{
			g_aModelSetInfo[nCntModel].fPos[nCntVector3] = 0.0f;
			g_aModelSetInfo[nCntModel].fPos[nCntVector3] = 0.0f;
		}
		g_aModelSetInfo[nCntModel].nCollision = 0;
		g_aModelSetInfo[nCntModel].nType = 0;

		g_LoadModel.cModelFile[nCntModel][0] = '\0';
		g_LoadModel.cTextureFile[nCntModel][0] = '\0';
	}
	g_LoadModel.nNumModel = 0;
	g_LoadModel.nNumTexture = 0;

	//モデルの情報読み取り
	ReadModelInfo();

	//モデルのロード
	for (int nCntModel = 0; nCntModel < g_LoadModel.nNumModel; nCntModel++)
	{
		//Xファイルの読み込み
		D3DXLoadMeshFromX(
			&g_LoadModel.cModelFile[nCntModel][0],	// ファイル名
			D3DXMESH_SYSTEMMEM,						// 固定
			pDevice,								// デバイス
			NULL,									// NULL固定
			&g_aModel[nCntModel].pBuffMat,			// マテリアル
			NULL,									// NULL固定
			&g_aModel[nCntModel].dwNumMat,			// マテリアル数
			&g_aModel[nCntModel].pMesh);			// メッシュ

		int nNumVtx;	// 頂点数
		DWORD sizeFVF;	// 頂点フォーマットのサイズ
		BYTE *pVtxBuff;	// 頂点バッファへのポインタ

		//頂点数を取得
		nNumVtx = g_aModel[nCntModel].pMesh->GetNumVertices();

		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_aModel[nCntModel].pMesh->GetFVF());

		//頂点バッファをロック
		g_aModel[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	//頂点座標代入

			//最大値最小値を抜き出す
			if (vtx.x < g_aModel[nCntModel].vtxMin.x) g_aModel[nCntModel].vtxMin.x = vtx.x;	// X最小値
			if (vtx.y > g_aModel[nCntModel].vtxMax.y) g_aModel[nCntModel].vtxMax.y = vtx.y;	// Y最大値
			if (vtx.y < g_aModel[nCntModel].vtxMin.y) g_aModel[nCntModel].vtxMin.y = vtx.y;	// Y最小値
			if (vtx.z > g_aModel[nCntModel].vtxMax.z) g_aModel[nCntModel].vtxMax.z = vtx.z;	// Z最大値
			if (vtx.z < g_aModel[nCntModel].vtxMin.z) g_aModel[nCntModel].vtxMin.z = vtx.z;	// Z最小値
			if (vtx.x > g_aModel[nCntModel].vtxMax.x) g_aModel[nCntModel].vtxMax.x = vtx.x;	// X最大値

			pVtxBuff += sizeFVF;		// 頂点フォーマットのサイズ分だけポインタを進める
		}

		//頂点バッファをアンロック
		g_aModel[nCntModel].pMesh->UnlockVertexBuffer();

		//マテリアルへのポインタ
		D3DXMATERIAL *pMat;

		//マテリアル情報へのポインタ
		pMat = (D3DXMATERIAL*)g_aModel[nCntModel].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aModel[nCntModel].dwNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				HRESULT hr = D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aModel[nCntModel].pTexture[nCntMat]);
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void UninitModel(void)
{
	for (int nCntModel = 0; nCntModel < MODEL_AMOUNT_MAX; nCntModel++)
	{
		//メッシュの破棄
		if (g_aModel[nCntModel].pMesh != NULL)
		{
			g_aModel[nCntModel].pMesh->Release();
			g_aModel[nCntModel].pMesh = NULL;
		}

		//マテリアルの破棄
		if (g_aModel[nCntModel].pBuffMat != NULL)
		{
			g_aModel[nCntModel].pBuffMat->Release();
			g_aModel[nCntModel].pBuffMat = NULL;
		}

		for (int nCntMat = 0; nCntMat < (int)g_aModel[nCntMat].dwNumMat; nCntMat++)\
		{
			//テクスチャ破棄
			if (g_aModel[nCntModel].pTexture[nCntMat] != NULL)
			{
				g_aModel[nCntModel].pTexture[nCntMat]->Release();
				g_aModel[nCntModel].pTexture[nCntMat] = NULL;
			}
		}

	}
}

//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void UpdateModel(void)
{
	//当たり判定（移動）
	ModelCollision();
}

//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntModel = 0; nCntModel < g_nCountModelSet; nCntModel++)
	{
		D3DXMATRIX mtxRot, mtxTrans;												// 計算用マトリックス
		D3DMATERIAL9 matDef;														// 現在のマテリアル保存用
		D3DXMATERIAL *pMat;															// マテリアルデータへのポインタ
		D3DXVECTOR3 pos = D3DXVECTOR3(g_aModelSetInfo[nCntModel].fPos[0], g_aModelSetInfo[nCntModel].fPos[1], g_aModelSetInfo[nCntModel].fPos[2]);	// 配置モデルの位置
		D3DXVECTOR3 rot = D3DXVECTOR3(D3DXToRadian(g_aModelSetInfo[nCntModel].fRot[0]), D3DXToRadian(g_aModelSetInfo[nCntModel].fRot[1]), D3DXToRadian(g_aModelSetInfo[nCntModel].fRot[2]));	// 配置モデルの向き

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aModelSetInfo[nCntModel].mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
		D3DXMatrixMultiply(&g_aModelSetInfo[nCntModel].mtxWorld, &g_aModelSetInfo[nCntModel].mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
		D3DXMatrixMultiply(&g_aModelSetInfo[nCntModel].mtxWorld, &g_aModelSetInfo[nCntModel].mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aModelSetInfo[nCntModel].mtxWorld);

		//現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);
		
		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_aModel[g_aModelSetInfo[nCntModel].nType].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aModel[g_aModelSetInfo[nCntModel].nType].dwNumMat; nCntMat++)
		{
			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_aModel[g_aModelSetInfo[nCntModel].nType].pTexture[nCntMat]);

			//モデルパーツの描画
			g_aModel[g_aModelSetInfo[nCntModel].nType].pMesh->DrawSubset(nCntMat);
		}

		//保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//-----------------------------------------------------------------------------
// モデルの情報取得
//-----------------------------------------------------------------------------
Model *GetModel(void)
{
	return &g_aModel[0];
}

//-----------------------------------------------------------------------------
// モデルの配置
//-----------------------------------------------------------------------------
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCntModel = 0; nCntModel < MODEL_AMOUNT_MAX; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == false)
		{
			//位置の設定
			g_aModel[nCntModel].pos = pos;

			//向きの設定
			g_aModel[nCntModel].rot = rot;

			//使用中に移行
			g_aModel[nCntModel].bUse = true;
			break;
		}
	}
}

void ModelCollision(void)
{
	Player *pPlayer = GetPlayer();
	float fPwidth = PlAYER_WIDTH;

	for (int nCntModel = 0; nCntModel < g_nCountModelSet; nCntModel++)
	{
		//モデルの配置場所
		D3DXVECTOR3 modelPos = D3DXVECTOR3(g_aModelSetInfo[nCntModel].fPos[0], g_aModelSetInfo[nCntModel].fPos[1], g_aModelSetInfo[nCntModel].fPos[2]);

		// 正方形 対 矩形 当たり判定 2d 判定後めり込み戻す
		//右から
		if (pPlayer->pos.x - fPwidth < modelPos.x + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMax.x && pPlayer->pos.x - fPwidth > modelPos.x + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMin.x &&
			pPlayer->pos.z + fPwidth > modelPos.z + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMin.z && pPlayer->pos.z - fPwidth < modelPos.z + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMax.z)
		{
			if (pPlayer->posOld.x + fPwidth > modelPos.x + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMax.x &&
				pPlayer->posOld.z + fPwidth > modelPos.z + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMin.z && pPlayer->posOld.z - fPwidth < modelPos.z + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMax.z)
			{
				pPlayer->pos.x = modelPos.x + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMax.x + fPwidth;
			}
		}
		//左から
		if (pPlayer->pos.x + fPwidth > modelPos.x + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMin.x && pPlayer->pos.x + fPwidth < modelPos.x + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMax.x &&
			pPlayer->pos.z + fPwidth > modelPos.z + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMin.z && pPlayer->pos.z - fPwidth < modelPos.z + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMax.z)
		{
			if (pPlayer->posOld.x - fPwidth < modelPos.x + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMin.x &&
				pPlayer->posOld.z + fPwidth > modelPos.z + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMin.z && pPlayer->posOld.z - fPwidth < modelPos.z + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMax.z)
			{
				pPlayer->pos.x = modelPos.x + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMin.x - fPwidth;
			}

		}
		//奥から
		if (pPlayer->pos.z - fPwidth < modelPos.z + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMax.z && pPlayer->pos.z - fPwidth > modelPos.z + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMin.z &&
			pPlayer->pos.x + fPwidth > modelPos.x + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMin.x && pPlayer->pos.x - fPwidth < modelPos.x + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMax.x)
		{
			if (pPlayer->posOld.z + fPwidth > modelPos.z + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMax.z &&
				pPlayer->posOld.x + fPwidth > modelPos.x + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMin.x && pPlayer->posOld.x - fPwidth < modelPos.x + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMax.x)
			{
				pPlayer->pos.z = modelPos.z + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMax.z + fPwidth;
			}
		}
		//手前から
		if (pPlayer->pos.z + fPwidth > modelPos.z + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMin.z && pPlayer->pos.z + fPwidth < modelPos.z + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMax.z &&
			pPlayer->pos.x + fPwidth > modelPos.x + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMin.x && pPlayer->pos.x - fPwidth < modelPos.x + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMax.x)
		{
			if (pPlayer->posOld.z - fPwidth < modelPos.z + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMin.z &&
				pPlayer->posOld.x + fPwidth > modelPos.x + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMin.x && pPlayer->posOld.x - fPwidth < modelPos.x + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMax.x)
			{
				pPlayer->pos.z = modelPos.z + g_aModel[g_aModelSetInfo[nCntModel].nType].vtxMin.z - fPwidth;
			}
		}
	}
}

void ReadModelInfo(void)
{
	FILE *pFile;				// ファイルへのポインタ
	char cRowChar[READROW][64];	// 読み込む文字列
	char cEqual[2];				// イコール比較

	int nCntTextureName = 0;	// カウント用（後で一つにまとめる）
	int nCntModelFileName = 0;	// 
	int nCntCol = 0;			// 
	int nCntType = 0;			// 
	int nCntPos = 0;			// 
	int nCntRot = 0;			// 

	//ファイルおーぷん
	pFile = fopen("model.txt", "r");
	if (pFile != NULL)
	{
		//READROW行探す
		for (int nCntRow = 0; nCntRow < READROW; nCntRow++)
		{
			//文字列読み込み
			fscanf(pFile, "%s", &cRowChar[nCntRow][0]);

			//SCRIPTの文字列を見つけたら
			if (strcmp(&cRowChar[nCntRow][0], "SCRIPT") == 0)
			{
				for (int nCntRow = 0; nCntRow < READROW; nCntRow++)
				{
					//次の文字列
					fscanf(pFile, "%s", &cRowChar[nCntRow][0]);

					//NUM_TEXTURE
					if (strcmp(&cRowChar[nCntRow][0], "NUM_TEXTURE") == 0)
					{
						//次の文字列
						fscanf(pFile, "%s", &cEqual[0]);

						//イコール見つけた
						if (strcmp(&cEqual[0], "=") == 0)
						{
							//イコールの先の数を変数に入れる
							fscanf(pFile, "%d", &g_LoadModel.nNumTexture);
						}
					}

					//TEXTURE_FILENAME
					if (strcmp(&cRowChar[nCntRow][0], "TEXTURE_FILENAME") == 0)
					{
						//次の文字
						fscanf(pFile, "%s", &cEqual[0]);

						//イコール見つけたら
						if (strcmp(&cEqual[0], "=") == 0)
						{
							//イコールの次の数を変数に入れる
							fscanf(pFile, "%s", &g_LoadModel.cTextureFile[nCntTextureName][0]);

							//カウントアップ
							nCntTextureName++;
						}
					}

					//NUM_MODEL
					if (strcmp(&cRowChar[nCntRow][0], "NUM_MODEL") == 0)
					{
						//次の文字列
						fscanf(pFile, "%s", &cEqual[0]);

						//イコール見つけた
						if (strcmp(&cEqual[0], "=") == 0)
						{
							//イコールの先の数を変数に入れる
							fscanf(pFile, "%d", &g_LoadModel.nNumModel);
						}
					}

					//MODEL_FILENAME
					if (strcmp(&cRowChar[nCntRow][0], "MODEL_FILENAME") == 0)
					{
						//次の文字
						fscanf(pFile, "%s", &cEqual[0]);

						//イコール見つけたら
						if (strcmp(&cEqual[0], "=") == 0)
						{
							//イコールの次の数を変数に入れる
							fscanf(pFile, "%s", &g_LoadModel.cModelFile[nCntModelFileName][0]);

							//カウントアップ
							nCntModelFileName++;
						}
					}

					//MODELSET
					if (strcmp(&cRowChar[nCntRow][0], "MODELSET") == 0)
					{
						g_nCountModelSet++;
						for (int nCntMSET = 0; nCntMSET < READROW; nCntMSET++)
						{
							//次の文字列
							fscanf(pFile, "%s", &cRowChar[nCntMSET][0]);

							//TYPE
							if (strcmp(&cRowChar[nCntMSET][0], "TYPE") == 0)
							{
								//次の文字列
								fscanf(pFile, "%s", &cEqual[0]);

								//イコール見つけた
								if (strcmp(&cEqual[0], "=") == 0)
								{
									//イコールの先の数を変数に入れる
									fscanf(pFile, "%d", &g_aModelSetInfo[nCntType].nType);

									//カウントアップ
									nCntType++;
								}
							}

							//POS
							if (strcmp(&cRowChar[nCntMSET][0], "POS") == 0)
							{
								//次の文字列
								fscanf(pFile, "%s", &cEqual[0]);

								//イコール見つけたら
								if (strcmp(&cEqual[0], "=") == 0)
								{
									for (int nCntVector3 = 0; nCntVector3 < 3; nCntVector3++)
									{
										//次の値を変数に入れる
										fscanf(pFile, "%f", &g_aModelSetInfo[nCntPos].fPos[nCntVector3]);
									}

									//カウントアップ
									nCntPos++;
								}
							}

							//ROT
							if (strcmp(&cRowChar[nCntMSET][0], "ROT") == 0)
							{
								//次の文字列
								fscanf(pFile, "%s", &cEqual[0]);

								//イコール見つけたら
								if (strcmp(&cEqual[0], "=") == 0)
								{
									for (int nCntVector3 = 0; nCntVector3 < 3; nCntVector3++)
									{
										fscanf(pFile, "%f", &g_aModelSetInfo[nCntRot].fRot[nCntVector3]);
									}
									nCntRot++;
								}
							}

							//COLLISION
							if (strcmp(&cRowChar[nCntMSET][0], "COLLISION") == 0)
							{
								//次の文字列
								fscanf(pFile, "%s", &cEqual[0]);

								//イコール見つけた
								if (strcmp(&cEqual[0], "=") == 0)
								{
									//イコールの先の数を変数に入れる
									fscanf(pFile, "%d", &g_aModelSetInfo[nCntCol].nCollision);

									//カウントアップ
									nCntCol++;
								}
							}

							//END_MODELSET見つけたらループ抜ける
							if (strcmp(&cRowChar[nCntMSET][0], "END_MODELSET") == 0)
							{
								break;
							}
						}
					}

					//END_SCRIPT見つけたらループ抜ける
					if (strcmp(&cRowChar[nCntRow][0], "END_SCRIPT") == 0)
					{
						break;
					}
				}
				break;
			}
		}
	}
}