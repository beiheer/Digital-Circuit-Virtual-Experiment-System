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
	//判断pIC是不是电线链接的元件
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
	//判断pos是否在pos1和pos2的连线上（这里的连线只能是水平或垂直的）
	bool between(const QPoint& pos1, const QPoint& pos2, 
		const QPoint& pos) const;
	//判断num是否在num1和num2两个数之间
	bool between(int num1, int num2, int num) const;

private:
	KBase* m_begin;
	KBase* m_end;
	int m_beginPinIndex;
	int m_endPinIndex;

	QList<QPoint> m_pointList;//拐点列表（包括起点，终点）

	bool m_bLegal;
};

#endif