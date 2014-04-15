#ifndef _GLOBALDEF_H_
#define _GLOBALDEF_H_

#if defined(_DEBUG)||defined(DEBUG)
#pragma comment( linker, "/subsystem:console /entry:mainCRTStartup" )  
#endif

//begin: include
#include <iostream>
#include <assert.h>
#include <QCursor>
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
const QString IMAGESPATH ="images/";

class KBase;

//��KInitICMap�︳ֵ
extern QMap<QString, KBase*> g_ICMap;
//��Kui��init()�︳������ֵ
extern QCursor g_PinCursor;
#endif