//-----------------------------------------------------------------------------
//
// �^�C�g���̏��� �w�b�_�[ [title.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _TITLE_H_
#define _TITLE_H_
#include "main.h"

//-----------------------------------------------------------------------------
// �}�N��
//-----------------------------------------------------------------------------
#define TITLE_BG_WIDTH (1920)
#define TITLE_BG_HEIGHT (1080)
#define TITLE_LOGO_WIDTH (1299)
#define TITLE_LOGO_HEIGHT (153)
#define TITLE_PRESS_WIDTH (755)
#define TITLE_PRESS_HEIGHT (94)
#define TITLE_LOGO_OFFSET (-200)
#define TITLE_PRESS_OFFSET (200)

//-----------------------------------------------------------------------------
// TITLE�\����
//-----------------------------------------------------------------------------
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;			// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9 pTexture;				// �|���S���̃e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 pos;							// �ʒu
} TITLE;

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
HRESULT InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);

#endif