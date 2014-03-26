#include <QtGui>
#include "createic.h"
#include "iclistwin.h"
#include "board.h"

KBoardWin::KBoardWin(QWidget* parent /* = 0 */)
	: QScrollArea(parent)
	
{
	m_pBoard = new KBoard(this);
	setWidget(m_pBoard);
	setStyleSheet("QScrollArea{background-color:gray}");
}

KBoardWin::~KBoardWin()
{
}

QSize KBoardWin::sizeHint() const
{
	return m_pBoard->getSize();
}


//---------------------KBoard---------------------------------

KBoard::KBoard(QWidget* parent /* = 0 */)
	: QLabel(parent)
	, m_factor(1.0)
	, m_offset(0, 0)
	, m_startPos(0, 0)
	, m_currentPos(0, 0)
	, m_size(1000, 750)
	, m_step(10)
	, m_pIC(NULL)
	, m_posFlag(KBoard::NONE)
	, m_nPinIndex(-1)
{
	setMouseTracking(true);
	setFocusPolicy(Qt::StrongFocus);
	setAutoFillBackground(true);
	setAcceptDrops(true);
	resize(m_size);

	addIC("POWER", QPoint(100, 50));
	addIC("POWER", QPoint(100, 80));
	addIC("POWER", QPoint(100, 110));
	addIC("POWER", QPoint(100, 140));
	addIC("74LS02", QPoint(200, 50));
	addIC("74LS86", QPoint(200, 110));
	addIC("74LS32", QPoint(300, 70));
	addIC("LED", QPoint(400, 70));

	m_powerList[0]->appendLink(1, m_ICList[0], 0);
	m_powerList[1]->appendLink(1, m_ICList[0], 1);
	m_powerList[2]->appendLink(1, m_ICList[1], 0);
	m_powerList[3]->appendLink(1, m_ICList[1], 1);

	m_ICList[0]->appendLink(2, m_ICList[2], 0);
	m_ICList[1]->appendLink(2, m_ICList[2], 1);

	m_ICList[2]->appendLink(2, m_LEDList[0], 0);

	turnOn();
}

KBoard::~KBoard()
{
	for (int i = 0; i < m_ICList.count(); ++i)
	{
		if (m_ICList[i])
			delete m_ICList[i];
	}
	m_ICList.clear();
}

QSize KBoard::getSize() const
{
	return  m_size;
}

void KBoard::dragEnterEvent(QDragEnterEvent* event)
{
	KICListWin* source = dynamic_cast<KICListWin*>(event->source());
	if (source)
	{	
		event->acceptProposedAction();
	}
}

void KBoard::keyPressEvent(QKeyEvent* event)
{
	if (Qt::Key_Delete == event->key())
	{
		deleteSelected();
	}
	update();
}

void KBoard::dropEvent(QDropEvent* event)
{
	setFocus();/*获取焦点*/
	KICListWin* source = dynamic_cast<KICListWin*>(event->source());
	if (source)
	{
		addIC(event->mimeData()->text(), event->pos());
		event->setDropAction(Qt::MoveAction);
		event->accept();	
		update();
	}
}

void KBoard::mouseMoveEvent(QMouseEvent* event)
{	
	m_currentPos = event->pos();
	if (event->buttons() & Qt::LeftButton)
	{
		m_offset = m_currentPos - m_startPos;
		update();
	}
	else
	{
		m_posFlag = posFlag(transform(m_currentPos));
		if (m_posFlag == ONPOWERSWITCH || m_posFlag == ONPIN)
			setCursor(Qt::PointingHandCursor);
		else if (m_posFlag == ONIC)
			setCursor(Qt::SizeAllCursor);
		else if (m_posFlag == NONE)
			setCursor(Qt::ArrowCursor);
	}	
}

void KBoard::mousePressEvent(QMouseEvent* event)
{
	m_startPos = event->pos();
	m_currentPos = m_startPos;
	m_offset = QPoint(0, 0);

	if (m_pIC && m_nPinIndex != -1)
		std::cout << m_pIC->getName().toStdString() << " " << m_nPinIndex << std::endl;

	updateSelectedList(event->modifiers());

	if (m_posFlag == ONPOWERSWITCH)
		updatePower(m_pIC);
	
	update();
	QLabel::mousePressEvent(event);
}

void KBoard::mouseReleaseEvent(QMouseEvent* event)
{
	offsetSelected();
	update();
	QLabel::mouseReleaseEvent(event);
	m_offset = QPoint(0, 0);//偏移清"0"
}

void KBoard::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setWindow(0, 0, m_size.width(), m_size.height());

	drawSolid(painter);
	drawBounding(painter);
	drawDash(painter);
}

void KBoard::wheelEvent(QWheelEvent* event)
{
	if (event->modifiers() == Qt::ControlModifier)
	{
		if (event->delta() < 0 && m_factor > 0.6)
		{
			m_factor -= 0.2;
			setFixedSize(m_size * m_factor);
		}
		else if (event->delta() > 0 && m_factor < 5)
		{
			m_factor += 0.2;
			setFixedSize(m_size * m_factor);
		}
		event->accept();
	}
	else
	{
		QLabel::wheelEvent(event);
	}
}

void KBoard::addIC(const QString& name, const QPoint& pos)
{
	KBase* temp = ::g_createIC(name);
	temp->setBoard(this);
	temp->setCenterPos(transform(pos));

	if (name == "POWER")
		m_powerList.append(temp);
	else if(name == "LED")
		m_LEDList.append(temp);
	else
		m_ICList.append(temp);

	m_selectedList.clear();
	m_selectedList.append(temp);
	m_pIC = temp;
}

