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
	struct wire{KBase* pIC1; int index1; KBase* pIC2; int index2;};
public:
	KBoard(QWidget* parent = 0);
	~KBoard();
	QSize getSize() const;
	QString fileName() const;
	void setFileName(const QString& sFileName);
	bool isModified();
	void setModified(bool val);
	qreal zoom();
	void setZoom(qreal newZoom);
	
signals:
	void zoomChanged(qreal zoomNum);
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
	void addWire(KBase* pIC1, int index1, KBase* pIC2, int index2);
	void createWire();
	//����任�������꣨������������꣩ӳ�䵽 Ԫ������ϵ����
	QPoint transform(const QPoint& pos);
	//��������Ϊm_step�ı���
	QPoint adjust(const QPoint& pos);

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
	bool atWire(const QPoint& pos, KWire** pWire);

	int indexOf(KBase* pIC, KBase::TYPE type) const;
	int count(KBase::TYPE type) const;

	/*�ƶ�ѡ��Ԫ�������ݸ�����ƫ��ֵ*/
	void offsetSelectedIC();

	//����pIC�ĵ�Դ����
	//void clickSwitch(KBase* pIC);
	void updateSelectedIC(Qt::KeyboardModifiers modifier);
	void updateSelectedWire(Qt::KeyboardModifiers modifier);

	void drawBackground(QPainter& painter);
	void drawICList(QPainter& painter);
	void drawWireList(QPainter& painter);
	void drawSelectedIC(QPainter& painter);
	void drawDash(QPainter& painter);
	void drawBounding(QPainter& painter);
	void drawSelectedWire(QPainter& painter);
	void drawPoint(QPainter& painter, const QPoint& pos, qreal width = 7);

private:
	bool m_modified;//���ݸı���

	qreal m_zoom;		/*��������*/
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
	QList<KWire*> m_WIREList;

	QList<KBase*> m_ICList;		/*ICԪ���б�*/
	QList<KBase*> m_selectedICList;	/*��ѡ��Ԫ���б�*/
	QList<KWire*> m_selectedWireList;/*��ѡ�е�wire*/

	QFile file;
};
#endif