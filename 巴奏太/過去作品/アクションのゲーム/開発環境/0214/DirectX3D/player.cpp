#define _CRT_SECURE_NO_WARNINGS
//-----------------------------------------------------------------------------
//
// �v���C���[�̏��� [player.cpp]
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
// �}�N����`
//-----------------------------------------------------------------------------
#define PLAYER_MODEL_AMOUNT (10)
#define PLAYER_SMOOTHTURN_SPEED (0.1f)											// ���炩�ɐU��������x
#define READROW (1028)	// �t�@�C���ǂݍ��݂œǂލő吔

//-----------------------------------------------------------------------------
// ���f�����̍\����
//-----------------------------------------------------------------------------
typedef struct
{
	int nModelNum;										// ���f���̐�	
	int nModelIdx[PLAYER_MODEL_AMOUNT];					// ���f���̔ԍ�
	int nModelParent[PLAYER_MODEL_AMOUNT];				// ���f���̐e
	char cModelFileName[PLAYER_MODEL_AMOUNT][128];		// ���f���̃t�@�C����
	float fModelPos[PLAYER_MODEL_AMOUNT][3];			// ���f���̈ʒu(�I�t�Z�b�g)
	float fModelRot[PLAYER_MODEL_AMOUNT][3];			// ���f���̌���
} ModelInfo;

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
void MovePlayer(float fMoveAngleDegree, float fMoveSpeed);
HRESULT LoadXFile(const char* cXFileName, int nCountModel);
void PlayerSmoothTurn(void);
void PlayMotion(bool bPlayMotion);
void ResetMotion(bool bPartsReset, bool bCounterReset, bool bKeyReset, bool bMotionTrig);
void StartMotion(MOTIONTYPE motionType);
void ReadPlayerInfo(void);

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
Player g_player;																// �v���C���[�̏��
D3DXVECTOR3 g_MotionKey[4][10];													// ���[�V�����̃L�[
KEY g_playerDefaultKey[10];
ModelInfo g_ModelInfo;														// �ǂݍ��񂾃��f���̏��

