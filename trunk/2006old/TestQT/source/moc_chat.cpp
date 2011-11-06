/****************************************************************************
** Meta object code from reading C++ file 'chat.h'
**
** Created: Ò»  9ÔÂ 25 22:40:33 2006
**      by: The Qt Meta Object Compiler version 59 (Qt 4.1.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "chat.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chat.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.1.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_Chat[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
       6,    5,    5,    5, 0x0a,
      26,   24,    5,    5, 0x0a,
      53,    5,    5,    5, 0x0a,
      77,    5,    5,    5, 0x0a,
      97,    5,    5,    5, 0x0a,
     115,    5,    5,    5, 0x0a,
     135,    5,    5,    5, 0x0a,
     149,    5,    5,    5, 0x0a,
     159,    5,    5,    5, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Chat[] = {
    "Chat\0\0showAndHideSlot()\0,\0enterSlot(QString,QString)\0"
    "changeButtonStateSlot()\0appendMessageSlot()\0createAboutSlot()\0"
    "newConnectionSlot()\0newDataSlot()\0addSlot()\0deleNameSlot()\0"
};

const QMetaObject Chat::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Chat,
      qt_meta_data_Chat, 0 }
};

const QMetaObject *Chat::metaObject() const
{
    return &staticMetaObject;
}

void *Chat::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Chat))
	return static_cast<void*>(const_cast<Chat*>(this));
    if (!strcmp(_clname, "Ui1::Form"))
	return static_cast<Ui1::Form*>(const_cast<Chat*>(this));
    return QWidget::qt_metacast(_clname);
}

int Chat::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: showAndHideSlot(); break;
        case 1: enterSlot((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: changeButtonStateSlot(); break;
        case 3: appendMessageSlot(); break;
        case 4: createAboutSlot(); break;
        case 5: newConnectionSlot(); break;
        case 6: newDataSlot(); break;
        case 7: addSlot(); break;
        case 8: deleNameSlot(); break;
        }
        _id -= 9;
    }
    return _id;
}
