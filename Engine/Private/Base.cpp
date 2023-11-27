#include "..\Public\Base.h"

CBase::CBase()
{
}

_ulong CBase::AddRef()
{
	return ++m_dwRefCnt;
}

/* �����ϰų� ���ҽ�Ű�ų�. */
_ulong CBase::Release()
{
	if (0 == m_dwRefCnt)
	{
		/* �Ҹ���. */
		Free();

		delete this;

		return 0;
	}
	else
		return m_dwRefCnt--;
}


