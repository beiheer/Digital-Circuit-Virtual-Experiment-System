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
	* @brief ��ַ���룬�������Ƶ�(��/��)��ַת��Ϊʮ����
	*/
	virtual int addrDecoder();

	QBitArray *m_bMemoryArray;//�洢����
	LevelSignal* m_outLevel;

	int m_nAddrNum;		//��ַλ��
	int m_nRowNum;		//�е�ַ��λ��
	int m_nColumnNum;	//�е�ַ��λ��
	//int m_nCtrlNum;		//�����źŵĸ���
};


//---------------------------- SRAMоƬ2114 --------------------------
// ��ַ�ߣ�10��	�е�ַ��6 (2^6 = 64��)
//				�е�ַ��4 (2^4 = 16��)
// �����ߣ�4��
// �����ߣ�/CS��R/W
// �洢������1K * 4λ
class KSRam2114 : public KSRam
{
public:
	KSRam2114(const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>(),
		const QList<ITips>& tipsList = QList<ITips>());
	~KSRam2114();
	KSRam2114* clone();

};

//---------------------------- SRAMоƬ6116 --------------------------
// ��ַ�ߣ�11��	�е�ַ��7 (2^7 = 128��)
//				�е�ַ��4 (2^4 = 16��)
// �����ߣ�8��
// �����ߣ�/CE��/WE��/OE
// �洢������2K * 8λ
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

//---------------------------- SRAMоƬ6264 --------------------------
// ��ַ�ߣ�13��	�е�ַ��9 (2^9 = 512��)
//				�е�ַ��4 (2^4 = 16��)
// �����ߣ�8��
// �����ߣ�/CS1��CS2��/WE��/OE
// �洢������8K * 8λ
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

//---------------------------- SRAMоƬ62256 --------------------------
// ��ַ�ߣ�15��	�е�ַ��9 (2^9 = 512��)
//				�е�ַ��6 (2^6 = 64��)
// �����ߣ�8��
// �����ߣ�/CS1��CS2��/WE��/OE
// �洢������32K * 8λ
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

//---------------------------- SRAMоƬ62128 --------------------------
// ��ַ�ߣ�14��	�е�ַ��9 (2^9 = 512��)
//				�е�ַ��5 (2^5 = 32��)
// �����ߣ�8��
// �����ߣ�/CS1��CS2��/WE��/OE
// �洢������16K * 8λ
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

//---------------------------- SRAMоƬ628128 --------------------------
// ��ַ�ߣ�17��	�е�ַ��9 (2^9 = 512��)
//				�е�ַ��8 (2^8 = 256��)
// �����ߣ�8��
// �����ߣ�/CS1��CS2��/WE��/OE
// �洢������128K * 8λ
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