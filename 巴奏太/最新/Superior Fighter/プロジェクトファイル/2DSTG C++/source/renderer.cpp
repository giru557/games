//=============================================================================
//
// �����_�����O [renderer.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "renderer.h"
#include "manager.h"
#include "fade.h"
#include "debugger.h"
#include "camera3d.h"
#include "game.h"

//*****************************************************************************
// �����_���[�N���X (��{)
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CRenderer::CRenderer()
{
	m_FBParam.bActive = false;
	m_FBParam.fOpacity = TARGET_OPACITYDEF;
	m_FBParam.fSizeOffset = TARGET_SIZEOFFSETDEF;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CRenderer::~CRenderer()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3D�I�u�W�F�N�g�̍쐬
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ���[�N���[���N���A
	d3dpp.BackBufferCount = 1;										// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth = SCREEN_WIDTH;							// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;							// �J���[���[�h�̎w��
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						// �f���M���ɓ������ăt���b�v����
	d3dpp.EnableAutoDepthStencil = TRUE;							// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;										// �E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// ���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// �C���^�[�o��

	// �f�o�C�X�̐���
	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice)))
	{
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &m_pD3DDevice)))
			{
				// �������s
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	// �T���v���[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);	// �e�N�X�`���g�厞�̕�Ԑݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);	// �e�N�X�`���k�����̕�Ԑݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`��U�l�̌J��Ԃ��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`��V�l�̌J��Ԃ��ݒ�

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	LPDIRECT3DSURFACE9 pRenderDef, pBuffDef;	// �ۑ��p
	for (int nCnt = 0; nCnt < 2; nCnt++) {
		// �����_�����O�^�[�Q�b�g�p�e�N�X�`������
		m_pD3DDevice->CreateTexture(
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT,
			&m_apTextureMT[nCnt],
			NULL);

		// �e�N�X�`�������_�����O�p�C���^�[�t�F�[�X����
		m_apTextureMT[nCnt]->GetSurfaceLevel(0, &m_apRenderMT[nCnt]);

		// �e�N�X�`�������_�����O�pZ�o�b�t�@����
		m_pD3DDevice->CreateDepthStencilSurface(
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			D3DFMT_D16,
			D3DMULTISAMPLE_NONE,
			0,
			TRUE,
			&m_apBuffMT[nCnt],
			NULL);

		// ���݂̃����_�����O�^�[�Q�b�g���擾 (�ۑ�)
		m_pD3DDevice->GetRenderTarget(0, &pRenderDef);

		// ���݂�Z�o�b�t�@���擾 (�ۑ�)
		m_pD3DDevice->GetDepthStencilSurface(&pBuffDef);

		// �����_�����O�^�[�Q�b�g�𐶐������e�N�X�`���ɐݒ�
		m_pD3DDevice->SetRenderTarget(0, m_apRenderMT[nCnt]);

		// Z�o�b�t�@��ݒ�
		m_pD3DDevice->SetDepthStencilSurface(m_apBuffMT[nCnt]);

		// �����_�����O�^�[�Q�b�g�p�̃T�[�t�F�C�X���N���A
		m_pD3DDevice->Clear(
			0,
			NULL,
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_RGBA(0, 0, 0, 0),
			1.0f,
			0);

		// �����_�����O�^�[�Q�b�g�����ɖ߂�
		m_pD3DDevice->SetRenderTarget(0, pRenderDef);

		// Z�o�b�t�@�����ɖ߂�
		m_pD3DDevice->SetDepthStencilSurface(pBuffDef);
	}

	// �e�N�X�`�������_�����O�p�r���[�|�[�g�̐ݒ�
	m_viewportMT.X = 0;
	m_viewportMT.Y = 0;
	m_viewportMT.Width = SCREEN_WIDTH;
	m_viewportMT.Height = SCREEN_HEIGHT;
	m_viewportMT.MinZ = 0.0f;
	m_viewportMT.MaxZ = 1.0f;

	// ���_�o�b�t�@�̐���
	if (FAILED(m_pD3DDevice->CreateVertexBuffer(sizeof(CScene2D::VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuffMT, NULL)))
	{
		return E_FAIL;
	}

	// ���_�o�b�t�@�����b�N
	CScene2D::VERTEX_2D *pVtx;
	m_pVtxBuffMT->Lock(0, 0, (void**)&pVtx, 0);

	float fOff = TARGET_OFFSET;
	D3DXVECTOR2 pos = SCREEN_CENTER;
	pVtx[0].pos = D3DXVECTOR3(-TARGET_SIZE.x / 2 + pos.x - fOff, TARGET_SIZE.y / 2 + pos.y - fOff, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(-TARGET_SIZE.x / 2 + pos.x - fOff, -TARGET_SIZE.y / 2 + pos.y - fOff, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TARGET_SIZE.x / 2 + pos.x - fOff, TARGET_SIZE.y / 2 + pos.y - fOff, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TARGET_SIZE.x / 2 + pos.x - fOff, -TARGET_SIZE.y / 2 + pos.y - fOff, 0.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++) {
		pVtx[nCntVtx].col = D3DXCOLOR(1, 1, 1, 1);
		pVtx[nCntVtx].rhw = 1.0f;
	}

	// ���_�o�b�t�@�A�����b�N
	m_pVtxBuffMT->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CRenderer::Uninit(void)
{
	for (int nCntMT = 0; nCntMT < 2; nCntMT++) {
		// �����_�����O�^�[�Q�b�g�p�e�N�X�`����j��
		if (m_apTextureMT[nCntMT] != NULL) {
			m_apTextureMT[nCntMT]->Release();
			m_apTextureMT[nCntMT] = NULL;
		}

		// �e�N�X�`�������_�����O�p�C���^�[�t�F�[�X��j��
		if (m_apRenderMT[nCntMT] != NULL) {
			m_apRenderMT[nCntMT]->Release();
			m_apRenderMT[nCntMT] = NULL;
		}

		// �e�N�X�`�������_�����O�pZ�o�b�t�@��j��
		if (m_apBuffMT[nCntMT] != NULL) {
			m_apBuffMT[nCntMT]->Release();
			m_apBuffMT[nCntMT] = NULL;
		}
	}

	// ���_�o�b�t�@�j��
	if (m_pVtxBuffMT != NULL) {
		m_pVtxBuffMT->Release();
		m_pVtxBuffMT = NULL;
	}

	// �f�o�C�X�̔j��
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CRenderer::Update(void)
{
	// �G�t�F�N�g�p�̃J�E���^�[
	CountFrame();

	// ���ׂẴI�u�W�F�N�g�̍X�V
	CScene::UpdateAll();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CRenderer::Draw(void)
{
	LPDIRECT3DSURFACE9 pRenderDef, pBuffDef;	// �����_�[�^�[�Q�b�g�AZ�o�b�t�@�ۑ��p
	D3DVIEWPORT9 viewportDef;					// �r���[�|�[�g�ۑ��p

	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		// ���݂̃����_�[�^�[�Q�b�g��ۑ�
		m_pD3DDevice->GetRenderTarget(0, &pRenderDef);
		m_pD3DDevice->GetDepthStencilSurface(&pBuffDef);
		m_pD3DDevice->GetViewport(&viewportDef);

		// �����_�[�^�[�Q�b�g���e�N�X�`��0�ɐݒ�
		m_pD3DDevice->SetRenderTarget(0, m_apRenderMT[0]);
		m_pD3DDevice->SetDepthStencilSurface(m_apBuffMT[0]);
		m_pD3DDevice->SetViewport(&m_viewportMT);

		// �e�N�X�`��0���N���A
		m_pD3DDevice->Clear(
			0,
			NULL,
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_RGBA(0, 0, 0, 0),
			1.0f,
			0);

		// �J������ݒu
		CManager::GetCamera3D()->SetCamera(m_pD3DDevice);

		// ���ׂẴI�u�W�F�N�g�̕`��
		CScene::DrawAll();

		// �t�F�[�h�̕`��
		CManager::GetFade()->Draw();

#ifdef _DEBUG
		CManager::GetDebugger()->Draw();
#endif
		// �t���O�������Ă�����|���S���Ƀe�N�X�`���P��\���ĕ`��
		if (m_FBParam.bActive)
			TargetPolyDraw(1);

		// �����_�[�^�[�Q�b�g�����ɖ߂�
		m_pD3DDevice->SetRenderTarget(0, pRenderDef);
		m_pD3DDevice->SetDepthStencilSurface(pBuffDef);
		m_pD3DDevice->SetViewport(&viewportDef);

		// �t�B�[�h�o�b�N�p�|���S���Ƀe�N�X�`���O��\��t���ĕ`��
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_ADD);	// ���Z����
		TargetPolyDraw(0);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

		// �e�N�X�`��0��1�����ւ���
		swap(m_apRenderMT[0], m_apRenderMT[1]);
		swap(m_apTextureMT[0], m_apTextureMT[1]);

		// Direct3D�ɂ��`��̏I��
		m_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// �t�B�[�h�o�b�N�G�t�F�N�g�𔭓�
//=============================================================================
void CRenderer::SetFeedbackEffect(int nFrames, float fOpacity, float fSizeOffset)
{
	// �V�����Ă΂ꂽ�t���[���������݂��傫���ꍇ�̂ݍX�V
	if (m_FBParam.nFrames < nFrames) {
		m_FBParam.nFrames = nFrames;
		m_FBParam.fOpacity = fOpacity;
		m_FBParam.fSizeOffset = fSizeOffset;
		m_FBParam.nCounter = 0;
		m_FBParam.bActive = true;
	}
}
void CRenderer::SetFeedbackEffect(int nFrames)
{
	// �V�����Ă΂ꂽ�t���[���������݂��傫���ꍇ�̂ݍX�V
	if (m_FBParam.nFrames < nFrames) {
		m_FBParam.nFrames = nFrames;
		m_FBParam.fOpacity = TARGET_OPACITYDEF;
		m_FBParam.fSizeOffset = TARGET_SIZEOFFSETDEF;
		m_FBParam.nCounter = 0;
		m_FBParam.bActive = true;
	}
}

//*****************************************************************************
// �G�t�F�N�g�𔭓�����t���[�����J�E���g����
//*****************************************************************************
void CRenderer::CountFrame(void)
{
	// ������
	if (m_FBParam.bActive) {
		m_FBParam.nCounter++;	// �J�E���g�A�b�v
		
		// �J�E���g�I��
		if (m_FBParam.nCounter >= m_FBParam.nFrames) {
			m_FBParam.bActive = false;
			m_FBParam.nCounter = 0;
			m_FBParam.nFrames = 0;
		}
	}
}

//*****************************************************************************
// �t�B�[�h�o�b�N�p�|���S����`��
//*****************************************************************************
void CRenderer::TargetPolyDraw(int nTextureIdx)
{
	// ���_�o�b�t�@�����b�N
	CScene2D::VERTEX_2D *pVtx;
	m_pVtxBuffMT->Lock(0, 0, (void**)&pVtx, 0);

	float fOff = TARGET_OFFSET;
	D3DXVECTOR2 pos = SCREEN_CENTER;
	D3DXVECTOR2 size = TARGET_SIZE;

	if (nTextureIdx != 0)
		size = D3DXVECTOR2(size.x - m_FBParam.fSizeOffset, size.y - m_FBParam.fSizeOffset);

	pVtx[0].pos = D3DXVECTOR3(-size.x / 2 + pos.x - fOff, size.y / 2 + pos.y - fOff, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(-size.x / 2 + pos.x - fOff, -size.y / 2 + pos.y - fOff, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(size.x / 2 + pos.x - fOff, size.y / 2 + pos.y - fOff, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(size.x / 2 + pos.x - fOff, -size.y / 2 + pos.y - fOff, 0.0f);

	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++) {
		if (nTextureIdx == 0)
			pVtx[nCntVtx].col = D3DXCOLOR(1, 1, 1, 1);
		else
			pVtx[nCntVtx].col = D3DXCOLOR(1, 1, 1, m_FBParam.fOpacity);
	}
	// ���_�o�b�t�@�A�����b�N
	m_pVtxBuffMT->Unlock();

	// �t�B�[�h�o�b�N�p�|���S���Ƀe�N�X�`����\��t���ĕ`��
	m_pD3DDevice->SetStreamSource(0, m_pVtxBuffMT, 0, sizeof(CScene2D::VERTEX_2D));
	m_pD3DDevice->SetFVF(FVF_VERTEX_2D);
	m_pD3DDevice->SetTexture(0, m_apTextureMT[nTextureIdx]);
	m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}