#ifndef _WIRE_H_
#define _WIRE_H_

class KBase;
class KWire
{
public:
	KWire(KBase* begin, int beginPinIndex, KBase* end, int endPinIndex,
		const QList<QPoint>& pointList);
	~KWire();

	bool contains(const QPoint& pos) const;
	//�ж�pIC�ǲ��ǵ������ӵ�Ԫ��
	void draw(QPainter& painter);
	void drawPoint(QPainter& painter);
	bool inWire(KBase* pIC);
	
	void get(KBase** begin, int* beginPinIndex,
		KBase** end, int* endPinIndex) const;
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