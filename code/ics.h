#ifndef _ICS_H_
#define _ICS_H_
#include "base.h"
#include <QThread>

/*以
1、POWER	2、PC（CLOCK）	3、LED
4、与门		5、或门			6、非门	
*/
//-------------------------INIC类型的元件的基类------------------
class KINICBase
{
public:
	KINICBase(){};
	virtual ~KINICBase(){};
	//*index为开关编号
	virtual bool atSwitch(const QPoint& pos, int* index = NULL) const PURE;
	//index 为开关编号
	virtual void click(int index = 0) PURE;
};
//--------------------------KPOWER-----------------------------
class KPOWER : public KBase, public KINICBase
{
public:
	KPOWER(const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>(),
		const QList<ITips>& tipsList = QList<ITips>());
	~KPOWER();
	KPOWER* clone();
	void calculate();
	//*index为开关编号
	bool atSwitch(const QPoint& pos, int* index = NULL) const;
	//index 为开关编号
	void click(int index = 0);

	void draw(QPainter& painter) const;
};

class KCLOCK : public KBase, public KINICBase, public QThread
{
public:
	KCLOCK(const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>(),
		const QList<ITips>& tipsList = QList<ITips>());
	KCLOCK(const KCLOCK& other);
	~KCLOCK();
	KCLOCK* clone();
	void calculate();
	//*index为开关编号
	bool atSwitch(const QPoint& pos, int* index = NULL) const;
	//index 为开关编号
	void click(int index = 0);
	void draw(QPainter& painter) const;
	void transmitPulseOnce();//发送单步脉冲
protected:
	void run();

private:
	bool m_start;//持续脉冲发送标志，为true时持续发送
	int m_time;//持续脉冲的发送间隔, 单位毫秒
};

//-------------------------------KLED------------------------------
class KLED : public KBase
{
public:
	KLED(const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>(),
		const QList<ITips>& tipsList = QList<ITips>());
	~KLED();
	KLED* clone();
	void calculate();

	void draw(QPainter& painter) const;
};

//----------------------------- 非门----------------------------
class K74LS04 : public KBase
{
public:
	K74LS04(const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>(),
		const QList<ITips>& tipsList = QList<ITips>());
	~K74LS04();
	K74LS04* clone();
	void calculate();
};


//-----------------------------与门-----------------------------
class K74LS08 : public KBase
{
public:
	K74LS08(const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>(),
		const QList<ITips>& tipsList = QList<ITips>());
	virtual ~K74LS08();
	K74LS08* clone();
	void calculate();
};

//----------------------------- 或门----------------------------
class K74LS32 : public KBase
{
public:
	K74LS32(const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>(),
		const QList<ITips>& tipsList = QList<ITips>());
	~K74LS32();
	K74LS32* clone();
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
		const QList<QPoint>& pinPosList = QList<QPoint>(),
		const QList<ITips>& tipsList = QList<ITips>());
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
	QList<IOutToIn> m_outToInList;		//保存连接，拷贝是用到
};
#endif