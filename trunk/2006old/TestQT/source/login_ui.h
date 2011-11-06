#ifndef UNTITLED2_H
#define UNTITLED2_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QWidget>

class Ui_Form2
{
	public:
		QWidget *layoutWidget;
		QHBoxLayout *hboxLayout;
		QPushButton *awayButton;
		QPushButton *enterButton;
		QWidget *layoutWidget1;
		QHBoxLayout *hboxLayout1;
		QLabel *label_2;
		QLineEdit *lineEdit_2;
		QRadioButton *clientButton;
		QRadioButton *serverButton;
		QGroupBox *groupBox;
		QLineEdit *lineEdit_3;

		void setupUi(QWidget *Form)
		{
			Form->setObjectName(QString::fromUtf8("Form"));
			Form->resize(QSize(232, 270).expandedTo(Form->minimumSizeHint()));
			layoutWidget = new QWidget(Form);
			layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
			layoutWidget->setGeometry(QRect(20, 220, 191, 33));
			hboxLayout = new QHBoxLayout(layoutWidget);
			hboxLayout->setSpacing(6);
			hboxLayout->setMargin(0);
			hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
			awayButton = new QPushButton(layoutWidget);
			awayButton->setObjectName(QString::fromUtf8("awayButton"));
			QFont font;
			font.setFamily(QString::fromUtf8("SimSun"));
			font.setPointSize(10);
			font.setBold(false);
			font.setItalic(false);
			font.setUnderline(false);
			font.setWeight(50);
			font.setStrikeOut(false);
			awayButton->setFont(font);

			hboxLayout->addWidget(awayButton);

			enterButton = new QPushButton(layoutWidget);
			enterButton->setObjectName(QString::fromUtf8("enterButton"));
			QFont font1;
			font1.setFamily(QString::fromUtf8("SimSun"));
			font1.setPointSize(10);
			font1.setBold(false);
			font1.setItalic(false);
			font1.setUnderline(false);
			font1.setWeight(50);
			font1.setStrikeOut(false);
			enterButton->setFont(font1);

			hboxLayout->addWidget(enterButton);

			layoutWidget1 = new QWidget(Form);
			layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
			layoutWidget1->setGeometry(QRect(20, 170, 191, 32));
			hboxLayout1 = new QHBoxLayout(layoutWidget1);
			hboxLayout1->setSpacing(6);
			hboxLayout1->setMargin(0);
			hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
			label_2 = new QLabel(layoutWidget1);
			label_2->setObjectName(QString::fromUtf8("label_2"));
			QFont font2;
			font2.setFamily(QString::fromUtf8("SimSun"));
			font2.setPointSize(10);
			font2.setBold(false);
			font2.setItalic(false);
			font2.setUnderline(false);
			font2.setWeight(50);
			font2.setStrikeOut(false);
			label_2->setFont(font2);

			hboxLayout1->addWidget(label_2);

			lineEdit_2 = new QLineEdit(layoutWidget1);
			lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
			QFont font3;
			font3.setFamily(QString::fromUtf8("SimSun"));
			font3.setPointSize(10);
			font3.setBold(false);
			font3.setItalic(false);
			font3.setUnderline(false);
			font3.setWeight(50);
			font3.setStrikeOut(false);
			lineEdit_2->setFont(font3);

			hboxLayout1->addWidget(lineEdit_2);

			clientButton = new QRadioButton(Form);
			clientButton->setObjectName(QString::fromUtf8("clientButton"));
			clientButton->setGeometry(QRect(22, 61, 141, 25));
			QFont font4;
			font4.setFamily(QString::fromUtf8("SimSun"));
			font4.setPointSize(10);
			font4.setBold(false);
			font4.setItalic(false);
			font4.setUnderline(false);
			font4.setWeight(50);
			font4.setStrikeOut(false);
			clientButton->setFont(font4);
			serverButton = new QRadioButton(Form);
			serverButton->setObjectName(QString::fromUtf8("serverButton"));
			serverButton->setGeometry(QRect(22, 33, 141, 22));
			QFont font5;
			font5.setFamily(QString::fromUtf8("SimSun"));
			font5.setPointSize(10);
			font5.setBold(false);
			font5.setItalic(false);
			font5.setUnderline(false);
			font5.setWeight(50);
			font5.setStrikeOut(false);
			serverButton->setFont(font5);
			groupBox = new QGroupBox(Form);
			groupBox->setObjectName(QString::fromUtf8("groupBox"));
			groupBox->setEnabled(false);
			groupBox->setGeometry(QRect(20, 100, 191, 51));
			QFont font6;
			font6.setFamily(QString::fromUtf8("SimSun"));
			font6.setPointSize(9);
			font6.setBold(false);
			font6.setItalic(false);
			font6.setUnderline(false);
			font6.setWeight(50);
			font6.setStrikeOut(false);
			groupBox->setFont(font6);
			lineEdit_3 = new QLineEdit(groupBox);
			lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
			lineEdit_3->setGeometry(QRect(10, 20, 171, 25));
			QFont font7;
			font7.setFamily(QString::fromUtf8("SimSun"));
			font7.setPointSize(10);
			font7.setBold(false);
			font7.setItalic(false);
			font7.setUnderline(false);
			font7.setWeight(50);
			font7.setStrikeOut(false);
			lineEdit_3->setFont(font7);
			retranslateUi(Form);
    //*******diy
			Form->setWindowTitle(QString::fromLocal8Bit("µÇÂ½"));
			lineEdit_2->setFocus();
			serverButton->setChecked(true);
    //**********
			QObject::connect(serverButton, SIGNAL(clicked(bool)), groupBox, SLOT(setDisabled(bool)));
			QObject::connect(clientButton, SIGNAL(clicked(bool)), groupBox, SLOT(setEnabled(bool)));

			QMetaObject::connectSlotsByName(Form);
		} // setupUi

		void retranslateUi(QWidget *Form)
		{
			Form->setWindowTitle(QApplication::translate("Form", "Form", 0, QApplication::UnicodeUTF8));
			awayButton->setText(QApplication::translate("Form", "\347\246\273\345\274\200", 0, QApplication::UnicodeUTF8));
			enterButton->setText(QApplication::translate("Form", "&\350\277\233\345\205\245", 0, QApplication::UnicodeUTF8));
			label_2->setText(QApplication::translate("Form", "\346\230\265\347\247\260:", 0, QApplication::UnicodeUTF8));
			clientButton->setText(QApplication::translate("Form", "\346\234\254\346\234\272\350\277\236\346\216\245\345\256\242\346\210\267\347\253\257", 0, QApplication::UnicodeUTF8));
			serverButton->setText(QApplication::translate("Form", "\346\234\254\346\234\272\344\275\234\344\270\272\346\234\215\345\212\241\347\253\257", 0, QApplication::UnicodeUTF8));
			groupBox->setTitle(QApplication::translate("Form", "\350\257\267\350\276\223\345\205\245\346\202\250\350\246\201\350\277\236\346\216\245\346\234\215\345\212\241\347\253\257\347\232\204IP\345\234\260\345\235\200", 0, QApplication::UnicodeUTF8));
			lineEdit_3->setText(QApplication::translate("Form", "", 0, QApplication::UnicodeUTF8));
			Q_UNUSED(Form);
		} // retranslateUi

};

namespace Ui2 {
	class Form: public Ui_Form2 {};
} // namespace Ui

#endif // UNTITLED_H
