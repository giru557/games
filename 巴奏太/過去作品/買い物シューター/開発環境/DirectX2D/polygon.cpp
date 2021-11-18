/*#include "polygon.h"

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9  g_pVtxBuffPolygon = NULL;  //���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTexturePolygon = NULL;		//�|���S���̃e�N�X�`���ւ̃|�C���^
int g_nCounterAnim;									//�A�j���[�V�����J�E���^�[
int g_nPatternAnim;									//�A�j���[�V�����p�^�[���i���o�[
D3DXVECTOR3 g_posPolygon;							//�|���S���̈ʒu
D3DXVECTOR3 g_rotPolygon;							//�|���S���̉�]�p
float g_fLengthPolygon;								//�|���S���̑Ίp���̒���
float g_fAnglePolygon;								//�|���S���̑Ίp���̊p�x
D3DXVECTOR3 g_movePolygon;							//�ړ���

//-----------------------------------------------------------------------------
//�|���S���̏���������
//-----------------------------------------------------------------------------
HRESULT InitPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVertex;
	
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\runningman000.png", &g_pTexturePolygon);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_AMOUNT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPolygon, NULL)))
	{
		return E_FAIL;
	}

	g_nCounterAnim = 0;	//�A�j���[�V�����J�E���^�[������
	g_nPatternAnim = 0;	//�A�j���[�V�����p�^�[���i���o�[������

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVertex, 0);

	g_posPolygon = D3DXVECTOR3(640, 400, 0.0f);

	//���_���W D3DXVECTOR3(X,Y, 0.0f);
	pVertex[0].pos = D3DXVECTOR3(POLYGON_X_LEFT, POLYGON_Y_BOTTOM, 0.0f);		 //TRIANGLESTRIP�Ŏl�p
	pVertex[1].pos = D3DXVECTOR3(POLYGON_X_LEFT, POLYGON_Y_UPPER, 0.0f);
	pVertex[2].pos = D3DXVECTOR3(POLYGON_X_RIGHT, POLYGON_Y_BOTTOM, 0.0f);
	pVertex[3].pos = D3DXVECTOR3(POLYGON_X_RIGHT, POLYGON_Y_UPPER, 0.0f);

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

	//UV�̐ݒ�
	pVertex[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVertex[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVertex[2].tex = D3DXVECTOR2(0.125f, 1.0f);
	pVertex[3].tex = D3DXVECTOR2(0.125f, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPolygon->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//�|���S���̏I������
//-----------------------------------------------------------------------------
void UninitPolygon(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pTexturePolygon != NULL)
	{
		g_pTexturePolygon->Release(); 
		g_pTexturePolygon = NULL;
	}
}

//-----------------------------------------------------------------------------
//�|���S���̍X�V����
//-----------------------------------------------------------------------------
void UpdatePolygon(void)
{
	VERTEX_2D *pVertex;
	g_nCounterAnim++; //�A�j���[�V�����J�E���^�[�X�V (�J�E���g�A�b�v)

	 //���_�o�b�t�@�����b�N���Ē��_���ւ̃|�C���^���擾
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVertex, 0);

	//ANIMATION_INTERVAL�̐��Ŋ���؂ꂽ��
	if (g_nCounterAnim % ANIMATION_INTERVAL == 0)
	{
		g_nPatternAnim = (g_nPatternAnim + 1) % 8; //8�܂ł����Ƃ��܂�0���������ă��Z�b�g

		//UV�ݒ�
		pVertex[0].tex = D3DXVECTOR2((float)g_nPatternAnim * 0.125f, 1.0f);
		pVertex[1].tex = D3DXVECTOR2((float)g_nPatternAnim * 0.125f, 0.0f);
		pVertex[2].tex = D3DXVECTOR2(((float)g_nPatternAnim + 1) * 0.125f, 1.0f);
		pVertex[3].tex = D3DXVECTOR2(((float)g_nPatternAnim + 1) * 0.125f, 0.0f);

	}

	////�|���S���̉�]
	//if (GetKeyboardPress(DIK_F) == true)
	//{
	//	g_rotPlayer.z += 0.1f;
	//	if (g_rotPlayer.z > D3DX_PI)
	//	{
	//		g_rotPlayer.z -= D3DX_PI * 2.0f;
	//	}
	//}

	//�L�[���͂ňړ�
	//�v����
	if (GetKeyboardPress(DIK_D) == true)
	{
		if (GetKeyboardPress(DIK_W) == true)
		{
			g_movePolygon.x += sinf(D3DX_PI * 0.75f) * 3;
			g_movePolygon.y += cosf(D3DX_PI * 0.75f) * 3;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{
			g_movePolygon.x += sinf(D3DX_PI * 0.25f) * 3;
			g_movePolygon.y += cosf(D3DX_PI * 0.25f) * 3;
		}
		else
		{
			g_movePolygon.x += sinf(D3DX_PI * 0.5f) * 3;
		}
	}
	else if (GetKeyboardPress(DIK_A) == true)
	{
		if (GetKeyboardPress(DIK_W) == true)
		{
			g_movePolygon.x += sinf(-D3DX_PI * 0.75f) * 3;
			g_movePolygon.y += cosf(-D3DX_PI * 0.75f) * 3;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{
			g_movePolygon.x += sinf(-D3DX_PI * 0.25f) * 3;
			g_movePolygon.y += cosf(-D3DX_PI * 0.25f) * 3;
		}
		else
		{
			g_movePolygon.x += sinf(-D3DX_PI * 0.25f) * 3;
		}
	}
	else if (GetKeyboardPress(DIK_W) == true)
	{
		if (GetKeyboardPress(DIK_S) == true)
		{
			g_movePolygon.y += cosf(D3DX_PI) * 0;
		}
		else
		{
			g_movePolygon.y += cosf(D3DX_PI) * 3;
		}
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{
		if (GetKeyboardPress(DIK_W) == true)
		{
			g_movePolygon.y += cosf(D3DX_PI) * 0;
		}
		else
		{
			g_movePolygon.y += cosf(0) * 3;
		}
	}

	if (GetKeyboardPress(DIK_F) == true)
	{
		g_rotPolygon.z += 0.1f;
		if (g_rotPolygon.z > D3DX_PI)
		{
			g_rotPolygon.z -= D3DX_PI * 2.0f;
		}
	}

	//�|���S���̈ʒu�X�V
	g_posPolygon.x += g_movePolygon.x;
	g_posPolygon.y += g_movePolygon.y;

	//�ړ��ʂ̌���
	g_movePolygon.x += (0 - g_movePolygon.x) * 0.2f;
	g_movePolygon.y += (0 - g_movePolygon.y) * 0.2f;

	//�Ίp���̒���
	g_fLengthPolygon = sqrtf((40) * (40) + (-60) * (-60));

	//�Ίp���̊p�x
	g_fAnglePolygon = atan2f(80, -120);

	//���_���W�̍X�V
	pVertex[0].pos.x = g_posPolygon.x + sinf(g_rotPolygon.z + (g_fAnglePolygon + D3DX_PI)) * g_fLengthPolygon;
	pVertex[0].pos.y = g_posPolygon.y + cosf(g_rotPolygon.z + (g_fAnglePolygon + D3DX_PI)) * g_fLengthPolygon;
	pVertex[0].pos.z = 0.0f;

	pVertex[1].pos.x = g_posPolygon.x + sinf(g_rotPolygon.z + (g_fAnglePolygon + D3DX_PI / 3)) * g_fLengthPolygon;
	pVertex[1].pos.y = g_posPolygon.y + cosf(g_rotPolygon.z + (g_fAnglePolygon + D3DX_PI / 3)) * g_fLengthPolygon;
	pVertex[1].pos.z = 0.0f;

	pVertex[2].pos.x = g_posPolygon.x + sinf(g_rotPolygon.z + (g_fAnglePolygon + -D3DX_PI / 2)) * g_fLengthPolygon;
	pVertex[2].pos.y = g_posPolygon.y + cosf(g_rotPolygon.z + (g_fAnglePolygon + -D3DX_PI / 2)) * g_fLengthPolygon;
	pVertex[2].pos.z = 0.0f;

	pVertex[3].pos.x = g_posPolygon.x + sinf(g_rotPolygon.z + g_fAnglePolygon) * g_fLengthPolygon;
	pVertex[3].pos.y = g_posPolygon.y + cosf(g_rotPolygon.z + g_fAnglePolygon) * g_fLengthPolygon;
	pVertex[3].pos.z = 0.0f;

	//pVertex[3].pos = D3DXVECTOR3(sinf(g_fAnglePolygon) * g_fLengthPolygon, cosf(g_fAnglePolygon) * g_fLengthPolygon, 0.0f);
	//pVertex[0].pos = D3DXVECTOR3(-40 + g_posPolygon.x, 60 + g_posPolygon.y, 0.0f);
	//pVertex[1].pos = D3DXVECTOR3(-40 + g_posPolygon.x, -60 + g_posPolygon.y, 0.0f);
	//pVertex[2].pos = D3DXVECTOR3(40 + g_posPolygon.x, 60 + g_posPolygon.y, 0.0f);
	//pVertex[3].pos = D3DXVECTOR3(40 + g_posPolygon.x, -60 + g_posPolygon.y, 0.0f);

	if (GetKeyboardTrigger(DIK_SPACE) == true)
	{
		D3DXVECTOR3 BulletMove;
		BulletMove = D3DXVECTOR3(0.0f, cosf(D3DX_PI) * 10, 0.0f);
		BulletMove.y += BulletMove.y;
		SetBullet(g_posPolygon, BulletMove, 50);
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffPolygon->Unlock();
}

//-----------------------------------------------------------------------------
//�|���S���̕`�揈��
//-----------------------------------------------------------------------------
void DrawPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturePolygon);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}*/