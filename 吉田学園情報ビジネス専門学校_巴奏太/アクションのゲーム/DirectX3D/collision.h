//-----------------------------------------------------------------------------
//
// 衝突判定の処理 ヘッダー [collision.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "main.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define MAX_VISIBLE_COLLISION (32)		// 可視化できるコリジョンの最大数

//-----------------------------------------------------------------------------
// 可視化コリジョン 構造体
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXMATRIX mtxWorld;	// ワールドマトリックス	
	LPD3DXMESH pMesh;		// メッシュ
	D3DMATERIAL9 mat;		// マテリアル
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 向き
	bool bUse;				// 使用中かどうか
	int nIdx;				// 番号
} VCollision;

// プロトタイプ宣言
//-----------------------------------------------------------------------------
void InitCollision(void);
void UninitCollision(void);
void UpdateCollision(void);
void DrawCollision(void);
VCollision *GetVCol(void);
void SetSphere(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nSlices, int nIdx);
void SetBox(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, float fDepth, int nIdx);
void MoveVCollision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pRot, int nIdx);
bool CollisionBoxBox(D3DXVECTOR3 *pPosA, D3DXVECTOR3*pPosB);
bool CollisionBoxSphere(D3DXVECTOR3 *pBoxPos, D3DXVECTOR3 *pSpherePos, float fBoxWidth, float fBoxHeight, float fBoxDepth, float fSphereRadius);

#endif