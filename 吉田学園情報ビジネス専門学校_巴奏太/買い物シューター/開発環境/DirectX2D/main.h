//-----------------------------------------------------------------------------
//
// DirectX2D�Q�[���̃w�b�_�[�t�@�C�� [main.h]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#ifndef _MAIN_H_
#define _MAIN_H_
#include <windows.h>
#include "d3dx9.h"						//�`�揈���ɕK�v
#define DIRECTINPUT_VERSION (0x0800)
#include "dinput.h"						//���͏����ɕK�v
#include "xaudio2.h"					//�T�E���h�Đ��ɕK�v


//-----------------------------------------------------------------------------
//���C�u�����̃����N
//-----------------------------------------------------------------------------
#pragma comment(lib, "d3d9.lib")	//�`�揈���ɕK�v
#pragma comment(lib, "d3dx9.lib")	//�g�����C�u����
#pragma comment(lib, "dxguid.lib")	//�R���|�[�l���g�g�p�ɕK�v
#pragma comment(lib, "dinput8.lib") //���͏����ɕK�v
#pragma comment(lib, "winmm.lib")	//�����̕\���ɕK�v

//-----------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)  //���_�t�H�[�}�b�g
#define VERTEX_AMOUNT (4)											  //���_�̐�
#define SCREEN_WIDTH  (1280)				//�X�N���[���̕�
#define SCREEN_HEIGHT (720)					//�X�N���[���̍���
#define CLASS_NAME "WindowClass"			//�E�B���h�E�N���X�̖��O
#define WINDOW_NAME "WindowsApp"			//�E�B���h�E�̖��O
#define WINDOW_POS_X (1)					//�E�B���h�E�����X���W
#define WINDOW_POS_Y (1)					//�E�B���h�E�����Y���W

//-----------------------------------------------------------------------------
//�\���̐錾
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	//���_���W
	float rhw;			//1.0�ŌŒ�
	D3DCOLOR col;		//���_�J���[
	D3DXVECTOR2 tex;	//�e�N�X�`�����W
}VERTEX_2D;

//���[�h�̎��
typedef enum
{
	MODE_TITLE = 0,	//�^�C�g�����
	MODE_GAME,		//�Q�[�����
	MODE_RESULT,	//���U���g���
	MODE_MAX
} MODE;

//�v���g�^�C�v�錾
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
MODE GetMode(void);
HWND GetWnd(void);

#endif