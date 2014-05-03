#include <QtXml/QtXml>
#include <QtTest/QTest>
#include <QFile>
#include <QPainter>
#include "board.h"
#include "ics.h"

//----------------------------KSWITCH-----------------------
KSWITCH::KSWITCH(const QPainterPath& path /* = QPainterPath()*/,
	const QList<QPoint>& pinPosList /* = QList<QPoint>()*/,
	const QList<ITips>& tipsList /* = QList<ITips>()*/)
	: KBase(1, 1, 2, "SWITCH", "单刀开关", path, pinPosList, tipsList)
	, m_bClosed(false)
{
	m_switchPath.lineTo(26, 0);
	m_type = SWITCH;
	for (int i = 0; i < m_nPinNum; ++i)
	{
		m_pPinLevelList[i] = LOW;
	}
}

KSWITCH::~KSWITCH()
{
}

KSWITCH* KSWITCH::clone()
{
	return new KSWITCH(*this);
}

void KSWITCH::calculate()
{
	if (m_bClosed)
		m_pPinLevelList[1] = (LevelSignal)m_pPinLevelList[0];
	else
		m_pPinLevelList[1] = LOW;
}

bool KSWITCH::atSwitch(const QPoint& pos, int* index /* = NULL*/) const
{
	QPoint relativePos = pos - m_centerPos;
	if (relativePos.x() > -13 && relativePos.x() < 13 &&
		relativePos.y() > -17 && relativePos.y() < 0)
	{
		if (index)
			*index = 0;
		return true;
	}
	return false;
}

void KSWITCH::click(int index/* = 0*/)
{
	if (index != 0)
		return;
	m_bClosed = !m_bClosed;
	m_pPinLevelList[0] = m_pPinLevelList[0] == HIGH ? LOW : HIGH;
	setIn(0, m_pPinLevelList[0] == HIGH ? LOW : HIGH);
}

void KSWITCH::draw(QPainter& painter) const
{
	KBase::draw(painter);

	painter.save();
	painter.translate(m_centerPos + QPoint(-13, 0));
	painter.rotate(m_bClosed ? 0 : -45);
	painter.drawPath(m_switchPath);
	painter.restore();
}

//----------------------------KCLOCK-----------------------------

KCLOCK::KCLOCK(const QPainterPath& path /* = QPainterPath()*/,
	const QList<QPoint>& pinPosList /* = QList<QPoint>()*/,
	const QList<ITips>& tipsList /* = QList<ITips>()*/)
	: KBase(1, 1, 2, "CLOCK", "时钟脉冲", path, pinPosList, tipsList)
	, m_start(false)
	, m_time(500)
{
	m_type = SWITCH;
}

KCLOCK::KCLOCK(const KCLOCK& other)
	: KBase(other)
	, KSWITCHBase(other)
	, QThread()
	, m_time(other.m_time)
	, m_start(false)
{
}

KCLOCK::~KCLOCK()
{
}

KCLOCK* KCLOCK::clone()
{
	return new KCLOCK(*this);
}

void KCLOCK::calculate()
{
	m_pPinLevelList[1] = (LevelSignal)m_pPinLevelList[0];
}

bool KCLOCK::atSwitch(const QPoint& pos, int* index /* = NULL*/) const
{
	QPoint relativePos = pos - m_centerPos;
	if (relativePos.x() > -50 && relativePos.x() < -20 &&
		relativePos.y() > -25 && relativePos.y() < -2)
	{
		if (index)
			*index = 0;
		return true;
	}
	if (relativePos.x() > -50 && relativePos.x() < -20 &&
		relativePos.y() > 2 && relativePos.y() < 25)
	{
		if (index)
			*index = 1;
		return true;
	}
	return false;
}

void KCLOCK::click(int index/* = 0*/)
{
	if (index != 0 && index != 1)
		return;
	 if (index == 0)
	 {
		 if (!m_start)
			transmitPulseOnce();
	 }
	 else
	 {
		 if (m_start)
			m_start = false;
		 else
		 {
			 m_start = true;
			 start();
		 }
	 }
}

void KCLOCK::draw(QPainter& painter) const
{
	KBase::draw(painter);

	painter.translate(m_centerPos);
	painter.drawText(-48, -8, "step");
	if (m_start)
		painter.drawText(-48, 17, "stop");
	else
		painter.drawText(-48, 17, "start");
	painter.translate(-m_centerPos);
}

void KCLOCK::run()
{
	while (m_start)
	{
		transmitPulseOnce();
		QTest::qSleep(m_time);
	}
}

void KCLOCK::transmitPulseOnce()
{
	setIn(0, LOW);
	setIn(0, HIGH);
	m_pBoard->update();
}

//----------------------------Vcc------------------------------
KVcc::KVcc(const QPainterPath& path /* = QPainterPath()*/,
	const QList<QPoint>& pinPosList /* = QList<QPoint>()*/,
	const QList<ITips>& tipsList /* = QList<ITips>()*/)
	: KBase(0, 1, 1, "Vcc", "Vcc电源", path, pinPosList, tipsList)
{
	m_type = INIC;
}

