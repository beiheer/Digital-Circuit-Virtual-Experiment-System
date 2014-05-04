#include <QPainter>
#include "base.h"
#include "wire.h"

KWire::KWire(KBase* begin, int beginPinIndex, KBase* end, int endPinIndex,
	const QList<QPoint>& pointList)
	: m_begin(begin)
	, m_end(end)
	, m_beginPinIndex(beginPinIndex)
	, m_endPinIndex(endPinIndex)
	, m_pointList(pointList)
{
	m_bLegal = createLink();
}

KWire::~KWire()
{
	if (m_bLegal)
	{
		ILink link = {m_beginPinIndex, m_end, m_endPinIndex};
		m_begin->removeLink(link);
	}
}

bool KWire::contains(const QPoint& pos, int* pPart/*=NULL*/) const
{
	for (int i = 0; i < m_pointList.count() - 1; ++i)
	{
		if (between(m_pointList[i], m_pointList[i + 1], pos))
		{
			if (pPart)
				*pPart = i;
			return true;
		}
	}
	return false;
}

int KWire::orientation(int part) const
{
	if (part < 0 || part >= m_pointList.count() - 1)
		return -1;
	if (m_pointList[part].y() == m_pointList[part + 1].y())
		return 0;
	if (m_pointList[part].x() == m_pointList[part + 1].x())
		return 1;
	return -1;
}

bool KWire::inWire(KBase* pIC) const
{
	return pIC == m_begin || pIC == m_end;
}

void KWire::offsetPart(const QPoint& offset, int part)
{
	assert(part >=0 && part < m_pointList.count() - 1);

	QPoint p1, p2, newP1, newP2;
	int partNum = m_pointList.count() - 1;

	getPoint(part, p1, p2);
	getPoint(part, offset, newP1, newP2);

	m_pointList[part] = newP1;
	m_pointList[part + 1] = newP2;
	
	if (part == 0)
		m_pointList.prepend(p1);
	if (part == partNum - 1)
		m_pointList.append(p2);
}

void KWire::draw(QPainter& painter)
{
	painter.save();
	if (m_bLegal)
		painter.setPen(
			QPen(Qt::blue, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	else
		painter.setPen(
			QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

	for (int i = 0; i < m_pointList.count() - 1; ++i)
	{
		if (m_pointList[i] != m_pointList[i + 1])
			painter.drawLine(m_pointList[i], m_pointList[i + 1]);
	}
	painter.restore();
}

void KWire::drawPoint(QPainter& painter)
{
	painter.save();
	painter.setPen(QPen(Qt::red, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

	for (int i = 0; i < m_pointList.count(); ++i)
	{
		painter.drawPoint(m_pointList[i]);
	}

	painter.restore();
}

void KWire::get(
	KBase** begin, int* beginPinIndex, 
	KBase** end, int* endPinIndex) const
{
	*begin = m_begin;
	*beginPinIndex = m_beginPinIndex;
	*end = m_end;
	*endPinIndex = m_endPinIndex;
}


void KWire::getPoint(int part, QPoint& p1, QPoint& p2) const
{
	assert(part >=0 && part < m_pointList.count() - 1);
	if (part < 0 || part >= m_pointList.count() - 1)
		return;
	p1 = m_pointList[part];
	p2 = m_pointList[part + 1];
}

void KWire::getPoint(int part, const QPoint& offset, 
	QPoint& p1, QPoint& p2) const
{
	assert(part >=0 && part < m_pointList.count() - 1);
	if (part < 0 || part >= m_pointList.count() - 1)
		return;
	int ornt = orientation(part);
	if (ornt == 0)
	{
		p1 = m_pointList[part] + QPoint(0, offset.y());
		p2 = m_pointList[part + 1] + QPoint(0, offset.y());
	}
	else if (ornt == 1)
	{
		p1 = m_pointList[part] + QPoint(offset.x() ,0);
		p2 = m_pointList[part + 1] + QPoint(offset.x(), 0);
	}
}

const QList<QPoint>& KWire::pointList() const
{
	return m_pointList;
}

void KWire::setPointList(const QList<QPoint>& pointList)
{
	m_pointList = pointList;
}

bool KWire::createLink()
{
	bool succeed = m_begin->appendLink(m_beginPinIndex, m_end, m_endPinIndex);
	if (succeed)
		return succeed;
	swap();
	succeed = m_begin->appendLink(m_beginPinIndex, m_end, m_endPinIndex);
	return succeed;
}

void KWire::swap()
{
	KBase* pICTemp = m_begin;
	int nIndexTemp = m_beginPinIndex;
	m_begin = m_end;
	m_beginPinIndex = m_endPinIndex;
	m_end = pICTemp;
	m_endPinIndex = nIndexTemp;
}

bool KWire::between(const QPoint& pos1, const QPoint& pos2, 
	const QPoint& pos) const
{
	if (pos1.x() == pos2.x())
 	{
 		if (between(pos1.y(), pos2.y(), pos.y()) 
			&& abs(pos.x() - pos1.x()) < 4)
 			return true;
 	}
 	else if (pos1.y() == pos2.y())
 	{
		if (between(pos1.x(), pos2.x(), pos.x()) 
			&& abs(pos.y() - pos1.y()) < 4)
			return true;
 	}
	return false;
}

bool KWire::between(int num1, int num2, int num) const
{
	if (num1 > num2)
	{
		if (num1 > num && num > num2)
			return true;
	}
	else
	{
		if (num2 > num && num > num1)
			return true;
	}
	return false;
}