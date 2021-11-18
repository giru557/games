//=============================================================================
//
// 3D�|���S�� [scene3D.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _SCENE_3D_H_
#define _SCENE_3D_H_

//*****************************************************************************
// �w�b�_�[�̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CLine;

//*****************************************************************************
// �|���S���N���X (�h�����F�I�u�W�F�N�g�N���X (scene) �j
//*****************************************************************************
class CScene3D : public CScene
{
public:
	// 3D�̒��_�t�H�[�}�b�g
	typedef struct {
		D3DXVECTOR3 pos;	// ���_���W
		D3DXVECTOR3 nor;	// �@���x�N�g��
		D3DCOLOR col;		// ���_�J���[
		D3DXVECTOR2 tex;	// �e�N�X�`�����W
		D3DXVECTOR2 tex2;	// �e�N�X�`�����W2
	} VERTEX_3D;

	// �e�N�X�`���̍\����
	typedef struct {
		LPDIRECT3DTEXTURE9 pTexture;	// �e�N�X�`���ւ̃|�C���^
		D3DTEXTUREOP blendState;		// �u�����h�̎��
		float fDivision;				// �����̔{��
	} MESHFIELD_TEXTURE;

	// �g�̃p�����[�^�[�\����
	typedef struct {
		float fFrequency;
		float fSpeed;
		float fAmplitude;
	} WAVEPARAM;

	// �g�̎��
	typedef enum {
		WAVETYPE_RIPPLE = 0,
		WAVETYPE_X,
		WAVETYPE_Z,

		WAVETYPE_MAX,
	}WAVETYPE;

	// �g�\����
	typedef struct {
		WAVETYPE type;
		WAVEPARAM texParam[2][2];
		WAVEPARAM vtxParam;
		D3DXVECTOR3 rippleOffset;
		D3DXVECTOR2 texScroll[2];
	} WAVE;

	// ���b�V���t�B�[���h�̏��������
	typedef struct {
		float fSizeX;	// �傫�� X
		float fSizeZ;	// �傫�� Z
		int nXDivision;	// ������ X
		int nZDivision;	// ������ Z
		vector<MESHFIELD_TEXTURE> vTexture;	// �e�N�X�`��
	} MESHFIELD;

	CScene3D();
	~CScene3D();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �e�N�X�`���̕����ݒ�
	void SetTextureDiv(int nIdx, float fDivision) { m_MeshField.vTexture[nIdx].fDivision = fDivision; }
	float GetTextureDiv(int nIdx) { return m_MeshField.vTexture[nIdx].fDivision; }

	// �e�N�X�`���̃X�N���[���ݒ�
	void SetTextureScroll(int nIdx, D3DXVECTOR2 scroll) { m_wave.texScroll[nIdx] = scroll; }
	D3DXVECTOR2 GetTextureScroll(int nIdx) { return m_wave.texScroll[nIdx]; }

	// �e�N�X�`���̔g�ݒ�
	void SetTextureWave(int nIdx, WAVEPARAM param, int XorY);
	WAVEPARAM GetTextureWave(int nIdx, int XorY);

	// ���_�̔g�I�t�Z�b�g�ʒu�ݒ�
	void SetVtxWaveOffset(D3DXVECTOR3 offset) { m_wave.rippleOffset = offset; }
	D3DXVECTOR3 GetVtxWaveOffset(void) { return m_wave.rippleOffset; }

	// ���_�̔g�̃p�����[�^�[�ݒ�
	void SetVtxWave(WAVEPARAM param) { m_wave.vtxParam = param; }
	WAVEPARAM GetVtxWave(void) { return m_wave.vtxParam; }

	// ���_�̔g�̎�ސݒ�
	void SetVtxWaveType(WAVETYPE waveType) { m_wave.type = waveType; }
	WAVETYPE GetVtxWaveType(void) { return m_wave.type; }

	// �@���\���ؑ�
	void ToggleNormal(void) { m_bShowNormal = !m_bShowNormal; }

	// ����
	static CScene3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, MESHFIELD meshInfo);

private:
	// �O�p�`�t�H�[�}�b�g
	typedef struct {
		int nV1;
		int nV2;
		int nV3;
		D3DXVECTOR3 nor;
	} TRIANGLE;

	D3DXMATRIX m_mtxWorld;					// ���[���h�}�g���b�N�X
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;		// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	MESHFIELD m_MeshField;					// ���b�V���t�B�[���h�̏��������
	WAVE m_wave;
	//D3DXVECTOR2 m_texScroll[2];				// �e�N�X�`�����W�̃X�N���[���l
	//WAVEPARAM m_aTexWave[2][2];				// �e�N�X�`���̔g�̃p�����[�^�[
	//WAVEPARAM m_vtxWave;					// ���_�̔g�̃p�����[�^�[
	//WAVETYPE m_waveType;					// ���݂̔g�̎��
	//D3DXVECTOR3 m_offset;					// �g�̊J�n�ʒu�I�t�Z�b�g
	int m_nVertices;						// ���_��
	int m_nIndices;							// �C���f�b�N�X��
	int m_nTriangles;						// �O�p�`��
	bool m_bShowNormal;						// �@�������t���O
	TRIANGLE *m_pTri;						// �O�p�`�t�H�[�}�b�g�̃|�C���^
	VERTEX_3D *m_pVertex;					// ���_�̃|�C���^
	vector<int> *m_pSharedVtx;				// ���L���_���X�g�̃|�C���^
	vector<CLine*> m_vLine;
};

#endif