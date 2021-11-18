// LINE TEST
#include "line.h"
#include "manager.h"
#include "input.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CLine::CLine()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CLine::~CLine()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLine::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size)
{
	// 線を生成
	D3DXCreateLine(CManager::GetRenderer()->GetDevice(), &m_pLine);
	m_pLine->SetWidth(m_fWidth);
	SetObjType(OBJTYPE_LINE);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLine::Uninit(void)
{
	// 線を破棄
	if (m_pLine != NULL) {
		m_pLine->Release();
		m_pLine = NULL;
	}

	// 開放
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CLine::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CLine::Draw(void)
{
	if (m_bDraw) {

		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// Device
		D3DXMATRIX mtxView, mtxProj, mtxWV, mtxWVP, mtxRot, mtxTrans;		// 計算用

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		// 向きを反映
		D3DXVECTOR3 rot = GetRot();
		D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		// 位置を反映
		D3DXVECTOR3 pos = GetPos();
		D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		// ビューマトリックスとプロジェクションマトリックスを取得してワールドマトリックスと掛け合わせる
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);
		pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);
		D3DXMatrixMultiply(&mtxWV, &m_mtxWorld, &mtxView);
		D3DXMatrixMultiply(&mtxWVP, &mtxWV, &mtxProj);

		// 掛け合わせたマトリックスを使って描画
		m_pLine->Begin();
		m_pLine->DrawTransform(m_aPoints, 2, &mtxWVP, m_col);
		m_pLine->End();

	}
}

//=============================================================================
// 生成関数
//=============================================================================
CLine *CLine::Create(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float fWidth, D3DXCOLOR col)
{
	CLine *pLine;
	pLine = new CLine;
	if (pLine != NULL)
	{
		pLine->m_aPoints[0] = pos1;
		pLine->m_aPoints[1] = pos2;
		pLine->m_col = col;
		pLine->m_fWidth = fWidth;
		pLine->Init(VECTOR3_ZERO, VECTOR3_ZERO, VECTOR3_ZERO);
	}

	return pLine;
}