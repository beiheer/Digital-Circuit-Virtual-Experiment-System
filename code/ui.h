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
class QMdiSubWindow;

class KUi : public QMainWindow
{
	Q_OBJECT
public:
	KUi();
	~KUi();

private:
	void init();
	void createActions();
	void createMenus();
	void createToolBars();

	QMdiSubWindow* createBoardWin();
	void createICListDock();

private slots:
	void showAboutUs();
	void buildIC();
	void insertTextBox();
	void newFile();
	void openFile();
	void saveFile();

private:
	QList<QString> fileNameList;

	QDockWidget* m_pICListDock;

	QMdiArea* m_pMdiArea;

	QMenu* m_pFileMenu;
	QMenu* m_pInsertMenu;
	QMenu* m_pBuildICMenu;
	QMenu* m_pAboutMenu;

	QToolBar* m_pFileToolBar;

	QAction* m_pNewAction;
	QAction* m_pOpenAction;
	QAction* m_pSaveAction;
	QAction* m_pBuildICAction;

	QAction* m_pInsertTextBoxAction;

	QAction* m_pAboutAction;
};

#endif