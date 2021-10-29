//-----------------------------------------------------------------------------
//
// UI �w�b�_�[ [UI.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _UI_H_
#define _UI_H_
#include "main.h"

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
#define HEALTH_OBJ_AMOUNT (2)			// �I�u�W�F��
#define HEALTH_FRONT_POS_X (0)			// HP�o�[�̈ʒu X (�O)
#define HEALTH_FRONT_POS_Y (0)			// HP�o�[�̈ʒu Y (�O)
#define HEALTH_BACK_POS_X (0)			// HP�o�[�̈ʒu X (��)
#define HEALTH_BACK_POS_Y (0)			// HP�o�[�̈ʒu Y (��)
#define HEALTH_BAR_WIDTH (66 / 2 * HEALTH_SIZE_MULTIPLIER)
#define HEALTH_BAR_HEIGHT (19 / 2 * HEALTH_SIZE_MULTIPLIER)
#define HEALTH_WIDTH (51 * HEALTH_SIZE_MULTIPLIER)
#define HEALTH_HEIGHT (19 / 2 * HEALTH_SIZE_MULTIPLIER)
#define HEALTH_SIZE_MULTIPLIER (5)

#define ENEMYUI_OBJ_AMOUNT (2)														// �I�u�W�F�N�g��
#define ENEMYUI_SHOWDIGIT (2)														// �\�����鐔���̐�
#define ENEMYUI_TEXTURE_SPLIT_NUM (10)												// �����e�N�X�`���̕�����
#define ENEMYUI_SPACE_BETWEEN_CHAR (100)											// �����Ԃ̃X�؁[�X
#define ENEMYUI_CHAR_WIDTH ((251 / 10) * ENEMYUI_CHAR_SIZE_MULTIPLIER)				// �����̕�
#define ENEMYUI_CHAR_HEIGHT (38 * ENEMYUI_CHAR_SIZE_MULTIPLIER)						// �����̍���
#define ENEMYUI_CHAR_SIZE_MULTIPLIER (2.5f)											// �����̃T�C�Y�{��
#define ENEMYUI_CHAR_POS_X (SCREEN_WIDTH / 2 + 50)										// �����̈ʒuX
#define ENEMYUI_CHAR_POS_Y (5)														// �����̈ʒuY
#define ENEMYUI_PICTURE_WIDTH (550 * ENEMYUI_PICTURE_SIZE_MULTIPLIER)				// �摜�̕�
#define ENEMYUI_PICTURE_HEIGHT (150 * ENEMYUI_PICTURE_SIZE_MULTIPLIER)				// �摜�̍���
#define ENEMYUI_PICTURE_SIZE_MULTIPLIER (0.7)										// �摜�̃T�C�Y�{��
#define ENEMYUI_PICTURE_POS_X (SCREEN_WIDTH / 2 - 250)								// �摜�̈ʒuX
#define ENEMYUI_PICTURE_POS_Y (0)													// �摜�̈ʒuY

#define MINUSENEMY_WIDTH (450 * MINUSENEMY_SIZE)
#define MINUSENEMY_HEIGHT (120 * MINUSENEMY_SIZE)
#define MINUSENEMY_SIZE (0.75f)
#define MINUSENEMY_POS_X (SCREEN_WIDTH / 2 + 250)
#define MINUSENEMY_POS_Y (SCREEN_HEIGHT / 2 - 50)
#define MINUSENEMY_AMOUNT_MAX (16)

//-----------------------------------------------------------------------------
// �̗̓o�[�̍\����
//-----------------------------------------------------------------------------
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;
	LPDIRECT3DTEXTURE9 pTexture;
	D3DXVECTOR3 pos;
} HPBAR;

//-----------------------------------------------------------------------------
// �Q�[���I�[�o�[�̍\����
//-----------------------------------------------------------------------------
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff[2];
	LPDIRECT3DTEXTURE9 pTexture[3];
	D3DXVECTOR3 pos;
	bool bGameOver;
	bool bDraw;
} GAMEOVER;

//-----------------------------------------------------------------------------
// �G�̎c�ʂ̍\����
//-----------------------------------------------------------------------------
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff[2];
	LPDIRECT3DTEXTURE9 pTexture[2];
	D3DXVECTOR3 pos[2];
	int nAmount;
}EnemyUI;

//-----------------------------------------------------------------------------
// �}�C�i�X�\���̍\����
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXMATRIX mtxWorld;										// ���[���h�}�g���b�N�X
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;							// ���_�o�b�t�@
	LPDIRECT3DTEXTURE9 pTexture;								// �e�N�X�`��
	D3DXVECTOR3 pos[MINUSENEMY_AMOUNT_MAX];						// �ʒu
	bool bUse[MINUSENEMY_AMOUNT_MAX];							// �g�p��
	float fMultiplier[MINUSENEMY_AMOUNT_MAX];					// �����x�̊���
} MinusEnemy;

//-----------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------
HRESULT InitUI(void);
void UninitUI(void);
void UpdateUI(void);
void DrawUI(void);
HPBAR *GetHPBAR(void);
GAMEOVER *GetGameOver(void);
EnemyUI *GetEnemyRemain(void);
void ShowEnemyDestroyed(void);

#endif

