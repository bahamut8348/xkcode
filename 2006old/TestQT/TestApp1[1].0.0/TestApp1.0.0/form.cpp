#include "form.h"

Form::Form()
{
	this->setObjectName(QString::fromUtf8("Form"));
	this->resize(QSize(370, 139).expandedTo(this->minimumSizeHint()));
	validator = new QDoubleValidator(this);
	
	widget = new QWidget(this);
	widget->setObjectName(QString::fromUtf8("widget"));
	widget->setGeometry(QRect(10, 50, 351, 26));
	hboxLayout = new QHBoxLayout(widget);
	hboxLayout->setSpacing(6);
	hboxLayout->setMargin(0);
	hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
	lineEdit_a = new QLineEdit(widget);
	lineEdit_a->setObjectName(QString::fromUtf8("lineEdit_a"));
	lineEdit_a->setValidator(validator);

	hboxLayout->addWidget(lineEdit_a);

	comboBox = new QComboBox(widget);
	comboBox->setObjectName(QString::fromUtf8("comboBox"));

	hboxLayout->addWidget(comboBox);
	lineEdit_b = new QLineEdit(widget);
	lineEdit_b->setObjectName(QString::fromUtf8("lineEdit_b"));
	lineEdit_b->setValidator(validator);

	hboxLayout->addWidget(lineEdit_b);

	label_eauql = new QLabel(widget);
	label_eauql->setObjectName(QString::fromUtf8("label_eauql"));

	hboxLayout->addWidget(label_eauql);

	lineEdit_c = new QLineEdit(widget);
	lineEdit_c->setObjectName(QString::fromUtf8("lineEdit_c"));
	lineEdit_c->setReadOnly(true);
	
	hboxLayout->addWidget(lineEdit_c);

	layoutWidget = new QWidget(this);
	layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
	layoutWidget->setGeometry(QRect(100, 100, 182, 30));
	hboxLayout1 = new QHBoxLayout(layoutWidget);
	hboxLayout1->setSpacing(6);
	hboxLayout1->setMargin(0);
	hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
	pushButton_ok = new QPushButton(layoutWidget);
	pushButton_ok->setObjectName(QString::fromUtf8("pushButton_ok"));

	hboxLayout1->addWidget(pushButton_ok);

	pushButton_close = new QPushButton(layoutWidget);
	pushButton_close->setObjectName(QString::fromUtf8("pushButton_close"));

	hboxLayout1->addWidget(pushButton_close);

	label_compute = new QLabel(this);
	label_compute->setObjectName(QString::fromUtf8("label_compute"));
	label_compute->setGeometry(QRect(10, 20, 71, 16));
	retranslateUi();
	QObject::connect(pushButton_close, SIGNAL(clicked()), this, SLOT(close()));
	QObject::connect(pushButton_ok, SIGNAL(clicked()), this, SLOT(compute()));

	QMetaObject::connectSlotsByName(this);

} // setupUi

void Form::retranslateUi()
{
	this->setWindowTitle(QApplication::translate("Form", "Calculator", 0, QApplication::UnicodeUTF8));
	lineEdit_a->setText(QApplication::translate("Form", "0", 0, QApplication::UnicodeUTF8));
	comboBox->clear();
	comboBox->addItem(QApplication::translate("Form", "+", 0, QApplication::UnicodeUTF8));
	comboBox->addItem(QApplication::translate("Form", "-", 0, QApplication::UnicodeUTF8));
	comboBox->addItem(QApplication::translate("Form", "*", 0, QApplication::UnicodeUTF8));
	comboBox->addItem(QApplication::translate("Form", "/", 0, QApplication::UnicodeUTF8));
	lineEdit_b->setText(QApplication::translate("Form", "0", 0, QApplication::UnicodeUTF8));
	label_eauql->setText(QApplication::translate("Form", "\357\274\235", 0, QApplication::UnicodeUTF8));
	lineEdit_c->setText(QApplication::translate("Form", "0", 0, QApplication::UnicodeUTF8));
	pushButton_ok->setText(QApplication::translate("Form", "Ok", 0, QApplication::UnicodeUTF8));
	pushButton_close->setText(QApplication::translate("Form", "Close", 0, QApplication::UnicodeUTF8));
	label_compute->setText(QApplication::translate("Form", "Input:", 0, QApplication::UnicodeUTF8));
	Q_UNUSED(this);
} // retranslateUi


void Form::compute()
{
	int opt;
	double a,b,c;
	QString str_a,str_b,str_c;
	str_a=lineEdit_a->text();
	a=str_a.toDouble();
	str_b=lineEdit_b->text();
	b=str_b.toDouble();
	
	opt=comboBox->currentIndex();
	switch(opt)
	{
		case 0:c=a+b;break;
		case 1:c=a-b;break;
		case 2:c=a*b;break;
		case 3:c=a/b;break;
		default:qDebug()<<"Error!";
	}
	str_c.setNum(c,'f',3);
	lineEdit_c->setText(str_c);
	//qDebug()<<str_c;
}
