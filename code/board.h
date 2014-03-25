#ifndef _BOARD_H_
#define _BOARD_H_

#include <QScrollArea>
#include <QLabel>
#include "globaldef.h"
#include "ics.h"

class KBoard;

class KBoardWin : public QScrollArea
{
public:
	KBoardWin(QWidget* parent = 0);
	~KBoardWin();
	QSize sizeHint() const;

protected:
	

private:
	KBoard* m_pBoard;
};

class KBoard : public QLabel
{
public:
	KBoard(QWidget* parent = 0);
	~KBoard();
	QSize getSize() const;

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
	//����任�������꣨������������꣩ӳ�䵽 Ԫ������ϵ����
	QPoint transform(const QPoint& pos);

	void deleteSelected();
	/*����posλ�õ�Ԫ��, pos����Ԫ���Ϸ���NULL*/
	KBase* ICAt(const QPoint& pos);
	//����posλ�õĵ�Դ���ص�����Ԫ�������ڷ���NULL
	KBase* powerSwitchAt(const QPoint& pos);
	/*�ƶ�ѡ��Ԫ�������ݸ�����ƫ��ֵ*/
	void offsetSelected();

	void updatePower(const QPoint& pos);//���µ�Դ����,���pos��ĳ����Դ������
	void updateSelectedList(Qt::KeyboardModifiers modifier, const QPoint& pos);

	//�򿪵�Դ������Դ�źŶ����ͳ�ȥ
	void turnOn();
	void turnOff();//�رյ�Դ
	void drawDash(QPainter& painter);
	void drawBounding(QPainter& painter);
	void drawSolid(QPainter& painter);

private:
	qreal m_factor;		/*��������*/
	QPoint m_offset;	/*�������ƫ��ֵ*/
	QPoint m_startPos;	/*��갴��ʱ������*/
	QPoint m_currentPos; /*��ǰ�������*/
	QSize m_size;		/*��ʼ��С*/
	int m_step;
	
	QList<KBase*> m_ICList;		/*Ԫ���б�*/
	QList<KBase*> m_powerList;	/*��Դ�б�*/
	QList<KBase*> m_LEDList;	/*LED���б�*/
	QList<KBase*> m_selectedList;	/*��ѡ��Ԫ���б�*/
};
#endif