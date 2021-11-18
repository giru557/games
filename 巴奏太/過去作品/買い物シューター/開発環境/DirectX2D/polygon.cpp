/*#include "polygon.h"

//グローバル変数
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffPolygon = NULL;  //頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTexturePolygon = NULL;		//ポリゴンのテクスチャへのポインタ
int g_nCounterAnim;									//アニメーションカウンター
int g_nPatternAnim;									//アニメーションパターンナンバー
D3DXVECTOR3 g_posPolygon;							//ポリゴンの位置
D3DXVECTOR3 g_rotPolygon;							//ポリゴンの回転角
float g_fLengthPolygon;								//ポリゴンの対角線の長さ
float g_fAnglePolygon;								//ポリゴンの対角線の角度
D3DXVECTOR3 g_movePolygon;							//移動量

//-----------------------------------------------------------------------------
//ポリゴンの初期化処理
//-----------------------------------------------------------------------------
HRESULT InitPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;
	
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\runningman000.png", &g_pTexturePolygon);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPolygon, NULL)))
	{
		return E_FAIL;
	}

	g_nCounterAnim = 0;	//アニメーションカウンター初期化
	g_nPatternAnim = 0;	//アニメーションパターンナンバー初期化

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVertex, 0);

	g_posPolygon = D3DXVECTOR3(640, 400, 0.0f);

	//頂点座標 D3DXVECTOR3(X,Y, 0.0f);
	pVertex[0].pos = D3DXVECTOR3(POLYGON_X_LEFT, POLYGON_Y_BOTTOM, 0.0f);		 //TRIANGLESTRIPで四角
	pVertex[1].pos = D3DXVECTOR3(POLYGON_X_LEFT, POLYGON_Y_UPPER, 0.0f);
	pVertex[2].pos = D3DXVECTOR3(POLYGON_X_RIGHT, POLYGON_Y_BOTTOM, 0.0f);
	pVertex[3].pos = D3DXVECTOR3(POLYGON_X_RIGHT, POLYGON_Y_UPPER, 0.0f);

	//rhwの設定
	pVertex[0].rhw = 1.0f;	//1.0f固定
	pVertex[1].rhw = 1.0f;
	pVertex[2].rhw = 1.0f;
	pVertex[3].rhw = 1.0f;

	//頂点カラーの設定
	pVertex[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);  //D3DCOLOR_RGBA(赤, 緑, 青, 透明度);
	pVertex[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVertex[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVertex[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//UVの設定
	pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVertex[2].tex = D3DXVECTOR2(0.125f, 1.0f);
	pVertex[3].tex = D3DXVECTOR2(0.125f, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffPolygon->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//ポリゴンの終了処理
//-----------------------------------------------------------------------------
void UninitPolygon(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}

	//テクスチャの開放
	if (g_pTexturePolygon != NULL)
	{
		g_pTexturePolygon->Release(); 
		g_pTexturePolygon = NULL;
	}
}

//-----------------------------------------------------------------------------
//ポリゴンの更新処理
//-----------------------------------------------------------------------------
void UpdatePolygon(void)
{
	VERTEX_2D *pVertex;
	g_nCounterAnim++; //アニメーションカウンター更新 (カウントアップ)

	 //頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVertex, 0);

	//ANIMATION_INTERVALの数で割り切れた時
	if (g_nCounterAnim % ANIMATION_INTERVAL == 0)
	{
		g_nPatternAnim = (g_nPatternAnim + 1) % 8; //8までいくとあまり0が代入されてリセット

		//UV設定
		pVertex[0].tex = D3DXVECTOR2((float)g_nPatternAnim * 0.125f, 1.0f);
		pVertex[1].tex = D3DXVECTOR2((float)g_nPatternAnim * 0.125f, 0.0f);
		pVertex[2].tex = D3DXVECTOR2(((float)g_nPatternAnim + 1) * 0.125f, 1.0f);
		pVertex[3].tex = D3DXVECTOR2(((float)g_nPatternAnim + 1) * 0.125f, 0.0f);

	}

	////ポリゴンの回転
	//if (GetKeyboardPress(DIK_F) == true)
	//{
	//	g_rotPlayer.z += 0.1f;
	//	if (g_rotPlayer.z > D3DX_PI)
	//	{
	//		g_rotPlayer.z -= D3DX_PI * 2.0f;
	//	}
	//}

	//キー入力で移動
	//要改良
	if (GetKeyboardPress(DIK_D) == true)
	{
		if (GetKeyboardPress(DIK_W) == true)
		{
			g_movePolygon.x += sinf(D3DX_PI * 0.75f) * 3;
			g_movePolygon.y += cosf(D3DX_PI * 0.75f) * 3;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{
			g_movePolygon.x += sinf(D3DX_PI * 0.25f) * 3;
			g_movePolygon.y += cosf(D3DX_PI * 0.25f) * 3;
		}
		else
		{
			g_movePolygon.x += sinf(D3DX_PI * 0.5f) * 3;
		}
	}
	else if (GetKeyboardPress(DIK_A) == true)
	{
		if (GetKeyboardPress(DIK_W) == true)
		{
			g_movePolygon.x += sinf(-D3DX_PI * 0.75f) * 3;
			g_movePolygon.y += cosf(-D3DX_PI * 0.75f) * 3;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{
			g_movePolygon.x += sinf(-D3DX_PI * 0.25f) * 3;
			g_movePolygon.y += cosf(-D3DX_PI * 0.25f) * 3;
		}
		else
		{
			g_movePolygon.x += sinf(-D3DX_PI * 0.25f) * 3;
		}
	}
	else if (GetKeyboardPress(DIK_W) == true)
	{
		if (GetKeyboardPress(DIK_S) == true)
		{
			g_movePolygon.y += cosf(D3DX_PI) * 0;
		}
		else
		{
			g_movePolygon.y += cosf(D3DX_PI) * 3;
		}
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{
		if (GetKeyboardPress(DIK_W) == true)
		{
			g_movePolygon.y += cosf(D3DX_PI) * 0;
		}
		else
		{
			g_movePolygon.y += cosf(0) * 3;
		}
	}

	if (GetKeyboardPress(DIK_F) == true)
	{
		g_rotPolygon.z += 0.1f;
		if (g_rotPolygon.z > D3DX_PI)
		{
			g_rotPolygon.z -= D3DX_PI * 2.0f;
		}
	}

	//ポリゴンの位置更新
	g_posPolygon.x += g_movePolygon.x;
	g_posPolygon.y += g_movePolygon.y;

	//移動量の減衰
	g_movePolygon.x += (0 - g_movePolygon.x) * 0.2f;
	g_movePolygon.y += (0 - g_movePolygon.y) * 0.2f;

	//対角線の長さ
	g_fLengthPolygon = sqrtf((40) * (40) + (-60) * (-60));

	//対角線の角度
	g_fAnglePolygon = atan2f(80, -120);

	//頂点座標の更新
	pVertex[0].pos.x = g_posPolygon.x + sinf(g_rotPolygon.z + (g_fAnglePolygon + D3DX_PI)) * g_fLengthPolygon;
	pVertex[0].pos.y = g_posPolygon.y + cosf(g_rotPolygon.z + (g_fAnglePolygon + D3DX_PI)) * g_fLengthPolygon;
	pVertex[0].pos.z = 0.0f;

	pVertex[1].pos.x = g_posPolygon.x + sinf(g_rotPolygon.z + (g_fAnglePolygon + D3DX_PI / 3)) * g_fLengthPolygon;
	pVertex[1].pos.y = g_posPolygon.y + cosf(g_rotPolygon.z + (g_fAnglePolygon + D3DX_PI / 3)) * g_fLengthPolygon;
	pVertex[1].pos.z = 0.0f;

	pVertex[2].pos.x = g_posPolygon.x + sinf(g_rotPolygon.z + (g_fAnglePolygon + -D3DX_PI / 2)) * g_fLengthPolygon;
	pVertex[2].pos.y = g_posPolygon.y + cosf(g_rotPolygon.z + (g_fAnglePolygon + -D3DX_PI / 2)) * g_fLengthPolygon;
	pVertex[2].pos.z = 0.0f;

	pVertex[3].pos.x = g_posPolygon.x + sinf(g_rotPolygon.z + g_fAnglePolygon) * g_fLengthPolygon;
	pVertex[3].pos.y = g_posPolygon.y + cosf(g_rotPolygon.z + g_fAnglePolygon) * g_fLengthPolygon;
	pVertex[3].pos.z = 0.0f;

	//pVertex[3].pos = D3DXVECTOR3(sinf(g_fAnglePolygon) * g_fLengthPolygon, cosf(g_fAnglePolygon) * g_fLengthPolygon, 0.0f);
	//pVertex[0].pos = D3DXVECTOR3(-40 + g_posPolygon.x, 60 + g_posPolygon.y, 0.0f);
	//pVertex[1].pos = D3DXVECTOR3(-40 + g_posPolygon.x, -60 + g_posPolygon.y, 0.0f);
	//pVertex[2].pos = D3DXVECTOR3(40 + g_posPolygon.x, 60 + g_posPolygon.y, 0.0f);
	//pVertex[3].pos = D3DXVECTOR3(40 + g_posPolygon.x, -60 + g_posPolygon.y, 0.0f);

	if (GetKeyboardTrigger(DIK_SPACE) == true)
	{
		D3DXVECTOR3 BulletMove;
		BulletMove = D3DXVECTOR3(0.0f, cosf(D3DX_PI) * 10, 0.0f);
		BulletMove.y += BulletMove.y;
		SetBullet(g_posPolygon, BulletMove, 50);
	}

	//頂点バッファをアンロック
	g_pVtxBuffPolygon->Unlock();
}

//-----------------------------------------------------------------------------
//ポリゴンの描画処理
//-----------------------------------------------------------------------------
void DrawPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePolygon);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}*/