//=============================================================================
//
// �v���C���[ [player.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "manager.h"
#include "bullet.h"
#include "camera.h"
#include "effect.h"
#include "game.h"
#include "spritesheet.h"
#include "life.h"
#include "textureloader.h"
#include "debugger.h"
#include "sound.h"
#include "energy.h"
#include "particle.h"
#include "bg3d.h"

//*****************************************************************************
// �v���C���[�N���X ( �p����: 2D�|���S�� [scene2D] )
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPlayer::CPlayer(int nPriority) : CScene2D(nPriority)
{
	// �N���A
	m_move = VECTOR3_ZERO;
	m_rotDest = VECTOR3_ZERO;
	m_bPowered = false;
	m_bToggleShoot = false;
	m_bToggleShoot4 = false;
	m_nShootCounter = 0;
	m_nShootCounter4 = 0;
	m_nShootSpeed = PLAYER_SHOOT_SPEED;
	m_nShootSpeed4 = PLAYER_SHOOT_SPEED4;
	m_fSpeedDecay = PLAYER_SPEED_DECAY;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// ����������
	if (FAILED(CScene2D::Init(pos, rot, size)))
	{
		E_FAIL;
	}
	
	// �e�N�X�`����ݒ�
	CScene2D::BindTexture(CManager::GetTextureLoad()->m_TextureMp["PLAYER"]);

	// �I�u�W�F�N�g�̎�ނ�ݒ�
	SetObjType(OBJTYPE_PLAYER);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPlayer::Uninit(void)
{
	CScene2D::Uninit();
}
//=============================================================================
// �X�V����
//=============================================================================
void CPlayer::Update(void)
{
	CScene2D::Update();

	// �K�v�ȕ����擾
	CInputMouse *pMouse = CManager::GetInputMouse();
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR2 cursorPos = pMouse->GetPos();
	D3DXVECTOR2 playerPosScreen = CCamera::WorldToScreenPosition(pos);

		// �ʒu�̍X�V�ƌ����̐ݒ�
		m_move.x += (0 - m_move.x) * m_fSpeedDecay;
		m_move.y += (0 - m_move.y) * m_fSpeedDecay;
		pos += m_move;

		// ��ԁA�t���O���Ǘ�����
		this->StateManage();

		// ���͂�����ꍇ
		if (m_bPowered) {
			CPlayer::Move();	// �v���C���[�̐��i
			this->Trail();		// �O�Ղ̐���
		}

		// �ˌ�
		if (m_bToggleShoot)
			this->FullAuto();
		else
			m_nShootCounter = 0;
		if (m_bToggleShoot4)
			this->FullAuto4();
		else
			m_nShootCounter4 = 0;

		// �_�b�V��
		if (m_bDash)
			this->Dash();

		// �ʒu�ݒ�
		CScene2D::SetPos(pos);

		// �����ݒ�
		m_rotDest.z = D3DX_PI + atan2f(cursorPos.x - playerPosScreen.x, cursorPos.y - playerPosScreen.y);

		// ��]�����炩��
		CPlayer::SmoothTurn();

		// �J�����ʒu�̐ݒ�
		CCamera *pCamera = CManager::GetCamera();
		pCamera->SetCameraCenter(GetPos());

		// ���S�m�F
		if (CGame::GetLife()->GetLife() <= 0) {
			this->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
			CSpriteSheet::Create(pos, VECTOR3_ZERO, D3DXVECTOR2(300.0f, 300.0f), CSpriteSheet::SPRITESHEET_EXPLOSION2);
			CManager::GetCamera()->Shake(5.0f, 60);
			Uninit();
		}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPlayer::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// �����֐�
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos)
{
	// �v���C���[�̐����A������
	CPlayer *pPlayer = NULL;
	pPlayer = new CPlayer;
	if (pPlayer != NULL)
	{
		pPlayer->Init(pos, D3DXVECTOR3(0, 0, 0), D3DXVECTOR2(SCENE2D_WIDTH_DEFAULT, SCENE2D_HEIGHT_DEFAULT));
	}
	
	return pPlayer;
}

CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// �v���C���[�̐����A������
	CPlayer *pPlayer = NULL;
	pPlayer = new CPlayer;
	if (pPlayer != NULL)
	{
		pPlayer->Init(pos, rot, size);
	}

	return pPlayer;
}

//=============================================================================
// �ړ��֐�
//=============================================================================
void CPlayer::Move()
{
	// �v���C���[�̌����Ɉړ�����
	D3DXVECTOR3 rotPlayer = GetRot();
	D3DXVECTOR3 move;
	move.x = sinf(rotPlayer.z + D3DX_PI);
	move.y = cosf(rotPlayer.z + D3DX_PI);
	m_move += move * PLAYER_SPEED;
}

