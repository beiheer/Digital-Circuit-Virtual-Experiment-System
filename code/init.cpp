#include <QFile>
#include <QFont>
#include "init.h"

void _Init()
{
	KInitICMap initEleMap;
	initEleMap.init();
}

KInitICMap::KInitICMap()
{
}

KInitICMap::~KInitICMap()
{
}

void KInitICMap::init()
{
	QFile file;

	file.setFileName(PATH + LISTFILENAME);
	file.open(QFile::ReadOnly | QFile::Text);
	m_listDoc.setContent(&file);
	file.close();

	file.setFileName(PATH + ICSLISTNAME);
	file.open(QFile::ReadOnly | QFile::Text);
	m_elementsDoc.setContent(&file);
	file.close();

	file.setFileName(PATH + SHELLSFILENAME);
	file.open(QFile::ReadOnly | QFile::Text);
	m_shellsDoc.setContent(&file);
	file.close();

	QDomNodeList nodeList = m_listDoc.documentElement().childNodes();
	KBase* pIC;
	QString ICName;
	for (int i = 0; i < nodeList.count(); ++i)
	{
		ICName = nodeList.at(i).toElement().attribute("id");
		pIC = createIC(ICName);
		::g_ICMap.insert(ICName, pIC);
	}
}

QDomElement KInitICMap::getElementById(QDomDocument& doc, 
	const QString& id)
{
	QDomNodeList nodeList = doc.documentElement().childNodes();
	int num = nodeList.count();
	for (int i = 0; i < num; ++i)
	{
		if (nodeList.at(i).toElement().attribute("id") == id)
			return nodeList.at(i).toElement();
	}
	return QDomElement();
}

KBase* KInitICMap::createIC(const QString& ICName)
{	
	QDomElement element = getElementById(m_shellsDoc, ICName);
	QPainterPath path = createPath(element.firstChildElement("path"));
	QList<QPoint> pinPosList = createPinPosList(
		element.firstChildElement("pinPosList"));
	QList<ITips> tipsList = createTipsList(
		element.firstChildElement("tipsList"));

	KBase* pIC = NULL;

	if (ICName ==  "74LS04")//NOT
		pIC = new K74LS04(path, pinPosList, tipsList);
	else if (ICName == "74LS08")//AND
		pIC = new K74LS08(path, pinPosList, tipsList);
	else if (ICName == "74LS32")//OR
		pIC = new K74LS32(path, pinPosList, tipsList); 
	else if (ICName == "74LS125")//ÈýÌ¬
		pIC = new K74LS125(path, pinPosList, tipsList);
	else if (ICName == "SRAM2114")
		pIC = new KSRam2114(path, pinPosList, tipsList);
	else if (ICName == "SRAM6116")
		pIC = new KSRam6116(path, pinPosList, tipsList);
	else if (ICName == "SRAM6264")
		pIC = new KSRam6264(path, pinPosList, tipsList);
	else if (ICName == "SRAM62256")
		pIC = new KSRam62256(path, pinPosList, tipsList);
	else if (ICName == "SRAM62128")
		pIC = new KSRam62128(path, pinPosList, tipsList);
	else if (ICName == "SRAM628128")
		pIC = new KSRam628128(path, pinPosList, tipsList);
	else if (ICName == "Vcc")
		pIC = new KVcc(path, pinPosList, tipsList);
	else if (ICName == "LED")
		pIC = new KLED(path, pinPosList, tipsList);
	else if (ICName == "SWITCH")
		pIC = new KSWITCH(path, pinPosList, tipsList);
	else if (ICName == "CLOCK")
		pIC = new KCLOCK(path, pinPosList, tipsList);
	else
		pIC = createUniversalIC(ICName, path, pinPosList, tipsList);
	
	return pIC;
}

