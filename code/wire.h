#ifndef _WIRE_H_
#define _WIRE_H_

class KBase;
class KWire
{
public:
	KWire();
	KWire(KBase* pIC1, int nIndex1, KBase* pIC2, int nIndex2);
	~KWire();

	void setIC1(KBase* m_pIC, int index);
	void setIC2(KBase* m_pIC, int index);
	//判断pIC是不是电线链接的元件
	bool inWire(KBase* pIC);
	void draw(QPainter& painter);
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