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
	//判断pIC是不是电线链接的元件
	void draw(QPainter& painter);
	bool inWire(KBase* pIC);
	
	void getBeginAndEnd(KBase** pIC1, int* index1,
		KBase** pIC2, int* index2) const;

private:
	bool createLink();
	void swap();

private:
	KBase* m_pIC1;
	KBase* m_pIC2;
	int m_nIndex1;
	int m_nIndex2;

	bool m_bLegal;
};

#endif