//=============================================================================
//
// �����\������ [number.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

//*****************************************************************************
// �w�b�_�[�̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "scene2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �����\���N���X ( ��{ )
//*****************************************************************************
class CNumber
{
public:
	CNumber();
	~CNumber();

	// 2D�̒��_�t�H�[�}�b�g
	typedef struct
	{
		D3DXVECTOR3 pos;	// ���_���W
		float rhw;			// 1.0�Œ�
		D3DCOLOR col;		// ���_�J���[
		D3DXVECTOR2 tex;	// �e�N�X�`�����W
	} VERTEX_2D;

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nInitNum);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetNumber(int nNumber);

	static CNumber *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nInitNum);

private:
	D3DXVECTOR3 m_pos;						// �ʒu
	D3DXVECTOR3 m_rot;						// ����
	D3DXVECTOR2 m_size;						// �T�C�Y
	float m_fLength;
	float m_fAngle;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@
	VERTEX_2D *m_pVertex;
};
#endif
