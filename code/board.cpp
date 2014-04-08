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
	, m_size(1500, 1500)
	, m_step(10)
	, m_pIC(NULL)
	, m_posFlag(KBoard::NOFLAG)
	, m_model(KBoard::NOMODEL)
	, m_nPinIndex(-1)
	, m_nSwitchIndex(-1)
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

QString KBoard::fileName() const
{
	return file.fileName();
}

void KBoard::setFileName(const QString& sFileName)
{
	if (file.fileName() != sFileName)
		file.setFileName(sFileName);
}

//begin: slots
void KBoard::buildIC()
{
	std::cout << "buildIC" << std::endl;

	QFile file;
	QString fileName = QFileDialog::getSaveFileName(
		this, "生成元件", "", "IC Files (*.xml)");
	file.setFileName(fileName);
	file.open(QIODevice::WriteOnly);
	QTextStream out(&file);

	QDomDocument doc;

	QDomElement element = doc.createElement("element");
	element.setAttribute("id", "");
	element.setAttribute("inPinNum", count(KBase::INIC));
	element.setAttribute("outPinNum", count(KBase::OUTIC));
	element.setAttribute("pinNum", count(KBase::INIC) + count(KBase::OUTIC));
	element.setAttribute("description", "");


	KBase* pIC;
	ILink link;

	//componentList
	QDomElement cE = doc.createElement("componentList");
	cE.setAttribute("num", count(KBase::OTHERIC));
	for (int i = 0; i < m_ICList.count(); ++i)
	{
		pIC = m_ICList[i];
		if (pIC->type() == KBase::OTHERIC)
		{
			QDomElement item = doc.createElement("item");
			item.setAttribute("id", pIC->name());
			cE.appendChild(item);
		}
	}

	//inToInList//outToInList  && outToOutList
	QDomElement iTIE = doc.createElement("inToInList");
	QDomElement oTOE = doc.createElement("outToOutList");
	QDomElement oTIE = doc.createElement("outToInList");
	for (int i = 0; i < m_ICList.count(); ++i)
	{
		pIC = m_ICList[i];
		QList<ILink> linkList = pIC->links();
		for (int j = 0; j < linkList.count(); ++j)
		{
			link = linkList[j];
			QDomElement item = doc.createElement("item");
			if (pIC->type() == KBase::INIC && link.p->type() == KBase::OTHERIC)//inToInList
			{
				item.setAttribute("index", indexOf(pIC, pIC->type()));
				item.setAttribute("target", indexOf(link.p, link.p->type()));
				item.setAttribute("targetIndex", link.j);
				iTIE.appendChild(item);
			}
			else if (pIC->type() == KBase::OTHERIC && link.p->type() == KBase::OUTIC)//outToOutList
			{
				item.setAttribute("source", indexOf(pIC, pIC->type()));
				item.setAttribute("sourceIndex", link.i);
				item.setAttribute("index", indexOf(link.p, link.p->type()) + count(KBase::INIC));
				oTOE.appendChild(item);
			}
			else if (pIC->type() == KBase::OTHERIC && link.p->type() == KBase::OTHERIC)//outToInList
			{
				item.setAttribute("source", indexOf(pIC, pIC->type()));
				item.setAttribute("sourceIndex", link.i);
				item.setAttribute("target", indexOf(link.p, link.p->type()));
				item.setAttribute("targetIndex", link.j);
				oTIE.appendChild(item);
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

void KBoard::openFile(const QString& sFileName)
{
	file.setFileName(sFileName);
	file.open(QIODevice::ReadOnly);	
	QDomDocument doc;
	doc.setContent(&file);
	file.close();

	QDomElement element;
	QString id;
	int x, y, source, sourceIndex, target, targetIndex;

	QDomNodeList ICNodeList = doc.documentElement().
		firstChildElement("ICList").childNodes();
	for (int i = 0; i < ICNodeList.count(); ++i)
	{
		element = ICNodeList.at(i).toElement();
		id = element.attribute("id");
		x = element.attribute("x").toInt();
		y = element.attribute("y").toInt();
		addIC(id, QPoint(x, y));
	}

	QDomNodeList WIRENodeList = doc.documentElement().
		firstChildElement("WIREList").childNodes();
	for (int i = 0; i < WIRENodeList.count(); ++i)
	{
		element = WIRENodeList.at(i).toElement();
		source = element.attribute("source").toInt();
		sourceIndex = element.attribute("sourceIndex").toInt();
		target = element.attribute("target").toInt();
		targetIndex = element.attribute("targetIndex").toInt();
		addWire(m_ICList[source], sourceIndex, m_ICList[target], targetIndex);
	}
	m_selectedICList.clear();
	m_selectedWireList.clear();
}

void KBoard::saveFile()
{
	file.open(QIODevice::WriteOnly);
	QTextStream out(&file);

	QDomDocument doc;
	QDomElement rootE = doc.createElement("root");

	QDomElement ICListE = doc.createElement("ICList");
	for(int i = 0; i < m_ICList.count(); ++i)
	{
		QDomElement item = doc.createElement("item");
		KBase* pIC = m_ICList[i];
		item.setAttribute("id", pIC->name());
		item.setAttribute("x", pIC->getCenterPos().x());
		item.setAttribute("y", pIC->getCenterPos().y());
		ICListE.appendChild(item);
	}

	QDomElement WIREListE = doc.createElement("WIREList");
	KBase* pSource, *pTarget;
	int sourceIndex, targetIndex;
	for (int i = 0; i < m_WIREList.count(); ++i)
	{
		m_WIREList[i]->get(&pSource, &sourceIndex, &pTarget, &targetIndex);
		QDomElement item = doc.createElement("item");
		item.setAttribute("source", m_ICList.indexOf(pSource));
		item.setAttribute("sourceIndex", sourceIndex);
		item.setAttribute("target", m_ICList.indexOf(pTarget));
		item.setAttribute("targetIndex", targetIndex);
		WIREListE.appendChild(item);
	}
	rootE.appendChild(ICListE);
	rootE.appendChild(WIREListE);

	doc.appendChild(rootE);
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
		if (m_posFlag == ONSWITCH || m_posFlag == ONPIN )
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
		std::cout << m_pIC->name().toStdString() << " " << m_nPinIndex << std::endl;
	if (event->buttons() & Qt::LeftButton)
	{
		if (m_posFlag == ONSWITCH)
			dynamic_cast<KINICBase*>(m_pIC)->click(m_nSwitchIndex);
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
	
	m_WIREList.append(wire);
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
		m_ICList.removeOne(m_selectedICList[i]);
		deleteWire(m_selectedICList[i]);
		delete m_selectedICList[i];
	}

	for (int i = 0; i < m_selectedWireList.count(); ++i)
	{
		m_WIREList.removeOne(m_selectedWireList[i]);
		delete m_selectedWireList[i];
	}

	m_selectedICList.clear();
	m_selectedWireList.clear();
}

void KBoard::deleteWire(KBase* pIC)
{
	QList<KWire*> deleteList;
	for (int i = 0; i < m_WIREList.count(); ++i)//找出与pIC相连的电线
	{
		if (m_WIREList[i]->inWire(pIC))
			deleteList.append(m_WIREList[i]);
	}
	for (int i = 0; i < deleteList.count(); ++i)//删除相连的电线
	{
		m_WIREList.removeOne(deleteList[i]);
		delete deleteList[i];
	}
}

KBoard::POSFLAG KBoard::posFlag(const QPoint& pos)
{	
	if (atSwitch(pos, &m_pIC, &m_nSwitchIndex))
		return ONSWITCH;
	if(atIC(pos, &m_pIC))
		return ONIC;
	if (atPin(pos, &m_pIC, &m_nPinIndex))
		return ONPIN;
	if (atWire(pos, &m_pWire))
		return ONWIRE;
	return NOFLAG;
}

bool KBoard::atIC(const QPoint& pos, KBase** pIC)
{
	for (int i = 0; i < m_ICList.count(); ++i)
	{
		if (m_ICList[i]->contains(pos))
		{
			*pIC = m_ICList[i];
			return true;
		}
	}	
	return false;
}

bool KBoard::atSwitch(const QPoint& pos, KBase** pIC, int* switchIndex)
{
	for (int i = 0; i < m_ICList.count(); ++i)
	{
		if (m_ICList[i]->type() == KBase::INIC)
		{
			if (dynamic_cast<KINICBase*>(m_ICList[i])->atSwitch(pos, switchIndex))
			{
				*pIC = m_ICList[i];	
				return true;
			}
		}	
	}
	return false;
}

bool KBoard::atPin(const QPoint& pos, KBase** pIC, int* pinIndex)
{
	int index = -1;
	for (int i = 0; i < m_ICList.count(); ++i)
	{
		index = m_ICList[i]->pinAt(pos);
		if (index != -1)
		{
			*pIC = m_ICList[i];
			*pinIndex = index;
			return true;
		}
	}	
	return false;
}

bool KBoard::atWire(const QPoint& pos, KWire** pWire)
{
	for (int i = 0; i < m_WIREList.count(); ++i)
	{
		if (m_WIREList[i]->contains(pos))
		{
			*pWire = m_WIREList[i];
			return true;
		}
	}
	return false;
}

int KBoard::indexOf(KBase* pIC, KBase::TYPE type) const
{
	int index  = 0;
	for (int i = 0; i < m_ICList.count(); ++i)
	{
		if (m_ICList[i]->type() == type)
		{
			if (pIC == m_ICList[i])
				return index;
			else 
				++index;
		}
	}
	return -1;
}
int KBoard::count(KBase::TYPE type) const
{
	int num = 0;
	for (int i = 0; i < m_ICList.count(); ++i)
	{
		if (m_ICList[i]->type() == type)
			++num;
	}
	return num;
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

// void KBoard::clickSwitch(KBase* pIC)
// {
// 	if (dynamic_cast<KINICBase*>(pIC))
// 		dynamic_cast<KINICBase*>(pIC)->click(m_nSwitchIndex);
// }

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
	painter.restore();
}

void KBoard::drawWireList(QPainter& painter)
{
	painter.save();
	for (int i = 0; i < m_WIREList.count(); ++i)
	{
		m_WIREList[i]->draw(painter);
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
		m_selectedWireList[i]->get(&pIC1, &index1, &pIC2, &index2);
		painter.drawPoint(pIC1->getPinPos(index1));
		painter.drawPoint(pIC2->getPinPos(index2));
		painter.drawPoint((pIC1->getPinPos(index1) + pIC2->getPinPos(index2)) / 2);
	}

	painter.restore();
}