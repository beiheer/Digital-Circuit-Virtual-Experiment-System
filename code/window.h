#ifndef _WINDOW_h_
#define _WINDOW_H_

#include <QMainWindow>

class QLabel;
class QPushButton;
class QWidget;
class KBase;

class KWindow : public QMainWindow
{
	Q_OBJECT
public:
	KWindow(QWidget* parent = 0, Qt::WindowFlags flags = 0);
	~KWindow();

private slots:
	void lBtnChange();
	void rBtnChange();
	void BtnChange3();
	void BtnChange4();

private:
	QWidget* m_pWidget; //ÓÃÓÚ×öcentralWidget
	QLabel* m_pLabel;
	QPushButton* m_pLButton;
	QPushButton* m_pRButton;
	QPushButton* m_p3Button;
	QPushButton* m_p4Button;
	int m_nLBtnVal;
	int m_nRBtnVal;
	int m_n3BtnVal;
	int m_n4BtnVal;

	KBase* m_pGates1;
	KBase* m_pGates2;
	KBase* m_pGates3;
};

#endif