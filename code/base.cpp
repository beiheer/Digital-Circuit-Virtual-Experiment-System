#include <QFile>
#include <QPainter>
#include "board.h"
#include "base.h"
//-------------------KBase------------------------

KBase::KBase(int nInNum, int nOutNum, int nPinNum,
	const QString& name, 
	const QString& description, 
	const QPainterPath& path, /*= QPainterPath()*/
	const QList<QPoint>& pinPosList, /*= QList<QPoint>()*/
	const QList<ITips>& tipsList /* = QList<ITips>()*/)
	: m_nInPinNum(nInNum)
	, m_nOutPinNum(nOutNum)
	, m_nPinNum(nPinNum)
	, m_sName(name)
	, m_sDescription(description)
	, m_path(path)
	, m_pinPosList(pinPosList)
	, m_tipsList(tipsList)
	, m_pBoard(0)
	, m_type(OTHERIC)
	, m_pinRadius(4)
{
	m_width = m_path.boundingRect().width();
	m_height =m_path.boundingRect().height();

	m_pPinLevelList = new LevelSignal[m_nPinNum];
	//将电平信号默认值设置为低电平
	for (int i = 0; i < m_nPinNum; ++i)
		m_pPinLevelList[i] = LOW;
}

KBase::KBase(const KBase& other)
	: m_nInPinNum(other.m_nInPinNum)
	, m_nOutPinNum(other.m_nOutPinNum)
	, m_nPinNum(other.m_nPinNum)
	, m_sName(other.m_sName)
	, m_sDescription(other.m_sDescription)
	, m_pPinLevelList(NULL)
	, m_path(other.m_path)
	, m_pinPosList(other.m_pinPosList)
	, m_tipsList(other.m_tipsList)
	, m_pBoard(other.m_pBoard)
	, m_type(other.m_type)
	, m_width(other.m_width)
	, m_height(other.m_height)
	, m_pinRadius(other.m_pinRadius)
{
	if (other.m_pPinLevelList)
	{
		m_pPinLevelList = new LevelSignal[m_nPinNum + 1];
		for (int i = 0; i < m_nPinNum; ++i)
			m_pPinLevelList[i] = other.m_pPinLevelList[i];
	}
}

KBase::~KBase()
{
	if (m_pPinLevelList)
		delete [] m_pPinLevelList;
}

int KBase::inPinNum() const 
{
	return m_nInPinNum;
}

int KBase::outPinNum() const 
{
	return m_nOutPinNum;
}

int KBase::pinNum() const 
{
	return m_nPinNum;
}

const QList<ILink>& KBase::links() const 
{
	return m_links;
}

const QString& KBase::name() const
{
	return m_sName;
}

const QString& KBase::description() const
{
	return m_sDescription;
}

const KBase::TYPE KBase::type() const
{
	return m_type;
}

void KBase::reset(LevelSignal val /* = NOSIGNAL */)
{
	for (int i = 0; i < m_nPinNum; ++i)
		m_pPinLevelList[i] = val;
}

LevelSignal KBase::get(int num /* = 0 */) const
{
	if (!m_pPinLevelList || num < 0 || num >= m_nPinNum)
		assert(false);
	return m_pPinLevelList[num];
}

LevelSignal KBase::getOut(int num /* = 0 */) const
{
	if (!m_pPinLevelList || num < 0 || num >= m_nOutPinNum)
		assert(false);
	return m_pPinLevelList[num + m_nPinNum - m_nOutPinNum];
}

LevelSignal KBase::getIn(int num /* = 0*/) const
{
	if (!m_pPinLevelList || num < 0 || num >= m_nInPinNum)
		assert(false);
	return m_pPinLevelList[num];
}

void KBase::setIn(int num, LevelSignal val)
{
	if (!m_pPinLevelList || num < 0 || num >= m_nInPinNum)
	{	
		assert(!"给定下标超出，或m_pPinLevelList == NULL");
		return;
	}
	if (m_pPinLevelList[num] != val)
	{
		LevelSignal* pOutList = new LevelSignal[m_nOutPinNum];
		for (int i = 0; i < m_nOutPinNum; ++i)//保存计算前输出电平
			pOutList[i] = m_pPinLevelList[i + m_nPinNum - m_nOutPinNum];

		m_pPinLevelList[num] = val;
		calculate();

		for (int i = 0; i < m_nOutPinNum; ++i)//发送输出电平变化信息
			if (pOutList[i] != m_pPinLevelList[i + m_nPinNum - m_nOutPinNum])
				sendChange(i + m_nPinNum - m_nOutPinNum);
		delete [] pOutList;
	}
}

