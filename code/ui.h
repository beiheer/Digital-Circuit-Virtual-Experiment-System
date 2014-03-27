#ifndef _UI_H_
#define _UI_H_

#include <QMainWindow>
#include "board.h"

class QSplitter;
class QFrame;
class QMenu;
class QToolBar;
class QAction;
class QDockWidget;
class QMdiArea;

class KUi : public QMainWindow
{
	Q_OBJECT
public:
	KUi();
	~KUi();

private:
	void createActions();
	void createMenus();
	void createToolBars();

	void createBoardWin();
	void createICListDock();

private slots:
		void showAboutUs();
		void turnOn();
		void turnOff();

private:
	QDockWidget* m_pICListDock;

	QMdiArea* m_pMdiArea;

	QMenu* m_pFileMenu;
	QMenu* m_pPowerMenu;
	QMenu* m_pAboutMenu;

	QToolBar* m_pPowerToolBar;

	QAction* m_pOpenAction;
	QAction* m_pSaveAction;
	QAction* m_pAboutAction;
	QAction* m_pTurnOnAction;
	QAction* m_pTurnOffAction;
};

#endif