#include "..\Public\Shader.h"

CShader::CShader(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{
}

CShader::CShader(const CShader & rhs)
	: CComponent(rhs)
	, m_pEffect(rhs.m_pEffect)
	, m_InputLayouts(rhs.m_InputLayouts)
	, m_TechniqueDesc(rhs.m_TechniqueDesc)
{
	for (auto& pInputLayout : m_InputLayouts)
	{
		Safe_AddRef(pInputLayout);
	}

	Safe_AddRef(m_pEffect);
}

/* ���̴������� �����Ͽ� ID3DX11Effect�� �����´�. */
/* ID3DX11Effect�κ��� Pass�� ���� �����Ͽ� ���ڵ� ���� ���������� �� ���̴� �н����� �� �޾��� ���ִ����� ���� InputLayout�� �������. */
HRESULT CShader::Initialize_Prototype(const wstring & strShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements)
{
	_uint		iHlslFlag = 0;

#ifdef _DEBUG
	iHlslFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	iHlslFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif

	/*strShaderFilePath��ο� �ۼ��Ǿ�Ӥ����� hlsl��� ���� �����Ͽ� ID3DX11Effect��� �༮�� ������.  */
	if (FAILED(D3DX11CompileEffectFromFile(strShaderFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, iHlslFlag, 0, m_pDevice, &m_pEffect, nullptr)))
		return E_FAIL;

	ID3DX11EffectTechnique*		pTechnique = m_pEffect->GetTechniqueByIndex(0);
	if (nullptr == pTechnique)
		return E_FAIL;	

	pTechnique->GetDesc(&m_TechniqueDesc);

	for (size_t i = 0; i < m_TechniqueDesc.Passes; i++)
	{
		ID3DX11EffectPass*		pPass = pTechnique->GetPassByIndex(i);

		D3DX11_PASS_DESC		PassDesc;
		pPass->GetDesc(&PassDesc);		

		/* InputLayout : ���� �׸������� ����ϴ� ������ �Է�����.  */
		/* dx11������ ������� ���������������ο� ���� ����� �����Ǿ���. */
		/* �츮�� ���� ������ ������������ �����ؾ��Ѵ�.(��������� ����������������) -> Shader */
		/* �׷��� �츮���� �ݵ�� ���̴��� �ʿ��ϴ�. */
		/* �츮�� �� �������� �׸������ؼ��� ���̴��� �ʿ��ϰ�, �� ���̴��� �ݵ�� ���� �׸������ϴ� ������ �޾��� �� �־���Ѵ�. */
		/* ���� �׸������ϴ� ������ ����Ϸ����ϴ� ���̴��� �Է��� ��������?�� ���� üũ�� ������ �̸� ó���ϰ�.
		�����ϴٸ� dx11�� InputLayout�̶� ��ü�� ������ش�. */
		ID3D11InputLayout*		pInputLayout = nullptr;

		if (FAILED(m_pDevice->CreateInputLayout(pElements, iNumElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &pInputLayout)))
			return E_FAIL;

		m_InputLayouts.push_back(pInputLayout);
	}
	
	return S_OK;
}

HRESULT CShader::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CShader::Begin(_uint iPassIndex)
{
	if (iPassIndex >= m_TechniqueDesc.Passes)
		return E_FAIL;

	ID3DX11EffectTechnique*		pTechnique = m_pEffect->GetTechniqueByIndex(0);
	if (nullptr == pTechnique)
		return E_FAIL;

	ID3DX11EffectPass*		pPass = pTechnique->GetPassByIndex(iPassIndex);

	pPass->Apply(0, m_pContext);

	m_pContext->IASetInputLayout(m_InputLayouts[iPassIndex]);

	return S_OK;
}

HRESULT CShader::Bind_Matrix(const _char* pConstantName, const _float4x4 * pMatrix)
{
	/* �� ���̴��� ����Ǿ��ִ� ���������� �ڵ��� ���´�.*/
	ID3DX11EffectVariable*		pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectMatrixVariable*	pMatrixVariable = pVariable->AsMatrix();
	if (nullptr == pMatrixVariable)
		return E_FAIL;

	return pMatrixVariable->SetMatrix((_float*)pMatrix);
}

HRESULT CShader::Bind_Matrices(const _char * pConstantName, const _float4x4 * pMatrix, _uint iNumMatrices)
{
	ID3DX11EffectVariable*		pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectMatrixVariable*	pMatrixVariable = pVariable->AsMatrix();
	if (nullptr == pMatrixVariable)
		return E_FAIL;

	return pMatrixVariable->SetMatrixArray((_float*)pMatrix, 0, iNumMatrices);
}

HRESULT CShader::Bind_SRV(const _char * pConstantName, ID3D11ShaderResourceView * pSRV)
{
	ID3DX11EffectVariable*		pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable*	pSRVariable = pVariable->AsShaderResource();
	if (nullptr == pSRVariable)
		return E_FAIL;

	return pSRVariable->SetResource(pSRV);
}

HRESULT CShader::Bind_SRVs(const _char * pConstantName, ID3D11ShaderResourceView ** ppSRV, _uint iNumTextures)
{
	ID3DX11EffectVariable*		pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable*	pSRVariable = pVariable->AsShaderResource();
	if (nullptr == pSRVariable)
		return E_FAIL;

	return pSRVariable->SetResourceArray(ppSRV, 0, iNumTextures);
}

CShader * CShader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const wstring & strShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements)
{
	CShader*		pInstance = new CShader(pDevice, pContext);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype(strShaderFilePath, pElements, iNumElements)))
	{
		MSG_BOX("Failed to Created : CShader");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CShader::Clone(void * pArg)
{
	CShader*		pInstance = new CShader(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CShader");
		Safe_Release(pInstance);
	}
	return pInstance;
}


void CShader::Free()
{
	__super::Free();

	for (auto& pInputLayout : m_InputLayouts)
		Safe_Release(pInputLayout);

	Safe_Release(m_pEffect);
}
