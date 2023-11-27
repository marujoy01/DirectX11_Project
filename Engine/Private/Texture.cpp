#include "..\Public\Texture.h"
#include "Shader.h"

CTexture::CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

CTexture::CTexture(const CTexture& rhs)
	: CComponent(rhs)
	, m_iNumTextures(rhs.m_iNumTextures)
	, m_SRVs(rhs.m_SRVs)				// �ؽ�ó�� �Ź� ������ �� ������, ���� �ؽ�ó�� ���� ���縦 ���� ����Ѵ�.
{
	for (auto& pSRV : m_SRVs)	// ���縦 ������, ���۷��� ī��Ʈ�� �÷��ش�. (���۷��� ī��Ʈ�� �÷ȴٸ� �����°͵� �Ű������.)
		Safe_AddRef(pSRV);
}

// ����
HRESULT CTexture::Initialize_Prototype(const wstring& strTextureFilePath, _uint iNumTextures)
{
	// Create�ϸ� �Ѱܹ��� ���ڰ� ���ϰ�ο� �ؽ�ó ������ ����Ѵ�.

	m_iNumTextures = iNumTextures;	// �ؽ�ó ����

	// �޾ƿ� �ؽ�ó ������ŭ ���� ���� ������ Ȯ���Ѵ�. (�̸� �޸� �Ҵ�)
	m_SRVs.reserve(m_iNumTextures);

	for (size_t i = 0; i < m_iNumTextures; i++)
	{
		/* ��ü ��� : ..\������\139\Framework\Client\Bin\Resources\Textures\Default.jpg */
		_tchar		szFullPath[MAX_PATH] = TEXT("");

		// �ؽ�ó�� �������� ��� i��° �ε����� ���ϸ� ����, �˸°� �־��� �� �ְ� ���ֱ�����. (ex : Monster %d.png�� %d�� ����)
		wsprintf(szFullPath, strTextureFilePath.c_str(), i);

		/* ����̺� : D:\ */
		_tchar		szDrive[MAX_PATH] = TEXT("");

		/* ���丮(������ ����ִ� ���) : ������\139\Framework\Client\Bin\Resources\Textures\ */
		_tchar		szDirectory[MAX_PATH] = TEXT("");

		/* ���ϸ� : Default_0 */
		_tchar		szFileName[MAX_PATH] = TEXT("");

		/* Ȯ���� : .jpg */
		_tchar		szExt[MAX_PATH] = TEXT("");

		// ��θ� ���� ��, �и��Ͽ� �ް������ ���� ������ ������ �˸°� �־��ְ�, �ް���� ������ nullptr�� ä���ش�.
		// ���� �ٷ� ������ ������ �������̴�.
		// �Ʒ� �Լ��� �̷��Եȴ�. (��ü���, �ȹ���, 0, �ȹ���, 0, �ȹ���, 0, Ȯ���� ����, ������ : MAX_PATH);
		_wsplitpath_s(szFullPath, nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);

		//
		ID3D11ShaderResourceView* pSRV = { nullptr };

		HRESULT			hr = 0;

		if (!lstrcmp(szExt, TEXT(".dds")))
		{
			hr = CreateDDSTextureFromFile(m_pDevice, szFullPath, nullptr, &pSRV);
		}
		else if (!lstrcmp(szExt, TEXT(".tga")))
		{

		}
		else
		{
			hr = CreateWICTextureFromFile(m_pDevice, szFullPath, nullptr, &pSRV);
		}

		if (FAILED(hr))
			return E_FAIL;

		m_SRVs.push_back(pSRV);
	}

	return S_OK;
}

HRESULT CTexture::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CTexture::Bind_ShaderResource(CShader* pShader, const _char* pConstantName, _uint iTextureIndex)
{
	return pShader->Bind_SRV(pConstantName, m_SRVs[iTextureIndex]);
}

HRESULT CTexture::Bind_ShaderResources(CShader* pShader, const _char* pConstantName)
{
	return pShader->Bind_SRVs(pConstantName, &m_SRVs.front(), m_iNumTextures);
}

CTexture* CTexture::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strTextureFilePath, _uint iNumTextures)
{
	CTexture* pInstance = new CTexture(pDevice, pContext);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */ // Initialize�� FilePath(���), NumTextures(�ؽ�ó ����)�� �Ѱ��ش�.
	if (FAILED(pInstance->Initialize_Prototype(strTextureFilePath, iNumTextures)))
	{
		MSG_BOX("Failed to Created : CTexture");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CTexture::Clone(void* pArg)
{
	CTexture* pInstance = new CTexture(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTexture");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTexture::Free()
{
	__super::Free();

	for (auto& pSRV : m_SRVs)	// ���۷��� ī��Ʈ ������
		Safe_Release(pSRV);

	m_SRVs.clear();
}
