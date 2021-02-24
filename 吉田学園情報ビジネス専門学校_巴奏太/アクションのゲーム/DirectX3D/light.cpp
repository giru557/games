//-----------------------------------------------------------------------------
//
// ���C�g�̏��� [light.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "light.h"

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
#define LIGHT_MAX (3)															// ���C�g�̐�

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
D3DLIGHT9 g_light[LIGHT_MAX];		// ���C�g�̏��

//-----------------------------------------------------------------------------
// ���C�g�̏���������
//-----------------------------------------------------------------------------
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�ւ̃|�C���^
	D3DXVECTOR3 vecDir;				// �ݒ�p�����x�N�g��

	//�f�o�C�X�擾
	pDevice = GetDevice();

	//���C�g���N���A����
	ZeroMemory(&g_light, sizeof(D3DLIGHT9));

	for (int nCntLight = 0; nCntLight < LIGHT_MAX; nCntLight++)
	{
		//���C�g�̐ݒ�
		switch (nCntLight)
		{
		case 0:	// ���C�g1
			//���C�g�̎�ނ�I��
			g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			//���C�g�̊g�U����ݒ�
			g_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//���C�g�̕�����ݒ�
			vecDir = D3DXVECTOR3(0.2f, -0.8f, -0.4f);	// ���������O�Ɍ���������
			D3DXVec3Normalize(&vecDir, &vecDir);		// ���K������
			g_light[nCntLight].Direction = vecDir;
			break;

		case 1:	// ���C�g2
			//���C�g�̎�ނ�I��
			g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			//���C�g�̊g�U����ݒ�
			g_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);

			//���C�g�̕�����ݒ�
			vecDir = D3DXVECTOR3(0.0, -1.0f, 0.0f);		// �ォ�牺�Ɍ��������� 
			D3DXVec3Normalize(&vecDir, &vecDir);		// ���K������
			g_light[nCntLight].Direction = vecDir;
			break;

		case 2: // ���C�g3
			//���C�g�̎�ނ�I��
			g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			//���C�g�̊g�U����ݒ�
			g_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

			//���C�g�̕�����ݒ�
			vecDir = D3DXVECTOR3(-0.5f, 0.0f, 0.4f);
			D3DXVec3Normalize(&vecDir, &vecDir);		// ���K������
			g_light[nCntLight].Direction = vecDir;
			break;

		default:
			break;
		}

		//���C�g��ݒ肷��
		pDevice->SetLight(nCntLight, &g_light[nCntLight]);

		//���C�g��L���ɂ���
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//-----------------------------------------------------------------------------
// ���C�g�̏I������
//-----------------------------------------------------------------------------
void UninitLight(void)
{

}

//-----------------------------------------------------------------------------
// ���C�g�̍X�V����
//-----------------------------------------------------------------------------
void UpdateLight(void)
{

}

