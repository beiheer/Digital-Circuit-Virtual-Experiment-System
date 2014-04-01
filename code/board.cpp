#include <QtGui>
#include <QtXml/QtXml>
#include <QTextStream>
#include "createic.h"
#include "iclistwin.h"
#include "wire.h"
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

KBoard* KBoardWin::getBoard() const
{
	return m_pBoard;
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
	, m_posFlag(KBoard::NOFLAG)
	, m_model(KBoard::NOMODEL)
	, m_nPinIndex(-1)
{
	setMouseTracking(true);
	setFocusPolicy(Qt::StrongFocus);
	setAutoFillBackground(true);
	setAcceptDrops(true);
	resize(m_size);
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

//begin: slots
void KBoard::buildIC()
{
	std::cout << "buildIC" << std::endl;
	QFile file("E://IC.xml");
	file.open(QIODevice::WriteOnly);
	QTextStream out(&file);

	QDomDocument doc;

	QDomElement element = doc.createElement("element");
	element.setAttribute("id", "");
	element.setAttribute("inPinNum", m_powerList.count());
	element.setAttribute("outPinNum", m_LEDList.count());
	element.setAttribute("pinNum", m_powerList.count() + m_LEDList.count());
	element.setAttribute("description", "");


	//componentList
	QDomElement cE = doc.createElement("componentList");
	cE.setAttribute("num", m_ICList.count());
	for (int i = 0; i < m_ICList.count(); ++i)
	{
		QDomElement item = doc.createElement("item");
		item.setAttribute("id", m_ICList[i]->getName());
		cE.appendChild(item);
	}

	//inToInList
	QDomElement iTIE = doc.createElement("inToInList");
	for (int i = 0; i < m_powerList.count(); ++i)
	{
		QList<ILink> linkList = m_powerList[i]->getLinks();
		for (int j = 0; j < linkList.count(); ++j)
		{
			QDomElement item = doc.createElement("item");
			item.setAttribute("index", i);
			item.setAttribute("target", m_ICList.indexOf(linkList[j].p));
			item.setAttribute("targetIndex", linkList[j].j);
			iTIE.appendChild(item);
		}
	}

	
	//outToInList  && outToOutList
	QDomElement oTOE = doc.createElement("outToOutList");
	QDomElement oTIE = doc.createElement("outToInList");
	for (int i = 0; i < m_ICList.count(); ++i)
	{
		QList<ILink> linkList = m_ICList[i]->getLinks();
		for (int j = 0; j < linkList.count(); ++j)
		{
			QDomElement item = doc.createElement("item");
			if (m_ICList.contains(linkList[j].p))//outToInList
			{
				item.setAttribute("source", i);
				item.setAttribute("sourceIndex", linkList[j].i);
				item.setAttribute("target", m_ICList.indexOf(linkList[j].p));
				item.setAttribute("targetIndex", linkList[j].j);
				oTIE.appendChild(item);
			}
			else if (m_LEDList.contains(linkList[j].p))//outToOutList
			{
				item.setAttribute("source", i);
				item.setAttribute("sourceIndex", linkList[j].i);
				item.setAttribute("index", m_LEDList.indexOf(linkList[j].p) + m_powerList.count());
				oTOE.appendChild(item);
			}	
		}
	}

	element.appendChild(cE);
	element.appendChild(iTIE);
	element.appendChild(oTOE);
	element.appendChild(oTIE);
	doc.appendChild(element);
	doc.save(out, 4, QDomNode::EncodingFromTextStream);
	file.close();
}
//end: slots

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
		if (m_model == CREATEWIRE)
			update();

		m_posFlag = posFlag(transform(m_currentPos));
		if (m_posFlag == ONPOWERSWITCH || m_posFlag == ONPIN )
			setCursor(Qt::PointingHandCursor);
		else if (m_posFlag == ONIC || m_posFlag == ONWIRE)
			setCursor(Qt::SizeAllCursor);
		else if (m_posFlag == NOFLAG)
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
	if (event->buttons() & Qt::LeftButton)
	{
		if (m_posFlag == ONPOWERSWITCH)
			updatePower(m_pIC);
		if (m_posFlag == ONPIN)
			createWire();
		updateSelectedIC(event->modifiers());
		updateSelectedWire(event->modifiers());
	}
	else if (event->buttons() & Qt::RightButton)
	{
		m_selectedICList.clear();
		m_model = NOMODEL;
	}
	update();
	QLabel::mousePressEvent(event);
}

void KBoard::mouseReleaseEvent(QMouseEvent* event)
{
	offsetSelectedIC();
	update();
	QLabel::mouseReleaseEvent(event);
	m_offset = QPoint(0, 0);//偏移清"0"
}

void KBoard::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setWindow(0, 0, m_size.width(), m_size.height());

	if (m_model == CREATEWIRE)
	{
		painter.drawLine(m_wire.pIC1->getPinPos(m_wire.index1), transform(m_currentPos));
	}
	drawICList(painter);
	drawWireList(painter);
	drawSelectedIC(painter);
	drawSelectedWire(painter);
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

	m_selectedICList.clear();
	m_selectedICList.append(temp);
	m_pIC = temp;
}

