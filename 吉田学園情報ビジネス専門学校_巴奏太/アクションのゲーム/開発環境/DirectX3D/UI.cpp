//-----------------------------------------------------------------------------
//
// UI�̏��� [UI.cpp]
// Author: Souta Tomoe
//
//-----------------------------------------------------------------------------
#include "UI.h"
#include "player.h"

//-----------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
HRESULT InitHPBar(void);			// HP�o�[
void UninitHPBar(void);				//
void UpdateHPBar(void);				//
void DrawHPBar(void);				//
HRESULT InitGameOver(void);			// �Q�[���I�[�o�[�\��
void UninitGameOver(void);			//
void UpdateGameOver(void);			//
void DrawGameOver(void);			//
HRESULT InitEnemyRemaining(void);	// �G�̎c�萔
void UninitEnemyRemaining(void);	//
void UpdateEnemyRemaining(void);	//
void DrawEnemyRemaining(void);		//

//-----------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------
HPBAR g_aHPBar[HEALTH_OBJ_AMOUNT];		// �̗̓o�[�̏��
GAMEOVER g_gameover;				// �Q�[���I�[�o�[�\���̏��
EnemyUI g_enemyUI;					// �G�̎c�萔�̏��

//-----------------------------------------------------------------------------
//UI�̏���������
//-----------------------------------------------------------------------------
HRESULT InitUI(void)
{
	//HP�o�[�̏�����
	HRESULT hrHPBar = InitHPBar();

	//�Q�[���I�[�o�[�̏�����
	HRESULT hrGameOver = InitGameOver();

	//�G�̎c�萔�̏�����
	//HRESULT hrEnemyRemaining = InitEnemyRemaining();

	return S_OK;
}

//-----------------------------------------------------------------------------
//UI�̏I������
//-----------------------------------------------------------------------------
void UninitUI(void)
{
	//�G�̎c�萔�̏I������
	//UninitEnemyRemaining();

	//�Q�[���I�[�o�[�̏I������
	UninitGameOver();

	//HP�o�[�̏I������
	UninitHPBar();
}

//-----------------------------------------------------------------------------
//UI�̍X�V����
//-----------------------------------------------------------------------------
void UpdateUI(void)
{
	//HP�o�[�̍X�V����
	UpdateHPBar();

	//�Q�[���I�[�o�[�̍X�V����
	UpdateGameOver();

	//�G�̎c�萔�̍X�V����
	//UpdateEnemyRemaining();
}

//-----------------------------------------------------------------------------
//UI�̕`�揈��
//-----------------------------------------------------------------------------
void DrawUI(void)
{
	//HP�o�[�̕`�揈��
	DrawHPBar();

	//�Q�[���I�[�o�[�̕`�揈��
	DrawGameOver();

	//�G�̎c�萔�̕`�揈��
	//DrawEnemyRemaining();
}

//-----------------------------------------------------------------------------
// HP�o�[�̏��擾
//-----------------------------------------------------------------------------
HPBAR *GetHPBAR(void)
{
	return &g_aHPBar[0];
}

//-----------------------------------------------------------------------------
// �Q�[���I�[�o�[��ʂ̏��擾
//-----------------------------------------------------------------------------
GAMEOVER *GetGameOver(void)
{
	return &g_gameover;
}

