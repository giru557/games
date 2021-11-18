//=============================================================================
//
// �Q�[����� [game.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "gamescene.h"
#include "input.h"
#include "manager.h"
#include "font.h"

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CFont *CGame::m_pFont = NULL;
CScene3D *CGame::m_pMeshfield = NULL;

//*****************************************************************************
// �Q�[����ʃN���X ( �p����: �I�u�W�F�N�g�N���X [CScene] )
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CGame::CGame()
{
	m_nTargetTex = 0;
	m_nXY = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// ������
//=============================================================================
HRESULT CGame::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size)
{
	// �����\���N���X����
	m_pFont = new CFont;
	if (m_pFont != NULL) {
		m_pFont->Init();
	}

	// ���b�V���t�B�[���h�̐ݒ��ǂݍ���
	CScene3D::MESHFIELD fieldInfo;
	ReadConfig("data/Meshfield.txt", &fieldInfo);

	// �g�̐ݒ��ǂݍ���
	CScene3D::WAVE waveInfo = {};
	ReadWaveConfig("data/wave.txt", &waveInfo);

	// ���b�V���t�B�[���h����
	m_pMeshfield = CScene3D::Create(VECTOR3_ZERO, VECTOR3_ZERO, fieldInfo);

	// �g�̐ݒ�K�p
	SetWaveInfo(waveInfo);

	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CGame::Uninit(void)
{
	// �����\���N���X�j��
	if (m_pFont != NULL) {
		m_pFont->Uninit();
		m_pFont = NULL;
	}

	// ���̃I�u�W�F�N�g�̊J��
	this->Release();
}

//=============================================================================
// �X�V
//=============================================================================
void CGame::Update(void)
{
	// �����\���X�V
	if (m_pFont != NULL) {
		m_pFont->Update();
	}

	// �@���\���؂�ւ�
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	if (pKeyboard->GetTrigger(DIK_N))
	{
		m_pMeshfield->ToggleNormal();
	}

	// �Ώۃe�N�X�`���ύX
	if (pKeyboard->GetTrigger(DIK_1))
		m_nTargetTex = 0;
	else if (pKeyboard->GetTrigger(DIK_2))
		m_nTargetTex = 1;

	// �摜����
	if (pKeyboard->GetTrigger(DIK_Z)) {
		m_pMeshfield->SetTextureDiv(m_nTargetTex, m_pMeshfield->GetTextureDiv(m_nTargetTex) - MESH_TEX_DIVISION_ADD);
	}
	else if (pKeyboard->GetTrigger(DIK_X)) {
		m_pMeshfield->SetTextureDiv(m_nTargetTex, m_pMeshfield->GetTextureDiv(m_nTargetTex) + MESH_TEX_DIVISION_ADD);
	}

	// �X�N���[��
	if (pKeyboard->GetTrigger(DIK_Q)) {
		m_pMeshfield->SetTextureScroll(m_nTargetTex, m_pMeshfield->GetTextureScroll(m_nTargetTex) + D3DXVECTOR2(MESH_TEX_SCROLL_ADD, 0));
	}
	else if (pKeyboard->GetTrigger(DIK_E)) {
		m_pMeshfield->SetTextureScroll(m_nTargetTex, m_pMeshfield->GetTextureScroll(m_nTargetTex) + D3DXVECTOR2(-MESH_TEX_SCROLL_ADD, 0));
	}
	if (pKeyboard->GetTrigger(DIK_R)) {
		m_pMeshfield->SetTextureScroll(m_nTargetTex, m_pMeshfield->GetTextureScroll(m_nTargetTex) + D3DXVECTOR2(0, MESH_TEX_SCROLL_ADD));
	}
	else if (pKeyboard->GetTrigger(DIK_F)) {
		m_pMeshfield->SetTextureScroll(m_nTargetTex, m_pMeshfield->GetTextureScroll(m_nTargetTex) + D3DXVECTOR2(0, -MESH_TEX_SCROLL_ADD));
	}

	// �c���ǂ����̔g��ҏW���邩�I��
	if (pKeyboard->GetTrigger(DIK_C)) {
		m_nXY = 0;
	}
	else if (pKeyboard->GetTrigger(DIK_V)) {
		m_nXY = 1;
	}

	// �e�N�X�`���̔g
	CScene3D::WAVEPARAM waveP;
	if (pKeyboard->GetTrigger(DIK_T)) {
		waveP = m_pMeshfield->GetTextureWave(m_nTargetTex, m_nXY);
		waveP.fFrequency += MESH_TEX_FREQUENCY_ADD;
		m_pMeshfield->SetTextureWave(m_nTargetTex, waveP, m_nXY);
	}
	else if (pKeyboard->GetTrigger(DIK_G)) {
		waveP = m_pMeshfield->GetTextureWave(m_nTargetTex, m_nXY);
		waveP.fFrequency -= MESH_TEX_FREQUENCY_ADD;
		m_pMeshfield->SetTextureWave(m_nTargetTex, waveP, m_nXY);
	}
	if (pKeyboard->GetTrigger(DIK_Y)) {
		waveP = m_pMeshfield->GetTextureWave(m_nTargetTex, m_nXY);
		waveP.fSpeed += MESH_TEX_SPEED_ADD;
		m_pMeshfield->SetTextureWave(m_nTargetTex, waveP, m_nXY);
	}
	else if (pKeyboard->GetTrigger(DIK_H)) {
		waveP = m_pMeshfield->GetTextureWave(m_nTargetTex, m_nXY);
		waveP.fSpeed -= MESH_TEX_SPEED_ADD;
		m_pMeshfield->SetTextureWave(m_nTargetTex, waveP, m_nXY);
	}
	if (pKeyboard->GetTrigger(DIK_U)) {
		waveP = m_pMeshfield->GetTextureWave(m_nTargetTex, m_nXY);
		waveP.fAmplitude += MESH_TEX_AMPLITUDE_ADD;
		m_pMeshfield->SetTextureWave(m_nTargetTex, waveP, m_nXY);
	}
	else if (pKeyboard->GetTrigger(DIK_J)) {
		waveP = m_pMeshfield->GetTextureWave(m_nTargetTex, m_nXY);
		waveP.fAmplitude -= MESH_TEX_AMPLITUDE_ADD;
		m_pMeshfield->SetTextureWave(m_nTargetTex, waveP, m_nXY);
	}
	
	// ���_�̔g�I�t�Z�b�g�ʒu
	if (pKeyboard->GetPress(DIK_W)) {
		D3DXVECTOR3 offset = m_pMeshfield->GetVtxWaveOffset();
		m_pMeshfield->SetVtxWaveOffset(offset + D3DXVECTOR3(0, 0, 1));
	}
	if (pKeyboard->GetPress(DIK_S)) {
		D3DXVECTOR3 offset = m_pMeshfield->GetVtxWaveOffset();
		m_pMeshfield->SetVtxWaveOffset(offset + D3DXVECTOR3(0, 0, -1));
	}
	if (pKeyboard->GetPress(DIK_A)) {
		D3DXVECTOR3 offset = m_pMeshfield->GetVtxWaveOffset();
		m_pMeshfield->SetVtxWaveOffset(offset + D3DXVECTOR3(-1, 0, 0));
	}
	if (pKeyboard->GetPress(DIK_D)) {
		D3DXVECTOR3 offset = m_pMeshfield->GetVtxWaveOffset();
		m_pMeshfield->SetVtxWaveOffset(offset + D3DXVECTOR3(1, 0, 0));
	}

	// ���_�̔g�̃p�����[�^�[�ݒ�
	if (pKeyboard->GetTrigger(DIK_I)) {
		waveP = m_pMeshfield->GetVtxWave();
		waveP.fFrequency += MESH_VTX_FREQUENCY_ADD;
		m_pMeshfield->SetVtxWave(waveP);
	}
	else if (pKeyboard->GetTrigger(DIK_K)) {
		waveP = m_pMeshfield->GetVtxWave();
		waveP.fFrequency -= MESH_VTX_FREQUENCY_ADD;
		m_pMeshfield->SetVtxWave(waveP);
	}
	if (pKeyboard->GetTrigger(DIK_O)) {
		waveP = m_pMeshfield->GetVtxWave();
		waveP.fSpeed += MESH_VTX_SPEED_ADD;
		m_pMeshfield->SetVtxWave(waveP);
	}
	else if (pKeyboard->GetTrigger(DIK_L)) {
		waveP = m_pMeshfield->GetVtxWave();
		waveP.fSpeed -= MESH_VTX_SPEED_ADD;
		m_pMeshfield->SetVtxWave(waveP);
	}
	if (pKeyboard->GetTrigger(DIK_P)) {
		waveP = m_pMeshfield->GetVtxWave();
		waveP.fAmplitude += MESH_VTX_AMPLITUDE_ADD;
		m_pMeshfield->SetVtxWave(waveP);
	}
	else if (pKeyboard->GetTrigger(DIK_SEMICOLON)) {
		waveP = m_pMeshfield->GetVtxWave();
		waveP.fAmplitude -= MESH_VTX_AMPLITUDE_ADD;
		m_pMeshfield->SetVtxWave(waveP);
	}

	// ���_�̔g�̎��
	if (pKeyboard->GetTrigger(DIK_3)) {
		m_pMeshfield->SetVtxWaveType(CScene3D::WAVETYPE_RIPPLE);
	}
	else if (pKeyboard->GetTrigger(DIK_4)) {
		m_pMeshfield->SetVtxWaveType(CScene3D::WAVETYPE_X);
	}
	else if (pKeyboard->GetTrigger(DIK_5)) {
		m_pMeshfield->SetVtxWaveType(CScene3D::WAVETYPE_Z);
	}

	// ���݂̒l��ۑ�
	if (pKeyboard->GetTrigger(DIK_F1)) {
		WriteWaveConfig("data/wave.txt", GetWaveInfo());
	}

	// ���Z�b�g
	if (pKeyboard->GetTrigger(DIK_F5)) {
		CScene3D::WAVE wave;
		memset(&wave, 0, sizeof(CScene3D::WAVE));
		SetWaveInfo(wave);
	}
}

//=============================================================================
// �`��
//=============================================================================
void CGame::Draw(void)
{
	// �����\���`��
	if (m_pFont != NULL) {
		m_pFont->Draw();
	}
}

//=============================================================================
// ���b�V���t�B�[���h�̐ݒ��ǂݍ���
//=============================================================================
HRESULT CGame::ReadConfig(char* pFilePath, CScene3D::MESHFIELD *meshInfo)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	FILE *pFile;

	pFile = fopen(pFilePath, "r");
	if (pFile != NULL)
	{//	File open succeeded
		bool bStart = false;
		char aReadStr[128];

		while (int nReturn = fscanf(pFile, "%s", aReadStr) != EOF)
		{
			// ������ǂݍ���
			if (bStart)
			{

				// �T�C�YX
				if (strcmp(aReadStr, "MESH_SIZE_X") == 0) {
					fscanf(pFile, "%s", aReadStr);
					if (strcmp(aReadStr, "=") == 0) {
						fscanf(pFile, "%f", &meshInfo->fSizeX);
					}
				}

				// �T�C�YZ
				if (strcmp(aReadStr, "MESH_SIZE_Z") == 0) {
					fscanf(pFile, "%s", aReadStr);
					if (strcmp(aReadStr, "=") == 0) {
						fscanf(pFile, "%f", &meshInfo->fSizeZ);
					}
				}

				// ������X
				if (strcmp(aReadStr, "MESH_DIVISION_X") == 0) {
					fscanf(pFile, "%s", aReadStr);
					if (strcmp(aReadStr, "=") == 0) {
						fscanf(pFile, "%d", &meshInfo->nXDivision);
					}
				}

				// ������Z
				if (strcmp(aReadStr, "MESH_DIVISION_Z") == 0) {
					fscanf(pFile, "%s", aReadStr);
					if (strcmp(aReadStr, "=") == 0) {
						fscanf(pFile, "%d", &meshInfo->nZDivision);
					}
				}

				// ������X
				if (strcmp(aReadStr, "MESH_DIVISION_X") == 0) {
					fscanf(pFile, "%s", aReadStr);
					if (strcmp(aReadStr, "=") == 0) {
						fscanf(pFile, "%d", &meshInfo->nXDivision);
					}
				}

				// �e�N�X�`��
				if (strcmp(aReadStr, "SET_MESH_TEXTURE") == 0) {
					while (1)
					{
						CScene3D::MESHFIELD_TEXTURE Tex;
						LPDIRECT3DTEXTURE9 pTexture;

						fscanf(pFile, "%s", aReadStr);

						// �摜�̃p�X
						if (strcmp(aReadStr, "PATH") == 0) {
							fscanf(pFile, "%s", aReadStr);
							if (strcmp(aReadStr, "=") == 0) {
								fscanf(pFile, "%s", aReadStr);
								D3DXCreateTextureFromFile(pDevice, aReadStr, &pTexture);
								Tex.pTexture = pTexture;
							}
						}

						// �u�����h�̎��
						if (strcmp(aReadStr, "BLEND") == 0) {
							fscanf(pFile, "%s", aReadStr);
							if (strcmp(aReadStr, "=") == 0) {
								fscanf(pFile, "%s", aReadStr);

								if (strcmp(aReadStr, "ADD") == 0) {
									Tex.blendState = D3DTOP_ADD;
								}
								else if (strcmp(aReadStr, "MODULATE") == 0) {
									Tex.blendState = D3DTOP_MODULATE;
								}
								else if (strcmp(aReadStr, "SUBTRACT") == 0) {
									Tex.blendState = D3DTOP_SUBTRACT;
								}
								else if (strcmp(aReadStr, "ADDSMOOTH") == 0) {
									Tex.blendState = D3DTOP_ADDSMOOTH;
								}
							}
						}

						// �摜�̕����̔{��
						if (strcmp(aReadStr, "DIVISION") == 0) {
							fscanf(pFile, "%s", aReadStr);
							if (strcmp(aReadStr, "=") == 0) {
								fscanf(pFile, "%f", &Tex.fDivision);
							}
						}

						// ���[�v�𔲂���
						if (strcmp(aReadStr, "END_MESH_TEXTURE") == 0) {
							meshInfo->vTexture.push_back(Tex);
							break;
						}
					}
				}

			}

			// #START / #END �������������
			if (strcmp(aReadStr, "#START") == 0)
				bStart = true;
			else if (strcmp(aReadStr, "#END") == 0)
				break;
		}

		fclose(pFile);
	}
	else return E_FAIL;

	return S_OK;
}

