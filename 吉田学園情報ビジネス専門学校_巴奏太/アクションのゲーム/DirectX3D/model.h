//-----------------------------------------------------------------------------
//
// モデルの処理 ヘッダー [model.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define MODEL_MOVESPEED (1.0f)													// モデルの移動速度
#define MODEL_SMOOTHTURNSPEED (0.1f)											// モデルを滑らかに回転させる速度 (0.0f ~ 1.0f)
#define MODEL_AMOUNT_MAX (64)													// モデルの最大数

//-----------------------------------------------------------------------------
// モデルの構造体
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXMATRIX mtxWorld;				// ワールドマトリックス
	LPD3DXMESH pMesh;					// メッシュへのポインタ
	LPD3DXBUFFER pBuffMat;				// マテリアルへのポインタ
	DWORD dwNumMat;						// マテリアルの数
	LPDIRECT3DTEXTURE9 pTexture[MODEL_AMOUNT_MAX];	// テクスチャへのポインタ
	D3DXVECTOR3 pos;					// 位置
	D3DXVECTOR3 move;					// 移動値
	D3DXVECTOR3 rot;					// 向き
	D3DXVECTOR3 vtxMin;					// 頂点の最小値
	D3DXVECTOR3 vtxMax;					// 頂点の最大値

	bool bUse;							// 使用状況
} Model;

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
Model *GetModel(void);
#endif