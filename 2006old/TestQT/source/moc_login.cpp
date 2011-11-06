/****************************************************************************
** Meta object code from reading C++ file 'login.h'
**
** Created: Ò»  9ÔÂ 25 21:42:50 2006
**      by: The Qt Meta Object Compiler version 59 (Qt 4.1.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "login.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'login.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.1.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_LoginDialog[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      15,   13,   12,   12, 0x05,
      49,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      66,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_LoginDialog[] = {
    "LoginDialog\0\0,\0sendEnterMessage(QString,QString)\0showChatWindow()\0"
    "enterSlot()\0"
};

const QMetaObject LoginDialog::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_LoginDialog,
      qt_meta_data_LoginDialog, 0 }
};

const QMetaObject *LoginDialog::metaObject() const
{
    return &staticMetaObject;
}

void *LoginDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LoginDialog))
	return static_cast<void*>(const_cast<LoginDialog*>(this));
    if (!strcmp(_clname, "Ui2::Form"))
	return static_cast<Ui2::Form*>(const_cast<LoginDialog*>(this));
    return QWidget::qt_metacast(_clname);
}

int LoginDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sendEnterMessage((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: showChatWindow(); break;
        case 2: enterSlot(); break;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void LoginDialog::sendEnterMessage(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void LoginDialog::showChatWindow()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
