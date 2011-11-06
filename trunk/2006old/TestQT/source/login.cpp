#include "login.h"

LoginDialog::LoginDialog()
{
	setupUi(this);
	connect(awayButton, SIGNAL(clicked()),qApp, SLOT(quit()));
	connect(enterButton, SIGNAL(clicked()), this, SLOT(enterSlot()));
}

void LoginDialog::enterSlot()
{
	
	if (lineEdit_2->text().isEmpty())
	{
		QMessageBox mess;
		mess.setFont(QFont("Sans Serif", 12, 50));
		mess.warning(this,
				     QString::fromLocal8Bit("����"),
				     QString::fromLocal8Bit("<font size=5>��������ȷ���ǳ�,лл!</font>"));
	
		return;
	}
	emit sendEnterMessage(lineEdit_2->text(), lineEdit_3->text());
	emit showChatWindow();
}

void LoginDialog::closeEvent(QCloseEvent *)
{
	qApp->quit();
}
