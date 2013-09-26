/****************************************************************************
** Meta object code from reading C++ file 'searchwidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "popupviews/searchwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'searchwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SearchWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   14,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      42,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SearchWidget[] = {
    "SearchWidget\0\0,,\0lookUp(QString,int,bool)\0"
    "querySet()\0"
};

void SearchWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SearchWidget *_t = static_cast<SearchWidget *>(_o);
        switch (_id) {
        case 0: _t->lookUp((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 1: _t->querySet(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SearchWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SearchWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SearchWidget,
      qt_meta_data_SearchWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SearchWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SearchWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SearchWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SearchWidget))
        return static_cast<void*>(const_cast< SearchWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int SearchWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void SearchWidget::lookUp(QString _t1, int _t2, bool _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
