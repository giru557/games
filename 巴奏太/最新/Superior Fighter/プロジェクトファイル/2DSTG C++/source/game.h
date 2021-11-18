//=============================================================================
//
// ゲーム画面 [game.h]
// Author: Sota Tomoe
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************************************
// ヘッダーのインクルード
//*****************************************************************************
#include "main.h"
#include "scene.h"
#include "enemy.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define GAME_FIELD_WIDTH (5000)		// フィールドの幅
#define GAME_FIELD_HEIGHT (5000)	// フィールドの高さ

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPlayer;
class CBackground;
class CEnemy;
class CScore;
class CLife;
class CCrosshair;
class CWave;
class CHelp;
class CMiniMap;
class CEnergy;
class CBossManager;
class CBackground3D;

//*****************************************************************************
// ゲーム画面クラス ( 継承元: オブジェクトクラス [CScene] )
//*****************************************************************************
class CGame : public CScene
{
public:
	// ゲーム進行状態
	typedef enum
	{
		GAMESTATUS_NONE = 0,		// 状態なし
		GAMESTATUS_INPROGRESS,		// ゲーム進行中
		GAMESTATUS_OVER,			// ゲームオーバー
		GAMESTATUS_CLEAR,			// ゲームクリア
		GAMESTATUS_MAX,
	} GAMESTATUS;

	CGame();
	~CGame();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPos(D3DXVECTOR3 pos) {};
	void SetRot(D3DXVECTOR3 rot) {};
	void SetSize(D3DXVECTOR2 size) {};
	D3DXVECTOR3 GetPos(void) { return VECTOR3_ZERO; }
	D3DXVECTOR3 GetRot(void) { return VECTOR3_ZERO; }
	D3DXVECTOR2 GetSize(void) { return (D3DXVECTOR2)VECTOR3_ZERO; }

	static void SetGameStatus(GAMESTATUS status) { m_status = status; }
	static GAMESTATUS GetGameStatus(void) { return m_status; }
	static CPlayer *GetPlayer(void) { return m_pPlayer; }
	static CBackground *GetBG(void) { return m_pBG; }
	static CScore *GetScore(void) { return m_pScore; }
	static CLife *GetLife(void) { return m_pLife; }
	static CEnemy **GetEnemy(void) { return m_apEnemy; }
	static CCrosshair *GetCrosshair(void) { return m_pCrosshair; }
	static CWave *GetWave(void) { return m_pWave; }
	static CHelp *GetHelp(void) { return m_pHelp; }
	static CMiniMap *GetMap(void) { return m_pMap; }
	static CEnergy *GetEnergy(void) { return m_pEnergy; }
	static CBossManager *GetBoss(void) { return m_pBoss; }
	static CBackground3D *GetBG3D(void) { return m_pBG3D; }

private:
	static GAMESTATUS m_status;
	static CPlayer *m_pPlayer;
	static CBackground *m_pBG;
	static CEnemy *m_apEnemy[ENEMY_MAX];
	static CScore *m_pScore;
	static CLife *m_pLife;
	static CCrosshair *m_pCrosshair;
	static CWave *m_pWave;
	static CHelp *m_pHelp;
	static CMiniMap *m_pMap;
	static CEnergy *m_pEnergy;
	static CBossManager *m_pBoss;
	static CBackground3D *m_pBG3D;
};
#endif

