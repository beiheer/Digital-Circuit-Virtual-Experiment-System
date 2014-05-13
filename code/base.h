#ifndef _BASE_H_
#define _BASE_H_

#include <QPainterPath>
#include <QList>
#include <QQueue>
#include <QString>

#include "globaldef.h"

enum LevelSignal{NOSIGNAL = -1, LOW, HIGH, HIZ};

class KBase;
class KBoard;
class QPainter;

//引脚编号都是从0开始
struct ILink
{
	int i;		/*芯片引脚（编号）*/
	KBase* p;	/*(另)芯片(注意：这里是指针)*/
	int j;		/*(另)芯片引脚编号*/

	bool operator==(const ILink& other) const
	{
		if (i == other.i && p == other.p && j == other.j)
			return true;
		return false;
	}
};
//电平信息设置记录项//要执行setIn操作
struct ISetLevel
{
	KBase* p;   /*元件*/
	int i;		/*逻辑元件引脚（编号）*/
	LevelSignal val; /*新的电平*/
};

//-------------------KBase------------------------
/*逻辑元件的基类（逻辑元件：逻辑门，芯片..）*/
class KBase
{
public:
	//元件类型分4种，输入（INIC）, 输出(OUTIC), 带开关(SWITCH), 其他(OTHERIC)
	enum TYPE{INIC, OUTIC, SWITCH, OTHERIC};
	struct ITips{qreal x; qreal y; QString text;};

public:
	KBase(int nInNum, int nOutNum, int nPinNum,
		const QString& name, 
		const QString& description, 
		const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>(),
		const QList<ITips>& tipsList = QList<ITips>());//不应该拷贝m_links

	KBase(const KBase& other);
	virtual ~KBase();

	int inPinNum() const;
	int outPinNum() const;
	int pinNum() const;
	bool isInPin(int index) const;
	bool isOutPin(int index) const;

	const QList<ILink>& links() const;
	const QString& name() const;
	const QString& description() const;
	const TYPE type() const;

	//将电平信号重置为val
	void reset(LevelSignal val = NOSIGNAL);

	/*获取num引脚的电平*/
	LevelSignal get(int num = 0) const;
	/*获取逻辑元件的输出电平，num指定第几个输出(下标从0开始 下同)*/
	LevelSignal getOut(int num = 0) const;
	/*获取逻辑元件的输入电平，num指定第几个输入*/
	LevelSignal getIn(int num = 0) const;
	/*设置逻辑元件num引脚的电平*/
	virtual bool setIn(int num, LevelSignal val);
	/*添加一条"i引脚" 到 "(另)p逻辑元件的j引脚"的链接*/
	/*i得是输出引脚， j得是输入引脚*/
	virtual bool appendLink(int i, KBase* p, int j);
	virtual bool appendLink(const ILink& link);
	virtual int linkAt(const ILink& link);
	virtual void removeLink(const ILink& link);
	virtual void removeLink(int index);
	

	virtual KBase* clone() PURE;//返回一个克隆体,不应包括m_links

	virtual void setBoard(KBoard* pBoard);

	/*begin: Shell操作相关*/
	virtual void offset(const QPoint& offset);
	virtual void setCenterPos(const QPoint& pos);
	const QPoint& getCenterPos() const;
	QRect getBoundingRect() const;
	const QPainterPath& getPath() const;
	const QList<QPoint>& getPinPosList() const;
	//坐标是加上m_centerPos后的真实坐标
	const QPoint getPinPos(int index) const;
	int getWidth() const;
	int getHeight() const;
	virtual bool contains(const QPoint& pos) const;
	//返回pos所在的引脚下标，没有返回-1
	int pinAt(const QPoint& pos) const;
	virtual void draw(QPainter& painter) const;
	/*end: Shell操作相关*/

	/*将电平信息发送出去,num指定第几个引脚*/
	virtual void levelChange(int num);

protected:
	/*计算输出电平*/
	virtual void calculate() PURE;
	
protected:
	int m_nInPinNum;			/*输入引脚的个数*/
	int m_nOutPinNum;			/*输出引脚的个数*/
	int m_nPinNum;				/*引脚数目*/
	QString m_sName;			/*逻辑元件的名字*/
	QString m_sDescription;		/*简介*/
	QList<ILink> m_links;		/*引脚链接列表*/
	LevelSignal* m_pPinLevelList;/*引脚电平列表*/
	int* m_pPinSetTimesList;	/*引脚设置次数记录*/
	int m_pinIndex;				/*正在操作的引脚*/
	TYPE m_type;				/*元件类型*/
	KBoard* m_pBoard;			/*IC所属board*/

	/*begin: shell相关*/
	int m_width;
	int m_height;
	QPoint m_centerPos;
	QPainterPath m_path;
	QList<QPoint> m_pinPosList;
	QList<ITips> m_tipsList;
	/*end: shell相关*/

	static QQueue<ISetLevel> ms_setLevelQueue;
	
private:
	qreal m_pinRadius;//引脚半径
// 	static int ms_i;
// 	int m_i;
};

#endif