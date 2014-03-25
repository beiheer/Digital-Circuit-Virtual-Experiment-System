#include <QtGui>
#include "ics.h"
#include "globaldef.h"
#include "iclistwin.h"

KICListWin::KICListWin(QWidget* parent /* = 0 */)
	: QListWidget(parent)
	, m_num(0)
{
	createItems();
	setIconSize(QSize(80, 40));
	setAcceptDrops(true);
	setAutoFillBackground(true);
}

KICListWin::~KICListWin()
{
}

void KICListWin::mousePressEvent(QMouseEvent* event)
{
	if (Qt::LeftButton == event->button())
		startPos = event->pos();
	QListWidget::mousePressEvent(event);
}

void KICListWin::mouseMoveEvent(QMouseEvent* event)
{
	if (Qt::LeftButton & event->buttons())
	{
		int distance = (event->pos() - startPos).manhattanLength();
		if (distance >= QApplication::startDragDistance())
			performDrag();
	}
	QListWidget::mouseMoveEvent(event);
}

void KICListWin::dragMoveEvent(QDragMoveEvent* event)
{
}

void KICListWin::dragEnterEvent(QDragEnterEvent* event)
{
	KICListWin* source = dynamic_cast<KICListWin*>(event->source());
	if (source)
		event->acceptProposedAction();
}

void KICListWin::dropEvent(QDropEvent* event)
{
}

void KICListWin::performDrag()
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

void KICListWin::createItems()
{
	QList<KBase*> ICList = ::g_ICMap.values();
	KBase* pIC;
	for(int i = 0; i < ICList.count(); ++i)
	{
		pIC = ICList[i];
		QPixmap pix = createPixmap(pIC);
		QListWidgetItem* item = new QListWidgetItem(
			QIcon(pix), pIC->getName());
		addItem(item);
	}
}

QPixmap KICListWin::createPixmap(const KBase* pIC)
{
	QPixmap pix(81, 41); 
	pix.fill(QColor(255, 255, 255, 0));//透明背景
	//将将painter移动到pix的中心
	QPainter painter(&pix);
	painter.translate(40, 20);
	pIC->draw(painter);

	return pix;
}