QPoint KBoard::transform(const QPoint& pos)
{
	return pos / m_factor;//除去放大因子
}

void KBoard::deleteSelected()
{
	if (m_selectedList.isEmpty())
		return;

	for (int i = 0; i < m_selectedList.count(); ++i)
	{
		if (m_selectedList[i]->getName() == "POWER")
			m_powerList.removeOne(m_selectedList[i]);
		else if (m_selectedList[i]->getName() == "LED")
			m_LEDList.removeOne(m_selectedList[i]);
		else
			m_ICList.removeOne(m_selectedList[i]);
		delete m_selectedList[i];
	}
	m_selectedList.clear();
}

KBoard::POSFLAG KBoard::posFlag(const QPoint& pos)
{	
	m_pIC = powerSwitchAt(pos);
	if (m_pIC)
		return ONPOWERSWITCH;
	m_pIC = ICAt(pos);
	if(m_pIC)
		return ONIC;
	m_pIC = pinAt(pos);
	if (m_pIC)
		return ONPIN;
	return NONE;
}

KBase* KBoard::ICAt(const QPoint& pos)
{
	for (int i = 0; i < m_ICList.count(); ++i)
	{
		if (m_ICList[i]->contains(pos))
			return m_ICList[i];
	}
	for (int i = 0; i < m_powerList.count(); ++i)
	{
		if (m_powerList[i]->contains(pos))
			return m_powerList[i];
	}
	for (int i = 0; i < m_LEDList.count(); ++i)
	{
		if (m_LEDList[i]->contains(pos))
			return m_LEDList[i];
	}
	return NULL;
}

KBase* KBoard::powerSwitchAt(const QPoint& pos)
{
	for (int i = 0; i < m_powerList.count(); ++i)
	{
		if (dynamic_cast<KPower*>(m_powerList[i])->onSwitch(pos))
			return m_powerList[i];
	}
	return NULL;
}

KBase* KBoard::pinAt(const QPoint& pos)
{
	for (int i = 0; i < m_ICList.count(); ++i)
	{
		m_nPinIndex = m_ICList[i]->onPin(pos);
		if (m_nPinIndex != -1)
			return m_ICList[i];
	}
	for (int i = 0; i < m_powerList.count(); ++i)
	{
		m_nPinIndex = m_powerList[i]->onPin(pos);
		if (m_nPinIndex != -1)
			return m_powerList[i];
	}
	for (int i = 0; i < m_LEDList.count(); ++i)
	{
		m_nPinIndex = m_LEDList[i]->onPin(pos);
		if (m_nPinIndex != -1)
			return m_LEDList[i];
	}	
	return NULL;
}

void KBoard::offsetSelected()
{
	if (m_selectedList.isEmpty())
		return;	 
	QPoint offset = transform(m_offset);
	for (int i = 0; i < m_selectedList.count(); ++i)
	{
		m_selectedList[i]->offset(offset);
	}
}

void KBoard::updatePower(KBase* pIC)
{
	if (dynamic_cast<KPower*>(pIC))
		dynamic_cast<KPower*>(pIC)->click();
}

void KBoard::updateSelectedList(Qt::KeyboardModifiers modifier)
{
	if (Qt::ControlModifier == modifier)
	{
		if (m_posFlag == ONIC && !m_selectedList.contains(m_pIC))
			m_selectedList.append(m_pIC);
		else if (m_posFlag == ONIC && m_selectedList.contains(m_pIC))
			m_selectedList.removeOne(m_pIC);
	}
	else
	{	 
		if (m_posFlag != ONIC)
			m_selectedList.clear();
		else if (!m_selectedList.contains(m_pIC))
		{
			m_selectedList.clear();
			m_selectedList.append(m_pIC);
		}
	}
}

void KBoard::turnOn()
{
	for (int i = 0; i < m_powerList.count(); ++i)
	{
		for (int j = 0; j < m_powerList[i]->getPinNum(); ++j)
			m_powerList[i]->sendChange(j);
	}
}

void KBoard::turnOff()
{
	for (int i = 0; i < m_ICList.count(); ++i)
	{
		m_ICList[i]->reset();
	}
}

void KBoard::drawDash(QPainter& painter)
{
	painter.save();
	painter.setPen(QPen(Qt::blue, 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));

	KBase* base;
	for (int i = 0; i < m_selectedList.count(); ++i)
	{
		base = m_selectedList[i];
		QPoint pos = base->getCenterPos();
		base->offset(transform(m_offset));
		base->draw(painter);
		base->setCenterPos(pos);
	}
	painter.restore();
}

void KBoard::drawBounding(QPainter& painter)
{	
	painter.save();
	painter.setPen(QPen(Qt::blue, 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));

	KBase* pIC;	
	for (int i = 0; i < m_selectedList.count(); ++i)
	{
		pIC = m_selectedList[i];
		QRect rect = pIC->getBoundingRect();
		int x1, y1, x2, y2;
		rect.getCoords(&x1, &y1, &x2, &y2);
		rect.setCoords(x1 - m_step, y1 - m_step, x2 + m_step, y2 + m_step);

		painter.translate(pIC->getCenterPos());
		painter.drawRect(rect);
		painter.translate(-pIC->getCenterPos());
	}
	painter.restore();
}

void KBoard::drawSolid(QPainter& painter)
{
	painter.save();
	painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	for (int i = 0; i < m_ICList.count(); ++i)
	{
		m_ICList[i]->draw(painter);
	}
	for (int i = 0; i < m_powerList.count(); ++i)
	{
		m_powerList[i]->draw(painter);
	}
	for (int i = 0; i < m_LEDList.count(); ++i)
	{
		m_LEDList[i]->draw(painter);
	}
	painter.restore();
}