//-----------------------------------------------------------------------------
//
// ���͂̏��� [input.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "input.h"

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECTINPUT8 g_pInput = NULL;													//DirectInput�I�u�W�F�N�g�̃|�C���^
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;										//���̓f�o�C�X�i�L�[�{�[�h�j�ւ̃|�C���^
LPDIRECTINPUTDEVICE8 g_pDevMouse = NULL;										//�}�E�X�̃f�o�C�X�ւ̃|�C���^
DIMOUSESTATE g_MouseState;														//�}�E�X�̏��
BYTE g_aKeyState[KEY_STATE];													//���͂��ꂽ�L�[�̔���
BYTE g_aKeyStateTrigger[KEY_STATE];												//���͂��ꂽ�L�[�̔���(�g���K�[)
BYTE g_aKeyStateRelease[KEY_STATE];												//���͂��ꂽ�L�[�̔���(�����[�X)
BYTE g_aKeyStateRepeat[KEY_STATE];												//���͂��ꂽ�L�[�̔���(���s�[�g)
BYTE g_aCountRepeat[256];														//���s�[�g����ŕK�v�ȃJ�E���^�[


//-----------------------------------------------------------------------------
// ����������
//-----------------------------------------------------------------------------
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	//���̓f�o�C�X�i�L�[�{�[�h�j�̐���
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//�}�E�X�̓��̓f�o�C�X����
	if (FAILED(g_pInput->CreateDevice(GUID_SysMouse, &g_pDevMouse, NULL)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g�̐ݒ�i�L�[�{�[�h�j
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g�̐ݒ�i�}�E�X�j
	if (FAILED(g_pDevMouse->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	//�������[�h�̐ݒ�i�L�[�{�[�h�j
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//�������[�h�̐ݒ�i�}�E�X�j
	if (FAILED(g_pDevMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
	{
		return E_FAIL;
	}

	//�A�N�Z�X���̎擾
	g_pDevKeyboard->Acquire();
	g_pDevMouse->Acquire();

	return S_OK;
}

//-----------------------------------------------------------------------------
// �I������
//-----------------------------------------------------------------------------
void UninitKeyboard(void)
{
	if (g_pDevKeyboard != NULL)
	{
		//�A�N�Z�X���̔j��
		g_pDevKeyboard->Unacquire();

		//�J��
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//-----------------------------------------------------------------------------
// �X�V����
//-----------------------------------------------------------------------------
void UpdateKeyboard(void)
{
	BYTE aKeyState[KEY_STATE];

	//�f�o�C�X����f�[�^���擾
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nKey = 0; nKey < KEY_STATE; nKey++)
		{
			g_aKeyStateTrigger[nKey] = aKeyState[nKey] & (g_aKeyState[nKey] ^ aKeyState[nKey]);
			g_aKeyStateRelease[nKey] = (~aKeyState[nKey]) & g_aKeyState[nKey];
			g_aKeyState[nKey] = aKeyState[nKey];
		}
	}
	else
	{
		//�A�N�Z�X���̎擾
		g_pDevKeyboard->Acquire();
	}

	//�f�[�^�擾
	if (SUCCEEDED(g_pDevMouse->GetDeviceState(sizeof(DIMOUSESTATE), &g_MouseState)))
	{
	}
	else
	{
		g_pDevMouse->Acquire();
	}
}

//-----------------------------------------------------------------------------
// �L�[�{�[�h���̎擾
//-----------------------------------------------------------------------------
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//-----------------------------------------------------------------------------
// �L�[�{�[�h���̎擾 (�g���K�[)
//-----------------------------------------------------------------------------
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//-----------------------------------------------------------------------------
// �L�[�{�[�h���̎擾 (�����[�X)
//-----------------------------------------------------------------------------
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//-----------------------------------------------------------------------------
// �L�[�{�[�h���̎擾 (���s�[�g)
//-----------------------------------------------------------------------------
bool GetKeyboardRepeat(int nKey)
{
	if (g_aKeyStateTrigger[nKey] & 0x80)
	{
		return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
	}
	if ((g_aKeyState[nKey] & 0x80))
	{
		g_aCountRepeat[nKey]++;
		if (g_aCountRepeat[nKey] > 20)
		{
			while (g_aKeyState[nKey] & 0x80)
			{
				return (g_aKeyState[nKey] & 0x80) ? true : false;
			}
		}
	}
	if (g_aKeyStateRelease[nKey] & 0x80)
	{
		g_aCountRepeat[nKey] = 0;
	}
	return NULL;
}

//-----------------------------------------------------------------------------
// �}�E�X�̈ړ��ʂ��擾
//-----------------------------------------------------------------------------
D3DXVECTOR2 GetMouseVelocity(void)
{
	return D3DXVECTOR2((float)g_MouseState.lX, (float)g_MouseState.lY);
}