void KBoard::addWire(KBase* pIC1, int index1, KBase* pIC2, int index2)
{
	if(pIC1 == pIC2 && index1 == index2)
		return;

	KWire* wire = new KWire(pIC1, index1, pIC2, index2);
	
	m_wireList.append(wire);
	m_selectedWireList.clear();
	m_selectedWireList.append(wire);
	m_pWire = wire;
}

void KBoard::createWire()
{
	if (m_model == CREATEWIRE)
	{
		m_wire.pIC2 = m_pIC;
		m_wire.index2 = m_nPinIndex;
		addWire(m_wire.pIC1, m_wire.index1, m_wire.pIC2, m_wire.index2);
		m_model = NOMODEL;
	}
	else
	{
		m_model = CREATEWIRE;
		m_wire.pIC1 = m_pIC;
		m_wire.index1 = m_nPinIndex;
	}
}

QPoint KBoard::transform(const QPoint& pos)
{
	return pos / m_factor;//除去放大因子
}

void KBoard::deleteSelected()
{
	if (m_selectedICList.isEmpty() && m_selectedWireList.isEmpty())
		return;

	for (int i = 0; i < m_selectedICList.count(); ++i)
	{
		if (m_selectedICList[i]->getName() == "POWER")
			m_powerList.removeOne(m_selectedICList[i]);
		else if (m_selectedICList[i]->getName() == "LED")
			m_LEDList.removeOne(m_selectedICList[i]);
		else
			m_ICList.removeOne(m_selectedICList[i]);
		
		deleteWire(m_selectedICList[i]);
		delete m_selectedICList[i];
	}

	for (int i = 0; i < m_selectedWireList.count(); ++i)
	{
		m_wireList.removeOne(m_selectedWireList[i]);
		delete m_selectedWireList[i];
	}

	m_selectedICList.clear();
	m_selectedWireList.clear();
}

void KBoard::deleteWire(KBase* pIC)
{
	QList<KWire*> deleteList;
	for (int i = 0; i < m_wireList.count(); ++i)//找出与pIC相连的电线
	{
		if (m_wireList[i]->inWire(pIC))
			deleteList.append(m_wireList[i]);
	}
	for (int i = 0; i < deleteList.count(); ++i)//删除相连的电线
	{
		m_wireList.removeOne(deleteList[i]);
		delete deleteList[i];
	}
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
	m_pWire = wireAt(pos);
	if (m_pWire)
		return ONWIRE;
	return NOFLAG;
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
		m_nPinIndex = m_ICList[i]->pinAt(pos);
		if (m_nPinIndex != -1)
			return m_ICList[i];
	}
	for (int i = 0; i < m_powerList.count(); ++i)
	{
		m_nPinIndex = m_powerList[i]->pinAt(pos);
		if (m_nPinIndex != -1)
			return m_powerList[i];
	}
	for (int i = 0; i < m_LEDList.count(); ++i)
	{
		m_nPinIndex = m_LEDList[i]->pinAt(pos);
		if (m_nPinIndex != -1)
			return m_LEDList[i];
	}	
	return NULL;
}

