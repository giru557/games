//=============================================================================
//
// �{�X�^���b�g�̏��� [boss_turret.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "boss_turret.h"
#include "manager.h"
#include "game.h"
#include "textureloader.h"
#include "player.h"
#include "camera.h"
#include "wave.h"
#include "bullet.h"
#include "input.h"
#include "particle.h"
#include "debris.h"

//*****************************************************************************
// �{�X�^���b�g�N���X ( �p����: �G�N���X [ enemy ] )
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBossTurret::CBossTurret()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBossTurret::~CBossTurret()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBossTurret::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// ������
	if (FAILED(CScene2D::Init(pos, rot, size))) {
		return E_FAIL;
	}

	m_nLife = BOSS_TURRET_LIFE;
	SetPriority(ENEMY_DRAW_PRIORITY);
	SetPos(pos);
	SetRot(rot);
	SetSize(BOSS_TURRET_SIZE);
	SetObjType(OBJTYPE_BOSS);
	BindTexture(CManager::GetTextureLoad()->m_TextureMp["BOSSTURRET"]);
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBossTurret::Uninit(void)
{
	CDebris::Create(GetPos(), VECTOR3_ZERO, D3DXVECTOR2(150, 150));

	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CBossTurret::Update(void)
{
	CScene2D::Update();

	// �ʒu���擾
	D3DXVECTOR3 pos = this->GetPos();

	// �N�[���^�C������
	this->CollisionCooltime();

	// �`���[�W�V���b�g�̏���
	ChargedShot();

	// ���G
	Detection();

	// �����ݒ�
	if (m_bAiming) {	// ���G�͈͓��ɂ���ꍇ�̂�
		D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();
		D3DXVECTOR3 relPlayer = playerPos - pos;
		m_rotDest.z = atan2f(relPlayer.x, relPlayer.y) + D3DX_PI;
	}

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
		Uninit();
		return;
	}

	UpdateVertex();

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_N)) {
		m_bChargeShot = true;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CBossTurret::Draw(void)
{
	CScene2D::Draw();

}

//=============================================================================
// �����֐�
//=============================================================================
CBossTurret *CBossTurret::Create(D3DXVECTOR3 pos)
{
	// �����A������
	CBossTurret *pTurret = new CBossTurret;
	if (pTurret != NULL)
	{
		pTurret->Init(pos, VECTOR3_ZERO, (D3DXVECTOR2)VECTOR3_ZERO);
	}

	return pTurret;
}

//=============================================================================
// ���ߔ��ˍU��
//=============================================================================
void CBossTurret::ChargedShot(void)
{
	// �`���[�W�V���b�g�t���O�������Ă���ꍇ
	if (m_bChargeShot) {
		// �`���[�W���ԃJ�E���g�J�n
		m_nChargeTime++;

		if (m_nChargeTime < BOSS_TURRET_CHARGE_TIME) {
			CParticle::Create(	// Particle
				GetPos() + BOSS_TURRET_OFFSET,
				VECTOR3_ZERO,
				D3DXCOLOR(1, 0, 0, 1),
				D3DXVECTOR2(10, 10),
				CEffect::EFFECT_TEX_DEFAULT,
				1,
				5.0f,
				1,
				0.001f,
				0.001f,
				0,
				0);
		}

		// �`���[�W����
		if (m_nChargeTime >= BOSS_TURRET_CHARGE_TIME) {
			// �A�˗p�̃J�E���g
			static int nCntFrame;
			nCntFrame++;
			// ���܂����A�ˑ��x��
			if (nCntFrame >= BOSS_TURRET_CHARGE_SHOT_SPEED) {
				// �v���C���[�ɑ΂��Ďˌ�������
				D3DXVECTOR3 vecToPlayer = CManager::GetVectorFromPointAtoB(GetPos(), CGame::GetPlayer()->GetPos());
				D3DXVECTOR3 enemyPos = GetPos();
				D3DXVECTOR3 enemyRot = GetRot();
				D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();
				vecToPlayer = vecToPlayer * BULLET_SPEED_ENEMY;
				CBullet::Create(GetPos() + BOSS_TURRET_OFFSET, D3DXVECTOR3(0.0f, 0.0f, atan2f(playerPos.x - enemyPos.x, playerPos.y - enemyPos.y) - D3DX_PI / 2), D3DXVECTOR2(BULLET_WIDTH_DEFAULT, BULLET_HEIGHT_DEFAULT) * 1.5 , vecToPlayer, OBJTYPE_BOSS);

				// ���������J�E���g�A�b�v�ƘA�˃J�E���g���Z�b�g
				m_nChargeShots++;
				nCntFrame = 0;

				// ���ׂđł��؂�����
				if (m_nChargeShots >= BOSS_TURRET_CHARGE_SHOTS) {
					m_nChargeTime = 0;
					m_nChargeShots = 0;
					m_bChargeShot = false;
				}
			}
		}
	}
}

//=============================================================================
// ���G�͈�
//=============================================================================
void CBossTurret::Detection(void)
{
	if (CManager::GetDistanceAB(CGame::GetPlayer()->GetPos(), this->GetPos()) < BOSS_TURRET_DETECT_DIST) {
		m_bAiming = true;
	}
	else {
		m_bAiming = false;
	}
}