//=============================================================================
// �L�����̉�]���Ȃ߂炩�ɂ���֐�
//=============================================================================
void CPlayer::SmoothTurn(void)
{
	D3DXVECTOR3 RotDiff;				// �ڕW�l�Ǝ��ۂ̉�]�̍���
	D3DXVECTOR3 PlayerRot = GetRot();	// ���ۂ̉�]�l
	float fTurnValue = PLAYER_SMOOTHTURN_VALUE;
	if (m_bDash) fTurnValue = PLAYER_SMOOTHTURN_VALUE_DASH;

	//�����v�Z
	RotDiff.z = m_rotDest.z - PlayerRot.z;

	//������D3DX_PI�ȏ�(�����ȏ�)�̏ꍇ�A�t��]
	if (RotDiff.z > D3DX_PI)
		PlayerRot.z -= ((D3DX_PI * 2) - RotDiff.z) * fTurnValue;
	else if (RotDiff.z < -D3DX_PI)
		PlayerRot.z += ((D3DX_PI * 2) + RotDiff.z) * fTurnValue;
	else
		PlayerRot.z += RotDiff.z * fTurnValue;

	// ��]�̏C��
	if (PlayerRot.z > D3DX_PI)
		PlayerRot.z -= D3DX_PI * 2.0f;
	else if (PlayerRot.z < -D3DX_PI)
		PlayerRot.z += D3DX_PI * 2.0f;
	if (m_rotDest.z > D3DX_PI)
		m_rotDest.z -= D3DX_PI * 2.0f;
	else if (m_rotDest.z < -D3DX_PI)
		m_rotDest.z += D3DX_PI * 2.0f;

	// �ύX������]�𔽉f������
	SetRot(PlayerRot);
}

//=============================================================================
// �O�Ղ̏���
//=============================================================================
void CPlayer::Trail(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	D3DXVECTOR3 trailPos = D3DXVECTOR3(pos.x + sinf(rot.z) * PLAYER_TRAIL_OFFSET, pos.y + cosf(rot.z) * PLAYER_TRAIL_OFFSET, 0.0f);
	if (m_bDash) {
		// �_�b�V�����͋O�Ղ�傫���Ԃ�����
		CEffect::Create(trailPos, rot, PLAYER_TRAIL_SIZE_DASH, PLAYER_TRAIL_COLOR_DASH, CEffect::EFFECT_TEX_DEFAULT, PLAYER_TRAIL_SHRINKSPEED, PLAYER_TRAIL_FADESPEED);
	}
	else {
		// �ʏ펞�͐�
		CEffect::Create(trailPos, rot, PLAYER_TRAIL_SIZE, PLAYER_TRAIL_COLOR, CEffect::EFFECT_TEX_DEFAULT, PLAYER_TRAIL_SHRINKSPEED, PLAYER_TRAIL_FADESPEED);
	}
	
}

//=============================================================================
// ����̕����ɉ���������֐�
//=============================================================================
void CPlayer::AddForce(D3DXVECTOR3 vec, float fValue)
{
	m_move += vec * fValue;
}

//=============================================================================
// �e�̔���
//=============================================================================
void CPlayer::Shoot(int nType)
{
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 pos = GetPos();

	if (nType == 0) {
		// �v���C���[�̌����ɒe�𔭎�
		D3DXVECTOR3 bulletMove;

		bulletMove.x = sinf(rot.z + D3DX_PI);
		bulletMove.y = cosf(rot.z + D3DX_PI);
		bulletMove = bulletMove * BULLET_SPEED_DEFAULT;

		// �C���X�^���X����
		CBullet::Create(pos, D3DXVECTOR3(0.0f, 0.0f, rot.z + D3DX_PI / 2), bulletMove, CScene::OBJTYPE_PLAYER);
	}
	else if (nType == 1) {
		// 4�����̊p�x
		float fRadAngBullet[4];
		fRadAngBullet[0] = rot.z + D3DXToRadian(45.0f);
		fRadAngBullet[1] = rot.z + D3DXToRadian(-45.0f);
		fRadAngBullet[2] = rot.z + D3DXToRadian(45.0f) + D3DX_PI;
		fRadAngBullet[3] = rot.z + D3DXToRadian(-45.0f) + D3DX_PI;

		for (int nCntBullet = 0; nCntBullet < 4; nCntBullet++) {
			// �p�x�����ɔ���
			D3DXVECTOR3 moveBullet = D3DXVECTOR3(sinf(fRadAngBullet[nCntBullet]), cosf(fRadAngBullet[nCntBullet]), 0) * BULLET_SPEED_DEFAULT;
			CBullet::Create(pos, D3DXVECTOR3(0.0f, 0.0f, fRadAngBullet[nCntBullet] - D3DX_PI / 2), moveBullet, OBJTYPE_PLAYER);
		}
	}

}

//=============================================================================
// �A�˗p�֐�
//=============================================================================
void CPlayer::FullAuto(void)
{
	// �J�E���g���܂����甭��
	m_nShootCounter++;
	if (m_nShootCounter > m_nShootSpeed)
	{
		// �G�l���M�[������Ȃ��ꍇ���ĂȂ�
		if (CGame::GetEnergy()->UseEnergy(2)) {
			this->Shoot(0);
			m_nShootCounter = 0;
		}
	}
}

