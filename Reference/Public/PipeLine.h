#pragma once

/* �ΰ��� �� ��ü���� �׸��� �������� ����ϱ����� ��, ��������� �����Ѵ�. */
// [�ʿ伺]
/* ī�޶� �ٶ󺸴� ������ ����� ���ؼ�, ��� ��ü���� �׷����� ī�޶��� ���� ����� (�� �����̽�)�� �ʿ��ϴ�. */
/* ����, ���� ������� (��, �Ȱ�, ������Ʈ ���)������ ī�޶��� ��ġ�� ���� �䱸�Ұ��̴�. */
// [���� ����]
/* Dx9���� ��ġ�� ����� �����Ͽ� ��𼭵� ���� �� �� �־����� Dx11���� ��ġ�� �׷� ����̾���. ���� �� ����� �Ȱ��� PipeLine�̶�� �༮�� ����� ����� ���̴�. */
// [���]
/* �̷��� ���� PipeLine Ŭ������ ī�޶� ����� �����ϰ�, ���Ҷ� ������ �� �ְ� ����� �ʿ��Ѱ����� ��������. */
#include "Base.h"

BEGIN(Engine)

class CPipeLine final : public CBase
{
public:	// ����� �迭�� �����Ұ��̴�, ���Ǽ��� ���� enum���� ID�� ��������.
	//							 ��	   ,	����   
	enum D3DTRANSFORMSTATE { D3DTS_VIEW, D3DTS_PROJ, D3DTS_END };
private:
	CPipeLine();
	virtual ~CPipeLine() = default;

public: // Set : ī�޶� ���� �߰�
	void Set_Transform(D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix);
	void Set_Transform(D3DTRANSFORMSTATE eState, _float4x4 TransformMatrix);

public:	// Get : ī�޶� ���� ��ȯ
	_matrix Get_TransformMatrix(D3DTRANSFORMSTATE eState);
	_float4x4 Get_TransformFloat4x4(D3DTRANSFORMSTATE eState);
	_matrix Get_TransformMatrixInverse(D3DTRANSFORMSTATE eState);
	_float4x4 Get_TransformFloat4x4Inverse(D3DTRANSFORMSTATE eState);
	_float4 Get_CamPosition();

public:
	HRESULT Initialize();

	void Tick();	// ������Ʈ
	/*	�� ī�޶� ������Ʈ�� ������?
		- ī�޶��� ������� ī�޶󿡼� ����Ͽ� ��ȯ�Ұ��ε�, ������� ��ȯ�ϴ� Get�Լ��� ȣ���Ҷ����� �Լ������� ������ �̷�����ԵǸ� ���귮�� �������Եȴ�.
		- ���� ������ �ּ�ȭ �ϱ����� Tick���� ����� �� ��, ����� ��� ������ �����ϰ�, Get�Լ����� ���� ����ִ� ��� ������ ��ȯ��Ű�°�
		- ī�޶� ��ġ�� ���� ����
	*/

private:
	_float4x4			m_Transform[D3DTS_END];			// ���
	_float4x4			m_Transform_Inverse[D3DTS_END];	// �����
	_float4				m_vCamPosition;					// ī�޶� ��ġ

public:
	static CPipeLine* Create();
	virtual void Free() override;
};

END