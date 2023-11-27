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

HRESULT CCamera::Initialize(void * pArg) // 클론 생성시 값 세팅
{
	if (nullptr == pArg)
		return E_FAIL;

	// 공통적으로 필요한 요소를 부모에 미리 정의해둔걸로 세팅 (TransformCom..등등)
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	// (생성)
	/* 카메라 구조체를 포인터형으로 캐스팅하여 생성 */
	/* 만들어둔 카메라 구조체 선언 */
	CAMERA_DESC*	pCameraDesc = (CAMERA_DESC*)pArg;	

	// (값 받기)
	/* 항등상태의 트랜스폼을 내가 셋팅하고 싶은 카메라의 상태로 동기화한다. */
	/* 카메라의 트랜스폼을 항등 상태로 만들어뒀으니, 내가 원하는 값(카메라 정보 구조체)을 넣어 세팅할 수 있다. */
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&pCameraDesc->vEye));	// 위치 : 카메라의 위치
	m_pTransformCom->Look_At(XMLoadFloat4(&pCameraDesc->vAt));	// 시선 : 카메라가 바라보는 곳

	// (값 채워주기)
	// 값이 채워진 구조체로 카메라 멤버변수에 넣어준다.
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
	// PipeLine 생성 후 세팅
	m_pGameInstance->Set_Transform(CPipeLine::D3DTS_VIEW, m_pTransformCom->Get_WorldMatrixInverse());
	m_pGameInstance->Set_Transform(CPipeLine::D3DTS_PROJ, XMMatrixPerspectiveFovLH(m_fFovy, m_fAspect, m_fNear, m_fFar));
	// 추 후 여기서 만들어진 값을 사용하기 위해 매니저에서, Tick과 Late_Tick 사이에 기능을 세팅한다.
}

void CCamera::Late_Tick(_float fTimeDelta)
{
}



void CCamera::Free()
{
	__super::Free();

}
