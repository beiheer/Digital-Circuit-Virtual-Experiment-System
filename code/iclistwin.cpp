#include <QtGui>
#include "ics.h"
#include "globaldef.h"
#include "iclistwin.h"

//------------------------KICListWin-------------------------------
KICListWin::KICListWin(QWidget* parent/* = 0*/)
	: QWidget(parent)
{
	m_pLineEdit = new QLineEdit(this);
	connect(m_pLineEdit, SIGNAL(textChanged(const QString&)), 
		this, SLOT(findIC(const QString&)));

	m_pICList = new KICList(this);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(m_pLineEdit);
	layout->addWidget(m_pICList);
	setLayout(layout);
}

KICListWin::~KICListWin()
{
}

void KICListWin::findIC(const QString& sICName)
{
	QListWidgetItem* item;
	for (int i = 0; i < m_pICList->count(); ++i)
	{
		item = m_pICList->item(i);
		if (item->text().contains(sICName,  Qt::CaseInsensitive))
			item->setHidden(false);
		else
			item->setHidden(true);
	}
}

//------------------------KICList-----------------------------------

KICList::KICList(QWidget* parent /* = 0 */)
	: QListWidget(parent)
	, m_num(0)
{
	createItems();
	setIconSize(QSize(80, 40));
	setAcceptDrops(true);
	setAutoFillBackground(true);
}

KICList::~KICList()
{
}

void KICList::mousePressEvent(QMouseEvent* event)
{
	if (Qt::LeftButton == event->button())
		startPos = event->pos();
	QListWidget::mousePressEvent(event);
}

void KICList::mouseMoveEvent(QMouseEvent* event)
{
	if (Qt::LeftButton & event->buttons())
	{
		int distance = (event->pos() - startPos).manhattanLength();
		if (distance >= QApplication::startDragDistance())
			performDrag();
	}
	QListWidget::mouseMoveEvent(event);
}

void KICList::dragMoveEvent(QDragMoveEvent* event)
{
}

void KICList::dragEnterEvent(QDragEnterEvent* event)
{
	KICList* source = dynamic_cast<KICList*>(event->source());
	if (source)
		event->acceptProposedAction();
}

void KICList::dropEvent(QDropEvent* event)
{
}

void KICList::performDrag()
{
	QListWidgetItem* item = currentItem();
	if (item)
	{
		QMimeData* mimeData = new QMimeData;
		mimeData->setText(item->text());

		QPixmap pix = createPixmap(::g_ICMap[item->text()]);

		QDrag* drag = new QDrag(this);
		drag->setMimeData(mimeData);
		drag->setPixmap(pix);
		drag->setHotSpot(QPoint(pix.width() / 2, pix.height() / 2));
		drag->exec(Qt::MoveAction);
	}
}

void KICList::createItems()
{
	QList<KBase*> ICList = ::g_ICMap.values();
	KBase* pIC;
	for(int i = 0; i < ICList.count(); ++i)
	{
		pIC = ICList[i];
		QPixmap pix = createPixmap(pIC);
		QListWidgetItem* item = new QListWidgetItem(
			QIcon(pix), pIC->name());
		item->setToolTip(pIC->description());
		addItem(item);
	}
}

QPixmap KICList::createPixmap(const KBase* pIC)
{
	QPixmap pix(pIC->getWidth() + 1, pIC->getHeight() + 1); 
	pix.fill(QColor(255, 255, 255, 0));//透明背景
	//将将painter移动到pix的中心
	QPainter painter(&pix);
	painter.translate(pIC->getWidth() / 2,  pIC->getHeight() / 2);
	pIC->draw(painter);

	return pix;
}