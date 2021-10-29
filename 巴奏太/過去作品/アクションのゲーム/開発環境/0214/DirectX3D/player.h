//-----------------------------------------------------------------------------
//
// プレイヤーの処理 ヘッダー [player.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define PLAYER_MOVESPEED (0.5f)													// プレイヤーの移動速度
#define PLAYER_ATTACK_RADIUS (12.0f)											// プレイヤーの攻撃範囲
#define PlAYER_WIDTH (7.5f)
#define PLAYER_HEIGHT (50.0f)
#define PLAYER_INVINCIBLE_TIME (500)	// 攻撃された時の無敵時間
#define PLAYER_HEALTH (10)				// プレイヤーの体力

//-----------------------------------------------------------------------------
// モーションの種類
//-----------------------------------------------------------------------------
typedef enum
{
	MOTIONTYPE_NEUTRAL = 0,			// ニュートラルモーション
	MOTIONTYPE_ATTACK,				// 攻撃モーション
	MOTIONTYPE_WALK,				// 歩きモーション
	MOTIONTYPE_MAX
} MOTIONTYPE;

//-----------------------------------------------------------------------------
// キーの構造体
//-----------------------------------------------------------------------------
typedef struct
{
	float fPosX;		// 位置X
	float fPosY;		// 位置Y
	float fPosZ;		// 位置Z
	float fRotX;		// 向きX
	float fRotY;		// 向きY
	float fRotZ;		// 向きZ
} KEY;

//-----------------------------------------------------------------------------
// キー情報の構造体
//-----------------------------------------------------------------------------
typedef struct
{
	int nFrame;			// 再生フレーム
	KEY aKey[10];		// キー (パーツ最大数分)
} KEY_INFO;

//-----------------------------------------------------------------------------
// モーション情報の構造体
//-----------------------------------------------------------------------------
typedef struct
{
	bool bLoop;					// ループの有無
	int nNumKey;				// キーの総数
	KEY_INFO aKeyInfo[10];		// キー情報	(キーの最大数)
} MOTION_INFO;

//-----------------------------------------------------------------------------
// モデルの構造体
//-----------------------------------------------------------------------------
typedef struct
{
	LPD3DXMESH pMeshModel;		// メッシュへのポインタ
	LPD3DXBUFFER pBuffMatModel;	// マテリアルへのポインタ
	DWORD nNumMatModel;			// マテリアルの数
	D3DXVECTOR3 pos;			// 位置(オフセット)
	D3DXVECTOR3 rot;			// 向き
	D3DXMATRIX mtxWorld;		// ワールドマトリックス
	int nIdxModelParent;		// 親モデルのインデックス
	int nMotionCount;
} PlayerModel;

//-----------------------------------------------------------------------------
// プレイヤーの構造体
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;				// 位置
	D3DXVECTOR3 posOld;				// 前回の位置
	D3DXVECTOR3 move;				// 移動値
	D3DXVECTOR3 rot;				// 向き
	D3DXVECTOR3 rotDest;			// 目標の向き
	D3DXMATRIX mtxWorld;			// ワールドマトリックス
	PlayerModel aModel[10];			// モデル(パーツ)
	int nNumModel;					// モデル(パーツ)数

	MOTION_INFO aMotionInfo[10];	// モーション情報	 (モーションの最大数)
	MOTIONTYPE motionType;			// モーションタイプ
	bool bLoopMotion;				// ループの有無
	int nCurrentMotion;				// 現在のモーション
	int nNumMotion;					// モーション数
	int nNumKey;					// キー数
	int nKey;						// キーナンバー
	int nCounterMotion;				// モーションカウンター
	bool bPlayMotion;				// モーション再生状態

	int nLifeMax;					// 最大体力
	int nLife;						// 体力
	bool bHit;						// 攻撃されている状態
	bool bInvincible;				// 無敵状態
	DWORD dwTime;					// 時間計算用
} Player;

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
KEY KeyPosRot(float posX, float posY, float posZ, float rotX, float rotY, float rotZ);

#endif