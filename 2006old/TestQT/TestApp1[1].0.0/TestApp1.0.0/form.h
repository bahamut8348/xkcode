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
#include <QtGui/QMainWindow>
#include <QDoubleValidator>
#include <QDebug>

class Form:public QMainWindow
{
  Q_OBJECT
  public:
    Form();
    void retranslateUi();
public slots:
    void compute();
public:
	QWidget *widget;
	QHBoxLayout *hboxLayout;
	QLineEdit *lineEdit_a;
	QComboBox *comboBox;
	QLineEdit *lineEdit_b;
	QLabel *label_eauql;
	QLineEdit *lineEdit_c;
	QWidget *layoutWidget;
	QHBoxLayout *hboxLayout1;
	QPushButton *pushButton_ok;
	QPushButton *pushButton_close;
	QLabel *label_compute;
        QDoubleValidator *validator;
};
#endif // FORM_H

