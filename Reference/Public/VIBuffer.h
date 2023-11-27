#pragma once

#include "Component.h"

/* Ư�� ����(Rect, Cube, Terrain, Model) �� �����ϱ����� Ŭ�������� �θ� �Ǵ� Ŭ������ .*/
/* VIBuffer = Vertex(����) + Index(�׸��� ������ ���� ������ �ε��� ����.) + Buffer(�޸𸮰���) */

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render();

public:

	HRESULT Bind_VIBuffers();
	/* ����, �ε���, �ν��Ͻ� ���۵��� �����Ѵ�. */
	HRESULT Create_Buffer(_Inout_ ID3D11Buffer**	ppBuffer);

protected:
	ID3D11Buffer*				m_pVB = { nullptr };
	ID3D11Buffer*				m_pIB = { nullptr };

	/* ���� ���� ������� �ϴ� ������ �Ӽ��� �����ϳ�. */
	D3D11_BUFFER_DESC			m_BufferDesc;
	D3D11_SUBRESOURCE_DATA		m_SubResourceData;
protected:
	_uint						m_iNumVertices = { 0 };
	_uint						m_iStride = { 0 };
	_uint						m_iNumVertexBuffers = { 0 };

	_uint						m_iNumIndices = { 0 };
	_uint						m_iIndexStride = { 0 };
	DXGI_FORMAT					m_eIndexFormat = {  };
	D3D11_PRIMITIVE_TOPOLOGY	m_eTopology = { };
public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END