QPainterPath KInitICMap::createPath(const QDomElement& element)
{
	QPainterPath path;
	if (element.isNull())
		return path;

	QDomNodeList nodeList = element.elementsByTagName("operate");
	int num = nodeList.count();
	if (num > 0)
	{
		QDomElement operateElement;
		QString operateName;
		QFont font;
		for (int i = 0; i < num; ++i)
		{
			operateElement = nodeList.at(i).toElement();
			operateName = operateElement.attribute("name");
			if (operateName == "moveTo")
			{
				path.moveTo(
					(qreal)operateElement.attribute("x").toDouble(),
					(qreal)operateElement.attribute("y").toDouble());
			}
			else if (operateName == "lineTo")
			{
				path.lineTo(
					(qreal)operateElement.attribute("x").toDouble(),
					(qreal)operateElement.attribute("y").toDouble());
			}
			else if (operateName == "arcTo")
			{
				path.arcTo(
					(qreal)operateElement.attribute("x").toDouble(),
					(qreal)operateElement.attribute("y").toDouble(),
					(qreal)operateElement.attribute("width").toDouble(),
					(qreal)operateElement.attribute("height").toDouble(),
					(qreal)operateElement.attribute("startAngle").toDouble(),
					(qreal)operateElement.attribute("sweepLength").toDouble());
			}
			else if (operateName == "text")
			{
				path.addText(
					(qreal)operateElement.attribute("x").toDouble(),
					(qreal)operateElement.attribute("y").toDouble(),
					font,
					operateElement.attribute("text"));
			}
		}
	}
	return path;
}

QList<QPoint> KInitICMap::createPinPosList(
	const QDomElement& element)
{
	QList<QPoint> pinPosList;
	if (element.isNull())
		return pinPosList;
	QDomNodeList nodeList = element.elementsByTagName("item");
	int num = nodeList.count();
	if (num > 0)
	{
		QPoint pinPos;
		QDomElement itemElement;
		for (int i = 0; i < num; ++i)
		{
			itemElement = nodeList.at(i).toElement();

			pinPos.setX(itemElement.attribute("x").toInt());
			pinPos.setY(itemElement.attribute("y").toInt());

			pinPosList.append(pinPos);
		}
	}
	return pinPosList;
}

QList<ITips> KInitICMap::createTipsList(
	const QDomElement& element)
{
	QList<ITips> tipsList;
	if (element.isNull())
		return tipsList;
	QDomNodeList nodeList = element.elementsByTagName("item");
	int num = nodeList.count();
	if (num > 0)
	{
		ITips tips;
		QDomElement itemElement;
		for (int i = 0; i < num; ++i)
		{
			itemElement = nodeList.at(i).toElement();

			tips.x = (qreal)itemElement.attribute("x").toDouble();
			tips.y = (qreal)itemElement.attribute("y").toDouble();
			tips.text = itemElement.attribute("text");

			tipsList.append(tips);
		}
	}
	return tipsList;
}

KUniversalIC* KInitICMap::createUniversalIC(const QString& ICName,
	const QPainterPath& path, /* = QPainterPath()*/
	const QList<QPoint>& pinPosList, /*= QList<QPoint>()*/
	const QList<ITips>& tipsList /*= QList<ITips>()*/)
{
	QDomElement element = getElementById(m_elementsDoc, ICName);

	int nInPinNum = element.attribute("inPinNum").toInt();
	int nOutPinNum = element.attribute("outPinNum").toInt();
	int nPinNum = element.attribute("pinNum").toInt();
	QString& description = element.attribute("description");
	QList<KBase*> componentList = createComponentList(
		element.firstChildElement("componentList"));
	QList<KUniversalIC::IInToIn> inToInList = createInToInList(
		element.firstChildElement("inToInList"));
	QList<KUniversalIC::IOutToOut> outToOutList = createOutToOutList(
		element.firstChildElement("outToOutList"));
	QList<KUniversalIC::IOutToIn> outToInList = createOutToInList(
		element.firstChildElement("outToInList"));
	return new KUniversalIC(nInPinNum, nOutPinNum, nPinNum, ICName, 
		description, componentList, inToInList, outToOutList, 
		outToInList, path, pinPosList, tipsList);
}

