//=============================================================================
//
// �X�v���C�g�V�[�g�̏��� [spritesheet.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _SPRITESHEET_H_
#define _SPRITESHEET_H_

//*****************************************************************************
// �w�b�_�[�̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "scene2d.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SPRITESHEET_DRAW_PRIORITY (2)

#define SPRITESHEET_EXPLOSION_COLUMN (7.0f)
#define SPRITESHEET_EXPLOSION_ROW (1.0f)
#define SPRITESHEET_EXPLOSION_ANIMSPEED (6)

#define SPRITESHEET_EXPLOSION2_COLUMN (30.0f)
#define SPRITESHEET_EXPLOSION2_ROW (1.0f)
#define SPRITESHEET_EXPLOSION2_ANIMSPEED (5)

#define SPRITESHEET_TEST_COLUMN (4.0f)
#define SPRITESHEET_TEST_ROW (2.0f)
#define SPRITESHEET_TEST_ANIMSPEED (30)

#define EXPLOSION_ANIM_SPEED (5)
#define EXPLOSION_ANIM_SPLIT (7.0f)
#define EXPLOSION_SIZE_DEFAULT (150.0f)

//*****************************************************************************
// �X�v���C�g�V�[�g�N���X ( �p����: 2D�|���S�� [scene2D] )
//*****************************************************************************
class CSpriteSheet : public CScene2D
{
public:
	// �g�p�\�Ȏ��
	typedef enum
	{
		SPRITESHEET_EXPLOSION = 0,
		SPRITESHEET_EXPLOSION2,
		SPRITESHEET_TEST,
		SPRITESHEET_MAX,
	} SPRITESHEET;

	CSpriteSheet();
	~CSpriteSheet();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetSheetInfo(SPRITESHEET type);

	static CSpriteSheet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size, SPRITESHEET type);

private:
	int m_nCounterAnim;	// �A�j���[�V�����̃J�E���^�[
	int m_nPatternH;	// �������̃p�^�[��
	int m_nPatternV;	// �c�����̃p�^�[��
	int m_nAnimationSpeed;	// �A�j���[�V�����̑��x
	float m_fSheetColumn;	// �X�v���C�g�V�[�g�̕����� (��)
	float m_fSheetRow;		// �X�v���C�g�V�[�g�̕����� (�c)
	SPRITESHEET m_type;		// �X�v���C�g�V�[�g�̎��
};

#endif