KVcc::~KVcc()
{
}

KVcc* KVcc::clone()
{
	return new KVcc(*this);
}

void KVcc::calculate()
{
}

bool KVcc::contains(const QPoint& pos) const
{
	QPoint relativePos = pos - m_centerPos;
	if (relativePos.x() > -20 && relativePos.x() < 16 &&
		relativePos.y() > -10 && relativePos.y() < 10)
		return true;
	return false;
}


//-----------------------------KLED----------------------------

KLED::KLED(const QPainterPath& path /* = QPainterPath()*/,
	const QList<QPoint>& pinPosList /* = QList<QPoint>()*/,
	const QList<ITips>& tipsList /* = QList<ITips>()*/)
	: KBase(1, 0, 1, "LED", "发光二极管", path, pinPosList, tipsList)
{
	m_type = OUTIC;
	for (int i = 0; i < m_nPinNum; ++i)
	{
		m_pPinLevelList[i] = LOW;
	}
}

KLED::~KLED()
{
}

KLED* KLED::clone()
{
	return new KLED(*this);
}

void KLED::calculate()
{
}

void KLED::draw(QPainter& painter) const
{
	painter.save();
	if (m_pPinLevelList[0] == HIGH)
		painter.setBrush(QBrush(Qt::red));	
	else
		painter.setBrush(QBrush(QColor(255, 192, 203)));

	KBase::draw(painter);

	painter.translate(m_centerPos);
	painter.drawText(12, 4, m_pPinLevelList[0] == HIGH ? "1" : "0");
	painter.restore();
}

//-----------------------------KNode-------------------------------------
KNode::KNode(const QPainterPath& path /* = QPainterPath()*/,
	const QList<QPoint>& pinPosList /* = QList<QPoint>()*/,
	const QList<ITips>& tipsList /* = QList<ITips>()*/)
	: KBase(1, 1, 1, "NODE", "节点", path, pinPosList, tipsList)
{
	calculate();
}

KNode::~KNode()
{
}

KNode* KNode::clone()
{
	return new KNode(*this);
}

void KNode::calculate()
{
}

bool KNode::contains(const QPoint& pos) const
{
	QPoint relativePos = pos - m_centerPos;
	if (relativePos.x() > -10 && relativePos.x() < 10 &&
		relativePos.y() > -10 && relativePos.y() < 10 &&
		!KBase::contains(pos))
		return true;
	return false;
}

