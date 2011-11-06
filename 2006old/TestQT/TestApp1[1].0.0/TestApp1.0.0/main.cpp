#include "form.h"
#include <QtGui/QtGui>

int main(int argc,char *argv[])
{
	QApplication app(argc,argv);

	Form *form=new Form;
	form->show();
	return app.exec();
	
}
