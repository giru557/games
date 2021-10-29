//=============================================================================
//
// 2D�|���S�� [scene2D.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _SCENE_2D_H_
#define _SCENE_2D_H_

//*****************************************************************************
// �w�b�_�[�̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SCENE2D_WIDTH_DEFAULT (200)		// �|���S���̕� (�f�t�H)
#define SCENE2D_HEIGHT_DEFAULT (200)	// �|���S���̍��� (�f�t�H)

//*****************************************************************************
// �|���S���N���X (�h�����F�I�u�W�F�N�g�N���X (scene) �j
//*****************************************************************************
class CScene2D : public CScene
{
public:
	// 2D�̒��_�t�H�[�}�b�g
	typedef struct
	{
		D3DXVECTOR3 pos;	// ���_���W
		float rhw;			// 1.0�Œ�
		D3DCOLOR col;		// ���_�J���[
		D3DXVECTOR2 tex;	// �e�N�X�`�����W
	} VERTEX_2D;

	CScene2D(int nPriority = 3);
	~CScene2D();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void UpdateVertex(void);

	// Set
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetSize(D3DXVECTOR2 size) { m_size = D3DXVECTOR2(size.x, size.y); }
	void SetTextureUV(D3DXVECTOR2 Vertex_0, D3DXVECTOR2 Vertex_1, D3DXVECTOR2 Vertex_2, D3DXVECTOR2 Vertex_3);
	void SetVertices(D3DXVECTOR3 Vertex_0, D3DXVECTOR3 Vertex_1, D3DXVECTOR3 Vertex_2, D3DXVECTOR3 Vertex_3);
	void SetColor(D3DXCOLOR col);
	void SetConvertState(bool bConvertFlag) { m_bConvertPosition = bConvertFlag; }
	void SetDrawFlag(bool bDraw) { m_bDraw = bDraw; }

	// Get
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetPosPrev(void) { return m_posPrev; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXVECTOR2 GetSize(void) { return m_size; }
	D3DXCOLOR GetColor(void) { return m_col; }
	D3DXVECTOR3 **GetVertices(void);
	bool GetDrawFlag(void) { return m_bDraw; }

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

	static CScene2D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);

private:
	LPDIRECT3DTEXTURE9 m_pTexture;			// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	D3DXCOLOR m_col;						// �F
	D3DXVECTOR3 m_pos;						// �ʒu
	D3DXVECTOR3 m_posPrev;					// �P�t���[���O�̈ʒu
	D3DXVECTOR3 m_rot;						// ����
	D3DXVECTOR2 m_size;						// �傫��
	float m_fLength;						// ���S���璸�_�܂ł̒���
	float m_fAngle;							// ���S���璸�_�ւ̊p�x
	bool m_bConvertPosition;				// ���[���h���W����X�N���[�����W�ɕϊ����邩�̃t���O
	bool m_bDraw;							// �`��t���O
	VERTEX_2D *m_pVertex;					// 2D�|���S���̃|�C���^

};

#endif