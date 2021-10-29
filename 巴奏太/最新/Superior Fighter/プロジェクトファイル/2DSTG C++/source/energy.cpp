//=============================================================================
//
// �G�l���M�[�̏��� [energy.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "energy.h"
#include "manager.h"
#include "game.h"


//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************


//*****************************************************************************
// �G�l���M�[�N���X ( �p����: �I�u�W�F�N�g�N���X [scene] )
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEnergy::CEnergy()
{
	m_nEnergy = ENERGY_POINTS_INIT;
	m_fEnergyRatio = 0.0f;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEnergy::~CEnergy()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CEnergy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	SetPos(pos);
	SetRot(rot);
	SetSize(size);

	// �V�[������
	for (int nCntScene = 0; nCntScene < ENERGY_LAYER; nCntScene++) {
		m_apScene[nCntScene] = CScene2D::Create(GetPos(), GetRot(), GetSize());
		m_apScene[nCntScene]->SetConvertState(false);
		m_apScene[nCntScene]->SetPriority(ENERGY_DRAW_PRIORITY);
	}

	// �F�ݒ�
	m_apScene[ENERGY_IDX_BACK]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f));
	m_apScene[ENERGY_IDX_FRONT]->SetColor(D3DXCOLOR(0.6f, 1.0f, 1.0f, 1.0f));
	m_apScene[ENERGY_IDX_MIDDLE]->SetColor(D3DXCOLOR(0.0f, 0.7f, 0.7f, 1.0f));
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEnergy::Uninit(void)
{
	// �J������ (scene�̃|�C���^�z�񂩂�폜�j
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEnergy::Update(void)
{
	// ���R��
	Regenerate(ENERGY_POINTS_REGENERATE);

	// �G�l���M�[�̃o�[�̏���
	BarProc();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEnergy::Draw(void)
{

}

//=============================================================================
// �����֐�
//=============================================================================
CEnergy *CEnergy::Create(void)
{
	// �����A������
	CEnergy *pEnergy;
	pEnergy = new CEnergy;
	if (pEnergy != NULL)
	{
		pEnergy->Init(ENERGY_POSITION, VECTOR3_ZERO, ENERGY_SIZE);
	}

	return pEnergy;
}

//=============================================================================
// �o�[�̏����܂Ƃ�
//=============================================================================
void CEnergy::BarProc(void)
{
	// ����
	m_fEnergyRatio = (float)m_nEnergy / (float)ENERGY_POINTS_MAX;

	// ���_�擾
	D3DXVECTOR3 *apVertexFront[4];
	memcpy(&apVertexFront[0], m_apScene[ENERGY_IDX_FRONT]->GetVertices(), sizeof(apVertexFront));

	// �ʒu�A�T�C�Y�ύX
	D3DXVECTOR2 size = ENERGY_SIZE;
	*apVertexFront[2] = D3DXVECTOR3(apVertexFront[0]->x + (size.x * m_fEnergyRatio), apVertexFront[2]->y, 0.0f);
	*apVertexFront[3] = D3DXVECTOR3(apVertexFront[1]->x + (size.x * m_fEnergyRatio), apVertexFront[3]->y, 0.0f);
	m_apScene[ENERGY_IDX_FRONT]->SetVertices(*apVertexFront[0], *apVertexFront[1], *apVertexFront[2], *apVertexFront[3]);

	// �e�N�X�`�����W
	m_apScene[ENERGY_IDX_FRONT]->SetTextureUV(
		D3DXVECTOR2(0.0f, 1.0f),
		D3DXVECTOR2(0.0f, 0.0f),
		D3DXVECTOR2(1.0f * m_fEnergyRatio, 1.0f),
		D3DXVECTOR2(1.0f * m_fEnergyRatio, 0.0f));



	// ���_�擾
	D3DXVECTOR3 *apVertexMid[4];
	memcpy(&apVertexMid[0], m_apScene[ENERGY_IDX_MIDDLE]->GetVertices(), sizeof(apVertexFront));

	if (apVertexMid[2]->x > apVertexFront[2]->x) {
		D3DXVec3Lerp(apVertexMid[2], apVertexMid[2], apVertexFront[2], 0.05f);
		D3DXVec3Lerp(apVertexMid[3], apVertexMid[3], apVertexFront[3], 0.05f);
	}
	else {
		apVertexMid[2] = apVertexFront[2];
		apVertexMid[3] = apVertexFront[3];
	}

	m_apScene[ENERGY_IDX_MIDDLE]->SetVertices(*apVertexMid[0], *apVertexMid[1], *apVertexMid[2], *apVertexMid[3]);

}

//=============================================================================
// �G�l���M�[�g�p ���������ꍇtrue��Ԃ�
//=============================================================================
bool CEnergy::UseEnergy(int nValue)
{
	if (m_nEnergy - nValue < 0) {
		return false;
	}
	else {
		m_nEnergy = m_nEnergy - nValue;
		return true;
	}
}

//=============================================================================
// �G�l���M�[���R��
//=============================================================================
void CEnergy::Regenerate(int nRegenSpeed)
{
	static int nTickRegen;
	nTickRegen++;
	if (nTickRegen > nRegenSpeed) {
		if (m_nEnergy < ENERGY_POINTS_MAX)
			m_nEnergy++;

		nTickRegen = 0;
	}
}

//=============================================================================
// �G�l���M�[���񕜂�����
//=============================================================================
void CEnergy::AddEnergy(int nValue)
{
	if (m_nEnergy + nValue >= ENERGY_POINTS_MAX) {
		m_nEnergy = ENERGY_POINTS_MAX;
	}
	else {
		m_nEnergy += nValue;
	}
}