//-----------------------------------------------------------------------------
// ����������
//-----------------------------------------------------------------------------
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�̎擾

	//�v���C���[�̏����ݒ�
	g_player.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu�̏����ݒ�
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����̏����ݒ�
	g_player.bHit = false;
	g_player.bInvincible = false;
	g_player.dwTime = NULL;
	g_player.nLife = PLAYER_HEALTH;
	g_player.nLifeMax = PLAYER_HEALTH;

	//�e�L�X�g�ǂݍ���
	ReadPlayerInfo();

	//�ǂݍ��񂾏����g����X�t�@�C���ǂݍ���
	for (int nCntModel = 0; nCntModel < g_ModelInfo.nModelNum; nCntModel++)
	{
		LoadXFile(&g_ModelInfo.cModelFileName[nCntModel][0], nCntModel);
	}

	//���f���̃p�[�c��
	g_player.nNumModel = g_ModelInfo.nModelNum;

	//�e�p�[�c�̊K�w�\���ݒ�
	for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
	{
		g_player.aModel[nCntModel].nIdxModelParent = g_ModelInfo.nModelParent[nCntModel];
		g_player.aModel[nCntModel].pos = D3DXVECTOR3(g_ModelInfo.fModelPos[nCntModel][0], g_ModelInfo.fModelPos[nCntModel][1], g_ModelInfo.fModelPos[nCntModel][2]);
		g_player.aModel[nCntModel].rot = D3DXVECTOR3(g_ModelInfo.fModelRot[nCntModel][0], g_ModelInfo.fModelRot[nCntModel][1], g_ModelInfo.fModelRot[nCntModel][2]);
	}

	//�����L�[
	for (int nCnt = 0; nCnt < g_player.nNumModel; nCnt++)
	{
		g_playerDefaultKey[nCnt] = KeyPosRot(g_player.aModel[nCnt].pos.x, g_player.aModel[nCnt].pos.y, g_player.aModel[nCnt].pos.z, 0, 0, 0);
	}

	//���[�V�����̐ݒ�
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
// �I������
//-----------------------------------------------------------------------------
void UninitPlayer(void)
{
	for (int nCntModel = 0; nCntModel < 2; nCntModel++)
	{
		//���b�V���̔j��
		if (g_player.aModel[nCntModel].pMeshModel != NULL)
		{
			g_player.aModel[nCntModel].pMeshModel->Release();
			g_player.aModel[nCntModel].pMeshModel = NULL;
		}

		//�}�e���A���̔j��
		if (g_player.aModel[nCntModel].pBuffMatModel != NULL)
		{
			g_player.aModel[nCntModel].pBuffMatModel->Release();
			g_player.aModel[nCntModel].pBuffMatModel = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
// �X�V����
//-----------------------------------------------------------------------------
void UpdatePlayer(void)
{
	DWORD dwCurrentTime = timeGetTime();	// ���ݎ���

	//�ʒu�ۑ�
	g_player.posOld = g_player.pos;

	//�ړ��ʒǉ�
	g_player.pos += g_player.move;

	//�ړ��ʌ���
	g_player.move.x += (0 - g_player.move.x) * 0.2f;
	g_player.move.z += (0 - g_player.move.z) * 0.2f;

	//�d��
	g_player.move.y -= 0.1f;

	//�ړ�����
	if (g_player.pos.x > FIELD_BORDER) g_player.pos.x = FIELD_BORDER;
	if (g_player.pos.x < -FIELD_BORDER) g_player.pos.x = -FIELD_BORDER;
	if (g_player.pos.z > FIELD_BORDER) g_player.pos.z = FIELD_BORDER;
	if (g_player.pos.z < -FIELD_BORDER) g_player.pos.z = -FIELD_BORDER;

	/* ���f���̈ړ� */
	if (GetKeyboardPress(DIK_W) == true)
	{
		if (GetKeyboardPress(DIK_D) == true) MovePlayer(45.0f, PLAYER_MOVESPEED);			// �E�O
		else if (GetKeyboardPress(DIK_A) == true) MovePlayer(-45.0f, PLAYER_MOVESPEED);		// ���O
		else MovePlayer(0.0f, PLAYER_MOVESPEED);												// �O
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{
		if (GetKeyboardPress(DIK_D) == true) MovePlayer(135.0f, PLAYER_MOVESPEED);			// �E���
		else if (GetKeyboardPress(DIK_A) == true) MovePlayer(-135.0f, PLAYER_MOVESPEED);		// �����
		else MovePlayer(-180.0f, PLAYER_MOVESPEED);												// ���
	}
	else if (GetKeyboardPress(DIK_A) == true) MovePlayer(-90.0f, PLAYER_MOVESPEED);			// ��
	else if (GetKeyboardPress(DIK_D) == true) MovePlayer(90.0f, PLAYER_MOVESPEED);			// �E

	/* �v���C���[�̐U����������炩�ɂ��� */
	PlayerSmoothTurn();

	/* ���[�V�����Đ� */
	PlayMotion(g_player.bPlayMotion);

	//�s���Ƀ��[�V��������
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

	//�U�����͈ړ��O�ɂ���
	if (g_player.motionType == MOTIONTYPE_ATTACK)
	{
		g_player.move = D3DXVECTOR3(0, 0, 0);
	}

	//���Ԍo�߂Ŗ��G���ԉ���
	if (dwCurrentTime - g_player.dwTime >= PLAYER_INVINCIBLE_TIME)
	{
		//���G����
		g_player.bInvincible = false;
	}

	//�̗͂Ȃ��Ȃ��
	if (g_player.nLife <= 0)
	{
		GAMEOVER *pGameOver = GetGameOver();	// �Q�[���I�[�o�[��ʂ̏����擾
		RESULT *pResult = GetResult();			// ���U���g�̏����擾

		//�Q�[���I�[�o�[��ʕ\��
		pGameOver->bDraw = true;
		pGameOver->bGameOver = true;
		pResult->bGameOver = true;

		//���U���g��ʂɃt�F�[�h�A�E�g
		SetFade(FADE_OUT, MODE_RESULT);
	}
}

//-----------------------------------------------------------------------------
// �`�揈��
//-----------------------------------------------------------------------------
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();									// �f�o�C�X�擾
	D3DXMATRIX mtxRot, mtxTrans;												// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;														// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;															// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL *pMatAlt;														// �ウ�̃}�e���A��

	//�v���C���[�̃��[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_player.mtxWorld);

	//�v���C���[�̌������f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxRot);

	//�v���C���[�̈ʒu���f
	D3DXMatrixTranslation(&mtxTrans, g_player.pos.x, g_player.pos.y, g_player.pos.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxTrans);

	//�v���C���[�̃��[���h�}�g���b�N�X�ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_player.mtxWorld);

	//���݂̃}�e���A���擾
	pDevice->GetMaterial(&matDef);

	for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
	{
		D3DXMATRIX mtxRotModel, mtxTransModel;	// �v�Z�p�}�g���b�N�X
		D3DXMATRIX mtxParent;					// �e�̃}�g���b�N�X

		//�e�p�[�c�̃��[���h�}�g���b�N�X������
		D3DXMatrixIdentity(&g_player.aModel[nCntModel].mtxWorld);

		//�e�p�[�c�̌����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_player.aModel[nCntModel].rot.y, g_player.aModel[nCntModel].rot.x, g_player.aModel[nCntModel].rot.z);
		D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld, &g_player.aModel[nCntModel].mtxWorld, &mtxRotModel);

		//�e�p�[�c�̈ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTransModel, g_player.aModel[nCntModel].pos.x, g_player.aModel[nCntModel].pos.y, g_player.aModel[nCntModel].pos.z);
		D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld, &g_player.aModel[nCntModel].mtxWorld, &mtxTransModel);

		//�e�p�[�c�̐e�̃}�g���b�N�X��ݒ�
		if (g_player.aModel[nCntModel].nIdxModelParent != -1)
		{
			mtxParent = g_player.aModel[g_player.aModel[nCntModel].nIdxModelParent].mtxWorld;
		}
		else
		{
			mtxParent = g_player.mtxWorld;	// �v���C���[�̃}�g���b�N�X��ݒ�
		}

		//�Z�o�����e�p�[�c�̃��[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���|�����킹��
		D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld, &g_player.aModel[nCntModel].mtxWorld, &mtxParent);

		//�e�p�[�c�̃��[���h�}�g���b�N�X�ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_player.aModel[nCntModel].mtxWorld);
		
		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_player.aModel[nCntModel].pBuffMatModel->GetBufferPointer();
		pMatAlt = (D3DXMATERIAL*)g_player.aModel[nCntModel].pBuffMatModel->GetBufferPointer();

		//���G��Ԃ����ă��f���̐F��ς���
		if (g_player.bInvincible == true)
		{//���G���
			pMatAlt->MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f);
			for (int nCntMat = 0; nCntMat < (int)g_player.aModel[nCntModel].nNumMatModel; nCntMat++)
			{
				//�}�e���A���ݒ�
				pDevice->SetMaterial(&pMatAlt[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, NULL);

				//���f���p�[�c�̕`��
				g_player.aModel[nCntModel].pMeshModel->DrawSubset(nCntMat);
			}
		}
		else if (g_player.bInvincible == false)
		{//�ʏ���
			pMat->MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0, 1.0f, 1.0f);
			for (int nCntMat = 0; nCntMat < (int)g_player.aModel[nCntModel].nNumMatModel; nCntMat++)
			{
				//�}�e���A���ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, NULL);

				//���f���p�[�c�̕`��
				g_player.aModel[nCntModel].pMeshModel->DrawSubset(nCntMat);
			}
		}
		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		//pMat = (D3DXMATERIAL*)g_player.aModel[nCntModel].pBuffMatModel->GetBufferPointer();

		//for (int nCntMat = 0; nCntMat < (int)g_player.aModel[nCntModel].nNumMatModel; nCntMat++)
		//{
		//	//�}�e���A���ݒ�
		//	pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//	//�e�N�X�`���̐ݒ�
		//	pDevice->SetTexture(0, NULL);

		//	//���f���p�[�c�̕`��
		//	g_player.aModel[nCntModel].pMeshModel->DrawSubset(nCntMat);
		//}
	}

	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);

}

