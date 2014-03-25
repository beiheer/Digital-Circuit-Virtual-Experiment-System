#include <QtGui>

#include "ui.h"

KUi::KUi()
{
	createActions();
	createMenus();
	
	m_pMdiArea = new QMdiArea;
	m_pBoardWin = new KBoardWin(this);
	QMdiSubWindow* subWindow = m_pMdiArea->addSubWindow(m_pBoardWin);
	subWindow->showMaximized();

	m_pICListWin = new KICListWin(this);
	m_pICListDock = new QDockWidget("元件列表");
	m_pICListDock->setObjectName("ICListDock");
	m_pICListDock->setWidget(m_pICListWin);

	addDockWidget(Qt::LeftDockWidgetArea, m_pICListDock);
	setCentralWidget(m_pMdiArea);
}

KUi::~KUi()
{

}

void KUi::showAboutUs()
{
	QMessageBox::information(this, "关于我们", 
		"<font style = \"font-size: 18px;\">没什么好谈的!<font>");
}

void KUi::createActions()
{
	m_pOpenAction = new QAction("打开", this);
	m_pSaveAction = new QAction("保存", this);
	m_pAboutAction = new QAction("关于我们", this);
	connect(m_pAboutAction, SIGNAL(triggered()), this, SLOT(showAboutUs()));
}

void KUi::createMenus()
{
	m_pFileMenu = menuBar()->addMenu("文件");
	m_pFileMenu->addAction(m_pOpenAction);
	m_pFileMenu->addAction(m_pSaveAction);

	m_pAboutMenu = menuBar()->addMenu("关于");
	m_pAboutMenu->addAction(m_pAboutAction);
}