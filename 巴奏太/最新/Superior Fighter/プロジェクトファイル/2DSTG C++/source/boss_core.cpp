//=============================================================================
//
// �{�X�^���b�g�̏��� [boss_turret.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "boss_core.h"
#include "manager.h"
#include "game.h"
#include "textureloader.h"
#include "player.h"
#include "camera.h"
#include "wave.h"
#include "bullet.h"
#include "input.h"
#include "particle.h"
#include "life.h"
#include "beam.h"
#include "sound.h"

//*****************************************************************************
// �{�X�R�A�N���X ( �p����: �G�N���X [ enemy ] )
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBossCore::CBossCore()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBossCore::~CBossCore()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBossCore::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// ������
	if (FAILED(CScene2D::Init(pos, rot, size))) {
		return E_FAIL;
	}

	m_nLife = BOSS_CORE_LIFE;
	SetPriority(ENEMY_DRAW_PRIORITY);
	SetPos(pos);
	SetRotDest(rot);
	SetRot(rot);
	SetSize(BOSS_CORE_SIZE);
	SetObjType(OBJTYPE_BOSSCORE);
	BindTexture(CManager::GetTextureLoad()->m_TextureMp["BOSSCORE"]);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBossCore::Uninit(void)
{
	if (m_pBeam != NULL) {
		m_pBeam->Uninit();
	}
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CBossCore::Update(void)
{
	CScene2D::Update();

	// �ʒu���擾
	D3DXVECTOR3 pos = GetPos();

	// �N�[���^�C������
	this->CollisionCooltime();

	// �r�[������
	this->BeamAttack();

	// �_���[�W�F��߂�
	FixColor();

	// ��]�����炩�ɂ���
	this->SmoothTurn();

	// ��ʊO�̂��`�悵�Ȃ�
	this->CheckScreen();

	// ���S�m�F
	if (m_nLife <= 0)
	{
		Dead();
		return;
	}

	UpdateVertex();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CBossCore::Draw(void)
{
	CScene2D::Draw();

}

//=============================================================================
// �����֐�
//=============================================================================
CBossCore *CBossCore::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// �����A������
	CBossCore *pCore = new CBossCore;
	if (pCore != NULL)
	{
		pCore->Init(pos, rot, (D3DXVECTOR2)VECTOR3_ZERO);
	}

	return pCore;
}

//=============================================================================
// �r�[���U������
//=============================================================================
void CBossCore::SetBeamAttack(void)
{
	m_bBeamAttack = true;
}

//=============================================================================
// �r�[���U������
//=============================================================================
void CBossCore::BeamAttack(void)
{
	static int nFrameCount;
	if (m_bBeamAttack) {
		nFrameCount++;

		if (nFrameCount <= BOSS_CORE_BEAMATTACK_CHARGETIME) {
			PreAttack();
		}
		else if (nFrameCount > BOSS_CORE_BEAMATTACK_CHARGETIME) {
			m_pBeam = CBeam::Create(this->GetPos(), this->GetRot(), BOSS_CORE_BEAMATTACK_SIZE, BOSS_CORE_BEAMATTACK_ACTIVETIME);
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_BEAM);

			m_bBeamAttack = false;
			nFrameCount = 0;
		}
	}
	
}

//=============================================================================
// �\������
//=============================================================================
void CBossCore::PreAttack(void)
{
	D3DXVECTOR3 rot = GetRot();
	float fAngle[4];
	fAngle[0] = rot.z;
	fAngle[1] = rot.z + D3DX_PI / 2;
	fAngle[2] = rot.z - D3DX_PI;
	fAngle[3] = rot.z - D3DX_PI / 2;
	for (int nCnt = 0; nCnt < 4; nCnt++) {
		CParticle::Create(	// Particle
			GetPos() + D3DXVECTOR3(sinf(fAngle[nCnt]), cosf(fAngle[nCnt]), 0) * 100,
			VECTOR3_ZERO,
			D3DXCOLOR(1, 0, 0, 1),
			D3DXVECTOR2(30, 30),
			CEffect::EFFECT_TEX_DEFAULT,
			1,
			8.0f,
			1,
			0.01f,
			0.01f,
			0,
			0);
	}
	static int nFrameCount;
	nFrameCount++;

	if (nFrameCount > 30) {
		for (int nCnt = 0; nCnt < 100; nCnt++) {
			float fAng1 = D3DX_PI * 2 / 100;
			float fAngAll = fAng1 * nCnt;

			CParticle::Create(
				GetPos() + D3DXVECTOR3(sinf(fAngAll), cosf(fAngAll), 0) * 300,
				VECTOR3_ZERO,
				D3DXCOLOR(1, 1, 0, 1),
				D3DXVECTOR2(40, 40),
				CEffect::EFFECT_TEX_DEFAULT,
				1,
				5.0f,
				1,
				0.0007f,
				0.0007f,
				D3DXToDegree(fAngAll),
				1);
		}
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_FORCEFIELD);

		nFrameCount = 0;
	}
}