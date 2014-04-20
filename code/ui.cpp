#include <QtGui>
#include <QPixmap>
#include "iclistwin.h"
#include "ui.h"

KUi::KUi()
{
	init();
	createCenterWidget();
	createActions();
	createMenus();
	createToolBars();	
	createStatusBar();
	createICListDock();

	newFile();
}

void KUi::init()
{
	g_PinCursor = QCursor(QPixmap(IMAGESPATH + "PinCursor.png"));

	m_pSizeList[0] = QSize(800, 600);
	m_pSizeList[1] = QSize(1000, 800);
	m_pSizeList[2] = QSize(1300, 1000);
	m_pSizeList[3] = QSize(1600, 1200);
	m_pSizeList[4] = QSize(3200, 2400);
}

KUi::~KUi()
{
}

void KUi::addTab(KBoardWin* pBoardWin)
{
	QString sFileName = pBoardWin->board()->fileName();
	m_pTabWidget->addTab(pBoardWin, QFileInfo(sFileName).fileName());
	m_pTabWidget->setCurrentWidget(pBoardWin);
	m_pTabWidget->setTabToolTip(m_pTabWidget->currentIndex(), sFileName);

	connect(pBoardWin->board(), SIGNAL(zoomChanged(qreal)), 
		this, SLOT(setZoomStatus(qreal)));
	connect(pBoardWin->board(), SIGNAL(modifiedChanged(bool)),
		this, SLOT(currentTabModified(bool)));
}

void KUi::createCenterWidget()
{
	m_pTabWidget = new QTabWidget(this);
	m_pTabWidget->setTabsClosable(true);
	connect(m_pTabWidget, SIGNAL(tabCloseRequested(int)), 
		this, SLOT(closeTab(int)));
	connect(m_pTabWidget, SIGNAL(currentChanged(int)),
		this, SLOT(currentTabChanged(int)));
	setCentralWidget(m_pTabWidget);
}

