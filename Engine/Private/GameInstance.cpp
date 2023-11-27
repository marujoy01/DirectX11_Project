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
	/* �׷��� ����̽��� �ʱ�ȭ ����.*/
	m_pGraphic_Device = CGraphic_Device::Create(GraphicDesc, ppDevice, ppContext);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;	

	/* Ÿ�̸Ӹ� ����� �غ� ����. */
	m_pTimer_Manager = CTimer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	/* ������ ����� �غ� ���� */
	m_pLevel_Manager = CLevel_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	/* ������Ʈ�� ����� �غ� ���� */
	m_pObject_Manager = CObject_Manager::Create(iNumLevels);
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	/* ������Ʈ�� ����� �غ� ���� */
	m_pComponent_Manager = CComponent_Manager::Create(iNumLevels);
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	/* �������� ����� �غ� ���� */
	m_pRenderer = CRenderer::Create(*ppDevice, *ppContext);
	if (nullptr == m_pRenderer)
		return E_FAIL;

	/* ������������ ����� �غ� ���� */
	m_pPipeLine = CPipeLine::Create();
	if (nullptr == m_pPipeLine)
		return E_FAIL;

	return S_OK;
}

// �Ŵ������� Tick(������Ʈ)��Ҹ� �̰����� �ѹ��� ȣ���Ѵ�. (���� ������ ����)
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

	/* ������Ʈ �Ŵ����� �������� ������ ���� ��ü�� �� Ư���� ��ü���� �����.  */
	m_pObject_Manager->Clear(iLevelIndex);

	/* ������Ʈ �Ŵ����� �������� ������ ���� ������Ʈ�� �� Ư���� ��ü���� �����.  */
	m_pComponent_Manager->Clear(iLevelIndex);
}

HRESULT CGameInstance::Render_Engine()
{
	if (nullptr == m_pLevel_Manager ||
		nullptr == m_pRenderer)
		return E_FAIL;

	m_pRenderer->Draw_RenderGroup();


#ifdef _DEBUG	// ����׿�
	m_pLevel_Manager->Render();
#endif

	return S_OK;
}


#pragma region �׷��� ����̽�(��ġ)
HRESULT CGameInstance::Clear_BackBuffer_View(_float4 vClearColor)
{// ���ؽ�Ʈ�� �� ����
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	return m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{// ���ؽ�Ʈ�� ���� ����
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	return m_pGraphic_Device->Clear_DepthStencil_View();
}

HRESULT CGameInstance::Present()
{// ȭ�鿡 �����ֱ� (����ü��)
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	return m_pGraphic_Device->Present();
}
#pragma endregion

#pragma region Ÿ�� �Ŵ���
HRESULT CGameInstance::Add_Timer(const wstring& strTimeTag)
{// Ÿ�̸� �߰�
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Add_Timer(strTimeTag);
}

_float CGameInstance::Compute_TimeDelta(const wstring& strTimeTag)
{// ���� 1�������� ��ȯ
	if (nullptr == m_pTimer_Manager)
		return 0.0f;

	return m_pTimer_Manager->Compute_TimeDelta(strTimeTag);
}
#pragma endregion

#pragma region ����(��) �Ŵ���
HRESULT CGameInstance::Open_Level(_uint iCurrentLevelIndex, CLevel* pNewLevel)
{// ����
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Open_Level(iCurrentLevelIndex, pNewLevel);
}
#pragma endregion

#pragma region ������Ʈ �Ŵ���
HRESULT CGameInstance::Add_Prototype(const wstring& strPrototypeTag, CGameObject* pPrototype)
{// ������Ʈ ���� �߰�
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Prototype(strPrototypeTag, pPrototype);
}

HRESULT CGameInstance::Add_CloneObject(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg)
{// ������Ʈ Ŭ�� �߰�
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_CloneObject(iLevelIndex, strLayerTag, strPrototypeTag, pArg);
}
#pragma endregion

#pragma region ������Ʈ �Ŵ���
HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag, CComponent* pPrototype)
{// ������Ʈ ���� �߰�
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Prototype(iLevelIndex, strPrototypeTag, pPrototype);
}

CComponent* CGameInstance::Clone_Component(_uint iLevelIndex, const wstring& strPrototypeTag, void* pArg)
{// ������Ʈ Ŭ�� �߰�
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iLevelIndex, strPrototypeTag, pArg);
}
#pragma endregion

#pragma region ����
HRESULT CGameInstance::Add_RenderGroup(CRenderer::RENDERGROUP eGroupID, CGameObject* pGameObject)
{// ���� �׷�
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return m_pRenderer->Add_RenderGroup(eGroupID, pGameObject);
}
#pragma endregion

#pragma region ������ ����
void CGameInstance::Set_Transform(CPipeLine::D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix)
{// ��� ���� (_fmatrix)
	if (nullptr == m_pPipeLine)
		return;

	m_pPipeLine->Set_Transform(eState, TransformMatrix);
}

void CGameInstance::Set_Transform(CPipeLine::D3DTRANSFORMSTATE eState, _float4x4 TransformMatrix)
{// ��� ���� (_float4x4)
	if (nullptr == m_pPipeLine)
		return;

	m_pPipeLine->Set_Transform(eState, TransformMatrix);
}

_matrix CGameInstance::Get_TransformMatrix(CPipeLine::D3DTRANSFORMSTATE eState)
{// ��� �������� (_matrix)
	if (nullptr == m_pPipeLine)
		return XMMatrixIdentity();

	return m_pPipeLine->Get_TransformMatrix(eState);
}

_float4x4 CGameInstance::Get_TransformFloat4x4(CPipeLine::D3DTRANSFORMSTATE eState)
{// ��� �������� (_float4x4)
	if (nullptr == m_pPipeLine)
		return _float4x4();

	return m_pPipeLine->Get_TransformFloat4x4(eState);
}

_matrix CGameInstance::Get_TransformMatrixInverse(CPipeLine::D3DTRANSFORMSTATE eState)
{// �����[�� �����̽�] �������� (_matrix)
	if (nullptr == m_pPipeLine)
		return XMMatrixIdentity();

	return m_pPipeLine->Get_TransformMatrixInverse(eState);
}

_float4x4 CGameInstance::Get_TransformFloat4x4Inverse(CPipeLine::D3DTRANSFORMSTATE eState)
{// �����[�� �����̽�] �������� (_float4x4)
	if (nullptr == m_pPipeLine)
		return _float4x4();

	return m_pPipeLine->Get_TransformFloat4x4Inverse(eState);
}

_float4 CGameInstance::Get_CamPosition()
{// ī�޶� ��ġ �������� (_float4)
	if (nullptr == m_pPipeLine)
		return _float4();

	return m_pPipeLine->Get_CamPosition();
}
#pragma endregion

#pragma region ���� ����
void CGameInstance::Release_Manager()
{// �Ŵ��� �� ��ġ ����
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pGraphic_Device);
}

void CGameInstance::Release_Engine()
{// �����Լ� ȣ��
	CGameInstance::GetInstance()->Release_Manager();
	CGameInstance::GetInstance()->DestroyInstance();
}
#pragma endregion