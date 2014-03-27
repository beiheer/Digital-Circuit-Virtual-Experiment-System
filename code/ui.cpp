#include <QtGui>
#include "iclistwin.h"
#include "ui.h"

KUi::KUi()
{
	createActions();
	createMenus();
	createToolBars();
	createICListDock();

	m_pMdiArea = new QMdiArea;
	setCentralWidget(m_pMdiArea);
	createBoardWin();
}

KUi::~KUi()
{
}

void KUi::createBoardWin()
{
	KBoardWin* pBoardWin = new KBoardWin(this);
	QMdiSubWindow* subWindow = m_pMdiArea->addSubWindow(pBoardWin);
	subWindow->showMaximized();
}

void KUi::createICListDock()
{
	KICListWin* pICListWin  = new KICListWin(this);
	m_pICListDock = new QDockWidget("元件列表");
	m_pICListDock->setObjectName("ICListDock");
	m_pICListDock->setWidget(pICListWin);	
	addDockWidget(Qt::LeftDockWidgetArea, m_pICListDock);
}

void KUi::createActions()
{
	m_pOpenAction = new QAction("打开", this);
	m_pSaveAction = new QAction("保存", this);

	m_pAboutAction = new QAction("关于我们", this);
	connect(m_pAboutAction, SIGNAL(triggered()), this, SLOT(showAboutUs()));

	m_pTurnOnAction = new QAction("打开电源", this);
	m_pTurnOnAction->setIcon(QIcon("./images/turnon.png"));
	connect(m_pTurnOnAction, SIGNAL(triggered()), this, SLOT(turnOn()));

	m_pTurnOffAction = new QAction("关闭电源", this);
	m_pTurnOffAction->setIcon(QIcon("./images/turnoff.png"));
	m_pTurnOffAction->setEnabled(false);
	connect(m_pTurnOffAction, SIGNAL(triggered()), this, SLOT(turnOff()));
}

void KUi::createMenus()
{
	m_pFileMenu = menuBar()->addMenu("文件");
	m_pFileMenu->addAction(m_pOpenAction);
	m_pFileMenu->addAction(m_pSaveAction);

	m_pPowerMenu = menuBar()->addMenu("电源");
	m_pPowerMenu->addAction(m_pTurnOnAction);
	m_pPowerMenu->addAction(m_pTurnOffAction);

	m_pAboutMenu = menuBar()->addMenu("关于");
	m_pAboutMenu->addAction(m_pAboutAction);
}

void KUi::createToolBars()
{
	m_pPowerToolBar = addToolBar(tr("电源工具栏"));
	m_pPowerToolBar->addAction(m_pTurnOnAction);
	m_pPowerToolBar->addAction(m_pTurnOffAction);

}

void KUi::showAboutUs()
{
	QMessageBox::information(this, "关于我们", 
		"<font style = \"font-size: 18px;\">没什么好谈的!<font>");
}

void KUi::turnOn()
{
	m_pTurnOnAction->setEnabled(false);
	m_pTurnOffAction->setEnabled(true);
	QMdiSubWindow* subWindow = m_pMdiArea->activeSubWindow();
	if (subWindow)
		dynamic_cast<KBoardWin*>(subWindow->widget())->getBoard()->turnOn();
}

void KUi::turnOff()
{
	m_pTurnOffAction->setEnabled(false);
	m_pTurnOnAction->setEnabled(true);
	QMdiSubWindow* subWindow = m_pMdiArea->activeSubWindow();
	if (subWindow)
		dynamic_cast<KBoardWin*>(subWindow->widget())->getBoard()->turnOff();
}