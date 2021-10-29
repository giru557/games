//=============================================================================
//
// �e�N�X�`���ǂݍ��� [textureloader.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "textureloader.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//*****************************************************************************
// �e�N�X�`���ǂݍ��݃N���X
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTextureLoad::CTextureLoad()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTextureLoad::~CTextureLoad()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTextureLoad::Init(void)
{
	// �e�L�X�g�t�@�C���ǂݍ���
	if (FAILED(this->Read()))
		return E_FAIL;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTextureLoad::Uninit(void)
{
	for (auto itr = m_TextureMp.begin(); itr != m_TextureMp.end(); itr++)
	{
		if (itr->second != NULL) {
			itr->second->Release();
			itr->second = NULL;
		}
	}
}

//=============================================================================
// �t�@�C����ǂݍ���
//=============================================================================
HRESULT CTextureLoad::Read(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	FILE *pFile;

	pFile = fopen(m_pFilename, "r");
	if (pFile != NULL)
	{//	File open succeeded
		bool bStart = false;
		char aReadStr[128];
		char aReadPath[128];
		char aReadTag[128];

		while (int nReturn = fscanf(pFile, "%s", &aReadStr[0]) != EOF)
		{
			// ������ǂݍ���
			if (bStart)
			{
				strcpy(&aReadPath[0], &aReadStr[0]);	// �e�N�X�`���̃p�X��ۑ�
				fscanf(pFile, "%s", &aReadStr[0]);
				if (strcmp(&aReadStr[0], "=") == 0)
				{
					fscanf(pFile, "%s", &aReadStr[0]);
					strcpy(&aReadTag[0], &aReadStr[0]);	// �e�N�X�`���̃^�O��ۑ�

					LPDIRECT3DTEXTURE9 pTexture;
					if (FAILED(D3DXCreateTextureFromFile(pDevice, &aReadPath[0], &pTexture))) {
						fclose(pFile);
						return E_FAIL;
					}
					
					m_TextureMp.insert(make_pair(aReadTag, pTexture));
				}
			}

			// #start�������������
			if (strcmp(&aReadStr[0], "#start") == 0)
				bStart = true;
			else if (strcmp(&aReadStr[0], "#end") == 0)
				break;
		}

		fclose(pFile);
	}
	else return E_FAIL;

	return S_OK;
}