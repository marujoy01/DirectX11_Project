#include "..\Public\PipeLine.h"

CPipeLine::CPipeLine()
{

}

// Set ī�޶� ���� ��� (_fmatrix)
void CPipeLine::Set_Transform(D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix)
{
	XMStoreFloat4x4(&m_Transform[eState], TransformMatrix);
}

// Set ī�޶� ���� ��� (_float4x4)
void CPipeLine::Set_Transform(D3DTRANSFORMSTATE eState, _float4x4 TransformMatrix)
{
	m_Transform[eState] = TransformMatrix;
}

// Get ī�޶� ���� ��� (_fmatrix)
_matrix CPipeLine::Get_TransformMatrix(D3DTRANSFORMSTATE eState)
{
	return XMLoadFloat4x4(&m_Transform[eState]);
}

// Get ī�޶� ���� ��� (_float4x4)
_float4x4 CPipeLine::Get_TransformFloat4x4(D3DTRANSFORMSTATE eState)
{
	return m_Transform[eState];
}

// Get ī�޶� ���� ����� (_fmatrix �� �����̽�)
_matrix CPipeLine::Get_TransformMatrixInverse(D3DTRANSFORMSTATE eState)
{
	return XMLoadFloat4x4(&m_Transform_Inverse[eState]);
}

// Get ī�޶� ���� ��� (_float4x4 �� �����̽�)
_float4x4 CPipeLine::Get_TransformFloat4x4Inverse(D3DTRANSFORMSTATE eState)
{
	return m_Transform_Inverse[eState];
}

// Get ���忡�� ī�޶� ��ġ (_float4)
_float4 CPipeLine::Get_CamPosition()
{
	return m_vCamPosition;
}

HRESULT CPipeLine::Initialize()
{
	// ī�޶��� ���� ��İ� �����(�� �����̽�) ����
	for (size_t i = 0; i < D3DTS_END; i++)
	{
		XMStoreFloat4x4(&m_Transform[i], XMMatrixIdentity());			// ����
		XMStoreFloat4x4(&m_Transform_Inverse[i], XMMatrixIdentity());	// �� �����̽�
	}

	return S_OK;
}

void CPipeLine::Tick()
{
	/* ī�޶� ���� ����� ��� (�� �����̽�) */
	for (size_t i = 0; i < D3DTS_END; i++)
	{
		//	��� ���� m_Transform_Inverse[i]�� ����
		XMStoreFloat4x4(&m_Transform_Inverse[i], XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_Transform[i])));
	}

	/* ī�޶� ���� ��� ��ġ ��� */
	// ��� ���� m_vCamPosition�� ����
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