#include <QMap>
#include <QString>
#include "createic.h"

KBase* g_createIC(const QString& name)
{
	return ::g_ICMap[name]->clone();
}