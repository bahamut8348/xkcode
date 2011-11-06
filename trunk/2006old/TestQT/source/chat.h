#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include <QMessageBox>
#include <QString>
#include <QDateTime>
#include <QFileDialog>
#include <QKeyEvent>
#include <QCloseEvent>
#include <QByteArray>
#include <QDataStream>
#include <QTextStream>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>


#include "window_ui.h"
#include "login.h"

class Chat:public QWidget, public Ui1::Form
{
	Q_OBJECT
	public:
		Chat();
		~Chat();
		void socketServer();
		void socketClient(QString host);
		
	protected:
		virtual void keyPressEvent(QKeyEvent *);
		//virtual bool eventFilter(QObject *, QEvent *);
		virtual void closeEvent(QCloseEvent *);
		
	private:
		LoginDialog *login;
        
		QString message;
		QString userName;
		QDateTime nowDateTime;
		
		quint16 port;
		QTcpServer *server;
		QTcpSocket *client;
		QTcpSocket *serverSocket;
		bool serverBool;
		
	public slots:
		void showAndHideSlot();
		void enterSlot(QString , QString);
		void changeButtonStateSlot();
		void appendMessageSlot();
		void createAboutSlot();
		
		void newConnectionSlot();
		void newDataSlot();
		void addSlot();
		void deleNameSlot();
};

#endif
