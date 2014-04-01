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
	m_pICListDock = new QDockWidget("Ԫ���б�");
	m_pICListDock->setObjectName("ICListDock");
	m_pICListDock->setWidget(pICListWin);	
	addDockWidget(Qt::LeftDockWidgetArea, m_pICListDock);
}

void KUi::createActions()
{
	m_pOpenAction = new QAction("��", this);
	m_pSaveAction = new QAction("����", this);
	m_pBuildICAction = new QAction("����Ԫ��", this);
	connect(m_pBuildICAction, SIGNAL(triggered()), this, SLOT(buildIC()));

	m_pInsertTextBoxAction = new QAction("�����ı���", this);
	connect(m_pInsertTextBoxAction, SIGNAL(triggered()), this, SLOT(insertTextBox()));

	m_pAboutAction = new QAction("��������", this);
	connect(m_pAboutAction, SIGNAL(triggered()), this, SLOT(showAboutUs()));
}

void KUi::createMenus()
{
	m_pFileMenu = menuBar()->addMenu("�ļ�");
	m_pFileMenu->addAction(m_pOpenAction);
	m_pFileMenu->addAction(m_pSaveAction);
	m_pFileMenu->addSeparator();
	

	m_pInsertMenu = menuBar()->addMenu("����");
	m_pInsertMenu->addAction(m_pInsertTextBoxAction);

	m_pBuildICMenu = menuBar()->addMenu("����");
	m_pBuildICMenu->addAction(m_pBuildICAction);

	m_pAboutMenu = menuBar()->addMenu("����");
	m_pAboutMenu->addAction(m_pAboutAction);
}

void KUi::createToolBars()
{

}

void KUi::showAboutUs()
{
	QMessageBox::information(this, "��������", 
		"<font style = \"font-size: 18px;\">ûʲô��̸��!<font>");
}

void KUi::buildIC()
{
	QMdiSubWindow* subWindow = m_pMdiArea->activeSubWindow();
	if (subWindow)
		dynamic_cast<KBoardWin*>(subWindow->widget())->getBoard()->buildIC();
}

void KUi::insertTextBox()
{

}