//=============================================================================
// �g�̐ݒ��ǂݍ���
//=============================================================================
HRESULT CGame::ReadWaveConfig(char* pFilePath, CScene3D::WAVE *waveInfo)
{
	FILE *pFile;

	pFile = fopen(pFilePath, "r");
	if (pFile != NULL)
	{//	File open succeeded
		char aStr[256];

		 // �g�̎�ޏ�������
		fscanf(pFile, "%s", aStr);
		if (strcmp(aStr, "RIPPLE") == 0)
			waveInfo->type = CScene3D::WAVETYPE_RIPPLE;
		else if (strcmp(aStr, "X") == 0)
			waveInfo->type = CScene3D::WAVETYPE_X;
		else if (strcmp(aStr, "Z") == 0)
			waveInfo->type = CScene3D::WAVETYPE_Z;

		// �g��̃I�t�Z�b�g
		fscanf(pFile, "%f", &waveInfo->rippleOffset.x);
		fscanf(pFile, "%f", &waveInfo->rippleOffset.z);

		// �e�N�X�`���X�N���[��
		fscanf(pFile, "%f", &waveInfo->texScroll[0].x);	// 0 X
		fscanf(pFile, "%f", &waveInfo->texScroll[0].y);	// 0 Y
		fscanf(pFile, "%f", &waveInfo->texScroll[1].x);	// 1 X
		fscanf(pFile, "%f", &waveInfo->texScroll[1].y);	// 1 Y

		// �e�N�X�`��0 �g X
		fscanf(pFile, "%f", &waveInfo->texParam[0][0].fFrequency);
		fscanf(pFile, "%f", &waveInfo->texParam[0][0].fSpeed);
		fscanf(pFile, "%f", &waveInfo->texParam[0][0].fAmplitude);

		// �e�N�X�`��0 �g Y
		fscanf(pFile, "%f", &waveInfo->texParam[0][1].fFrequency);
		fscanf(pFile, "%f", &waveInfo->texParam[0][1].fSpeed);
		fscanf(pFile, "%f", &waveInfo->texParam[0][1].fAmplitude);

		// �e�N�X�`��1 �g X
		fscanf(pFile, "%f", &waveInfo->texParam[1][0].fFrequency);
		fscanf(pFile, "%f", &waveInfo->texParam[1][0].fSpeed);
		fscanf(pFile, "%f", &waveInfo->texParam[1][0].fAmplitude);

		// �e�N�X�`��1 �g Y
		fscanf(pFile, "%f", &waveInfo->texParam[1][1].fFrequency);
		fscanf(pFile, "%f", &waveInfo->texParam[1][1].fSpeed);
		fscanf(pFile, "%f", &waveInfo->texParam[1][1].fAmplitude);

		// ���_ �g
		fscanf(pFile, "%f", &waveInfo->vtxParam.fFrequency);
		fscanf(pFile, "%f", &waveInfo->vtxParam.fSpeed);
		fscanf(pFile, "%f", &waveInfo->vtxParam.fAmplitude);

		fclose(pFile);
	}
	else return E_FAIL;

	return S_OK;
}

