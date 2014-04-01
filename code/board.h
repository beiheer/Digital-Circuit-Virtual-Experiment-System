#ifndef _BOARD_H_
#define _BOARD_H_

#include <QScrollArea>
#include <QLabel>
#include <QFile>
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
	enum MODEL{NOMODEL, CREATEWIRE};
	enum POSFLAG{NOFLAG, ONSWITCH, ONIC, ONPIN, ONWIRE};
	struct wire{KBase* pIC1; int index1; KBase* pIC2; int index2;};
public:
	KBoard(QWidget* parent = 0);
	~KBoard();
	QSize getSize() const;
	void setFileName(const QString& sFileName);
	QString fileName() const;

public slots:
	void buildIC();
	void openFile(const QString& sFileName);
	void saveFile();

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
	KBase* switchAt(const QPoint& pos);
	//����posλ�õ����ŵ�����Ԫ�������ڷ���NULL
	KBase* pinAt(const QPoint& pos);
	//����posλ�õĵ��ߣ����ڷ���NULL
	KWire* wireAt(const QPoint& pos);

	int indexOf(KBase* pIC, KBase::TYPE type) const;
	int count(KBase::TYPE type) const;

	/*�ƶ�ѡ��Ԫ�������ݸ�����ƫ��ֵ*/
	void offsetSelectedIC();

	//����pIC�ĵ�Դ����
	void clickSwitch(KBase* pIC);
	void updateSelectedIC(Qt::KeyboardModifiers modifier);
	void updateSelectedWire(Qt::KeyboardModifiers modifier);

	void drawICList(QPainter& painter);
	void drawWireList(QPainter& painter);
	void drawSelectedIC(QPainter& painter);
	void drawDash(QPainter& painter);
	void drawBounding(QPainter& painter);
	void drawSelectedWire(QPainter& painter);

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
	/*POWERFLAG m_powerFlag;*/

	wire m_wire;//�����½�����
	KWire* m_pWire;
	QList<KWire*> m_WIREList;

	QList<KBase*> m_ICList;		/*ICԪ���б�*/
	QList<KBase*> m_selectedICList;	/*��ѡ��Ԫ���б�*/
	QList<KWire*> m_selectedWireList;/*��ѡ�е�wire*/

	QFile file;
};
#endif