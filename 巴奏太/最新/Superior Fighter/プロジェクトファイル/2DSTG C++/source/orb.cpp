//=============================================================================
//
// �I�[�u�̏��� [orb.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "orb.h"
#include "manager.h"
#include "game.h"
#include "textureloader.h"
#include "player.h"
#include "particle.h"
#include "energy.h"
#include "sound.h"

//*****************************************************************************
// �I�[�u�N���X ( �p����: 2D�I�u�W�F�N�g�N���X [scene2d] )
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
COrb::COrb()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
COrb::~COrb()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT COrb::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	SetPos(pos);
	SetRot(rot);
	SetSize(size);

	// ������
	if (FAILED(CScene2D::Init(pos, rot, size))) {
		return E_FAIL;
	}

	SetObjType(OBJTYPE_ORB);
	BindTexture(CManager::GetTextureLoad()->m_TextureMp["EFFECT"]);
	SetColor(ORB_COLOR);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void COrb::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void COrb::Update(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();
	float fDistToPlayer = CManager::GetDistanceAB(playerPos, pos);

	// �v���C���[�����m�����ɋ�����t���O���Ă�
	if (fDistToPlayer < ORB_FIND_TRIGGERDIST)
		m_bTrigger = true;

	// �t���O�����Ă���ǔ����ċz��������
	if (m_bTrigger) {
		D3DXVec3Lerp(&pos, &pos, &playerPos, 0.03f);
		if (fDistToPlayer < ORB_GET_TRIGGERDIST) {
			Apply();
			Uninit();
			return;
		}
	}
	SetPos(pos);

	// �I�[�u����p�[�e�B�N���o��
	CParticle::Create(
		pos,
		VECTOR3_ZERO,
		ORB_COLOR,
		D3DXVECTOR2(10, 10),
		CEffect::EFFECT_TEX_DEFAULT,
		1,
		1.0f,
		1,
		0.001f,
		0.001f,
		0,
		0);
}

//=============================================================================
// �`�揈��
//=============================================================================
void COrb::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �����_�[�X�e�[�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);		//�\�[�X�i�`�挳�j�̍������@�̐ݒ�
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			//�f�X�e�B�l�[�V�����i�`���j�̍������@�̐ݒ�

	// �`��
	CScene2D::Draw();

	// �����_�[�X�e�[�g�����Ƃɖ߂�
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		//�\�[�X�i�`�挳�j�̍������@�̐ݒ�
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	//�f�X�e�B�l�[�V�����i�`���j�̍������@�̐ݒ�
}

//=============================================================================
// �����֐�
//=============================================================================
COrb *COrb::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// �����A������
	COrb *pOrb;
	pOrb = new COrb;
	if (pOrb != NULL)
	{
		pOrb->Init(pos, rot, size);
	}

	return pOrb;
}

COrb *COrb::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size, int nCreateChance)
{
	int nRandCreate = rand() % 100 + 1;
	if (nCreateChance >= nRandCreate) {
		// �����A������
		COrb *pOrb;
		pOrb = new COrb;
		if (pOrb != NULL)
		{
			pOrb->Init(pos, rot, size);
		}

		return pOrb;
	}

	return NULL;
}

//=============================================================================
// �I�[�u�̌��ʂ�K�p����
//=============================================================================
void COrb::Apply(void)
{
	CGame::GetEnergy()->AddEnergy(ORB_ENERGY_GAIN);	// �G�l���M�[����
	CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_ORB);	// ����

	CParticle::Create(	// �p�[�e�B�N��
		CGame::GetPlayer()->GetPos(),
		VECTOR3_ZERO,
		ORB_COLOR,
		D3DXVECTOR2(20, 20),
		CEffect::EFFECT_TEX_DEFAULT,
		1,
		10.0f,
		50,
		0.01f,
		0.01f,
		0,
		0);
}