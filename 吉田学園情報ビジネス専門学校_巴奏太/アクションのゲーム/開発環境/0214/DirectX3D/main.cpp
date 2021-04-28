#define _CRT_SECURE_NO_WARNINGS
//-----------------------------------------------------------------------------
//
// DirectX3D�Q�[�� [main.cpp]
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
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND nWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawFPS(void);
void DrawText(void);

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3D9 g_pD3D = NULL;							// Direct3D�I�u�W�F�N�g�̃|�C���^
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;				// Direct3D�f�o�C�X�ւ̃|�C���^(�`��ɕK�v)
LPD3DXFONT g_pFont = NULL;							// �t�H���g�ւ̃|�C���^
MODE g_mode = MODE_TITLE;							// ��ʂ̃��[�h
int g_nUpdateSpeed = 60;							// ��ʂ̍X�V�p�x(fps)
int g_nCountFPS;									// FPS�J�E���^

//-----------------------------------------------------------------------------
// ���C���֐�
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;	//�E�B���h�E�n���h��(���ʎq)
	MSG msg;	//���b�Z�[�W���i�[����
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

	//�E�B���h�E�T�C�Y�̒���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	//�E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	//�E�B���h�E�̍쐬
	hWnd = CreateWindowEx(
		0,
		CLASS_NAME,					//�E�B���h�E�N���X�̖��O
		WINDOW_NAME,				//�E�B���h�E�̖��O(�L���v�V����)
		WS_OVERLAPPEDWINDOW,		//�E�B���h�E�X�^�C��
		CW_USEDEFAULT,				//�E�B���h�E�̍���X���W
		CW_USEDEFAULT,				//�E�B���h�E�̍���Y���W
		rect.right - rect.left,		//�E�B���h�E�̕�
		rect.bottom - rect.top,		//�E�B���h�E�̍���
		NULL,						//�e�E�B���h�E�̃n���h��
		NULL,
		hInstance,
		NULL);

	//����������
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	//�E�B���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0) //���b�Z�[�W���擾���Ȃ������ꍇ0��Ԃ�
		{//Windows�̏���
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
		{//DirectX�̏���
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5�b�o��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}
			if ((dwCurrentTime - dwExecLastTime) > (unsigned)(1000 / g_nUpdateSpeed))
			{
				dwExecLastTime = dwCurrentTime;

				//�X�V����
				Update();

				//�`�揈��
				Draw();

				dwFrameCount++;
			}
		}
	}

	//�I������
	Uninit();

	//�E�B���h�E�N���X�̓o�^����
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//-----------------------------------------------------------------------------
// �E�B���h�E�v���V�[�W��
//-----------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	//���b�Z�[�W�̑I��
	switch (uMsg)
	{
	//�L�[������
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_DELETE:
			//ESC�L�[�ŏI��
			nID = MessageBox(NULL, "�{���ɏI�����܂����H", NULL, MB_YESNO);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd); //WM_DESTROY���b�Z�[�W��Ԃ�
			}
			break;
		default:
			break;
		}
		break;
	//���̃��b�Z�[�W���Ԃ��ꂽ��I�����b�Z�[�W��n��
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
} 

//-----------------------------------------------------------------------------
// ����������
//-----------------------------------------------------------------------------
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;
	D3DPRESENT_PARAMETERS d3dpp;

	//Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//�f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
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

	//Direct3D�f�o�C�X�̐���
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

	//�T���v���[�X�e�[�g�̐ݒ�
	//g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	//g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//�e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//�����_�[�X�e�[�g�̐ݒ�	
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				//�J�����O�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				//���u�����h�i���l�̍����j�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		//�\�[�X�i�`�挳�j�̍������@�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	//�f�X�e�B�l�[�V�����i�`���j�̍������@�̐ݒ�

	//�t�H���g�̐���
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFont);

	//�L�[���͂̏���������
	InitKeyboard(hInstance, hWnd);

	//�T�E���h�̏���������
	InitSound(hWnd);

	//�t�F�[�h�̏���������
	InitFade(MODE_TITLE);

	//���[�h�̎��
	SetMode(g_mode);

	return S_OK;
}

//-----------------------------------------------------------------------------
// �I������
//-----------------------------------------------------------------------------
void Uninit(void)
{
	//�t�F�[�h�̏I������
	UninitFade();

	//�T�E���h�̏I������
	UninitSound();

	//�L�[���͂̏I������
	UninitKeyboard();

	//Direct3D�f�o�C�X�̊J��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3D�I�u�W�F�N�g�̊J��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	//�t�H���g�̊J��
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}
}

