#ifndef _GLOBALDEF_H_
#define _GLOBALDEF_H_

#if defined(_DEBUG)||defined(DEBUG)
#pragma comment( linker, "/subsystem:console /entry:mainCRTStartup" )  
#endif

//begin: include
#include <iostream>
#include <assert.h>
#include <QMap>
#include <QString>
//end: include

#ifndef PURE
#define PURE =0
#endif

#ifndef NULL
#define NULL 0
#endif

const QString PATH = "ics/";
const QString LISTFILENAME = "list";
const QString ICSLISTNAME = "ics";
const QString SHELLSFILENAME = "shells";

class KBase;
extern QMap<QString, KBase*> g_ICMap;

#endif