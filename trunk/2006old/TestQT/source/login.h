#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QFont>
#include <QMessageBox>
#include <QCloseEvent>
#include <QApplication>

#include "login_ui.h"

class LoginDialog:public QWidget, public Ui2::Form
{
	Q_OBJECT
	public:
		LoginDialog();
		
	protected:
        virtual void closeEvent(QCloseEvent *);
		
	public slots:
		void enterSlot();
	
	signals:
		void sendEnterMessage(QString , QString );
		void showChatWindow();
};

#endif
