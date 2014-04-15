#include <QtGui>
#include <QPixmap>
#include "iclistwin.h"
#include "ui.h"

KUi::KUi()
{
	init();
	createActions();
	createMenus();
	createToolBars();
	createICListDock();

	m_pMdiArea = new QMdiArea;
	setCentralWidget(m_pMdiArea);
	newFile();
}

void KUi::init()
{
	g_PinCursor = QCursor(QPixmap(IMAGESPATH + "PinCursor.png"));
}

KUi::~KUi()
{
}

void KUi::createActions()
{
	m_pNewAction = new QAction("新建", this);
	connect(m_pNewAction, SIGNAL(triggered()), this, SLOT(newFile()));

	m_pOpenAction = new QAction("打开", this);
	connect(m_pOpenAction, SIGNAL(triggered()), this, SLOT(openFile()));

	m_pSaveAction = new QAction("保存", this);
	connect(m_pSaveAction, SIGNAL(triggered()), this, SLOT(saveFile()));

	m_pBuildICAction = new QAction("生成元件", this);
	connect(m_pBuildICAction, SIGNAL(triggered()), this, SLOT(buildIC()));

	m_pInsertTextBoxAction = new QAction("插入文本框", this);
	connect(m_pInsertTextBoxAction, SIGNAL(triggered()), this, SLOT(insertTextBox()));

	m_pAboutAction = new QAction("关于我们", this);
	connect(m_pAboutAction, SIGNAL(triggered()), this, SLOT(showAboutUs()));
}

void KUi::createMenus()
{
	m_pFileMenu = menuBar()->addMenu("文件");
	m_pFileMenu->addAction(m_pNewAction);
	m_pFileMenu->addAction(m_pOpenAction);
	m_pFileMenu->addAction(m_pSaveAction);
	m_pFileMenu->addSeparator();
	

	m_pInsertMenu = menuBar()->addMenu("插入");
	m_pInsertMenu->addAction(m_pInsertTextBoxAction);

	m_pBuildICMenu = menuBar()->addMenu("生成");
	m_pBuildICMenu->addAction(m_pBuildICAction);

	m_pAboutMenu = menuBar()->addMenu("关于");
	m_pAboutMenu->addAction(m_pAboutAction);
}

void KUi::createToolBars()
{
	m_pFileToolBar = addToolBar(("File"));
	m_pFileToolBar->addAction(m_pOpenAction);
	m_pFileToolBar->addAction(m_pSaveAction);
}

QMdiSubWindow* KUi::createBoardWin()
{
	KBoardWin* pBoardWin = new KBoardWin(this);
	QMdiSubWindow* subWindow = m_pMdiArea->addSubWindow(pBoardWin);
	subWindow->showMaximized();
	return subWindow;
}

void KUi::createICListDock()
{
	KICListWin* pICListWin  = new KICListWin(this);
	m_pICListDock = new QDockWidget("元件列表");
	m_pICListDock->setObjectName("ICListDock");
	m_pICListDock->setWidget(pICListWin);	
	addDockWidget(Qt::LeftDockWidgetArea, m_pICListDock);
}

void KUi::showAboutUs()
{
	QMessageBox::information(this, "关于我们", 
		"<font style = \"font-size: 18px;\">没什么好谈的!<font>");
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

void KUi::newFile()
{
	static int i = 1;
	QMdiSubWindow* subWindow = createBoardWin();
	subWindow->setWindowTitle(QString("电路%1").arg(i));
	++i;
}

void KUi::openFile()
{
	QString sFileName = QFileDialog::getOpenFileName(
		this, "打开文件", "", "IC Files (*.ic)");
	if (!fileNameList.contains(sFileName))
	{
		QMdiSubWindow* subWindow = createBoardWin();
		KBoard* pBoard = dynamic_cast<KBoardWin*>(subWindow->widget())->getBoard();
		pBoard->openFile(sFileName);
		subWindow->setWindowTitle(sFileName);
	}
}

void KUi::saveFile()
{
	QMdiSubWindow* subWindow = m_pMdiArea->activeSubWindow();
	if (subWindow)
	{
		KBoard* pBoard = dynamic_cast<KBoardWin*>(subWindow->widget())->getBoard();
		if (pBoard->fileName() == "")
		{
			QString fileName = QFileDialog::getSaveFileName(
				this, "保存文件", "", "IC Files (*.ic)");
			pBoard->setFileName(fileName);
		}
		pBoard->saveFile();
	}
}