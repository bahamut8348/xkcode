#include "chat.h"

Chat::Chat()
{
    serverSocket = NULL;
    server = NULL;
    client = NULL;
    
	setupUi(this);

	login = new LoginDialog;
	login->show();
	connect(login, SIGNAL(showChatWindow()), this, SLOT(showAndHideSlot()));
	
	connect(login, SIGNAL(sendEnterMessage(QString , QString)),
    this,SLOT(enterSlot(QString , QString )));
	
	connect(writeMessageEdit, SIGNAL(textChanged()), this, SLOT(changeButtonStateSlot()));
		
	connect(aboutButton, SIGNAL(clicked()), this, SLOT(createAboutSlot()));
	
	connect(sendButton, SIGNAL(clicked()), this, SLOT(appendMessageSlot()));
	
	connect(quitButton, SIGNAL(clicked()), qApp, SLOT(quit()));
}

void Chat::showAndHideSlot()
{
	delete login;
	this->show();
}

void Chat::createAboutSlot()
{}

void Chat::enterSlot(QString name, QString host)
{
     port = 22222;
     
     if (host.isEmpty())
     {
         socketServer();
     }
     else
     {
         socketClient(host);
     }
     
	 userName = name;
	 onlineMessageList->addItem(name);
}

void Chat::changeButtonStateSlot()
{
	bool boo_dis = writeMessageEdit->toPlainText().isEmpty();
	sendButton->setDisabled(boo_dis);
}

void Chat::appendMessageSlot()
{
	QString content = writeMessageEdit->toPlainText();
	if (content.isEmpty())
	{
		QMessageBox::warning(this, "出错", QString::fromLocal8Bit("发送的内容不能为空"));
		return;
	}
	nowDateTime = QDateTime::currentDateTime();
	showMessageEdit->append(
			QString("\n[%1] %2 %3\n%4")
			.arg(nowDateTime.toString("yyyy-MM-dd  hh:mm:ss"))
			.arg(userName)
			.arg(QString::fromLocal8Bit("说道:"))
			.arg(content) );
	
	QDataStream out;
	if (serverBool)
	{
		out.setDevice(serverSocket);
    	}
    	else if (!serverBool)
    	{
		out.setDevice(client);
	}
	int mark = 0;
	out <<  mark;
	out << userName << content;
  	writeMessageEdit->clear();
}

void Chat::socketServer()
{
     serverBool = true;
     server = new QTcpServer(this);
     serverSocket = new QTcpSocket(this);
     server->listen(QHostAddress::Any, port);
     connect(server, SIGNAL(newConnection()), this, SLOT(newConnectionSlot()));
}

void Chat::newConnectionSlot()
{
     serverSocket = server->nextPendingConnection();
     connect(serverSocket, SIGNAL(readyRead()), this, SLOT(newDataSlot()));
     connect(serverSocket, SIGNAL(disconnected()), this, SLOT(deleNameSlot()));
}

void Chat::socketClient(QString host)
{
     serverBool = false;
     client = new QTcpSocket(this);
     client->connectToHost(host, port);
     connect(client, SIGNAL(connected()), this, SLOT(addSlot()));
     connect(client, SIGNAL(readyRead()), this, SLOT(newDataSlot()));
     connect(client, SIGNAL(disconnected()), this, SLOT(deleNameSlot()));
}

void Chat::addSlot()
{
     QString myName = userName;
     QString content;
     QDataStream out(client);
     int mark = 11;
     out << mark;
     out << myName << content;
}


void Chat::newDataSlot()
{
     int mark;
     QString hisName;
     QString content;
     QDataStream in;
     if (serverBool)
     {
	      in.setDevice(serverSocket);
     }
     else if (!serverBool)
     {
	     in.setDevice(client);
     }
     in >> mark;
     
     
     // 数据解析
     if (mark == 11)  //addList
     {
	     in >> hisName >> content;
	     if (content.isEmpty())
	     {
		     onlineMessageList->addItem(hisName);
		     if (serverBool)
		     {
			     QString content;
			     QDataStream out(serverSocket);
			     int mark = 11;
			     out << mark;
			     out << userName<< content;
		     }
	     }
	     return;
     }

     else if (mark == 0)   //message
     {
	     in >> hisName >> content;
              nowDateTime = QDateTime::currentDateTime();
	     showMessageEdit->append(
			QString("\n[%1] %2 %3\n%4")
			.arg(nowDateTime.toString("yyyy-MM-dd  hh:mm:ss"))
			.arg(hisName)
			.arg(QString::fromLocal8Bit("说道:"))
			.arg(content) );
	     return;
     }
}

void Chat::deleNameSlot()
{

      onlineMessageList->takeItem(1);
}
     
     
void Chat::keyPressEvent(QKeyEvent *event) //ctrl+s
{
	if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_S)
	{
		appendMessageSlot();
	}
	else
	{
		event->ignore();
	}
}

void Chat::closeEvent(QCloseEvent *event) //close application event
{
     if (server != NULL)
     {
          server->close();
     }
     if (client != NULL)
     {
          client->close();
     }
     if (serverSocket != NULL)
     {
          serverSocket->close();
     }
     event->accept();
}

Chat::~Chat()
{
     //free stack memory
}
