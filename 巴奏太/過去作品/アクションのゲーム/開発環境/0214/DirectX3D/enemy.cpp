#define _CRT_SECURE_NO_WARNINGS
//-----------------------------------------------------------------------------
//
// 敵の処理 [enemy.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "enemy.h"
#include "camera.h"
#include "input.h"
#include "shadow.h"
#include "bullet.h"
#include "collision.h"
#include "fade.h"
#include "UI.h"
#include "result.h"
#include "sound.h"
#include <stdio.h>

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define PLAYER_MODEL_AMOUNT (10)
#define PLAYER_SMOOTHTURN_SPEED (0.1f)											// 滑らかに振り向く速度

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
void MoveEnemy(float fMoveAngleDegree, float fMoveSpeed);
void LoadXFileEnemy(const char* cXFileName, int nCountModel);
void EnemySmoothTurn(void);

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
Enemy g_aEnemy[ENEMY_AMOUNT_MAX];	// 敵の情報
int g_nEnemyAlive = 0;				// 敵の生存数
bool g_bEliminated;					// 全滅したか
DWORD g_dwTimeTransition;			// 画面遷移時間計算

//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスの取得

	//Xファイルの読み込み
	LoadXFileEnemy("data\\MODEL\\snake.x", 0);

	for (int nCntEnemy = 0; nCntEnemy < ENEMY_AMOUNT_MAX; nCntEnemy++)
	{
		//初期設定
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置の初期設定
		g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向きの初期設定
		g_aEnemy[nCntEnemy].bUse = false;
		g_aEnemy[nCntEnemy].bInvincible = false;
		g_aEnemy[nCntEnemy].nLife = ENEMY_HP_MAX;
		g_aEnemy[nCntEnemy].nLifeMax = ENEMY_HP_MAX;
		g_aEnemy[nCntEnemy].bHit = false;
		g_aEnemy[nCntEnemy].bAttack = false;
		g_aEnemy[nCntEnemy].dwTimeAtk = NULL;
		g_aEnemy[nCntEnemy].dwTimeInv = NULL;
		g_aEnemy[nCntEnemy].nIdx = NULL;
	}
	g_nEnemyAlive = 0;
	g_dwTimeTransition = NULL;
	g_bEliminated = false;
}

