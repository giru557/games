//=============================================================================
//
// 3D�w�i�N���X [bg3d.cpp]
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
// 3D�w�i�N���X (�h�����F�I�u�W�F�N�g�N���X [CScene]�j
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBackground3D::CBackground3D(int nPriority) : CScene(nPriority)
{
	// �g��̏����l
	m_ripple.fFrequency = BG_WAVE_FREQUENCY;
	m_ripple.fSpeed		= BG_WAVE_SPEED;
	m_ripple.fAmplitude = BG_WAVE_AMPLITUDE;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBackground3D::~CBackground3D()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBackground3D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// ��������ݒ�
	SetPos(pos);
	SetRot(rot);
	SetSize(size);

	// �I�u�W�F�N�g�̐ݒ�
	SetPriority(0);	// �`��D��x
	BindTexture(	// �e�N�X�`���ݒ�
		CManager::GetTextureLoad()->m_TextureMp["BACKGROUND"],
		CManager::GetTextureLoad()->m_TextureMp["BACKGROUND2"],
		CManager::GetTextureLoad()->m_TextureMp["BACKGROUND3"]);

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �K�v���_�����v�Z
	m_nVertices = (m_nXdivision + 1) * (m_nZdivision + 1);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nVertices, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL)))
	{
		return E_FAIL;
	}

	// ���_�o�b�t�@�����b�N���ă|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVertex, 0);

	// ���_�ʒu
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

	// ���_�F
	for (int nCntVtx = 0; nCntVtx < m_nVertices; nCntVtx++)
		m_pVertex[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W
	for (int nCntZ = 0; nCntZ < m_nZdivision + 1; nCntZ++) {
		for (int nCntX = 0; nCntX < m_nXdivision + 1; nCntX++) {
			float x = 1.0f / (m_nXdivision);
			float z = 1.0f / (m_nZdivision);

			int nCurrentVertex = nCntX + (nCntZ * (m_nXdivision + 1));
			D3DXVECTOR2 setTex = D3DXVECTOR2(x * nCntX, z * nCntZ);
			m_pVertex[nCurrentVertex].tex = setTex;
		}
	}

	// �e�N�X�`�����W2
	for (int nCntZ = 0; nCntZ < m_nZdivision + 1; nCntZ++) {
		for (int nCntX = 0; nCntX < m_nXdivision + 1; nCntX++) {
			float x = 1.0f / (m_nXdivision);
			float z = 1.0f / (m_nZdivision);

			int nCurrentVertex = nCntX + (nCntZ * (m_nXdivision + 1));
			D3DXVECTOR2 setTex = D3DXVECTOR2(x * nCntX, z * nCntZ);
			m_pVertex[nCurrentVertex].tex2 = setTex;
		}
	}

	// ���ׂĂ̎O�p�`���Z�o
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

	// ���_�o�b�t�@�A�����b�N
	m_pVtxBuff->Unlock();


	// �K�v�ȃC���f�b�N�X�̐����Z�o
	if (m_nZdivision >= 3) {
		m_nIndices = (m_nXdivision + 1) * 2 * m_nZdivision + (m_nZdivision - 1) * 2;
	}
	else {
		m_nIndices = (m_nXdivision + 1) * 2 * m_nZdivision + 2;
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
	for (int nCntZ = 0; nCntZ < m_nZdivision; nCntZ++) {

		// �Ō�̍s (�k�ނ��l���Ȃ��s�j
		if (nCntZ >= m_nZdivision - 1) {
			for (int nCntX = 0; nCntX < m_nXdivision + 1; nCntX++) {
				pIdx[nCntIdx] = (nCntX + (m_nXdivision + 1)) + nCntZ * (m_nXdivision + 1);
				pIdx[nCntIdx + 1] = nCntX + nCntZ * (m_nXdivision + 1);

				nCntIdx += 2;
			}
		}
		// �k�ނ��l����s
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

	//�C���f�b�N�X�o�b�t�@���A�����b�N
	m_pIdxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBackground3D::Uninit(void)
{
	// ���_�o�b�t�@���J��
	if (m_pVtxBuff != NULL){
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �C���f�b�N�X�o�b�t�@���J��
	if (m_pIdxBuff != NULL) {
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// ���I�m�ۂ����O�p�`���J��
	delete[] m_pTri;
	m_pTri = NULL;

	// ���I�m�ۂ������L���_���J��
	delete[] m_pSharedVtx;
	m_pSharedVtx = NULL;

	// �C���X�^���X�j��
	this->Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CBackground3D::Update(void)
{
	// ���ԃJ�E���g
	static int nCntFrame;
	nCntFrame++;

	// �g��̒l�ύX�^�C�}�[
	RippleTimer();

	// ���_�o�b�t�@�����b�N���ă|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVertex, 0);
	
	// �S�O�p�`���[�v
	for (int nCnt = 0; nCnt < m_nTriangles; nCnt++) {

		// �O�ς��g���đS�Ă̖ʖ@�����v�Z
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

		// ���S���璸�_�܂ł̋������g���Ĕg��G�t�F�N�g
		D3DXVECTOR3 offset = CCamera::WorldToScreenPosition(CGame::GetPlayer()->GetPos() + D3DXVECTOR3(-SCREEN_WIDTH / 2, -SCREEN_HEIGHT / 2, 0.0f));
		float distToVtx = D3DXVec2Length(&(D3DXVECTOR2(m_pVertex[nCnt].pos.x, m_pVertex[nCnt].pos.z) - D3DXVECTOR2(offset.x, offset.y)));
		m_pVertex[nCnt].pos.y = cosf(-distToVtx * m_ripple.fFrequency + nCntFrame * m_ripple.fSpeed) * m_ripple.fAmplitude;

		// �e�N�X�`�����W���v���C���[�̈ړ��l�ɍ��킹�Ă��炷
		D3DXVECTOR3 playerMove = CGame::GetPlayer()->GetMove();
		m_pVertex[nCnt].tex += D3DXVECTOR2(playerMove.x, playerMove.y) * 0.0002f;
		m_pVertex[nCnt].tex2 += D3DXVECTOR2(playerMove.x, playerMove.y) * 0.0004f;
	}

	// ���_�o�b�t�@�A�����b�N
	m_pVtxBuff->Unlock();

}

//=============================================================================
// �`�揈��
//=============================================================================
void CBackground3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// Device
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	
	// �e�N�X�`���X�e�[�W1�̐ݒ�
	if (!m_bBossTex)
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADDSMOOTH);
	else
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADDSIGNED2X);
	pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
	pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);

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

	// �e�N�X�`��
	pDevice->SetTexture(0, m_apTexture[0]);
	if (!m_bBossTex)
		pDevice->SetTexture(1, m_apTexture[1]);
	else
		pDevice->SetTexture(1, m_apTexture[2]);

	// �v���~�e�B�u�����v�Z
	int nPrimitives;
	if (m_nZdivision == 1) 
		nPrimitives = m_nXdivision * 2;
	else
		nPrimitives = (m_nXdivision * 2 * m_nZdivision) + (m_nXdivision - 1) * 4;

	// �`��
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		m_nIndices,
		0,
		nPrimitives);

	// �e�N�X�`�������Ƃɖ߂�
	pDevice->SetTexture(1, NULL);
	pDevice->SetTexture(0, NULL);
}

//=============================================================================
// �e�N�X�`���̃o�C���h
//=============================================================================
void CBackground3D::BindTexture(LPDIRECT3DTEXTURE9 pTexture1, LPDIRECT3DTEXTURE9 pTexture2, LPDIRECT3DTEXTURE9 pTexture3)
{
	m_apTexture[0] = pTexture1;
	m_apTexture[1] = pTexture2;
	m_apTexture[2] = pTexture3;
}

//=============================================================================
// �����֐�
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
// �g��̒l�ύX�^�C�}�[
//=============================================================================
void CBackground3D::RippleTimer(void)
{
	// �����^�C�}�[
	if (m_ripple.nCountFreq > 0) {
		m_ripple.nCountFreq--;
	}
	else {
		m_ripple.fFrequency += (BG_WAVE_FREQUENCY - m_ripple.fFrequency) * 0.1f;
		//m_ripple.fFrequency = BG_WAVE_FREQUENCY;
	}

	// ���x�^�C�}�[
	if (m_ripple.nCountSpd > 0) {
		m_ripple.nCountSpd--;
	}
	else {
		//m_ripple.fSpeed += (BG_WAVE_SPEED - m_ripple.fSpeed) * 0.005f;
		m_ripple.fSpeed = BG_WAVE_SPEED;
	}

	// �����^�C�}�[
	if (m_ripple.nCountAmp > 0) {
		m_ripple.nCountAmp--;
	}
	else {
		m_ripple.fAmplitude += (BG_WAVE_AMPLITUDE - m_ripple.fAmplitude) * 0.1f;
		//m_ripple.fAmplitude = BG_WAVE_AMPLITUDE;
	}
}