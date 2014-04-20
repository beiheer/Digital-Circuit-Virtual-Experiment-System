#ifndef _SRAM_H_
#define _SRAM_H_

#include "base.h"

typedef bool byte;

class QBitArray;

class KSRam : public KBase
{
public:
	KSRam(int nInNum, int nOutNum,
		int nAddrNum, int nPinNum,
		const QString &name,
		const QString &description,
		const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>(),
		const QList<ITips>& tipsList = QList<ITips>());
	virtual ~KSRam();

	virtual void read();
	virtual void write();
	virtual void calculate();
	void setIn(int num, LevelSignal val);

protected:
	/**
	* @brief 地址译码，将二进制的(行/列)地址转换为十进制
	*/
	virtual int addrDecoder();

	QBitArray *m_bMemoryArray;//存储阵列
	LevelSignal* m_outLevel;

	int m_nAddrNum;		//地址位数
	int m_nRowNum;		//行地址的位数
	int m_nColumnNum;	//列地址的位数
	//int m_nCtrlNum;		//控制信号的个数
};


//---------------------------- SRAM芯片2114 --------------------------
// 地址线：10条	行地址：6 (2^6 = 64行)
//				列地址：4 (2^4 = 16列)
// 数据线：4条
// 控制线：/CS、R/W
// 存储容量：1K * 4位
class KSRam2114 : public KSRam
{
public:
	KSRam2114(const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>(),
		const QList<ITips>& tipsList = QList<ITips>());
	~KSRam2114();
	KSRam2114* clone();

};

//---------------------------- SRAM芯片6116 --------------------------
// 地址线：11条	行地址：7 (2^7 = 128行)
//				列地址：4 (2^4 = 16列)
// 数据线：8条
// 控制线：/CE、/WE、/OE
// 存储容量：2K * 8位
class KSRam6116 : public KSRam
{
public:
	KSRam6116(const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>(),
		const QList<ITips>& tipsList = QList<ITips>());
	~KSRam6116();
	KSRam6116* clone();

	void calculate();
};

//---------------------------- SRAM芯片6264 --------------------------
// 地址线：13条	行地址：9 (2^9 = 512行)
//				列地址：4 (2^4 = 16列)
// 数据线：8条
// 控制线：/CS1、CS2、/WE、/OE
// 存储容量：8K * 8位
class KSRam6264 : public KSRam
{
public:
	KSRam6264(const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>(),
		const QList<ITips>& tipsList = QList<ITips>());
	~KSRam6264();
	KSRam6264* clone();

	void calculate();
};

//---------------------------- SRAM芯片62256 --------------------------
// 地址线：15条	行地址：9 (2^9 = 512行)
//				列地址：6 (2^6 = 64列)
// 数据线：8条
// 控制线：/CS1、CS2、/WE、/OE
// 存储容量：32K * 8位
class KSRam62256 : public KSRam
{
public:
	KSRam62256(const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>(),
		const QList<ITips>& tipsList = QList<ITips>());
	~KSRam62256();
	KSRam62256* clone();

	void calculate();
};

//---------------------------- SRAM芯片62128 --------------------------
// 地址线：14条	行地址：9 (2^9 = 512行)
//				列地址：5 (2^5 = 32列)
// 数据线：8条
// 控制线：/CS1、CS2、/WE、/OE
// 存储容量：16K * 8位
class KSRam62128 : public KSRam
{
public:
	KSRam62128(const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>(),
		const QList<ITips>& tipsList = QList<ITips>());
	~KSRam62128();
	KSRam62128* clone();

	void calculate();
};

//---------------------------- SRAM芯片628128 --------------------------
// 地址线：17条	行地址：9 (2^9 = 512行)
//				列地址：8 (2^8 = 256列)
// 数据线：8条
// 控制线：/CS1、CS2、/WE、/OE
// 存储容量：128K * 8位
class KSRam628128 : public KSRam
{
public:
	KSRam628128(const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>(),
		const QList<ITips>& tipsList = QList<ITips>());
	~KSRam628128();
	KSRam628128* clone();

	void calculate();
};

#endif