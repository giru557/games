//=============================================================================
//
// テクスチャ読み込み [textureloader.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "textureloader.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//*****************************************************************************
// テクスチャ読み込みクラス
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CTextureLoad::CTextureLoad()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CTextureLoad::~CTextureLoad()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTextureLoad::Init(void)
{
	// テキストファイル読み込む
	if (FAILED(this->Read()))
		return E_FAIL;

	return S_OK;
}

//=============================================================================
// 終了処理
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
// ファイルを読み込む
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
			// 文字列読み込み
			if (bStart)
			{
				strcpy(&aReadPath[0], &aReadStr[0]);	// テクスチャのパスを保存
				fscanf(pFile, "%s", &aReadStr[0]);
				if (strcmp(&aReadStr[0], "=") == 0)
				{
					fscanf(pFile, "%s", &aReadStr[0]);
					strcpy(&aReadTag[0], &aReadStr[0]);	// テクスチャのタグを保存

					LPDIRECT3DTEXTURE9 pTexture;
					if (FAILED(D3DXCreateTextureFromFile(pDevice, &aReadPath[0], &pTexture))) {
						fclose(pFile);
						return E_FAIL;
					}
					
					m_TextureMp.insert(make_pair(aReadTag, pTexture));
				}
			}

			// #start文字列を見つける
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