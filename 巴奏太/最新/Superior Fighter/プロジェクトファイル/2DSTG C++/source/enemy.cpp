//=============================================================================
//
// �G�N���X [enemy.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "enemy.h"
#include "manager.h"
#include "input.h"
#include "bullet.h"
#include "spritesheet.h"
#include "player.h"
#include "life.h"
#include "camera.h"
#include "game.h"
#include "textureloader.h"
#include "sound.h"
#include "wave.h"
#include "particle.h"
#include "effect.h"
#include "orb.h"

//*****************************************************************************
// �G�N���X ( �p����: 2D�|���S�� [scene2D] )
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEnemy::CEnemy()
{
	// initialize
	m_nLife = ENEMY_LIFE_DEFAULT;
	m_move = VECTOR3_ZERO;
	m_rotDest = VECTOR3_ZERO;
	m_fSpeed = ENEMY_SPEED_DEFAULT;
	m_enemyType = ENEMY_TYPE_NONE;
	m_act = ENEMY_ACTION_NONE;
	m_nShootSpeed = ENEMY_SHOOT_SPEED;
	m_nShootCounter = 0;
	m_fColorGradient = 1.0f;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEnemy::~CEnemy()
{
}

//=============================================================================
// ������
//=============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// ����������
	if (FAILED(CScene2D::Init(pos, rot, size)))
	{
		return E_FAIL;
	}

	SetPriority(ENEMY_DRAW_PRIORITY);

	// �I�u�W�F�N�g�̎�ނ�ݒ�
	if (GetObjType() == OBJTYPE_NONE)
	{
		SetObjType(OBJTYPE_ENEMY);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEnemy::Uninit(void)
{
	// �I��
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEnemy::Update(void)
{
	CScene2D::Update();

	// �ʒu���擾
	D3DXVECTOR3 pos = this->GetPos();

	// �ʒu�̍X�V�ƌ����̐ݒ�
	m_move.x += (0 - m_move.x) * ENEMY_SPEED_DECAY;
	m_move.y += (0 - m_move.y) * ENEMY_SPEED_DECAY;
	pos += m_move;

	// �ʒu�𔽉f
	this->SetPos(pos);

	// ���݂̍s���^�C�v�����ɓG�𓮂���
	this->ActionProc();

	// �ˌ��̏���
	this->ShootPlayer();

	// �N�[���^�C������
	this->CollisionCooltime();

	// �O��
	this->Trail();

	// �_���[�W�F���C��
	this->FixColor();

	// �����ݒ�
	D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();
	D3DXVECTOR3 relPlayer = playerPos - pos;
	m_rotDest.z = atan2f(relPlayer.x, relPlayer.y) + D3DX_PI;

	// ��]�����炩�ɂ���
	this->SmoothTurn();

	// ��ʊO�͕`�悵�Ȃ�
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
void CEnemy::Draw(void)
{
	// �`��
	CScene2D::Draw();
}

//=============================================================================
// �_���[�W��^����
//=============================================================================
void CEnemy::Damage(int nValue)
{
	// ��ʂ�h�炷
	CManager::GetCamera()->Shake(3.0f, 5);

	// �_���[�W�F�ɐ��߂�
	m_fColorGradient = 0.0f;

	// �̗͂���_���[�W��������
	m_nLife -= nValue;
}

//=============================================================================
// �e�N�X�`���̐ݒ�
//=============================================================================
void CEnemy::SetTexture(ENEMY_TYPE enemyType)
{
	CTextureLoad *pTex = CManager::GetTextureLoad();

	switch (enemyType)
	{
	case ENEMY_TYPE_NONE:
		this->BindTexture(pTex->m_TextureMp["ENEMY"]);
		break;

	case ENEMY_TYPE_GUNNER:
		this->BindTexture(pTex->m_TextureMp["ENEMY_GUNNER"]);
		break;

	case ENEMY_TYPE_CHARGER:
		this->BindTexture(pTex->m_TextureMp["ENEMY_CHARGER"]);
		break;
	
	default:
		break;
	}
}

//=============================================================================
// �s���̓��e�������֐�
//=============================================================================
void CEnemy::ActionProc(void)
{
	CPlayer *pPlayer = CGame::GetPlayer();
	D3DXVECTOR3 playerPos = pPlayer->GetPos();
	D3DXVECTOR3 playerRot = pPlayer->GetRot();
	D3DXVECTOR3 enemyPos = GetPos();
	D3DXVECTOR3 enemyRot = GetRot();

	ToggleShooting(false);
	switch (m_act)
	{
	case ENEMY_ACTION_NONE:
		// �������Ȃ�
		break;

	case ENEMY_ACTION_FOLLOW:
		// �v���C���[��ǔ�����A�����͕ς��Ȃ�
		m_move.x = sinf(atan2f(playerPos.x - enemyPos.x, playerPos.y - enemyPos.y));
		m_move.y = cosf(atan2f(playerPos.x - enemyPos.x, playerPos.y - enemyPos.y));
		m_move = m_move * m_fSpeed;
		
		break;

	case ENEMY_ACTION_FOLLOW_FACEPC:
		// �v���C���[��ǔ�����A�v���C���[�̕�������
		this->FacePlayer();
		m_move.x = sinf(enemyRot.z + D3DX_PI);
		m_move.y = cosf(enemyRot.z + D3DX_PI);
		m_move = m_move * PLAYER_SPEED;
		break;

	case ENEMY_ACTION_FOLLOW_SHOOT:
		// �v���C���[��ǔ��A�ˌ�����B �����͕ς��Ȃ�
		m_move.x = sinf(atan2f(playerPos.x - enemyPos.x, playerPos.y - enemyPos.y));
		m_move.y = cosf(atan2f(playerPos.x - enemyPos.x, playerPos.y - enemyPos.y));
		m_move = m_move * m_fSpeed;

		break;

	case ENEMY_ACTION_FOLLOW_SHOOT_FACEPC:
		// �v���C���[��ǔ��A�ˌ�����B �v���C���[�̕�������
		this->FacePlayer();
		this->ToggleShooting(true);
		m_move.x = sinf(atan2f(playerPos.x - enemyPos.x, playerPos.y - enemyPos.y));
		m_move.y = cosf(atan2f(playerPos.x - enemyPos.x, playerPos.y - enemyPos.y));
		m_move = m_move * m_fSpeed;

		break;

	case ENEMY_ACTION_SHOOT:
		// �v���C���[���ˌ�����B �����͕ς��Ȃ�
		this->ToggleShooting(true);

		break;

	case ENEMY_ACTION_SHOOT_FACEPC:
		// �v���C���[���ˌ�����B �v���C���[�̕�������
		this->FacePlayer();
		this->ToggleShooting(true);

		break;

	default:
		break;
	}
}

//=============================================================================
// �L�����̉�]���Ȃ߂炩�ɂ���֐�
//=============================================================================
void CEnemy::SmoothTurn(void)
{
	D3DXVECTOR3 RotDiff;				// �ڕW�l�Ǝ��ۂ̉�]�̍���
	D3DXVECTOR3 EnemyRot = GetRot();	// ���ۂ̉�]�l

	//�����v�Z
	RotDiff.z = m_rotDest.z - EnemyRot.z;

	//������D3DX_PI�ȏ�(�����ȏ�)�̏ꍇ�A�t��]
	if (RotDiff.z > D3DX_PI)
	{
		EnemyRot.z -= ((D3DX_PI * 2) - RotDiff.z) * ENEMY_SMOOTHTURN_VALUE;
	}
	else if (RotDiff.z < -D3DX_PI)
	{
		EnemyRot.z += ((D3DX_PI * 2) + RotDiff.z) * ENEMY_SMOOTHTURN_VALUE;
	}
	else
	{
		EnemyRot.z += RotDiff.z * ENEMY_SMOOTHTURN_VALUE;
	}

	// ��]�̏C�� (3.14��������}�t��)
	if (EnemyRot.z > D3DX_PI)
	{
		EnemyRot.z -= D3DX_PI * 2.0f;
	}
	else if (EnemyRot.z < -D3DX_PI)
	{
		EnemyRot.z += D3DX_PI * 2.0f;
	}
	if (m_rotDest.z > D3DX_PI)
	{
		m_rotDest.z -= D3DX_PI * 2.0f;
	}
	else if (m_rotDest.z < -D3DX_PI)
	{
		m_rotDest.z += D3DX_PI * 2.0f;
	}

	// �ύX������]�𔽉f������
	SetRot(EnemyRot);
}

//=============================================================================
// �v���C���[�̕����Ɍ�����ς���֐�
//=============================================================================
void CEnemy::FacePlayer(void)
{
	D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();
	D3DXVECTOR3 enemyPos = GetPos();
	m_rotDest.z = D3DX_PI + atan2f(playerPos.x - enemyPos.x, playerPos.y - enemyPos.y);
}

//=============================================================================
// �v���C���[�ɑ΂��Ďˌ�����֐�
//=============================================================================
void CEnemy::ShootPlayer(void)
{
	// �ˌ��g�O�����I���̏ꍇ
	if (m_bToggleShooting == true)
	{
		m_nShootCounter++;	// �ˌ��J�E���g�A�b�v

		// �J�E���g���w�肳�ꂽ�t���[�����𒴂�����
		if (m_nShootCounter >= m_nShootSpeed)
		{
			// �v���C���[�ɑ΂��Ďˌ�������
			D3DXVECTOR3 vecToPlayer = CManager::GetVectorFromPointAtoB(GetPos(), CGame::GetPlayer()->GetPos());
			D3DXVECTOR3 enemyPos = GetPos();
			D3DXVECTOR3 enemyRot = GetRot();
			D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPos();
			vecToPlayer = vecToPlayer * 5;

			CBullet::Create(GetPos(), D3DXVECTOR3(0.0f, 0.0f, atan2f(playerPos.x - enemyPos.x, playerPos.y - enemyPos.y) - D3DX_PI / 2), vecToPlayer, GetObjType());
			m_nShootCounter = 0;
			m_nShootSpeed = ENEMY_SHOOT_SPEED;
		}
	}
	else
	{
		m_nShootCounter = 0;
	}
}

//=============================================================================
// �����蔻�莞�̃N�[���^�C��
//=============================================================================
void CEnemy::CollisionCooltime(void)
{
	if (m_bHit)
	{
		m_nHitCounter++;
		if (m_nHitCounter > ENEMY_COLLISION_COOLTIME)
		{
			m_bHit = false;
			m_nHitCounter = 0;
		}
	}
}

//=============================================================================
// ����̕����ɉ���������֐�
//=============================================================================
void CEnemy::AddForce(D3DXVECTOR3 vec, float fValue)
{
	m_move = m_move + vec * fValue;
}

//=============================================================================
// �O�Ղ̏���
//=============================================================================
void CEnemy::Trail(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	static int nTickTrail;
	nTickTrail++;
	if (nTickTrail >= ENEMY_TRAIL_TICK) {
		D3DXVECTOR3 trailPos = D3DXVECTOR3(pos.x + sinf(rot.z) * PLAYER_TRAIL_OFFSET, pos.y + cosf(rot.z) * PLAYER_TRAIL_OFFSET, 0.0f);
		CEffect::Create(trailPos, rot, ENEMY_TRAIL_SIZE, ENEMY_TRAIL_COLOR, CEffect::EFFECT_TEX_DEFAULT, PLAYER_TRAIL_SHRINKSPEED, PLAYER_TRAIL_FADESPEED);

		nTickTrail = 0;
	}
}

//=============================================================================
// ���S������
//=============================================================================
void CEnemy::Dead(void)
{
	CSpriteSheet::Create(this->GetPos(), VECTOR3_ZERO, D3DXVECTOR2(EXPLOSION_SIZE_DEFAULT, EXPLOSION_SIZE_DEFAULT), CSpriteSheet::SPRITESHEET_EXPLOSION);	// ���j
	CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_EXPLOSION00);	// ��
	CGame::GetWave()->EnemyDied(); // �E�F�[�u���̓G����
	COrb::Create(GetPos(), VECTOR3_ZERO, ORB_SIZE_DEFAULT, 33);
	CParticle::Create(	// �p�[�e�B�N��
		GetPos(),
		VECTOR3_ZERO,
		D3DXCOLOR(1.0f, 1.0f, 0.4f, 1.0f),
		D3DXVECTOR2(35, 35),
		CEffect::EFFECT_TEX_DEFAULT,
		1,
		4.0f,
		35,
		0.001f,
		0.001f,
		0.0f,
		0.0f);

	Uninit();
}

//=============================================================================
// ��ʓ������肵�ĕ`��
//=============================================================================
void CEnemy::CheckScreen(void)
{
	// ��ʊO�̂��`�悵�Ȃ�
	D3DXVECTOR3 screenPos = CCamera::WorldToScreenPosition(GetPos());
	D3DXVECTOR2 size = GetSize();
	if (screenPos.x > SCREEN_WIDTH + size.x ||
		screenPos.x < 0 - size.x ||
		screenPos.y > SCREEN_HEIGHT + size.y ||
		screenPos.y < 0 - size.y)
		this->SetDrawFlag(false);
	else {
		this->SetDrawFlag(true);
	}
}

//=============================================================================
// �Փˏ���
//=============================================================================
void CEnemy::CollideEnemy(CEnemy *pEnemy1, CEnemy *pEnemy2)
{
	// ���݂����m�b�N�o�b�N������
	pEnemy1->AddForce(CManager::GetVectorFromPointAtoB(pEnemy2->GetPos(), pEnemy1->GetPos()), 1.0f);
	pEnemy2->AddForce(CManager::GetVectorFromPointAtoB(pEnemy1->GetPos(), pEnemy2->GetPos()), 1.0f);
}

//=============================================================================
// �F�𒼂�
//=============================================================================
void CEnemy::FixColor(void)
{
	if (m_fColorGradient < 1.0f) {
		m_fColorGradient += 0.1f;
	}
	else if (m_fColorGradient >= 1.0f) {
		m_fColorGradient = 1.0f;
	}
	SetColor(D3DXCOLOR(1.0f, m_fColorGradient, m_fColorGradient, 1.0f));
}