//=============================================================================
// �g�̐ݒ����������
//=============================================================================
HRESULT CGame::WriteWaveConfig(char* pFilePath, CScene3D::WAVE waveInfo)
{
	FILE *pFile;

	pFile = fopen(pFilePath, "w");
	if (pFile != NULL)
	{//	File open succeeded

		// �g�̎�ޏ�������
		if (waveInfo.type == CScene3D::WAVETYPE_RIPPLE)
			fprintf(pFile, "RIPPLE\n");
		else if (waveInfo.type == CScene3D::WAVETYPE_X)
			fprintf(pFile, "X\n");
		else if (waveInfo.type == CScene3D::WAVETYPE_Z)
			fprintf(pFile, "Z\n");

		// �g��̃I�t�Z�b�g
		fprintf(pFile, "%f\n", waveInfo.rippleOffset.x);
		fprintf(pFile, "%f\n", waveInfo.rippleOffset.z);

		// �e�N�X�`���X�N���[��
		fprintf(pFile, "%f\n", waveInfo.texScroll[0].x);	// 0 X
		fprintf(pFile, "%f\n", waveInfo.texScroll[0].y);	// 0 Y
		fprintf(pFile, "%f\n", waveInfo.texScroll[1].x);	// 1 X
		fprintf(pFile, "%f\n", waveInfo.texScroll[1].y);	// 1 Y

		// �e�N�X�`��0 �g X
		fprintf(pFile, "%f\n", waveInfo.texParam[0][0].fFrequency);
		fprintf(pFile, "%f\n", waveInfo.texParam[0][0].fSpeed);
		fprintf(pFile, "%f\n", waveInfo.texParam[0][0].fAmplitude);

		// �e�N�X�`��0 �g Y
		fprintf(pFile, "%f\n", waveInfo.texParam[0][1].fFrequency);
		fprintf(pFile, "%f\n", waveInfo.texParam[0][1].fSpeed);
		fprintf(pFile, "%f\n", waveInfo.texParam[0][1].fAmplitude);

		// �e�N�X�`��1 �g X
		fprintf(pFile, "%f\n", waveInfo.texParam[1][0].fFrequency);
		fprintf(pFile, "%f\n", waveInfo.texParam[1][0].fSpeed);
		fprintf(pFile, "%f\n", waveInfo.texParam[1][0].fAmplitude);

		// �e�N�X�`��1 �g Y
		fprintf(pFile, "%f\n", waveInfo.texParam[1][1].fFrequency);
		fprintf(pFile, "%f\n", waveInfo.texParam[1][1].fSpeed);
		fprintf(pFile, "%f\n", waveInfo.texParam[1][1].fAmplitude);

		// ���_ �g
		fprintf(pFile, "%f\n", waveInfo.vtxParam.fFrequency);
		fprintf(pFile, "%f\n", waveInfo.vtxParam.fSpeed);
		fprintf(pFile, "%f\n", waveInfo.vtxParam.fAmplitude);

		fclose(pFile);
	}
	else return E_FAIL;

	return S_OK;
}

