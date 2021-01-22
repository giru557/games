#define _CRT_SECURE_NO_WARNINGS
//-----------------------------------------------------------------------------
//
// DirectX2Dアクションゲーム [main.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "main.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "player.h"
#include "leaderboard.h"
#include "rankedscore.h"
#include "resultscore.h"
#include "goalflag.h"
#include "enemy.h"
#include "tutorial.h"
#include <stdio.h>

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND nWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawFPS(void);

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3D9 g_pD3D = NULL;							//Direct3Dオブジェクトのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;				//Direct3Dデバイスへのポインタ(描画に必要)
LPD3DXFONT g_pFont = NULL;							//フォントへのポインタ
int g_nCountFPS;									//FPSカウンタ
int g_aMapData[MAP_TILE_ROW][MAP_TILE_COLUMN];
MODE g_mode = MODE_TITLE;							//モードの種類

//-----------------------------------------------------------------------------
// メイン関数
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;	//ウィンドウハンドル(識別子)
	MSG msg;	//メッセージを格納する
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	dwExecLastTime = timeGetTime();
	dwFPSLastTime = timeGetTime();
	dwCurrentTime = timeGetTime();
	dwFrameCount = 0;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WindowProc,
		0,
		0,
		hInstance,
		LoadIcon(NULL, IDI_APPLICATION),
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		LoadIcon(NULL, IDI_APPLICATION)
	};

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	//ウィンドウサイズの調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	//ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	//ウィンドウの作成
	hWnd = CreateWindowEx(
		0,
		CLASS_NAME,					//ウィンドウクラスの名前
		WINDOW_NAME,				//ウィンドウの名前(キャプション)
		WS_OVERLAPPEDWINDOW,		//ウィンドウスタイル
		CW_USEDEFAULT,				//ウィンドウの左上X座標
		CW_USEDEFAULT,				//ウィンドウの左上Y座標
		rect.right - rect.left,		//ウィンドウの幅
		rect.bottom - rect.top,		//ウィンドウの高さ
		NULL,						//親ウィンドウのハンドル
		NULL,
		hInstance,
		NULL);

	//初期化処理
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	//ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0) //メッセージを取得しなかった場合0を返す
		{//Windowsの処理
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5秒経過
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if ((dwCurrentTime - dwExecLastTime) > (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;

				//更新処理
				Update();

				//描画処理
				Draw();

				dwFrameCount++;
			}
		}
	}

	//終了処理
	Uninit();

	//ウィンドウクラスの登録解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//-----------------------------------------------------------------------------
// ウィンドウプロシージャ
//-----------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	//メッセージの選択
	switch (uMsg)
	{
	//キー押下時
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_DELETE:
			//ESCキーで終了
			nID = MessageBox(NULL, "本当に終了しますか？", NULL, MB_YESNO);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd); //WM_DESTROYメッセージを返す
			}
			break;
		default:
			break;
		}
		break;
	//このメッセージが返されたら終了メッセージを渡す
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
} 

//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;
	D3DPRESENT_PARAMETERS d3dpp;

	//Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//ディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.Windowed = bWindow;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//Direct3Dデバイスの生成
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//サンプラーステートの設定
	//g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	//g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				//カリングの設定
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				//αブレンド（α値の合成）の設定
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		//ソース（描画元）の合成方法の設定
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	//デスティネーション（描画先）の合成方法の設定

	//フォントの生成
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFont);

	//キー入力の初期化処理
	InitKeyboard(hInstance, hWnd);

	//サウンドの初期化処理
	InitSound(hWnd);

	//フェードの初期化処理
	InitFade(MODE_TITLE);

	// モードの種類
	SetMode(g_mode);

	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void Uninit(void)
{
	//フェードの終了処理
	UninitFade();

	//サウンドの終了処理
	UninitSound();

	//キー入力の終了処理
	UninitKeyboard();

	//Direct3Dデバイスの開放
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3Dオブジェクトの開放
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	//フォントの開放
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}
}

//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void Update(void)
{
	//キー入力の更新処理
	UpdateKeyboard();

	switch (g_mode)
	{
	case MODE_TITLE:
		//タイトルの更新処理
		UpdateTitle();
		break;

	case MODE_TUTORIAL:
		//チュートリアルの更新処理
		UpdateTutorial();
		break;

	case MODE_GAME:
		//ゲームの更新処理
		UpdateGame();
		break;

	case MODE_RESULT:
		//リザルトの更新処理
		UpdateResult();
		UpdateResultScore();
		break;

	case MODE_RANKING:
		//ランキングの更新処理
		UpdateRanking();
		UpdateRScore();
		break;

	default:
		break;
	}

	//フェードの更新処理
	UpdateFade();
}

//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void Draw(void)
{
	//バックバッファ&Zバッファのクリア(画面のクリア)
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//描画の開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		switch (g_mode)
		{
		case MODE_TITLE:
			//タイトルの描画処理
			DrawTitle();
			break;

		case MODE_TUTORIAL:
			//チュートリアルの描画処理
			DrawTutorial();
			break;

		case MODE_GAME:
			//ゲームの描画処理
			DrawGame();
			break;

		case MODE_RESULT:
			//リザルトの描画処理
			DrawResult();
			DrawResultScore();
			break;

		case MODE_RANKING:
			//ランキングの描画処理
			DrawRanking();
			DrawRScore();
			break;

		default:
			break;
		}

		//フェードの描画処理
		DrawFade();

#ifdef _DEBUG

		//FPSの描画処理
		DrawFPS();

#endif

		//描画の終了
		g_pD3DDevice->EndScene();
	}
	
	//バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//-----------------------------------------------------------------------------
