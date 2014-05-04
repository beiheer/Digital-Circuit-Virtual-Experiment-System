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

	//����µ�Ԫ��,��������centerPos
	void addIC(const QString& name, const QPoint& pos);
	/*��ӵĽڵ���wire�ϣ�Ҫ���д���*/
	void nodeOnWire(KBase* pIC);
	void addWire(KBase* pBegin, 
		int beginPinIndex, 
		KBase* pEnd, 
		int endPinIndex,
		const QList<QPoint>& pointList);
	void createWire();
	//����任�������꣨������������꣩ӳ�䵽 Ԫ������ϵ����
	QPoint transform(const QPoint& pos);
	//��������Ϊm_step�ı���
	QPoint adjust(const QPoint& pos);
	//����Լ������巶Χ�ڵ�����
	QPoint makeInRange(const QPoint& pos);

	void deleteSelected();
	//ɾ����pIC������wire
	void deleteWire(KBase* pIC);
	// ����λ�ñ�־
	POSFLAG posFlag(const QPoint& pos);
	/*pos�ڲ���ICԪ����, ����pICΪposλ���ϵ�Ԫ��*/
	bool atIC(const QPoint& pos, KBase** pIC);
	//pos�ڲ��ڿ����ϣ�pICΪposλ���Ͽ���������Ԫ��
	bool atSwitch(const QPoint& pos, KBase** pIC, int* switchIndex);
	//pos�ڲ��������ϣ�pICΪposλ��������������Ԫ��, pinIndexΪ���ű��
	bool atPin(const QPoint& pos, KBase** pIC, int* pinIndex);
	//pos�ڲ��ڵ����ϣ�pWireΪposλ���ϵĵ���
	bool atWire(const QPoint& pos, KWire** pWire, int* part);

	int indexOf(KBase* pIC, KBase::TYPE type) const;
	int count(KBase::TYPE type) const;

	/*�ƶ�ѡ��Ԫ�������ݸ�����ƫ��ֵ*/
	void offsetSelectedIC(const QPoint& offset);
	/*�ƶ�m_pWireѡ�еĵ���*/
	void offsetWire(const QPoint& offset);
	/*������pIC�����ĵ��ߵĹյ�*/
	void updateWire(KBase* pIC);

	//����pIC�ĵ�Դ����
	//void clickSwitch(KBase* pIC);
	void updateSelectedIC(Qt::KeyboardModifiers modifier);
	void updateSelectedWire(Qt::KeyboardModifiers modifier);

	void drawBackground(QPainter& painter);
	void drawLevel(QPainter& painter);
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
	bool m_modified;//���ݸı���
	bool m_bShowGrid;
	bool m_bShowLevel;

	int m_zoom;		/*�������� 100Ϊһ��*/
	QPoint m_offset;	/*�������ƫ��ֵ*/
	QPoint m_startPos;	/*��갴��ʱ������*/
	QPoint m_currentPos; /*��ǰ�������*/
	QSize m_size;		/*��ʼ��С*/
	int m_step;

	//��ǰ���λ�����ڵ�Ԫ�����ڿ��غ������϶���Ϊ�����Ԫ���ϣ�������ʱΪNULL
	KBase* m_pIC;
	//���ű�ţ����λ�õ����ű��
	int m_nPinIndex;
	//���ر�ţ����λ�õĿ��ر��
	int m_nSwitchIndex;
	MODEL m_model;
	POSFLAG m_posFlag;
	/*POWERFLAG m_powerFlag;*/

	wire m_wire;//�����½�����
	KWire* m_pWire;
	int m_nPart;//���ߵĵڼ���
	QList<KWire*> m_wireList;

	QList<KBase*> m_ICList;		/*ICԪ���б�*/
	QList<KBase*> m_selectedICList;	/*��ѡ��Ԫ���б�*/
	QList<KWire*> m_selectedWireList;/*��ѡ�е�wire*/

	QFile file;
};
#endif