#define _CRT_SECURE_NO_WARNINGS
//-----------------------------------------------------------------------------
//
// プレイヤーの処理 [player.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "player.h"
#include "camera.h"
#include "input.h"
#include "shadow.h"
#include "bullet.h"
#include "collision.h"
#include "fade.h"
#include "UI.h"
#include "result.h"
#include "field.h"
#include <stdio.h>
#include <time.h>

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define PLAYER_MODEL_AMOUNT (10)
#define PLAYER_SMOOTHTURN_SPEED (0.1f)											// 滑らかに振り向く速度
#define READROW (1028)	// ファイル読み込みで読む最大数

//-----------------------------------------------------------------------------
// モデル情報の構造体
//-----------------------------------------------------------------------------
typedef struct
{
	int nModelNum;										// モデルの数	
	int nModelIdx[PLAYER_MODEL_AMOUNT];					// モデルの番号
	int nModelParent[PLAYER_MODEL_AMOUNT];				// モデルの親
	char cModelFileName[PLAYER_MODEL_AMOUNT][128];		// モデルのファイル名
	float fModelPos[PLAYER_MODEL_AMOUNT][3];			// モデルの位置(オフセット)
	float fModelRot[PLAYER_MODEL_AMOUNT][3];			// モデルの向き
} ModelInfo;

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
void MovePlayer(float fMoveAngleDegree, float fMoveSpeed);
HRESULT LoadXFile(const char* cXFileName, int nCountModel);
void PlayerSmoothTurn(void);
void PlayMotion(bool bPlayMotion);
void ResetMotion(bool bPartsReset, bool bCounterReset, bool bKeyReset, bool bMotionTrig);
void StartMotion(MOTIONTYPE motionType);
void ReadPlayerInfo(void);

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
Player g_player;																// プレイヤーの情報
D3DXVECTOR3 g_MotionKey[4][10];													// モーションのキー
KEY g_playerDefaultKey[10];
ModelInfo g_ModelInfo;														// 読み込んだモデルの情報

