//=============================================================================
//
// �����_�����O [renderer.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

//*****************************************************************************
// �w�b�_�̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TARGET_OFFSET (0.5f)
#define TARGET_SIZE (D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT))
#define TARGET_OPACITYDEF (0.75f)
#define TARGET_SIZEOFFSETDEF (-40.0f)

//*****************************************************************************
// �����_���[�N���X
//*****************************************************************************
class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }	// Direct3D�f�o�C�X���擾

	void SetFeedbackEffect(int nFrames, float fOpacity, float fSizeOffset);		// �t�B�[�h�o�b�N�G�t�F�N�g��L���� (�t���[�����w��j
	void SetFeedbackEffect(int nFrames);										// �s�����x�A�T�C�Y�I�t�Z�b�g���w�肳��Ȃ��ꍇ�͏����l�Ŕ���

	void SetFeedbackEffectStatus(bool bFlag) { m_FBParam.bActive = bFlag; }		// �t�B�[�h�o�b�N�G�t�F�N�g��L��/�����ɂ���
	bool GetFeedbackEffectStatus(void) { return m_FBParam.bActive; }			// ���݂̃t�B�[�h�o�b�N�G�t�F�N�g�̏�Ԃ��擾

	void SetBlurOpacity(float fValue) { m_FBParam.fOpacity = fValue; }			// �t�B�[�h�o�b�N�p�|���S���̕s�����x��ݒ�
	float GetBlurOpacity(void) { return m_FBParam.fOpacity; }					//								�@��Ԃ�

	void SetBlurSizeOffset(float fValue) { m_FBParam.fSizeOffset = fValue; }	// �t�B�[�h�o�b�N�p�|���S���̃T�C�Y�I�t�Z�b�g��ݒ�
	float GetBlurSizeOffset(void) { return m_FBParam.fSizeOffset; }				// �@�@

private:
	// �t�B�[�h�o�b�N�G�t�F�N�g�̌��ʂ�ς���p�����[�^�[
	typedef struct {
		bool  bActive;				// �G�t�F�N�g��L���ɂ��邩�ǂ���
		float fOpacity;				// �|���S���̕s�����x�A�Ⴍ����ƃu���[�������Ȃ�
		float fSizeOffset;			// �T�C�Y�̃I�t�Z�b�g�A�}�C�i�X�ŊO���A�v���X�œ����ɂԂ��
		int nFrames;				// �������鎞�� (�t���[����)
		int nCounter;
	} FEEDBACK_PARAM;

	void CountFrame(void);
	void TargetPolyDraw(int nTextureIdx);	// �t�B�[�h�o�b�N�p�|���S���̕`��

	LPDIRECT3D9 m_pD3D;				// Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9 m_pD3DDevice;	// �f�o�C�X�I�u�W�F�N�g

	LPDIRECT3DTEXTURE9 m_apTextureMT[2];		// �����_�����O�^�[�Q�b�g�p�e�N�X�`��
	LPDIRECT3DSURFACE9 m_apRenderMT[2];			// �e�N�X�`�������_�����O�p�C���^�[�t�F�[�X
	LPDIRECT3DSURFACE9 m_apBuffMT[2];			// �e�N�X�`�������_�����O�pZ�o�b�t�@
	D3DVIEWPORT9 m_viewportMT;					// �e�N�X�`�������_�����O�p�r���[�|�[�g
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffMT;		// �t�B�[�h�o�b�N�p�|���S�����_�o�b�t�@
	FEEDBACK_PARAM m_FBParam;					// �t�B�[�h�o�b�N�G�t�F�N�g�̃p�����[�^�[
};
#endif