//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void UninitEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < ENEMY_AMOUNT_MAX; nCntEnemy++)
	{
		//メッシュ破棄
		if (g_aEnemy[nCntEnemy].pMesh != NULL)
		{
			g_aEnemy[nCntEnemy].pMesh->Release();
			g_aEnemy[nCntEnemy].pMesh = NULL;
		}

		//マテリアル破棄
		if (g_aEnemy[nCntEnemy].pBuffMat != NULL)
		{
			g_aEnemy[nCntEnemy].pBuffMat->Release();
			g_aEnemy[nCntEnemy].pBuffMat = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void UpdateEnemy(void)
{
	DWORD dwCurrentTime = timeGetTime();	// 現在時間

	EnemyUI *pEnemyUI = GetEnemyRemain();	// UI情報取得
	pEnemyUI->nAmount = g_nEnemyAlive;		// 敵の残量をUI情報に渡す

	//使用中の敵を探す
	for (int nCntEnemy = 0; nCntEnemy < ENEMY_AMOUNT_MAX; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			Player *pPlayer = GetPlayer();	// プレイヤーの取得

			//移動量追加
			g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;

			//移動量減衰
			g_aEnemy[nCntEnemy].move.x = g_aEnemy[nCntEnemy].move.x * 0.2f;
			g_aEnemy[nCntEnemy].move.z = g_aEnemy[nCntEnemy].move.z * 0.2f;

			//重力
			g_aEnemy[nCntEnemy].move.y -= ENEMY_FALLSPEED;

			//体力なくなったら
			if (g_aEnemy[nCntEnemy].nLife <= 0)
			{
				VCollision *pVCol = GetVCol();

				//使用状況を切り替える
				g_aEnemy[nCntEnemy].bUse = false;

				//生存数を減算
				g_nEnemyAlive--;

				//敵撃破表示をする
				ShowEnemyDestroyed();

				//可視化コリジョン消す
				pVCol[2].bUse = false;
			}

			//プレイヤーとの距離
			float fDistanceToPlayer = sqrtf((pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x) * ((pPlayer->pos.x) - g_aEnemy[nCntEnemy].pos.x) + (pPlayer->pos.z - g_aEnemy[nCntEnemy].pos.z) * (pPlayer->pos.z - g_aEnemy[nCntEnemy].pos.z));

			//距離が検知範囲以内だったら
			if (fDistanceToPlayer <= ENEMY_DETECT_RADIUS)
			{
				//プレイヤーに向かう単位ベクトル
				D3DXVECTOR3 vecToPlayer = pPlayer->pos - g_aEnemy[nCntEnemy].pos;
				D3DXVec3Normalize(&vecToPlayer, &vecToPlayer);

				//移動を加算
				g_aEnemy[nCntEnemy].move += vecToPlayer * ENEMY_MOVESPEED;

				//プレイヤーへのラジアン角
				float fRadianToPlayer = atan2f(g_aEnemy[nCntEnemy].pos.x - pPlayer->pos.x, g_aEnemy[nCntEnemy].pos.z - pPlayer->pos.z);
				g_aEnemy[nCntEnemy].rot.y = fRadianToPlayer;
			}

			//距離が攻撃範囲内だったら
			if (fDistanceToPlayer <= ENEMY_ATTACK_RADIUS)
			{
				//すでに攻撃していない場合
				if (g_aEnemy[nCntEnemy].bAttack != true)
				{
					//プレイヤーの無敵時間じゃない場合
					if (pPlayer->bInvincible != true)
					{
						//現在時間取得（無敵時間計算）
						g_aEnemy[nCntEnemy].dwTimeAtk = timeGetTime();

						//現在時間取得（攻撃CT計算）
						pPlayer->dwTime = timeGetTime();

						//体力減少
						pPlayer->nLife--;

						//敵から自分に向かうベクトル
						D3DXVECTOR3 vecEnemyToPlayer = pPlayer->pos - g_aEnemy[nCntEnemy].pos;
						D3DXVec3Normalize(&vecEnemyToPlayer, &vecEnemyToPlayer);

						//ノックバック
						pPlayer->move += vecEnemyToPlayer * ENEMY_KNOCKBACK;

						//無敵になる
						pPlayer->bInvincible = true;

						//敵が攻撃クールタイムに入る
						g_aEnemy[nCntEnemy].bAttack = true;

						//ダメージ音
						PlaySound(SOUND_LABEL_SE_DMGPLAYER);
					}
				}
			}

			//時間経過で攻撃CT解除
			if (dwCurrentTime - g_aEnemy[nCntEnemy].dwTimeAtk >= ENEMY_ATTACK_COOLTIME)
			{
				//CT解除
				g_aEnemy[nCntEnemy].bAttack = false;
			}

			//下に落ちてったら消す
			if (g_aEnemy[nCntEnemy].pos.y < -50.0f)
			{
				g_aEnemy[nCntEnemy].bUse = false;
			}
		}
	}

	//全滅
	if (g_nEnemyAlive <= 0)
	{
		if (g_bEliminated != true)
		{
			GAMEOVER *pGameOver = GetGameOver();
			RESULT *pResult = GetResult();

			//ゲームオーバー画面を描画
			pGameOver->bDraw = true;
			pGameOver->bGameOver = false;
			pResult->bGameOver = false;

			//時間取得
			g_dwTimeTransition = timeGetTime();

			g_bEliminated = true;
		}
		
		//1.5秒経ったら
		if (dwCurrentTime - g_dwTimeTransition >= RESULT_WAITTIME)
		{
			//リザルトに遷移
			SetFade(FADE_OUT, MODE_RESULT);
		}
	}
}

//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();									// デバイス取得

	for (int nCntEnemy = 0; nCntEnemy < ENEMY_AMOUNT_MAX; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			D3DXMATRIX mtxRot, mtxTrans;												// 計算用マトリックス
			D3DMATERIAL9 matDef;														// 現在のマテリアル保存用
			D3DXMATERIAL *pMat;															// マテリアルデータへのポインタ
			D3DXMATERIAL *pMatAlt;														// 代えのマテリアル
			//プレイヤーのワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].mtxWorld);

			//プレイヤーの向き反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].rot.y, g_aEnemy[nCntEnemy].rot.x, g_aEnemy[nCntEnemy].rot.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxRot);

			//プレイヤーの位置反映
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxTrans);

			//プレイヤーのワールドマトリックス設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].mtxWorld);

			//現在のマテリアル取得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aEnemy[nCntEnemy].pBuffMat->GetBufferPointer();
			pMatAlt = (D3DXMATERIAL*)g_aEnemy[nCntEnemy].pBuffMat->GetBufferPointer();

			//無敵状態か見てモデルの色を変える
			if (g_aEnemy[nCntEnemy].bInvincible == true)
			{//無敵状態
				pMatAlt->MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f);
				for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntEnemy].nNumMat; nCntMat++)
				{
					//マテリアル設定
					pDevice->SetMaterial(&pMatAlt[nCntMat].MatD3D);

					//テクスチャの設定
					pDevice->SetTexture(0, g_aEnemy[nCntEnemy].pTexture[nCntMat]);

					//モデルパーツの描画
					g_aEnemy[nCntEnemy].pMesh->DrawSubset(nCntMat);
				}
			}
			else if (g_aEnemy[nCntEnemy].bInvincible == false)
			{//通常状態
				pMat->MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0, 1.0f, 1.0f);
				for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntEnemy].nNumMat; nCntMat++)
				{
					//マテリアル設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					//テクスチャの設定
					pDevice->SetTexture(0, g_aEnemy[nCntEnemy].pTexture[nCntMat]);

					//モデルパーツの描画
					g_aEnemy[nCntEnemy].pMesh->DrawSubset(nCntMat);
				}
			}

			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//-----------------------------------------------------------------------------
