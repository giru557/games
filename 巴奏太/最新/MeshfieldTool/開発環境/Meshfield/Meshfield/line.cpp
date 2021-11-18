// LINE TEST
#include "line.h"
#include "manager.h"
#include "input.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLine::CLine()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLine::~CLine()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLine::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size)
{
	// ���𐶐�
	D3DXCreateLine(CManager::GetRenderer()->GetDevice(), &m_pLine);
	m_pLine->SetWidth(m_fWidth);
	SetObjType(OBJTYPE_LINE);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CLine::Uninit(void)
{
	// ����j��
	if (m_pLine != NULL) {
		m_pLine->Release();
		m_pLine = NULL;
	}

	// �J��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CLine::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CLine::Draw(void)
{
	if (m_bDraw) {

		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// Device
		D3DXMATRIX mtxView, mtxProj, mtxWV, mtxWVP, mtxRot, mtxTrans;		// �v�Z�p

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

		// �r���[�}�g���b�N�X�ƃv���W�F�N�V�����}�g���b�N�X���擾���ă��[���h�}�g���b�N�X�Ɗ|�����킹��
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);
		pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);
		D3DXMatrixMultiply(&mtxWV, &m_mtxWorld, &mtxView);
		D3DXMatrixMultiply(&mtxWVP, &mtxWV, &mtxProj);

		// �|�����킹���}�g���b�N�X���g���ĕ`��
		m_pLine->Begin();
		m_pLine->DrawTransform(m_aPoints, 2, &mtxWVP, m_col);
		m_pLine->End();

	}
}

//=============================================================================
// �����֐�
//=============================================================================
CLine *CLine::Create(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float fWidth, D3DXCOLOR col)
{
	CLine *pLine;
	pLine = new CLine;
	if (pLine != NULL)
	{
		pLine->m_aPoints[0] = pos1;
		pLine->m_aPoints[1] = pos2;
		pLine->m_col = col;
		pLine->m_fWidth = fWidth;
		pLine->Init(VECTOR3_ZERO, VECTOR3_ZERO, VECTOR3_ZERO);
	}

	return pLine;
}