//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスの取得

	//プレイヤーの初期設定
	g_player.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置の初期設定
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向きの初期設定
	g_player.bHit = false;
	g_player.bInvincible = false;
	g_player.dwTime = NULL;
	g_player.nLife = PLAYER_HEALTH;
	g_player.nLifeMax = PLAYER_HEALTH;

	//テキスト読み込み
	ReadPlayerInfo();

	//読み込んだ情報を使ってXファイル読み込み
	for (int nCntModel = 0; nCntModel < g_ModelInfo.nModelNum; nCntModel++)
	{
		LoadXFile(&g_ModelInfo.cModelFileName[nCntModel][0], nCntModel);
	}

	//モデルのパーツ数
	g_player.nNumModel = g_ModelInfo.nModelNum;

	//各パーツの階層構造設定
	for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
	{
		g_player.aModel[nCntModel].nIdxModelParent = g_ModelInfo.nModelParent[nCntModel];
		g_player.aModel[nCntModel].pos = D3DXVECTOR3(g_ModelInfo.fModelPos[nCntModel][0], g_ModelInfo.fModelPos[nCntModel][1], g_ModelInfo.fModelPos[nCntModel][2]);
		g_player.aModel[nCntModel].rot = D3DXVECTOR3(g_ModelInfo.fModelRot[nCntModel][0], g_ModelInfo.fModelRot[nCntModel][1], g_ModelInfo.fModelRot[nCntModel][2]);
	}

	//初期キー
	for (int nCnt = 0; nCnt < g_player.nNumModel; nCnt++)
	{
		g_playerDefaultKey[nCnt] = KeyPosRot(g_player.aModel[nCnt].pos.x, g_player.aModel[nCnt].pos.y, g_player.aModel[nCnt].pos.z, 0, 0, 0);
	}

	//モーションの設定
	g_player.nNumMotion = 3;

	g_player.aMotionInfo[MOTIONTYPE_NEUTRAL].nNumKey = 2;
	g_player.aMotionInfo[MOTIONTYPE_NEUTRAL].bLoop = true;
	g_player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame = 65;
	g_player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].nFrame = 65;

	g_player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[0] = KeyPosRot(0.0f, 0.0f, 0.0f, -0.07f, 0.0f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[1] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.01f, 0.0f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[2] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.0f, -0.75f, 1.0f);
	g_player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[3] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[4] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.0f, 0.75f, -1.0f);
	g_player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[5] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[6] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -0.09f);
	g_player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[7] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[8] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.09f);
	g_player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[9] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	g_player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[0] = KeyPosRot(0.0f, 0.0f, 0.0f, -0.06f, 0.0f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[1] = KeyPosRot(0.0f, 0.0f, 0.0f, -0.06f, 0.0f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[2] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.0f, -1.22f, 0.79f);
	g_player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[3] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.0f, -0.53f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[4] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.0f, 1.22f, -0.79f);
	g_player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[5] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[6] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.06f, 0.0f, -0.09f);
	g_player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[7] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.0f, -0.18f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[8] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.06f, 0.0f, 0.09f);
	g_player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[9] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	g_player.aMotionInfo[MOTIONTYPE_ATTACK].nNumKey = 4;
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].bLoop = false;
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[0].nFrame = 10;
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[1].nFrame = 10;
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[2].nFrame = 10;
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[3].nFrame = 30;

	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[0].aKey[0] = KeyPosRot(0.0f, -2.80f, 0.0f, 0.48f, 0.25f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[0].aKey[1] = KeyPosRot(0.0f, 0.0f, 0.0f, -0.31f, 0.0f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[0].aKey[2] = KeyPosRot(0.0f, 0.0f, 0.0f, -0.19f, 0.44f, -0.31f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[0].aKey[3] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.0f, -0.97f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[0].aKey[4] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.0f, 1.07f, -1.17f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[0].aKey[5] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.72f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[0].aKey[6] = KeyPosRot(0.0f, 0.50f, 0.0f, -0.85f, -0.35f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[0].aKey[7] = KeyPosRot(0.0f, 0.0f, 0.0f, -0.4f, 0.0f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[0].aKey[8] = KeyPosRot(0.0f, 0.50f, 0.0f, 0.5f, -0.35f, 0.13f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[0].aKey[9] = KeyPosRot(0.0f, 0.0f, 0.0f, -0.59f, 0.0f, 0.0f);

	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[1].aKey[0] = KeyPosRot(-0.00f, -5.40f, -3.20f, 0.41f, 0.35f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[1].aKey[1] = KeyPosRot(0.0f, 0.0f, 0.0f, -0.31f, 0.0f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[1].aKey[2] = KeyPosRot(0.0f, 0.0f, 0.0f, -0.19f, 0.66f, -0.31f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[1].aKey[3] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.0f, -1.45f, -0.82f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[1].aKey[4] = KeyPosRot(0.0f, 0.0f, 0.0f, -0.16f, 1.07f, -1.16f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[1].aKey[5] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.72f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[1].aKey[6] = KeyPosRot(0.0f, 0.0f, 0.0f, -1.19f, -0.41f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[1].aKey[7] = KeyPosRot(0.0f, 0.0f, 0.0f, -0.41f, 0.0f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[1].aKey[8] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.5f, -0.41f, 0.13f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[1].aKey[9] = KeyPosRot(0.0f, 0.0f, 0.0f, -0.60f, 0.0f, 0.0f);

	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[2].aKey[0] = KeyPosRot(0.00f, -6.10f, -26.60f, -0.41f, -0.44f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[2].aKey[1] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[2].aKey[2] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.13f, -1.23f, -0.41f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[2].aKey[3] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[2].aKey[4] = KeyPosRot(0.0f, 0.0f, 0.0f, -0.62f, 0.06f, -1.16f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[2].aKey[5] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.0f, 1.38f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[2].aKey[6] = KeyPosRot(0.0f, 0.0f, 0.0f, 1.41f, 0.72f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[2].aKey[7] = KeyPosRot(0.0f, 0.0f, 0.0f, -0.97f, 0.0f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[2].aKey[8] = KeyPosRot(0.0f, 0.0f, 0.0f, -0.38f, 0.28f, 0.31f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[2].aKey[9] = KeyPosRot(0.0f, 0.0f, 0.0f, -0.66f, 0.0f, 0.0f);

	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[3].aKey[0] = KeyPosRot(0.00f, -6.40f, -27.00f, -0.44f, -0.44f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[3].aKey[1] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[3].aKey[2] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.13f, -1.23f, -0.41f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[3].aKey[3] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[3].aKey[4] = KeyPosRot(0.0f, 0.0f, 0.0f, -1.13f, 0.47f, -1.16f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[3].aKey[5] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.0f, 1.38f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[3].aKey[6] = KeyPosRot(0.0f, 0.0f, 0.0f, 1.41f, 0.72f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[3].aKey[7] = KeyPosRot(0.0f, 0.0f, 0.0f, -0.97f, 0.0f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[3].aKey[8] = KeyPosRot(0.0f, 0.0f, 0.0f, -0.38f, 0.28f, 0.31f);
	g_player.aMotionInfo[MOTIONTYPE_ATTACK].aKeyInfo[3].aKey[9] = KeyPosRot(0.0f, 0.0f, 0.0f, -0.66f, 0.0f, 0.0f);

	g_player.aMotionInfo[MOTIONTYPE_WALK].nNumKey = 4;
	g_player.aMotionInfo[MOTIONTYPE_WALK].bLoop = true;
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].nFrame = 9;
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].nFrame = 6;
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].nFrame = 9;
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].nFrame = 6;

	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[0] = KeyPosRot(0.0f, -1.6f, 0.0f, -0.19f, 0.00f, 0.00f);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[1] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[2] = KeyPosRot(0.0f, 0.0f, 0.0f, -0.01f, 0.00f, 1.60f);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[3] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[4] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.01f, 0.0f, -1.60f);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[5] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[6] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.01f, 0.0f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[7] = KeyPosRot(0.0f, 0.0f, 0.0f, -0.0, 0.0f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[8] = KeyPosRot(0.0f, 0.0f, 0.0f, -0.01f, -0.0f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[9] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[0] = KeyPosRot(0.0f, -4.2f, 0.0f, -0.35f, 0.41f, 0.00f);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[1] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.00f, -0.41f, 0.00f);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[2] = KeyPosRot(0.0f, 0.0f, 0.0f, -1.50f, -0.00f, 1.60f);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[3] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.03f, -1.00f, 0.00f);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[4] = KeyPosRot(0.0f, 0.0f, 0.0f, 2.04f, -0.41f, -1.60f);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[5] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.00f, 0.41f, 0.16f);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[6] = KeyPosRot(0.0f, 0.0f, 0.0f, 1.82f, -0.41f, 0.00f);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[7] = KeyPosRot(0.0f, 0.0f, 0.0f, -0.57f, 0.00f, 0.00f);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[8] = KeyPosRot(0.0f, 0.0f, 0.0f, -2.01f, -0.41f, 0.00f);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[9] = KeyPosRot(0.0f, 0.0f, 0.0f, -0.69f, 0.00f, 0.00f);

	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[0] = KeyPosRot(0.0f, -1.6f, 0.0f, -0.19f, 0.0f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[1] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[2] = KeyPosRot(0.0f, 0.0f, 0.0f, -0.01f, 0.00f, 1.60f);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[3] = KeyPosRot(0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[4] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.01f, -0.00f, -1.60f);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[5] = KeyPosRot(0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[6] = KeyPosRot(0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[7] = KeyPosRot(0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[8] = KeyPosRot(0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[9] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[0] = KeyPosRot(0.0f, -4.2f, 0.0f, -0.35f, -0.41f, 0.00f);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[1] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.0f, 0.41f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[2] = KeyPosRot(0.0f, 0.0f, 0.0f, 2.07f, 0.41f, 1.60f);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[3] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.00f, -0.41f, 0.16f);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[4] = KeyPosRot(0.0f, 0.0f, 0.0f, -1.50f, 0.00f, -1.60f);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[5] = KeyPosRot(0.0f, 0.0f, 0.0f, 0.0f, 0.97f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[6] = KeyPosRot(0.0f, 0.0f, 0.0f, -2.01f, 0.41f, 0.00f);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[7] = KeyPosRot(0.0f, 0.0f, 0.0f, -0.85f, 0.0f, 0.0f);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[8] = KeyPosRot(0.0f, 0.0f, 0.0f, 1.82f, 0.41f, 0.00f);
	g_player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[9] = KeyPosRot(0.0f, 0.0f, 0.0f, -0.47f, 0.0f, 0.0f);

}

//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void UninitPlayer(void)
{
	for (int nCntModel = 0; nCntModel < 2; nCntModel++)
	{
		//メッシュの破棄
		if (g_player.aModel[nCntModel].pMeshModel != NULL)
		{
			g_player.aModel[nCntModel].pMeshModel->Release();
			g_player.aModel[nCntModel].pMeshModel = NULL;
		}

		//マテリアルの破棄
		if (g_player.aModel[nCntModel].pBuffMatModel != NULL)
		{
			g_player.aModel[nCntModel].pBuffMatModel->Release();
			g_player.aModel[nCntModel].pBuffMatModel = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void UpdatePlayer(void)
{
	DWORD dwCurrentTime = timeGetTime();	// 現在時間

	//位置保存
	g_player.posOld = g_player.pos;

	//移動量追加
	g_player.pos += g_player.move;

	//移動量減衰
	g_player.move.x += (0 - g_player.move.x) * 0.2f;
	g_player.move.z += (0 - g_player.move.z) * 0.2f;

	//重力
	g_player.move.y -= 0.1f;

	//移動制限
	if (g_player.pos.x > FIELD_BORDER) g_player.pos.x = FIELD_BORDER;
	if (g_player.pos.x < -FIELD_BORDER) g_player.pos.x = -FIELD_BORDER;
	if (g_player.pos.z > FIELD_BORDER) g_player.pos.z = FIELD_BORDER;
	if (g_player.pos.z < -FIELD_BORDER) g_player.pos.z = -FIELD_BORDER;

	/* モデルの移動 */
	if (GetKeyboardPress(DIK_W) == true)
	{
		if (GetKeyboardPress(DIK_D) == true) MovePlayer(45.0f, PLAYER_MOVESPEED);			// 右前
		else if (GetKeyboardPress(DIK_A) == true) MovePlayer(-45.0f, PLAYER_MOVESPEED);		// 左前
		else MovePlayer(0.0f, PLAYER_MOVESPEED);												// 前
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{
		if (GetKeyboardPress(DIK_D) == true) MovePlayer(135.0f, PLAYER_MOVESPEED);			// 右後ろ
		else if (GetKeyboardPress(DIK_A) == true) MovePlayer(-135.0f, PLAYER_MOVESPEED);		// 左後ろ
		else MovePlayer(-180.0f, PLAYER_MOVESPEED);												// 後ろ
	}
	else if (GetKeyboardPress(DIK_A) == true) MovePlayer(-90.0f, PLAYER_MOVESPEED);			// 左
	else if (GetKeyboardPress(DIK_D) == true) MovePlayer(90.0f, PLAYER_MOVESPEED);			// 右

	/* プレイヤーの振り向きを滑らかにする */
	PlayerSmoothTurn();

	/* モーション再生 */
	PlayMotion(g_player.bPlayMotion);

	//行動にモーションつける
	if (GetKeyboardPress(DIK_SPACE) == true)
	{
		StartMotion(MOTIONTYPE_ATTACK);
	}

	else if (GetKeyboardPress(DIK_W) ||
		GetKeyboardPress(DIK_S) ||
		GetKeyboardPress(DIK_A) ||
		GetKeyboardPress(DIK_D) == true)
	{
		StartMotion(MOTIONTYPE_WALK);
	}
	else
	{
		StartMotion(MOTIONTYPE_NEUTRAL);
	}

	//攻撃中は移動０にする
	if (g_player.motionType == MOTIONTYPE_ATTACK)
	{
		g_player.move = D3DXVECTOR3(0, 0, 0);
	}

	//時間経過で無敵時間解除
	if (dwCurrentTime - g_player.dwTime >= PLAYER_INVINCIBLE_TIME)
	{
		//無敵解除
		g_player.bInvincible = false;
	}

	//体力なくなると
	if (g_player.nLife <= 0)
	{
		GAMEOVER *pGameOver = GetGameOver();	// ゲームオーバー画面の情報を取得
		RESULT *pResult = GetResult();			// リザルトの情報を取得

		//ゲームオーバー画面表示
		pGameOver->bDraw = true;
		pGameOver->bGameOver = true;
		pResult->bGameOver = true;

		//リザルト画面にフェードアウト
		SetFade(FADE_OUT, MODE_RESULT);
	}
}

//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();									// デバイス取得
	D3DXMATRIX mtxRot, mtxTrans;												// 計算用マトリックス
	D3DMATERIAL9 matDef;														// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;															// マテリアルデータへのポインタ
	D3DXMATERIAL *pMatAlt;														// 代えのマテリアル

	//プレイヤーのワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_player.mtxWorld);

	//プレイヤーの向き反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxRot);

	//プレイヤーの位置反映
	D3DXMatrixTranslation(&mtxTrans, g_player.pos.x, g_player.pos.y, g_player.pos.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxTrans);

	//プレイヤーのワールドマトリックス設定
	pDevice->SetTransform(D3DTS_WORLD, &g_player.mtxWorld);

	//現在のマテリアル取得
	pDevice->GetMaterial(&matDef);

	for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
	{
		D3DXMATRIX mtxRotModel, mtxTransModel;	// 計算用マトリックス
		D3DXMATRIX mtxParent;					// 親のマトリックス

		//各パーツのワールドマトリックス初期化
		D3DXMatrixIdentity(&g_player.aModel[nCntModel].mtxWorld);

		//各パーツの向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_player.aModel[nCntModel].rot.y, g_player.aModel[nCntModel].rot.x, g_player.aModel[nCntModel].rot.z);
		D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld, &g_player.aModel[nCntModel].mtxWorld, &mtxRotModel);

		//各パーツの位置を反映
		D3DXMatrixTranslation(&mtxTransModel, g_player.aModel[nCntModel].pos.x, g_player.aModel[nCntModel].pos.y, g_player.aModel[nCntModel].pos.z);
		D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld, &g_player.aModel[nCntModel].mtxWorld, &mtxTransModel);

		//各パーツの親のマトリックスを設定
		if (g_player.aModel[nCntModel].nIdxModelParent != -1)
		{
			mtxParent = g_player.aModel[g_player.aModel[nCntModel].nIdxModelParent].mtxWorld;
		}
		else
		{
			mtxParent = g_player.mtxWorld;	// プレイヤーのマトリックスを設定
		}

		//算出した各パーツのワールドマトリックスと親のマトリックスを掛け合わせる
		D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld, &g_player.aModel[nCntModel].mtxWorld, &mtxParent);

		//各パーツのワールドマトリックス設定
		pDevice->SetTransform(D3DTS_WORLD, &g_player.aModel[nCntModel].mtxWorld);
		
		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_player.aModel[nCntModel].pBuffMatModel->GetBufferPointer();
		pMatAlt = (D3DXMATERIAL*)g_player.aModel[nCntModel].pBuffMatModel->GetBufferPointer();

		//無敵状態か見てモデルの色を変える
		if (g_player.bInvincible == true)
		{//無敵状態
			pMatAlt->MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f);
			for (int nCntMat = 0; nCntMat < (int)g_player.aModel[nCntModel].nNumMatModel; nCntMat++)
			{
				//マテリアル設定
				pDevice->SetMaterial(&pMatAlt[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, NULL);

				//モデルパーツの描画
				g_player.aModel[nCntModel].pMeshModel->DrawSubset(nCntMat);
			}
		}
		else if (g_player.bInvincible == false)
		{//通常状態
			pMat->MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0, 1.0f, 1.0f);
			for (int nCntMat = 0; nCntMat < (int)g_player.aModel[nCntModel].nNumMatModel; nCntMat++)
			{
				//マテリアル設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, NULL);

				//モデルパーツの描画
				g_player.aModel[nCntModel].pMeshModel->DrawSubset(nCntMat);
			}
		}
		//マテリアルデータへのポインタを取得
		//pMat = (D3DXMATERIAL*)g_player.aModel[nCntModel].pBuffMatModel->GetBufferPointer();

		//for (int nCntMat = 0; nCntMat < (int)g_player.aModel[nCntModel].nNumMatModel; nCntMat++)
		//{
		//	//マテリアル設定
		//	pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//	//テクスチャの設定
		//	pDevice->SetTexture(0, NULL);

		//	//モデルパーツの描画
		//	g_player.aModel[nCntModel].pMeshModel->DrawSubset(nCntMat);
		//}
	}

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

}

//-----------------------------------------------------------------------------
// プレイヤーの情報を取得
//-----------------------------------------------------------------------------
Player *GetPlayer(void)
{
	return &g_player;
}

/* モデルを移動させる関数 */
void MovePlayer(float fMoveAngleDegree, float fMoveSpeed)
{
	Camera *pCamera = GetCamera();	// カメラの情報取得

	g_player.move.x += sinf(pCamera->rot.y + D3DXToRadian(fMoveAngleDegree)) * fMoveSpeed;
	g_player.move.z += cosf(pCamera->rot.y + D3DXToRadian(fMoveAngleDegree)) * fMoveSpeed;
	g_player.rotDest.y = pCamera->rot.y + D3DXToRadian(fMoveAngleDegree) + D3DX_PI;
}

/* Xファイルからモデルを読み込む関数*/
HRESULT LoadXFile(const char* cXFileName, int nCountModel)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス取得
	HRESULT hres;

	//Xファイルの読み込み
	hres = D3DXLoadMeshFromX(
		cXFileName,										// 読み込むファイル名
		D3DXMESH_SYSTEMMEM,								// 固定
		pDevice,										// デバイス
		NULL,											// NULL固定
		&g_player.aModel[nCountModel].pBuffMatModel,		// マテリアル
		NULL,											// NULL固定
		&g_player.aModel[nCountModel].nNumMatModel,		// マテリアル数
		&g_player.aModel[nCountModel].pMeshModel);		// メッシュ

	return hres;
}

/* キーのPos,Rotを簡易的に入力させる関数*/
KEY KeyPosRot(float posX, float posY, float posZ, float rotX, float rotY, float rotZ)
{
	KEY key;

	key.fPosX = posX;
	key.fPosY = posY;
	key.fPosZ = posZ;

	key.fRotX = rotX;
	key.fRotY = rotY;
	key.fRotZ = rotZ;

	return key;
}

/* キャラの回転を滑らかにする関数 */
void PlayerSmoothTurn(void)
{
	D3DXVECTOR3 RotDiff;

	//差分計算
	RotDiff.y = g_player.rotDest.y - g_player.rot.y;

	//差分がD3DX_PI以上(半周以上)の場合、逆回転
	if (RotDiff.y > D3DX_PI)
	{
		g_player.rot.y -= ((D3DX_PI * 2) - RotDiff.y) * PLAYER_SMOOTHTURN_SPEED;
	}
	else if (RotDiff.y < -D3DX_PI)
	{
		g_player.rot.y += ((D3DX_PI * 2) + RotDiff.y) * PLAYER_SMOOTHTURN_SPEED;
	}
	else
	{
		g_player.rot.y += RotDiff.y * PLAYER_SMOOTHTURN_SPEED;
	}

	// 回転の修正 (3.14超えたら±逆に)
	if (g_player.rot.y > D3DX_PI)
	{
		g_player.rot.y -= D3DX_PI * 2.0f;
	}
	else if (g_player.rot.y < -D3DX_PI)
	{
		g_player.rot.y += D3DX_PI * 2.0f;
	}
	if (g_player.rotDest.y > D3DX_PI)
	{
		g_player.rotDest.y -= D3DX_PI * 2.0f;
	}
	else if (g_player.rotDest.y < -D3DX_PI)
	{
		g_player.rotDest.y += D3DX_PI * 2.0f;
	}
}

/* モーションの再生関数 */
void PlayMotion(bool bPlayMotion)
{
	if (bPlayMotion == true)
	{
		//ループ
		g_player.bLoopMotion = g_player.aMotionInfo[g_player.motionType].bLoop;

		//モーションカウントアップ
		g_player.nCounterMotion++;

		//モデル数分回す
		for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
		{
			//差分計算用
			KEY keyDiff[20];

			//現在のキーと次のキーとの差分を計算
			if (g_player.nKey >= g_player.aMotionInfo[g_player.motionType].nNumKey - 1 && g_player.bLoopMotion == true)
			{	// ループの場合
				keyDiff[nCntModel].fPosX = g_player.aMotionInfo[g_player.motionType].aKeyInfo[0].aKey[nCntModel].fPosX - g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fPosX;
				keyDiff[nCntModel].fPosY = g_player.aMotionInfo[g_player.motionType].aKeyInfo[0].aKey[nCntModel].fPosY - g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fPosY;
				keyDiff[nCntModel].fPosZ = g_player.aMotionInfo[g_player.motionType].aKeyInfo[0].aKey[nCntModel].fPosZ - g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fPosZ;
				keyDiff[nCntModel].fRotX = g_player.aMotionInfo[g_player.motionType].aKeyInfo[0].aKey[nCntModel].fRotX - g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotX;
				keyDiff[nCntModel].fRotY = g_player.aMotionInfo[g_player.motionType].aKeyInfo[0].aKey[nCntModel].fRotY - g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotY;
				keyDiff[nCntModel].fRotZ = g_player.aMotionInfo[g_player.motionType].aKeyInfo[0].aKey[nCntModel].fRotZ - g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotZ;
			}
			else
			{	// それ以外
				keyDiff[nCntModel].fPosX = g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey + 1].aKey[nCntModel].fPosX - g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fPosX;
				keyDiff[nCntModel].fPosY = g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey + 1].aKey[nCntModel].fPosY - g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fPosY;
				keyDiff[nCntModel].fPosZ = g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey + 1].aKey[nCntModel].fPosZ - g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fPosZ;
				keyDiff[nCntModel].fRotX = g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey + 1].aKey[nCntModel].fRotX - g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotX;
				keyDiff[nCntModel].fRotY = g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey + 1].aKey[nCntModel].fRotY - g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotY;
				keyDiff[nCntModel].fRotZ = g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey + 1].aKey[nCntModel].fRotZ - g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotZ;
			}

			//現在のキーに差分をモーションカウントで割った分を足したものをrotに代入
			g_player.aModel[nCntModel].pos.x = (g_playerDefaultKey[nCntModel].fPosX + g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fPosX) + keyDiff[nCntModel].fPosX * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].nFrame);
			g_player.aModel[nCntModel].pos.y = (g_playerDefaultKey[nCntModel].fPosY + g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fPosY) + keyDiff[nCntModel].fPosY * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].nFrame);
			g_player.aModel[nCntModel].pos.z = (g_playerDefaultKey[nCntModel].fPosZ + g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fPosZ) + keyDiff[nCntModel].fPosZ * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].nFrame);

			//if (g_player.nKey <= g_player.aMotionInfo[g_player.motionType].nNumKey - 2 || g_player.bLoopMotion == true)
			//{
			//	g_player.aModel[nCntModel].rot.x = g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotX + keyDiff[nCntModel].fRotX * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].nFrame);
			//	g_player.aModel[nCntModel].rot.y = g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotY + keyDiff[nCntModel].fRotY * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].nFrame);
			//	g_player.aModel[nCntModel].rot.z = g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotZ + keyDiff[nCntModel].fRotZ * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].nFrame);
			//}

			g_player.aModel[nCntModel].rot.x = g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotX + keyDiff[nCntModel].fRotX * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].nFrame);
			g_player.aModel[nCntModel].rot.y = g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotY + keyDiff[nCntModel].fRotY * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].nFrame);
			g_player.aModel[nCntModel].rot.z = g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotZ + keyDiff[nCntModel].fRotZ * ((float)g_player.nCounterMotion / (float)g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].nFrame);
		}

		//現在キーの再生フレーム数に到達したら
		if (g_player.nCounterMotion >= g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].nFrame)
		{
			//次のキーに
			g_player.nKey++;

			//モーションカウンタリセット
			ResetMotion(false, true, false, false);
		}

		//現在キーがモーションのキー数に到達したら
		if (g_player.nKey >= g_player.aMotionInfo[g_player.motionType].nNumKey - 1 && g_player.bLoopMotion == false)
		{//ループしない場合

			//モーションリセット
			ResetMotion(false, true, true, true);
		}
		else if (g_player.nKey >= g_player.aMotionInfo[g_player.motionType].nNumKey && g_player.bLoopMotion == true)
		{//ループの場合

			//カウンタとキーをリセット
			ResetMotion(false, true, true, false);
		}
	}
	else if (g_player.bPlayMotion == false)
	{
		//モーションリセット
		ResetMotion(false, true, true, false);
	}
}

