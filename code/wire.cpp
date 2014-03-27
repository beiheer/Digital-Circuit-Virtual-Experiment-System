#include <QPainter>
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
	m_bLegal = createLink();
}

KWire::~KWire()
{
	if (m_bLegal)
	{
		ILink link = {m_nIndex1, m_pIC2, m_nIndex2};
		m_pIC1->removeLink(link);
	}
}

bool KWire::inWire(KBase* pIC)
{
	return pIC == m_pIC1 || pIC == m_pIC2;
}

void KWire::draw(QPainter& painter)
{
	painter.save();
	painter.setRenderHint(QPainter::Antialiasing, true);
	if (m_bLegal)
		painter.setPen(Qt::blue);
	else
		painter.setPen(Qt::red);
	painter.drawLine(m_pIC1->getPinPos(m_nIndex1), m_pIC2->getPinPos(m_nIndex2));
	painter.restore();
}

bool KWire::createLink()
{
	if (!m_pIC1 || !m_pIC2)
		return false;
	bool succeed = m_pIC1->appendLink(m_nIndex1, m_pIC2, m_nIndex2);
	if (succeed)
		return succeed;
	swap();
	succeed = m_pIC1->appendLink(m_nIndex1, m_pIC2, m_nIndex2);
	return succeed;
}

void KWire::swap()
{
	KBase* pICTemp = m_pIC1;
	int nIndexTemp = m_nIndex1;
	m_pIC1 = m_pIC2;
	m_nIndex1 = m_nIndex2;
	m_pIC2 = pICTemp;
	m_nIndex2 = nIndexTemp;
}
