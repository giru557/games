#define _CRT_SECURE_NO_WARNINGS
//-----------------------------------------------------------------------------
//
// DirectX3Dゲーム [main.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "main.h"
#include "input.h"
#include "game.h"
#include "light.h"
#include "camera.h"
#include "polygon.h"
#include "model.h"
#include "shadow.h"
#include "wall.h"
#include "billboard.h"
#include "meshfield.h"
#include "bullet.h"
#include "player.h"
#include "result.h"
#include "title.h"
#include "fade.h"
#include "collision.h"
#include "sound.h"
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
void DrawText(void);

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3D9 g_pD3D = NULL;							// Direct3Dオブジェクトのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;				// Direct3Dデバイスへのポインタ(描画に必要)
LPD3DXFONT g_pFont = NULL;							// フォントへのポインタ
MODE g_mode = MODE_TITLE;							// 画面のモード
int g_nUpdateSpeed = 60;							// 画面の更新頻度(fps)
int g_nCountFPS;									// FPSカウンタ

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
			if ((dwCurrentTime - dwExecLastTime) > (unsigned)(1000 / g_nUpdateSpeed))
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

	//モードの種類
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
	if (GetKeyboardRepeat(DIK_O) == true)
	{
		g_nUpdateSpeed--;
	}
	else if (GetKeyboardRepeat(DIK_P) == true)
	{
		g_nUpdateSpeed++;
	}

	//キー入力の更新処理
	UpdateKeyboard();

	switch (g_mode)
	{
	case MODE_TITLE:
		UpdateTitle();
		break;

	case MODE_GAME:
		//ゲームの更新処理
		UpdateGame();
		break;

	case MODE_RESULT:
		UpdateResult();
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

		case MODE_GAME:
			//ゲームの描画処理
			DrawGame();

#ifdef _DEBUG
			DrawText();
#endif

			break;

		case MODE_RESULT:
			DrawResult();
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
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_RIGHT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//-----------------------------------------------------------------------------
// テキストの表示
//-----------------------------------------------------------------------------
void DrawText(void)
{
	Model *pModel = GetModel();
	Player *pPlayer = GetPlayer();
	Camera *pCamera = GetCamera();

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[1024];
	int nCharNum = 0;
	D3DXVECTOR2 testMouse = GetMouseVelocity();

	nCharNum = sprintf(&aStr[nCharNum], "画面の更新頻度: %d\n", g_nUpdateSpeed);
	nCharNum += sprintf(&aStr[nCharNum], "プレイヤーの向き: %f \n", pPlayer->rot.y);
	nCharNum += sprintf(&aStr[nCharNum], "プレイヤーの目標向き: %f\n", pPlayer->rotDest.y);
	nCharNum += sprintf(&aStr[nCharNum], "カメラの向きY: %f\n", pCamera->rot.y);
	nCharNum += sprintf(&aStr[nCharNum], "カメラの向きZ: %f\n", pCamera->rot.z);
	nCharNum += sprintf(&aStr[nCharNum], "key: %d\n", pPlayer->nKey);
	nCharNum += sprintf(&aStr[nCharNum], "frame: %d\n", pPlayer->nCounterMotion);
	nCharNum += sprintf(&aStr[nCharNum], "PlayerPosition: x: %.2f y: %.2f z: %.2f\n", pPlayer->pos.x, pPlayer->pos.y, pPlayer->pos.z);
	nCharNum += sprintf(&aStr[nCharNum], "\n\n\n\n");
	nCharNum += sprintf(&aStr[nCharNum], "[ENTER]: リザルト画面へ遷移\n");
	nCharNum += sprintf(&aStr[nCharNum], "[W] [A] [S] [D] モデルの移動\n");
	nCharNum += sprintf(&aStr[nCharNum], "[SPACE]: アタックモーション&反射確認の弾発射\n");
	nCharNum += sprintf(&aStr[nCharNum], "O, P: 更新頻度変更\n");
	nCharNum += sprintf(&aStr[nCharNum], "マウスの移動テスト: x[ %.2f ] y[ %.2f ]\n", testMouse.x, testMouse.y);
	nCharNum += sprintf(&aStr[nCharNum], "プレイヤーの移動値: [ x:%.4f z:%.4f ]\n", pPlayer->move.x, pPlayer->move.z);
	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
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
		StopSound();
		break;

	case MODE_GAME:
		//ゲーム画面の終了処理
		UninitGame();
		StopSound();
		break;

	case MODE_RESULT:
		UninitResult();
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
		PlaySound(SOUND_LABEL_BGM_TITLE);
		break;

	case MODE_GAME:
		//ゲームの初期化処理
		InitGame();
		PlaySound(SOUND_LABEL_BGM_GAME);
		break;

	case MODE_RESULT:
		//リザルトの初期化処理
		InitResult();
		PlaySound(SOUND_LABEL_BGM_RESULT);
		break;

	default:
		break;
	}

	g_mode = mode;
}