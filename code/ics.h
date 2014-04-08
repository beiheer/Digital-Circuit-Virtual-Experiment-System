#ifndef _ICS_H_
#define _ICS_H_
#include "base.h"

/*��ʵ��12���߼���
1������				2��3��������			3��4��������
4������				5������					6�������
7��3���������		8��4���������			9�������
10��3��������		11��4��������			12�������
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
//--------------------------KPower-----------------------------
class KPower : public KBase, public KINICBase
{
public:
	KPower(const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>(),
		const QList<ITips>& tipsList = QList<ITips>());
	~KPower();
	KPower* clone();
	void calculate();
	//*indexΪ���ر��
	bool atSwitch(const QPoint& pos, int* index = NULL) const;
	//index Ϊ���ر��
	void click(int index = 0);

	void draw(QPainter& painter) const;
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