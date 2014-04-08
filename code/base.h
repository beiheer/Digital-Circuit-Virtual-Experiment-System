#ifndef _BASE_H_
#define _BASE_H_

#include <QPainterPath>
#include <QList>
#include <QString>

#include "globaldef.h"

enum LevelSignal{NOSIGNAL = -1, LOW, HIGH, HIZ};

class KBase;
class KBoard;
class QPainter;

//���ű�Ŷ��Ǵ�0��ʼ
struct ILink
{
	int i;		/*�߼�Ԫ�����ţ���ţ�*/
	KBase* p;	/*(��)�߼�Ԫ��(ע�⣺������ָ��)*/
	int j;		/*(��)�߼�Ԫ�����ű��*/

	bool operator==(const ILink& other) const
	{
		if (i == other.i && p == other.p &&
			j == other.j)
			return true;
		return false;
	}
};

//-------------------KBase------------------------
/*�߼�Ԫ���Ļ��ࣨ�߼�Ԫ�����߼��ţ�оƬ..��*/
class KBase
{
public:
	//Ԫ�����ͷ�3�֣����루INIC��, �����(OUTIC), ����(OTHERIC)
	enum TYPE{INIC, OUTIC, OTHERIC};
	struct ITips{qreal x; qreal y; QString text;};

public:
	KBase(int nInNum, int nOutNum, int nPinNum,
		const QString& name, 
		const QString& description, 
		const QPainterPath& path = QPainterPath(),
		const QList<QPoint>& pinPosList = QList<QPoint>(),
		const QList<ITips>& tipsList = QList<ITips>());//��Ӧ�ÿ���m_links

	KBase(const KBase& other);
	virtual ~KBase();

	int inPinNum() const;
	int outPinNum() const;
	int pinNum() const;

	const QList<ILink>& links() const;
	const QString& name() const;
	const QString& description() const;
	const TYPE type() const;

	//����ƽ�ź�����Ϊval
	void reset(LevelSignal val = NOSIGNAL);

	/*��ȡnum���ŵĵ�ƽ*/
	LevelSignal get(int num = 0) const;
	/*��ȡ�߼�Ԫ���������ƽ��numָ���ڼ������(�±��0��ʼ ��ͬ)*/
	LevelSignal getOut(int num = 0) const;
	/*��ȡ�߼�Ԫ���������ƽ��numָ���ڼ�������*/
	LevelSignal getIn(int num = 0) const;
	/*�����߼�Ԫ��num���ŵĵ�ƽ*/
	virtual void setIn(int num, LevelSignal val);
	/*���һ��"i����" �� "(��)p�߼�Ԫ����j����"������*/
	/*i����������ţ� j������������*/
	virtual bool appendLink(int i, KBase* p, int j);
	virtual bool appendLink(const ILink& link);
	virtual int linkAt(const ILink& link);
	virtual void removeLink(const ILink& link);
	virtual void removeLink(int index);
	

	virtual KBase* clone() PURE;//����һ����¡��,��Ӧ����m_links

	virtual void setBoard(KBoard* pBoard);

	/*begin: Shell�������*/
	virtual void offset(const QPoint& offset);
	virtual void setCenterPos(const QPoint& pos);
	const QPoint& getCenterPos() const;
	QRect getBoundingRect() const;
	const QPainterPath& getPath() const;
	const QList<QPoint>& getPinPosList() const;
	//�����Ǽ���m_centerPos�����ʵ����
	const QPoint getPinPos(int index) const;
	int getWidth() const;
	int getHeight() const;
	bool contains(const QPoint& pos) const;
	//����pos���ڵ������±꣬û�з���-1
	int pinAt(const QPoint& pos) const;
	virtual void draw(QPainter& painter) const;
	/*end: Shell�������*/

	/*����ƽ��Ϣ���ͳ�ȥ,numָ���ڼ�������*/
	virtual void sendChange(int num);

protected:
	/*���������ƽ*/
	virtual void calculate() PURE;
	
protected:
	int m_nInPinNum;			/*�������ŵĸ���*/
	int m_nOutPinNum;			/*������ŵĸ���*/
	int m_nPinNum;				/*������Ŀ*/
	QString m_sName;			/*�߼�Ԫ��������*/
	QString m_sDescription;		/*���*/
	QList<ILink> m_links;		/*���������б�*/
	LevelSignal* m_pPinLevelList;/*���ŵ�ƽ�б�*/
	TYPE m_type;				/*Ԫ������*/
	KBoard* m_pBoard;			/*IC����board*/

	/*begin: shell���*/
	int m_width;
	int m_height;
	QPoint m_centerPos;
	QPainterPath m_path;
	QList<QPoint> m_pinPosList;
	QList<ITips> m_tipsList;
	/*end: shell���*/

private:
	qreal m_pinRadius;//���Ű뾶
// 	static int ms_i;
// 	int m_i;
};

#endif