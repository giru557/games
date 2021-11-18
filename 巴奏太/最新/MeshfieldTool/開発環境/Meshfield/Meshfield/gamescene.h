//=============================================================================
//
// �Q�[����� [game.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************************************
// �w�b�_�[�̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "scene3d.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MESH_VTX_OFFSET_MOVESPEED (2.0f)	// ���_�̔g�I�t�Z�b�g�ړ����x
#define MESH_VTX_FREQUENCY_ADD (0.01f)		// ���_�̔g�̎����ϓ�
#define MESH_VTX_SPEED_ADD (0.05f)			// ���_�̔g�̃X�s�[�h�ϓ�
#define MESH_VTX_AMPLITUDE_ADD (1.0f)		// ���_�̔g�̑傫���ϓ�

#define MESH_TEX_DIVISION_ADD (1.0f)		// �e�N�X�`���̕������̕ϓ�
#define MESH_TEX_SCROLL_ADD (0.005f)			// �e�N�X�`���̃X�N���[���ϓ�
#define MESH_TEX_AMPLITUDE_ADD (0.1f)		// �e�N�X�`���̔g�̑傫���ϓ�
#define MESH_TEX_SPEED_ADD (0.1f)			// �e�N�X�`���̔g�̃X�s�[�h�ϓ�
#define MESH_TEX_FREQUENCY_ADD (0.01f)		// �e�N�X�`���̔g�̎����ϓ�

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CFont;

//*****************************************************************************
// �Q�[����ʃN���X ( �p����: �I�u�W�F�N�g�N���X [CScene] )
//*****************************************************************************
class CGame : public CScene
{
public:
	CGame();
	~CGame();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	int GetTargetTexIdx(void) { return m_nTargetTex; }	// �ΏۂɂȂ��Ă���e�N�X�`���ԍ����擾
	int GetTargetXY(void) { return m_nXY; }

	static CFont *GetFont(void) { return m_pFont; }
	static CScene3D *GetMeshfield(void) { return m_pMeshfield; }
private:
	HRESULT ReadConfig(char* pFilePath, CScene3D::MESHFIELD *meshInfo);		// ���b�V���t�B�[���h�̐ݒ��ǂݍ���
	HRESULT ReadWaveConfig(char* pFilePath, CScene3D::WAVE *waveInfo);		// �g�̐ݒ��ǂݍ���
	HRESULT WriteWaveConfig(char* pFilePath, CScene3D::WAVE waveInfo);
	void SetWaveInfo(CScene3D::WAVE waveInfo);
	CScene3D::WAVE GetWaveInfo(void);

	int m_nTargetTex;		// ���ݑΏۂɂȂ��Ă���e�N�X�`���̔ԍ�
	int m_nXY;

	static CFont *m_pFont;			// �����\���N���X
	static CScene3D *m_pMeshfield;	// ���b�V���t�B�[���h
};
#endif

