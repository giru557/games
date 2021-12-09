//=============================================================================
//
// �X�R�A�̏��� [score.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

//*****************************************************************************
// �w�b�_�[�̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SCORE_DIGIT (8)	// �X�R�A�̌���
#define SCORE_POSITION (D3DXVECTOR3(1500.0f, 80.0f, 0.0f))

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CNumber;

//*****************************************************************************
// �X�R�A�N���X ( �p����: �I�u�W�F�N�g�N���X [scene] )
//*****************************************************************************
class CScore : public CScene
{
public:
	CScore();
	~CScore();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetScore(int nScore);
	void AddScore(int nValue);
	int GetScore(void);

	static CScore *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);

private:
	D3DXVECTOR3 m_pos;					// �ʒu
	D3DXVECTOR3 m_rot;					// ����
	D3DXVECTOR2 m_size;					// �T�C�Y
	CNumber *m_apNumber[SCORE_DIGIT];	// �����\���N���X�̃|�C���^
	int m_nScore;						// ���ۂ̃X�R�A
};
#endif

