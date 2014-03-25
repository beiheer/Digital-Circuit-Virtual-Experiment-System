#ifndef _ICLISTWIN_H_
#define _ICLISTWIN_H_

#include <QListWidget>

class KBase;
class KICListWin : public QListWidget
{
public:
	KICListWin(QWidget* parent = 0);
	~KICListWin();

protected:
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent* event);
	//��дdropEvent����ListWidgetItem˳�򲻿ɸı�
	void dropEvent(QDropEvent* event);

private:
	void performDrag();
	//����ListWidgetItem��icon ��text��
	void createItems();
	//��IC����Pixmap��
	QPixmap createPixmap(const KBase* pIC);

private:
	int m_num;
	QPoint startPos;//�϶��ж�
};

#endif