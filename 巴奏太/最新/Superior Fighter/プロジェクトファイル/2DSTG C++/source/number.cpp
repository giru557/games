//=============================================================================
//
// 数字表示処理 [number.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "number.h"
#include "manager.h"
#include "scene2D.h"
#include "textureloader.h"

//*****************************************************************************
// 数字表示クラス ( 基本 )
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CNumber::CNumber()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CNumber::~CNumber()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CNumber::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nInitNum)
{
	m_pos = pos;
	m_size = size;

	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL)))
	{
		return E_FAIL;
	}

	// 対角線の長さ
	m_fLength = sqrtf((m_size.x * m_size.x) + (m_size.y * m_size.y));

	// 対角線の角度
	m_fAngle = atan2f(-m_size.x, m_size.y);

	// 頂点バッファをロックしてポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVertex, 0);

	// 頂点情報を設定
	// 位置
	m_pVertex[0].pos = D3DXVECTOR3(m_pos.x + sinf(m_fAngle + m_rot.z) * m_fLength / 2, m_pos.y + cosf(m_fAngle + m_rot.z) * m_fLength / 2, 0);
	m_pVertex[1].pos = D3DXVECTOR3(m_pos.x + sinf((-m_fAngle + D3DX_PI) + m_rot.z) * m_fLength / 2, m_pos.y + cosf((-m_fAngle + D3DX_PI) + m_rot.z) * m_fLength / 2, 0);
	m_pVertex[2].pos = D3DXVECTOR3(m_pos.x + sinf(-m_fAngle + m_rot.z) * m_fLength / 2, m_pos.y + cosf(-m_fAngle + m_rot.z) * m_fLength / 2, 0);
	m_pVertex[3].pos = D3DXVECTOR3(m_pos.x + sinf(m_fAngle + D3DX_PI + m_rot.z) * m_fLength / 2, m_pos.y + cosf(m_fAngle + D3DX_PI + m_rot.z) * m_fLength / 2, 0);

	// rhw
	m_pVertex[0].rhw = 1.0f;
	m_pVertex[1].rhw = 1.0f;
	m_pVertex[2].rhw = 1.0f;
	m_pVertex[3].rhw = 1.0f;

	// 色
	m_pVertex[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_pVertex[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_pVertex[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_pVertex[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ
	m_pVertex[0].tex = D3DXVECTOR2((float)nInitNum / 10.0f, 1.0f);
	m_pVertex[1].tex = D3DXVECTOR2((float)nInitNum / 10.0f, 0.0f);
	m_pVertex[2].tex = D3DXVECTOR2(((float)nInitNum + 1) / 10.0f, 1.0f);
	m_pVertex[3].tex = D3DXVECTOR2(((float)nInitNum + 1) / 10.0f, 0.0f);

	// 頂点バッファアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CNumber::Uninit(void)
{
	// 頂点バッファ開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CNumber::Update(void)
{
	// 頂点バッファをロックしてポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVertex, 0);

	// 頂点位置
	m_pVertex[0].pos = D3DXVECTOR3(m_pos.x + sinf(m_fAngle + m_rot.z) * m_fLength / 2, m_pos.y + cosf(m_fAngle + m_rot.z) * m_fLength / 2, 0);
	m_pVertex[1].pos = D3DXVECTOR3(m_pos.x + sinf((-m_fAngle + D3DX_PI) + m_rot.z) * m_fLength / 2, m_pos.y + cosf((-m_fAngle + D3DX_PI) + m_rot.z) * m_fLength / 2, 0);
	m_pVertex[2].pos = D3DXVECTOR3(m_pos.x + sinf(-m_fAngle + m_rot.z) * m_fLength / 2, m_pos.y + cosf(-m_fAngle + m_rot.z) * m_fLength / 2, 0);
	m_pVertex[3].pos = D3DXVECTOR3(m_pos.x + sinf(m_fAngle + D3DX_PI + m_rot.z) * m_fLength / 2, m_pos.y + cosf(m_fAngle + D3DX_PI + m_rot.z) * m_fLength / 2, 0);

	// 頂点バッファアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void CNumber::Draw(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	pDevice->SetTexture(0, CManager::GetTextureLoad()->m_TextureMp["NUMBER"]);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// 表示する数字を指定
//=============================================================================
void CNumber::SetNumber(int nNumber)
{
	// 頂点バッファをロックしてポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVertex, 0);

	// テクスチャ
	m_pVertex[0].tex = D3DXVECTOR2((float)nNumber / 10.0f, 1.0f);
	m_pVertex[1].tex = D3DXVECTOR2((float)nNumber / 10.0f, 0.0f);
	m_pVertex[2].tex = D3DXVECTOR2(((float)nNumber + 1) / 10.0f, 1.0f);
	m_pVertex[3].tex = D3DXVECTOR2(((float)nNumber + 1) / 10.0f, 0.0f);

	// 頂点バッファアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 生成関数
//=============================================================================
CNumber *CNumber::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nInitNum)
{
	CNumber *pNumber;
	pNumber = new CNumber;
	if (pNumber != NULL)
	{
		pNumber->Init(pos, size, nInitNum);
	}

	return pNumber;
}