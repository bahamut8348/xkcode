/****************************************************************************
** Meta object code from reading C++ file 'test.h'
**
** Created: Fri May 18 17:55:03 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "test.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'test.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_Ui_MainWindow[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Ui_MainWindow[] = {
    "Ui_MainWindow\0\0test()\0"
};

const QMetaObject Ui_MainWindow::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Ui_MainWindow,
      qt_meta_data_Ui_MainWindow, 0 }
};

const QMetaObject *Ui_MainWindow::metaObject() const
{
    return &staticMetaObject;
}

void *Ui_MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Ui_MainWindow))
	return static_cast<void*>(const_cast< Ui_MainWindow*>(this));
    return QObject::qt_metacast(_clname);
}

int Ui_MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: test(); break;
        }
        _id -= 1;
    }
    return _id;
}
