//-----------------------------------------------------------------------------
//
// �����L���O�̏��� [leaderboard.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "leaderboard.h"
#include "input.h"
#include "fade.h"

//-----------------------------------------------------------------------------
// �}�N��
//-----------------------------------------------------------------------------
#define RANKING_POLYGON (1)

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffRanking = NULL;							//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureRanking[RANKING_POLYGON] = { NULL };			//�|���S���̃e�N�X�`���ւ̃|�C���^

//-----------------------------------------------------------------------------
// �����L���O�̏���������
//-----------------------------------------------------------------------------
HRESULT InitRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\ranking.png", &g_pTextureRanking[0]);


	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * RANKING_POLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffRanking, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVertex, 0);

	for (int nCntT = 0; nCntT < RANKING_POLYGON; nCntT++)
	{
		//���_���W D3DXVECTOR3(X,Y, 0.0f);
		pVertex[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);		 //TRIANGLESTRIP�Ŏl�p
		pVertex[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVertex[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		pVertex[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

		//rhw�̐ݒ�
		pVertex[0].rhw = 1.0f;	//1.0f�Œ�
		pVertex[1].rhw = 1.0f;
		pVertex[2].rhw = 1.0f;
		pVertex[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVertex[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);  //D3DCOLOR_RGBA(��, ��, ��, �����x);
		pVertex[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVertex[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVertex[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//���_���̐ݒ�
		pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVertex[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVertex[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVertex += 4; //���_���炵
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRanking->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
// �����L���O�̏I������
//-----------------------------------------------------------------------------
void UninitRanking(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffRanking != NULL)
	{
		g_pVtxBuffRanking->Release();
		g_pVtxBuffRanking = NULL;
	}

	for (int nCntTex = 0; nCntTex < RANKING_POLYGON; nCntTex++)
	{
		//�e�N�X�`���̊J��
		if (g_pTextureRanking[nCntTex] != NULL)
		{
			g_pTextureRanking[nCntTex]->Release();
			g_pTextureRanking[nCntTex] = NULL;
		}
	}

}

//-----------------------------------------------------------------------------
// �����L���O�̍X�V����
//-----------------------------------------------------------------------------
void UpdateRanking(void)
{
	VERTEX_2D *pVertex;

	if (g_pVtxBuffRanking != NULL)
	{
		//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
		g_pVtxBuffRanking->Lock(0, 0, (void**)&pVertex, 0);

		for (int nCntT = 0; nCntT < RANKING_POLYGON; nCntT++)
		{
			pVertex += 4;
		}

		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffRanking->Unlock();
	}

	if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_BACKSPACE) == true)
	{
		SetFade(FADE_OUT, MODE_TITLE);
	}
}

//-----------------------------------------------------------------------------
// �����L���O�̕`�揈��
//-----------------------------------------------------------------------------
void DrawRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRanking, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (int nCntTex = 0; nCntTex < RANKING_POLYGON; nCntTex++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureRanking[nCntTex]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTex * 4, 2);
	}
}