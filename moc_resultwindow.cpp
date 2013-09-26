/****************************************************************************
** Meta object code from reading C++ file 'resultwindow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "resultwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'resultwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ResultWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   14,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      48,   40,   13,   13, 0x0a,
      71,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ResultWindow[] = {
    "ResultWindow\0\0file\0fileChanged(QString)\0"
    "changed\0sampleChanged(QString)\0"
    "updateFileNames()\0"
};

void ResultWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ResultWindow *_t = static_cast<ResultWindow *>(_o);
        switch (_id) {
        case 0: _t->fileChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->sampleChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->updateFileNames(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ResultWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ResultWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ResultWindow,
      qt_meta_data_ResultWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ResultWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ResultWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ResultWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ResultWindow))
        return static_cast<void*>(const_cast< ResultWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int ResultWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void ResultWindow::fileChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
