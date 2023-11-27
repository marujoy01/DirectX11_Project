#include "..\Public\Camera.h"
#include "Transform.h"

#include "GameInstance.h"

CCamera::CCamera(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CCamera::CCamera(const CCamera & rhs)
	: CGameObject(rhs)
{
}

HRESULT CCamera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera::Initialize(void * pArg) // Ŭ�� ������ �� ����
{
	if (nullptr == pArg)
		return E_FAIL;

	// ���������� �ʿ��� ��Ҹ� �θ� �̸� �����صаɷ� ���� (TransformCom..���)
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	// (����)
	/* ī�޶� ����ü�� ������������ ĳ�����Ͽ� ���� */
	/* ������ ī�޶� ����ü ���� */
	CAMERA_DESC*	pCameraDesc = (CAMERA_DESC*)pArg;	

	// (�� �ޱ�)
	/* �׵������ Ʈ�������� ���� �����ϰ� ���� ī�޶��� ���·� ����ȭ�Ѵ�. */
	/* ī�޶��� Ʈ�������� �׵� ���·� ����������, ���� ���ϴ� ��(ī�޶� ���� ����ü)�� �־� ������ �� �ִ�. */
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&pCameraDesc->vEye));	// ��ġ : ī�޶��� ��ġ
	m_pTransformCom->Look_At(XMLoadFloat4(&pCameraDesc->vAt));	// �ü� : ī�޶� �ٶ󺸴� ��

	// (�� ä���ֱ�)
	// ���� ä���� ����ü�� ī�޶� ��������� �־��ش�.
	m_fFovy = pCameraDesc->fFovy;
	m_fAspect = pCameraDesc->fAspect;
	m_fNear = pCameraDesc->fNear;
	m_fFar = pCameraDesc->fFar;

	return S_OK;
}

void CCamera::Priority_Tick(_float fTimeDelta)
{
}

void CCamera::Tick(_float fTimeDelta)
{
	// PipeLine ���� �� ����
	m_pGameInstance->Set_Transform(CPipeLine::D3DTS_VIEW, m_pTransformCom->Get_WorldMatrixInverse());
	m_pGameInstance->Set_Transform(CPipeLine::D3DTS_PROJ, XMMatrixPerspectiveFovLH(m_fFovy, m_fAspect, m_fNear, m_fFar));
	// �� �� ���⼭ ������� ���� ����ϱ� ���� �Ŵ�������, Tick�� Late_Tick ���̿� ����� �����Ѵ�.
}

void CCamera::Late_Tick(_float fTimeDelta)
{
}



void CCamera::Free()
{
	__super::Free();

}
