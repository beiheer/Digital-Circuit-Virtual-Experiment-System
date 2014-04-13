#ifndef _ICS_H_
#define _ICS_H_
#include "base.h"
#include <QThread>

/*��
1��POWER	2��PC��CLOCK��	3��LED
4������		5������			6������	
*/
//-------------------------INIC���͵�Ԫ���Ļ���------------------
class KINICBase
{
public:
	KINICBase(){};
	virtual ~KINICBase(){};
	//*indexΪ���ر��
	virtual bool atSwitch(const QPoint& pos, int* index = NULL) const PURE;
	//index Ϊ���ر��
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
	//*indexΪ���ر��
	bool atSwitch(const QPoint& pos, int* index = NULL) const;
	//index Ϊ���ر��
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
	//*indexΪ���ر��
	bool atSwitch(const QPoint& pos, int* index = NULL) const;
	//index Ϊ���ر��
	void click(int index = 0);
	void draw(QPainter& painter) const;
	void transmitPulseOnce();//���͵�������
protected:
	void run();

private:
	bool m_start;//�������巢�ͱ�־��Ϊtrueʱ��������
	int m_time;//��������ķ��ͼ��, ��λ����
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

//----------------------------- ����----------------------------
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


//-----------------------------����-----------------------------
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

//----------------------------- ����----------------------------
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

//-----------------------------��̬��� 74LS125---------------------

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
	QList<IOutToIn> m_outToInList;		//�������ӣ��������õ�
};
#endif