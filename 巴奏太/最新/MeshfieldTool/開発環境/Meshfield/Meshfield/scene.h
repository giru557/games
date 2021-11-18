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
#define SCENE_PRIORITY_MAX (5)
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
		OBJTYPE_LINE,
		OBJTYPE_MAX,
	} OBJTYPE;

	CScene(int nPriority = SCENE_PRIORITY_DEFAULT);
	virtual ~CScene();

	virtual HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	// SET
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetSize(D3DXVECTOR3 size) { m_size = size; }
	void SetObjType(OBJTYPE type) { m_objType = type; }
	void SetPriority(int nPriority);

	// GET
	virtual D3DXVECTOR3 GetPos(void) { return m_pos; }
	virtual D3DXVECTOR3 GetRot(void) { return m_rot; }
	virtual D3DXVECTOR3 GetSize(void) { return m_size; }
	OBJTYPE GetObjType(void) { return m_objType; }
	int GetPriority(void) { return m_nPriority; }

	// Static
	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static vector<CScene*> *CScene::GetSceneList(int nPriority);

protected:
	void Release(void);		// 継承後のUninitから呼ぶからprotected

private:
	D3DXVECTOR3 m_pos;		// Position
	D3DXVECTOR3 m_rot;		// Rotation
	D3DXVECTOR3 m_size;		// Size
	int m_nPriority;		// 優先順位の番号
	OBJTYPE m_objType;		// オブジェクトの種類

	static vector<CScene*> m_aSceneVc[SCENE_PRIORITY_MAX];	// シーンのコンテナ
};

#endif
