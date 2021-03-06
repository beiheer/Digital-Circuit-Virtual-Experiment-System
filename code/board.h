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

	KBoard* board() const;

private:
	KBoard* m_pBoard;
};

class KBoard : public QLabel
{
	Q_OBJECT
	enum MODEL{NOMODEL, CREATEWIRE};
	enum POSFLAG{NOFLAG, ONSWITCH, ONIC, ONPIN, ONWIRE};
	struct wire
	{
		KBase* pBegin; 
		int beginPinIndex; 
		KBase* pEnd; 
		int endPinIndex;
	};
public:
	KBoard(QWidget* parent = 0);
	~KBoard();
	QSize getSize() const;
	void setSize(const QSize newSize);
	QString fileName() const;
	void setFileName(const QString& sFileName);
	bool isModified();
	void setModified(bool val);
	int zoom() const;
	void setZoom(int newZoom);
	bool isShowGrid() const;
	void setShowGrid(bool val);
	bool isShowLevel() const;
	void setShowLevel(bool val);
	bool isShowPinPos() const;
	void setShowPinPos(bool val);
	
signals:
	void sizeChanged(QSize newSize);
	void zoomChanged(int zoomNum);
	void modifiedChanged(bool modified);

public slots:
	void buildIC();
	bool openFile(const QString& sFileName);
	bool saveFile();

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
	/*添加的节点在wire上，要进行处理*/
	void nodeOnWire(KBase* pIC);
	void addWire(KBase* pBegin, 
		int beginPinIndex, 
		KBase* pEnd, 
		int endPinIndex,
		const QList<QPoint>& pointList);
	void createWire();
	//仿射变换：把坐标（这里是鼠标坐标）映射到 元件坐标系坐标
	QPoint transform(const QPoint& pos);
	//调整坐标为m_step的倍数
	QPoint adjust(const QPoint& pos);
	//返回约束在面板范围内的坐标
	QPoint makeInRange(const QPoint& pos);

	void deleteSelected();
	//删除与pIC相连的wire
	void deleteWire(KBase* pIC);
	// 返回位置标志
	POSFLAG posFlag(const QPoint& pos);
	/*pos在不在IC元件上, 参数pIC为pos位置上的元件*/
	bool atIC(const QPoint& pos, KBase** pIC);
	//pos在不在开关上，pIC为pos位置上开关所属的元件
	bool atSwitch(const QPoint& pos, KBase** pIC, int* switchIndex);
	//pos在不在引脚上，pIC为pos位置上引脚所属的元件, pinIndex为引脚编号
	bool atPin(const QPoint& pos, KBase** pIC, int* pinIndex);
	//pos在不在电线上，pWire为pos位置上的电线
	bool atWire(const QPoint& pos, KWire** pWire, int* part);

	int indexOf(KBase* pIC, KBase::TYPE type) const;
	int count(KBase::TYPE type) const;

	/*移动选中元件，根据给定的偏移值*/
	void offsetSelectedIC(const QPoint& offset);
	/*移动m_pWire选中的电线*/
	void offsetWire(const QPoint& offset);
	/*更新与pIC相连的电线的拐点*/
	void updateWire(KBase* pIC);

	//触发pIC的电源开关
	//void clickSwitch(KBase* pIC);
	void updateSelectedIC(Qt::KeyboardModifiers modifier);
	void updateSelectedWire(Qt::KeyboardModifiers modifier);

	void drawBackground(QPainter& painter);
	void drawLevel(QPainter& painter);
	void drawPinPos(QPainter& painter);
	void drawICList(QPainter& painter);
	void drawWireList(QPainter& painter);
	void drawCreateWire(QPainter& painter);
	void drawSelectedIC(QPainter& painter);
	void drawDash(QPainter& painter);
	void drawBounding(QPainter& painter);
	void drawSelectedWire(QPainter& painter);
	void drawOffsetWire(QPainter& painter);
	void drawPoint(QPainter& painter, const QPoint& pos, qreal width = 7);

	QList<QPoint> A_Start(const QPoint& begin, const QPoint& end);

private:
	bool m_modified;//内容改变标记
	bool m_bShowGrid;
	bool m_bShowLevel;
	bool m_bShowPinPos;

	int m_zoom;		/*缩放因子 100为一倍*/
	QPoint m_offset;	/*鼠标坐标偏移值*/
	QPoint m_startPos;	/*鼠标按下时的坐标*/
	QPoint m_currentPos; /*当前鼠标坐标*/
	QSize m_size;		/*初始大小*/
	int m_step;

	//当前鼠标位置所在的元件（在开关和引脚上都视为在这个元件上），不在时为NULL
	KBase* m_pIC;
	//引脚编号，鼠标位置的引脚编号
	int m_nPinIndex;
	//开关编号，鼠标位置的开关编号
	int m_nSwitchIndex;
	MODEL m_model;
	POSFLAG m_posFlag;
	/*POWERFLAG m_powerFlag;*/

	wire m_wire;//用于新建电线
	KWire* m_pWire;
	int m_nPart;//电线的第几段
	QList<KWire*> m_wireList;

	QList<KBase*> m_ICList;		/*IC元件列表*/
	QList<KBase*> m_selectedICList;	/*被选中元件列表*/
	QList<KWire*> m_selectedWireList;/*被选中的wire*/

	QFile file;
};
#endif