#include "base.h"
#include "wire.h"
KWire::KWire()
	: m_pIC1(NULL)
	, m_pIC2(NULL)
	, m_nIndex1(-1)
	, m_nIndex2(-1)
	, m_bLegal(false)
{
}

KWire::KWire(KBase* pIC1, int nIndex1, KBase* pIC2, int nIndex2)
	: m_pIC1(pIC1)
	, m_pIC2(pIC2)
	, m_nIndex1(nIndex1)
	, m_nIndex2(nIndex2)
{
	if (m_pIC1)
		m_bLegal = m_pIC1->appendLink(m_nIndex1, m_pIC2, m_nIndex2);
}

KWire::~KWire()
{
	if (m_bLegal)
	{
		ILink link = {m_nIndex1, m_pIC2, m_nIndex2};
		m_pIC1->removeLink(link);
	}
}