#include "..\Public\GameInstance.h"
#include "Graphic_Device.h"
#include "TImer_Manager.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "Renderer.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{
}

HRESULT CGameInstance::Initialize_Engine(_uint iNumLevels, const GRAPHIC_DESC& GraphicDesc, _Inout_ ID3D11Device** ppDevice, _Inout_ ID3D11DeviceContext** ppContext)
{
	/* 그래픽 디바이스를 초기화 하자.*/
	m_pGraphic_Device = CGraphic_Device::Create(GraphicDesc, ppDevice, ppContext);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;	

	/* 타이머를 사용할 준비를 하자. */
	m_pTimer_Manager = CTimer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	/* 레벨을 사용할 준비를 하자 */
	m_pLevel_Manager = CLevel_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	/* 오브젝트를 사용할 준비를 하자 */
	m_pObject_Manager = CObject_Manager::Create(iNumLevels);
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	/* 컴포넌트를 사용할 준비를 하자 */
	m_pComponent_Manager = CComponent_Manager::Create(iNumLevels);
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	/* 랜더러를 사용할 준비를 하자 */
	m_pRenderer = CRenderer::Create(*ppDevice, *ppContext);
	if (nullptr == m_pRenderer)
		return E_FAIL;

	/* 파이프라인을 사용할 준비를 하자 */
	m_pPipeLine = CPipeLine::Create();
	if (nullptr == m_pPipeLine)
		return E_FAIL;

	return S_OK;
}

// 매니저들의 Tick(업데이트)요소를 이곳에서 한번에 호출한다. (순서 관리에 좋음)
void CGameInstance::Tick_Engine(_float fTimeDelta)
{
	if (nullptr == m_pLevel_Manager)
		return;

	m_pObject_Manager->Priority_Tick(fTimeDelta);

	m_pObject_Manager->Tick(fTimeDelta);

	m_pObject_Manager->Late_Tick(fTimeDelta);

	m_pLevel_Manager->Tick(fTimeDelta);
}

void CGameInstance::Clear(_uint iLevelIndex)
{
	if (nullptr == m_pObject_Manager ||
		nullptr == m_pComponent_Manager)
		return;

	/* 오브젝트 매니져에 레벨별로 구분해 놓은 객체들 중 특정된 객체들을 지운다.  */
	m_pObject_Manager->Clear(iLevelIndex);

	/* 컴포넌트 매니져에 레벨별로 구분해 놓은 컴포넌트들 중 특정된 객체들을 지운다.  */
	m_pComponent_Manager->Clear(iLevelIndex);
}

HRESULT CGameInstance::Render_Engine()
{
	if (nullptr == m_pLevel_Manager ||
		nullptr == m_pRenderer)
		return E_FAIL;

	m_pRenderer->Draw_RenderGroup();


#ifdef _DEBUG	// 디버그용
	m_pLevel_Manager->Render();
#endif

	return S_OK;
}


#pragma region 그래픽 디바이스(장치)
HRESULT CGameInstance::Clear_BackBuffer_View(_float4 vClearColor)
{// 콘텍스트의 백 버퍼
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	return m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{// 콘텍스트의 깊이 버퍼
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	return m_pGraphic_Device->Clear_DepthStencil_View();
}

HRESULT CGameInstance::Present()
{// 화면에 보여주기 (스왑체인)
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	return m_pGraphic_Device->Present();
}
#pragma endregion

#pragma region 타임 매니저
HRESULT CGameInstance::Add_Timer(const wstring& strTimeTag)
{// 타이머 추가
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Add_Timer(strTimeTag);
}

_float CGameInstance::Compute_TimeDelta(const wstring& strTimeTag)
{// 계산된 1프레임을 반환
	if (nullptr == m_pTimer_Manager)
		return 0.0f;

	return m_pTimer_Manager->Compute_TimeDelta(strTimeTag);
}
#pragma endregion

#pragma region 레벨(씬) 매니저
HRESULT CGameInstance::Open_Level(_uint iCurrentLevelIndex, CLevel* pNewLevel)
{// 레벨
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Open_Level(iCurrentLevelIndex, pNewLevel);
}
#pragma endregion

#pragma region 오브젝트 매니저
HRESULT CGameInstance::Add_Prototype(const wstring& strPrototypeTag, CGameObject* pPrototype)
{// 오브젝트 원형 추가
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Prototype(strPrototypeTag, pPrototype);
}

HRESULT CGameInstance::Add_CloneObject(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg)
{// 오브젝트 클론 추가
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_CloneObject(iLevelIndex, strLayerTag, strPrototypeTag, pArg);
}
#pragma endregion

#pragma region 컴포넌트 매니저
HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag, CComponent* pPrototype)
{// 컴포넌트 원형 추가
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Prototype(iLevelIndex, strPrototypeTag, pPrototype);
}

