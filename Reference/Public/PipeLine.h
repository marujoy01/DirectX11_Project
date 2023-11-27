#pragma once

/* 인게임 내 객체들을 그릴때 공통으로 사용하기위한 뷰, 투영행렬을 보관한다. */
// [필요성]
/* 카메라가 바라보는 세상을 만들기 위해선, 모든 객체들이 그려질때 카메라의 월드 역행렬 (뷰 스페이스)가 필요하다. */
/* 또한, 많은 연산과정 (빛, 안개, 린라이트 등등)에서도 카메라의 위치를 많이 요구할것이다. */
// [만든 이유]
/* Dx9때는 장치에 행렬을 저장하여 어디서든 꺼내 쓸 수 있었지만 Dx11에선 장치에 그런 기능이없다. 따라서 이 방법과 똑같이 PipeLine이라는 녀석을 만들어 사용할 것이다. */
// [사용]
/* 이렇게 만든 PipeLine 클래스로 카메라 행렬을 저장하고, 원할때 꺼내쓸 수 있게 만든뒤 필요한곳에서 꺼내쓰자. */
#include "Base.h"

BEGIN(Engine)

class CPipeLine final : public CBase
{
public:	// 행렬을 배열로 저장할것이니, 편의성을 위해 enum으로 ID를 지정하자.
	//							 뷰	   ,	투영   
	enum D3DTRANSFORMSTATE { D3DTS_VIEW, D3DTS_PROJ, D3DTS_END };
private:
	CPipeLine();
	virtual ~CPipeLine() = default;

public: // Set : 카메라 정보 추가
	void Set_Transform(D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix);
	void Set_Transform(D3DTRANSFORMSTATE eState, _float4x4 TransformMatrix);

public:	// Get : 카메라 정보 반환
	_matrix Get_TransformMatrix(D3DTRANSFORMSTATE eState);
	_float4x4 Get_TransformFloat4x4(D3DTRANSFORMSTATE eState);
	_matrix Get_TransformMatrixInverse(D3DTRANSFORMSTATE eState);
	_float4x4 Get_TransformFloat4x4Inverse(D3DTRANSFORMSTATE eState);
	_float4 Get_CamPosition();

public:
	HRESULT Initialize();

	void Tick();	// 업데이트
	/*	왜 카메라에 업데이트가 있을까?
		- 카메라의 역행렬을 카메라에서 계산하여 반환할것인데, 역행렬을 반환하는 Get함수를 호출할때마다 함수내에서 계산까지 이루어지게되면 연산량이 많아지게된다.
		- 따라서 연산을 최소화 하기위해 Tick에서 계산을 한 뒤, 역행렬 멤버 변수에 저장하고, Get함수에선 값이 들어있는 멤버 변수만 반환시키는것
		- 카메라 위치도 같은 이유
	*/

private:
	_float4x4			m_Transform[D3DTS_END];			// 행렬
	_float4x4			m_Transform_Inverse[D3DTS_END];	// 역행렬
	_float4				m_vCamPosition;					// 카메라 위치

public:
	static CPipeLine* Create();
	virtual void Free() override;
};

END