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
class QTabWidget;

class KUi : public QMainWindow
{
	Q_OBJECT
public:
	KUi();
	~KUi();

private:
	void init();
	void addTab(KBoardWin* pBoardWin);
	void createCenterWidget();
	void createActions();
	void createMenus();
	void createToolBars();
	void createStatusBar();
	void createICListDock();

	bool okToContinue();

private slots:
	void showAboutUs();
	void buildIC();
	void insertTextBox();
	void newFile();
	void openFile();
	void saveFile();
	void closeTab(int index);

	void currentTabChanged(int index);
	void currentTabModified(bool modified);

	void setMsgStatus(const QString& msg);
	void setZoomStatus(qreal zoomNum);
	void updateStatusbar();

private:
	QList<QString> fileNameList;

	QDockWidget* m_pICListDock;

	QTabWidget* m_pTabWidget;
	KBoardWin* m_currentBoardWin;

	QLabel* m_pZoomLabel;
	QLabel* m_pMsgLabel;

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