//-----------------------------------------------------------------------------
// �v���C���[�̏����擾
//-----------------------------------------------------------------------------
Player *GetPlayer(void)
{
	return &g_player;
}

/* ���f�����ړ�������֐� */
void MovePlayer(float fMoveAngleDegree, float fMoveSpeed)
{
	Camera *pCamera = GetCamera();	// �J�����̏��擾

	g_player.move.x += sinf(pCamera->rot.y + D3DXToRadian(fMoveAngleDegree)) * fMoveSpeed;
	g_player.move.z += cosf(pCamera->rot.y + D3DXToRadian(fMoveAngleDegree)) * fMoveSpeed;
	g_player.rotDest.y = pCamera->rot.y + D3DXToRadian(fMoveAngleDegree) + D3DX_PI;
}

/* X�t�@�C�����烂�f����ǂݍ��ފ֐�*/
HRESULT LoadXFile(const char* cXFileName, int nCountModel)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�擾
	HRESULT hres;

	//X�t�@�C���̓ǂݍ���
	hres = D3DXLoadMeshFromX(
		cXFileName,										// �ǂݍ��ރt�@�C����
		D3DXMESH_SYSTEMMEM,								// �Œ�
		pDevice,										// �f�o�C�X
		NULL,											// NULL�Œ�
		&g_player.aModel[nCountModel].pBuffMatModel,		// �}�e���A��
		NULL,											// NULL�Œ�
		&g_player.aModel[nCountModel].nNumMatModel,		// �}�e���A����
		&g_player.aModel[nCountModel].pMeshModel);		// ���b�V��

	return hres;
}