CComponent* CGameInstance::Clone_Component(_uint iLevelIndex, const wstring& strPrototypeTag, void* pArg)
{// 컴포넌트 클론 추가
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iLevelIndex, strPrototypeTag, pArg);
}
#pragma endregion

#pragma region 랜더
HRESULT CGameInstance::Add_RenderGroup(CRenderer::RENDERGROUP eGroupID, CGameObject* pGameObject)
{// 랜더 그룹
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return m_pRenderer->Add_RenderGroup(eGroupID, pGameObject);
}
#pragma endregion

#pragma region 파이프 라인
void CGameInstance::Set_Transform(CPipeLine::D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix)
{// 행렬 세팅 (_fmatrix)
	if (nullptr == m_pPipeLine)
		return;

	m_pPipeLine->Set_Transform(eState, TransformMatrix);
}

void CGameInstance::Set_Transform(CPipeLine::D3DTRANSFORMSTATE eState, _float4x4 TransformMatrix)
{// 행렬 세팅 (_float4x4)
	if (nullptr == m_pPipeLine)
		return;

	m_pPipeLine->Set_Transform(eState, TransformMatrix);
}

_matrix CGameInstance::Get_TransformMatrix(CPipeLine::D3DTRANSFORMSTATE eState)
{// 행렬 가져오기 (_matrix)
	if (nullptr == m_pPipeLine)
		return XMMatrixIdentity();

	return m_pPipeLine->Get_TransformMatrix(eState);
}

_float4x4 CGameInstance::Get_TransformFloat4x4(CPipeLine::D3DTRANSFORMSTATE eState)
{// 행렬 가져오기 (_float4x4)
	if (nullptr == m_pPipeLine)
		return _float4x4();

	return m_pPipeLine->Get_TransformFloat4x4(eState);
}

_matrix CGameInstance::Get_TransformMatrixInverse(CPipeLine::D3DTRANSFORMSTATE eState)
{// 역행렬[뷰 스페이스] 가져오기 (_matrix)
	if (nullptr == m_pPipeLine)
		return XMMatrixIdentity();

	return m_pPipeLine->Get_TransformMatrixInverse(eState);
}

_float4x4 CGameInstance::Get_TransformFloat4x4Inverse(CPipeLine::D3DTRANSFORMSTATE eState)
{// 역행렬[뷰 스페이스] 가져오기 (_float4x4)
	if (nullptr == m_pPipeLine)
		return _float4x4();

	return m_pPipeLine->Get_TransformFloat4x4Inverse(eState);
}

_float4 CGameInstance::Get_CamPosition()
{// 카메라 위치 가져오기 (_float4)
	if (nullptr == m_pPipeLine)
		return _float4();

	return m_pPipeLine->Get_CamPosition();
}
#pragma endregion

#pragma region 삭제 관련
void CGameInstance::Release_Manager()
{// 매니저 및 장치 삭제
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pGraphic_Device);
}

void CGameInstance::Release_Engine()
{// 삭제함수 호출
	CGameInstance::GetInstance()->Release_Manager();
	CGameInstance::GetInstance()->DestroyInstance();
}
#pragma endregion