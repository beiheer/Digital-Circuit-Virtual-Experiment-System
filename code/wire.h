#ifndef _WIRE_H_
#define _WIRE_H_

class KBase;

//节点，电线的起点与终点
class KNode
{
public:
	enum FLAG{PIN, WIRE};

	KNode(KBase* pIC, int pinIndex, QPoint pos, FLAG flag = PIN);
	~KNode();

	KBase* IC() const;
	int pinIndex() const;
	FLAG flag() const;
	bool isHidden() const;
	void setHidden(bool val);
	QPoint pos() const;
	void setPos(const QPoint& pos);
	bool contains(const QPoint& pos) const;
	void draw(QPainter& painter);

private:
	KBase* m_pIC;
	int m_nPinIndex;
	FLAG m_flag;
	bool m_bHidden;
	QPoint m_pos;
	int m_radius;
};

class KWire
{
public:
	KWire(KNode* begin, KNode* end, QList<QPoint> pointList);
	~KWire();

	bool contains(const QPoint& pos) const;
	//判断pIC是不是电线链接的元件
	void draw(QPainter& painter);
	void drawPoint(QPainter& painter);
	bool inWire(KBase* pIC);
	bool inWire(KNode* pNode);
	
	void get(KNode** begin, KNode** end ) const;

private:
	bool createLink();
	void swap();
	//判断pos是否在pos1和pos2的连线上（这里的连线只能是水平或垂直的）
	bool between(const QPoint& pos1, const QPoint& pos2, 
		const QPoint& pos) const;
	//判断num是否在num1和num2两个数之间
	bool between(int num1, int num2, int num) const;

private:
	KNode* m_begin;
	KNode* m_end;
	QList<QPoint> m_pointList;//拐点列表（包括起点，终点）
	bool m_bLegal;
};

#endif