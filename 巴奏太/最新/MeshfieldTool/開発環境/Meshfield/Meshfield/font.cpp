//=============================================================================
//
// 文字表示クラス [font.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "font.h"
#include "manager.h"
#include "renderer.h"
#include "gamescene.h"

//*****************************************************************************
// デバッグクラス
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CFont::CFont()
{
	
}

//=============================================================================
// デストラクタ
//=============================================================================
CFont::~CFont()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CFont::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 説明文用のフォント生成
	if (FAILED(D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont[FONT_IDX_TEXT]))) {
		return E_FAIL;
	}

	// 数値用のフォント生成
	if (FAILED(D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont[FONT_IDX_VALUE]))) {
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CFont::Uninit(void)
{
	// デバッグ情報表示用フォントの破棄
	for (int nCnt = 0; nCnt < FONT_IDX_MAX; nCnt++) {
		if (m_pFont[nCnt] != NULL) {
			m_pFont[nCnt]->Release();
			m_pFont[nCnt] = NULL;
		}
	}
}

//=============================================================================
// 更新
//=============================================================================
void CFont::Update(void)
{

}

//=============================================================================
// 描画
//=============================================================================
void CFont::Draw(void)
{
	// 文字描画
	DrawTexts();
	DrawValues();
}

//=============================================================================
// 説明文の描画処理
//=============================================================================
void CFont::DrawTexts(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[512];

	int nNum = sprintf(&aStr[0], "[W] [A] [S] [D] : 頂点の波の原点移動\n");
	nNum += sprintf(&aStr[nNum], "[N] : 頂点の法線を可視化\n");
	nNum += sprintf(&aStr[nNum], "[M] : ワイヤーフレーム\n");
	nNum += sprintf(&aStr[nNum], "[F1] : 保存\n");
	nNum += sprintf(&aStr[nNum], "[F5] : リセット\n");

	// テキスト描画
	m_pFont[FONT_IDX_TEXT]->DrawText(NULL, aStr, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}

//=============================================================================
// 数値の描画処理
//=============================================================================
void CFont::DrawValues(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[512];

	// 値取得
	D3DXVECTOR3 offsetPos = CManager::GetGame()->GetMeshfield()->GetVtxWaveOffset();
	CScene3D::WAVETYPE waveType = CManager::GetGame()->GetMeshfield()->GetVtxWaveType();
	int nTargetIdx = CManager::GetGame()->GetTargetTexIdx();
	int nXY = CManager::GetGame()->GetTargetXY();

	int nNum = sprintf(&aStr[0], "現在の対象テクスチャ [1] [2] : %d\n", nTargetIdx);
	nNum += sprintf(&aStr[nNum], "現在のテクスチャ分割数 [Z] [X] : %.2f\n", CManager::GetGame()->GetMeshfield()->GetTextureDiv(nTargetIdx));
	nNum += sprintf(&aStr[nNum], "現在のテクスチャスクロール値(X) [Q] [E] : %.3f\n", CManager::GetGame()->GetMeshfield()->GetTextureScroll(nTargetIdx).x);
	nNum += sprintf(&aStr[nNum], "現在のテクスチャスクロール値(Y) [R] [F]: %.3f\n", CManager::GetGame()->GetMeshfield()->GetTextureScroll(nTargetIdx).y);
	if (nXY == 0) 	nNum += sprintf(&aStr[nNum], "現在変更しているテクスチャ波の方向 [C] [V]: X\n");
	if (nXY == 1) 	nNum += sprintf(&aStr[nNum], "現在変更しているテクスチャ波の方向 [C] [V]: Y\n");
	nNum += sprintf(&aStr[nNum], "テクスチャの波の周期 [T] [G] : %.2f\n", CManager::GetGame()->GetMeshfield()->GetTextureWave(nTargetIdx, nXY).fFrequency);
	nNum += sprintf(&aStr[nNum], "テクスチャの波の速さ [Y] [H]: %.2f\n", CManager::GetGame()->GetMeshfield()->GetTextureWave(nTargetIdx, nXY).fSpeed);
	nNum += sprintf(&aStr[nNum], "テクスチャの波の大きさ [U] [J]: %.2f\n", CManager::GetGame()->GetMeshfield()->GetTextureWave(nTargetIdx, nXY).fAmplitude);

	nNum += sprintf(&aStr[nNum], "\n");
	if (waveType == CScene3D::WAVETYPE_RIPPLE) nNum += sprintf(&aStr[nNum], "頂点の波の種類 [3] [4] [5]: 波紋\n");
	if (waveType == CScene3D::WAVETYPE_X) nNum += sprintf(&aStr[nNum], "頂点の波の種類 [3] [4] [5]: X座標基準\n");
	if (waveType == CScene3D::WAVETYPE_Z) nNum += sprintf(&aStr[nNum], "頂点の波の種類 [3] [4] [5]: Z座標基準\n");
	nNum += sprintf(&aStr[nNum], "頂点の波の周期 [I] [K]: %.2f\n", CManager::GetGame()->GetMeshfield()->GetVtxWave().fFrequency);
	nNum += sprintf(&aStr[nNum], "頂点の波の速さ [O] [L]: %.2f\n", CManager::GetGame()->GetMeshfield()->GetVtxWave().fSpeed);
	nNum += sprintf(&aStr[nNum], "頂点の波の大きさ [P] [;]: %.2f\n", CManager::GetGame()->GetMeshfield()->GetVtxWave().fAmplitude);
	nNum += sprintf(&aStr[nNum], "波紋のオフセット位置 X:%.2f Z:%.2f\n", offsetPos.x, offsetPos.z);

	// テキスト描画
	m_pFont[FONT_IDX_TEXT]->DrawText(NULL, aStr, -1, &rect, DT_RIGHT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}