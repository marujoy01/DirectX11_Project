#pragma once

#include "Engine_Defines.h"

/* ���۷���ī��Ʈ�� �����ϱ����� ����� �����Ѵ�. */

BEGIN(Engine)

class ENGINE_DLL CBase abstract
{
protected:
	CBase();
	virtual ~CBase() = default;

public:
	/* ���۷��� ī��Ʈ�� ������Ű��. */
	/* _ulong : ������Ű�� �� ������ ���� ����. */
	_ulong AddRef();

	/* ���۷��� ī��Ʈ�� ���ҽ�Ű�ų� �����ϰų�.. */
	/* _ulong : ���ҽ�Ű�� ������ ���� ���� .*/
	_ulong Release();

private:
	_ulong			m_dwRefCnt = { 0 };

public:
	virtual void Free() {}
};

END