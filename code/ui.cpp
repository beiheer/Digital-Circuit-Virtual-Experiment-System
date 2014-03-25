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
	m_pICListDock = new QDockWidget("Ԫ���б�");
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
	QMessageBox::information(this, "��������", 
		"<font style = \"font-size: 18px;\">ûʲô��̸��!<font>");
}

void KUi::createActions()
{
	m_pOpenAction = new QAction("��", this);
	m_pSaveAction = new QAction("����", this);
	m_pAboutAction = new QAction("��������", this);
	connect(m_pAboutAction, SIGNAL(triggered()), this, SLOT(showAboutUs()));
}

void KUi::createMenus()
{
	m_pFileMenu = menuBar()->addMenu("�ļ�");
	m_pFileMenu->addAction(m_pOpenAction);
	m_pFileMenu->addAction(m_pSaveAction);

	m_pAboutMenu = menuBar()->addMenu("����");
	m_pAboutMenu->addAction(m_pAboutAction);
}