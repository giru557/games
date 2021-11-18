//-----------------------------------------------------------------------------
//
// �����̏��� [explosion.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "explosion.h"

//-----------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------
#define MAX_EXPLOSION (256) //�����̍ő吔
#define ANIM_SPLIT (7)		//�A�j���[�V�����̕�����
#define ANIM_UPDATE (5)		//�A�j���[�V�����X�V�p�x

//-----------------------------------------------------------------------------
//�\���̐錾
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	int nSize;				//�����̃T�C�Y
	int nCounterAnim = 0;	//�A�j���[�V�����J�E���^�[������
	int nPatternAnim = 0;	//�A�j���[�V�����p�^�[���i���o�[������
	bool bUse;				//�g�p���Ă��邩
} EXPLOSION;

//-----------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;
EXPLOSION g_aExplosion[MAX_EXPLOSION]; //�����̏��


//-----------------------------------------------------------------------------
//�����̏���������
//-----------------------------------------------------------------------------
HRESULT InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\explosion001.png", &g_pTextureExplosion);

	//EXPLOSION�̏�����
	for (int nCntExp = 0; nCntExp < MAX_EXPLOSION; nCntExp++)
	{
		g_aExplosion[nCntExp].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExp].bUse = false;
		g_aExplosion[nCntExp].nCounterAnim = 0;
		g_aExplosion[nCntExp].nPatternAnim = 0;
	}

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * MAX_EXPLOSION, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffExplosion, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�̃��b�N
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVertex, 0);

	//���_���W
	for (int nCntExp = 0; nCntExp < MAX_EXPLOSION; nCntExp++)
	{
		//���_���W�̐ݒ�
		pVertex[0].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVertex[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVertex[2].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVertex[3].pos = D3DXVECTOR3(0, 0, 0.0f);

		//rhw�̐ݒ�
		pVertex[0].rhw = 1.0f;	//1.0f�Œ�
		pVertex[1].rhw = 1.0f;
		pVertex[2].rhw = 1.0f;
		pVertex[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVertex[0].col = D3DCOLOR_RGBA(255, 165, 0, 255);  //D3DCOLOR_RGBA(��, ��, ��, �����x);
		pVertex[1].col = D3DCOLOR_RGBA(255, 165, 0, 255);
		pVertex[2].col = D3DCOLOR_RGBA(255, 165, 0, 255);
		pVertex[3].col = D3DCOLOR_RGBA(255, 165, 0, 255);

		//UV�̐ݒ�
		pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVertex[2].tex = D3DXVECTOR2(1.0f / ANIM_SPLIT, 1.0f);
		pVertex[3].tex = D3DXVECTOR2(1.0f / ANIM_SPLIT, 0.0f);

		pVertex += 4; //���_�̃|�C���^�����炷
	}
	
	g_pVtxBuffExplosion->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//�����̏I������
//-----------------------------------------------------------------------------
void UninitExplosion(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}
}

//-----------------------------------------------------------------------------
//�����̍X�V����
//-----------------------------------------------------------------------------
void UpdateExplosion(void)
{
	EXPLOSION *pExplosion;
	VERTEX_2D *pVertex;

	pExplosion = &g_aExplosion[0];

	if (g_pVtxBuffExplosion != NULL)
	{
		//���_�o�b�t�@�̃��b�N
		g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVertex, 0);

		for (int nCntExp = 0; nCntExp < MAX_EXPLOSION; nCntExp++, pExplosion++)
		{
			if (pExplosion->bUse == true)
			{
				//���_���W�̐ݒ�
				pVertex[0].pos = D3DXVECTOR3(-g_aExplosion[nCntExp].nSize + g_aExplosion[nCntExp].pos.x, g_aExplosion[nCntExp].nSize + g_aExplosion[nCntExp].pos.y, 0.0f);
				pVertex[1].pos = D3DXVECTOR3(-g_aExplosion[nCntExp].nSize + g_aExplosion[nCntExp].pos.x, -g_aExplosion[nCntExp].nSize + g_aExplosion[nCntExp].pos.y, 0.0f);
				pVertex[2].pos = D3DXVECTOR3(g_aExplosion[nCntExp].nSize + g_aExplosion[nCntExp].pos.x, g_aExplosion[nCntExp].nSize + g_aExplosion[nCntExp].pos.y, 0.0f);
				pVertex[3].pos = D3DXVECTOR3(g_aExplosion[nCntExp].nSize + g_aExplosion[nCntExp].pos.x, -g_aExplosion[nCntExp].nSize + g_aExplosion[nCntExp].pos.y, 0.0f);

				//�A�j���[�V�����J�E���^�[
				g_aExplosion[nCntExp].nCounterAnim++;

				//�A�j���[�V�����̍X�V�p�x
				if (g_aExplosion[nCntExp].nCounterAnim % ANIM_UPDATE == 0)
				{
					//�p�^�[���X�V
					g_aExplosion[nCntExp].nPatternAnim++;

					//UV�ݒ�
					pVertex[0].tex = D3DXVECTOR2((float)g_aExplosion[nCntExp].nPatternAnim * (1.0f / ANIM_SPLIT), 1.0f);
					pVertex[1].tex = D3DXVECTOR2((float)g_aExplosion[nCntExp].nPatternAnim * (1.0f / ANIM_SPLIT), 0.0f);
					pVertex[2].tex = D3DXVECTOR2(((float)g_aExplosion[nCntExp].nPatternAnim + 1) * (1.0f / ANIM_SPLIT), 1.0f);
					pVertex[3].tex = D3DXVECTOR2(((float)g_aExplosion[nCntExp].nPatternAnim + 1) * (1.0f / ANIM_SPLIT), 0.0f);

					//�A�j���[�V�������I������Ƃ�
					//if (g_aExplosion[nCntExp].nCounterAnim > ANIM_UPDATE * ANIM_SPLIT)
					if (g_aExplosion[nCntExp].nPatternAnim >= ANIM_SPLIT)
					{
						//�����̔j��
						pExplosion->bUse = false;

						//�A�j���[�V�����J�E���^�[�̏�����
						g_aExplosion[nCntExp].nCounterAnim = 0;
						g_aExplosion[nCntExp].nPatternAnim = 0;
					}
				}
			}
			pVertex += 4; //���_�̃|�C���^�����炷
		}

		//���_�o�b�t�@�̃A�����b�N
		g_pVtxBuffExplosion->Unlock();
	}
}