//-----------------------------------------------------------------------------
// �X�V����
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

	//�L�[���͂̍X�V����
	UpdateKeyboard();

	switch (g_mode)
	{
	case MODE_TITLE:
		UpdateTitle();
		break;

	case MODE_GAME:
		//�Q�[���̍X�V����
		UpdateGame();
		break;

	case MODE_RESULT:
		UpdateResult();
		break;

	default:
		break;
	}

	//�t�F�[�h�̍X�V����
	UpdateFade();
}

//-----------------------------------------------------------------------------
// �`�揈��
//-----------------------------------------------------------------------------
void Draw(void)
{
	//�o�b�N�o�b�t�@&Z�o�b�t�@�̃N���A(��ʂ̃N���A)
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//�`��̊J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		switch (g_mode)
		{
		case MODE_TITLE:
			//�^�C�g���̕`�揈��
			DrawTitle();
			break;

		case MODE_GAME:
			//�Q�[���̕`�揈��
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

		//�t�F�[�h�̕`�揈��
		DrawFade();

#ifdef _DEBUG
		//FPS�̕`�揈��
		DrawFPS();
#endif

		//�`��̏I��
		g_pD3DDevice->EndScene();
	}
	
	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//-----------------------------------------------------------------------------
// �f�o�C�X�̎擾
//-----------------------------------------------------------------------------
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//-----------------------------------------------------------------------------
// FPS�̕\��
//-----------------------------------------------------------------------------
void DrawFPS(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[256];

	wsprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_RIGHT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//-----------------------------------------------------------------------------
// �e�L�X�g�̕\��
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

	nCharNum = sprintf(&aStr[nCharNum], "��ʂ̍X�V�p�x: %d\n", g_nUpdateSpeed);
	nCharNum += sprintf(&aStr[nCharNum], "�v���C���[�̌���: %f \n", pPlayer->rot.y);
	nCharNum += sprintf(&aStr[nCharNum], "�v���C���[�̖ڕW����: %f\n", pPlayer->rotDest.y);
	nCharNum += sprintf(&aStr[nCharNum], "�J�����̌���Y: %f\n", pCamera->rot.y);
	nCharNum += sprintf(&aStr[nCharNum], "�J�����̌���Z: %f\n", pCamera->rot.z);
	nCharNum += sprintf(&aStr[nCharNum], "key: %d\n", pPlayer->nKey);
	nCharNum += sprintf(&aStr[nCharNum], "frame: %d\n", pPlayer->nCounterMotion);
	nCharNum += sprintf(&aStr[nCharNum], "PlayerPosition: x: %.2f y: %.2f z: %.2f\n", pPlayer->pos.x, pPlayer->pos.y, pPlayer->pos.z);
	nCharNum += sprintf(&aStr[nCharNum], "\n\n\n\n");
	nCharNum += sprintf(&aStr[nCharNum], "[ENTER]: ���U���g��ʂ֑J��\n");
	nCharNum += sprintf(&aStr[nCharNum], "[W] [A] [S] [D] ���f���̈ړ�\n");
	nCharNum += sprintf(&aStr[nCharNum], "[SPACE]: �A�^�b�N���[�V����&���ˊm�F�̒e����\n");
	nCharNum += sprintf(&aStr[nCharNum], "O, P: �X�V�p�x�ύX\n");
	nCharNum += sprintf(&aStr[nCharNum], "�}�E�X�̈ړ��e�X�g: x[ %.2f ] y[ %.2f ]\n", testMouse.x, testMouse.y);
	nCharNum += sprintf(&aStr[nCharNum], "�v���C���[�̈ړ��l: [ x:%.4f z:%.4f ]\n", pPlayer->move.x, pPlayer->move.z);
	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

//-----------------------------------------------------------------------------
// ���[�h�̐ݒ�
//-----------------------------------------------------------------------------
void SetMode(MODE mode)
{
	//���݂̏������I��
	switch (g_mode)
	{
	case MODE_TITLE:
		//�^�C�g���̏I������
		UninitTitle();
		StopSound();
		break;

	case MODE_GAME:
		//�Q�[����ʂ̏I������
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

	//�J�ڌ�̏�����������
	switch (mode)
	{
	case MODE_TITLE:
		//�^�C�g���̏���������
		InitTitle();
		PlaySound(SOUND_LABEL_BGM_TITLE);
		break;

	case MODE_GAME:
		//�Q�[���̏���������
		InitGame();
		PlaySound(SOUND_LABEL_BGM_GAME);
		break;

	case MODE_RESULT:
		//���U���g�̏���������
		InitResult();
		PlaySound(SOUND_LABEL_BGM_RESULT);
		break;

	default:
		break;
	}

	g_mode = mode;
}