// プレイヤーの情報を取得
//-----------------------------------------------------------------------------
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//-----------------------------------------------------------------------------
// 敵を配置する
//-----------------------------------------------------------------------------
void SetEnemy(D3DXVECTOR3 pos)
{
	for (int nCntEnemy = 0; nCntEnemy < ENEMY_AMOUNT_MAX; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{
			//位置を指定
			g_aEnemy[nCntEnemy].pos = pos;

			//番号付け
			g_aEnemy[nCntEnemy].nIdx = nCntEnemy;

			//生存数に加算
			g_nEnemyAlive++;

			//使用中に移行
			g_aEnemy[nCntEnemy].bUse = true;
			break;
		}
	}
}

/* モデルを移動させる関数 */
void MoveEnemy(float fMoveAngleDegree, float fMoveSpeed)
{
	/*Camera *pCamera = GetCamera();	// カメラの情報取得

	g_enemy.pos.x += sinf(pCamera->rot.y + D3DXToRadian(fMoveAngleDegree)) * fMoveSpeed;
	g_enemy.pos.z += cosf(pCamera->rot.y + D3DXToRadian(fMoveAngleDegree)) * fMoveSpeed;
	g_enemy.rotDest.y = pCamera->rot.y + D3DXToRadian(fMoveAngleDegree) + D3DX_PI;*/
}

/* Xファイルからモデルを読み込む関数*/
void LoadXFileEnemy(const char* cXFileName, int nCountModel)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス取得

	for (int nCntEnemy = 0; nCntEnemy < ENEMY_AMOUNT_MAX; nCntEnemy++)
	{
		//Xファイルの読み込み
		D3DXLoadMeshFromX(
			cXFileName,										// 読み込むファイル名
			D3DXMESH_SYSTEMMEM,								// 固定
			pDevice,										// デバイス
			NULL,											// NULL固定
			&g_aEnemy[nCntEnemy].pBuffMat,					// マテリアル
			NULL,											// NULL固定
			&g_aEnemy[nCntEnemy].nNumMat,					// マテリアル数
			&g_aEnemy[nCntEnemy].pMesh);					// メッシュ

		//マテリアル情報へのポインタ
		D3DXMATERIAL *pMat = (D3DXMATERIAL*)g_aEnemy[nCntEnemy].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntEnemy].nNumMat; nCntMat++)
		{
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aEnemy[nCntEnemy].pTexture[nCntMat]);
		}
	}
}

/* キャラの回転を滑らかにする関数 */
void EnemySmoothTurn(void)
{
	/*D3DXVECTOR3 RotDiff;

	//差分計算
	RotDiff.y = g_enemy.rotDest.y - g_enemy.rot.y;

	//差分がD3DX_PI以上(半周以上)の場合、逆回転
	if (RotDiff.y > D3DX_PI)
	{
		g_enemy.rot.y -= ((D3DX_PI * 2) - RotDiff.y) * PLAYER_SMOOTHTURN_SPEED;
	}
	else if (RotDiff.y < -D3DX_PI)
	{
		g_enemy.rot.y += ((D3DX_PI * 2) + RotDiff.y) * PLAYER_SMOOTHTURN_SPEED;
	}
	else
	{
		g_enemy.rot.y += RotDiff.y * PLAYER_SMOOTHTURN_SPEED;
	}

	// 回転の修正 (3.14超えたら±逆に)
	if (g_enemy.rot.y > D3DX_PI)
	{
		g_enemy.rot.y -= D3DX_PI * 2.0f;
	}
	else if (g_enemy.rot.y < -D3DX_PI)
	{
		g_enemy.rot.y += D3DX_PI * 2.0f;
	}
	if (g_enemy.rotDest.y > D3DX_PI)
	{
		g_enemy.rotDest.y -= D3DX_PI * 2.0f;
	}
	else if (g_enemy.rotDest.y < -D3DX_PI)
	{
		g_enemy.rotDest.y += D3DX_PI * 2.0f;
	}*/
}