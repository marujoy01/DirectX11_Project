#include "..\Public\Texture.h"
#include "Shader.h"

CTexture::CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

CTexture::CTexture(const CTexture& rhs)
	: CComponent(rhs)
	, m_iNumTextures(rhs.m_iNumTextures)
	, m_SRVs(rhs.m_SRVs)				// 텍스처를 매번 가져올 수 없으니, 만든 텍스처를 얕은 복사를 통해 사용한다.
{
	for (auto& pSRV : m_SRVs)	// 복사를 했으니, 레퍼런스 카운트를 올려준다. (레퍼런스 카운트를 올렸다면 내리는것도 신경써주자.)
		Safe_AddRef(pSRV);
}

// 세팅
HRESULT CTexture::Initialize_Prototype(const wstring& strTextureFilePath, _uint iNumTextures)
{
	// Create하며 넘겨받은 인자값 파일경로와 텍스처 개수를 사용한다.

	m_iNumTextures = iNumTextures;	// 텍스처 개수

	// 받아온 텍스처 개수만큼 원소 저장 공간을 확보한다. (미리 메모리 할당)
	m_SRVs.reserve(m_iNumTextures);

	for (size_t i = 0; i < m_iNumTextures; i++)
	{
		/* 전체 경로 : ..\정의훈\139\Framework\Client\Bin\Resources\Textures\Default.jpg */
		_tchar		szFullPath[MAX_PATH] = TEXT("");

		// 텍스처가 여러개일 경우 i번째 인덱스가 파일명에 들어가니, 알맞게 넣어줄 수 있게 해주기위함. (ex : Monster %d.png의 %d와 같음)
		wsprintf(szFullPath, strTextureFilePath.c_str(), i);

		/* 드라이브 : D:\ */
		_tchar		szDrive[MAX_PATH] = TEXT("");

		/* 디렉토리(파일이 들어있는 경로) : 정의훈\139\Framework\Client\Bin\Resources\Textures\ */
		_tchar		szDirectory[MAX_PATH] = TEXT("");

		/* 파일명 : Default_0 */
		_tchar		szFileName[MAX_PATH] = TEXT("");

		/* 확장자 : .jpg */
		_tchar		szExt[MAX_PATH] = TEXT("");

		// 경로를 받을 때, 분리하여 받고싶은건 위에 선언한 변수를 알맞게 넣어주고, 받고싶지 않은건 nullptr을 채워준다.
		// 변수 바로 다음은 변수의 사이즈이다.
		// 아래 함수는 이렇게된다. (전체경로, 안받음, 0, 안받음, 0, 안받음, 0, 확장자 저장, 사이즈 : MAX_PATH);
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

	/* 원형객체를 초기화한다.  */ // Initialize에 FilePath(경로), NumTextures(텍스처 개수)를 넘겨준다.
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

	/* 원형객체를 초기화한다.  */
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

	for (auto& pSRV : m_SRVs)	// 레퍼런스 카운트 내리기
		Safe_Release(pSRV);

	m_SRVs.clear();
}
