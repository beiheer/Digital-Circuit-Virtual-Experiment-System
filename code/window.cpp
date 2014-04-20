#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "createic.h"
#include "window.h"

KWindow::KWindow(QWidget* parent /* = 0 */, Qt::WindowFlags flags /* = 0 */)
	: QMainWindow(parent, flags)
	, m_nLBtnVal(0)
	, m_nRBtnVal(0)
	, m_n3BtnVal(0)
	, m_n4BtnVal(0)
{
/*	m_pGates1 = new KCustomGate("K74LS21");*/
	m_pGates2 = ::g_createIC("74LS21");
	m_pGates1 = m_pGates2->clone();
	m_pGates3 = ::g_createIC("74LS21");
	m_pGates1->setIn(0, LOW);
	m_pGates1->setIn(1, LOW);
	m_pGates1->setIn(2, LOW);
	m_pGates1->setIn(3, LOW);
	m_pLabel = new QLabel(QString::number(m_pGates1->getOut()));


	m_pLButton = new QPushButton("0");
	m_pRButton = new QPushButton("0");
	m_p3Button = new QPushButton("0");
	m_p4Button = new QPushButton("0");
	m_pWidget = new QWidget(this);

	connect(m_pLButton, SIGNAL(clicked()), this, SLOT(lBtnChange()));
	connect(m_pRButton, SIGNAL(clicked()), this, SLOT(rBtnChange()));
	connect(m_p3Button, SIGNAL(clicked()), this, SLOT(BtnChange3()));
	connect(m_p4Button, SIGNAL(clicked()), this, SLOT(BtnChange4()));

	QHBoxLayout* pBtnLayout = new QHBoxLayout;
	pBtnLayout->addWidget(m_pLButton);
	pBtnLayout->addWidget(m_pRButton);
	pBtnLayout->addWidget(m_p3Button);
	pBtnLayout->addWidget(m_p4Button);

	QVBoxLayout* pMainLayout = new QVBoxLayout;
	pMainLayout->addWidget(m_pLabel);
	pMainLayout->addLayout(pBtnLayout);

	m_pWidget->setLayout(pMainLayout);
	setCentralWidget(m_pWidget);
}

KWindow::~KWindow()
{
	if (m_pGates1)
		delete m_pGates1;
	if (m_pGates2)
		delete m_pGates2;
	if (m_pGates3)
		delete m_pGates3;
}

void KWindow::lBtnChange()
{
	m_nLBtnVal = m_nLBtnVal ? 0 : 1;
	m_pLButton->setText(QString::number(m_nLBtnVal));
	m_pGates1->setIn(0, (LevelSignal)m_nLBtnVal);
	//m_pLabel->setText(QString::number(m_pGates1->getOut()));
	m_pLabel->setText(QString::number(m_pGates1->getOut()));
}

void KWindow::rBtnChange()
{
	m_nRBtnVal = m_nRBtnVal ? 0 : 1;
	m_pRButton->setText(QString::number(m_nRBtnVal));
	m_pGates1->setIn(1, (LevelSignal)m_nRBtnVal);
	m_pLabel->setText(QString::number(m_pGates1->getOut()));
}

void KWindow::BtnChange3()
{
	m_n3BtnVal = m_n3BtnVal ? 0 : 1;
	m_p3Button->setText(QString::number(m_n3BtnVal));
	m_pGates1->setIn(2, (LevelSignal)m_n3BtnVal);
	m_pLabel->setText(QString::number(m_pGates1->getOut()));
}

void KWindow::BtnChange4()
{
	m_n4BtnVal = m_n4BtnVal ? 0 : 1;
	m_p4Button->setText(QString::number(m_n4BtnVal));
	m_pGates1->setIn(3, (LevelSignal)m_n4BtnVal);
	m_pLabel->setText(QString::number(m_pGates1->getOut()));
}
