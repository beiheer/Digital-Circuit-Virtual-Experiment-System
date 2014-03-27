#ifndef _BOARD_H_
#define _BOARD_H_

#include <QScrollArea>
#include <QLabel>
#include "globaldef.h"
#include "ics.h"

class KBoard;
class KWire;

class KBoardWin : public QScrollArea
{
public:
	KBoardWin(QWidget* parent = 0);
	~KBoardWin();
	QSize sizeHint() const;

	KBoard* getBoard() const;

protected:
	

private:
	KBoard* m_pBoard;
};

class KBoard : public QLabel
{
	Q_OBJECT
	enum POWERFLAG{ON, OFF};
	enum MODEL{NOMODEL, CREATEWIRE};
	enum POSFLAG{NOFLAG, ONPOWERSWITCH, ONIC, ONPIN};
	struct wire{KBase* pIC1; int index1; KBase* pIC2; int index2;};
public:
	KBoard(QWidget* parent = 0);
	~KBoard();
	QSize getSize() const;

public slots:
	void turnOn();//�򿪵�Դ������Դ�źŶ����ͳ�ȥ
	void turnOff();//�رյ�Դ

protected:
	void dragEnterEvent(QDragEnterEvent* event);
	void dropEvent(QDropEvent* event);
	void keyPressEvent(QKeyEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void paintEvent(QPaintEvent* event);
	//void resizeEvent(QResizeEvent* event);
	void wheelEvent(QWheelEvent* event);

private:
	//����µ�Ԫ��,��������centerPos
	void addIC(const QString& name, const QPoint& pos);
	void addWire(KBase* pIC1, int index1, KBase* pIC2, int index2);
	void createWire();
	//����任�������꣨������������꣩ӳ�䵽 Ԫ������ϵ����
	QPoint transform(const QPoint& pos);

	void deleteSelected();
	//ɾ����pIC������wire
	void deleteWire(KBase* pIC);
	// ����λ�ñ�־
	POSFLAG posFlag(const QPoint& pos);
	/*����posλ�õ�Ԫ��, pos����Ԫ���Ϸ���NULL*/
	KBase* ICAt(const QPoint& pos);
	//����posλ�õĵ�Դ���ص�����Ԫ�������ڷ���NULL
	KBase* powerSwitchAt(const QPoint& pos);
	//����posλ�õ����ŵ�����Ԫ�������ڷ���NULL
	KBase* pinAt(const QPoint& pos);
	/*�ƶ�ѡ��Ԫ�������ݸ�����ƫ��ֵ*/
	void offsetSelected();

	//����pIC�ĵ�Դ����
	void updatePower(KBase* pIC);
	void updateSelectedList(Qt::KeyboardModifiers modifier);

	void drawDash(QPainter& painter);
	void drawBounding(QPainter& painter);
	void drawSolid(QPainter& painter);
	void drawWires(QPainter& painter);

	void alert();
private:
	qreal m_factor;		/*��������*/
	QPoint m_offset;	/*�������ƫ��ֵ*/
	QPoint m_startPos;	/*��갴��ʱ������*/
	QPoint m_currentPos; /*��ǰ�������*/
	QSize m_size;		/*��ʼ��С*/
	int m_step;

	//��ǰ���λ�����ڵ�Ԫ�����ڿ��غ������϶���Ϊ�����Ԫ���ϣ�������ʱΪNULL
	KBase* m_pIC;
	//���ű�ţ����λ�õ����ű��
	int m_nPinIndex;
	MODEL m_model;
	POSFLAG m_posFlag;
	POWERFLAG m_powerFlag;

	wire m_wire;//�����½�����
	QList<KWire*> m_wires;

	QList<KBase*> m_ICList;		/*Ԫ���б�*/
	QList<KBase*> m_powerList;	/*��Դ�б�*/
	QList<KBase*> m_LEDList;	/*LED���б�*/
	QList<KBase*> m_selectedList;	/*��ѡ��Ԫ���б�*/
};
#endif