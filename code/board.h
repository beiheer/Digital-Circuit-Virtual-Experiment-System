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
	//添加新的元件,并设置其centerPos
	void addIC(const QString& name, const QPoint& pos);
	void addWire(KBase* pIC1, int index1, KBase* pIC2, int index2);
	void createWire();
	//仿射变换：把坐标（这里是鼠标坐标）映射到 元件坐标系坐标
	QPoint transform(const QPoint& pos);

	void deleteSelected();
	//删除与pIC相连的wire
	void deleteWire(KBase* pIC);
	// 返回位置标志
	POSFLAG posFlag(const QPoint& pos);
	/*返回pos位置的元件, pos不在元件上返回NULL*/
	KBase* ICAt(const QPoint& pos);
	//返回pos位置的电源开关的所属元件，不在返回NULL
	KBase* switchAt(const QPoint& pos);
	//返回pos位置的引脚的所属元件，不在返回NULL
	KBase* pinAt(const QPoint& pos);
	//返回pos位置的电线，不在返回NULL
	KWire* wireAt(const QPoint& pos);

	int indexOf(KBase* pIC, KBase::TYPE type) const;
	int count(KBase::TYPE type) const;

	/*移动选中元件，根据给定的偏移值*/
	void offsetSelectedIC();

	//触发pIC的电源开关
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
	qreal m_factor;		/*缩放因子*/
	QPoint m_offset;	/*鼠标坐标偏移值*/
	QPoint m_startPos;	/*鼠标按下时的坐标*/
	QPoint m_currentPos; /*当前鼠标坐标*/
	QSize m_size;		/*初始大小*/
	int m_step;

	//当前鼠标位置所在的元件（在开关和引脚上都视为在这个元件上），不在时为NULL
	KBase* m_pIC;
	//引脚编号，鼠标位置的引脚编号
	int m_nPinIndex;
	MODEL m_model;
	POSFLAG m_posFlag;
	/*POWERFLAG m_powerFlag;*/

	wire m_wire;//用于新建电线
	KWire* m_pWire;
	QList<KWire*> m_WIREList;

	QList<KBase*> m_ICList;		/*IC元件列表*/
	QList<KBase*> m_selectedICList;	/*被选中元件列表*/
	QList<KWire*> m_selectedWireList;/*被选中的wire*/

	QFile file;
};
#endif