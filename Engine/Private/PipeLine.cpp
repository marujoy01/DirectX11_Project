#include "..\Public\PipeLine.h"

CPipeLine::CPipeLine()
{

}

// Set 카메라 월드 행렬 (_fmatrix)
void CPipeLine::Set_Transform(D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix)
{
	XMStoreFloat4x4(&m_Transform[eState], TransformMatrix);
}

// Set 카메라 월드 행렬 (_float4x4)
void CPipeLine::Set_Transform(D3DTRANSFORMSTATE eState, _float4x4 TransformMatrix)
{
	m_Transform[eState] = TransformMatrix;
}

// Get 카메라 월드 행렬 (_fmatrix)
_matrix CPipeLine::Get_TransformMatrix(D3DTRANSFORMSTATE eState)
{
	return XMLoadFloat4x4(&m_Transform[eState]);
}

// Get 카메라 월드 행렬 (_float4x4)
_float4x4 CPipeLine::Get_TransformFloat4x4(D3DTRANSFORMSTATE eState)
{
	return m_Transform[eState];
}

// Get 카메라 월드 역행렬 (_fmatrix 뷰 스페이스)
_matrix CPipeLine::Get_TransformMatrixInverse(D3DTRANSFORMSTATE eState)
{
	return XMLoadFloat4x4(&m_Transform_Inverse[eState]);
}

// Get 카메라 월드 행렬 (_float4x4 뷰 스페이스)
_float4x4 CPipeLine::Get_TransformFloat4x4Inverse(D3DTRANSFORMSTATE eState)
{
	return m_Transform_Inverse[eState];
}

// Get 월드에서 카메라 위치 (_float4)
_float4 CPipeLine::Get_CamPosition()
{
	return m_vCamPosition;
}

HRESULT CPipeLine::Initialize()
{
	// 카메라의 월드 행렬과 역행렬(뷰 스페이스) 세팅
	for (size_t i = 0; i < D3DTS_END; i++)
	{
		XMStoreFloat4x4(&m_Transform[i], XMMatrixIdentity());			// 월드
		XMStoreFloat4x4(&m_Transform_Inverse[i], XMMatrixIdentity());	// 뷰 스페이스
	}

	return S_OK;
}

void CPipeLine::Tick()
{
	/* 카메라 월드 역행렬 계산 (뷰 스페이스) */
	for (size_t i = 0; i < D3DTS_END; i++)
	{
		//	멤버 변수 m_Transform_Inverse[i]에 저장
		XMStoreFloat4x4(&m_Transform_Inverse[i], XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_Transform[i])));
	}

	/* 카메라 월드 행렬 위치 계산 */
	// 멤버 변수 m_vCamPosition에 저장
	memcpy(&m_vCamPosition, &m_Transform_Inverse[D3DTS_VIEW].m[3], sizeof(_float4));
}

CPipeLine* CPipeLine::Create()
{
	CPipeLine* pInstance = new CPipeLine;

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CPipeLine");
		Safe_Release(pInstance);
	}
	return pInstance;
}


void CPipeLine::Free()
{

}