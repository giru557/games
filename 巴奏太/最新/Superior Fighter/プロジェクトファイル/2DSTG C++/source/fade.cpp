//=============================================================================
//
// フェード処理 [fade.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "fade.h"
#include "manager.h"

//*****************************************************************************
// フェードクラス
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CFade::CFade()
{
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pVertex = NULL;
	m_fAlpha = 1.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CFade::~CFade()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CFade::Init(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(CScene2D::VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL)))
	{
		return E_FAIL;
	}

	// 頂点バッファをロックしてポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVertex, 0);

	// 頂点情報を設定
	// 位置
	m_pVertex[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0);
	m_pVertex[1].pos = D3DXVECTOR3(0, 0, 0);
	m_pVertex[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	m_pVertex[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0);

	// rhw
	m_pVertex[0].rhw = 1.0f;
	m_pVertex[1].rhw = 1.0f;
	m_pVertex[2].rhw = 1.0f;
	m_pVertex[3].rhw = 1.0f;

	// 色
	m_pVertex[0].col = D3DXCOLOR(0, 1.0f, 0, m_fAlpha);
	m_pVertex[1].col = D3DXCOLOR(0, 1.0f, 0, m_fAlpha);
	m_pVertex[2].col = D3DXCOLOR(0, 1.0f, 0, m_fAlpha);
	m_pVertex[3].col = D3DXCOLOR(0, 1.0f, 0, m_fAlpha);

	// テクスチャ
	m_pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	m_pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	m_pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	m_pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// 頂点バッファアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CFade::Uninit(void)
{
	// 頂点バッファを開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// テクスチャ開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// 更新
//=============================================================================
void CFade::Update(void)
{
	// 透明度と状態の管理
	switch (m_fadeState)
	{
	case CFade::FADE_IN:
		m_fAlpha -= FADE_SPEED;
		if (m_fAlpha < 0.0f)
		{
			m_fAlpha = 0.0f;
			m_fadeState = CFade::FADE_NONE;
		}
		break;

	case CFade::FADE_OUT:
		m_fAlpha += FADE_SPEED;
		if (m_fAlpha > 1.0f)
		{
			m_fAlpha = 1.0f;
			m_fadeState = CFade::FADE_IN;
			CManager::SetMode(m_modeNext);
		}
		break;

	default:
		break;
	}

	// カラーの更新
	// 頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVertex, 0);

	//頂点カラーの設定
	m_pVertex[0].col = D3DXCOLOR(0, 0, 0, m_fAlpha);
	m_pVertex[1].col = D3DXCOLOR(0, 0, 0, m_fAlpha);
	m_pVertex[2].col = D3DXCOLOR(0, 0, 0, m_fAlpha);
	m_pVertex[3].col = D3DXCOLOR(0, 0, 0, m_fAlpha);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 描画
//=============================================================================
void CFade::Draw(void)
{
	if (m_fadeState != CFade::FADE_NONE)
	{
		// デバイス取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(CScene2D::VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// ポリゴンの描画
		pDevice->SetTexture(0, m_pTexture);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//=============================================================================
// フェードイン
//=============================================================================
void CFade::FadeIN(void)
{
	if (m_fadeState != FADE_IN)
	{
		m_fAlpha = 1.0f;
		m_fadeState = CFade::FADE_IN;
	}
}

//=============================================================================
// フェードアウト
//=============================================================================
void CFade::FadeOUT(CManager::MODE modeNext)
{
	if (m_fadeState != FADE_OUT)
	{
		m_fAlpha = 0.0f;
		m_fadeState = CFade::FADE_OUT;
		m_modeNext = modeNext;
	}
}