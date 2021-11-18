//=============================================================================
//
// 3D�|���S�� [scene3D.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "scene3D.h"
#include "manager.h"
#include "input.h"
#include "line.h"

//*****************************************************************************
// �|���S���N���X (�h�����F�I�u�W�F�N�g�N���X�j
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScene3D::CScene3D()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScene3D::~CScene3D()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CScene3D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size)
{
	SetPos(pos);
	SetRot(rot);
	SetSize(size);

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �K�v���_�����v�Z
	m_nVertices = (m_MeshField.nXDivision + 1) * (m_MeshField.nZDivision + 1);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nVertices, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL))) {
		return E_FAIL;
	}

	// ���_�o�b�t�@�����b�N���ă|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVertex, 0);

	// ���_���W��ݒ�
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

	// ���_�F��ݒ�
	for (int nCntVtx = 0; nCntVtx < m_nVertices; nCntVtx++)
		m_pVertex[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W��ݒ�
	for (int nCntTex = 0; nCntTex < (signed)m_MeshField.vTexture.size(); nCntTex++) {

		// ���W�ݒ�
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


	// ���ׂĂ̎O�p�`���Z�o
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

	// ���ׂĂ̖ʖ@�����v�Z
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

	// ���_�����L���Ă���ʂ�������
	m_pSharedVtx = new vector<int>[m_nVertices];
	for (int nCnt = 0; nCnt < m_nVertices; nCnt++) {
		// �ʂ𑍓���ŋ��L���Ă钸�_������
		for (int nCntTri = 0; nCntTri < m_nTriangles; nCntTri++) {
			if (nCnt == m_pTri[nCntTri].nV1 ||
				nCnt == m_pTri[nCntTri].nV2 ||
				nCnt == m_pTri[nCntTri].nV3) {
				m_pSharedVtx[nCnt].push_back(nCntTri);
			}
		}
	}


	// �S���_�̖@������ŕ\��
	for (int nCntVtx = 0; nCntVtx < m_nVertices; nCntVtx++) {
		m_vLine.push_back(CLine::Create(m_pVertex[nCntVtx].pos, m_pVertex[nCntVtx].pos + m_pVertex[nCntVtx].nor * 10, 1, D3DXCOLOR(0, 1, 0, 1)));
	}


	// ���_�o�b�t�@�A�����b�N
	m_pVtxBuff->Unlock();


	// �K�v�ȃC���f�b�N�X�̐����v�Z
	if (m_MeshField.nZDivision >= 3) {
		m_nIndices = (m_MeshField.nXDivision + 1) * 2 * m_MeshField.nZDivision + (m_MeshField.nZDivision - 1) * 2;
	}
	else {
		m_nIndices = (m_MeshField.nXDivision + 1) * 2 * m_MeshField.nZDivision + 2;
	}

	// �C���f�b�N�X�o�b�t�@����
	pDevice->CreateIndexBuffer(
		sizeof(WORD) * m_nIndices,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	WORD *pIdx;	// �C���f�b�N�X���ւ̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N���A�ԍ��f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	//�C���f�b�N�X�ݒ�
	int nCntIdx = 0;
	for (int nCntZ = 0; nCntZ < m_MeshField.nZDivision; nCntZ++) {

		// �Ō�̍s (�k�ނ��l���Ȃ��s�j
		if (nCntZ >= m_MeshField.nZDivision - 1) {
			for (int nCntX = 0; nCntX < m_MeshField.nXDivision + 1; nCntX++) {
				pIdx[nCntIdx] = (nCntX + (m_MeshField.nXDivision + 1)) + nCntZ * (m_MeshField.nXDivision + 1);
				pIdx[nCntIdx + 1] = nCntX + nCntZ * (m_MeshField.nXDivision + 1);

				nCntIdx += 2;
			}
		}
		// �k�ނ��l����s
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

	//�C���f�b�N�X�o�b�t�@���A�����b�N
	m_pIdxBuff->Unlock();



	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CScene3D::Uninit(void)
{
	// ���_�o�b�t�@���J��
	if (m_pVtxBuff != NULL) {
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �C���f�b�N�X�o�b�t�@���J��
	if (m_pIdxBuff != NULL) {
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// �O�p�`�t�H�[�}�b�g���J��
	delete[] m_pTri;

	// ���L���_���J��
	delete[] m_pSharedVtx;

	// �C���X�^���X�j��
	this->Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CScene3D::Update(void)
{
	// ���ԃJ�E���g
	static int nCntFrame;
	nCntFrame++;

	// �X�N���[���l���Z
	static D3DXVECTOR2 scroll[2];
	scroll[0] += m_wave.texScroll[0];
	scroll[1] += m_wave.texScroll[1];

	// ���_�o�b�t�@�����b�N���ă|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVertex, 0);
	
	// �S�O�p�`���[�v
	for (int nCnt = 0; nCnt < m_nTriangles; nCnt++) {

		// ���ׂĂ̖ʖ@�����v�Z
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
	
	// �S���_���[�v
	D3DXVECTOR3 nor = VECTOR3_ZERO;
	for (int nCnt = 0; nCnt < m_nVertices; nCnt++) {

		// ���L���Ă钸�_��S������
		for (int nCntShared = 0; nCntShared < (signed)m_pSharedVtx[nCnt].size(); nCntShared++) {
			nor += m_pTri[m_pSharedVtx[nCnt][nCntShared]].nor;
		}

		// ���ρi�S�������Đ��K���j���đ��
		D3DXVec3Normalize(&nor, &nor);
		m_pVertex[nCnt].nor = nor;

		// ���_�ړ�
		if (m_wave.type == WAVETYPE_RIPPLE) {
			// �I�t�Z�b�g�ƒ��_�̋������g���Čv�Z
			float distToVtx = D3DXVec2Length(&(D3DXVECTOR2(m_pVertex[nCnt].pos.x, m_pVertex[nCnt].pos.z) - D3DXVECTOR2(m_wave.rippleOffset.x, m_wave.rippleOffset.z)));
			m_pVertex[nCnt].pos.y = cosf(-distToVtx * m_wave.vtxParam.fFrequency + nCntFrame * m_wave.vtxParam.fSpeed) * m_wave.vtxParam.fAmplitude;
		}
		else if (m_wave.type == WAVETYPE_X) {
			// X���̍��W���g���Čv�Z
			m_pVertex[nCnt].pos.y = sinf(m_pVertex[nCnt].pos.x * m_wave.vtxParam.fFrequency + nCntFrame * m_wave.vtxParam.fSpeed) * m_wave.vtxParam.fAmplitude;
		}
		else if (m_wave.type == WAVETYPE_Z) {
			// Z���̍��W���g���Čv�Z
			m_pVertex[nCnt].pos.y = sinf(m_pVertex[nCnt].pos.z * m_wave.vtxParam.fFrequency + nCntFrame * m_wave.vtxParam.fSpeed) * m_wave.vtxParam.fAmplitude;
		}

		// �@������
		m_vLine[nCnt]->SetDraw(m_bShowNormal);
		m_vLine[nCnt]->SetPos(m_pVertex[nCnt].pos, m_pVertex[nCnt].pos + m_pVertex[nCnt].nor * 10);
	}

	// �e�N�X�`�����W�X�V
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

	// ���_�o�b�t�@�A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CScene3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// Device
	D3DXMATRIX mtxRot, mtxTrans;	// Temporary matrices
	
	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f
	D3DXVECTOR3 rot = GetRot();
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXVECTOR3 pos = GetPos();
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);
	
	for (int nCntTex = 0; nCntTex < (signed)m_MeshField.vTexture.size(); nCntTex++) {
		if (nCntTex > 0) {
			// �e�N�X�`���X�e�[�W�̐ݒ�
			pDevice->SetTextureStageState(nCntTex, D3DTSS_COLOROP, m_MeshField.vTexture[nCntTex].blendState);
			pDevice->SetTextureStageState(nCntTex, D3DTSS_COLORARG1, D3DTA_CURRENT);
			pDevice->SetTextureStageState(nCntTex, D3DTSS_COLORARG2, D3DTA_TEXTURE);
		}

		// �e�N�X�`���ݒ�
		pDevice->SetTexture(nCntTex, m_MeshField.vTexture[nCntTex].pTexture);
	}


	// �v���~�e�B�u���Z�o
	int nPrimitives;
	if (m_MeshField.nZDivision == 1) {
		nPrimitives = m_MeshField.nXDivision * 2;
	}
	else{
		nPrimitives = (m_MeshField.nXDivision * 2 * m_MeshField.nZDivision) + (m_MeshField.nXDivision - 1) * 4;
	}

	// �`��
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		m_nIndices,
		0,
		nPrimitives);

	// �e�N�X�`�������ɖ߂�
	for (int nCntTex = 0; nCntTex < (signed)m_MeshField.vTexture.size(); nCntTex++) {
		pDevice->SetTexture(nCntTex, NULL);
	}
}

//=============================================================================
// �����֐�
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
// �e�N�X�`���̔g�̐ݒ�
//=============================================================================
void CScene3D::SetTextureWave(int nIdx, WAVEPARAM param, int XorY)
{
	// 0��X
	if (XorY == 0) {
		m_wave.texParam[nIdx][0] = param;
	}
	// 1��Y
	else if (XorY == 1) {
		m_wave.texParam[nIdx][1] = param;
	}
}

//=============================================================================
// �e�N�X�`���̔g�̐ݒ���擾
//=============================================================================
CScene3D::WAVEPARAM CScene3D::GetTextureWave(int nIdx, int XorY)
{
	// 0��X
	if (XorY == 0) {
		return m_wave.texParam[nIdx][0];
	}
	// 1��Y
	else if (XorY == 1) {
		return m_wave.texParam[nIdx][1];
	}

	WAVEPARAM empty = { 0,0,0 };
	return empty;
}