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

bool KWire::contains(const QPoint& pos) const
{
	QPoint pos1 = m_pIC1->getPinPos(m_nIndex1);
	QPoint pos2 = m_pIC2->getPinPos(m_nIndex2);
	if (pos1.x() == pos2.x())
	{
		if (abs(pos.y() - (pos1.y() + pos2.y()) / 2) > abs(pos1.y() - pos2.y()) / 2)
			return false;
		return true;
	}
	else if (pos1.y() == pos2.y())
	{
		if (abs(pos.x() - (pos1.x() + pos2.x()) / 2) > abs(pos1.x() - pos2.x()) / 2)
			return false;
		return true;
	}
	else
	{
		if (pos.x() > (pos1.x() > pos2.x() ? pos1.x() : pos2.x()) ||
			pos.x() < (pos1.x() < pos2.x() ? pos1.x() : pos2.x()) ||
			pos.y() > (pos1.y() > pos2.y() ? pos1.y() : pos2.y()) ||
			pos.y() < (pos1.y() < pos2.y() ? pos1.y() : pos2.y()))
		return false;
		double k = (double)(pos1.y() - pos2.y()) / (pos1.x() - pos2.x());
		if (abs(k * (pos.x() - pos1.x()) + pos1.y() - pos.y()) < 4)
			return true;
		if (abs((pos.y() - pos1.y()) / k + pos1.x() - pos.x()) < 4)
			return true;
	}
	return false;
}

void KWire::draw(QPainter& painter)
{
	painter.save();
	painter.setRenderHint(QPainter::Antialiasing, true);
	if (m_bLegal)
		painter.setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	else
		painter.setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter.drawLine(m_pIC1->getPinPos(m_nIndex1), m_pIC2->getPinPos(m_nIndex2));
	painter.restore();
}

bool KWire::inWire(KBase* pIC)
{
	return pIC == m_pIC1 || pIC == m_pIC2;
}

void KWire::get(KBase** pIC1, int* index1, 
	KBase** pIC2, int* index2) const
{
	*pIC1 = m_pIC1;
	*index1 = m_nIndex1;
	*pIC2 = m_pIC2;
	*index2 = m_nIndex2;
}

bool KWire::createLink()
{
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
