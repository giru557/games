//-----------------------------------------------------------------------------
//
// �o���A�̏��� [barrier.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "barrier.h"
#include "player.h"

//-----------------------------------------------------------------------------
//�}�N��
//-----------------------------------------------------------------------------
#define BARRIER_SIZE (100)

//-----------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffBarrier = NULL;  //���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureBarrier = NULL;		//�|���S���̃e�N�X�`���ւ̃|�C���^
bool g_bBought;										//�w���ς݂�
bool g_bActive;										//�o���A���L�����ǂ���
D3DXVECTOR3 g_PosBarrier;							//�o���A�̈ʒu
int g_nCounterBarrier;								//�o���A�̃J�E���^

//-----------------------------------------------------------------------------
//Barrier�̏���������
//-----------------------------------------------------------------------------
HRESULT InitBarrier(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	g_bBought = false;
	g_bActive = false;
	g_nCounterBarrier = 0;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\barrier.png", &g_pTextureBarrier);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBarrier, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffBarrier->Lock(0, 0, (void**)&pVertex, 0);

	//���_���W D3DXVECTOR3(X,Y, 0.0f);
	pVertex[0].pos = D3DXVECTOR3(0, 0, 0.0f);		 //TRIANGLESTRIP�Ŏl�p
	pVertex[1].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVertex[2].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVertex[3].pos = D3DXVECTOR3(0, 0, 0.0f);

	//rhw�̐ݒ�
	pVertex[0].rhw = 1.0f;	//1.0f�Œ�
	pVertex[1].rhw = 1.0f;
	pVertex[2].rhw = 1.0f;
	pVertex[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVertex[0].col = D3DCOLOR_RGBA(255, 255, 255, 128);  //D3DCOLOR_RGBA(��, ��, ��, �����x);
	pVertex[1].col = D3DCOLOR_RGBA(255, 255, 255, 128);
	pVertex[2].col = D3DCOLOR_RGBA(255, 255, 255, 128);
	pVertex[3].col = D3DCOLOR_RGBA(255, 255, 255, 128);

	//���_���̐ݒ�
	pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBarrier->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//Barrier�̏I������
//-----------------------------------------------------------------------------
void UninitBarrier(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffBarrier != NULL)
	{
		g_pVtxBuffBarrier->Release();
		g_pVtxBuffBarrier = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pTextureBarrier != NULL)
	{
		g_pTextureBarrier->Release();
		g_pTextureBarrier = NULL;
	}
}

//-----------------------------------------------------------------------------
//Barrier�̍X�V����
//-----------------------------------------------------------------------------
void UpdateBarrier(void)
{
	VERTEX_2D *pVertex;
	PLAYER *pPlayer;

	pPlayer = GetPlayer();
	g_PosBarrier = pPlayer->pos;

	if (g_pVtxBuffBarrier != NULL)
	{
		//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
		g_pVtxBuffBarrier->Lock(0, 0, (void**)&pVertex, 0);

		//���_���W�̍X�V
		pVertex[0].pos = D3DXVECTOR3(g_PosBarrier.x - BARRIER_SIZE, g_PosBarrier.y + BARRIER_SIZE, 0.0f);
		pVertex[1].pos = D3DXVECTOR3(g_PosBarrier.x - BARRIER_SIZE, g_PosBarrier.y - BARRIER_SIZE, 0.0f);
		pVertex[2].pos = D3DXVECTOR3(g_PosBarrier.x + BARRIER_SIZE, g_PosBarrier.y + BARRIER_SIZE, 0.0f);
		pVertex[3].pos = D3DXVECTOR3(g_PosBarrier.x + BARRIER_SIZE, g_PosBarrier.y - BARRIER_SIZE, 0.0f);

		if (g_bActive == false)
		{
			//���_�J���[�̐ݒ�
			pVertex[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);  //D3DCOLOR_RGBA(��, ��, ��, �����x);
			pVertex[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
			pVertex[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
			pVertex[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
		}
		else
		{
			//���_�J���[�̐ݒ�
			pVertex[0].col = D3DCOLOR_RGBA(255, 255, 255, 128);  //D3DCOLOR_RGBA(��, ��, ��, �����x);
			pVertex[1].col = D3DCOLOR_RGBA(255, 255, 255, 128);
			pVertex[2].col = D3DCOLOR_RGBA(255, 255, 255, 128);
			pVertex[3].col = D3DCOLOR_RGBA(255, 255, 255, 128);
		}

		if (g_bBought == true)
		{
			if (g_bActive == false)
			{
				g_nCounterBarrier++;

				if (g_nCounterBarrier % 300 == 0)
				{
					g_bActive = true;
					g_nCounterBarrier = 0;
				}
			}
		}

		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffBarrier->Unlock();
	}
}

//-----------------------------------------------------------------------------
//Barrier�̕`�揈��
//-----------------------------------------------------------------------------
void DrawBarrier(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBarrier, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBarrier);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 4);
}

//-----------------------------------------------------------------------------
//�Z�b�g�o���A
//-----------------------------------------------------------------------------
void SetBarrier(void)
{
	g_bActive = true;
	g_bBought = true;
}

//-----------------------------------------------------------------------------
//�o���A�폜
//-----------------------------------------------------------------------------
void RemoveBarrier(void)
{
	g_bActive = false;
	g_bBought = false;
}

//-----------------------------------------------------------------------------
//�o���A�j��
//-----------------------------------------------------------------------------
void BreakBarrier(void)
{
	g_bActive = false;
}
//-----------------------------------------------------------------------------
//�o���A�`�F�b�N
//-----------------------------------------------------------------------------
bool CheckBarrier(void)
{
	if (g_bBought == true)
	{
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
//�o���A�L���`�F�b�N
//-----------------------------------------------------------------------------
bool CheckBarrierActive(void)

{
	if (g_bActive == true)
	{
		return true;
	}

	return false;
}