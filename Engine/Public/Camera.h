#pragma once

// Ŭ���̾�Ʈ �����ڰ� ��������ϴ� ī�޶���� �������� �θ� �Ǳ����� Ŭ������. (ī�޶��� �⺻ Ʋ)

#include "GameObject.h"

BEGIN(Engine)

// �����ִ� Ŭ�����̴�, ENGINE_DLL �߰�, ī�޶� GameObject�̴� ���.
class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct tagCameraDesc : public tagGameObjectDesc	// ī�޶��� �⺻ �������� ���� ����ü. (���ӿ�����Ʈ ����ü ���)
	{														// ����ü�� ���� �޾� ��������� ä���� ���̴�.
		_float4		vEye, vAt;	// ī�޶� ��ġ, ī�޶� ���°�
		_float		fFovy, fAspect, fNear, fFar;
	}CAMERA_DESC;	// ����ü �̸�

protected:	// ���� �� �Ҹ���
	CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;

public:	// �⺻ ���� �Լ���
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;

protected:	// ī�޶� ���� ������
	// (Eye, At���� �⺻ ���� �������� �ʱ⿡ �ѹ��� �����ϱ� ������, ��� ������ �������� �ʴ´�.)
	// -> Eye, At�� ���� �����Ͽ� �����̰ų� �������� �ʰ�, TransformCom�� ���� �����Ұ��̱� ����. ���� Eye, At���� �⺻�������� �ѹ��� �����صֵ� �ȴ�.
#pragma region ������ ���� ������
	_float				m_fFovy = { 0.f };		// �þ߰�
	_float				m_fAspect = { 0.0f };	// ��Ⱦ��
	_float				m_fNear = { 0.0f };		// ����
	_float				m_fFar = { 0.0f };		// ��
#pragma endregion

public:	// ���� �� ����
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END