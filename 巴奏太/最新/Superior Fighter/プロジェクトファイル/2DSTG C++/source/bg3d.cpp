//=============================================================================
//
// 3D背景クラス [bg3d.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "bg3d.h"
#include "manager.h"
#include "input.h"
#include "player.h"
#include "game.h"
#include "camera.h"
#include "textureloader.h"

//*****************************************************************************
// 3D背景クラス (派生元：オブジェクトクラス [CScene]）
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CBackground3D::CBackground3D(int nPriority) : CScene(nPriority)
{
	// 波紋の初期値
	m_ripple.fFrequency = BG_WAVE_FREQUENCY;
	m_ripple.fSpeed		= BG_WAVE_SPEED;
	m_ripple.fAmplitude = BG_WAVE_AMPLITUDE;
}

//=============================================================================
// デストラクタ
//=============================================================================
CBackground3D::~CBackground3D()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBackground3D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// 引数から設定
	SetPos(pos);
	SetRot(rot);
	SetSize(size);

	// オブジェクトの設定
	SetPriority(0);	// 描画優先度
	BindTexture(	// テクスチャ設定
		CManager::GetTextureLoad()->m_TextureMp["BACKGROUND"],
		CManager::GetTextureLoad()->m_TextureMp["BACKGROUND2"],
		CManager::GetTextureLoad()->m_TextureMp["BACKGROUND3"]);

	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 必要頂点数を計算
	m_nVertices = (m_nXdivision + 1) * (m_nZdivision + 1);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nVertices, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL)))
	{
		return E_FAIL;
	}

	// 頂点バッファをロックしてポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVertex, 0);

	// 頂点位置
	for (int nCntZ = 0; nCntZ < m_nZdivision + 1; nCntZ++) {
		for (int nCntX = 0; nCntX < m_nXdivision + 1; nCntX++) {
			float x = size.x / (m_nXdivision);
			float z = size.y / (m_nZdivision);

			int nCurrentVertex = nCntX + (nCntZ * (m_nXdivision + 1));
			D3DXVECTOR3 setPos = D3DXVECTOR3(x * nCntX, 0.0f, -z * nCntZ);
			D3DXVECTOR3 offset = D3DXVECTOR3(size.x / 2, 0.0f, -size.y / 2);
			m_pVertex[nCurrentVertex].pos = setPos - offset;
		}
	}

	// 頂点色
	for (int nCntVtx = 0; nCntVtx < m_nVertices; nCntVtx++)
		m_pVertex[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標
	for (int nCntZ = 0; nCntZ < m_nZdivision + 1; nCntZ++) {
		for (int nCntX = 0; nCntX < m_nXdivision + 1; nCntX++) {
			float x = 1.0f / (m_nXdivision);
			float z = 1.0f / (m_nZdivision);

			int nCurrentVertex = nCntX + (nCntZ * (m_nXdivision + 1));
			D3DXVECTOR2 setTex = D3DXVECTOR2(x * nCntX, z * nCntZ);
			m_pVertex[nCurrentVertex].tex = setTex;
		}
	}

	// テクスチャ座標2
	for (int nCntZ = 0; nCntZ < m_nZdivision + 1; nCntZ++) {
		for (int nCntX = 0; nCntX < m_nXdivision + 1; nCntX++) {
			float x = 1.0f / (m_nXdivision);
			float z = 1.0f / (m_nZdivision);

			int nCurrentVertex = nCntX + (nCntZ * (m_nXdivision + 1));
			D3DXVECTOR2 setTex = D3DXVECTOR2(x * nCntX, z * nCntZ);
			m_pVertex[nCurrentVertex].tex2 = setTex;
		}
	}

	// すべての三角形を算出
	m_pTri = new TRIANGLE[m_nXdivision * m_nZdivision * 2];
	m_nTriangles = 0;
	for (int nCntZ = 0; nCntZ < m_nZdivision; nCntZ++) {
		for (int nCntX = 0; nCntX < m_nXdivision; nCntX++) {
			m_pTri[m_nTriangles].nV1 = nCntX + nCntZ * (m_nZdivision + 1);
			m_pTri[m_nTriangles].nV2 = (nCntX + (m_nXdivision + 1)) + nCntZ * (m_nXdivision + 1);
			m_pTri[m_nTriangles].nV3 = (nCntX + 1 + (m_nXdivision + 1)) + nCntZ * (m_nXdivision + 1);

			m_pTri[m_nTriangles + 1].nV1 = nCntX + nCntZ * (m_nZdivision + 1);
			m_pTri[m_nTriangles + 1].nV2 = (nCntX - m_nXdivision + (m_nXdivision + 1)) + nCntZ * (m_nXdivision + 1);
			m_pTri[m_nTriangles + 1].nV3 = (nCntX + 1 + (m_nXdivision + 1)) + nCntZ * (m_nXdivision + 1);
			m_nTriangles += 2;
		}
	}

	// すべての面法線を計算
	for (int nCnt = 0; nCnt < m_nTriangles; nCnt++) {
		D3DXVECTOR3 nor;
		if (nCnt % 2 == 0) {
			D3DXVec3Cross(&nor, &(m_pVertex[m_pTri[nCnt].nV3].pos - m_pVertex[m_pTri[nCnt].nV1].pos), &(m_pVertex[m_pTri[nCnt].nV2].pos - m_pVertex[m_pTri[nCnt].nV1].pos));
			D3DXVec3Normalize(&nor, &nor);
		}
		else {
			D3DXVec3Cross(&nor, &(m_pVertex[m_pTri[nCnt].nV2].pos - m_pVertex[m_pTri[nCnt].nV1].pos), &(m_pVertex[m_pTri[nCnt].nV3].pos - m_pVertex[m_pTri[nCnt].nV1].pos));
			D3DXVec3Normalize(&nor, &nor);
		}

		m_pTri[nCnt].nor = nor;
	}

	// 頂点を共有している面を見つける
	m_pSharedVtx = new vector<int>[m_nVertices];
	for (int nCnt = 0; nCnt < m_nVertices; nCnt++) {
		// 面を総当りで共有してる頂点見つける
		for (int nCntTri = 0; nCntTri < m_nTriangles; nCntTri++) {
			if (nCnt == m_pTri[nCntTri].nV1 ||
				nCnt == m_pTri[nCntTri].nV2 ||
				nCnt == m_pTri[nCntTri].nV3) {
				m_pSharedVtx[nCnt].push_back(nCntTri);
			}
		}
	}

	// 頂点バッファアンロック
	m_pVtxBuff->Unlock();


	// 必要なインデックスの数を算出
	if (m_nZdivision >= 3) {
		m_nIndices = (m_nXdivision + 1) * 2 * m_nZdivision + (m_nZdivision - 1) * 2;
	}
	else {
		m_nIndices = (m_nXdivision + 1) * 2 * m_nZdivision + 2;
	}

	// インデックスバッファ生成
	pDevice->CreateIndexBuffer(
		sizeof(WORD) * m_nIndices,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	WORD *pIdx;	// インデックス情報へのポインタ

	//インデックスバッファをロックし、番号データへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	//インデックス設定
	int nCntIdx = 0;
	for (int nCntZ = 0; nCntZ < m_nZdivision; nCntZ++) {

		// 最後の行 (縮退を考えない行）
		if (nCntZ >= m_nZdivision - 1) {
			for (int nCntX = 0; nCntX < m_nXdivision + 1; nCntX++) {
				pIdx[nCntIdx] = (nCntX + (m_nXdivision + 1)) + nCntZ * (m_nXdivision + 1);
				pIdx[nCntIdx + 1] = nCntX + nCntZ * (m_nXdivision + 1);

				nCntIdx += 2;
			}
		}
		// 縮退を考える行
		else {
			for (int nCntX = 0; nCntX < m_nXdivision + 2; nCntX++) {
				if (nCntX < m_nXdivision + 1) {
					pIdx[nCntIdx] = (nCntX + (m_nXdivision + 1)) + nCntZ * (m_nXdivision + 1);
					pIdx[nCntIdx + 1] = nCntX + nCntZ * (m_nXdivision + 1);

					nCntIdx += 2;
				}
				else {
					pIdx[nCntIdx] = (nCntX - 1) + nCntZ * (m_nXdivision + 1);
					pIdx[nCntIdx + 1] = (nCntZ + 2) * (m_nXdivision + 1);

					nCntIdx += 2;
				}
			}
		}

	}

	//インデックスバッファをアンロック
	m_pIdxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBackground3D::Uninit(void)
{
	// 頂点バッファを開放
	if (m_pVtxBuff != NULL){
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// インデックスバッファを開放
	if (m_pIdxBuff != NULL) {
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// 動的確保した三角形を開放
	delete[] m_pTri;
	m_pTri = NULL;

	// 動的確保した共有頂点を開放
	delete[] m_pSharedVtx;
	m_pSharedVtx = NULL;

	// インスタンス破棄
	this->Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBackground3D::Update(void)
{
	// 時間カウント
	static int nCntFrame;
	nCntFrame++;

	// 波紋の値変更タイマー
	RippleTimer();

	// 頂点バッファをロックしてポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVertex, 0);
	
	// 全三角形ループ
	for (int nCnt = 0; nCnt < m_nTriangles; nCnt++) {

		// 外積を使って全ての面法線を計算
		D3DXVECTOR3 nor;
		if (nCnt % 2 == 0) {
			D3DXVec3Cross(&nor, &(m_pVertex[m_pTri[nCnt].nV3].pos - m_pVertex[m_pTri[nCnt].nV1].pos), &(m_pVertex[m_pTri[nCnt].nV2].pos - m_pVertex[m_pTri[nCnt].nV1].pos));
			D3DXVec3Normalize(&nor, &nor);
		}
		else {
			D3DXVec3Cross(&nor, &(m_pVertex[m_pTri[nCnt].nV2].pos - m_pVertex[m_pTri[nCnt].nV1].pos), &(m_pVertex[m_pTri[nCnt].nV3].pos - m_pVertex[m_pTri[nCnt].nV1].pos));
			D3DXVec3Normalize(&nor, &nor);
		}
		m_pTri[nCnt].nor = nor;

	}

	// 全頂点ループ
	D3DXVECTOR3 nor = VECTOR3_ZERO;
	for (int nCnt = 0; nCnt < m_nVertices; nCnt++) {

		// 共有してる頂点を全部足す
		for (int nCntShared = 0; nCntShared < (signed)m_pSharedVtx[nCnt].size(); nCntShared++) {
			nor += m_pTri[m_pSharedVtx[nCnt][nCntShared]].nor;
		}

		// 平均（全部足して正規化）して代入
		D3DXVec3Normalize(&nor, &nor);
		m_pVertex[nCnt].nor = nor;

		// 中心から頂点までの距離を使って波紋エフェクト
		D3DXVECTOR3 offset = CCamera::WorldToScreenPosition(CGame::GetPlayer()->GetPos() + D3DXVECTOR3(-SCREEN_WIDTH / 2, -SCREEN_HEIGHT / 2, 0.0f));
		float distToVtx = D3DXVec2Length(&(D3DXVECTOR2(m_pVertex[nCnt].pos.x, m_pVertex[nCnt].pos.z) - D3DXVECTOR2(offset.x, offset.y)));
		m_pVertex[nCnt].pos.y = cosf(-distToVtx * m_ripple.fFrequency + nCntFrame * m_ripple.fSpeed) * m_ripple.fAmplitude;

		// テクスチャ座標をプレイヤーの移動値に合わせてずらす
		D3DXVECTOR3 playerMove = CGame::GetPlayer()->GetMove();
		m_pVertex[nCnt].tex += D3DXVECTOR2(playerMove.x, playerMove.y) * 0.0002f;
		m_pVertex[nCnt].tex2 += D3DXVECTOR2(playerMove.x, playerMove.y) * 0.0004f;
	}

	// 頂点バッファアンロック
	m_pVtxBuff->Unlock();

}

//=============================================================================
// 描画処理
//=============================================================================
void CBackground3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// Device
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	
	// テクスチャステージ1の設定
	if (!m_bBossTex)
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADDSMOOTH);
	else
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADDSIGNED2X);
	pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
	pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXVECTOR3 rot = GetRot();
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXVECTOR3 pos = GetPos();
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャ
	pDevice->SetTexture(0, m_apTexture[0]);
	if (!m_bBossTex)
		pDevice->SetTexture(1, m_apTexture[1]);
	else
		pDevice->SetTexture(1, m_apTexture[2]);

	// プリミティブ数を計算
	int nPrimitives;
	if (m_nZdivision == 1) 
		nPrimitives = m_nXdivision * 2;
	else
		nPrimitives = (m_nXdivision * 2 * m_nZdivision) + (m_nXdivision - 1) * 4;

	// 描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		m_nIndices,
		0,
		nPrimitives);

	// テクスチャをもとに戻す
	pDevice->SetTexture(1, NULL);
	pDevice->SetTexture(0, NULL);
}

//=============================================================================
// テクスチャのバインド
//=============================================================================
void CBackground3D::BindTexture(LPDIRECT3DTEXTURE9 pTexture1, LPDIRECT3DTEXTURE9 pTexture2, LPDIRECT3DTEXTURE9 pTexture3)
{
	m_apTexture[0] = pTexture1;
	m_apTexture[1] = pTexture2;
	m_apTexture[2] = pTexture3;
}

//=============================================================================
// 生成関数
//=============================================================================
CBackground3D *CBackground3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size, int nXDivision, int nZDivision)
{
	CBackground3D *pBG3D;
	pBG3D = new CBackground3D;
	if (pBG3D != NULL)
	{
		pBG3D->m_nXdivision = nXDivision;
		pBG3D->m_nZdivision = nZDivision;
		pBG3D->Init(pos, rot, size);
	}

	return pBG3D;
}

//=============================================================================
// 波紋の値変更タイマー
//=============================================================================
void CBackground3D::RippleTimer(void)
{
	// 周期タイマー
	if (m_ripple.nCountFreq > 0) {
		m_ripple.nCountFreq--;
	}
	else {
		m_ripple.fFrequency += (BG_WAVE_FREQUENCY - m_ripple.fFrequency) * 0.1f;
		//m_ripple.fFrequency = BG_WAVE_FREQUENCY;
	}

	// 速度タイマー
	if (m_ripple.nCountSpd > 0) {
		m_ripple.nCountSpd--;
	}
	else {
		//m_ripple.fSpeed += (BG_WAVE_SPEED - m_ripple.fSpeed) * 0.005f;
		m_ripple.fSpeed = BG_WAVE_SPEED;
	}

	// 周期タイマー
	if (m_ripple.nCountAmp > 0) {
		m_ripple.nCountAmp--;
	}
	else {
		m_ripple.fAmplitude += (BG_WAVE_AMPLITUDE - m_ripple.fAmplitude) * 0.1f;
		//m_ripple.fAmplitude = BG_WAVE_AMPLITUDE;
	}
}