#include <math.h>
#include <QBitArray>
#include <QDebug>
#include "sram.h"

KSRam::KSRam(int nInNum, int nOutNum,
	int nAddrNum, int nPinNum,
	const QString &name, 
	const QString &description, 
	const QPainterPath& path /* = QPainterPath */,
	const QList<QPoint>& pinPosList /*= QList<QPoint>()*/,
	const QList<ITips>& tipsList /* = QList<ITips>()*/)
	: KBase(nInNum, nOutNum, nPinNum, name, description, path, pinPosList, tipsList)
	, m_nAddrNum(nAddrNum)
{
	m_nRowNum = 0;
	m_nColumnNum = 0;

	int nTemp = (int)pow(2.0, nAddrNum);
	m_pMemoryArray = new QBitArray(nTemp*nOutNum);

	m_pOutLevel = new LevelSignal[nOutNum];
}

KSRam::KSRam(const KSRam &other)
	: KBase(other)
	, m_nAddrNum(other.m_nAddrNum)
{
	m_nRowNum = other.m_nRowNum;
	m_nColumnNum = other.m_nColumnNum;

	int nTemp = (int)pow(2.0, m_nAddrNum);
	m_pMemoryArray = new QBitArray(nTemp*m_nOutPinNum);

	m_pOutLevel = new LevelSignal[m_nOutPinNum];
}

KSRam::~KSRam()
{
	delete m_pMemoryArray;
	delete m_pOutLevel;
}

void KSRam::setIn(int num, LevelSignal val)
{
	if (num >= m_nPinNum - m_nOutPinNum && num < m_nPinNum)
		m_pOutLevel[num - (m_nPinNum - m_nOutPinNum)] = val;
	else
		KBase::setIn(num, val);
}

void KSRam::calculate()
{
	if (m_pPinLevelList[m_nAddrNum+1] == LOW)
	{
		if (m_pPinLevelList[m_nAddrNum] == HIGH)
		{
			read();
		} 
		else if (m_pPinLevelList[m_nAddrNum] == LOW)
		{
			write();
		}
	}
}

int KSRam::addrDecoder()
{
	int nRAddr = 0;	//ÐÐµØÖ·
	int nCAddr = 0;	//ÁÐµØÖ·

	for (int i = 0; i < m_nRowNum; i++)
	{
		nRAddr += m_pPinLevelList[0+i] * (int)pow(2.0, m_nRowNum-1-i);
	}
	for (int i = 0; i < m_nColumnNum; i++)
	{
		nRAddr += m_pPinLevelList[m_nRowNum+i] * (int)pow(2.0, m_nColumnNum-1-i);
	}
	
	int nAddr = (nRAddr * (int)pow(2.0, m_nColumnNum) + nCAddr) * m_nOutPinNum;

	return nAddr >= m_pMemoryArray->size() ? 0 : nAddr;
}

void KSRam::read()
{
	int nAddr = addrDecoder();
	int nTemp = m_nPinNum - m_nOutPinNum;
	for (int i = 0; i < m_nOutPinNum; i++)
	{
		m_pPinLevelList[nTemp+i] = (LevelSignal)m_pMemoryArray->at(nAddr+i);
	}
}

void KSRam::write()
{
	int nAddr = addrDecoder();
	for (int i = 0; i < m_nOutPinNum; i++)
	{
		m_pMemoryArray->setBit(nAddr+i, m_pOutLevel[i]);
	}
}

//---------------------------- SRAMÐ¾Æ¬2114 --------------------------

KSRam2114::KSRam2114(const QPainterPath& path /*= QPainterPath() */,
	const QList<QPoint>& pinPosList /*= QList<QPoint>() */,
	const QList<ITips>& tipsList /* = QList<ITips>()*/)
	: KSRam(16, 4, 10, 16, "SRAM2114", "¾²Ì¬´æ´¢Æ÷Ð¾Æ¬2114 1K*4Î»", path, pinPosList, tipsList)
{
	m_nRowNum = 6;
	m_nColumnNum = 4;
	//calculate();
}

KSRam2114::~KSRam2114()
{

}

KSRam2114* KSRam2114::clone()
{
	return new KSRam2114(*this);
}

// void KSRam2114::calculate()
// {
// 	if (m_pPinLevelList[m_nAddrNum+1] == LOW)
// 	{
// 		if (m_pPinLevelList[m_nAddrNum] == HIGH)
// 		{
// 			read();
// 		} 
// 		else if (m_pPinLevelList[m_nAddrNum] == LOW)
// 		{
// 			write();
// 		}
// 	}
// }

//---------------------------- SRAMÐ¾Æ¬6116 --------------------------

KSRam6116::KSRam6116(const QPainterPath& path /*= QPainterPath() */,
	const QList<QPoint>& pinPosList /*= QList<QPoint>() */,
	const QList<ITips>& tipsList /* = QList<ITips>()*/)
	: KSRam(22, 8, 11, 22, "SRAM6116", "¾²Ì¬´æ´¢Æ÷Ð¾Æ¬6116 2K*8Î»", path, pinPosList, tipsList)
{
	m_nRowNum = 7;
	m_nColumnNum = 4;
	//calculate();
}

