#ifndef _UI_H_
#define _UI_H_

#include <QMainWindow>
#include "iclistwin.h"
#include "board.h"

class QSplitter;
class QFrame;
class QMenu;
class QAction;
class QDockWidget;
class QMdiArea;

class KUi : public QMainWindow
{
	Q_OBJECT
public:
	KUi();
	~KUi();

private slots:
	void showAboutUs();

private:
	void createActions();
	void createMenus();
	
private:
	QDockWidget* m_pICListDock;
	KICListWin* m_pICListWin;

	QMdiArea* m_pMdiArea;
	KBoardWin* m_pBoardWin;

	QMenu* m_pFileMenu;
	QMenu* m_pAboutMenu;

	QAction* m_pOpenAction;
	QAction* m_pSaveAction;
	QAction* m_pAboutAction;
};

#endif