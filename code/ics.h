#ifndef _ICS_H_
#define _ICS_H_
#include "base.h"
#include <QThread>

/*以
1、POWER	2、PC（CLOCK）	3、LED
4、与门		5、或门			6、非门	
*/
//-------------------------开关类型的元件的基类------------------
class KSWITCHBase
{
public:
	KSWITCHBase(){};
	virtual ~KSWITCHBase(){};
	//*index为开关编号
	virtual bool atSwitch(const QPoint& pos, int* index = NULL) const PURE;
	//index 为开关编号
	virtual void click(int index = 0) PURE;
};
//--------------------------KSWITCH-----------------------------
class KSWITCH : public KBase, public KSWITCHBase
{
public:
	KSWITCH(const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>(),
		const QList<ITips>& tipsList = QList<ITips>());
	~KSWITCH();
	KSWITCH* clone();
	void calculate();
	//*index为开关编号
	bool atSwitch(const QPoint& pos, int* index = NULL) const;
	//index 为开关编号
	void click(int index = 0);

	void draw(QPainter& painter) const;
private:
	bool m_bClosed;
	QPainterPath m_switchPath;
};

//----------------------------KCLOCK--------------------------------

class KCLOCK : public KBase, public KSWITCHBase, public QThread
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

//-------------------------------Vcc-------------------------------
class KVcc : public KBase
{
public:
	KVcc(const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>(),
		const QList<ITips>& tipsList = QList<ITips>());
	~KVcc();
	KVcc* clone();
	void calculate();
	bool contains(const QPoint& pos) const;
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


//--------------------------KNode(节点元件,用于连线)----------------------
class KNode : public KBase
{
public:
	KNode(const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>(),
		const QList<ITips>& tipsList = QList<ITips>());
	~KNode();
	KNode* clone();
	void calculate();
	bool contains(const QPoint& pos) const;

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

//-----------------------------三态输出 74LS125---------------------

class K74LS125 : public KBase
{
public:
	K74LS125(const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>(),
		const QList<ITips>& tipsList = QList<ITips>());
	~K74LS125();
	K74LS125* clone();
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