// デバイスの取得
//-----------------------------------------------------------------------------
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//-----------------------------------------------------------------------------
// FPSの表示
//-----------------------------------------------------------------------------
void DrawFPS(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[256];

	wsprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//-----------------------------------------------------------------------------
// モードの設定
//-----------------------------------------------------------------------------
void SetMode(MODE mode)
{
	//現在の処理を終了
	switch (g_mode)
	{
	case MODE_TITLE:
		//タイトルの終了処理
		UninitTitle();
		break;
		
	case MODE_TUTORIAL:
		//チュートリアルの終了処理
		UninitTutorial();
		StopSound();
		break;

	case MODE_GAME:
		//ゲームの終了処理
		UninitGame();
		break;

	case MODE_RESULT:
		//リザルトの終了処理
		UninitResultScore();
		UninitResult();
		StopSound();
		break;

	case MODE_RANKING:
		//ランキングの終了処理
		UninitRScore();
		UninitRanking();
		StopSound();
		break;

	default:
		break;
	}

	//遷移後の処理を初期化
	switch (mode)
	{
	case MODE_TITLE:
		//タイトルの初期化処理
		InitTitle();
		PlaySound(SOUND_LABEL_BGM_OP);
		break;

	case MODE_TUTORIAL:
		//チュートリアルの初期化処理
		InitTutorial();
		break;

	case MODE_GAME:
		//ゲームの初期化処理
		InitGame();
		break;

	case MODE_RESULT:
		//リザルトの初期化処理
		InitResult();
		InitResultScore();
		PlaySound(SOUND_LABEL_BGM_RESULT);
		break;

	case MODE_RANKING:
		//ランキングの初期化処理
		InitRanking();
		InitRScore();
		PlaySound(SOUND_LABEL_BGM_RANKING);
		break;

	default:
		break;
	}

	g_mode = mode;
}

MODE GetMode(void)
{
	return g_mode;
}

void LoadMapData(STAGE stage)
{
	FILE *pFile;

	//マップデータを配列に読み込み
	if (stage == STAGE_1)
	{
		pFile = fopen("MapData.csv", "r");
	}
	else if (stage == STAGE_2)
	{
		pFile = fopen("MapData2.csv", "r");
	}
	else
	{
		pFile = NULL;
	}
	if (pFile != NULL)
	{
		int nCntX = 0, nCntY = 0;
		char aLine[MAP_MAXCHAR];
		while (fgets(aLine, MAP_MAXCHAR, pFile) != NULL)
		{
			nCntX = 0;
			char *pToken = strtok(aLine, ",");
			while (pToken != NULL)
			{
				int nNum = atoi(pToken);
				g_aMapData[nCntY][nCntX] = nNum;
				pToken = strtok(NULL, ",");
				nCntX++;
			}
			nCntY++;
		}
	}
	fclose(pFile);

	//読み込んだマップデータからブロックを配置する
	for (int nCntY = 0; nCntY < MAP_TILE_ROW; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAP_TILE_COLUMN; nCntX++)
		{
			D3DXVECTOR3 pos = D3DXVECTOR3(nCntX * 50.0f, (nCntY) * 50.0f, 0.0f);

			switch (g_aMapData[nCntY][nCntX])
			{
			case 1:
				SetBlock(pos, 50.0f, 50.0f, 0, BLOCKTEXTURE_NORMAL, NULL, NULL);
				break;
			case 2:
				//上側ブロック配置
				SetBlock(pos, 50.0f, 50.0f, 0, BLOCKTEXTURE_TOP, NULL, NULL);
				break;
			case 3:
				//上側右端ブロック配置
				SetBlock(pos, 50, 50, 0, BLOCKTEXTURE_TOPRIGHT, NULL, NULL);
				break;
			case 4:
				//上側左端ブロック配置
				SetBlock(pos, 50, 50, 0, BLOCKTEXTURE_TOPLEFT, NULL, NULL);
				break;
			case 5:
				//右端ブロック
				SetBlock(pos, 50, 50, 0, BLOCKTEXTURE_RIGHT, NULL, NULL);
				break;
			case 6:
				//左端ブロック
				SetBlock(pos, 50, 50, 0, BLOCKTEXTURE_LEFT, NULL, NULL);
				break;
			case 10:
				//ゴール配置
				SetFlag(pos, 100, 100);
				break;
			case 20:
				//敵配置(左移動)
				SetEnemy(pos, D3DXVECTOR3(-2, 0, 0));
				break;
			case 21:
				//敵配置(右移動
				SetEnemy(pos, D3DXVECTOR3(2, 0, 0));
				break;
			case 30:
				//移動床(縦)1
				SetBlock(pos, 150, 50, 2, BLOCKTEXTURE_TOP, 6000, 2);
				break;
			case 31:
				//移動床(横)1
				SetBlock(pos, 150, 50, 1, BLOCKTEXTURE_TOP, 3000, 2);
				break;
			default:
				break;
			}
		}
	}
}