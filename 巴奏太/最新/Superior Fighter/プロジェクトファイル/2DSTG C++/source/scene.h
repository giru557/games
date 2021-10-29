//=============================================================================
//
// �I�u�W�F�N�g�N���X [scene.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

//*****************************************************************************
// �w�b�_�[�̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SCENE_PRIORITY_MAX (6)
#define SCENE_PRIORITY_DEFAULT (3)

//*****************************************************************************
// �I�u�W�F�N�g�N���X (����)
//*****************************************************************************
class CScene
{
public:
	// �I�u�W�F�N�g�̎��
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_PLAYER,
		OBJTYPE_ENEMY,
		OBJTYPE_BOSS,
		OBJTYPE_BOSSCORE,
		OBJTYPE_BULLET,
		OBJTYPE_BEAM,
		OBJTYPE_BEAMBASE,
		OBJTYPE_ORB,
		OBJTYPE_DEBRIS,

		OBJTYPE_MAX
	} OBJTYPE;

	CScene(int nPriority = SCENE_PRIORITY_DEFAULT);
	virtual ~CScene();

	virtual HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	// SET
	virtual void SetPos(D3DXVECTOR3 pos) = 0;
	virtual void SetRot(D3DXVECTOR3 rot) = 0;
	virtual void SetSize(D3DXVECTOR2 size) = 0;
	void SetObjType(OBJTYPE type);
	void SetPriority(int nPriority);

	// GET
	virtual D3DXVECTOR3 GetPos(void) = 0;
	virtual D3DXVECTOR3 GetRot(void) = 0;
	virtual D3DXVECTOR2 GetSize(void) = 0;
	OBJTYPE GetObjType(void);
	int GetPriority(void) { return m_nPriority; }

	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static vector<CScene*> *CScene::GetSceneList(int nPriority);

protected:
	void Release(void);		// Scene2D��Uninit����ĂԂ���protected

private:
	int m_nPriority;						// �D�揇�ʂ̔ԍ�
	OBJTYPE m_objType;						// �I�u�W�F�N�g�̎��

	static vector<CScene*> m_aSceneVc[SCENE_PRIORITY_MAX];	// �V�[���̃��X�g
	static int m_nNumAll;									// �S�̂̐�����
};

#endif
