#pragma once

#include "Component.h"

/* ������ ��ο� �ִ� �ؽ��ĵ��� �ε��Ͽ� ��üȭ��Ų��. */
/* ��üȭ�� �ؽ��ĵ��� �����Ѵ�. */
/* ���̴��� �ؽ����� ������ ������ ���̴��ȿ��� �� �ؽ����� �ȼ��� ���� ���ø��Ͽ� ȭ�鿡 ��٤���. */
/* ȭ�鿡 �׸������� �ؽ���.*/

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
	
private:
	CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;

public:
	virtual HRESULT Initialize_Prototype(const wstring& strTextureFilePath, _uint iNumTextures);
	virtual HRESULT Initialize(void* pArg) override;
public:
	HRESULT  Bind_ShaderResource(class CShader* pShader, const _char* pConstantName, _uint iTextureIndex = 0);
	HRESULT  Bind_ShaderResources(class CShader* pShader, const _char* pConstantName);



private:
	_uint										m_iNumTextures = { 0 };
	vector<ID3D11ShaderResourceView*>			m_SRVs;	

public:
	static CTexture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strTextureFilePath, _uint iNumTextures = 1);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END