#include <QtXml/QtXml>
#include <QFile>
#include <QPainter>
#include "board.h"
#include "ics.h"

//----------------------------KPower-----------------------
KPower::KPower(const QPainterPath& path /* = QPainterPath()*/,
	const QList<IPinPos>& pinPosList /* = QList<IPinPos>()*/)
	: KBase(1, 1, "POWER", "输入开关", path, pinPosList)
{
	m_switchPath.addRect(-40, -10, 22, 20);
	m_pPinLevelList[0] = m_pPinLevelList[1] = LOW;
}

KPower::~KPower()
{
}

KPower* KPower::clone()
{
	return new KPower(*this);
}

void KPower::calculate()
{
	m_pPinLevelList[1] = (LevelSignal)m_pPinLevelList[0];
}

bool KPower::onSwitch(const QPoint& pos) const
{
	return m_switchPath.contains(pos - m_centerPos);
}

void KPower::click()
{
	setIn(0, m_pPinLevelList[0] == HIGH ? LOW : HIGH);
}

void KPower::draw(QPainter& painter) const
{
	painter.translate(m_centerPos);
	painter.drawPath(m_switchPath);
	painter.drawPath(m_path);
	painter.drawText(-30, 3, m_pPinLevelList[1] == HIGH ? "1" : "0");
	painter.translate(-m_centerPos);
}
//-----------------------------KLED----------------------------

KLED::KLED(const QPainterPath& path /* = QPainterPath()*/,
	const QList<IPinPos>& pinPosList /* = QList<IPinPos>()*/)
	: KBase(1, 0, "LED", "发光二极管", path, pinPosList)
{
}

KLED::~KLED()
{
}

KLED* KLED::clone()
{
	return new KLED(*this);
}

void KLED::draw(QPainter& painter) const
{
	painter.save();

	if (m_pPinLevelList[0] == HIGH)
		painter.setBrush(QBrush(Qt::red));	
	else
		painter.setBrush(QBrush(QColor(255, 192, 203)));
	painter.translate(m_centerPos);
	painter.drawPath(m_path);
	painter.drawText(0, 3, m_pPinLevelList[0] == HIGH ? "1" : "0");
	painter.restore();
}

