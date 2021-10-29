//=============================================================================
//
// �e�̏��� [bullet.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "bullet.h"
#include "manager.h"
#include "spritesheet.h"
#include "enemy.h"
#include "life.h"
#include "score.h"
#include "game.h"
#include "textureloader.h"
#include "sound.h"
#include "particle.h"
#include "effect.h"
#include "boss_turret.h"

//*****************************************************************************
// �o���b�g�N���X ( �p����: 2D�|���S�� [scene2D] )
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBullet::CBullet()
{
	m_move = VECTOR3_ZERO;
	m_nLife = BULLET_LIFE_DEFAULT;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBullet::~CBullet()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBullet::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size, D3DXVECTOR3 move, CScene::OBJTYPE owner)
{
	// ����������
	if (FAILED(CScene2D::Init(pos, rot, size)))
	{
		E_FAIL;
	}

	// �I�u�W�F�N�g�̎�ނ�ݒ�
	// �e�N�X�`���̐ݒ�
	// �ړ��l�̐ݒ�
	// ���L�҂̐ݒ�
	SetObjType(OBJTYPE_BULLET);
	BindTexture(CManager::GetTextureLoad()->m_TextureMp["BULLET"]);
	m_move = move;
	m_owner = owner;
	if (m_owner == OBJTYPE_PLAYER) {
		m_nLife = BULLET_LIFE_PC;
	}
	else if (m_owner == OBJTYPE_ENEMY) {
		this->SetColor(D3DXCOLOR(1, 0.3f, 1, 1.0f));
	}
	else if (m_owner == OBJTYPE_BOSS) {
		this->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}

	CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_LASER00);
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBullet::Uninit(void)
{
	CScene2D::Uninit();
}
//=============================================================================
// �X�V����
//=============================================================================
void CBullet::Update(void)
{
	CScene2D::Update();

	// ���C�t����
	m_nLife--;

	// �ʒu�擾
	D3DXVECTOR3 pos = GetPos();

	// �ړ��ʂ𑫂�
	pos += m_move;

	// �ʒu���X�V
	SetPos(pos);

	// ���_���X�V
	CScene2D::Update();

	// ���C�t0�̎�
	if (m_nLife <= 0)
	{
		Uninit();
		return;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CBullet::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// �����֐�
//=============================================================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size, D3DXVECTOR3 move, CScene::OBJTYPE owner)
{
	// �v���C���[�̐����A������
	CBullet *pBullet = NULL;
	pBullet = new CBullet;
	if (pBullet != NULL)
	{
		pBullet->Init(pos, rot, size, move, owner);
	}

	return pBullet;
}
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, CScene::OBJTYPE owner)
{
	// �v���C���[�̐����A������
	CBullet *pBullet = NULL;
	pBullet = new CBullet;
	if (pBullet != NULL)
	{
		pBullet->Init(pos, rot, D3DXVECTOR2(BULLET_WIDTH_DEFAULT, BULLET_HEIGHT_DEFAULT), move, owner);
	}

	return pBullet;
}

//=============================================================================
// �v���C���[�ɓ�����������
//=============================================================================
void CBullet::CollidePlayer(void)
{
	CGame::GetLife()->Damage(1);								// �_���[�W
	CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_HIT00);	// ����
	CParticle::Create(											// �p�[�e�B�N��
		GetPos(),
		VECTOR3_ZERO,
		D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f),
		D3DXVECTOR2(25, 25),
		CEffect::EFFECT_TEX_DEFAULT,
		1,
		5.0f,
		35,
		0.007f,
		0.007f,
		D3DXToDegree(GetRot().z - D3DX_PI / 2),
		90.0f);

	Uninit();	// �e�j��
}

//=============================================================================
// �G�ɓ�����������
//=============================================================================
void CBullet::CollideEnemy(CEnemy *pEnemy)
{
	CGame::GetScore()->AddScore(100);							// �X�R�A���Z
	pEnemy->Damage(1);	// �G�Ƀ_���[�W
	CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_HIT03);	// ���Đ�
	CParticle::Create(											// �p�[�e�B�N��
		GetPos(),
		VECTOR3_ZERO,
		D3DXCOLOR(0.5f, 0.7f, 1.0f, 1.0f),
		D3DXVECTOR2(25, 25),
		CEffect::EFFECT_TEX_DEFAULT,
		1,
		5.0f,
		20,
		0.01f,
		0.01f,
		D3DXToDegree(GetRot().z - D3DX_PI / 2),
		45.0f);

	Uninit();	// �e�j��
}