//-----------------------------------------------------------------------------
//�����̕`�揈��
//-----------------------------------------------------------------------------
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureExplosion);

	for (int nCntExp = 0; nCntExp < MAX_EXPLOSION; nCntExp++)
	{
		if (g_aExplosion[nCntExp].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExp * 4, 2);
		}
	}
}

//-----------------------------------------------------------------------------
//�����̐ݒ�
//-----------------------------------------------------------------------------
void SetExplosion(D3DXVECTOR3 pos, int nExplosionSize)
{
	EXPLOSION *pExplosion;
	VERTEX_2D *pVertex;

	pExplosion = &g_aExplosion[0];

	//���_�o�b�t�@�̃��b�N
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntExp = 0; nCntExp < MAX_EXPLOSION; nCntExp++, pExplosion++)
	{
		if (pExplosion->bUse == false)
		{
			//�ʒu��ݒ�
			g_aExplosion[nCntExp].pos = pos;

			//�����̃T�C�Y�ݒ�
			g_aExplosion[nCntExp].nSize = nExplosionSize;

			//���_���W��ݒ�
			pVertex[0].pos = D3DXVECTOR3(-g_aExplosion[nCntExp].nSize + g_aExplosion[nCntExp].pos.x, g_aExplosion[nCntExp].nSize + g_aExplosion[nCntExp].pos.y, 0.0f);
			pVertex[1].pos = D3DXVECTOR3(-g_aExplosion[nCntExp].nSize + g_aExplosion[nCntExp].pos.x, -g_aExplosion[nCntExp].nSize + g_aExplosion[nCntExp].pos.y, 0.0f);
			pVertex[2].pos = D3DXVECTOR3(g_aExplosion[nCntExp].nSize + g_aExplosion[nCntExp].pos.x, g_aExplosion[nCntExp].nSize + g_aExplosion[nCntExp].pos.y, 0.0f);
			pVertex[3].pos = D3DXVECTOR3(g_aExplosion[nCntExp].nSize + g_aExplosion[nCntExp].pos.x, -g_aExplosion[nCntExp].nSize + g_aExplosion[nCntExp].pos.y, 0.0f);

			//�g�p���ɕύX
			pExplosion->bUse = true;

			break;

		}
		//���_�̃|�C���^�����炷
		pVertex += 4;
	}
	g_pVtxBuffExplosion->Unlock();
}