/* �L�[��Pos,Rot���ȈՓI�ɓ��͂�����֐�*/
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

/* �L�����̉�]�����炩�ɂ���֐� */
void PlayerSmoothTurn(void)
{
	D3DXVECTOR3 RotDiff;

	//�����v�Z
	RotDiff.y = g_player.rotDest.y - g_player.rot.y;

	//������D3DX_PI�ȏ�(�����ȏ�)�̏ꍇ�A�t��]
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

	// ��]�̏C�� (3.14��������}�t��)
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

/* ���[�V�����̍Đ��֐� */
void PlayMotion(bool bPlayMotion)
{
	if (bPlayMotion == true)
	{
		//���[�v
		g_player.bLoopMotion = g_player.aMotionInfo[g_player.motionType].bLoop;

		//���[�V�����J�E���g�A�b�v
		g_player.nCounterMotion++;

		//���f��������
		for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
		{
			//�����v�Z�p
			KEY keyDiff[20];

			//���݂̃L�[�Ǝ��̃L�[�Ƃ̍������v�Z
			if (g_player.nKey >= g_player.aMotionInfo[g_player.motionType].nNumKey - 1 && g_player.bLoopMotion == true)
			{	// ���[�v�̏ꍇ
				keyDiff[nCntModel].fPosX = g_player.aMotionInfo[g_player.motionType].aKeyInfo[0].aKey[nCntModel].fPosX - g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fPosX;
				keyDiff[nCntModel].fPosY = g_player.aMotionInfo[g_player.motionType].aKeyInfo[0].aKey[nCntModel].fPosY - g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fPosY;
				keyDiff[nCntModel].fPosZ = g_player.aMotionInfo[g_player.motionType].aKeyInfo[0].aKey[nCntModel].fPosZ - g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fPosZ;
				keyDiff[nCntModel].fRotX = g_player.aMotionInfo[g_player.motionType].aKeyInfo[0].aKey[nCntModel].fRotX - g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotX;
				keyDiff[nCntModel].fRotY = g_player.aMotionInfo[g_player.motionType].aKeyInfo[0].aKey[nCntModel].fRotY - g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotY;
				keyDiff[nCntModel].fRotZ = g_player.aMotionInfo[g_player.motionType].aKeyInfo[0].aKey[nCntModel].fRotZ - g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotZ;
			}
			else
			{	// ����ȊO
				keyDiff[nCntModel].fPosX = g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey + 1].aKey[nCntModel].fPosX - g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fPosX;
				keyDiff[nCntModel].fPosY = g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey + 1].aKey[nCntModel].fPosY - g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fPosY;
				keyDiff[nCntModel].fPosZ = g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey + 1].aKey[nCntModel].fPosZ - g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fPosZ;
				keyDiff[nCntModel].fRotX = g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey + 1].aKey[nCntModel].fRotX - g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotX;
				keyDiff[nCntModel].fRotY = g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey + 1].aKey[nCntModel].fRotY - g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotY;
				keyDiff[nCntModel].fRotZ = g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey + 1].aKey[nCntModel].fRotZ - g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntModel].fRotZ;
			}

			//���݂̃L�[�ɍ��������[�V�����J�E���g�Ŋ��������𑫂������̂�rot�ɑ��
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

		//���݃L�[�̍Đ��t���[�����ɓ��B������
		if (g_player.nCounterMotion >= g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].nFrame)
		{
			//���̃L�[��
			g_player.nKey++;

			//���[�V�����J�E���^���Z�b�g
			ResetMotion(false, true, false, false);
		}

		//���݃L�[�����[�V�����̃L�[���ɓ��B������
		if (g_player.nKey >= g_player.aMotionInfo[g_player.motionType].nNumKey - 1 && g_player.bLoopMotion == false)
		{//���[�v���Ȃ��ꍇ

			//���[�V�������Z�b�g
			ResetMotion(false, true, true, true);
		}
		else if (g_player.nKey >= g_player.aMotionInfo[g_player.motionType].nNumKey && g_player.bLoopMotion == true)
		{//���[�v�̏ꍇ

			//�J�E���^�ƃL�[�����Z�b�g
			ResetMotion(false, true, true, false);
		}
	}
	else if (g_player.bPlayMotion == false)
	{
		//���[�V�������Z�b�g
		ResetMotion(false, true, true, false);
	}
}