void KUi::createActions()
{
	m_pNewAction = new QAction("新建", this);
	m_pNewAction->setIcon(QPixmap(IMAGESPATH + "new.png"));
	m_pNewAction->setShortcut(QKeySequence::New);
	connect(m_pNewAction, SIGNAL(triggered()), this, SLOT(newFile()));

	m_pOpenAction = new QAction("打开", this);
	m_pOpenAction->setIcon(QPixmap(IMAGESPATH + "open.png"));
	m_pOpenAction->setShortcut(QKeySequence::Open);
	m_pOpenAction->setToolTip("打开 (Ctrl+O)");
	connect(m_pOpenAction, SIGNAL(triggered()), this, SLOT(openFile()));

	m_pSaveAction = new QAction("保存", this);
	m_pSaveAction->setIcon(QPixmap(IMAGESPATH + "save.png"));
	m_pSaveAction->setShortcut(QKeySequence::Save);
	m_pSaveAction->setToolTip("保存 (Ctrl+S)");
	connect(m_pSaveAction, SIGNAL(triggered()), this, SLOT(saveFile()));

	m_pSaveAsAction = new QAction("另存为", this);
	m_pSaveAsAction->setIcon(QPixmap(IMAGESPATH + "saveAs.png"));
	m_pSaveAsAction->setShortcut(QKeySequence::SaveAs);
	connect(m_pSaveAsAction, SIGNAL(triggered()), this, SLOT(saveFileAs()));

	m_pZoomInAction = new QAction("放大", this);
	m_pZoomInAction->setIcon(QPixmap(IMAGESPATH + "zoomIn.png"));
	connect(m_pZoomInAction, SIGNAL(triggered()), this, SLOT(zoomIn()));

	m_pZoomOutAction = new QAction("缩小", this);
	m_pZoomOutAction->setIcon(QPixmap(IMAGESPATH + "zoomOut.png"));
	connect(m_pZoomOutAction, SIGNAL(triggered()), this, SLOT(zoomOut()));

	m_pResetZoomAction = new QAction("正常大小", this);
	m_pResetZoomAction->setIcon(QPixmap(IMAGESPATH + "resetzoom.png"));
	connect(m_pResetZoomAction, SIGNAL(triggered()), this, SLOT(resetZoom()));

	m_pSizeActionGroup = new QActionGroup(this);

	{
		m_pSizeAction[0] = new QAction("800 * 600", this);
		m_pSizeAction[0]->setCheckable(true);
		m_pSizeAction[0]->setWhatsThis(0);
		m_pSizeActionGroup->addAction(m_pSizeAction[0]);
		connect(m_pSizeAction[0], SIGNAL(triggered()), this, SLOT(setPanelSize()));

		m_pSizeAction[1] = new QAction("1000 * 800", this);
		m_pSizeAction[1]->setCheckable(true);
		m_pSizeAction[1]->setWhatsThis("1");
		m_pSizeActionGroup->addAction(m_pSizeAction[1]);
		connect(m_pSizeAction[1], SIGNAL(triggered()), this, SLOT(setPanelSize()));

		m_pSizeAction[2] = new QAction("1300 * 1000", this);
		m_pSizeAction[2]->setCheckable(true);
		m_pSizeAction[2]->setWhatsThis("2");
		m_pSizeActionGroup->addAction(m_pSizeAction[2]);
		connect(m_pSizeAction[2], SIGNAL(triggered()), this, SLOT(setPanelSize()));

		m_pSizeAction[3] = new QAction("1600 * 1200", this);
		m_pSizeAction[3]->setCheckable(true);
		m_pSizeAction[3]->setWhatsThis("3");
		m_pSizeActionGroup->addAction(m_pSizeAction[3]);
		connect(m_pSizeAction[3], SIGNAL(triggered()), this, SLOT(setPanelSize()));

		m_pSizeAction[4] = new QAction("3200 * 2400", this);
		m_pSizeAction[4]->setCheckable(true);
		m_pSizeAction[4]->setWhatsThis("4");
		m_pSizeActionGroup->addAction(m_pSizeAction[4]);
		connect(m_pSizeAction[4], SIGNAL(triggered()), this, SLOT(setPanelSize()));
	}
	
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
	m_pFileMenu->addAction(m_pSaveAsAction);

	m_pSizeMenu = new QMenu("设置面板大小");
	for (int i = 0; i < 5; i++)
	{
		m_pSizeMenu->addAction(m_pSizeAction[i]);
	}

	m_pViewMenu = menuBar()->addMenu("视图");
	m_pViewMenu->addAction(m_pZoomInAction);
	m_pViewMenu->addAction(m_pZoomOutAction);
	m_pViewMenu->addAction(m_pResetZoomAction);
	m_pViewMenu->addSeparator();
	m_pViewMenu->addMenu(m_pSizeMenu);

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
	m_pFileToolBar->addAction(m_pNewAction);
	m_pFileToolBar->addAction(m_pOpenAction);
	m_pFileToolBar->addAction(m_pSaveAction);
	m_pFileToolBar->addAction(m_pSaveAsAction);

	m_pViewToolBar = addToolBar(("View"));
	m_pViewToolBar->addAction(m_pZoomInAction);
	m_pViewToolBar->addAction(m_pZoomOutAction);
	m_pViewToolBar->addAction(m_pResetZoomAction);
}

void KUi::createStatusBar()
{
	m_pMsgLabel = new QLabel;
	m_pMsgLabel->setMinimumSize(m_pMsgLabel->sizeHint()); 
	m_pMsgLabel->setAlignment(Qt::AlignHCenter); 

	m_pZoomLabel = new QLabel;
	m_pZoomLabel->setMinimumSize(m_pZoomLabel->sizeHint()); 
	m_pZoomLabel->setAlignment(Qt::AlignHCenter);

	statusBar()->addWidget(m_pMsgLabel);
	statusBar()->addPermanentWidget(m_pZoomLabel);
	statusBar()->setStyleSheet(QString("QStatusBar::item{border: 0px}"));
}

void KUi::createICListDock()
{
	KICListWin* pICListWin  = new KICListWin(this);
	m_pICListDock = new QDockWidget("元件列表");
	m_pICListDock->setObjectName("ICListDock");
	m_pICListDock->setWidget(pICListWin);	
	addDockWidget(Qt::LeftDockWidgetArea, m_pICListDock);
}