//----------------------------与门--------------------------
K74LS08::K74LS08(const QPainterPath& path /* = QPainterPath()*/,
	const QList<IPinPos>& pinPosList /* = QList<IPinPos>()*/)
	: KBase(2, 1, "74LS08", "2输入与门", path, pinPosList)
{
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

//-------------------------- 3输入与门----------------------------
K74LS11::K74LS11(const QPainterPath& path /* = QPainterPath()*/,
	const QList<IPinPos>& pinPosList /* = QList<IPinPos>()*/)
	: KBase(3, 1, "74LS11", "3输入与门", path, pinPosList)
{
}

K74LS11::~K74LS11()
{
}

K74LS11* K74LS11::clone()
{
	return new K74LS11(*this);
}

void K74LS11::calculate()
{
	m_pPinLevelList[3] = (LevelSignal)(
		m_pPinLevelList[0] && m_pPinLevelList[1] && m_pPinLevelList[2]);
}

//-------------------------- 4输入与门---------------------------
K74LS21::K74LS21(const QPainterPath& path /* = QPainterPath()*/,
	const QList<IPinPos>& pinPosList /* = QList<IPinPos>()*/)
	: KBase(4, 1, "74LS21", "4输入与门", path, pinPosList)
{
}

K74LS21::~K74LS21()
{
}

K74LS21* K74LS21::clone()
{
	return new K74LS21(*this);
}

void K74LS21::calculate()
{
	m_pPinLevelList[4] = (LevelSignal)(
		m_pPinLevelList[0] &&m_pPinLevelList[1] &&
		m_pPinLevelList[2] && m_pPinLevelList[3]);
}

//----------------------------- 或门----------------------------
K74LS32::K74LS32(const QPainterPath& path /* = QPainterPath()*/,
	const QList<IPinPos>& pinPosList /* = QList<IPinPos>()*/)
	: KBase(2, 1, "74LS32", "2输入或门", path, pinPosList)
{
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

//----------------------------- 非门-----------------------------
K74LS04::K74LS04(const QPainterPath& path /* = QPainterPath()*/,
	const QList<IPinPos>& pinPosList /* = QList<IPinPos>()*/)
	: KBase(1, 1, "74LS04", "非门", path, pinPosList)
{
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

//----------------------------- 与非门----------------------------
K74LS00::K74LS00(const QPainterPath& path /* = QPainterPath()*/,
	const QList<IPinPos>& pinPosList /* = QList<IPinPos>()*/)
	: KBase(2, 1, "74LS00", "与非门", path, pinPosList)
{
}

K74LS00::~K74LS00()
{
}

K74LS00* K74LS00::clone()
{
	return new K74LS00(*this);
}

void K74LS00::calculate()
{
	m_pPinLevelList[2] = (LevelSignal)(!(m_pPinLevelList[0] && m_pPinLevelList[1]));
}

//-------------------------- 3输入与非门----------------------------
K74LS10::K74LS10(const QPainterPath& path /* = QPainterPath()*/,
	const QList<IPinPos>& pinPosList /* = QList<IPinPos>()*/)
	: KBase(3, 1, "74LS10", "3输入与非门", path, pinPosList)
{
}

K74LS10::~K74LS10()
{
}

K74LS10* K74LS10::clone()
{
	return new K74LS10(*this);
}

void K74LS10::calculate()
{
	m_pPinLevelList[3] = (LevelSignal)(
		!(m_pPinLevelList[0] && m_pPinLevelList[1] && m_pPinLevelList[2]));
}

//-------------------------- 4输入与非门----------------------------
K74LS20::K74LS20(const QPainterPath& path /* = QPainterPath()*/,
	const QList<IPinPos>& pinPosList /* = QList<IPinPos>()*/)
	: KBase(4, 1, "74LS20", "4输入与非门", path, pinPosList)
{
}

K74LS20::~K74LS20()
{
}

K74LS20* K74LS20::clone()
{
	return new K74LS20(*this);
}

void K74LS20::calculate()
{
	m_pPinLevelList[4] = (LevelSignal)(
		!(m_pPinLevelList[0] && m_pPinLevelList[1] &&
		m_pPinLevelList[2] && m_pPinLevelList[3]));
}

//----------------------------- 或非门----------------------------
K74LS02::K74LS02(const QPainterPath& path /* = QPainterPath()*/,
	const QList<IPinPos>& pinPosList /* = QList<IPinPos>()*/)
	: KBase(2, 1, "74LS02", "或非门", path, pinPosList)
{
}

K74LS02::~K74LS02()
{
}

K74LS02* K74LS02::clone()
{
	return new K74LS02(*this);
}

void K74LS02::calculate()
{
	m_pPinLevelList[2] = (LevelSignal)(!(m_pPinLevelList[0] || m_pPinLevelList[1]));
}

//-------------------------3输入或非门---------------------------
K74LS27::K74LS27(const QPainterPath& path /* = QPainterPath()*/,
	const QList<IPinPos>& pinPosList /* = QList<IPinPos>()*/)
	: KBase(3, 1, "74LS27", "3输入或非门", path, pinPosList)
{
}

K74LS27::~K74LS27()
{
}

K74LS27* K74LS27::clone()
{
	return new K74LS27(*this);
}

void K74LS27::calculate()
{
	m_pPinLevelList[3] = (LevelSignal)(
		!(m_pPinLevelList[0] || m_pPinLevelList[1] || m_pPinLevelList[2]));
}

//-------------------------4输入或非门--------------------------
KCD4002::KCD4002(const QPainterPath& path /* = QPainterPath()*/,
	const QList<IPinPos>& pinPosList /* = QList<IPinPos>()*/)
	: KBase(4, 1, "CD4002", "4输入或非门",path, pinPosList)
{
}

KCD4002::~KCD4002()
{
}

KCD4002* KCD4002::clone()
{
	return new KCD4002(*this);
}

void KCD4002::calculate()
{
	m_pPinLevelList[4] = (LevelSignal)(
		!(m_pPinLevelList[0] || m_pPinLevelList[1] || 
		m_pPinLevelList[2] || m_pPinLevelList[3]));
}

//---------------------------- 异或门 --------------------------
K74LS86::K74LS86(const QPainterPath& path /* = QPainterPath()*/,
	const QList<IPinPos>& pinPosList /* = QList<IPinPos>()*/)
	: KBase(2, 1, "74LS86", "异或门", path, pinPosList)
{
}

K74LS86::~K74LS86()
{
}

K74LS86* K74LS86::clone()
{ 
	return new K74LS86(*this);
}

void K74LS86::calculate()
{
	m_pPinLevelList[2] = (LevelSignal)(
		(!m_pPinLevelList[0] && m_pPinLevelList[1]) ||
		(m_pPinLevelList[0] && !m_pPinLevelList[1]));
}

//----------------------------KUniversalIC-----------------------------
KUniversalIC::KUniversalIC(
	int nInNum,
	int nOutNum,
	const QString& name, 
	const QString& description,
	const QList<KBase*>& componentList,
	const QList<IInToIn>& inToInList,
	const QList<IOutToOut>& outToOutList,
	const QList<IOutToIn>& outToInList,
	const QPainterPath& path /* = QPainterPath()*/,
	const QList<IPinPos>& pinPosList /* = QList<IPinPos>()*/)
	: KBase(nInNum, nOutNum, name, description, path, pinPosList)
	, m_componentList(componentList)
	, m_inToInList(inToInList)
	, m_outToOutList(outToOutList)
	, m_outToInList(outToInList)
{
	makeLinks();
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
		LevelSignal* pOutList = new LevelSignal[m_nOutPinNum];
		for (int i = 0; i < m_nOutPinNum; ++i)//保存计算前输出电平
			pOutList[i] = m_pPinLevelList[i + m_nInPinNum];


		for (int i = 0; i < m_inToInList.count(); ++i)
		{ 
			if (m_inToInList[i].index == num)
				m_componentList[m_inToInList[i].target]->setIn(
				m_inToInList[i].targetIndex, m_pPinLevelList[num]);
		}
		m_pPinLevelList[num] = val;
		calculate();
		for (int i = 0; i < m_nOutPinNum; ++i)//发送输出电平变化信息
			if (pOutList[i] != m_pPinLevelList[i + m_nInPinNum])
				sendChange(i + m_nInPinNum);
		delete [] pOutList;
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