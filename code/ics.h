#ifndef _ICS_H_
#define _ICS_H_
#include "base.h"

/*��ʵ��12���߼���
1������				2��3��������			3��4��������
4������				5������					6�������
7��3���������		8��4���������			9�������
10��3��������		11��4��������			12�������
*/
//--------------------------KPower-----------------------------
class KPower : public KBase
{
public:
	KPower(const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>());
	~KPower();
	KPower* clone();
	void calculate();

	bool onSwitch(const QPoint& pos) const;
	void click();

	void draw(QPainter& painter) const;
private:
	QPainterPath m_switchPath;
};

//-------------------------------KLED------------------------------
class KLED : public KBase
{
public:
	KLED(const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>());
	~KLED();
	KLED* clone();

	void draw(QPainter& painter) const;
};

//-----------------------------����-----------------------------
class K74LS08 : public KBase
{
public:
	K74LS08(const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>());
	virtual ~K74LS08();
	K74LS08* clone();
	void calculate();
};

//--------------------------3��������---------------------------
class K74LS11 : public KBase
{
public:
	K74LS11(const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>());
	~K74LS11();
	K74LS11* clone();
	void calculate();
};

//-------------------------- 4��������----------------------------
class K74LS21 : public KBase
{
public:
	K74LS21(const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>());
	~K74LS21();
	K74LS21* clone();
	void calculate();
};

//----------------------------- ����----------------------------
class K74LS32 : public KBase
{
public:
	K74LS32(const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>());
	~K74LS32();
	K74LS32* clone();
	void calculate();
};

//----------------------------- ����----------------------------
class K74LS04 : public KBase
{
public:
	K74LS04(const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>());
	~K74LS04();
	K74LS04* clone();
	void calculate();
};

//----------------------------- �����----------------------------
class K74LS00 : public KBase
{
public:
	K74LS00(const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>());
	~K74LS00();
	K74LS00* clone();
	void calculate();
};

//-------------------------- 3���������----------------------------
class K74LS10 : public KBase
{
public:
	K74LS10(const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>());
	~K74LS10();
	K74LS10* clone();
	void calculate();
};

//-------------------------- 4���������----------------------------
class K74LS20 : public KBase
{
public:
	K74LS20(const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>());
	~K74LS20();
	K74LS20* clone();
	void calculate();
};

//----------------------------- �����----------------------------
class K74LS02 : public KBase
{
public:
	K74LS02(const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>());
	~K74LS02();
	K74LS02* clone();
	void calculate();
};

//-------------------------3��������---------------------------
class K74LS27 : public KBase
{
public:
	K74LS27(const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>());
	~K74LS27();
	K74LS27* clone();
	void calculate();
};

//-------------------------4��������--------------------------
class KCD4002 : public KBase
{
public:
	KCD4002(const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>());
	~KCD4002();
	KCD4002* clone();
	void calculate();
};

//---------------------------- ����� --------------------------
class K74LS86 : public KBase
{
public:
	K74LS86(const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>());
	~K74LS86();
	K74LS86* clone();
	void calculate();
};


//---------------------------KUniversalIC--------------------------------

class KUniversalIC : public KBase
{
public:
	struct IInToIn{int index; int target; int targetIndex;};
	struct IOutToOut{int source; int sourceIndex; int index;};
	struct IOutToIn{int source; int sourceIndex; int target; int targetIndex;};

public:
	KUniversalIC(
		int nInNum,
		int nOutNum,
		int nPinNum,
		const QString& name, 
		const QString& description,
		const QList<KBase*>& componentList,
		const QList<IInToIn>& inToInList,
		const QList<IOutToOut>& outToOutList,
		const QList<IOutToIn>& outToInList,
		const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>());
	KUniversalIC(const KUniversalIC& other);
	~KUniversalIC();

	void setIn(int num, LevelSignal val);
	KUniversalIC* clone();

protected:
	void calculate();

private:
	void makeLinks();

private:
	QList<KBase*> m_componentList;
	QList<IInToIn> m_inToInList;
	QList<IOutToOut> m_outToOutList;
	QList<IOutToIn> m_outToInList;		//�������ӣ��������õ�
};
#endif