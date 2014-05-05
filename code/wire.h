#ifndef _WIRE_H_
#define _WIRE_H_

class KBase;
class KWire
{
public:
	KWire(KBase* begin, int beginPinIndex, KBase* end, int endPinIndex,
		const QList<QPoint>& pointList);
	~KWire();

	bool contains(const QPoint& pos, int* pPart = NULL) const;
	//���ظ�������part���ֵķ���0��ˮƽ��1��ֱ��
	int orientation(int part) const;
	//�ж�pIC�ǲ��ǵ������ӵ�Ԫ��
	bool inWire(KBase* pIC) const;

	void draw(QPainter& painter, int width = 1);
	void drawPoint(QPainter& painter);
	
	void get(KBase** begin, int* beginPinIndex,
		KBase** end, int* endPinIndex) const;

	/*����part���ֵ����Ҷ˵�, �����ڲ���*/
	void setPoint(int part, const QPoint& newP1, const QPoint& newP2);
	void getPoint(int part, QPoint& p1, QPoint& p2) const;
	void getPoint(int part, const QPoint& offset, 
		QPoint& p1, QPoint& p2) const;

	const QList<QPoint>& pointList() const;
	void setPointList(const QList<QPoint>& pointList);
	

private:
	/*ȥ����ͬһֱ���ϵ������㣨��������ĵ㣩*/
	void adjust();
	bool createLink();
	void swap();
	/*�ж�pos�Ƿ���pos1��pos2�������ϣ����������ֻ����ˮƽ��ֱ�ģ�
	margin��ƫ�ƾ������
	*/
	bool between(const QPoint& pos1, const QPoint& pos2, 
		const QPoint& pos, int margin = 3) const;
	//�ж�num�Ƿ���num1��num2������֮��
	bool between(int num1, int num2, int num) const;

private:
	KBase* m_begin;
	KBase* m_end;
	int m_beginPinIndex;
	int m_endPinIndex;

	QList<QPoint> m_pointList;//�յ��б�������㣬�յ㣩

	bool m_bLegal;
};

#endif