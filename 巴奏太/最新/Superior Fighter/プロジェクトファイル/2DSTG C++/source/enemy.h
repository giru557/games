//=============================================================================
//
// �G�N���X [enemy.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************************************
// �w�b�_�[�̃C���N���[�h
//*****************************************************************************
#include "scene2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ENEMY_DRAW_PRIORITY (3)
#define ENEMY_MAX (128 )
#define ENEMY_LIFE_DEFAULT (2)
#define ENEMY_SMOOTHTURN_VALUE (0.02f)

#define ENEMY_TRAIL_TICK (5)
#define ENEMY_TRAIL_SIZE (D3DXVECTOR2(15, 15))
#define ENEMY_TRAIL_COLOR (D3DXCOLOR(1.0f, 0.7f, 0.7f, 1.0f))

#define ENEMY_SPEED_DEFAULT (4)
#define ENEMY_SPEED_DECAY (0.03f)

#define ENEMY_SHOOT_SPEED_RANGE (360)
#define ENEMY_SHOOT_SPEED (rand() % ENEMY_SHOOT_SPEED_RANGE)

#define ENEMY_COLLISION_COOLTIME (60)

//*****************************************************************************
// �G�N���X ( �p����: 2D�|���S�� [scene2d] )
//*****************************************************************************
class CEnemy : public CScene2D
{
public:
	// �G�̎��
	typedef enum
	{
		ENEMY_TYPE_NONE = 0,	// ��ނȂ�
		ENEMY_TYPE_GUNNER,		// �K���i�[�^�C�v
		ENEMY_TYPE_CHARGER,		// �`���[�W���[�^�C�v
		ENEMY_TYPE_MAX			// ��ނ̍ő吔
	} ENEMY_TYPE;

	// �G�̍s���^�C�v
	typedef enum
	{
		ENEMY_ACTION_NONE = 0,				// �s���Ȃ�
		ENEMY_ACTION_FOLLOW,				// �ǔ�
		ENEMY_ACTION_FOLLOW_FACEPC,			// �ǔ��A�v���C���[�̕���������
		ENEMY_ACTION_FOLLOW_SHOOT,			// �ǔ��A�ˌ�
		ENEMY_ACTION_FOLLOW_SHOOT_FACEPC,	// �ǔ��A�ˌ��A�v���C���[�̕���������
		ENEMY_ACTION_SHOOT,					// �ˌ�
		ENEMY_ACTION_SHOOT_FACEPC,			// �ˌ��A�v���C���[�̕���������
		ENEMY_ACTION_MAX,					// �s���̍ő吔
	} ENEMY_ACTION;

	CEnemy();
	~CEnemy();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Damage(int nValue);											// �_���[�W��^����
	void AddForce(D3DXVECTOR3 vec, float fValue);						// ����̕����ɉ���������֐�

	void SetRotDest(D3DXVECTOR3 rotDest) { m_rotDest = rotDest; }		// �ڕW��]�l��ݒ�
	D3DXVECTOR3 GetRotDest(void) { return m_rotDest; }					// �ڕW��]�l���擾
	void SetLife(int nValue) { m_nLife = nValue; }						// �̗͂�ݒ肷��
	int GetLife(void) { return m_nLife; }								// �̗͂��擾
	void SetAction(ENEMY_ACTION enemyAct) { m_act = enemyAct; }			// �s����Ԃ�ݒ�
	ENEMY_ACTION GetAction(void) { return m_act; }						// �s����Ԃ��擾
	bool GetHit(void) { return m_bHit; }								// �q�b�g��Ԃ��擾
	void SetHit(bool bHit) { m_bHit = bHit; }							// �q�b�g��Ԃ�ς���
	void ToggleShooting(bool toggle) { m_bToggleShooting = toggle; }	// �ˌ��̃g�O��
	void SetTexture(ENEMY_TYPE enemyType);								// �e�N�X�`����ݒ肷��

	static void CollideEnemy(CEnemy* pEnemy1, CEnemy* pEnemy2);

protected:
	int m_nLife;			// �̗�
	D3DXVECTOR3 m_rotDest;	// ��]�̖ڕW�l
	bool m_bHit;			// �Փ˃t���O

	void CollisionCooltime(void);	// �����蔻�莞�̃N�[���^�C���J�E���^�[
	void FacePlayer(void);			// �v���C���[�̕�������������֐�
	void SmoothTurn(void);			// ��]�����炩�ɂ���֐�
	void CheckScreen(void);			// ��ʊO����
	void Dead(void);				// ���S������
	void FixColor(void);			// �F�𒼂�

private:
	float m_fSpeed;			// �G�̈ړ����x
	float m_fColorGradient;
	D3DXVECTOR3 m_move;		// �ړ��l,�����x
	int m_nShootSpeed;		// �ˌ��̑���
	int m_nShootCounter;	// �ˌ��p�̃J�E���^�[
	int m_nHitCounter;		// ����p�J�E���^�[
	bool m_bToggleShooting;	// �ˌ����
	ENEMY_TYPE m_enemyType;	// �G�̎��
	ENEMY_ACTION m_act;		// �s��

	void ActionProc(void);			// �s���̓��e�������֐�
	void ShootPlayer(void);			// �v���C���[�ɑ΂��Ďˌ�����֐�
	void Trail(void);				// �O�Ղ̏���
};

#endif
