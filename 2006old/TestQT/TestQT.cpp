// TestQT.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include "test.cpp"

int _tmain(int argc, _TCHAR* argv[])
{

	QApplication app(argc, argv);
	QMainWindow *mainw = new QMainWindow;
	Ui::MainWindow *ui = new Ui::MainWindow();
	ui->setupUi(mainw);
	mainw->show();
	return app.exec();
	



}

