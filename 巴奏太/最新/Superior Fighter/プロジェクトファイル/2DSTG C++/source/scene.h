//=============================================================================
//
// オブジェクトクラス [scene.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

//*****************************************************************************
// ヘッダーのインクルード
//*****************************************************************************
#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCENE_PRIORITY_MAX (6)
#define SCENE_PRIORITY_DEFAULT (3)

//*****************************************************************************
// オブジェクトクラス (抽象)
//*****************************************************************************
class CScene
{
public:
	// オブジェクトの種類
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
	virtual void SetPos(D3DXVECTOR3 pos) {};
	virtual void SetRot(D3DXVECTOR3 rot) {};
	virtual void SetSize(D3DXVECTOR2 size) {};
	void SetObjType(OBJTYPE type);
	void SetPriority(int nPriority);

	// GET
	virtual D3DXVECTOR3 GetPos(void) { return VECTOR3_ZERO; }
	virtual D3DXVECTOR3 GetRot(void) { return VECTOR3_ZERO; }
	virtual D3DXVECTOR2 GetSize(void) { return D3DXVECTOR2(0, 0); }
	OBJTYPE GetObjType(void);
	int GetPriority(void) { return m_nPriority; }

	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static vector<CScene*> *CScene::GetSceneList(int nPriority);

protected:
	void Release(void);		// Scene2DのUninitから呼ぶからprotected

private:
	int m_nPriority;						// 優先順位の番号
	OBJTYPE m_objType;						// オブジェクトの種類

	static vector<CScene*> m_aSceneVc[SCENE_PRIORITY_MAX];	// シーンのリスト
};

#endif