QList<KBase*> KInitICMap::createComponentList(
	const QDomElement& element)
{
	QList<KBase*> componentList;
	if (element.isNull())
		return componentList;

	QDomNodeList nodeList = element.childNodes();
	for (int i = 0; i < nodeList.count(); ++i)
	{
		componentList.append(createComponent(
			nodeList.at(i).toElement().attribute("id")));
	}
	return componentList;
}

KBase* KInitICMap::createComponent(const QString& ICName)
{
	KBase* pIC = NULL;
	if (ICName ==  "74LS04")//NOT
		 pIC = new K74LS04();
	else if (ICName == "74LS08")//AND
		pIC = new K74LS08();
	else if (ICName == "74LS32")//OR
		pIC = new K74LS32();
	else if (ICName == "74LS125")//ÈýÌ¬
		pIC = new K74LS125();
	else if (ICName == "SRAM2114")
		pIC = new KSRam2114();
	else if (ICName == "SRAM6116")
		pIC = new KSRam6116();
	else if (ICName == "SRAM6264")
		pIC = new KSRam6264();
	else if (ICName == "SRAM62256")
		pIC = new KSRam62256();
	else if (ICName == "SRAM62128")
		pIC = new KSRam62128();
	else if (ICName == "SRAM628128")
		pIC = new KSRam628128();
	else if (ICName == "Vcc")
		pIC = new KVcc();
	else if (ICName == "LED")
		pIC = new KLED();
	else if (ICName == "SWITCH")
		pIC = new KSWITCH();
	else if (ICName == "CLOCK")
		pIC = new KCLOCK();
	else
		pIC = createUniversalIC(ICName);

	return pIC;
}

QList<KUniversalIC::IInToIn> KInitICMap::createInToInList(
	const QDomElement& element)
{
	QList<KUniversalIC::IInToIn> inToInList;
	if (element.isNull())
		return inToInList;

	QDomNodeList nodeList = element.elementsByTagName("item");
	int num = nodeList.count();
	if (num > 0)
	{
		KUniversalIC::IInToIn inToIn;
		QDomElement itemElement;
		for (int i = 0; i < num; ++i)
		{
			itemElement = nodeList.at(i).toElement();

			inToIn.index = itemElement.attribute("index").toInt();
			inToIn.target = itemElement.attribute("target").toInt();
			inToIn.targetIndex = itemElement.attribute("targetIndex").toInt();

			inToInList.append(inToIn);
		}
	}
	return inToInList;
}

QList<KUniversalIC::IOutToOut> KInitICMap::createOutToOutList(
	const QDomElement& element)
{
	QList<KUniversalIC::IOutToOut> outToOutList;
	if (element.isNull())
		return outToOutList;
	QDomNodeList nodeList = element.elementsByTagName("item");
	int num = nodeList.count();
	if (num > 0)
	{
		KUniversalIC::IOutToOut outToOut;
		QDomElement itemElement;
		for (int i = 0; i < num; ++i)
		{
			itemElement = nodeList.at(i).toElement();

			outToOut.source = itemElement.attribute("source").toInt();
			outToOut.sourceIndex = itemElement.attribute("sourceIndex").toInt();
			outToOut.index = itemElement.attribute("index").toInt();

			outToOutList.append(outToOut);
		}
	}
	return outToOutList;
}

QList<KUniversalIC::IOutToIn> KInitICMap::createOutToInList(
	const QDomElement& element)
{
	QList<KUniversalIC::IOutToIn> outToInList;
	if (element.isNull())
		return outToInList;
	QDomNodeList nodeList = element.elementsByTagName("item");
	int num = nodeList.count();
	if (num > 0)
	{
		QDomElement itemElement;
		KUniversalIC::IOutToIn outToIn;
		for (int i = 0; i < num; ++i)
		{
			itemElement = nodeList.at(i).toElement();

			outToIn.source = itemElement.attribute("source").toInt();
			outToIn.sourceIndex = itemElement.attribute("sourceIndex").toInt();
			outToIn.target = itemElement.attribute("target").toInt();
			outToIn.targetIndex = itemElement.attribute("targetIndex").toInt();

			outToInList.append(outToIn);
		}
	}
	return outToInList;
}
