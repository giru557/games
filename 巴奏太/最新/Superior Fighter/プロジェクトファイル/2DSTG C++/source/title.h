//=============================================================================
//
// �^�C�g�� [title.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

//*****************************************************************************
// �w�b�_�[�̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "scene2d.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TITLE_TEXTURE_COUNT (2)
#define TITLE_TEXTURE_TITLE_FILEPATH ("resource\\texture\\superiorfighter_title.jpg")
#define TITLE_TEXTURE_START_FILEPATH ("resource\\texture\\superiorfighter_start.png")
#define TITLE_TEXTURE_START_SIZE (D3DXVECTOR2(634.0f, 284.0f))
#define TITLE_TEXTURE_START_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, 900.0f, 0.0f))
#define TITLE_START_BLINK_FREQUENCY (60)

//*****************************************************************************
// �^�C�g���N���X ( �p����: �I�u�W�F�N�g�N���X [scene] )
//*****************************************************************************
class CTitle : public CScene
{
public:
	CTitle();
	~CTitle();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPos(D3DXVECTOR3 pos) {};
	void SetRot(D3DXVECTOR3 rot) {};
	void SetSize(D3DXVECTOR2 size) {};
	D3DXVECTOR3 GetPos(void) { return VECTOR3_ZERO; }
	D3DXVECTOR3 GetRot(void) { return VECTOR3_ZERO; }
	D3DXVECTOR2 GetSize(void) { return (D3DXVECTOR2)VECTOR3_ZERO; }

private:
	CScene2D *m_pSceneTitle;	// �^�C�g���w�i�̃I�u�W�F�N�g
	CScene2D *m_pSceneStart;	// �Q�[���X�^�[�g�{�^���̃I�u�W�F�N�g
	int m_nBlinkCounter;		// �_�ŗp�J�E���^�[
	bool m_bAppear;				// �_�ŗp�t���O

	static LPDIRECT3DTEXTURE9 m_apTexture[TITLE_TEXTURE_COUNT];

	void BlinkStartObj(void);	// �_�ŗp�֐�
};
#endif
