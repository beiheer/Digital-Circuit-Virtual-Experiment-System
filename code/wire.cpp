#include <QPainter>
#include "base.h"
#include "wire.h"

//------------------------------KNode----------------------------------

KNode::KNode(KBase* pIC, int pinIndex, QPoint pos, FLAG flag/*= PIN*/)
	: m_pIC(pIC)
	, m_nPinIndex(pinIndex)
	, m_pos(pos)
	, m_flag(flag)
	, m_bHidden(false)
	, m_radius(4)
{
}

KNode::~KNode()
{
}

KBase* KNode::IC() const
{
	return m_pIC;
}

int KNode::pinIndex() const
{
	return m_nPinIndex;
}

KNode::FLAG KNode::flag() const
{
	return m_flag;
}

bool KNode::isHidden() const
{
	return m_bHidden;
}

void KNode::setHidden(bool val)
{
	m_bHidden = val;
}


QPoint KNode::pos() const
{
	return m_pos;
}

void KNode::setPos(const QPoint& pos)
{
	m_pos = pos;
}

bool KNode::contains(const QPoint& pos) const
{
	if (pos.x() < m_pos.x() + m_radius &&
		pos.x() > m_pos.x() - m_radius &&
		pos.y() < m_pos.y() + m_radius &&
		pos.y() > m_pos.y() - m_radius)
		return true;
	return false;
}

void KNode::draw(QPainter& painter)
{
	if (!m_bHidden)
	{
		painter.save();
		painter.setPen(QPen(Qt::red, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
		painter.drawPoint(m_pos);
		painter.restore();
	}
}

//------------------------KWire-------------------------------------------

KWire::KWire(KNode* begin, KNode* end, QList<QPoint> pointList)
	: m_begin(begin)
	, m_end(end)
	, m_pointList(pointList)
{
	m_bLegal = createLink();
}

KWire::~KWire()
{
	if (m_bLegal)
	{
		ILink link = {m_begin->pinIndex(), m_end->IC(), m_end->pinIndex()};
		m_begin->IC()->removeLink(link);
	}
}

bool KWire::contains(const QPoint& pos) const
{
	for (int i = 0; i < m_pointList.count() - 1; ++i)
	{
		if (between(m_pointList[i], m_pointList[i + 1], pos))
			return true;
	}
	return false;
}

void KWire::draw(QPainter& painter)
{
	painter.save();
	if (m_bLegal)
		painter.setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	else
		painter.setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

	for (int i = 0; i < m_pointList.count() - 1; ++i)
	{
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

bool KWire::inWire(KBase* pIC)
{
	return pIC == m_begin->IC() || pIC == m_end->IC();
}

bool KWire::inWire(KNode* pNode)
{
	return pNode == m_begin || pNode == m_end;
}

void KWire::get(KNode** begin, KNode** end) const
{
	*begin = m_begin;
	*end = m_end;
}

bool KWire::createLink()
{
	bool succeed = m_begin->IC()->appendLink(m_begin->pinIndex(), 
		m_end->IC(), m_end->pinIndex());
	if (succeed)
		return succeed;
	swap();
	succeed = m_begin->IC()->appendLink(m_begin->pinIndex(), 
		m_end->IC(), m_end->pinIndex());
	return succeed;
}

void KWire::swap()
{
	KNode* nodeTemp = m_begin;
	m_begin = m_end;
	m_end = nodeTemp;
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