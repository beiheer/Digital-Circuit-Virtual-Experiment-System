#ifndef _ICLISTWIN_H_
#define _ICLISTWIN_H_

#include <QListWidget>

class KBase;
class QLineEdit;
class KICList;

class KICListWin : public QWidget
{
	Q_OBJECT

public:
	KICListWin(QWidget* parent = 0);
	~KICListWin();

private slots:
	void findIC(const QString& sICName);

private:
	QLineEdit* m_pLineEdit;
	KICList* m_pICList;
};

class KICList : public QListWidget
{
public:
	KICList(QWidget* parent = 0);
	~KICList();

protected:
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent* event);
	//重写dropEvent，让ListWidgetItem顺序不可改变
	void dropEvent(QDropEvent* event);

private:
	void performDrag();
	//创建ListWidgetItem（icon 、text）
	void createItems();
	//将IC画到Pixmap上
	QPixmap createPixmap(const KBase* pIC);

private:
	int m_num;
	QPoint startPos;//拖动判断
};

#endif