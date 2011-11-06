/********************************************************************************
** Form generated from reading ui file 'test.ui'
**
** Created: Fri May 18 18:15:19 2007
**      by: Qt User Interface Compiler version 4.2.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef TEST_H
#define TEST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

class Ui_MainWindow
{
public:
    QAction *actionQuit;
    QAction *action_M;
    QAction *actionBeatme;
    QWidget *centralwidget;
    QPushButton *btnClickMe;
    QTextEdit *textEdit;
    QMenuBar *menubar;
    QMenu *menu;
    QMenu *menu_A;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
    MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
    QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(7), static_cast<QSizePolicy::Policy>(7));
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
    MainWindow->setSizePolicy(sizePolicy);
    QFont font;
    font.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
    font.setPointSize(9);
    font.setItalic(false);
    font.setUnderline(true);
    MainWindow->setFont(font);
    actionQuit = new QAction(MainWindow);
    actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
    action_M = new QAction(MainWindow);
    action_M->setObjectName(QString::fromUtf8("action_M"));
    actionBeatme = new QAction(MainWindow);
    actionBeatme->setObjectName(QString::fromUtf8("actionBeatme"));
    centralwidget = new QWidget(MainWindow);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    btnClickMe = new QPushButton(centralwidget);
    btnClickMe->setObjectName(QString::fromUtf8("btnClickMe"));
    btnClickMe->setGeometry(QRect(90, 10, 75, 23));
    textEdit = new QTextEdit(centralwidget);
    textEdit->setObjectName(QString::fromUtf8("textEdit"));
    textEdit->setGeometry(QRect(0, 60, 401, 231));
    MainWindow->setCentralWidget(centralwidget);
    menubar = new QMenuBar(MainWindow);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    menubar->setGeometry(QRect(0, 0, 400, 18));
    menu = new QMenu(menubar);
    menu->setObjectName(QString::fromUtf8("menu"));
    menu_A = new QMenu(menubar);
    menu_A->setObjectName(QString::fromUtf8("menu_A"));
    MainWindow->setMenuBar(menubar);
    statusbar = new QStatusBar(MainWindow);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    MainWindow->setStatusBar(statusbar);

    menubar->addAction(menu->menuAction());
    menubar->addAction(menu_A->menuAction());
    menu->addAction(actionQuit);
    menu_A->addAction(action_M);

    retranslateUi(MainWindow);

    QSize size(400, 328);
    size = size.expandedTo(MainWindow->minimumSizeHint());
    MainWindow->resize(size);


    QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
    MainWindow->setWindowTitle(QApplication::translate("MainWindow", "TestQT", 0, QApplication::UnicodeUTF8));
    MainWindow->setToolTip(QApplication::translate("MainWindow", "ttttttttttt", 0, QApplication::UnicodeUTF8));
    actionQuit->setText(QApplication::translate("MainWindow", "\351\200\200\345\207\272(&Q)", 0, QApplication::UnicodeUTF8));
    action_M->setText(QApplication::translate("MainWindow", "\347\202\271\346\210\221(&M)", 0, QApplication::UnicodeUTF8));
    actionBeatme->setText(QApplication::translate("MainWindow", "beatme", 0, QApplication::UnicodeUTF8));
    btnClickMe->setToolTip(QApplication::translate("MainWindow", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'\345\256\213\344\275\223'; font-size:9pt; font-weight:400; font-style:normal; text-decoration:none;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">ffff</span></p></body></html>", 0, QApplication::UnicodeUTF8));
    btnClickMe->setText(QApplication::translate("MainWindow", "\347\202\271\346\210\221", 0, QApplication::UnicodeUTF8));
    menu->setTitle(QApplication::translate("MainWindow", "\346\226\207\344\273\266(&F)", 0, QApplication::UnicodeUTF8));
    menu_A->setTitle(QApplication::translate("MainWindow", "\345\212\250\344\275\234(&A)", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

#endif // TEST_H
