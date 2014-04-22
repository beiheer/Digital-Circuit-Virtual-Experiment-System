#ifndef _UI_H_
#define _UI_H_

#include <QMainWindow>
#include "board.h"

class QSplitter;
class QFrame;
class QMenu;
class QToolBar;
class QAction;
class QActionGroup;
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
	void saveFileAs();
	void zoomIn();
	void zoomOut();
	void resetZoom();
	void showGrid();
	void showLevel();
	void setPanelSize();
	void closeTab(int index);

	void currentTabChanged(int index);
	void currentTabModified(bool modified);

	void setMsgStatus(const QString& msg);
	void setZoomStatus(int zoomNum);
	void setSizeStatus(QSize size);
	void updateStatusbar();

private:
	QList<QString> fileNameList;

	QDockWidget* m_pICListDock;

	QTabWidget* m_pTabWidget;
	KBoardWin* m_currentTab;

	QLabel* m_pZoomLabel;
	QLabel* m_pMsgLabel;
	QLabel* m_pSizeLabel;

	QMenu* m_pFileMenu;
	QMenu* m_pInsertMenu;
	QMenu* m_pViewMenu;
	QMenu* m_pSizeMenu;
	QMenu* m_pBuildICMenu;
	QMenu* m_pAboutMenu;

	QToolBar* m_pFileToolBar;
	QToolBar* m_pViewToolBar;

	QAction* m_pNewAction;
	QAction* m_pOpenAction;
	QAction* m_pSaveAction;
	QAction* m_pSaveAsAction;

	QAction* m_pZoomInAction;
	QAction* m_pZoomOutAction;
	QAction* m_pResetZoomAction;
	QAction* m_pShowGridAction;
	QAction* m_pShowLevelAction;

	QActionGroup* m_pSizeActionGroup;
	QAction* m_pSizeAction[5];
	QSize m_pSizeList[5];

	QAction* m_pBuildICAction;

	QAction* m_pInsertTextBoxAction;

	QAction* m_pAboutAction;
};

#endif