//=============================================================================
//
// �r�[���̏��� [beam.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "beam.h"
#include "manager.h"
#include "game.h"
#include "textureloader.h"
#include "life.h"
#include "particle.h"
#include "player.h"
#include "sound.h"
#include "bg3d.h"
#include "boss_core.h"

//*****************************************************************************
// �r�[���N���X ( �p����: �I�u�W�F�N�g�N���X [scene] )
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBeam::CBeam()
{
	m_nDamageCooltime = BEAM_DAMAGE_COOLTIME;
	m_bHitPlayer = false;
	m_beamState = BEAMSTATE_START;
	m_fBeamSizeMul = 0.1f;
	m_fBeamColMul = 1.0f;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBeam::~CBeam()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBeam::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	m_pos = pos;
	m_rot = rot;
	m_sizeV = size;
	for (int nCntBeam = 0; nCntBeam < BEAM_OBJUSAGE; nCntBeam++) {
		if (nCntBeam == 0) {
			m_apScene[nCntBeam] = CScene2D::Create(pos, rot, m_sizeV);
		}
		else {
			m_apScene[nCntBeam] = CScene2D::Create(pos, D3DXVECTOR3(rot.x, rot.y, rot.z + D3DX_PI / 2), m_sizeV);
		}
		m_apScene[nCntBeam]->BindTexture(CManager::GetTextureLoad()->m_TextureMp["LASER"]);
		m_apScene[nCntBeam]->SetPriority(2);
		m_apScene[nCntBeam]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		m_apScene[nCntBeam]->SetObjType(CScene::OBJTYPE_BEAM);
	}

	this->SetObjType(OBJTYPE_BEAMBASE);
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBeam::Uninit(void)
{
	// �r�[���{�̂�j��
	for (int nCnt = 0; nCnt < BEAM_OBJUSAGE; nCnt++) {
		if (m_apScene[nCnt] != NULL) {
			m_apScene[nCnt]->Uninit();
		}
	}
	CManager::GetSound()->Stop(CSound::SOUND_LABEL_SE_BEAM);

	// �J��
	Release();
}
//=============================================================================
// �X�V����
//=============================================================================
void CBeam::Update(void)
{
	static int nCountFrame;
	nCountFrame++;
	
	// �r�[����Ԃɂ�鏈��
	switch (m_beamState)
	{
	case BEAMSTATE_START:
		m_fBeamSizeMul += 0.06f;
		if (m_fBeamSizeMul >= 1.0f) {
			m_fBeamSizeMul = 1.0f;
			m_beamState = BEAMSTATE_NONE;
		}
		break;

	case BEAMSTATE_END:
		m_fBeamSizeMul -= 0.01f;
		m_fBeamColMul -= 0.01f;
		if (m_fBeamSizeMul <= 0 && m_fBeamColMul <= 0) {
			nCountFrame = 0;
			Uninit();
			return;
		}
		break;

	default:
		break;
	}

	// �����t���[�����J�E���g���ăr�[��������
	if (nCountFrame >= m_nActiveFrames) {
		m_beamState = BEAMSTATE_END;
	}

	// �v���C���[�ɑ΂���_���[�W�N�[���^�C����݂���
	Cooltime();
	
	for (int nCnt = 0; nCnt < 2; nCnt++) {
		// �T�C�Y�A�����x�ύX
		m_apScene[nCnt]->SetSize(D3DXVECTOR2(BOSS_CORE_BEAMATTACK_SIZE.x * m_fBeamSizeMul, BOSS_CORE_BEAMATTACK_SIZE.y));
		m_apScene[nCnt]->SetColor(D3DXCOLOR(1, 0, 0, m_fBeamColMul));

		// ��]�X�V
		m_apScene[0]->SetRot(m_rot);
		m_apScene[1]->SetRot(D3DXVECTOR3(m_rot.x, m_rot.y, m_rot.z + D3DX_PI / 2));

		m_apScene[nCnt]->UpdateVertex();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CBeam::Draw(void)
{

}

//=============================================================================
// �����֐�
//=============================================================================
CBeam *CBeam::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size, int nActiveFrames)
{
	// �v���C���[�̐����A������
	CBeam *pBeam = NULL;
	pBeam = new CBeam;
	if (pBeam != NULL)
	{
		pBeam->m_nActiveFrames = nActiveFrames;
		pBeam->Init(pos, rot, size);
	}

	return pBeam;
}

//=============================================================================
// �N�[���^�C��
//=============================================================================
void CBeam::Cooltime(void)
{
	static int nFrameCounter;
	if (m_bHitPlayer) {
		if (nFrameCounter == 0) {
			CGame::GetBG3D()->SetRippleFrequency(0.07f, 1);
			CGame::GetLife()->Damage(1);
			CParticle::Create(
				CGame::GetPlayer()->GetPos(),
				VECTOR3_ZERO,
				D3DXCOLOR(1, 0, 0, 1),
				D3DXVECTOR2(40, 40),
				CEffect::EFFECT_TEX_DEFAULT,
				1,
				7.0f,
				20,
				0.01f,
				0.01f,
				0,
				0);
		}
		if (nFrameCounter >= m_nDamageCooltime) {
			CGame::GetLife()->Damage(1);

			m_bHitPlayer = false;
			nFrameCounter = 0;
		}
		nFrameCounter++;
	}
	else {
		nFrameCounter = 0;
	}
}