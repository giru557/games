//=============================================================================
//
// �v���C���[ [player.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************************************
// �w�b�_�[�̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "scene2D.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CEnemy;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PLAYER_WIDTH (75.0f)
#define PLAYER_HEIGHT (75.0f)
#define PLAYER_DASH_FRAMES (50)
#define PLAYER_DRAW_PRIORITY (3)
#define PLAYER_SMOKE_FREQ (60)

#define PLAYER_SHOOT_SPEED (5)
#define PLAYER_SHOOT_SPEED4 (15)

#define PLAYER_SMOOTHTURN_VALUE (0.05f)
#define PLAYER_SMOOTHTURN_VALUE_DASH (0.02f)

#define PLAYER_SPEED (2)
#define PLAYER_SPEED_DASH (3)
#define PLAYER_SPEED_DECAY (0.25f)
#define PLAYER_SPEED_DECAY_OFF (0.01f)

#define PLAYER_TRAIL_SIZE (D3DXVECTOR2(30.0f, 30.0f))
#define PLAYER_TRAIL_SIZE_DASH (D3DXVECTOR2(60.0f, 60.0f))
#define PLAYER_TRAIL_OFFSET (30.0f)
#define PLAYER_TRAIL_COLOR (D3DXCOLOR(0.0f, 0.8f, 0.8f, 1.0f))
#define PLAYER_TRAIL_COLOR_DASH (D3DXCOLOR(1.0f, 0.5f, 0.15f, 1.0f))
#define PLAYER_TRAIL_SHRINKSPEED (0.005f)
#define PLAYER_TRAIL_FADESPEED (0.001f)

//*****************************************************************************
// �v���C���[�N���X ( �p����: 2D�|���S�� [scene2D] )
//*****************************************************************************
class CPlayer : public CScene2D
{
public:
	CPlayer(int nPriority = 4);
	~CPlayer();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Shoot(int nType);
	void AddForce(D3DXVECTOR3 vec, float fValue);
	bool GetDashState(void) { return m_bDash; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void CollideEnemy(CEnemy *pEnemy);

	static CPlayer *Create(D3DXVECTOR3 pos);
	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);

private:
	void StateManage(void);			// ��ԁA�t���O�Ǘ�
	void Dash(void);				// �_�b�V��
	void Move(void);				// �v���C���[�̈ړ�
	void SmoothTurn(void);			// ��]�����炩�ɂ���֐�
	void FullAuto(void);			// �A�˗p�̊֐�
	void FullAuto4(void);			// 4�����ˌ��A�˗p
	void Trail(void);				// �O�Ղ����

	D3DXVECTOR3 m_move;		// �v���C���[�̈ړ��l
	D3DXVECTOR3 m_rotDest;	// �v���C���[�̉�]�ڕW�l
	bool m_bPowered;		// ���i�͂��^�����Ă��邩
	bool m_bToggleShoot;	// �ˌ��̐؂�ւ�
	bool m_bToggleShoot4;	// 4�����ˌ��̐؂�ւ�
	bool m_bDash;			// �_�b�V�����
	int m_nShootSpeed;		// �ˌ��X�s�[�h
	int m_nShootCounter;	// �ˌ��p�J�E���^�[
	int m_nShootSpeed4;		// 4�����ˌ��X�s�[�h
	int m_nShootCounter4;	// 4�����ˌ��J�E���^�[
	float m_fSpeedDecay;	// ���x�����l
};

#endif
