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
	//返回给出电线part部分的方向（0：水平，1垂直）
	int orientation(int part) const;
	//判断pIC是不是电线链接的元件
	bool inWire(KBase* pIC) const;
	//移动电线part部分的电线，偏移值pos
	void offsetPart(const QPoint& offset, int part);
	//得到电线part部分的左右端点
	

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