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
}

KUi::~KUi()
{
}

void KUi::addTab(KBoardWin* pBoardWin)
{
	QString sFileName = pBoardWin->getBoard()->fileName();
	m_pTabWidget->addTab(pBoardWin, QFileInfo(sFileName).fileName());
	m_pTabWidget->setCurrentWidget(pBoardWin);
	m_pTabWidget->setTabToolTip(m_pTabWidget->currentIndex(), sFileName);

	connect(pBoardWin->getBoard(), SIGNAL(zoomChanged(qreal)), 
		this, SLOT(setZoomStatus(qreal)));
	connect(pBoardWin->getBoard(), SIGNAL(modifiedChanged(bool)),
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
	m_pNewAction = new QAction("�½�", this);
	connect(m_pNewAction, SIGNAL(triggered()), this, SLOT(newFile()));

	m_pOpenAction = new QAction("��", this);
	connect(m_pOpenAction, SIGNAL(triggered()), this, SLOT(openFile()));

	m_pSaveAction = new QAction("����", this);
	connect(m_pSaveAction, SIGNAL(triggered()), this, SLOT(saveFile()));

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
	m_pFileMenu->addAction(m_pNewAction);
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
	m_pFileToolBar = addToolBar(("File"));
	m_pFileToolBar->addAction(m_pOpenAction);
	m_pFileToolBar->addAction(m_pSaveAction);
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
	m_pICListDock = new QDockWidget("Ԫ���б�");
	m_pICListDock->setObjectName("ICListDock");
	m_pICListDock->setWidget(pICListWin);	
	addDockWidget(Qt::LeftDockWidgetArea, m_pICListDock);
}

bool KUi::okToContinue()
{
	if (m_currentBoardWin && m_currentBoardWin->getBoard()->isModified())
	{
		int r = QMessageBox::warning(this, "����",
			"�Ƿ񱣴�� \"" + 
			QFileInfo(m_currentBoardWin->getBoard()->fileName()).fileName() + 
			"\" ���޸�",
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
	QMessageBox::information(this, "��������", 
		"<font style = \"font-size: 18px;\">ûʲô��̸��!<font>");
}

void KUi::buildIC()
{
	if (m_currentBoardWin)
		m_currentBoardWin->getBoard()->buildIC();
}

void KUi::insertTextBox()
{

}

void KUi::newFile()
{
	static int i = 1;
	KBoardWin* pBoardWin = new KBoardWin;
	
	pBoardWin->getBoard()->setFileName(QString("δ����-%1").arg(i));
	addTab(pBoardWin);
	++i;
	setMsgStatus("����");
}

void KUi::openFile()
{
	setMsgStatus("���ڴ��ļ�...");
	QString sFileName = QFileDialog::getOpenFileName(
		this, "���ļ�", "", "IC Files (*.ic)");
	if (!sFileName.isEmpty() && !fileNameList.contains(sFileName))
	{	
		KBoardWin* pBoardWin = new KBoardWin;
		if (pBoardWin->getBoard()->openFile(sFileName))
			addTab(pBoardWin);
		else
		{
			delete pBoardWin;
			QMessageBox::warning(this, "����", "�ļ���ʧ��:" + sFileName);
		}
	}
	setMsgStatus("����");
}

void KUi::saveFile()
{
	if (m_currentBoardWin)
	{
		setMsgStatus("���ڱ����ļ�...");
		if (QFileInfo(m_currentBoardWin->getBoard()->fileName()).isRelative())
		{
			QString sFileName = QFileDialog::getSaveFileName(
				this, "�����ļ�", "", "IC Files (*.ic)");
			if (!sFileName.isEmpty())
			{	
				m_currentBoardWin->getBoard()->setFileName(sFileName);
				m_pTabWidget->setTabText(m_pTabWidget->currentIndex(), 
					QFileInfo(sFileName).fileName());
				m_pTabWidget->setTabToolTip(m_pTabWidget->currentIndex(), sFileName);
				m_currentBoardWin->getBoard()->saveFile();
			}
		}
		else
		{
			m_currentBoardWin->getBoard()->saveFile();	
		}	
		setMsgStatus("����");
	}
}

void KUi::closeTab(int index)
{
	if (index != -1)
	{
		m_pTabWidget->setCurrentIndex(index);
		if (okToContinue())
		{
			m_currentBoardWin->close();
			m_pTabWidget->removeTab(index);
		}
	}
}

void KUi::currentTabChanged(int index)
{
	if (index != -1)
	{
		m_currentBoardWin = dynamic_cast<KBoardWin*>(m_pTabWidget->currentWidget());
		updateStatusbar();
	}
	else
	{
		m_currentBoardWin = NULL;
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
	setZoomStatus(m_currentBoardWin->getBoard()->zoom());
}