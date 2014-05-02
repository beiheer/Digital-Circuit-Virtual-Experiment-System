#ifndef _WIRE_H_
#define _WIRE_H_

class KBase;

//�ڵ㣬���ߵ�������յ�
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
	//�ж�pIC�ǲ��ǵ������ӵ�Ԫ��
	void draw(QPainter& painter);
	void drawPoint(QPainter& painter);
	bool inWire(KBase* pIC);
	bool inWire(KNode* pNode);
	
	void get(KNode** begin, KNode** end ) const;

private:
	bool createLink();
	void swap();
	//�ж�pos�Ƿ���pos1��pos2�������ϣ����������ֻ����ˮƽ��ֱ�ģ�
	bool between(const QPoint& pos1, const QPoint& pos2, 
		const QPoint& pos) const;
	//�ж�num�Ƿ���num1��num2������֮��
	bool between(int num1, int num2, int num) const;

private:
	KNode* m_begin;
	KNode* m_end;
	QList<QPoint> m_pointList;//�յ��б�������㣬�յ㣩
	bool m_bLegal;
};

#endif