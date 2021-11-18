//=============================================================================
//
// ゲーム画面 [game.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "gamescene.h"
#include "input.h"
#include "manager.h"
#include "font.h"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CFont *CGame::m_pFont = NULL;
CScene3D *CGame::m_pMeshfield = NULL;

//*****************************************************************************
// ゲーム画面クラス ( 継承元: オブジェクトクラス [CScene] )
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CGame::CGame()
{
	m_nTargetTex = 0;
	m_nXY = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CGame::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size)
{
	// 文字表示クラス生成
	m_pFont = new CFont;
	if (m_pFont != NULL) {
		m_pFont->Init();
	}

	// メッシュフィールドの設定を読み込む
	CScene3D::MESHFIELD fieldInfo;
	ReadConfig("data/Meshfield.txt", &fieldInfo);

	// 波の設定を読み込む
	CScene3D::WAVE waveInfo = {};
	ReadWaveConfig("data/wave.txt", &waveInfo);

	// メッシュフィールド生成
	m_pMeshfield = CScene3D::Create(VECTOR3_ZERO, VECTOR3_ZERO, fieldInfo);

	// 波の設定適用
	SetWaveInfo(waveInfo);

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CGame::Uninit(void)
{
	// 文字表示クラス破棄
	if (m_pFont != NULL) {
		m_pFont->Uninit();
		m_pFont = NULL;
	}

	// このオブジェクトの開放
	this->Release();
}

//=============================================================================
// 更新
//=============================================================================
void CGame::Update(void)
{
	// 文字表示更新
	if (m_pFont != NULL) {
		m_pFont->Update();
	}

	// 法線表示切り替え
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	if (pKeyboard->GetTrigger(DIK_N))
	{
		m_pMeshfield->ToggleNormal();
	}

	// 対象テクスチャ変更
	if (pKeyboard->GetTrigger(DIK_1))
		m_nTargetTex = 0;
	else if (pKeyboard->GetTrigger(DIK_2))
		m_nTargetTex = 1;

	// 画像分割
	if (pKeyboard->GetTrigger(DIK_Z)) {
		m_pMeshfield->SetTextureDiv(m_nTargetTex, m_pMeshfield->GetTextureDiv(m_nTargetTex) - MESH_TEX_DIVISION_ADD);
	}
	else if (pKeyboard->GetTrigger(DIK_X)) {
		m_pMeshfield->SetTextureDiv(m_nTargetTex, m_pMeshfield->GetTextureDiv(m_nTargetTex) + MESH_TEX_DIVISION_ADD);
	}

	// スクロール
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

	// 縦横どっちの波を編集するか選択
	if (pKeyboard->GetTrigger(DIK_C)) {
		m_nXY = 0;
	}
	else if (pKeyboard->GetTrigger(DIK_V)) {
		m_nXY = 1;
	}

	// テクスチャの波
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
	
	// 頂点の波オフセット位置
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

	// 頂点の波のパラメーター設定
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

	// 頂点の波の種類
	if (pKeyboard->GetTrigger(DIK_3)) {
		m_pMeshfield->SetVtxWaveType(CScene3D::WAVETYPE_RIPPLE);
	}
	else if (pKeyboard->GetTrigger(DIK_4)) {
		m_pMeshfield->SetVtxWaveType(CScene3D::WAVETYPE_X);
	}
	else if (pKeyboard->GetTrigger(DIK_5)) {
		m_pMeshfield->SetVtxWaveType(CScene3D::WAVETYPE_Z);
	}

	// 現在の値を保存
	if (pKeyboard->GetTrigger(DIK_F1)) {
		WriteWaveConfig("data/wave.txt", GetWaveInfo());
	}

	// リセット
	if (pKeyboard->GetTrigger(DIK_F5)) {
		CScene3D::WAVE wave;
		memset(&wave, 0, sizeof(CScene3D::WAVE));
		SetWaveInfo(wave);
	}
}

//=============================================================================
// 描画
//=============================================================================
void CGame::Draw(void)
{
	// 文字表示描画
	if (m_pFont != NULL) {
		m_pFont->Draw();
	}
}

//=============================================================================
// メッシュフィールドの設定を読み込む
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
			// 文字列読み込み
			if (bStart)
			{

				// サイズX
				if (strcmp(aReadStr, "MESH_SIZE_X") == 0) {
					fscanf(pFile, "%s", aReadStr);
					if (strcmp(aReadStr, "=") == 0) {
						fscanf(pFile, "%f", &meshInfo->fSizeX);
					}
				}

				// サイズZ
				if (strcmp(aReadStr, "MESH_SIZE_Z") == 0) {
					fscanf(pFile, "%s", aReadStr);
					if (strcmp(aReadStr, "=") == 0) {
						fscanf(pFile, "%f", &meshInfo->fSizeZ);
					}
				}

				// 分割数X
				if (strcmp(aReadStr, "MESH_DIVISION_X") == 0) {
					fscanf(pFile, "%s", aReadStr);
					if (strcmp(aReadStr, "=") == 0) {
						fscanf(pFile, "%d", &meshInfo->nXDivision);
					}
				}

				// 分割数Z
				if (strcmp(aReadStr, "MESH_DIVISION_Z") == 0) {
					fscanf(pFile, "%s", aReadStr);
					if (strcmp(aReadStr, "=") == 0) {
						fscanf(pFile, "%d", &meshInfo->nZDivision);
					}
				}

				// 分割数X
				if (strcmp(aReadStr, "MESH_DIVISION_X") == 0) {
					fscanf(pFile, "%s", aReadStr);
					if (strcmp(aReadStr, "=") == 0) {
						fscanf(pFile, "%d", &meshInfo->nXDivision);
					}
				}

				// テクスチャ
				if (strcmp(aReadStr, "SET_MESH_TEXTURE") == 0) {
					while (1)
					{
						CScene3D::MESHFIELD_TEXTURE Tex;
						LPDIRECT3DTEXTURE9 pTexture;

						fscanf(pFile, "%s", aReadStr);

						// 画像のパス
						if (strcmp(aReadStr, "PATH") == 0) {
							fscanf(pFile, "%s", aReadStr);
							if (strcmp(aReadStr, "=") == 0) {
								fscanf(pFile, "%s", aReadStr);
								D3DXCreateTextureFromFile(pDevice, aReadStr, &pTexture);
								Tex.pTexture = pTexture;
							}
						}

						// ブレンドの種類
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

						// 画像の分割の倍率
						if (strcmp(aReadStr, "DIVISION") == 0) {
							fscanf(pFile, "%s", aReadStr);
							if (strcmp(aReadStr, "=") == 0) {
								fscanf(pFile, "%f", &Tex.fDivision);
							}
						}

						// ループを抜ける
						if (strcmp(aReadStr, "END_MESH_TEXTURE") == 0) {
							meshInfo->vTexture.push_back(Tex);
							break;
						}
					}
				}

			}

			// #START / #END 文字列を見つける
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
// 波の設定を読み込む
//=============================================================================
HRESULT CGame::ReadWaveConfig(char* pFilePath, CScene3D::WAVE *waveInfo)
{
	FILE *pFile;

	pFile = fopen(pFilePath, "r");
	if (pFile != NULL)
	{//	File open succeeded
		char aStr[256];

		 // 波の種類書き込み
		fscanf(pFile, "%s", aStr);
		if (strcmp(aStr, "RIPPLE") == 0)
			waveInfo->type = CScene3D::WAVETYPE_RIPPLE;
		else if (strcmp(aStr, "X") == 0)
			waveInfo->type = CScene3D::WAVETYPE_X;
		else if (strcmp(aStr, "Z") == 0)
			waveInfo->type = CScene3D::WAVETYPE_Z;

		// 波紋のオフセット
		fscanf(pFile, "%f", &waveInfo->rippleOffset.x);
		fscanf(pFile, "%f", &waveInfo->rippleOffset.z);

		// テクスチャスクロール
		fscanf(pFile, "%f", &waveInfo->texScroll[0].x);	// 0 X
		fscanf(pFile, "%f", &waveInfo->texScroll[0].y);	// 0 Y
		fscanf(pFile, "%f", &waveInfo->texScroll[1].x);	// 1 X
		fscanf(pFile, "%f", &waveInfo->texScroll[1].y);	// 1 Y

		// テクスチャ0 波 X
		fscanf(pFile, "%f", &waveInfo->texParam[0][0].fFrequency);
		fscanf(pFile, "%f", &waveInfo->texParam[0][0].fSpeed);
		fscanf(pFile, "%f", &waveInfo->texParam[0][0].fAmplitude);

		// テクスチャ0 波 Y
		fscanf(pFile, "%f", &waveInfo->texParam[0][1].fFrequency);
		fscanf(pFile, "%f", &waveInfo->texParam[0][1].fSpeed);
		fscanf(pFile, "%f", &waveInfo->texParam[0][1].fAmplitude);

		// テクスチャ1 波 X
		fscanf(pFile, "%f", &waveInfo->texParam[1][0].fFrequency);
		fscanf(pFile, "%f", &waveInfo->texParam[1][0].fSpeed);
		fscanf(pFile, "%f", &waveInfo->texParam[1][0].fAmplitude);

		// テクスチャ1 波 Y
		fscanf(pFile, "%f", &waveInfo->texParam[1][1].fFrequency);
		fscanf(pFile, "%f", &waveInfo->texParam[1][1].fSpeed);
		fscanf(pFile, "%f", &waveInfo->texParam[1][1].fAmplitude);

		// 頂点 波
		fscanf(pFile, "%f", &waveInfo->vtxParam.fFrequency);
		fscanf(pFile, "%f", &waveInfo->vtxParam.fSpeed);
		fscanf(pFile, "%f", &waveInfo->vtxParam.fAmplitude);

		fclose(pFile);
	}
	else return E_FAIL;

	return S_OK;
}

//=============================================================================
// 波の設定を書き込む
//=============================================================================
HRESULT CGame::WriteWaveConfig(char* pFilePath, CScene3D::WAVE waveInfo)
{
	FILE *pFile;

	pFile = fopen(pFilePath, "w");
	if (pFile != NULL)
	{//	File open succeeded

		// 波の種類書き込み
		if (waveInfo.type == CScene3D::WAVETYPE_RIPPLE)
			fprintf(pFile, "RIPPLE\n");
		else if (waveInfo.type == CScene3D::WAVETYPE_X)
			fprintf(pFile, "X\n");
		else if (waveInfo.type == CScene3D::WAVETYPE_Z)
			fprintf(pFile, "Z\n");

		// 波紋のオフセット
		fprintf(pFile, "%f\n", waveInfo.rippleOffset.x);
		fprintf(pFile, "%f\n", waveInfo.rippleOffset.z);

		// テクスチャスクロール
		fprintf(pFile, "%f\n", waveInfo.texScroll[0].x);	// 0 X
		fprintf(pFile, "%f\n", waveInfo.texScroll[0].y);	// 0 Y
		fprintf(pFile, "%f\n", waveInfo.texScroll[1].x);	// 1 X
		fprintf(pFile, "%f\n", waveInfo.texScroll[1].y);	// 1 Y

		// テクスチャ0 波 X
		fprintf(pFile, "%f\n", waveInfo.texParam[0][0].fFrequency);
		fprintf(pFile, "%f\n", waveInfo.texParam[0][0].fSpeed);
		fprintf(pFile, "%f\n", waveInfo.texParam[0][0].fAmplitude);

		// テクスチャ0 波 Y
		fprintf(pFile, "%f\n", waveInfo.texParam[0][1].fFrequency);
		fprintf(pFile, "%f\n", waveInfo.texParam[0][1].fSpeed);
		fprintf(pFile, "%f\n", waveInfo.texParam[0][1].fAmplitude);

		// テクスチャ1 波 X
		fprintf(pFile, "%f\n", waveInfo.texParam[1][0].fFrequency);
		fprintf(pFile, "%f\n", waveInfo.texParam[1][0].fSpeed);
		fprintf(pFile, "%f\n", waveInfo.texParam[1][0].fAmplitude);

		// テクスチャ1 波 Y
		fprintf(pFile, "%f\n", waveInfo.texParam[1][1].fFrequency);
		fprintf(pFile, "%f\n", waveInfo.texParam[1][1].fSpeed);
		fprintf(pFile, "%f\n", waveInfo.texParam[1][1].fAmplitude);

		// 頂点 波
		fprintf(pFile, "%f\n", waveInfo.vtxParam.fFrequency);
		fprintf(pFile, "%f\n", waveInfo.vtxParam.fSpeed);
		fprintf(pFile, "%f\n", waveInfo.vtxParam.fAmplitude);

		fclose(pFile);
	}
	else return E_FAIL;

	return S_OK;
}

//=============================================================================
// 波の情報を設定
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
// 波の情報を取得
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