bool KUi::okToContinue()
{
	if (m_currentTab && m_currentTab->board()->isModified())
	{
		int r = QMessageBox::warning(this, "警告",
			"是否保存对 \"" + 
			QFileInfo(m_currentTab->board()->fileName()).fileName() + 
			"\" 的修改",
			QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		if (r == QMessageBox::Yes)
		{
			saveFile();
			return true;
		}
		else if (r == QMessageBox::Cancel)
		{
			return false;
		}
	}
	return true;
}

void KUi::showAboutUs()
{
	QMessageBox::information(this, "关于我们", 
		"<font style = \"font-size: 18px;\">没什么好谈的!<font>");
}

void KUi::buildIC()
{
	if (m_currentTab)
		m_currentTab->board()->buildIC();
}

void KUi::insertTextBox()
{

}

void KUi::newFile()
{
	static int i = 1;
	KBoardWin* pBoardWin = new KBoardWin;
	
	pBoardWin->board()->setFileName(QString("未标题-%1").arg(i));
	addTab(pBoardWin);
	++i;
	setMsgStatus("就绪");
}

void KUi::openFile()
{
	setMsgStatus("正在打开文件...");
	QString sFileName = QFileDialog::getOpenFileName(
		this, "打开文件", "", "IC Files (*.ic)");
	if (!sFileName.isEmpty() && !fileNameList.contains(sFileName))
	{	
		KBoardWin* pBoardWin = new KBoardWin;
		if (pBoardWin->board()->openFile(sFileName))
			addTab(pBoardWin);
		else
		{
			delete pBoardWin;
			QMessageBox::warning(this, "警告", "文件打开失败:" + sFileName);
		}
	}
	setMsgStatus("就绪");
}

void KUi::saveFile()
{
	if (m_currentTab)
	{
		setMsgStatus("正在保存文件...");
		if (QFileInfo(m_currentTab->board()->fileName()).isRelative())
		{
			QString sFileName = QFileDialog::getSaveFileName(
				this, "保存文件", "", "IC Files (*.ic)");
			if (!sFileName.isEmpty())
			{	
				m_currentTab->board()->setFileName(sFileName);
				m_pTabWidget->setTabText(m_pTabWidget->currentIndex(), 
					QFileInfo(sFileName).fileName());
				m_pTabWidget->setTabToolTip(m_pTabWidget->currentIndex(), sFileName);
				m_currentTab->board()->saveFile();
			}
		}
		else
		{
			m_currentTab->board()->saveFile();	
		}	
		setMsgStatus("就绪");
	}
}

void KUi::saveFileAs()
{
	if (m_currentTab)
	{
		setMsgStatus("正在保存文件...");

		QString sFileName = QFileDialog::getSaveFileName(
			this, "保存文件", "", "IC Files (*.ic)");
		if (!sFileName.isEmpty())
		{	
			m_currentTab->board()->setFileName(sFileName);
			m_pTabWidget->setTabText(m_pTabWidget->currentIndex(), 
				QFileInfo(sFileName).fileName());
			m_pTabWidget->setTabToolTip(m_pTabWidget->currentIndex(), sFileName);
			m_currentTab->board()->saveFile();
		}

		setMsgStatus("就绪");
	}
}

void KUi::zoomIn()
{
	m_currentTab->board()->setZoom(
		m_currentTab->board()->zoom()+0.2);
}

void KUi::zoomOut()
{
	m_currentTab->board()->setZoom(
		m_currentTab->board()->zoom()-0.2);
}

void KUi::resetZoom()
{
	m_currentTab->board()->setZoom(1.0);
}

void KUi::setPanelSize()
{
	QAction *sizeAcion = dynamic_cast<QAction*>(sender());
	int i = QString(sizeAcion->whatsThis()).toInt();
	
}

void KUi::closeTab(int index)
{
	if (index != -1)
	{
		m_pTabWidget->setCurrentIndex(index);
		if (okToContinue())
		{
			m_currentTab->close();
			m_pTabWidget->removeTab(index);
		}
	}
}

void KUi::currentTabChanged(int index)
{
	if (index != -1)
	{
		m_currentTab = dynamic_cast<KBoardWin*>(m_pTabWidget->currentWidget());
		updateStatusbar();
	}
	else
	{
		m_currentTab = NULL;
	}
}

void KUi::currentTabModified(bool modified)
{
	int index = m_pTabWidget->currentIndex();
	if (modified)
		m_pTabWidget->setTabText(index, m_pTabWidget->tabText(index) + " *");
	else
		m_pTabWidget->setTabText(index, 
			m_pTabWidget->tabText(index).replace(" *", ""));
}

void KUi::setMsgStatus(const QString& msg)
{
	m_pMsgLabel->setText(msg);
}

void KUi::setZoomStatus(const qreal zoomNum)
{
	m_pZoomLabel->setText(QString("%1%").arg(zoomNum * 100));
}

void KUi::updateStatusbar()
{
	setZoomStatus(m_currentTab->board()->zoom());
}