#include <QApplication>
#include <QDesktopWidget>

#include <QTextCodec>
#include "ui.h"
#include "window.h"
#include "init.h"

int main(int argc, char* argv[])
{
	QTextCodec *codec = QTextCodec::codecForName("gb2312");
	QTextCodec::setCodecForTr(codec);
	QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
	QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());

	QApplication app(argc, argv);

	_Init();

// 	KWindow win;
// 	win.show();

	KUi ui;
	ui.resize(app.desktop()->width() * 2 / 3 , app.desktop()->height() * 2 / 3);
	ui.show();

	return app.exec();
}