#pragma once

#include "Client_Defines.h"
#include "Level.h"


/* �ε� ������ �����ְ� ��������� �����ϰ� �����ش�. */

/* m_eNextLevelID�� ���� ������ �η��� �ڿ��� �ֺ��� ������ �۾��� �ϴ� �����带 �������ش�. . */

BEGIN(Client)

class CLevel_Loading final : public CLevel
{
private:
	CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Loading() = default;

public:
	virtual HRESULT Initialize(LEVEL eNextLevelID);
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	LEVEL			m_eNextLevelID = { LEVEL_END };
	class CLoader*	m_pLoader = { nullptr };
	
public:
	static CLevel_Loading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID);
	virtual void Free() override;
};

END