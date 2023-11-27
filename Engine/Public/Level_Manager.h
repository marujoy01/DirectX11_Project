#pragma once

#include "Base.h"

/* ���� ������ ƽ, ������ ������ ȣ���Ѵ�. */

/* Open_Level() */
/* ���� ȭ�鿡 ��������� ������ �ּҸ� ���´�. */
/* ���� ������ �����Ѵ�. */
/* ���� ������ �ڿ����� �����Ѵ�. */

BEGIN(Engine)

class CLevel_Manager final : public CBase
{
private:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;

public:
	HRESULT Initialize();
	void Tick(_float fTimeDelta);
	HRESULT Render();

public:
	HRESULT Open_Level(_uint iCurrentLevelIndex, class CLevel* pNewLevel);

private:
	_uint						m_iCurrentLevelIndex = { 0 };
	class CLevel*				m_pCurrentLevel = { nullptr };
	class CGameInstance*		m_pGameInstance = { nullptr };

public:
	static CLevel_Manager* Create();
	virtual void Free() override;
};

END