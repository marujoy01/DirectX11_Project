#pragma once

#include "Component.h"

/* 지정한 경로에 있는 텍스쳐들을 로드하여 객체화시킨다. */
/* 객체화한 텍스쳐들을 보관한다. */
/* 셰이더에 텍스쳐의 정보를 던지고 셰이더안에서 이 텍스쳐의 픽셀의 색을 샘플링하여 화면에 찍는다. */
/* 화면에 그리기위한 텍스쳐.*/
/* 이곳에서 생성해 매니저에 보관하여 관리한다. */

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
	// 외부 파일로부터 읽어야하니, 경로를 받아야한다. 인자값으로 (디바이스, 콘텍스트, wstring& strTextureFilePath(파일경로), _uint iNumTextures(몇장 받을건지))
	static CTexture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strTextureFilePath, _uint iNumTextures = 1);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END