KSRam6116::~KSRam6116()
{

}

KSRam6116* KSRam6116::clone()
{
	return new KSRam6116(*this);
}

void KSRam6116::calculate()
{
	if (m_pPinLevelList[13] == LOW)
	{
		if (m_pPinLevelList[12] == HIGH && 
			m_pPinLevelList[11] == LOW)
		{
			read();
		} 
		else if (m_pPinLevelList[12] == LOW)
		{
			write();
		}
	}
}

//---------------------------- SRAMÐ¾Æ¬6264 --------------------------

KSRam6264::KSRam6264(const QPainterPath& path /*= QPainterPath() */,
	const QList<QPoint>& pinPosList /*= QList<QPoint>() */,
	const QList<ITips>& tipsList /* = QList<ITips>()*/)
	: KSRam(25, 8, 13, 25, "SRAM6264", "HM6264B Series 8K*8Î»", path, pinPosList, tipsList)
{
	m_nRowNum = 9;
	m_nColumnNum = 4;
	//calculate();
}

KSRam6264::~KSRam6264()
{

}

KSRam6264* KSRam6264::clone()
{
	return new KSRam6264(*this);
}

void KSRam6264::calculate()
{
	if (m_pPinLevelList[16] == LOW && 
		m_pPinLevelList[15] == HIGH)
	{
		if (m_pPinLevelList[14] == HIGH && 
			m_pPinLevelList[13] == LOW)
		{
			read();
		} 
		else if (m_pPinLevelList[14] == LOW)
		{
			write();
		}
	}
}

//---------------------------- SRAMÐ¾Æ¬62256 --------------------------

KSRam62256::KSRam62256(const QPainterPath& path /*= QPainterPath() */,
	const QList<QPoint>& pinPosList /*= QList<QPoint>() */,
	const QList<ITips>& tipsList /* = QList<ITips>()*/)
	: KSRam(27, 8, 15, 27, "SRAM62256", "¾²Ì¬´æ´¢Æ÷Ð¾Æ¬62256 32K*8Î»", path, pinPosList, tipsList)
{
	m_nRowNum = 9;
	m_nColumnNum = 6;
	//calculate();
}

KSRam62256::~KSRam62256()
{

}

KSRam62256* KSRam62256::clone()
{
	return new KSRam62256(*this);
}

void KSRam62256::calculate()
{
	if (m_pPinLevelList[18] == LOW && 
		m_pPinLevelList[17] == HIGH)
	{
		if (m_pPinLevelList[16] == HIGH && 
			m_pPinLevelList[15] == LOW)
		{
			read();
		} 
		else if (m_pPinLevelList[16] == LOW)
		{
			write();
		}
	}
}

//---------------------------- SRAMÐ¾Æ¬62128 --------------------------

KSRam62128::KSRam62128(const QPainterPath& path /*= QPainterPath() */,
	const QList<QPoint>& pinPosList /*= QList<QPoint>() */,
	const QList<ITips>& tipsList /* = QList<ITips>()*/)
	: KSRam(26, 8, 14, 26, "SRAM62128", "¾²Ì¬´æ´¢Æ÷Ð¾Æ¬62128 16K*8Î»", path, pinPosList, tipsList)
{
	m_nRowNum = 9;
	m_nColumnNum = 5;
	//calculate();
}

KSRam62128::~KSRam62128()
{

}

KSRam62128* KSRam62128::clone()
{
	return new KSRam62128(*this);
}

void KSRam62128::calculate()
{
	if (m_pPinLevelList[17] == LOW && 
		m_pPinLevelList[16] == HIGH)
	{
		if (m_pPinLevelList[15] == HIGH && 
			m_pPinLevelList[14] == LOW)
		{
			read();
		} 
		else if (m_pPinLevelList[15] == LOW)
		{
			write();
		}
	}
}

//---------------------------- SRAMÐ¾Æ¬628128 --------------------------

KSRam628128::KSRam628128(const QPainterPath& path /*= QPainterPath() */,
	const QList<QPoint>& pinPosList /*= QList<QPoint>() */,
	const QList<ITips>& tipsList /* = QList<ITips>()*/)
	: KSRam(29, 8, 17, 29, "SRAM628128", "HM628128A Series 128K*8Î»", path, pinPosList, tipsList)
{
	m_nRowNum = 9;
	m_nColumnNum = 8;
	//calculate();
}

KSRam628128::~KSRam628128()
{

}

KSRam628128* KSRam628128::clone()
{
	return new KSRam628128(*this);
}

void KSRam628128::calculate()
{
	if (m_pPinLevelList[20] == LOW && 
		m_pPinLevelList[19] == HIGH)
	{
		if (m_pPinLevelList[18] == HIGH && 
			m_pPinLevelList[17] == LOW)
		{
			read();
		} 
		else if (m_pPinLevelList[18] == LOW)
		{
			write();
		}
	}
}
