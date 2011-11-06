#ifndef FORM_H
#define FORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

class Ui_Form
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *hboxLayout;
    QLineEdit *lineEdit_a;
    QComboBox *comboBox;
    QLineEdit *lineEdit_b;
    QLabel *label_eauql;
    QLineEdit *lineEdit_c;
    QWidget *layoutWidget1;
    QHBoxLayout *hboxLayout1;
    QPushButton *pushButton_ok;
    QPushButton *pushButton_close;
    QLabel *label_compute;

    void setupUi(QWidget *Form)
    {
    Form->setObjectName(QString::fromUtf8("Form"));
    Form->resize(QSize(370, 139).expandedTo(Form->minimumSizeHint()));
    layoutWidget = new QWidget(Form);
    layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
    layoutWidget->setGeometry(QRect(10, 50, 351, 26));
    hboxLayout = new QHBoxLayout(layoutWidget);
    hboxLayout->setSpacing(6);
    hboxLayout->setMargin(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    lineEdit_a = new QLineEdit(layoutWidget);
    lineEdit_a->setObjectName(QString::fromUtf8("lineEdit_a"));

    hboxLayout->addWidget(lineEdit_a);

    comboBox = new QComboBox(layoutWidget);
    comboBox->setObjectName(QString::fromUtf8("comboBox"));

    hboxLayout->addWidget(comboBox);

    lineEdit_b = new QLineEdit(layoutWidget);
    lineEdit_b->setObjectName(QString::fromUtf8("lineEdit_b"));

    hboxLayout->addWidget(lineEdit_b);

    label_eauql = new QLabel(layoutWidget);
    label_eauql->setObjectName(QString::fromUtf8("label_eauql"));

    hboxLayout->addWidget(label_eauql);

    lineEdit_c = new QLineEdit(layoutWidget);
    lineEdit_c->setObjectName(QString::fromUtf8("lineEdit_c"));
    lineEdit_c->setReadOnly(true);

    hboxLayout->addWidget(lineEdit_c);

    layoutWidget1 = new QWidget(Form);
    layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
    layoutWidget1->setGeometry(QRect(100, 100, 182, 30));
    hboxLayout1 = new QHBoxLayout(layoutWidget1);
    hboxLayout1->setSpacing(6);
    hboxLayout1->setMargin(0);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    pushButton_ok = new QPushButton(layoutWidget1);
    pushButton_ok->setObjectName(QString::fromUtf8("pushButton_ok"));

    hboxLayout1->addWidget(pushButton_ok);

    pushButton_close = new QPushButton(layoutWidget1);
    pushButton_close->setObjectName(QString::fromUtf8("pushButton_close"));

    hboxLayout1->addWidget(pushButton_close);

    label_compute = new QLabel(Form);
    label_compute->setObjectName(QString::fromUtf8("label_compute"));
    label_compute->setGeometry(QRect(10, 20, 71, 16));
    retranslateUi(Form);
    QObject::connect(pushButton_close, SIGNAL(clicked()), Form, SLOT(close()));

    QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
    Form->setWindowTitle(QApplication::translate("Form", "Calculator", 0, QApplication::UnicodeUTF8));
    lineEdit_a->setInputMask(QApplication::translate("Form", "", 0, QApplication::UnicodeUTF8));
    lineEdit_a->setText(QApplication::translate("Form", "0", 0, QApplication::UnicodeUTF8));
    comboBox->clear();
    comboBox->addItem(QApplication::translate("Form", "+", 0, QApplication::UnicodeUTF8));
    comboBox->addItem(QApplication::translate("Form", "-", 0, QApplication::UnicodeUTF8));
    comboBox->addItem(QApplication::translate("Form", "*", 0, QApplication::UnicodeUTF8));
    comboBox->addItem(QApplication::translate("Form", "/", 0, QApplication::UnicodeUTF8));
    lineEdit_b->setInputMask(QApplication::translate("Form", "", 0, QApplication::UnicodeUTF8));
    lineEdit_b->setText(QApplication::translate("Form", "0", 0, QApplication::UnicodeUTF8));
    label_eauql->setText(QApplication::translate("Form", "\357\274\235", 0, QApplication::UnicodeUTF8));
    lineEdit_c->setText(QApplication::translate("Form", "0", 0, QApplication::UnicodeUTF8));
    pushButton_ok->setText(QApplication::translate("Form", "Ok", 0, QApplication::UnicodeUTF8));
    pushButton_close->setText(QApplication::translate("Form", "Close", 0, QApplication::UnicodeUTF8));
    label_compute->setText(QApplication::translate("Form", "Compute:", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Form);
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

#endif // FORM_H
