
//=============================================================================
//
// 2D�|���S�� [scene2D.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "camera.h"

//*****************************************************************************
// �|���S���N���X (�h�����F�I�u�W�F�N�g�N���X�j
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScene2D::CScene2D(int nPriority) : CScene(nPriority)
{
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pos = VECTOR3_ZERO;
	m_rot = VECTOR3_ZERO;
	m_size = D3DXVECTOR2(0, 0);
	m_fLength = 0;
	m_fAngle = 0;
	m_pVertex = NULL;
	m_bConvertPosition = true;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_bDraw = true;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScene2D::~CScene2D()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CScene2D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// ��������ݒ�
	SetPos(pos);
	SetRot(rot);
	SetSize(size);

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL)))
	{
		return E_FAIL;
	}

	// �Ίp���̒���
	m_fLength = sqrtf((m_size.x * m_size.x) + (m_size.y * m_size.y));

	// �Ίp���̊p�x
	m_fAngle = atan2f(-m_size.x, m_size.y);

	// ���_�o�b�t�@�����b�N���ă|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVertex, 0);

	// ���_����ݒ�
	// �ʒu
	m_pVertex[0].pos = D3DXVECTOR3(m_pos.x + sinf(m_fAngle + m_rot.z) * m_fLength / 2, m_pos.y + cosf(m_fAngle + m_rot.z) * m_fLength / 2, 0);
	m_pVertex[1].pos = D3DXVECTOR3(m_pos.x + sinf((-m_fAngle + D3DX_PI) + m_rot.z) * m_fLength / 2, m_pos.y + cosf((-m_fAngle + D3DX_PI) + m_rot.z) * m_fLength / 2, 0);
	m_pVertex[2].pos = D3DXVECTOR3(m_pos.x + sinf(-m_fAngle + m_rot.z) * m_fLength / 2, m_pos.y + cosf(-m_fAngle + m_rot.z) * m_fLength / 2, 0);
	m_pVertex[3].pos = D3DXVECTOR3(m_pos.x + sinf(m_fAngle + D3DX_PI + m_rot.z) * m_fLength / 2, m_pos.y + cosf(m_fAngle + D3DX_PI + m_rot.z) * m_fLength / 2, 0);

	// rhw
	m_pVertex[0].rhw = 1.0f;
	m_pVertex[1].rhw = 1.0f;
	m_pVertex[2].rhw = 1.0f;
	m_pVertex[3].rhw = 1.0f;

	// �F
	m_pVertex[0].col = m_col;
	m_pVertex[1].col = m_col;
	m_pVertex[2].col = m_col;
	m_pVertex[3].col = m_col;

	// �e�N�X�`��
	m_pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	m_pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	m_pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	m_pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// ���_�o�b�t�@�A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CScene2D::Uninit(void)
{
	// ���_�o�b�t�@���J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �C���X�^���X�j��
	this->Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CScene2D::Update(void)
{
	// �ʒu�̕ۑ�
	m_posPrev = GetPos();
}

//=============================================================================
// ���_�̍X�V
//=============================================================================
void CScene2D::UpdateVertex(void)
{
	// �Ίp���̒���
	m_fLength = sqrtf((m_size.x * m_size.x) + (m_size.y * m_size.y));

	// �Ίp���̊p�x
	m_fAngle = atan2f(-m_size.x, m_size.y);

	// ���_�o�b�t�@�����b�N���ă|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVertex, 0);

	// ���_�ʒu
	m_pVertex[0].pos = D3DXVECTOR3(m_pos.x + sinf(m_fAngle + m_rot.z) * m_fLength / 2, m_pos.y + cosf(m_fAngle + m_rot.z) * m_fLength / 2, 0);
	m_pVertex[1].pos = D3DXVECTOR3(m_pos.x + sinf((-m_fAngle + D3DX_PI) + m_rot.z) * m_fLength / 2, m_pos.y + cosf((-m_fAngle + D3DX_PI) + m_rot.z) * m_fLength / 2, 0);
	m_pVertex[2].pos = D3DXVECTOR3(m_pos.x + sinf(-m_fAngle + m_rot.z) * m_fLength / 2, m_pos.y + cosf(-m_fAngle + m_rot.z) * m_fLength / 2, 0);
	m_pVertex[3].pos = D3DXVECTOR3(m_pos.x + sinf(m_fAngle + D3DX_PI + m_rot.z) * m_fLength / 2, m_pos.y + cosf(m_fAngle + D3DX_PI + m_rot.z) * m_fLength / 2, 0);

	// ���_�o�b�t�@�A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CScene2D::Draw(void)
{
	if (m_bDraw) {
		// UI�Ƃ��Ďg�p�����ꍇ�͍��W�ϊ����s��Ȃ�
		if (m_bConvertPosition)
		{
			D3DXVECTOR3 screenPos = CManager::GetCamera()->WorldToScreenPosition(m_pos);

			// ���[���h���W����X�N���[�����W�ɕϊ����Ē��_�̈ʒu��ݒ肷��
			m_pVtxBuff->Lock(0, 0, (void**)&m_pVertex, 0);
			
			m_pVertex[0].pos = D3DXVECTOR3(screenPos.x + sinf(m_fAngle + m_rot.z) * m_fLength / 2, screenPos.y + cosf(m_fAngle + m_rot.z) * m_fLength / 2, 0);
			m_pVertex[1].pos = D3DXVECTOR3(screenPos.x + sinf((-m_fAngle + D3DX_PI) + m_rot.z) * m_fLength / 2, screenPos.y + cosf((-m_fAngle + D3DX_PI) + m_rot.z) * m_fLength / 2, 0);
			m_pVertex[2].pos = D3DXVECTOR3(screenPos.x + sinf(-m_fAngle + m_rot.z) * m_fLength / 2, screenPos.y + cosf(-m_fAngle + m_rot.z) * m_fLength / 2, 0);
			m_pVertex[3].pos = D3DXVECTOR3(screenPos.x + sinf(m_fAngle + D3DX_PI + m_rot.z) * m_fLength / 2, screenPos.y + cosf(m_fAngle + D3DX_PI + m_rot.z) * m_fLength / 2, 0);
			m_pVtxBuff->Unlock();
		}

		// �f�o�C�X�擾
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �|���S���̕`��
		pDevice->SetTexture(0, m_pTexture);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//=============================================================================
// �e�N�X�`���̃o�C���h
//=============================================================================
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void CScene2D::SetTextureUV(D3DXVECTOR2 Vertex_0, D3DXVECTOR2 Vertex_1, D3DXVECTOR2 Vertex_2, D3DXVECTOR2 Vertex_3)
{
	// ���_�o�b�t�@�����b�N���ă|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVertex, 0);

	// �e�N�X�`�����W�ݒ�
	m_pVertex[0].tex = Vertex_0;
	m_pVertex[1].tex = Vertex_1;
	m_pVertex[2].tex = Vertex_2;
	m_pVertex[3].tex = Vertex_3;

	// ���_�o�b�t�@�A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ���_�̐ݒ�
//=============================================================================
void CScene2D::SetVertices(D3DXVECTOR3 Vertex_0, D3DXVECTOR3 Vertex_1, D3DXVECTOR3 Vertex_2, D3DXVECTOR3 Vertex_3)
{
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVertex, 0);

	m_pVertex[0].pos = Vertex_0;
	m_pVertex[1].pos = Vertex_1;
	m_pVertex[2].pos = Vertex_2;
	m_pVertex[3].pos = Vertex_3;

	// ���_�o�b�t�@�A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ���_�ʒu�̎擾
//=============================================================================
D3DXVECTOR3 **CScene2D::GetVertices(void)
{
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVertex, 0);

	D3DXVECTOR3 **pVertex;
	D3DXVECTOR3 *apVertex[4];
	for (int nCnt = 0; nCnt < 4; nCnt++) {
		apVertex[nCnt] = &m_pVertex[nCnt].pos;
	}
	pVertex = apVertex;
	
	m_pVtxBuff->Unlock();

	return pVertex;
}

//=============================================================================
// �J���[�̐ݒ�
//=============================================================================
void CScene2D::SetColor(D3DXCOLOR col)
{
	m_col = col;

	// ���_�o�b�t�@�����b�N���ă|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVertex, 0);

	// �F�̐ݒ�
	m_pVertex[0].col = col;
	m_pVertex[1].col = col;
	m_pVertex[2].col = col;
	m_pVertex[3].col = col;

	// ���_�o�b�t�@�A�����b�N
	m_pVtxBuff->Unlock();
}


//=============================================================================
// �����֐�
//=============================================================================
CScene2D *CScene2D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	CScene2D *pScene2D;
	pScene2D = new CScene2D;
	if (pScene2D != NULL)
	{
		pScene2D->Init(pos, rot, size);
	}

	return pScene2D;
}