KWire* KBoard::wireAt(const QPoint& pos)
{
	for (int i = 0; i < m_wireList.count(); ++i)
	{
		if (m_wireList[i]->contains(pos))
			return m_wireList[i];
	}
	return NULL;
}

void KBoard::offsetSelectedIC()
{
	if (m_selectedICList.isEmpty())
		return;	 
	QPoint offset = transform(m_offset);
	for (int i = 0; i < m_selectedICList.count(); ++i)
	{
		m_selectedICList[i]->offset(offset);
	}
}

void KBoard::updatePower(KBase* pIC)
{
	if (dynamic_cast<KPower*>(pIC))
		dynamic_cast<KPower*>(pIC)->click();
}

void KBoard::updateSelectedIC(Qt::KeyboardModifiers modifier)
{
	if (Qt::ControlModifier == modifier)
	{
		if (m_posFlag == ONIC && !m_selectedICList.contains(m_pIC))
			m_selectedICList.append(m_pIC);
		else if (m_posFlag == ONIC && m_selectedICList.contains(m_pIC))
			m_selectedICList.removeOne(m_pIC);
	}
	else
	{	 
		if (m_posFlag != ONIC)
			m_selectedICList.clear();
		else if (!m_selectedICList.contains(m_pIC))
		{
			m_selectedICList.clear();
			m_selectedICList.append(m_pIC);
		}
	}
}

void KBoard::updateSelectedWire(Qt::KeyboardModifiers modifier)
{
	if (Qt::ControlModifier == modifier)
	{
		if (m_posFlag == ONWIRE && !m_selectedWireList.contains(m_pWire))
			m_selectedWireList.append(m_pWire);
		else if (m_posFlag == ONWIRE && m_selectedWireList.contains(m_pWire))
			m_selectedWireList.removeOne(m_pWire);
	}
	else
	{	 
		if (m_posFlag != ONWIRE)
			m_selectedWireList.clear();
		else if (!m_selectedWireList.contains(m_pWire))
		{
			m_selectedWireList.clear();
			m_selectedWireList.append(m_pWire);
		}
	}
}

void KBoard::drawICList(QPainter& painter)
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

void KBoard::drawWireList(QPainter& painter)
{
	painter.save();
	for (int i = 0; i < m_wireList.count(); ++i)
	{
		m_wireList[i]->draw(painter);
	}
	painter.restore();
}

void KBoard::drawSelectedIC(QPainter& painter)
{
	drawBounding(painter);
	drawDash(painter);
}

void KBoard::drawDash(QPainter& painter)
{
	painter.save();
	painter.setPen(QPen(Qt::blue, 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));

	KBase* base;
	for (int i = 0; i < m_selectedICList.count(); ++i)
	{
		base = m_selectedICList[i];
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
	for (int i = 0; i < m_selectedICList.count(); ++i)
	{
		pIC = m_selectedICList[i];
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


void KBoard::drawSelectedWire(QPainter& painter)
{
	painter.save();
	KBase* pIC1, *pIC2;
	int index1, index2;
	painter.setPen(QPen(Qt::red, 5, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
	for (int i = 0; i < m_selectedWireList.count(); ++i)
	{
		m_selectedWireList[i]->getBeginAndEnd(&pIC1, &index1, &pIC2, &index2);
		painter.drawPoint(pIC1->getPinPos(index1));
		painter.drawPoint(pIC2->getPinPos(index2));
		painter.drawPoint((pIC1->getPinPos(index1) + pIC2->getPinPos(index2)) / 2);
	}

	painter.restore();
}