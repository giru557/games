//=============================================================================
//
// �����\������ [number.cpp]
// Author: Sota Tomoe
//
//=============================================================================
#include "number.h"
#include "manager.h"
#include "scene2D.h"
#include "textureloader.h"

//*****************************************************************************
// �����\���N���X ( ��{ )
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CNumber::CNumber()
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CNumber::~CNumber()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CNumber::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nInitNum)
{
	m_pos = pos;
	m_size = size;

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL)))
	{
		return E_FAIL;
	}

	// �Ίp���̒���
	m_fLength = sqrtf((m_size.x * m_size.x) + (m_size.y * m_size.y));

	// �Ίp���̊p�x
	m_fAngle = atan2f(-m_size.x, m_size.y);

	// ���_�o�b�t�@�����b�N���ă|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVertex, 0);

	// ���_����ݒ�
	// �ʒu
	m_pVertex[0].pos = D3DXVECTOR3(m_pos.x + sinf(m_fAngle + m_rot.z) * m_fLength / 2, m_pos.y + cosf(m_fAngle + m_rot.z) * m_fLength / 2, 0);
	m_pVertex[1].pos = D3DXVECTOR3(m_pos.x + sinf((-m_fAngle + D3DX_PI) + m_rot.z) * m_fLength / 2, m_pos.y + cosf((-m_fAngle + D3DX_PI) + m_rot.z) * m_fLength / 2, 0);
	m_pVertex[2].pos = D3DXVECTOR3(m_pos.x + sinf(-m_fAngle + m_rot.z) * m_fLength / 2, m_pos.y + cosf(-m_fAngle + m_rot.z) * m_fLength / 2, 0);
	m_pVertex[3].pos = D3DXVECTOR3(m_pos.x + sinf(m_fAngle + D3DX_PI + m_rot.z) * m_fLength / 2, m_pos.y + cosf(m_fAngle + D3DX_PI + m_rot.z) * m_fLength / 2, 0);

	// rhw
	m_pVertex[0].rhw = 1.0f;
	m_pVertex[1].rhw = 1.0f;
	m_pVertex[2].rhw = 1.0f;
	m_pVertex[3].rhw = 1.0f;

	// �F
	m_pVertex[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_pVertex[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_pVertex[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_pVertex[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`��
	m_pVertex[0].tex = D3DXVECTOR2((float)nInitNum / 10.0f, 1.0f);
	m_pVertex[1].tex = D3DXVECTOR2((float)nInitNum / 10.0f, 0.0f);
	m_pVertex[2].tex = D3DXVECTOR2(((float)nInitNum + 1) / 10.0f, 1.0f);
	m_pVertex[3].tex = D3DXVECTOR2(((float)nInitNum + 1) / 10.0f, 0.0f);

	// ���_�o�b�t�@�A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CNumber::Uninit(void)
{
	// ���_�o�b�t�@�J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CNumber::Update(void)
{
	// ���_�o�b�t�@�����b�N���ă|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVertex, 0);

	// ���_�ʒu
	m_pVertex[0].pos = D3DXVECTOR3(m_pos.x + sinf(m_fAngle + m_rot.z) * m_fLength / 2, m_pos.y + cosf(m_fAngle + m_rot.z) * m_fLength / 2, 0);
	m_pVertex[1].pos = D3DXVECTOR3(m_pos.x + sinf((-m_fAngle + D3DX_PI) + m_rot.z) * m_fLength / 2, m_pos.y + cosf((-m_fAngle + D3DX_PI) + m_rot.z) * m_fLength / 2, 0);
	m_pVertex[2].pos = D3DXVECTOR3(m_pos.x + sinf(-m_fAngle + m_rot.z) * m_fLength / 2, m_pos.y + cosf(-m_fAngle + m_rot.z) * m_fLength / 2, 0);
	m_pVertex[3].pos = D3DXVECTOR3(m_pos.x + sinf(m_fAngle + D3DX_PI + m_rot.z) * m_fLength / 2, m_pos.y + cosf(m_fAngle + D3DX_PI + m_rot.z) * m_fLength / 2, 0);

	// ���_�o�b�t�@�A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CNumber::Draw(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	pDevice->SetTexture(0, CManager::GetTextureLoad()->m_TextureMp["NUMBER"]);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// �\�����鐔�����w��
//=============================================================================
void CNumber::SetNumber(int nNumber)
{
	// ���_�o�b�t�@�����b�N���ă|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVertex, 0);

	// �e�N�X�`��
	m_pVertex[0].tex = D3DXVECTOR2((float)nNumber / 10.0f, 1.0f);
	m_pVertex[1].tex = D3DXVECTOR2((float)nNumber / 10.0f, 0.0f);
	m_pVertex[2].tex = D3DXVECTOR2(((float)nNumber + 1) / 10.0f, 1.0f);
	m_pVertex[3].tex = D3DXVECTOR2(((float)nNumber + 1) / 10.0f, 0.0f);

	// ���_�o�b�t�@�A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �����֐�
//=============================================================================
CNumber *CNumber::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nInitNum)
{
	CNumber *pNumber;
	pNumber = new CNumber;
	if (pNumber != NULL)
	{
		pNumber->Init(pos, size, nInitNum);
	}

	return pNumber;
}