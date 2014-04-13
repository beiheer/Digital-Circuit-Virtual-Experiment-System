#ifndef _WIRE_H_
#define _WIRE_H_

class KBase;
class KWire
{
public:
	KWire();
	KWire(KBase* pIC1, int nIndex1, KBase* pIC2, int nIndex2);
	~KWire();

	bool contains(const QPoint& pos) const;
	//�ж�pIC�ǲ��ǵ������ӵ�Ԫ��
	void draw(QPainter& painter);
	void drawPoint(QPainter& painter);
	bool inWire(KBase* pIC);
	
	void get(KBase** pIC1, int* index1,
		KBase** pIC2, int* index2) const;

private:
	bool createLink();
	void swap();
	//�ж�pos�Ƿ���pos1��pos2�������ϣ����������ֻ����ˮƽ��ֱ�ģ�
	bool between(const QPoint& pos1, const QPoint& pos2, 
		const QPoint& pos) const;
	//�ж�num�Ƿ���num1��num2������֮��
	bool between(int num1, int num2, int num) const;

private:
	KBase* m_pIC1;
	KBase* m_pIC2;
	int m_nIndex1;
	int m_nIndex2;

	bool m_bLegal;
};

#endif