/* ���[�V�������Z�b�g�֐� */
void ResetMotion(bool bPartsReset, bool bCounterReset, bool bKeyReset, bool bMotionTrig)
{
	if (bPartsReset == true)
	{
		//�v���C���[�̃p�[�c��]������
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
		//���[�V�����J�E���^������
		g_player.nCounterMotion = 0;
	}

	if (bKeyReset == true)
	{
		//���݃L�[��������
		g_player.nKey = 0;
	}

	if (bMotionTrig == true)
	{
		g_player.bPlayMotion = false;
	}
}

/* ���[�V�����J�n�֐� */
void StartMotion(MOTIONTYPE motionType)
{
	//���[�V�����^�C�v���ύX����悤�Ƃ��Ă�����
	if (motionType != g_player.motionType)
	{
		if (g_player.bPlayMotion == true && g_player.bLoopMotion == false)
		{

		}
		else
		{
			//���[�V�����ύX
			g_player.motionType = motionType;		// ���[�V�����^�C�v�ύX
			ResetMotion(false, true, true, false);	// ���[�V�������Z�b�g
		}

	}
	
	//���[�V�����J�n
	if (g_player.bPlayMotion == false)
	{
		g_player.bPlayMotion = true;
	}

}

/* �L�����N�^�[�̃��f������ǂݍ��ފ֐� */
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
		//1028�s�T��
		for (int nCntRow = 0; nCntRow < READROW; nCntRow++)
		{
			//������ǂݍ���
			fscanf(pFile, "%s", &cCharRead[nCntRow][0]);

			//SCRIPT�̕��������������
			if (strcmp(&cCharRead[nCntRow][0], "SCRIPT") == 0)
			{
				for (int nCntRow = 0; nCntRow < READROW; nCntRow++)
				{
					//���̕�����ǂݍ���
					fscanf(pFile, "%s", &cCharRead[nCntRow][0]);

					if (strcmp(&cCharRead[nCntRow][0], "NUM_MODEL") == 0)
					{
						//���̕�����
						fscanf(pFile, "%s", &cEqual[0]);

						//�C�R�[������������
						if (strcmp(&cEqual[0], "=") == 0)
						{
							//�C�R�[���̎��̐�����ϐ��ɓ����
							fscanf(pFile, "%d", &g_ModelInfo.nModelNum);
						}
					}

					if (strcmp(&cCharRead[nCntRow][0], "MODEL_FILENAME") == 0)
					{
						//���̕�����
						fscanf(pFile, "%s", &cEqual[0]);

						//�C�R�[����������
						if (strcmp(&cEqual[0], "=") == 0)
						{
							//�C�R�[���̎��̕������z��ɓ����
							fscanf(pFile, "%s", &g_ModelInfo.cModelFileName[nCountModelNum][0]);

							//�J�E���g�A�b�v
							nCountModelNum++;
						}
					}

					if (strcmp(&cCharRead[nCntRow][0], "CHARACTERSET") == 0)
					{
						for (int nCntCSET = 0; nCntCSET < READROW; nCntCSET++)
						{
							fscanf(pFile, "%s", &cCharRead[nCntCSET][0]);

							//PARTSSET������
							if (strcmp(&cCharRead[nCntCSET][0], "PARTSSET") == 0)
							{
								for (int nCntPSET = 0; nCntCSET < READROW; nCntCSET++)
								{
									fscanf(pFile, "%s", &cCharRead[nCntCSET][0]);

									//INDEX
									if (strcmp(&cCharRead[nCntCSET][0], "INDEX") == 0)
									{
										//���̕�����
										fscanf(pFile, "%s", &cEqual[0]);

										//�C�R�[����������
										if (strcmp(&cEqual[0], "=") == 0)
										{
											fscanf(pFile, "%d", &g_ModelInfo.nModelIdx[nCountModelIdx]);

											nCountModelIdx++;
										}
									}

									//PARENT
									if (strcmp(&cCharRead[nCntCSET][0], "PARENT") == 0)
									{
										//���̕�����
										fscanf(pFile, "%s", &cEqual[0]);

										//�C�R�[����������
										if (strcmp(&cEqual[0], "=") == 0)
										{
											fscanf(pFile, "%d", &g_ModelInfo.nModelParent[nCountModelParent]);

											nCountModelParent++;
										}
									}

									//POS
									if (strcmp(&cCharRead[nCntCSET][0], "POS") == 0)
									{
										//���̕�����
										fscanf(pFile, "%s", &cEqual[0]);

										//�C�R�[����������
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
										//���̕�����
										fscanf(pFile, "%s", &cEqual[0]);

										//�C�R�[����������
										if (strcmp(&cEqual[0], "=") == 0)
										{
											for (int nCntVector3 = 0; nCntVector3 < 3; nCntVector3++)
											{
												fscanf(pFile, "%f", &g_ModelInfo.fModelRot[nCountModelRot][nCntVector3]);
											}
											nCountModelRot++;
										}
									}

									//END_PARTSSET�������烋�[�v������
									if (strcmp(&cCharRead[nCntCSET][0], "END_PARTSSET") == 0)
									{
										break;
									}
								}
							}

							//END_CHARACTERSET�������烋�[�v������
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