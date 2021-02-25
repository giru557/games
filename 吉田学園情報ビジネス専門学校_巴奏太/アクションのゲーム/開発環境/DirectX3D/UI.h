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

#define ENEMYUI_OBJ_AMOUNT (2)					// �I�u�W�F�N�g��
#define ENEMYUI_SHOWDIGIT (2)					// �\�����鐔���̐�
#define ENEMYUI_TEXTURE_SPLIT_NUM (10)			// �����e�N�X�`���̕�����
#define ENEMYUI_SPACE_BETWEEN_CHAR (100)			// �����Ԃ̃X�؁[�X
#define ENEMYUI_CHAR_WIDTH (100)				// �����̕�
#define ENEMYUI_CHAR_HEIGHT (150)				// �����̍���
#define ENEMYUI_CHAR_POS_X (SCREENWIDTH / 2)	// �����̈ʒuX
#define ENEMYUI_CHAR_POS_Y (0)					// �����̈ʒuY
#define ENEMYUI_PICTURE_WIDTH (200)				// �摜�̕�
#define ENEMYUI_PICTURE_HEIGHT (100)			// �摜�̍���
#define ENEMYUI_PICTURE_POS_X (0)				// �摜�̈ʒuX
#define ENEMYUI_PICTURE_POS_Y (0)				// �摜�̈ʒuY

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
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------
HRESULT InitUI(void);
void UninitUI(void);
void UpdateUI(void);
void DrawUI(void);
HPBAR *GetHPBAR(void);
GAMEOVER *GetGameOver(void);

#endif

