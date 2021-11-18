#define _CRT_SECURE_NO_WARNINGS
//=============================================================================
//
// ���C������ [main.h]
// Author : Sota Tomoe
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include <windows.h>
#include <vector>
#include <unordered_map>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800)
#include "dinput.h"
#include "xaudio2.h"

using namespace std;

//*****************************************************************************
// ���C�u�����t�@�C���̃����N
//*****************************************************************************
#pragma comment(lib, "d3d9.lib")	// �`�揈���ɕK�v
#pragma comment(lib, "d3dx9.lib")	// �g�����C�u����
#pragma comment(lib, "dxguid.lib")	// �R���|�[�l���g�g�p�ɕK�v
#pragma comment(lib, "dinput8.lib")	// ���͏����ɕK�v
#pragma comment(lib, "winmm.lib")	// �����̕\���ɕK�v

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2)	// ���_�t�H�[�}�b�g3D
#define SCREEN_WIDTH (1280)			// �X�N���[���̕�	
#define SCREEN_HEIGHT (720)			// �X�N���[���̍���
#define SCREEN_CENTER (D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2))	// �X�N���[���̒��S
#define WINDOW_POS_X (1)			// �E�B���h�E����X���W
#define WINDOW_POS_Y (1)			// �E�B���h�E����Y���W
#define VECTOR3_ZERO (D3DXVECTOR3(0,0,0))

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
int GetFPS(void);

#endif