//-----------------------------------------------------------------------------
//HP�o�[�̏�����
HRESULT InitHPBar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�擾

	//������
	g_aHPBar[0].pos = D3DXVECTOR3((200), 70, 0.0f);
	g_aHPBar[1].pos = D3DXVECTOR3((105), 70, 0.0f);

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFileEx(pDevice, "data\\TEXTURE\\health_bar_decoration.png", 66, 19, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0xFF000000, NULL, NULL, &g_aHPBar[0].pTexture);
	D3DXCreateTextureFromFileEx(pDevice, "data\\TEXTURE\\health_bar.png", 51, 19, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0xFF000000, NULL, NULL, &g_aHPBar[1].pTexture);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_aHPBar[0].pVtxBuff, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVertexBack;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_aHPBar[0].pVtxBuff->Lock(0, 0, (void**)&pVertexBack, 0);

	//���_���W D3DXVECTOR3(X,Y, 0.0f);
	pVertexBack[0].pos = D3DXVECTOR3(g_aHPBar[0].pos.x - HEALTH_BAR_WIDTH, g_aHPBar[1].pos.y + HEALTH_BAR_HEIGHT, 0.0f);		 //TRIANGLESTRIP�Ŏl�p
	pVertexBack[1].pos = D3DXVECTOR3(g_aHPBar[0].pos.x - HEALTH_BAR_WIDTH, g_aHPBar[1].pos.y - HEALTH_BAR_HEIGHT, 0.0f);
	pVertexBack[2].pos = D3DXVECTOR3(g_aHPBar[0].pos.x + HEALTH_BAR_WIDTH, g_aHPBar[1].pos.y + HEALTH_BAR_HEIGHT, 0.0f);
	pVertexBack[3].pos = D3DXVECTOR3(g_aHPBar[0].pos.x + HEALTH_BAR_WIDTH, g_aHPBar[1].pos.y - HEALTH_BAR_HEIGHT, 0.0f);

	//rhw�̐ݒ�
	for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertexBack[nCntVtx].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertexBack[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//���_���̐ݒ�
	pVertexBack[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVertexBack[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVertexBack[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVertexBack[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_aHPBar[0].pVtxBuff->Unlock();

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_aHPBar[1].pVtxBuff, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVertexFront;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_aHPBar[1].pVtxBuff->Lock(0, 0, (void**)&pVertexFront, 0);

	//���_���W D3DXVECTOR3(X,Y, 0.0f);
	pVertexFront[0].pos = D3DXVECTOR3(g_aHPBar[1].pos.x - HEALTH_WIDTH, g_aHPBar[1].pos.y + HEALTH_HEIGHT, 0.0f);
	pVertexFront[1].pos = D3DXVECTOR3(g_aHPBar[1].pos.x - HEALTH_WIDTH, g_aHPBar[1].pos.y - HEALTH_HEIGHT, 0.0f);
	pVertexFront[2].pos = D3DXVECTOR3(g_aHPBar[1].pos.x + HEALTH_WIDTH, g_aHPBar[1].pos.y + HEALTH_HEIGHT, 0.0f);
	pVertexFront[3].pos = D3DXVECTOR3(g_aHPBar[1].pos.x + HEALTH_WIDTH, g_aHPBar[1].pos.y - HEALTH_HEIGHT, 0.0f);

	//rhw�̐ݒ�
	for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertexFront[nCntVtx].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertexFront[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//���_���̐ݒ�
	pVertexFront[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVertexFront[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVertexFront[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVertexFront[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//���_�o�b�t�@�A�����b�N
	g_aHPBar[1].pVtxBuff->Unlock();

	return S_OK;
}

//HP�o�[�̏I������
void UninitHPBar(void)
{
	for (int nCntH = 0; nCntH < HEALTH_OBJ_AMOUNT; nCntH++)
	{
		//���_�o�b�t�@�̊J��
		if (g_aHPBar[nCntH].pVtxBuff != NULL)
		{
			g_aHPBar[nCntH].pVtxBuff->Release();
			g_aHPBar[nCntH].pVtxBuff = NULL;
		}

		//�e�N�X�`���̊J��
		if (g_aHPBar[nCntH].pTexture != NULL)
		{
			g_aHPBar[nCntH].pTexture->Release();
			g_aHPBar[nCntH].pTexture = NULL;
		}
	}
}

//HP�o�[�̍X�V����
void UpdateHPBar(void)
{
	VERTEX_2D *pVertex;
	Player *pPlayer = GetPlayer();
	static float fValue = 1.0f;

	//�ő�̗͂ƌ��ݑ̗͂̊���
	fValue = (float)pPlayer->nLife / (float)pPlayer->nLifeMax;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_aHPBar[1].pVtxBuff->Lock(0, 0, (void**)&pVertex, 0);

	////���_���W D3DXVECTOR3(X,Y, 0.0f);
	pVertex[0].pos = D3DXVECTOR3(g_aHPBar[1].pos.x, g_aHPBar[1].pos.y + HEALTH_HEIGHT, 0.0f);		 //TRIANGLESTRIP�Ŏl�p
	pVertex[1].pos = D3DXVECTOR3(g_aHPBar[1].pos.x, g_aHPBar[1].pos.y - HEALTH_HEIGHT, 0.0f);
	pVertex[2].pos = D3DXVECTOR3(g_aHPBar[1].pos.x + (HEALTH_WIDTH * fValue), g_aHPBar[1].pos.y + HEALTH_HEIGHT, 0.0f);
	pVertex[3].pos = D3DXVECTOR3(g_aHPBar[1].pos.x + (HEALTH_WIDTH * fValue), g_aHPBar[1].pos.y - HEALTH_HEIGHT, 0.0f);

	//���_���̐ݒ�
	pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVertex[2].tex = D3DXVECTOR2(1.0f * fValue, 1.0f);
	pVertex[3].tex = D3DXVECTOR2(1.0f * fValue, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_aHPBar[1].pVtxBuff->Unlock();
}

//HP�o�[�̕`�揈��
void DrawHPBar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�擾

	for (int nCntH = 0; nCntH < HEALTH_OBJ_AMOUNT; nCntH++)
	{
		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_aHPBar[nCntH].pVtxBuff, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_aHPBar[nCntH].pTexture);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 4);
	}
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//�Q�[���I�[�o�[�̏�����
HRESULT InitGameOver(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�擾

	//������
	g_gameover.bDraw = false;
	g_gameover.bGameOver = false;
	g_gameover.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\stage_completedBG.png", &g_gameover.pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\stage_completed.png", &g_gameover.pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, NULL, &g_gameover.pTexture[2]);

	//�w�i�p
	VERTEX_2D *pVertexBG;

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_gameover.pVtxBuff[0], NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_gameover.pVtxBuff[0]->Lock(0, 0, (void**)&pVertexBG, 0);

	//���_���W D3DXVECTOR3(X,Y, 0.0f);
	pVertexBG[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);		 //TRIANGLESTRIP�Ŏl�p
	pVertexBG[1].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVertexBG[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	pVertexBG[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

	//rhw�̐ݒ�
	for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertexBG[nCntVtx].rhw = 1.0f;	// 1.0�Œ�

	//���_�J���[�̐ݒ�
	for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertexBG[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//���_���̐ݒ�
	pVertexBG[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVertexBG[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVertexBG[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVertexBG[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//���_�o�b�t�@�A�����b�N
	g_gameover.pVtxBuff[0]->Unlock();

	return S_OK;
}

//�Q�[���I�[�o�[�̏I������
void UninitGameOver(void)
{
	//���_�o�b�t�@�̔j��
	for (int nCntVtxBuff = 0; nCntVtxBuff < 2; nCntVtxBuff++)
	{
		if (g_gameover.pVtxBuff[nCntVtxBuff] != NULL)
		{
			g_gameover.pVtxBuff[nCntVtxBuff]->Release();
			g_gameover.pVtxBuff[nCntVtxBuff] = NULL;
		}
	}

	//�e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < 3; nCntTex++)
	{
		if (g_gameover.pTexture[nCntTex] != NULL)
		{
			g_gameover.pTexture[nCntTex]->Release();
			g_gameover.pTexture[nCntTex] = NULL;
		}
	}
}

//�Q�[���I�[�o�[�̍X�V����
void UpdateGameOver(void)
{

}

//�Q�[���I�[�o�[�̕`�揈��
void DrawGameOver(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�擾

	if (g_gameover.bDraw == true)
	{
		for (int nCntVtxBuff = 0; nCntVtxBuff < 2; nCntVtxBuff++)
		{
			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_gameover.pVtxBuff[nCntVtxBuff], 0, sizeof(VERTEX_2D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);

			switch (nCntVtxBuff)
			{
			case 0:
				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_gameover.pTexture[0]);
				break;
			case 1:
				if (g_gameover.bGameOver == true)
				{
					//�e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_gameover.pTexture[2]);
				}
				else
				{
					//�e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_gameover.pTexture[1]);
				}
			default:
				break;
			}

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		}
	}
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//�G�c�ʂ̏�����
HRESULT InitEnemyRemaining(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�擾

	//������
	g_enemyUI.pos[0] = D3DXVECTOR3(SCREEN_WIDTH / 2, 0.0f, 0.0f);
	g_enemyUI.pos[1] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number000.png", &g_enemyUI.pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, NULL, &g_enemyUI.pTexture[1]);

	for (int nCntObj = 0; nCntObj < ENEMYUI_OBJ_AMOUNT; nCntObj++)
	{
		//���_�o�b�t�@�̐���
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT * ENEMYUI_SHOWDIGIT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_enemyUI.pVtxBuff[nCntObj], NULL)))
		{
			return E_FAIL;
		}
	}

	//�����p
	VERTEX_2D *pVertexNUM;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_enemyUI.pVtxBuff[0]->Lock(0, 0, (void**)&pVertexNUM, 0);

	for (int nCntNum = 0; nCntNum < ENEMYUI_SHOWDIGIT; nCntNum++, pVertexNUM += 4)
	{
		//���_���W�̐ݒ�
		pVertexNUM[0].pos = D3DXVECTOR3((g_enemyUI.pos[0].x) + nCntNum * ENEMYUI_SPACE_BETWEEN_CHAR, g_enemyUI.pos[0].y + ENEMYUI_CHAR_HEIGHT, 0.0f);
		pVertexNUM[1].pos = D3DXVECTOR3((g_enemyUI.pos[0].x) + nCntNum * ENEMYUI_SPACE_BETWEEN_CHAR, g_enemyUI.pos[0].y, 0.0f);
		pVertexNUM[2].pos = D3DXVECTOR3((g_enemyUI.pos[0].x + ENEMYUI_CHAR_WIDTH) + nCntNum * ENEMYUI_SPACE_BETWEEN_CHAR, g_enemyUI.pos[0].y + ENEMYUI_CHAR_HEIGHT, 0.0f);
		pVertexNUM[3].pos = D3DXVECTOR3((g_enemyUI.pos[0].x + ENEMYUI_CHAR_WIDTH) + nCntNum * ENEMYUI_SPACE_BETWEEN_CHAR, g_enemyUI.pos[0].y, 0.0f);

		//rhw�̐ݒ�
		for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertexNUM[nCntVtx].rhw = 1.0f;	// 1.0f�Œ�

		//���_�J���[�̐ݒ�
		for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertexNUM[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//���_���̐ݒ�
		pVertexNUM[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVertexNUM[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVertexNUM[2].tex = D3DXVECTOR2(1.0f * 0.1f, 1.0f);
		pVertexNUM[3].tex = D3DXVECTOR2(1.0f * 0.1f, 0.0f);
	}

	//�A�����b�N
	g_enemyUI.pVtxBuff[0]->Unlock();

	//�摜�p
	VERTEX_2D *pVertexPIC;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_enemyUI.pVtxBuff[1]->Lock(0, 0, (void**)&pVertexPIC, 0);

	//���_���W�̐ݒ�
	pVertexPIC[0].pos = D3DXVECTOR3(g_enemyUI.pos[1].x, g_enemyUI.pos[1].y + ENEMYUI_PICTURE_HEIGHT, 0.0f);
	pVertexPIC[1].pos = D3DXVECTOR3(g_enemyUI.pos[1].x, g_enemyUI.pos[1].y, 0.0f);
	pVertexPIC[2].pos = D3DXVECTOR3(g_enemyUI.pos[1].x + ENEMYUI_PICTURE_WIDTH, g_enemyUI.pos[1].y + ENEMYUI_PICTURE_HEIGHT, 0.0f);
	pVertexPIC[3].pos = D3DXVECTOR3(g_enemyUI.pos[1].x + ENEMYUI_PICTURE_WIDTH, g_enemyUI.pos[1].y, 0.0f);

	//rhw�ݒ�
	for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertexPIC[nCntVtx].rhw = 1.0f;	// 1.0f�Œ�

	//���_�J���[�̐ݒ�
	for (int nCntVtx = 0; nCntVtx < VERTEX_AMOUNT; nCntVtx++) pVertexPIC[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	
	//���_���̐ݒ�
	pVertexPIC[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVertexPIC[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVertexPIC[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVertexPIC[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//�A�����b�N
	g_enemyUI.pVtxBuff[1]->Unlock();

	return S_OK;
}

//�G�c�ʂ̏I������
void UninitEnemyRemaining(void)
{
	for (int nCntObj = 0; nCntObj < ENEMYUI_OBJ_AMOUNT; nCntObj++)
	{
		//���_�o�b�t�@�̊J��
		if (g_enemyUI.pVtxBuff[nCntObj] != NULL)
		{
			g_enemyUI.pVtxBuff[nCntObj]->Release();
			g_enemyUI.pVtxBuff[nCntObj] = NULL;
		}

		//�e�N�X�`���̊J��
		if (g_enemyUI.pTexture[nCntObj] != NULL)
		{
			g_enemyUI.pTexture[nCntObj]->Release();
			g_enemyUI.pTexture[nCntObj] = NULL;
		}
	}
}

//�G�c�ʂ̍X�V����
void UpdateEnemyRemaining(void)
{
	VERTEX_2D *pVertexNUM;				// �����p
	int aNumAmount[ENEMYUI_SHOWDIGIT];	// �ꕶ�����z���

	//���Ԃ𕪉�
	aNumAmount[0] = g_enemyUI.nAmount % 100 / 10;
	aNumAmount[1] = g_enemyUI.nAmount % 10 / 1;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_enemyUI.pVtxBuff[0]->Lock(0, 0, (void**)&pVertexNUM, 0);

	for (int nCntNum = 0; nCntNum < ENEMYUI_SHOWDIGIT; nCntNum++)
	{
		//���_���W D3DXVECTOR3(X,Y, 0.0f);
		pVertexNUM[0].pos = D3DXVECTOR3((g_enemyUI.pos[0].x) + (nCntNum + 1) * ENEMYUI_SPACE_BETWEEN_CHAR, (g_enemyUI.pos[0].y + ENEMYUI_CHAR_HEIGHT), 0.0f);		 //TRIANGLESTRIP�Ŏl�p
		pVertexNUM[1].pos = D3DXVECTOR3((g_enemyUI.pos[0].x) + (nCntNum + 1) * ENEMYUI_SPACE_BETWEEN_CHAR, (g_enemyUI.pos[0].y), 0.0f);
		pVertexNUM[2].pos = D3DXVECTOR3((g_enemyUI.pos[0].x + ENEMYUI_CHAR_WIDTH) + (nCntNum + 1) * ENEMYUI_SPACE_BETWEEN_CHAR, (g_enemyUI.pos[0].y + ENEMYUI_CHAR_HEIGHT), 0.0f);
		pVertexNUM[3].pos = D3DXVECTOR3((g_enemyUI.pos[0].x + ENEMYUI_CHAR_WIDTH) + (nCntNum + 1) * ENEMYUI_SPACE_BETWEEN_CHAR, (g_enemyUI.pos[0].y), 0.0f);

		//�e�N�X�`�����W�̍X�V
		pVertexNUM[0].tex = D3DXVECTOR2(0.1f * (aNumAmount[nCntNum]), 1.0f);
		pVertexNUM[1].tex = D3DXVECTOR2(0.1f * (aNumAmount[nCntNum]), 0.0f);
		pVertexNUM[2].tex = D3DXVECTOR2(0.1f * (aNumAmount[nCntNum] + 1), 1.0f);
		pVertexNUM[3].tex = D3DXVECTOR2(0.1f * (aNumAmount[nCntNum] + 1), 0.0f);
	}

	//���_�o�b�t�@���A�����b�N����
	g_enemyUI.pVtxBuff[0]->Unlock();

	g_enemyUI.nAmount++;
	if (g_enemyUI.nAmount >= 100) g_enemyUI.nAmount = 0;
}

//�G�c�ʂ̕`�揈��
void DrawEnemyRemaining(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�擾

	for (int nCntObj = 0; nCntObj < ENEMYUI_OBJ_AMOUNT; nCntObj++)
	{
		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_enemyUI.pVtxBuff[nCntObj], 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		pDevice->SetTexture(0, g_enemyUI.pTexture[nCntObj]);

		//�摜�������ŃX�C�b�`
		switch (nCntObj)
		{
			//����
		case 0:
			for (int nCntNum = 0; nCntNum < ENEMYUI_SHOWDIGIT; nCntNum++)
			{
				//�|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntNum * 4, 2);
			}
			break;

			//�摜
		case 1:
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
			break;

		default:
			break;
		}
	}
}