//=============================================================================
// 4�����ˌ��A�˗p
//=============================================================================
void CPlayer::FullAuto4(void)
{
	// �J�E���g���܂����甭��
	m_nShootCounter4++;
	if (m_nShootCounter4 > m_nShootSpeed4) {
		// �G�l���M�[����ĂȂ��ƑłĂȂ�
		if (CGame::GetEnergy()->UseEnergy(10)) {
			this->Shoot(1);

			m_nShootCounter4 = 0;
		}
	}
}

//=============================================================================
// �L�����̃_�b�V��
//=============================================================================
void CPlayer::Dash(void)
{
	D3DXVECTOR3 rot = GetRot();
	
	// �_�b�V�����̃J�E���g�����܂�����_�b�V���t���O��؂�
	static int nDashCounter;
	nDashCounter++;
	if (nDashCounter > PLAYER_DASH_FRAMES) {
		m_bDash = false;
		nDashCounter = 0;
	}

	// �_�b�V�����ɂ��鏈��
	CManager::GetCamera()->Shake(2.0f, PLAYER_DASH_FRAMES);
	D3DXVECTOR3 dirForward = D3DXVECTOR3(sinf(rot.z + D3DX_PI), cosf(rot.z + D3DX_PI), 0.0f);
	AddForce(dirForward, PLAYER_SPEED_DASH);
}

//=============================================================================
// ��ԁA�t���O�̊Ǘ�
//=============================================================================
void CPlayer::StateManage(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CInputMouse *pMouse = CManager::GetInputMouse();

	// Shift��������Ă���ԃG���W����؂�
	m_bPowered = true;
	if (!m_bDash) {
		if (pKeyboard->GetPress(DIK_LSHIFT)) m_bPowered = false;
	}

	// �N���b�N���Ă���ԁA�ˌ��g�O����ON�ɂ���
	m_bToggleShoot = false;
	m_bToggleShoot4 = false;
	if (pMouse->GetPress(CInputMouse::MOUSESTATE_LCLICK)) m_bToggleShoot = true;
	if (pMouse->GetPress(CInputMouse::MOUSESTATE_RCLICK)) m_bToggleShoot4 = true;

	// �X�y�[�X�L�[�Ń_�b�V����L���ɂ���
	if (m_bPowered) {
		if (pKeyboard->GetTrigger(DIK_SPACE)) {
			if (GetDashState() == false) {
				if (CGame::GetEnergy()->UseEnergy(20)) {
					m_bDash = true;

					// �_�b�V�����̌���
					CManager::GetRenderer()->SetFeedbackEffect(PLAYER_DASH_FRAMES);
					CGame::GetBG3D()->SetRippleFrequency(0.07f, PLAYER_DASH_FRAMES);
					CGame::GetBG3D()->SetRippleAmplitude(25.0f, PLAYER_DASH_FRAMES);
					CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_DASH00);
				}
			}
		}
	}

	// �G���W����؂��Ă��鎞�͈ړ����������Ȃ��悤�ɂ���
	if (m_bPowered) m_fSpeedDecay = PLAYER_SPEED_DECAY;
	else m_fSpeedDecay = PLAYER_SPEED_DECAY_OFF;
}

//=============================================================================
// �G�ɏՓˎ�
//=============================================================================
void CPlayer::CollideEnemy(CEnemy *pEnemy)
{
	if (GetDashState() == false) {	// �v���C���[���_�b�V�����łȂ��ꍇ
		AddForce(CManager::GetVectorFromPointAtoB(pEnemy->GetPos(), GetPos()), 5.0f);	// �m�b�N�o�b�N
		if (!pEnemy->GetHit()) {
			// �Փ˃N�[���_�E������������Ă���ꍇ

			CGame::GetLife()->Damage(1);									// �v���C���[�Ƀ_���[�W
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_IMPACT00);	// ���ʉ�
			CGame::GetBG3D()->SetRippleFrequency(0.07f, 1);					// �g��G�t�F�N�g
			CParticle *pParticle = CParticle::Create(						// �p�[�e�B�N��
				GetPos(),
				true,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				D3DXVECTOR2(40, 40),
				CEffect::EFFECT_TEX_PARTS1,
				1,
				7.0f,
				5,
				0.005f,
				0.01f,
				0.0f,
				0.0f);
			pParticle->SetPriority(4);
			pParticle->SetRenderState(D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA);

			pEnemy->SetHit(true);	// �Փ˃N�[���_�E����ݒ�
		}
	}
	else {
		// �v���C���[���_�b�V����

		if (!pEnemy->GetHit()) {
			// �Փ˃N�[���_�E������������Ă���ꍇ

			CParticle::Create(	// �ˌ��p�[�e�B�N��
				GetPos() + D3DXVECTOR3(sinf(GetRot().z + D3DX_PI), cosf(GetRot().z + D3DX_PI), 0) * 50,
				VECTOR3_ZERO,
				D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f),
				D3DXVECTOR2(30, 30),
				CEffect::EFFECT_TEX_DEFAULT,
				1,
				10.0f,
				20,
				0.01f,
				0.01f,
				D3DXToDegree(GetRot().z),
				160);
			pEnemy->Damage(1);				// �G�Ƀ_���[�W

			pEnemy->SetHit(true);	// �Փ˃N�[���_�E���ݒ�
		}
	}
}
