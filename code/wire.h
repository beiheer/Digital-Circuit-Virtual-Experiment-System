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
	//�ƶ�����part���ֵĵ��ߣ�ƫ��ֵpos
	void offsetPart(const QPoint& offset, int part);
	//�õ�����part���ֵ����Ҷ˵�
	

	void draw(QPainter& painter);
	void drawPoint(QPainter& painter);
	
	void get(KBase** begin, int* beginPinIndex,
		KBase** end, int* endPinIndex) const;

	void getPoint(int part, QPoint& p1, QPoint& p2) const;

	void getPoint(int part, const QPoint& offset, 
		QPoint& p1, QPoint& p2) const;

	const QList<QPoint>& pointList() const;
	void setPointList(const QList<QPoint>& pointList);
	

private:
	bool createLink();
	void swap();
	//�ж�pos�Ƿ���pos1��pos2�������ϣ����������ֻ����ˮƽ��ֱ�ģ�
	bool between(const QPoint& pos1, const QPoint& pos2, 
		const QPoint& pos) const;
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