void KNode::draw(QPainter& painter) const
{
	painter.save();
	painter.setRenderHint(QPainter::Antialiasing, true); 
	painter.setPen(QPen(Qt::red, 6, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter.drawPoint(m_centerPos);
	painter.restore();
}

//----------------------------- 非门-----------------------------
K74LS04::K74LS04(const QPainterPath& path /* = QPainterPath()*/,
	const QList<QPoint>& pinPosList /* = QList<QPoint>()*/,
	const QList<ITips>& tipsList /* = QList<ITips>()*/)
	: KBase(1, 1, 2, "74LS04", "非门", path, pinPosList, tipsList)
{
	calculate();
}

K74LS04::~K74LS04()
{
}

K74LS04* K74LS04::clone()
{
	return new K74LS04(*this);
}

void K74LS04::calculate()
{
	m_pPinLevelList[1] = (LevelSignal)(!m_pPinLevelList[0]);
}

//----------------------------与门--------------------------
K74LS08::K74LS08(const QPainterPath& path /* = QPainterPath()*/,
	const QList<QPoint>& pinPosList /* = QList<QPoint>()*/,
	const QList<ITips>& tipsList /* = QList<ITips>()*/)
	: KBase(2, 1, 3, "74LS08", "2输入与门", path, pinPosList, tipsList)
{
	calculate();
}

K74LS08::~K74LS08()
{
}

K74LS08* K74LS08::clone()
{
	return new K74LS08(*this);
}

void K74LS08::calculate()
{
	m_pPinLevelList[2] = (LevelSignal)(m_pPinLevelList[0] && m_pPinLevelList[1]);
}

//----------------------------- 或门----------------------------
K74LS32::K74LS32(const QPainterPath& path /* = QPainterPath()*/,
	const QList<QPoint>& pinPosList /* = QList<QPoint>()*/,
	const QList<ITips>& tipsList /* = QList<ITips>()*/)
	: KBase(2, 1, 3, "74LS32", "2输入或门", path, pinPosList, tipsList)
{
	calculate();
}

K74LS32::~K74LS32()
{
}

K74LS32* K74LS32::clone()
{
	return  new K74LS32(*this);
}

void K74LS32::calculate()
{
	m_pPinLevelList[2] = (LevelSignal)(m_pPinLevelList[0] || m_pPinLevelList[1]);
}

//------------------------ 三态输出 74LS125--------------------------------
K74LS125::K74LS125(const QPainterPath& path /* = QPainterPath()*/,
	const QList<QPoint>& pinPosList /* = QList<QPoint>()*/,
	const QList<ITips>& tipsList /* = QList<ITips>()*/)
	: KBase(2, 1, 3, "74LS125", "三态输出", path, pinPosList, tipsList)
{
	calculate();
}

K74LS125::~K74LS125()
{
}

K74LS125* K74LS125::clone()
{
	return  new K74LS125(*this);
}

void K74LS125::calculate()
{
	if (m_pPinLevelList[1] == HIGH)
		m_pPinLevelList[2] = HIZ;
	else
	{
		m_pPinLevelList[2] = m_pPinLevelList[0];
	}
}

//----------------------------KUniversalIC-----------------------------
KUniversalIC::KUniversalIC(
	int nInNum,
	int nOutNum,
	int nPinNum,
	const QString& name, 
	const QString& description,
	const QList<KBase*>& componentList,
	const QList<IInToIn>& inToInList,
	const QList<IOutToOut>& outToOutList,
	const QList<IOutToIn>& outToInList,
	const QPainterPath& path /* = QPainterPath()*/,
	const QList<QPoint>& pinPosList /* = QList<QPoint>()*/,
	const QList<ITips>& tipsList /* = QList<ITips>()*/)
	: KBase(nInNum, nOutNum, nPinNum, name, description, path, pinPosList, tipsList)
	, m_componentList(componentList)
	, m_inToInList(inToInList)
	, m_outToOutList(outToOutList)
	, m_outToInList(outToInList)
{
	makeLinks();
	calculate();
}

KUniversalIC::KUniversalIC(const KUniversalIC& other)
	: KBase(other)
	, m_inToInList(other.m_inToInList)
	, m_outToOutList(other.m_outToOutList)
	, m_outToInList(other.m_outToInList)
{
	
	for (int i = 0; i < other.m_componentList.count(); ++i)
	{
		if (other.m_componentList[i])
			 m_componentList.append(other.m_componentList[i]->clone());
	}
	makeLinks();
}

KUniversalIC::~KUniversalIC()
{
	for (int i = 0; i < m_componentList.count(); ++i)
	{
		if (m_componentList[i])
		delete m_componentList[i];
	}
	m_componentList.clear();
}

void KUniversalIC::setIn(int num, LevelSignal val)
{
	if (!m_pPinLevelList || num < 0 || num >= m_nInPinNum)
	{	
		assert(!"给定下标超出，或m_pPinLevelList == NULL");
		return;
	}
	if (m_pPinLevelList[num] != val)
	{
		ISetLevel setLevel;

		LevelSignal* pOutList = new LevelSignal[m_nOutPinNum];
		for (int i = 0; i < m_nOutPinNum; ++i)//保存计算前输出电平
			pOutList[i] = m_pPinLevelList[i + m_nPinNum - m_nOutPinNum];

		m_pPinLevelList[num] = val;
		for (int i = 0; i < m_inToInList.count(); ++i)
		{ 
			if (m_inToInList[i].index == num)
			{
				setLevel.p = m_componentList[m_inToInList[i].target];
				setLevel.i = m_inToInList[i].targetIndex;
				setLevel.val = m_pPinLevelList[num];
				ms_setLevelQueue.append(setLevel);
			}
		}
		while (!ms_setLevelQueue.isEmpty())
		{
			setLevel = ms_setLevelQueue.dequeue();
			setLevel.p->setIn(setLevel.i, setLevel.val);
		}

		calculate();

		for (int i = 0; i < m_nOutPinNum; ++i)//发送输出电平变化信息
			if (pOutList[i] != m_pPinLevelList[i + m_nPinNum - m_nOutPinNum])
				levelChange(i + m_nPinNum - m_nOutPinNum);
		delete [] pOutList;

		while (!ms_setLevelQueue.isEmpty())
		{
			setLevel = ms_setLevelQueue.dequeue();
			setLevel.p->setIn(setLevel.i, setLevel.val);
		}
	}
}

KUniversalIC* KUniversalIC::clone()
{
	return new KUniversalIC(*this);
}

void KUniversalIC::calculate()
{
	int index, source, sourceIndex;
	for (int i = 0; i < m_outToOutList.count(); ++i)
	{
		index = m_outToOutList[i].index;
		source = m_outToOutList[i].source;
		sourceIndex = m_outToOutList[i].sourceIndex;
		m_pPinLevelList[index] = m_componentList[source]->get(sourceIndex);
	}
}


void KUniversalIC::makeLinks()
{
	IOutToIn outToIn;
	for (int i = 0; i < m_outToInList.count(); ++i)
	{
		outToIn = m_outToInList[i];
		m_componentList[outToIn.source]->appendLink(outToIn.sourceIndex, 
			m_componentList[outToIn.target], outToIn.targetIndex);
	}
}