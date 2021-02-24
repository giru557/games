//-----------------------------------------------------------------------------
//
// 弾の処理 [bullet.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "bullet.h"
#include "input.h"
#include "model.h"
#include "shadow.h"
#include "player.h"
#include "wall.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffBullet = NULL;									// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;											// ポリゴンのテクスチャへのポインタ
Bullet g_aBullet[BULLET_MAX];														// 弾の情報
int g_aShadowIdxBullet[SHADOW_MAX];													// 弾の影番号

//-----------------------------------------------------------------------------
// 弾の初期化処理
//-----------------------------------------------------------------------------
HRESULT InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//弾の情報配列を初期化
	for (int nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		g_aBullet[nCntBullet].bUse = false;
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLifeTime = 60;
	}

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\bullet000.png" , &g_pTextureBullet);

	//頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VERTEX_AMOUNT * BULLET_MAX,		//サイズ
								D3DUSAGE_WRITEONLY,									//
								FVF_VERTEX_3D,										//フォーマット
								D3DPOOL_MANAGED,									//
								&g_pVtxBuffBullet,									//頂点バッファへのポインタ
								NULL);

	VERTEX_3D *pVertex;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++, pVertex+=4)
	{
		//頂点座標設定 ポリゴンの中心から縦に立てて設定
		pVertex[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLET_WIDTH, g_aBullet[nCntBullet].pos.y - BULLET_HEIGHT, g_aBullet[nCntBullet].pos.z);
		pVertex[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLET_WIDTH, g_aBullet[nCntBullet].pos.y + BULLET_HEIGHT, g_aBullet[nCntBullet].pos.z);
		pVertex[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLET_WIDTH, g_aBullet[nCntBullet].pos.y - BULLET_HEIGHT, g_aBullet[nCntBullet].pos.z);
		pVertex[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLET_WIDTH, g_aBullet[nCntBullet].pos.y + BULLET_HEIGHT, g_aBullet[nCntBullet].pos.z);

		//法線ベクトルの設定
		pVertex[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVertex[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVertex[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVertex[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVertex[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVertex[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVertex[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVertex[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
// 弾の終了処理
//-----------------------------------------------------------------------------
void UninitBullet(void)
{
	//テクスチャの破棄
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//-----------------------------------------------------------------------------
// 弾の更新処理
//-----------------------------------------------------------------------------
void UpdateBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	Model *pModel = GetModel();

	//スペースキーが押されたら
	if (GetKeyboardTrigger(DIK_SPACE) == true)
	{
		//SetBullet(D3DXVECTOR3(pModel->pos.x, 20.0f, pModel->pos.z), D3DXVECTOR3(sinf(pModel->rot.y) * 2, 0.0f, cosf(pModel->rot.y) * 2));
	}

	//使用中の弾を見つける
	for (int nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			//弾の位置を更新
			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;

			//弾の影の位置更新
			SetPositionShadow(g_aShadowIdxBullet[nCntBullet], D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, 0.0f, g_aBullet[nCntBullet].pos.z));

			//当たり判定テスト
			Player *pPlayer = GetPlayer();
			Wall *pWall = GetWall();
			D3DXVECTOR3 vecA;
			D3DXVECTOR3 vecB;
			D3DXVECTOR3 aPos[2];

			//壁の位置から向きの左右に100移動した場所にposを配置
			aPos[0] = pWall->pos + D3DXVECTOR3(sinf(pWall->rot.y + -D3DX_PI / 2) * 100, 0.0f, cosf(pWall->rot.y + -D3DX_PI / 2) * 100);
			aPos[1] = pWall->pos + D3DXVECTOR3(sinf(pWall->rot.y + D3DX_PI / 2) * 100, 0.0f, cosf(pWall->rot.y + D3DX_PI / 2) * 100);

			//pos0から1へのベクトル
			vecA = aPos[1] - aPos[0];

			//pos0から弾の位置へのベクトル
			vecB = g_aBullet[nCntBullet].pos - aPos[0];

			//外積結果がマイナスだった場合
			if ((vecA.z * vecB.x) - (vecA.x  * vecB.z) <= 0)
			{
				D3DXVECTOR3 vecA, vecB;
				float fDot;

				//弾の移動ベクトル
				vecA = g_aBullet[nCntBullet].move;

				//壁の法線ベクトル
				vecB = D3DXVECTOR3(sinf(pWall->rot.y), 0.0f, cosf(pWall->rot.y));

				//正規化
				D3DXVec3Normalize(&vecB, &vecB);

				//内積結果が大きさ
				fDot = ((vecA.x * -1) * vecB.x) + ((vecA.z * -1) * vecB.z);

				//法線に内積結果の大きさの2倍をかける
				vecB = vecB * (fDot * 2);

				//弾の移動ベクトル(vecA)と法線の方向に向かう大きさfDotのベクトル(vecB)を足す
				g_aBullet[nCntBullet].move = vecA + vecB;
			}

			//寿命
			//g_aBullet[nCntBullet].nLifeTime--;
			//if (g_aBullet[nCntBullet].nLifeTime <= 0)
			//{
			//	g_aBullet[nCntBullet].bUse = false;
			//}
		}
	}
}

//-----------------------------------------------------------------------------
// 弾の描画処理
//-----------------------------------------------------------------------------
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// デバイスへのポインタ
	D3DXMATRIX mtxView;					// ビューマトリックス取得
	D3DXMATRIX mtxTrans;				// 計算用マトリックス

	//デバイス取得
	pDevice = GetDevice();

	for (int nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			//透明色を描画しない(アルファテスト)
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// アルファテスト有効化
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// アルファテスト比較、値よりも大きい場合
			pDevice->SetRenderState(D3DRS_ALPHAREF, 127);				// アルファ値参照

			//比較をやめて常に手前に描画
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aBullet[nCntBullet].mtxWorld);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);
			
			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aBullet[nCntBullet].mtxWorld, NULL, &mtxView);
			g_aBullet[nCntBullet].mtxWorld._41 = 0.0f;
			g_aBullet[nCntBullet].mtxWorld._42 = 0.0f;
			g_aBullet[nCntBullet].mtxWorld._43 = 0.0f;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y, g_aBullet[nCntBullet].pos.z);
			D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCntBullet].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBullet);
			
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntBullet, 2);

			//初期値に戻す
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		}
	}

}

//-----------------------------------------------------------------------------
// 弾の設置、設定
//-----------------------------------------------------------------------------
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	for (int nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{
			//弾の位置設定
			g_aBullet[nCntBullet].pos = pos;

			//弾の移動設定
			g_aBullet[nCntBullet].move = move;

			//弾の使用状態を使用中に
			g_aBullet[nCntBullet].bUse = true;

			//弾の影をつける
			g_aShadowIdxBullet[nCntBullet] = SetShadow(pos, 50, 50);

			break;
		}
	}
}