//=============================================================================
// �g�̏���ݒ�
//=============================================================================
void CGame::SetWaveInfo(CScene3D::WAVE waveInfo)
{
	m_pMeshfield->SetVtxWaveType(waveInfo.type);
	m_pMeshfield->SetVtxWaveOffset(waveInfo.rippleOffset);
	m_pMeshfield->SetTextureScroll(0, waveInfo.texScroll[0]);
	m_pMeshfield->SetTextureScroll(1, waveInfo.texScroll[1]);
	m_pMeshfield->SetTextureWave(0, waveInfo.texParam[0][0], 0);
	m_pMeshfield->SetTextureWave(0, waveInfo.texParam[0][1], 1);
	m_pMeshfield->SetTextureWave(1, waveInfo.texParam[1][0], 0);
	m_pMeshfield->SetTextureWave(1, waveInfo.texParam[1][1], 1);
	m_pMeshfield->SetVtxWave(waveInfo.vtxParam);
}

//=============================================================================
// �g�̏����擾
//=============================================================================
CScene3D::WAVE CGame::GetWaveInfo(void)
{
	CScene3D::WAVE wave;

	wave.type = m_pMeshfield->GetVtxWaveType();
	wave.rippleOffset = m_pMeshfield->GetVtxWaveOffset();
	wave.texScroll[0] = m_pMeshfield->GetTextureScroll(0);
	wave.texScroll[1] = m_pMeshfield->GetTextureScroll(1);
	wave.texParam[0][0] = m_pMeshfield->GetTextureWave(0, 0);
	wave.texParam[0][1] = m_pMeshfield->GetTextureWave(0, 1);
	wave.texParam[1][0] = m_pMeshfield->GetTextureWave(1, 0);
	wave.texParam[1][1] = m_pMeshfield->GetTextureWave(1, 1);
	wave.vtxParam = m_pMeshfield->GetVtxWave();

	return wave;
}