bool KBase::appendLink(int i, KBase* p, int j)
{
	if (i < m_nPinNum - m_nOutPinNum || i >= m_nPinNum ||
		!p || j < 0 || j >= p->inPinNum() ||
		this == p)
	{
		return false;
	}
	ILink link = {i, p, j};
	if (m_links.contains(link))
		return false;
	m_links.append(link);
	link.p->setIn(link.j, m_pPinLevelList[link.i]);
	return true;
}

bool KBase::appendLink(const ILink& link)
{
	if (link.i < m_nPinNum - m_nOutPinNum || link.i >= m_nPinNum ||
		!link.p || link.j < 0 || link.j >= link.p->inPinNum() ||
		this == link.p)
	{
		return false;
	}
	if (m_links.contains(link))
		return false;
	m_links.append(link);
	link.p->setIn(link.j, m_pPinLevelList[link.i]);
	return true;
}

int KBase::LinkAt(const ILink& link)
{
	return m_links.indexOf(link);
}

void  KBase::removeLink(const ILink& link)
{
	m_links.removeOne(link);
}

void  KBase::removeLink(int index)
{
	m_links.removeAt(index);
}

void KBase::setBoard(KBoard* pBoard)
{
	m_pBoard = pBoard;
}

void KBase::offset(const QPoint& offset)
{
	m_centerPos.setX((m_centerPos.x() + offset.x()) > 0 ? 
		(m_centerPos.x() + offset.x()) : 0);
	m_centerPos.setY((m_centerPos.y() + offset.y()) > 0 ? 
		(m_centerPos.y() + offset.y()) : 0);
}

void KBase::setCenterPos(const QPoint& pos)
{
	m_centerPos.setX(pos.x() > 0 ? pos.x() : 0);
	m_centerPos.setY(pos.y() > 0 ? pos.y() : 0);
}

QRect KBase::getBoundingRect() const
{
	return m_path.boundingRect().toRect();
}

const QPoint& KBase::getCenterPos() const
{
	return m_centerPos;
}

const QPainterPath& KBase::getPath() const
{
	return m_path;
}

const QList<QPoint>& KBase::getPinPosList() const
{
	return m_pinPosList;
}

const QPoint KBase::getPinPos(int index) const
{
	assert(index >= 0 && index < m_nPinNum);
	return m_pinPosList[index] + m_centerPos;
}

int KBase::getWidth() const
{
	return m_width;
}

int KBase::getHeight() const
{
	return m_height;
}

bool KBase::contains(const QPoint& pos) const
{
	return m_path.contains(pos - m_centerPos);
}

int KBase::pinAt(const QPoint& pos) const
{
	QPoint relativePos = pos - m_centerPos; 
	for (int i = 0; i < m_pinPosList.count(); ++i)
	{
		if (relativePos.x() < m_pinPosList[i].x() + m_pinRadius &&
			relativePos.x() > m_pinPosList[i].x() - m_pinRadius &&
			relativePos.y() < m_pinPosList[i].y() + m_pinRadius &&
			relativePos.y() > m_pinPosList[i].y() - m_pinRadius)
		return i;
	}
	return -1;
}

void KBase::draw(QPainter& painter) const
{
	painter.translate(m_centerPos);

	painter.drawPath(m_path);
	for(int i = 0; i < m_tipsList.count(); ++i)
	{
		painter.drawText(m_tipsList[i].x, m_tipsList[i].y, m_tipsList[i].text);
	}

	painter.translate(-m_centerPos);
}

void KBase::sendChange(int num)
{
	if (m_pPinLevelList[num] != HIZ)//高阻
	{
		ILink link;
		for(int i = 0; i < m_links.count(); ++i)
		{
			link = m_links.at(i);
			if (num == link.i)
			{
				link.p->setIn(link.j, m_pPinLevelList[num]);
			}
		}	
	}
}
/*
void KBase::calculate()
{
}*/