/* モーションリセット関数 */
void ResetMotion(bool bPartsReset, bool bCounterReset, bool bKeyReset, bool bMotionTrig)
{
	if (bPartsReset == true)
	{
		//プレイヤーのパーツ回転初期化
		for (int nCnt = 0; nCnt < g_player.nNumModel; nCnt++)
		{
			g_player.aModel[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_player.aModel[nCnt].pos.x = g_playerDefaultKey[nCnt].fPosX;
			g_player.aModel[nCnt].pos.y = g_playerDefaultKey[nCnt].fPosY;
			g_player.aModel[nCnt].pos.z = g_playerDefaultKey[nCnt].fPosZ;
		}
	}

	if (bCounterReset == true)
	{
		//モーションカウンタ初期化
		g_player.nCounterMotion = 0;
	}

	if (bKeyReset == true)
	{
		//現在キーを初期化
		g_player.nKey = 0;
	}

	if (bMotionTrig == true)
	{
		g_player.bPlayMotion = false;
	}
}

/* モーション開始関数 */
void StartMotion(MOTIONTYPE motionType)
{
	//モーションタイプが変更されようとしていたら
	if (motionType != g_player.motionType)
	{
		if (g_player.bPlayMotion == true && g_player.bLoopMotion == false)
		{

		}
		else
		{
			//モーション変更
			g_player.motionType = motionType;		// モーションタイプ変更
			ResetMotion(false, true, true, false);	// モーションリセット
		}

	}
	
	//モーション開始
	if (g_player.bPlayMotion == false)
	{
		g_player.bPlayMotion = true;
	}

}

/* キャラクターのモデル情報を読み込む関数 */
void ReadPlayerInfo(void)
{
	FILE *pFile;
	char cCharRead[READROW][128];
	char cEqual[2];

	int nCountModelNum = 0;
	int nCountModelIdx = 0;
	int nCountModelParent = 0;
	int nCountModelPos = 0;
	int nCountModelRot = 0;

	pFile = fopen("player.txt", "r");
	if (pFile != NULL)
	{
		//1028行探す
		for (int nCntRow = 0; nCntRow < READROW; nCntRow++)
		{
			//文字列読み込み
			fscanf(pFile, "%s", &cCharRead[nCntRow][0]);

			//SCRIPTの文字列を見つけたら
			if (strcmp(&cCharRead[nCntRow][0], "SCRIPT") == 0)
			{
				for (int nCntRow = 0; nCntRow < READROW; nCntRow++)
				{
					//次の文字列読み込み
					fscanf(pFile, "%s", &cCharRead[nCntRow][0]);

					if (strcmp(&cCharRead[nCntRow][0], "NUM_MODEL") == 0)
					{
						//次の文字列
						fscanf(pFile, "%s", &cEqual[0]);

						//イコール見つかったら
						if (strcmp(&cEqual[0], "=") == 0)
						{
							//イコールの次の整数を変数に入れる
							fscanf(pFile, "%d", &g_ModelInfo.nModelNum);
						}
					}

					if (strcmp(&cCharRead[nCntRow][0], "MODEL_FILENAME") == 0)
					{
						//次の文字列
						fscanf(pFile, "%s", &cEqual[0]);

						//イコール見つけたら
						if (strcmp(&cEqual[0], "=") == 0)
						{
							//イコールの次の文字列を配列に入れる
							fscanf(pFile, "%s", &g_ModelInfo.cModelFileName[nCountModelNum][0]);

							//カウントアップ
							nCountModelNum++;
						}
					}

					if (strcmp(&cCharRead[nCntRow][0], "CHARACTERSET") == 0)
					{
						for (int nCntCSET = 0; nCntCSET < READROW; nCntCSET++)
						{
							fscanf(pFile, "%s", &cCharRead[nCntCSET][0]);

							//PARTSSET見つけた
							if (strcmp(&cCharRead[nCntCSET][0], "PARTSSET") == 0)
							{
								for (int nCntPSET = 0; nCntCSET < READROW; nCntCSET++)
								{
									fscanf(pFile, "%s", &cCharRead[nCntCSET][0]);

									//INDEX
									if (strcmp(&cCharRead[nCntCSET][0], "INDEX") == 0)
									{
										//次の文字列
										fscanf(pFile, "%s", &cEqual[0]);

										//イコール見つけたら
										if (strcmp(&cEqual[0], "=") == 0)
										{
											fscanf(pFile, "%d", &g_ModelInfo.nModelIdx[nCountModelIdx]);

											nCountModelIdx++;
										}
									}

									//PARENT
									if (strcmp(&cCharRead[nCntCSET][0], "PARENT") == 0)
									{
										//次の文字列
										fscanf(pFile, "%s", &cEqual[0]);

										//イコール見つけたら
										if (strcmp(&cEqual[0], "=") == 0)
										{
											fscanf(pFile, "%d", &g_ModelInfo.nModelParent[nCountModelParent]);

											nCountModelParent++;
										}
									}

									//POS
									if (strcmp(&cCharRead[nCntCSET][0], "POS") == 0)
									{
										//次の文字列
										fscanf(pFile, "%s", &cEqual[0]);

										//イコール見つけたら
										if (strcmp(&cEqual[0], "=") == 0)
										{
											for (int nCntVector3 = 0; nCntVector3 < 3; nCntVector3++)
											{
												fscanf(pFile, "%f", &g_ModelInfo.fModelPos[nCountModelPos][nCntVector3]);
											}
											nCountModelPos++;
										}
									}

									//ROT
									if (strcmp(&cCharRead[nCntCSET][0], "ROT") == 0)
									{
										//次の文字列
										fscanf(pFile, "%s", &cEqual[0]);

										//イコール見つけたら
										if (strcmp(&cEqual[0], "=") == 0)
										{
											for (int nCntVector3 = 0; nCntVector3 < 3; nCntVector3++)
											{
												fscanf(pFile, "%f", &g_ModelInfo.fModelRot[nCountModelRot][nCntVector3]);
											}
											nCountModelRot++;
										}
									}

									//END_PARTSSET見つけたらループ抜ける
									if (strcmp(&cCharRead[nCntCSET][0], "END_PARTSSET") == 0)
									{
										break;
									}
								}
							}

							//END_CHARACTERSET見つけたらループ抜ける
							if (strcmp(&cCharRead[nCntCSET][0], "END_CHARACTERSET") == 0)
							{
								break;
							}
						}
					}
				}
				break;
			}
		}
	}
}