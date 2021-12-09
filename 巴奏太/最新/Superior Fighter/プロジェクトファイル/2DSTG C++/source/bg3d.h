//=============================================================================
//
// 3D�w�i [bg3D.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _BG_3D_H_
#define _BG_3D_H_

//*****************************************************************************
// �w�b�_�[�̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BG_WAVE_FREQUENCY (0.05f)	// �h��̎���
#define BG_WAVE_SPEED (0.1f)		// �h��̑���
#define BG_WAVE_AMPLITUDE (15.0f)	// �h��̑傫��
#define BG_SIZE (D3DXVECTOR2(800, 500))		// �|���S���̕�����
#define BG_DIVISION_X (64)					// �|���S���̉�������
#define BG_DIVISION_Z (64)					//			�c������

//*****************************************************************************
// 3D�w�i�N���X (�h�����F�I�u�W�F�N�g�N���X (CScene) �j
//*****************************************************************************
class CBackground3D : public CScene
{
public:
	// 3D�̒��_�t�H�[�}�b�g
	typedef struct
	{
		D3DXVECTOR3 pos;	// ���_���W
		D3DXVECTOR3 nor;	// �@���x�N�g��
		D3DCOLOR col;		// ���_�J���[
		D3DXVECTOR2 tex;	// �e�N�X�`�����W
		D3DXVECTOR2 tex2;	// �e�N�X�`�����W�Q
	} VERTEX_3D;

	CBackground3D(int nPriority = 3);
	~CBackground3D();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ʒu�����T�C�Y
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetSize(D3DXVECTOR2 size) { m_size = D3DXVECTOR2(size.x, size.y); }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXVECTOR2 GetSize(void) { return m_size; }

	// �g��̐ݒ�
	void SetRippleFrequency(float fValue, int nFrames) { m_ripple.fFrequency = fValue; m_ripple.nCountFreq = nFrames; }
	void SetRippleSpeed(float fValue, int nFrames) { m_ripple.fSpeed = fValue; m_ripple.nCountSpd = nFrames; }
	void SetRippleAmplitude(float fValue, int nFrames) { m_ripple.fAmplitude = fValue; m_ripple.nCountAmp = nFrames; }

	// �e�N�X�`���ݒ�
	void SetBossTexture(void) { m_bBossTex = true; }
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture1, LPDIRECT3DTEXTURE9 pTexture2, LPDIRECT3DTEXTURE9 pTexture3);

	static CBackground3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size, int nXDivision, int nZDivision);

private:
	// �O�p�`�t�H�[�}�b�g
	typedef struct {
		int nV1;
		int nV2;
		int nV3;
		D3DXVECTOR3 nor;
	} TRIANGLE;

	// �g��̐��l
	typedef struct {
		float fFrequency;
		float fSpeed;
		float fAmplitude;
		int nCountFreq;
		int nCountSpd;
		int nCountAmp;
	} RIPPLE_PARAM;

	void RippleTimer(void);	// �g��̒l�ύX�^�C�}�[

	D3DXVECTOR3 m_pos;	// Position
	D3DXVECTOR3 m_rot;	// Rotation
	D3DXVECTOR2 m_size;	// Size

	D3DXMATRIX m_mtxWorld;					// ���[���h�}�g���b�N�X
	LPDIRECT3DTEXTURE9 m_apTexture[3];		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;		// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	RIPPLE_PARAM m_ripple;					// �g��̒l
	bool m_bBossTex;						// �{�X�p�̃e�N�X�`���ɕύX�t���O
	int m_nXdivision;						// X���̕�����
	int m_nZdivision;						// Z���̕�����
	int m_nVertices;						// ���_��
	int m_nIndices;							// �C���f�b�N�X��
	int m_nTriangles;						// �O�p�`��
	VERTEX_3D *m_pVertex;					// ���_���ւ̃|�C���^
	TRIANGLE *m_pTri;						// �O�p�`�̒�`�Ƃ��̖ʖ@��
	vector<int> *m_pSharedVtx;				// ���_���L���X�g
};

#endif