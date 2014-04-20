#ifndef _INIT_H_
#define _INIT_H_

#include "globaldef.h"
#include "base.h"
#include "sram.h"
#include "ics.h"

#include <QtXml/QtXml>

class QDomDocument;
class QDomElement;
class KShell;

extern void _Init();

typedef KBase::ITips ITips;

class KInitICMap
{
public:
	KInitICMap();
	~KInitICMap();

	void init();

private:
	//自作、局限的
	QDomElement getElementById(QDomDocument& doc, const QString& id);

	KBase* createIC(const QString& ICName);
	KUniversalIC* createUniversalIC(const QString& ICName,
		const QPainterPath& path  = QPainterPath(),
		const QList<QPoint>& pinPosList  = QList<QPoint>(),
		const QList<ITips>& tipsList = QList<ITips>());

	QPainterPath createPath(const QDomElement& element);
	QList<QPoint> createPinPosList(const QDomElement& element);
	QList<ITips> createTipsList(const QDomElement& element);

	QList<KBase*> createComponentList(const QDomElement& element);
	KBase* createComponent(const QString& name);
	QList<KUniversalIC::IInToIn> createInToInList(const QDomElement& element);
	QList<KUniversalIC::IOutToOut> createOutToOutList(const QDomElement& element);
	QList<KUniversalIC::IOutToIn> createOutToInList(const QDomElement& element);
private:
	QDomDocument m_listDoc;
	QDomDocument m_elementsDoc;
	QDomDocument m_shellsDoc;
};

#endif