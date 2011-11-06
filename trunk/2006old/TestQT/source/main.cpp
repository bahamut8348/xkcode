#include <QApplication>
#include <QTextCodec>

#include "chat.h"

int main( int argc, char ** argv )
{
	QApplication app(argc, argv);
	QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
	Chat chat;
	
	return app.exec();
}
