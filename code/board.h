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
	//添加新的元件,并设置其centerPos
	void addIC(const QString& name, const QPoint& pos);
	//仿射变换：把坐标（这里是鼠标坐标）映射到 元件坐标系坐标
	QPoint transform(const QPoint& pos);

	void deleteSelected();
	/*返回pos位置的元件, pos不在元件上返回NULL*/
	KBase* ICAt(const QPoint& pos);
	//返回pos位置的电源开关的所属元件，不在返回NULL
	KBase* powerSwitchAt(const QPoint& pos);
	/*移动选中元件，根据给定的偏移值*/
	void offsetSelected();

	void updatePower(const QPoint& pos);//更新电源输入,如果pos在某个电源开关上
	void updateSelectedList(Qt::KeyboardModifiers modifier, const QPoint& pos);

	//打开电源，将电源信号都发送出去
	void turnOn();
	void turnOff();//关闭电源
	void drawDash(QPainter& painter);
	void drawBounding(QPainter& painter);
	void drawSolid(QPainter& painter);

private:
	qreal m_factor;		/*缩放因子*/
	QPoint m_offset;	/*鼠标坐标偏移值*/
	QPoint m_startPos;	/*鼠标按下时的坐标*/
	QPoint m_currentPos; /*当前鼠标坐标*/
	QSize m_size;		/*初始大小*/
	int m_step;
	
	QList<KBase*> m_ICList;		/*元件列表*/
	QList<KBase*> m_powerList;	/*电源列表*/
	QList<KBase*> m_LEDList;	/*LED灯列表*/
	QList<KBase*> m_selectedList;	/*被选中元件列表*/
};
#endif