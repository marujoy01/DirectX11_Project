#pragma once

// 클라이언트 개발자가 만들고자하는 카메라들의 공통적인 부모가 되기위한 클래스다. (카메라의 기본 틀)

#include "GameObject.h"

BEGIN(Engine)

// 보여주는 클래스이니, ENGINE_DLL 추가, 카메라도 GameObject이니 상속.
class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct tagCameraDesc : public tagGameObjectDesc	// 카메라의 기본 정보들을 받을 구조체. (게임오브젝트 구조체 상속)
	{														// 구조체로 값을 받아 멤버변수를 채워줄 것이다.
		_float4		vEye, vAt;	// 카메라 위치, 카메라가 보는곳
		_float		fFovy, fAspect, fNear, fFar;
	}CAMERA_DESC;	// 구조체 이름

protected:	// 생성 및 소멸자
	CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;

public:	// 기본 세팅 함수들
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;

protected:	// 카메라 정보 변수들
	// (Eye, At같은 기본 정보 변수들은 초기에 한번만 세팅하기 때문에, 멤버 변수로 만들어두지 않는다.)
	// -> Eye, At의 값을 변경하여 움직이거나 조작하지 않고, TransformCom을 통해 조작할것이기 때문. 따라서 Eye, At같은 기본정보들은 한번만 세팅해둬도 된다.
#pragma region 투영을 위한 변수들
	_float				m_fFovy = { 0.f };		// 시야각
	_float				m_fAspect = { 0.0f };	// 종횡비
	_float				m_fNear = { 0.0f };		// 시작
	_float				m_fFar = { 0.0f };		// 끝
#pragma endregion

public:	// 생성 및 삭제
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END