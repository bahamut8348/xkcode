#ifndef UNTITLED1_H
#define UNTITLED1_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

class Ui_Form1
{
	public:
		QPushButton *aboutButton;
		QPushButton *sendButton;
		QPushButton *quitButton;
		QTextEdit *writeMessageEdit;
		QTextEdit *showMessageEdit;
		QListWidget *onlineMessageList;

		void setupUi(QWidget *Form)
		{
			Form->setObjectName(QString::fromUtf8("Form"));
			Form->resize(QSize(513, 345).expandedTo(Form->minimumSizeHint()));
			Form->setMinimumSize(QSize(513, 345));
			Form->setMaximumSize(QSize(513, 345));
			aboutButton = new QPushButton(Form);
			aboutButton->setObjectName(QString::fromUtf8("aboutButton"));
			aboutButton->setGeometry(QRect(20, 310, 87, 26));
			QFont font;
			font.setFamily(QString::fromUtf8("SimSun"));
			font.setPointSize(10);
			font.setBold(false);
			font.setItalic(false);
			font.setUnderline(false);
			font.setWeight(50);
			font.setStrikeOut(false);
			aboutButton->setFont(font);
			sendButton = new QPushButton(Form);
			sendButton->setObjectName(QString::fromUtf8("sendButton"));
			sendButton->setGeometry(QRect(290, 310, 101, 26));
			QFont font1;
			font1.setFamily(QString::fromUtf8("SimSun"));
			font1.setPointSize(10);
			font1.setBold(false);
			font1.setItalic(false);
			font1.setUnderline(false);
			font1.setWeight(50);
			font1.setStrikeOut(false);
			sendButton->setFont(font1);
			sendButton->setDisabled(true);
			quitButton = new QPushButton(Form);
			quitButton->setObjectName(QString::fromUtf8("quitButton"));
			quitButton->setGeometry(QRect(410, 310, 81, 26));
			QFont font2;
			font2.setFamily(QString::fromUtf8("SimSun"));
			font2.setPointSize(10);
			font2.setBold(false);
			font2.setItalic(false);
			font2.setUnderline(false);
			font2.setWeight(50);
			font2.setStrikeOut(false);
			quitButton->setFont(font2);
			writeMessageEdit = new QTextEdit(Form);
			writeMessageEdit->setObjectName(QString::fromUtf8("writeMessageEdit"));
			writeMessageEdit->setGeometry(QRect(20, 230, 371, 71));
			QFont font3;
			font3.setFamily(QString::fromUtf8("SimSun"));
			font3.setPointSize(13);
			font3.setBold(false);
			font3.setItalic(false);
			font3.setUnderline(false);
			font3.setWeight(50);
			font3.setStrikeOut(false);
			writeMessageEdit->setFont(font3);
			showMessageEdit = new QTextEdit(Form);
			showMessageEdit->setObjectName(QString::fromUtf8("showMessageEdit"));
			showMessageEdit->setGeometry(QRect(20, 20, 371, 191));
			QFont font4;
			font4.setFamily(QString::fromUtf8("SimSun"));
			font4.setPointSize(12);
			font4.setBold(false);
			font4.setItalic(false);
			font4.setUnderline(false);
			font4.setWeight(50);
			font4.setStrikeOut(false);
			showMessageEdit->setFont(font4);
			showMessageEdit->setFocusPolicy(Qt::NoFocus);
			showMessageEdit->setReadOnly(true);
			onlineMessageList = new QListWidget(Form);
			onlineMessageList->setObjectName(QString::fromUtf8("onlineMessageList"));
			onlineMessageList->setGeometry(QRect(410, 20, 81, 281));
			QFont font5;
			font5.setFamily(QString::fromUtf8("SimSun"));
			font5.setPointSize(12);
			font5.setBold(false);
			font5.setItalic(false);
			font5.setUnderline(false);
			font5.setWeight(50);
			font5.setStrikeOut(false);
			onlineMessageList->setFont(font5);
			retranslateUi(Form);
			Form->setWindowTitle("Chat v0.1");
			QMetaObject::connectSlotsByName(Form);
		} // setupUi

		void retranslateUi(QWidget *Form)
		{
			Form->setWindowTitle(QApplication::translate("Form", "Form", 0, QApplication::UnicodeUTF8));
			aboutButton->setText(QApplication::translate("Form", "&\345\205\263\344\272\216", 0, QApplication::UnicodeUTF8));
			sendButton->setText(QApplication::translate("Form", "\345\217\221\351\200\201(CTRL+S)", 0, QApplication::UnicodeUTF8));
			quitButton->setText(QApplication::translate("Form", "\351\200\200\345\207\272", 0, QApplication::UnicodeUTF8));
			Q_UNUSED(Form);
		} // retranslateUi

};

namespace Ui1{
	class Form: public Ui_Form1 {};
} // namespace Ui

#endif // UNTITLED_H
