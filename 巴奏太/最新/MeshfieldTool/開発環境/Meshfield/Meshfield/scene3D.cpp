//=============================================================================
//
// 3Dポリゴン [scene3D.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "scene3D.h"
#include "manager.h"
#include "input.h"
#include "line.h"

//*****************************************************************************
// ポリゴンクラス (派生元：オブジェクトクラス）
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CScene3D::CScene3D()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CScene3D::~CScene3D()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScene3D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size)
{
	SetPos(pos);
	SetRot(rot);
	SetSize(size);

	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 必要頂点数を計算
	m_nVertices = (m_MeshField.nXDivision + 1) * (m_MeshField.nZDivision + 1);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nVertices, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL))) {
		return E_FAIL;
	}

	// 頂点バッファをロックしてポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVertex, 0);

	// 頂点座標を設定
	for (int nCntZ = 0; nCntZ < m_MeshField.nZDivision + 1; nCntZ++) {
		for (int nCntX = 0; nCntX < m_MeshField.nXDivision + 1; nCntX++) {
			float x = size.x / (m_MeshField.nXDivision);
			float z = size.z / (m_MeshField.nZDivision);

			int nCurrentVertex = nCntX + (nCntZ * (m_MeshField.nXDivision + 1));
			D3DXVECTOR3 setPos = D3DXVECTOR3(x * nCntX, 0.0f, -z * nCntZ);
			D3DXVECTOR3 offset = D3DXVECTOR3(size.x / 2, 0.0f, -size.z / 2);
			m_pVertex[nCurrentVertex].pos = setPos - offset;
		}
	}

	// 頂点色を設定
	for (int nCntVtx = 0; nCntVtx < m_nVertices; nCntVtx++)
		m_pVertex[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標を設定
	for (int nCntTex = 0; nCntTex < (signed)m_MeshField.vTexture.size(); nCntTex++) {

		// 座標設定
		for (int nCntZ = 0; nCntZ < m_MeshField.nZDivision + 1; nCntZ++) {
			for (int nCntX = 0; nCntX < m_MeshField.nXDivision + 1; nCntX++) {

				float x = m_MeshField.vTexture[nCntTex].fDivision / (m_MeshField.nXDivision);
				float z = m_MeshField.vTexture[nCntTex].fDivision / (m_MeshField.nZDivision);

				int nCurrentVertex = nCntX + (nCntZ * (m_MeshField.nXDivision + 1));
				D3DXVECTOR2 setTex = D3DXVECTOR2(x * nCntX, z * nCntZ);
				if (nCntTex == 0) 
					m_pVertex[nCurrentVertex].tex = setTex;
				else
					m_pVertex[nCurrentVertex].tex2 = setTex;

			}
		}

	}


	// すべての三角形を算出
	m_pTri = new TRIANGLE[m_MeshField.nXDivision * m_MeshField.nZDivision * 2];
	m_nTriangles = 0;
	for (int nCntZ = 0; nCntZ < m_MeshField.nZDivision; nCntZ++) {
		for (int nCntX = 0; nCntX < m_MeshField.nXDivision; nCntX++) {
			m_pTri[m_nTriangles].nV1 = nCntX + nCntZ * (m_MeshField.nZDivision + 1);
			m_pTri[m_nTriangles].nV2 = (nCntX + (m_MeshField.nXDivision + 1)) + nCntZ * (m_MeshField.nXDivision + 1);
			m_pTri[m_nTriangles].nV3 = (nCntX + 1 + (m_MeshField.nXDivision + 1)) + nCntZ * (m_MeshField.nXDivision + 1);

			m_pTri[m_nTriangles + 1].nV1 = nCntX + nCntZ * (m_MeshField.nZDivision + 1);
			m_pTri[m_nTriangles + 1].nV2 = (nCntX - m_MeshField.nXDivision + (m_MeshField.nXDivision + 1)) + nCntZ * (m_MeshField.nXDivision + 1);
			m_pTri[m_nTriangles + 1].nV3 = (nCntX + 1 + (m_MeshField.nXDivision + 1)) + nCntZ * (m_MeshField.nXDivision + 1);
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


	// 全頂点の法線を線で表示
	for (int nCntVtx = 0; nCntVtx < m_nVertices; nCntVtx++) {
		m_vLine.push_back(CLine::Create(m_pVertex[nCntVtx].pos, m_pVertex[nCntVtx].pos + m_pVertex[nCntVtx].nor * 10, 1, D3DXCOLOR(0, 1, 0, 1)));
	}


	// 頂点バッファアンロック
	m_pVtxBuff->Unlock();


	// 必要なインデックスの数を計算
	if (m_MeshField.nZDivision >= 3) {
		m_nIndices = (m_MeshField.nXDivision + 1) * 2 * m_MeshField.nZDivision + (m_MeshField.nZDivision - 1) * 2;
	}
	else {
		m_nIndices = (m_MeshField.nXDivision + 1) * 2 * m_MeshField.nZDivision + 2;
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
	for (int nCntZ = 0; nCntZ < m_MeshField.nZDivision; nCntZ++) {

		// 最後の行 (縮退を考えない行）
		if (nCntZ >= m_MeshField.nZDivision - 1) {
			for (int nCntX = 0; nCntX < m_MeshField.nXDivision + 1; nCntX++) {
				pIdx[nCntIdx] = (nCntX + (m_MeshField.nXDivision + 1)) + nCntZ * (m_MeshField.nXDivision + 1);
				pIdx[nCntIdx + 1] = nCntX + nCntZ * (m_MeshField.nXDivision + 1);

				nCntIdx += 2;
			}
		}
		// 縮退を考える行
		else {
			for (int nCntX = 0; nCntX < m_MeshField.nXDivision + 2; nCntX++) {
				if (nCntX < m_MeshField.nXDivision + 1) {
					pIdx[nCntIdx] = (nCntX + (m_MeshField.nXDivision + 1)) + nCntZ * (m_MeshField.nXDivision + 1);
					pIdx[nCntIdx + 1] = nCntX + nCntZ * (m_MeshField.nXDivision + 1);

					nCntIdx += 2;
				}
				else {
					pIdx[nCntIdx] = (nCntX - 1) + nCntZ * (m_MeshField.nXDivision + 1);
					pIdx[nCntIdx + 1] = (nCntZ + 2) * (m_MeshField.nXDivision + 1);

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
void CScene3D::Uninit(void)
{
	// 頂点バッファを開放
	if (m_pVtxBuff != NULL) {
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// インデックスバッファを開放
	if (m_pIdxBuff != NULL) {
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// 三角形フォーマットを開放
	delete[] m_pTri;

	// 共有頂点を開放
	delete[] m_pSharedVtx;

	// インスタンス破棄
	this->Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CScene3D::Update(void)
{
	// 時間カウント
	static int nCntFrame;
	nCntFrame++;

	// スクロール値加算
	static D3DXVECTOR2 scroll[2];
	scroll[0] += m_wave.texScroll[0];
	scroll[1] += m_wave.texScroll[1];

	// 頂点バッファをロックしてポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVertex, 0);
	
	// 全三角形ループ
	for (int nCnt = 0; nCnt < m_nTriangles; nCnt++) {

		// すべての面法線を計算
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

		// 頂点移動
		if (m_wave.type == WAVETYPE_RIPPLE) {
			// オフセットと頂点の距離を使って計算
			float distToVtx = D3DXVec2Length(&(D3DXVECTOR2(m_pVertex[nCnt].pos.x, m_pVertex[nCnt].pos.z) - D3DXVECTOR2(m_wave.rippleOffset.x, m_wave.rippleOffset.z)));
			m_pVertex[nCnt].pos.y = cosf(-distToVtx * m_wave.vtxParam.fFrequency + nCntFrame * m_wave.vtxParam.fSpeed) * m_wave.vtxParam.fAmplitude;
		}
		else if (m_wave.type == WAVETYPE_X) {
			// X軸の座標を使って計算
			m_pVertex[nCnt].pos.y = sinf(m_pVertex[nCnt].pos.x * m_wave.vtxParam.fFrequency + nCntFrame * m_wave.vtxParam.fSpeed) * m_wave.vtxParam.fAmplitude;
		}
		else if (m_wave.type == WAVETYPE_Z) {
			// Z軸の座標を使って計算
			m_pVertex[nCnt].pos.y = sinf(m_pVertex[nCnt].pos.z * m_wave.vtxParam.fFrequency + nCntFrame * m_wave.vtxParam.fSpeed) * m_wave.vtxParam.fAmplitude;
		}

		// 法線可視化
		m_vLine[nCnt]->SetDraw(m_bShowNormal);
		m_vLine[nCnt]->SetPos(m_pVertex[nCnt].pos, m_pVertex[nCnt].pos + m_pVertex[nCnt].nor * 10);
	}

	// テクスチャ座標更新
	for (int nCntTex = 0; nCntTex < (signed)m_MeshField.vTexture.size(); nCntTex++) {

		for (int nCntZ = 0; nCntZ < m_MeshField.nZDivision + 1; nCntZ++) {
			for (int nCntX = 0; nCntX < m_MeshField.nXDivision + 1; nCntX++) {

				float x = m_MeshField.vTexture[nCntTex].fDivision / (m_MeshField.nXDivision);
				float z = m_MeshField.vTexture[nCntTex].fDivision / (m_MeshField.nZDivision);

				int nCurrentVertex = nCntX + (nCntZ * (m_MeshField.nXDivision + 1));
				D3DXVECTOR2 setTex = D3DXVECTOR2(x * nCntX, z * nCntZ);
				if (nCntTex == 0) {
					m_pVertex[nCurrentVertex].tex = setTex + scroll[0];
					m_pVertex[nCurrentVertex].tex.x = m_pVertex[nCurrentVertex].tex.x + sinf(m_pVertex[nCurrentVertex].pos.z * m_wave.texParam[0][0].fFrequency + nCntFrame * m_wave.texParam[0][0].fSpeed) * m_wave.texParam[0][0].fAmplitude;
					m_pVertex[nCurrentVertex].tex.y = m_pVertex[nCurrentVertex].tex.y + sinf(m_pVertex[nCurrentVertex].pos.x * m_wave.texParam[0][1].fFrequency + nCntFrame * m_wave.texParam[0][1].fSpeed) * m_wave.texParam[0][1].fAmplitude;
				}
				else
					m_pVertex[nCurrentVertex].tex2 = setTex + scroll[1];
					m_pVertex[nCurrentVertex].tex2.x = m_pVertex[nCurrentVertex].tex2.x + sinf(m_pVertex[nCurrentVertex].pos.z * m_wave.texParam[1][0].fFrequency + nCntFrame * m_wave.texParam[1][0].fSpeed) * m_wave.texParam[1][0].fAmplitude;
					m_pVertex[nCurrentVertex].tex2.y = m_pVertex[nCurrentVertex].tex2.y + sinf(m_pVertex[nCurrentVertex].pos.x * m_wave.texParam[1][1].fFrequency + nCntFrame * m_wave.texParam[1][1].fSpeed) * m_wave.texParam[1][1].fAmplitude;
			}
		}

	}

	// 頂点バッファアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void CScene3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// Device
	D3DXMATRIX mtxRot, mtxTrans;	// Temporary matrices
	
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
	
	for (int nCntTex = 0; nCntTex < (signed)m_MeshField.vTexture.size(); nCntTex++) {
		if (nCntTex > 0) {
			// テクスチャステージの設定
			pDevice->SetTextureStageState(nCntTex, D3DTSS_COLOROP, m_MeshField.vTexture[nCntTex].blendState);
			pDevice->SetTextureStageState(nCntTex, D3DTSS_COLORARG1, D3DTA_CURRENT);
			pDevice->SetTextureStageState(nCntTex, D3DTSS_COLORARG2, D3DTA_TEXTURE);
		}

		// テクスチャ設定
		pDevice->SetTexture(nCntTex, m_MeshField.vTexture[nCntTex].pTexture);
	}


	// プリミティブ数算出
	int nPrimitives;
	if (m_MeshField.nZDivision == 1) {
		nPrimitives = m_MeshField.nXDivision * 2;
	}
	else{
		nPrimitives = (m_MeshField.nXDivision * 2 * m_MeshField.nZDivision) + (m_MeshField.nXDivision - 1) * 4;
	}

	// 描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		m_nIndices,
		0,
		nPrimitives);

	// テクスチャを元に戻す
	for (int nCntTex = 0; nCntTex < (signed)m_MeshField.vTexture.size(); nCntTex++) {
		pDevice->SetTexture(nCntTex, NULL);
	}
}

//=============================================================================
// 生成関数
//=============================================================================
CScene3D *CScene3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, MESHFIELD meshInfo)
{
	CScene3D *pScene3D;
	pScene3D = new CScene3D;
	if (pScene3D != NULL)
	{
		pScene3D->m_MeshField = meshInfo;
		pScene3D->Init(pos, rot, D3DXVECTOR3(meshInfo.fSizeX, 0, meshInfo.fSizeZ));
	}

	return pScene3D;
}

//=============================================================================
// テクスチャの波の設定
//=============================================================================
void CScene3D::SetTextureWave(int nIdx, WAVEPARAM param, int XorY)
{
	// 0がX
	if (XorY == 0) {
		m_wave.texParam[nIdx][0] = param;
	}
	// 1がY
	else if (XorY == 1) {
		m_wave.texParam[nIdx][1] = param;
	}
}

//=============================================================================
// テクスチャの波の設定を取得
//=============================================================================
CScene3D::WAVEPARAM CScene3D::GetTextureWave(int nIdx, int XorY)
{
	// 0がX
	if (XorY == 0) {
		return m_wave.texParam[nIdx][0];
	}
	// 1がY
	else if (XorY == 1) {
		return m_wave.texParam[nIdx][1];
	}

	WAVEPARAM empty = { 0,0,0 };
	return empty;
}