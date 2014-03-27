#include <QFile>
#include <QFont>
#include "init.h"

void _Init()
{
	KInitElementMap initEleMap;
	initEleMap.init();
}

KInitElementMap::KInitElementMap()
{
}

KInitElementMap::~KInitElementMap()
{
}

void KInitElementMap::init()
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

QDomElement KInitElementMap::getElementById(QDomDocument& doc, 
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

KBase* KInitElementMap::createIC(const QString& ICName)
{	
	QDomElement element = getElementById(m_shellsDoc, ICName);
	QPainterPath path = createPath(element.firstChildElement("path"));
	QList<IPinPos> pinPosList = createPinPosList(
		element.firstChildElement("pinPosList"));

	KBase* pIC = NULL;

	if (ICName == "74LS08")//AND
		pIC = new K74LS08(path, pinPosList);
	else if (ICName == "74LS11")//AND_3
		pIC = new K74LS11(path, pinPosList);
	else if (ICName == "74LS21")//AND_4
		pIC = createUniversalIC(ICName, path, pinPosList);
		//pIC = new K74LS21(path, pinPosList);
	else if (ICName == "74LS32")//OR
		pIC = new K74LS32(path, pinPosList);
	else if (ICName ==  "74LS04")//NOT
		pIC = new K74LS04(path, pinPosList);
	else if (ICName == "74LS00")//NAND
		pIC = new K74LS00(path, pinPosList);
	else if (ICName == "74LS10")//NAND_3
		pIC = new K74LS10(path, pinPosList);
	else if (ICName == "74LS20")//NAND_4
		pIC = new K74LS20(path, pinPosList);
	else if (ICName == "74LS02")//NOR
		pIC = new K74LS02(path, pinPosList);
	else if (ICName == "74LS27")//NOR_3
		pIC = new K74LS27(path, pinPosList);
	else if (ICName == "CD4002")//NOR_4
		pIC = new KCD4002(path, pinPosList);
	else if (ICName ==  "74LS86")//XOR
		pIC = new K74LS86(path, pinPosList);
	else if (ICName == "LED")
		pIC = new KLED(path, pinPosList);
	else if (ICName == "POWER")
		pIC = new KPower(path, pinPosList);
	else
		pIC = createUniversalIC(ICName, path, pinPosList);
	
	return pIC;
}

QPainterPath KInitElementMap::createPath(const QDomElement& element)
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

QList<IPinPos> KInitElementMap::createPinPosList(
	const QDomElement& element)
{
	QList<IPinPos> pinPosList;
	if (element.isNull())
		return pinPosList;
	QDomNodeList nodeList = element.elementsByTagName("item");
	int num = nodeList.count();
	if (num > 0)
	{
		IPinPos pinPos;
		QDomElement itemElement;
		for (int i = 0; i < num; ++i)
		{
			itemElement = nodeList.at(i).toElement();

			pinPos.index = itemElement.attribute("index").toInt();
			pinPos.x = (qreal)itemElement.attribute("x").toDouble();
			pinPos.y = (qreal)itemElement.attribute("y").toDouble();

			pinPosList.append(pinPos);
		}
	}
	return pinPosList;
}

KUniversalIC* KInitElementMap::createUniversalIC(const QString& ICName,
	const QPainterPath& path, /* = QPainterPath()*/
	const QList<IPinPos>& pinPosList /*= QList<IPinPos>()*/)
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
		outToInList, path, pinPosList);
}

QList<KBase*> KInitElementMap::createComponentList(
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

KBase* KInitElementMap::createComponent(const QString& ICName)
{
	KBase* pIC = NULL;
	if (ICName == "74LS08")//AND
		pIC = new K74LS08();
	else if (ICName == "74LS11")//AND_3
		pIC = new K74LS11();
	else if (ICName == "74LS21")//AND_4
		pIC = new K74LS21();
	else if (ICName == "74LS32")//OR
		pIC = new K74LS32();
	else if (ICName ==  "74LS04")//NOT
		pIC = new K74LS04();
	else if (ICName == "74LS00")//NAND
		pIC = new K74LS00();
	else if (ICName == "74LS10")//NAND_3
		pIC = new K74LS10();
	else if (ICName == "74LS20")//NAND_4
		pIC = new K74LS20();
	else if (ICName == "74LS02")//NOR
		pIC = new K74LS02();
	else if (ICName == "74LS27")//NOR_3
		pIC = new K74LS27();
	else if (ICName == "CD4002")//NOR_4
		pIC = new KCD4002();
	else if (ICName ==  "74LS86")//XOR
		pIC = new K74LS86();
	else if (ICName == "LED")
		pIC = new KLED();
	else if (ICName == "POWER")
		pIC = new KPower();
	else
		pIC = createUniversalIC(ICName);

	return pIC;
}

QList<KUniversalIC::IInToIn> KInitElementMap::createInToInList(
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

QList<KUniversalIC::IOutToOut> KInitElementMap::createOutToOutList(
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

QList<KUniversalIC::IOutToIn> KInitElementMap::createOutToInList(
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
