#include "..\Public\Transform.h"
#include "Shader.h"

CTransform::CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

CTransform::CTransform(const CTransform& rhs)
	: CComponent(rhs)
{
}


// 크기 세팅
void CTransform::Set_Scaling(_float fScaleX, _float fScaleY, _float fScaleZ)
{
	Set_State(STATE_RIGHT, XMVector3Normalize(Get_State(STATE_RIGHT)) * fScaleX);
	Set_State(STATE_UP, XMVector3Normalize(Get_State(STATE_UP)) * fScaleY);
	Set_State(STATE_LOOK, XMVector3Normalize(Get_State(STATE_LOOK)) * fScaleZ);
}

// 원형 객체 세팅
HRESULT CTransform::Initialize_Prototype(_float fSpeedPerSec, _float fRotationPerSec)
{
	m_fSpeedPerSec = fSpeedPerSec;
	m_fRotationPerSec = fRotationPerSec;

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	_matrix			matTmp;
	/*
	matTmp = XMLoadFloat4x4(&m_WorldMatrix);*/


	/*_float3		vTmp = _float3(0.f, 0.f, 0.f);
	_float4		vTmp2 = _float4(0.f, 0.f, 0.f, 0.f);

	_vector		vTmp1 = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	_vector		vTmp3 = XMVectorSet(0.f, 0.f, 0.f, 0.f);

	XMVector3Normalize(vTmp1);

	_float		fDot = XMVectorGetX(XMVector3Dot(vTmp1, vTmp3));

	_float		fLength = XMVectorGetX(XMVector3Length(vTmp1));

	XMStoreFloat3(&vTmp, vTmp1);
	XMStoreFloat4(&vTmp2, vTmp1);*/


	return S_OK;
}

// 앞으로 이동
void CTransform::Go_Straight(_float fTimeDelta)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vLook = Get_State(STATE_LOOK);

	vPosition += XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

// 왼쪽 이동
void CTransform::Go_Left(_float fTimeDelta)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vRight = Get_State(STATE_RIGHT);

	vPosition -= XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

// 오른쪽 이동
void CTransform::Go_Right(_float fTimeDelta)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vRight = Get_State(STATE_RIGHT);

	vPosition += XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

// 뒤로 이동
void CTransform::Go_Backward(_float fTimeDelta)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vLook = Get_State(STATE_LOOK);

	vPosition -= XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

// 키 입력시 회전
void CTransform::Turn(_fvector vAxis, _float fTimeDelta)
{
	_vector		vRight = Get_State(STATE_RIGHT);
	_vector		vUp = Get_State(STATE_UP);
	_vector		vLook = Get_State(STATE_LOOK);

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, m_fRotationPerSec * fTimeDelta);

	Set_State(STATE_RIGHT, XMVector3TransformNormal(vRight, RotationMatrix));
	Set_State(STATE_UP, XMVector3TransformNormal(vUp, RotationMatrix));
	Set_State(STATE_LOOK, XMVector3TransformNormal(vLook, RotationMatrix));
}

// 입력받은 각도로 고정
void CTransform::Rotation(_fvector vAxis, _float fRadian)
{
	_float3		vScale = Get_Scaled();

	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScale.x;
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScale.y;
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScale.z;

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	Set_State(STATE_RIGHT, XMVector3TransformNormal(vRight, RotationMatrix));
	Set_State(STATE_UP, XMVector3TransformNormal(vUp, RotationMatrix));
	Set_State(STATE_LOOK, XMVector3TransformNormal(vLook, RotationMatrix));
}

// 타겟쪽으로 이동
void CTransform::Go_Target(_fvector vTargetPos, _float fTimeDelta, _float fSpare)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vDir = vTargetPos - vPosition;

	_float		fDistance = XMVectorGetX(XMVector3Length(vDir));

	if (fDistance >= fSpare)
		vPosition += XMVector3Normalize(vDir) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

// 타겟을 바라봄
void CTransform::Look_At(_fvector vTargetPos)
{
	_float3		vScale = Get_Scaled();

	_vector		vPosition = Get_State(STATE_POSITION);

	_vector		vLook = XMVector3Normalize(vTargetPos - vPosition) * vScale.z;
	_vector		vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook)) * vScale.x;
	_vector		vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight)) * vScale.y;

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
}

// 타겟을 수평으로 바라봄
void CTransform::Look_At_OnLand(_fvector vTargetPos)
{
	_float3		vScale = Get_Scaled();

	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vLook = vTargetPos - vPosition;

	_vector		vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook)) * vScale.x;

	vLook = XMVector3Normalize(XMVector3Cross(vRight, XMVectorSet(0.f, 1.f, 0.f, 0.f))) * vScale.z;

	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScale.y;

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
}

// 셰이더에 정보를 넘겨줌
HRESULT CTransform::Bind_ShaderResource(CShader* pShader, const _char* pConstantName)
{
	return pShader->Bind_Matrix(pConstantName, &m_WorldMatrix);
}

// 원형 객체 생성
CTransform* CTransform::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _float fSpeedPerSec, _float fRotationPerSec)
{
	CTransform* pInstance = new CTransform(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype(fSpeedPerSec, fRotationPerSec)))
	{
		MSG_BOX("Failed to Created : CTransform");
		Safe_Release(pInstance);
	}
	return pInstance;
}

// 클론 생성
CComponent* CTransform::Clone(void* pArg)
{
	CTransform* pInstance = new CTransform(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTransform");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTransform::Free()
{
	__super::Free();

}
